/**
 * \file rq_xml_parser.h
 * \author Brett Hutley
 *
 * \brief An XML Parser for Risk Quantify
 */
/*
** rq_xml_parser.h
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
#ifndef rq_xml_parser_h
#define rq_xml_parser_h

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

#include "rq_stream.h"

enum rq_xml_parser_parse_event_type {
    RQ_XML_PARSER_PARSE_EVENT_TYPE_ENTERING_ELEMENT,
    RQ_XML_PARSER_PARSE_EVENT_TYPE_LEAVING_ELEMENT,
    RQ_XML_PARSER_PARSE_EVENT_TYPE_GOT_ATTRIBUTE_VALUE,
    RQ_XML_PARSER_PARSE_EVENT_TYPE_GOT_ELEMENT_VALUE
};

#define RQ_XML_PARSER_PARSE_BUFFER_INITIAL_SIZE 1024
#define RQ_XML_PARSER_PARSE_BUFFER_GROW_SIZE 1024
#define RQ_XML_PARSER_TOKEN_BUFFER_INITIAL_SIZE 1024
#define RQ_XML_PARSER_TOKEN_BUFFER_GROW_SIZE 256

enum rq_xml_parser_states {
    RQ_XML_PARSER_STATE_WANT_TOPLEVEL_TOKEN,
    RQ_XML_PARSER_STATE_GOT_LESSTHAN,
    RQ_XML_PARSER_STATE_GOT_COMMENT1,
    RQ_XML_PARSER_STATE_GOT_DOCQM,
    RQ_XML_PARSER_STATE_IN_ELEMENT_NAME,
    RQ_XML_PARSER_STATE_GOT_COMMENT2,
    RQ_XML_PARSER_STATE_PARSE_ERROR,
    RQ_XML_PARSER_STATE_GOT_COMMENT3,
    RQ_XML_PARSER_STATE_READING_COMMENT,
    RQ_XML_PARSER_STATE_COMMENT_END_DASH1,
    RQ_XML_PARSER_STATE_COMMENT_END_DASH2,
    RQ_XML_PARSER_STATE_IN_ELEMENT,
    RQ_XML_PARSER_STATE_IN_ATTRIBUTE_NAME,
    RQ_XML_PARSER_STATE_AFTER_ATTRIBUTE_NAME,
    RQ_XML_PARSER_STATE_BEFORE_ATTRIBUTE_VALUE,
    RQ_XML_PARSER_STATE_IN_ATTRIBUTE_VALUE,
    RQ_XML_PARSER_STATE_IN_CLOSING_ELEMENT_NAME,
    RQ_XML_PARSER_STATE_IN_ELEMENT_VALUE,
    RQ_XML_PARSER_STATE_START_SHORTCUT_CLOSE
};

enum rq_xml_parser_token_type {
    RQ_XML_PARSER_TOKEN_TYPE_ELEMENT_NAME,
    RQ_XML_PARSER_TOKEN_TYPE_ELEMENT_VALUE,
    RQ_XML_PARSER_TOKEN_TYPE_ATTRIBUTE_NAME,
    RQ_XML_PARSER_TOKEN_TYPE_ATTRIBUTE_VALUE
};

struct rq_xml_parser_token {
    struct rq_xml_parser_token *next;
    struct rq_xml_parser_token *prev;

    enum rq_xml_parser_token_type token_type;
    char *token;

    struct rq_xml_parser_token *refers_to_element;
    struct rq_xml_parser_token *refers_to_attribute;
};

enum rq_xml_parser_event_type {
    RQ_XML_PARSER_EVENT_TYPE_GOT_ELEMENT_NAME,
    RQ_XML_PARSER_EVENT_TYPE_GOT_ELEMENT_VALUE,
    RQ_XML_PARSER_EVENT_TYPE_GOT_ATTRIBUTE_NAME,
    RQ_XML_PARSER_EVENT_TYPE_GOT_ATTRIBUTE_VALUE,
    RQ_XML_PARSER_EVENT_TYPE_ELEMENT_FINISHED
};

/** the handle to the xml parser */
typedef struct rq_xml_parser {
    char *parse_buffer; /**< The buffer to read the stream into */
    unsigned parse_buffer_len; /**< The current length of the buffer */
    unsigned parse_max_position; /**< The maximum char offset in the parse buffer */
    unsigned parse_position; /**< The current parse offset char */

    char *token_buffer; /**< The buffer to read the current token into */
    unsigned token_buffer_len; /**< The current token buffer length */
    unsigned token_position; /**< the current token offset char */

    int token_read_state; /**< 0 if we haven't got a token, 1 if we have, 2 if we are in the process of reading the token */

    char waiting_for_char;

    int state;
    int prev_state;

    struct rq_xml_parser_token *head_token;
    struct rq_xml_parser_token *curr_token;
    struct rq_xml_parser_token *last_element_token;

    enum rq_xml_parser_event_type event_type;

    void *callback_data;
    void (*callback)(void *, enum rq_xml_parser_parse_event_type, const char *, const char *, const char *);
} *rq_xml_parser_t;

/** Test whether the rq_asset is NULL */
RQ_EXPORT int rq_xml_parser_is_null(rq_xml_parser_t obj);

/**
 * Allocate an xml parser object
 */
RQ_EXPORT rq_xml_parser_t rq_xml_parser_alloc();

/**
 * Free the allocated xml parser
 */
RQ_EXPORT void rq_xml_parser_free(rq_xml_parser_t parser);

/** Initialize the parser.
 */
RQ_EXPORT void rq_xml_parser_init(struct rq_xml_parser *parser);

/**
 * Perform the parse
 *
 * @return RQ_OK if success. Otherwise an error code
 */
RQ_EXPORT int rq_xml_parser_parse(rq_xml_parser_t parser, rq_stream_t stream);

/**
 * Sets the callback data that will be handed to the callback.
 */
RQ_EXPORT void rq_xml_parser_set_callback_data(rq_xml_parser_t parser, void *callback_data);

/**
 * Sets a callback handler to be called whenever a new element is 
 * entered, attributes are acquired, or an element is left.
 * The parameters passed to the callback are; the callback data,
 * the event type, and the data acquired. 
 */
RQ_EXPORT void 
rq_xml_parser_set_callback(
    rq_xml_parser_t parser, 
    void (*callback)(void *, enum rq_xml_parser_parse_event_type, const char *, const char *, const char *)
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
