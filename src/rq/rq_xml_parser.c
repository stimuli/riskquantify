/*
** rq_xml_parser.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2003-2008 Brett Hutley
**
** This file is part of the Risk Quantify Library
**
** Risk Quantify is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or (at your option) any later version.
**
** Risk Quantify is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
** You should have received a copy of the GNU Library General Public
** License along with Risk Quantify; if not, write to the Free
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include "rq_xml_parser.h"
#include "rq_error.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


RQ_EXPORT int 
rq_xml_parser_is_null(rq_xml_parser_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT void
rq_xml_parser_init(struct rq_xml_parser *parser)
{
    parser->parse_max_position = 0;
    parser->parse_position = 0;
    
    parser->token_position = 0;
    parser->token_read_state = 0;

    parser->state = RQ_XML_PARSER_STATE_WANT_TOPLEVEL_TOKEN;

    while (parser->head_token)
    {
        struct rq_xml_parser_token *t = parser->head_token;
        parser->head_token = t->next;

        RQ_FREE(t->token);
        RQ_FREE(t);
    }

    parser->head_token = NULL;
    parser->curr_token = NULL;

    parser->last_element_token = NULL;
}

RQ_EXPORT rq_xml_parser_t 
rq_xml_parser_alloc()
{
    struct rq_xml_parser *parser = (struct rq_xml_parser *)
        RQ_MALLOC(sizeof(struct rq_xml_parser));

    parser->parse_buffer = 
        (char *)RQ_MALLOC(RQ_XML_PARSER_PARSE_BUFFER_INITIAL_SIZE);
    parser->parse_buffer_len = RQ_XML_PARSER_PARSE_BUFFER_INITIAL_SIZE;
    parser->parse_max_position = 0;
    parser->parse_position = 0;

    parser->token_buffer = 
        (char *)RQ_MALLOC(RQ_XML_PARSER_TOKEN_BUFFER_INITIAL_SIZE);
    parser->token_buffer_len = RQ_XML_PARSER_TOKEN_BUFFER_INITIAL_SIZE;
    parser->token_position = 0;
    parser->token_read_state = 0;

    parser->state = RQ_XML_PARSER_STATE_WANT_TOPLEVEL_TOKEN;

    parser->head_token = NULL;
    parser->curr_token = NULL;
    parser->last_element_token = NULL;

    parser->callback_data = NULL;
    parser->callback = NULL;

    return parser;
}

void 
rq_xml_parser_free(rq_xml_parser_t p)
{
    if (p->parse_buffer)
        RQ_FREE(p->parse_buffer);

    if (p->token_buffer)
        RQ_FREE(p->token_buffer);

    while (p->head_token)
    {
        struct rq_xml_parser_token *t = p->head_token;
        p->head_token = t->next;

        RQ_FREE(t->token);
        RQ_FREE(t);
    }

    RQ_FREE(p);
}

static void
add_char_to_token_buffer(struct rq_xml_parser *p, char ch)
{
    if (p->token_position == p->token_buffer_len)
    {
        unsigned new_token_buffer_len = 
            p->token_buffer_len + RQ_XML_PARSER_TOKEN_BUFFER_GROW_SIZE;
        p->token_buffer = (char *)realloc(
            p->token_buffer, 
            new_token_buffer_len
            );
        p->token_buffer_len = new_token_buffer_len;
    }

    p->token_buffer[p->token_position] = ch;
    p->token_position++;
}

static void 
add_token(struct rq_xml_parser *p, enum rq_xml_parser_token_type token_type)
{
    struct rq_xml_parser_token *t = (struct rq_xml_parser_token *)
        calloc(1, sizeof(struct rq_xml_parser_token));

    t->token_type = token_type;
    t->token = RQ_STRDUP(p->token_buffer);

    if (!p->head_token)
        p->head_token = p->curr_token = t;
    else
    {
        t->prev = p->curr_token;
        p->curr_token->next = t;
        p->curr_token = t;
    }

    if (t->token_type != RQ_XML_PARSER_TOKEN_TYPE_ELEMENT_NAME && t->prev)
    {
        if (t->prev->token_type == RQ_XML_PARSER_TOKEN_TYPE_ELEMENT_NAME)
            t->refers_to_element = t->prev;
        else
            t->refers_to_element = t->prev->refers_to_element;
    }

    if (t->token_type == RQ_XML_PARSER_TOKEN_TYPE_ATTRIBUTE_VALUE && t->prev)
    {
        if (t->prev->token_type == RQ_XML_PARSER_TOKEN_TYPE_ATTRIBUTE_NAME)
            t->refers_to_attribute = t->prev;
        else
            t->refers_to_attribute = t->prev->refers_to_attribute;
    }
}

static void
pop_tokens_until_curr_element_gone(struct rq_xml_parser *p)
{
    while (p->curr_token)
    {
        struct rq_xml_parser_token *t = p->curr_token;
        int finished = (t->token_type == RQ_XML_PARSER_TOKEN_TYPE_ELEMENT_NAME && !strcmp(t->token, p->token_buffer));
        p->curr_token = t->prev;
        RQ_FREE(t->token);
        RQ_FREE(t);

        if (p->curr_token)
            p->curr_token->next = NULL;

        if (finished)
            break;
    }

    if (!p->curr_token)
        p->head_token = NULL;
}

static void
change_state(struct rq_xml_parser *p, enum rq_xml_parser_states state)
{
    p->prev_state = p->state;
    p->state = state;
}

RQ_EXPORT int
rq_xml_parser_parse(rq_xml_parser_t p, rq_stream_t stream)
{
    if (!rq_stream_is_open(stream))
    {
        int err = rq_stream_open(stream);
        if (err != RQ_OK)
            return err;
    }

    p->prev_state = RQ_XML_PARSER_STATE_WANT_TOPLEVEL_TOKEN;

    while (p->state != RQ_XML_PARSER_STATE_PARSE_ERROR)
    {
        /* Do we need more data from the stream? */
        if (p->parse_position == p->parse_max_position)
        {
            int num_bytes_to_read;
            int num_bytes_read;

            if (rq_stream_at_end(stream))
                break; /* We're finished! */

            /* do we need to grow the parse buffer? */
            if (p->parse_max_position == p->parse_buffer_len)
            {
                unsigned new_buffer_size = 
                    p->parse_buffer_len + 
                    RQ_XML_PARSER_PARSE_BUFFER_GROW_SIZE;
                p->parse_buffer = (char *)realloc(p->parse_buffer, new_buffer_size);
                p->parse_buffer_len = new_buffer_size;
            }

            num_bytes_to_read = p->parse_buffer_len - p->parse_max_position;
            num_bytes_read = rq_stream_read(stream, p->parse_buffer + p->parse_max_position, num_bytes_to_read);
            if (num_bytes_read > 0)
            {
                p->parse_max_position += num_bytes_read;
            }
        }

        if (p->parse_position < p->parse_max_position)
        {
            /* 
               Eat characters in the parse_buffer until we get a
               token, read to the end of the parse buffer, or get an
               error 
            */
            while (p->token_read_state != 1 && p->parse_position < p->parse_max_position && p->state != RQ_XML_PARSER_STATE_PARSE_ERROR)
            {
                char ch = p->parse_buffer[p->parse_position];

                switch (p->state)
                {
                    case RQ_XML_PARSER_STATE_WANT_TOPLEVEL_TOKEN:
                        /* looking for element, PI, comment, doctype */
                        /* eat any white space */
                        if (ch == '<')
                        {
                            change_state(p, RQ_XML_PARSER_STATE_GOT_LESSTHAN);
                        }
                        break;

                    case RQ_XML_PARSER_STATE_GOT_LESSTHAN:
                        if (ch == '!')
                            change_state(p, RQ_XML_PARSER_STATE_GOT_COMMENT1);
                        /* else if (ch == '?')
                              p->state = RQ_XML_PARSER_STATE_GOT_DOCQM;
                        */
                        else if ((ch >= 'A' && ch <= 'Z') || 
                                 (ch >= 'a' && ch <= 'z'))
                        {
                            add_char_to_token_buffer(p, ch);
                            change_state(p, RQ_XML_PARSER_STATE_IN_ELEMENT_NAME);
                            p->token_read_state = 2;
                        }
                        else if (ch == '/')
                        {
                            change_state(p, RQ_XML_PARSER_STATE_IN_CLOSING_ELEMENT_NAME);
                            p->token_read_state = 2;
                        }
                        break;

                    case RQ_XML_PARSER_STATE_GOT_COMMENT1:
                        if (ch == '-')
                            change_state(p, RQ_XML_PARSER_STATE_GOT_COMMENT2);
                        else
                            change_state(p, RQ_XML_PARSER_STATE_PARSE_ERROR);
                        break;

                    case RQ_XML_PARSER_STATE_GOT_COMMENT2:
                        if (ch == '-')
                            change_state(p, RQ_XML_PARSER_STATE_READING_COMMENT);
                        else
                            change_state(p, RQ_XML_PARSER_STATE_PARSE_ERROR);
                        break;

                    case RQ_XML_PARSER_STATE_READING_COMMENT:
                        if (ch == '-')
                            change_state(p, RQ_XML_PARSER_STATE_COMMENT_END_DASH1);
                        break;

                    case RQ_XML_PARSER_STATE_COMMENT_END_DASH1:
                        if (ch == '-')
                            change_state(p, RQ_XML_PARSER_STATE_COMMENT_END_DASH2);
                        else
                            change_state(p, RQ_XML_PARSER_STATE_READING_COMMENT);

                        break;

                    case RQ_XML_PARSER_STATE_COMMENT_END_DASH2:
                        if (ch == '>')
                            change_state(p, RQ_XML_PARSER_STATE_WANT_TOPLEVEL_TOKEN);
                        else
                            change_state(p, RQ_XML_PARSER_STATE_READING_COMMENT);

                        break;

                    case RQ_XML_PARSER_STATE_IN_ELEMENT_NAME:
                        if ((ch >= 'A' && ch <= 'Z') || 
                            (ch >= 'a' && ch <= 'z') ||
                            (ch >= '0' && ch <= '9') ||
                            ch == '_' ||
                            ch == '-')
                        {
                            add_char_to_token_buffer(p, ch);
                        }
                        else if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
                        {
                            change_state(p, RQ_XML_PARSER_STATE_IN_ELEMENT);
                            p->token_read_state = 1;
                            add_char_to_token_buffer(p, '\0');
                            add_token(p, RQ_XML_PARSER_TOKEN_TYPE_ELEMENT_NAME);
                            p->event_type = RQ_XML_PARSER_EVENT_TYPE_GOT_ELEMENT_NAME;
                        }
                        else if (ch == '>')
                        {
                            change_state(p, RQ_XML_PARSER_STATE_IN_ELEMENT_VALUE);
                            p->token_read_state = 1;
                            add_char_to_token_buffer(p, '\0');
                            add_token(p, RQ_XML_PARSER_TOKEN_TYPE_ELEMENT_NAME);
                            p->event_type = RQ_XML_PARSER_EVENT_TYPE_GOT_ELEMENT_NAME;
                        }
                        else if (ch == '/')
                        {
                            p->token_read_state = 1;
                            add_char_to_token_buffer(p, '\0');
                            add_token(p, RQ_XML_PARSER_TOKEN_TYPE_ELEMENT_NAME);
                            change_state(p, RQ_XML_PARSER_STATE_START_SHORTCUT_CLOSE);
                            p->event_type = RQ_XML_PARSER_EVENT_TYPE_GOT_ELEMENT_NAME;
                        }
                        break;

                    case RQ_XML_PARSER_STATE_IN_ELEMENT:
                        if ((ch >= 'A' && ch <= 'Z') || 
                            (ch >= 'a' && ch <= 'z'))
                        {
                            add_char_to_token_buffer(p, ch);
                            p->token_read_state = 2;
                            change_state(p, RQ_XML_PARSER_STATE_IN_ATTRIBUTE_NAME);
                        }
                        else if (ch == '>')
                        {
                            change_state(p, RQ_XML_PARSER_STATE_IN_ELEMENT_VALUE);
                        }
                        else if (ch == '/')
                        {
                            add_char_to_token_buffer(p, '\0');
                            change_state(p, RQ_XML_PARSER_STATE_START_SHORTCUT_CLOSE);
                        }
                        break;

                    case RQ_XML_PARSER_STATE_IN_ATTRIBUTE_NAME:
                        if ((ch >= 'A' && ch <= 'Z') || 
                            (ch >= 'a' && ch <= 'z') ||
                            (ch >= '0' && ch <= '9') ||
                            ch == '_' ||
                            ch == '-')
                        {
                            add_char_to_token_buffer(p, ch);
                        }
                        else if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
                        {
                            change_state(p, RQ_XML_PARSER_STATE_AFTER_ATTRIBUTE_NAME);
                            p->token_read_state = 1;
                            add_char_to_token_buffer(p, '\0');
                            add_token(p, RQ_XML_PARSER_TOKEN_TYPE_ATTRIBUTE_NAME);
                            p->event_type = RQ_XML_PARSER_EVENT_TYPE_GOT_ATTRIBUTE_NAME;
                        }
                        else if (ch == '=')
                        {
                            change_state(p, RQ_XML_PARSER_STATE_BEFORE_ATTRIBUTE_VALUE);
                            p->token_read_state = 1;
                            add_char_to_token_buffer(p, '\0');
                            add_token(p, RQ_XML_PARSER_TOKEN_TYPE_ATTRIBUTE_NAME);
                            p->event_type = RQ_XML_PARSER_EVENT_TYPE_GOT_ATTRIBUTE_NAME;
                        }
                        break;

                    case RQ_XML_PARSER_STATE_AFTER_ATTRIBUTE_NAME:
                        if (ch == '=')
                            change_state(p, RQ_XML_PARSER_STATE_BEFORE_ATTRIBUTE_VALUE);
                        break;

                    case RQ_XML_PARSER_STATE_BEFORE_ATTRIBUTE_VALUE:
                        if (ch == '\'' || ch == '"')
                        {
                            p->waiting_for_char = ch;
                            /* add_char_to_token_buffer(p, ch); */
                            change_state(p, RQ_XML_PARSER_STATE_IN_ATTRIBUTE_VALUE);
                            p->token_read_state = 2;
                        }
                        break;

                    case RQ_XML_PARSER_STATE_IN_ATTRIBUTE_VALUE:
                        if (ch == p->waiting_for_char)
                        {
                            change_state(p, RQ_XML_PARSER_STATE_IN_ELEMENT);
                            p->token_read_state = 1;
                            add_char_to_token_buffer(p, '\0');
                            add_token(p, RQ_XML_PARSER_TOKEN_TYPE_ATTRIBUTE_VALUE);
                            p->event_type = RQ_XML_PARSER_EVENT_TYPE_GOT_ATTRIBUTE_VALUE;
                        }
                        else
                        {
                            add_char_to_token_buffer(p, ch);
                        }
                        break;

                    case RQ_XML_PARSER_STATE_IN_CLOSING_ELEMENT_NAME:
                        if ((ch >= 'A' && ch <= 'Z') || 
                            (ch >= 'a' && ch <= 'z') ||
                            (ch >= '0' && ch <= '9') ||
                            ch == '_' ||
                            ch == '-')
                        {
                            add_char_to_token_buffer(p, ch);
                        }
                        else if (ch == '>')
                        {
                            p->token_read_state = 1;
                            add_char_to_token_buffer(p, '\0');
                            change_state(p, RQ_XML_PARSER_STATE_WANT_TOPLEVEL_TOKEN);
                            pop_tokens_until_curr_element_gone(p);
                            p->event_type = RQ_XML_PARSER_EVENT_TYPE_ELEMENT_FINISHED;
                        }
                        break;

                    case RQ_XML_PARSER_STATE_IN_ELEMENT_VALUE:
                        if (ch == '<')
                        {
                            p->token_read_state = 1;
                            add_char_to_token_buffer(p, '\0');
                            change_state(p, RQ_XML_PARSER_STATE_GOT_LESSTHAN);
                            add_token(p, RQ_XML_PARSER_TOKEN_TYPE_ELEMENT_VALUE);
                            p->event_type = RQ_XML_PARSER_EVENT_TYPE_GOT_ELEMENT_VALUE;
                        }
                        else
                        {
                            p->token_read_state = 2;
                            add_char_to_token_buffer(p, ch);
                        }
                        break;

                    case RQ_XML_PARSER_STATE_START_SHORTCUT_CLOSE:
                        if (ch == '>')
                        {
                            int finished = 0;

                            p->token_read_state = 1;

                            while (p->curr_token)
                            {
                                struct rq_xml_parser_token *t = p->curr_token;
                                if (t->token_type == RQ_XML_PARSER_TOKEN_TYPE_ELEMENT_NAME)
                                {
                                    strcpy(p->token_buffer, t->token);
                                    finished = 1;
                                }

                                p->curr_token = t->prev;
                                RQ_FREE(t->token);
                                RQ_FREE(t);
                                if (finished)
                                    break;
                            }

                            if (!p->curr_token)
                                p->head_token = NULL;

                            change_state(p, RQ_XML_PARSER_STATE_WANT_TOPLEVEL_TOKEN);
                            p->event_type = RQ_XML_PARSER_EVENT_TYPE_ELEMENT_FINISHED;
                        }
                        else
                        {
                            p->state = p->prev_state;
                        }
                        break;

                    default:
                        assert(0);

                }

                p->parse_position++;
            }

            if (p->state != RQ_XML_PARSER_STATE_PARSE_ERROR)
            {
                /* Move the charaters still to be processed to the 
                   beginning of the parse_buffer. This is so we can
                   read more from the stream without having to grow
                   the buffer.
                */
                if (p->parse_position < p->parse_max_position)
                {
                    if (p->parse_position > 0)
                    {
                        int bytes_left_in_buffer = p->parse_max_position - p->parse_position;
                        memcpy(p->parse_buffer, p->parse_buffer + p->parse_position, bytes_left_in_buffer);
                        p->parse_position = 0;
                        p->parse_max_position = bytes_left_in_buffer;
                    }
                }
                else
                {
                    p->parse_position = 0;
                    p->parse_max_position = 0;
                }

                /* Have we got a token?
                 */
                if (p->token_read_state == 1)
                {
                    if (p->event_type != RQ_XML_PARSER_EVENT_TYPE_ELEMENT_FINISHED)
                    {
                        if (p->curr_token->token_type == RQ_XML_PARSER_TOKEN_TYPE_ELEMENT_NAME)
                        {
                            if (p->callback)
                                (*p->callback)(p->callback_data, RQ_XML_PARSER_PARSE_EVENT_TYPE_ENTERING_ELEMENT, p->curr_token->token, NULL, NULL);
                        }
                        else if (p->curr_token->token_type == RQ_XML_PARSER_TOKEN_TYPE_ELEMENT_VALUE)
                        {
                            if (p->callback)
                                (*p->callback)(p->callback_data, RQ_XML_PARSER_PARSE_EVENT_TYPE_GOT_ELEMENT_VALUE, p->curr_token->refers_to_element->token, p->curr_token->token, NULL);
                        }
                        else if (p->curr_token->token_type == RQ_XML_PARSER_TOKEN_TYPE_ATTRIBUTE_VALUE)
                        {
                            if (p->callback)
                                (*p->callback)(p->callback_data, RQ_XML_PARSER_PARSE_EVENT_TYPE_GOT_ATTRIBUTE_VALUE, p->curr_token->refers_to_element->token, p->curr_token->refers_to_attribute->token, p->curr_token->token);
                        }
                    }
                    else
                    {
                        (*p->callback)(p->callback_data, RQ_XML_PARSER_PARSE_EVENT_TYPE_LEAVING_ELEMENT, p->token_buffer, NULL, NULL);
                    }

                    p->token_read_state = 0;
                    p->token_position = 0;
                }
            }
        }
    }

    return RQ_OK;
}

RQ_EXPORT void 
rq_xml_parser_set_callback_data(rq_xml_parser_t p, void *callback_data)
{
    p->callback_data = callback_data;
}

RQ_EXPORT void 
rq_xml_parser_set_callback(
    rq_xml_parser_t p, 
    void (*callback)(void *, enum rq_xml_parser_parse_event_type, const char *, const char *, const char *)
    )
{
    p->callback = callback;
}
