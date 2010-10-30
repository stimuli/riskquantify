/*
** rq_interpreter.c
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
/* -- defines ----------------------------------------------------- */
#include "rq_interpreter.h"
#include "rq_symbol_table.h"
#include "rq_double_linked_list.h"
#include "rq_error.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <ctype.h>

/* -- defines ----------------------------------------------------- */
#define OBJECT_POOL_SIZE 512

/* parser defines */
#define RQ_INTERP_PARSER_PARSE_BUFFER_INITIAL_SIZE 1024
#define RQ_INTERP_PARSER_PARSE_BUFFER_GROW_SIZE 1024
#define RQ_INTERP_PARSER_TOKEN_BUFFER_INITIAL_SIZE 1024
#define RQ_INTERP_PARSER_TOKEN_BUFFER_GROW_SIZE 256

/* parser states */
#define RQ_INTERP_PARSER_STATE_PARSE_ERROR -1
#define RQ_INTERP_PARSER_STATE_WANT_FORM 0

/* token types */
#define RQ_INTERP_TOKEN_TYPE_EOS 0
#define RQ_INTERP_TOKEN_TYPE_INTEGER 1
#define RQ_INTERP_TOKEN_TYPE_DOUBLE 2
#define RQ_INTERP_TOKEN_TYPE_STRING 3
#define RQ_INTERP_TOKEN_TYPE_DATE 4
#define RQ_INTERP_TOKEN_TYPE_IDENTIFIER 5


/* -- prototypes -------------------------------------------------- */
static int rq_interpreter_getc(struct rq_interpreter *i, rq_stream_t stream);
static void rq_interpreter_ungetc(struct rq_interpreter *i);
static void add_char_to_token_buffer(struct rq_interpreter *i, char ch);
static int rq_interpreter_gettoken(struct rq_interpreter *i, rq_stream_t stream);
static rq_object_t rq_interpreter_parse_form(struct rq_interpreter *i, rq_stream_t stream);
static rq_object_t rq_interpreter_parse_quote(struct rq_interpreter *i, rq_stream_t stream);
static rq_object_t rq_interpreter_parse_object(struct rq_interpreter *i, rq_stream_t stream, int havetoken);


/* -- interpreter code -------------------------------------------- */
RQ_EXPORT int 
rq_interpreter_is_null(rq_interpreter_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_interpreter_t 
rq_interpreter_alloc()
{
    struct rq_interpreter *i = (struct rq_interpreter *)RQ_MALLOC(sizeof(struct rq_interpreter));

    i->object_mgr = rq_object_mgr_alloc();
    i->error_list = rq_double_linked_list_alloc(_rq_interpreter_error_free);
    i->error_severity = RQ_ERROR_SEVERITY_NO_ERROR;

    i->parse_buffer = (char *)RQ_MALLOC(RQ_INTERP_PARSER_PARSE_BUFFER_INITIAL_SIZE);
    i->parse_buffer_len = RQ_INTERP_PARSER_PARSE_BUFFER_INITIAL_SIZE;
    i->parse_max_position = 0;
    i->parse_position = 0;

    i->token_buffer = 
        (char *)RQ_MALLOC(RQ_INTERP_PARSER_TOKEN_BUFFER_INITIAL_SIZE);
    i->token_buffer_len = RQ_INTERP_PARSER_TOKEN_BUFFER_INITIAL_SIZE;

    i->token_position = 0;
    i->parse_state = RQ_INTERP_PARSER_STATE_WANT_FORM;

    return i;
}


RQ_EXPORT void 
rq_interpreter_free(rq_interpreter_t i)
{
    rq_object_mgr_free(i->object_mgr);
    rq_double_linked_list_free(i->error_list);

    if (i->parse_buffer)
        RQ_FREE(i->parse_buffer);

    if (i->token_buffer)
        RQ_FREE(i->token_buffer);

    RQ_FREE(i);
}

static rq_object_t 
rq_interpreter_eval_func(rq_interpreter_t interp, rq_object_t p, rq_object_t args)
{
	rq_object_t func_id = rq_object_get_cons_car(p);

	if (rq_object_get_object_type(func_id) == RQ_OBJECT_TYPE_IDENTIFIER && 
        !strcmp(rq_object_get_identifier(func_id), "lambda")) 
    {
		rq_object_t args_expected = rq_object_get_cons_car(rq_object_get_cons_cdr(p));
        rq_object_t ret = rq_object_nil;
        int num_args_expected = rq_object_list_size(args_expected);
        int num_args_passed = rq_object_list_size(args);
        rq_object_t obj_eval;

        /* create a new variable block */
        rq_interpreter_variable_block_enter(interp);

		if (num_args_passed != num_args_expected) 
        {
            rq_interpreter_signal_error(
                interp,
                RQ_ERROR_SEVERITY_CRITICAL,
                "wrong number of parameters, expected %d, got %d", num_args_expected, num_args_passed
                );
			return rq_object_nil;
		}

        /*
         * Set the value of the function input variables in the
         * local variable block.
         */
        while (!rq_object_is_nil(args_expected) && !rq_object_is_nil(args))
        {
            rq_object_t arg_expected = rq_object_get_cons_car(args_expected);
            rq_interpreter_set_local(
                interp,
                rq_object_get_identifier(arg_expected),
                rq_interpreter_eval(interp, rq_object_get_cons_car(args))
                );
            args_expected = rq_object_get_cons_cdr(args_expected);
            args = rq_object_get_cons_cdr(args);
        }
		
        obj_eval = rq_object_get_cons_cdr(rq_object_get_cons_cdr(p));
        while (!rq_object_is_nil(obj_eval))
        {
            ret = rq_interpreter_eval(interp, rq_object_get_cons_car(obj_eval));
            obj_eval = rq_object_get_cons_cdr(obj_eval);
        }

        rq_interpreter_variable_block_leave(interp);

		return ret;
	}

	return rq_object_nil;
}

RQ_EXPORT rq_object_t 
rq_interpreter_eval(rq_interpreter_t i, rq_object_t obj)
{
    enum rq_object_type object_type = rq_object_get_object_type(obj);
    rq_object_t ret = rq_object_nil;

    if (i->error_severity != RQ_ERROR_SEVERITY_NO_ERROR)
        return rq_object_nil;

    if (object_type == RQ_OBJECT_TYPE_CONS)
    {
        rq_object_t car = rq_object_get_cons_car(obj);
        enum rq_object_type car_object_type = rq_object_get_object_type(car);
        if (car_object_type == RQ_OBJECT_TYPE_NIL)
            return rq_object_nil;
        if (car_object_type == RQ_OBJECT_TYPE_IDENTIFIER)
        {
            const char *car_id = rq_object_get_identifier(car);
            rq_object_t args = rq_object_get_cons_cdr(obj);
            rq_object_t func;


/* --
            if (!strcmp(car_id, "lambda"))
            {
                return obj;
            }
-- */
            func = rq_object_mgr_symbol_find(
                i->object_mgr, 
                car_id
                );

            if (rq_object_get_object_type(func) == RQ_OBJECT_TYPE_BUILTIN)
                ret = rq_object_call_builtin(func, i, args);
            else if (!rq_object_is_nil(func))
                ret = rq_interpreter_eval_func(i, func, args);
            else
			{
                rq_interpreter_signal_error(i, RQ_ERROR_SEVERITY_CRITICAL, "function '%s' is not defined", car_id);
			}

        }
    }
    else if (object_type == RQ_OBJECT_TYPE_IDENTIFIER)
    {
        ret = rq_object_mgr_symbol_find(
            i->object_mgr, 
            rq_object_get_identifier(obj)
            );
    }
    else
    {
        ret = obj;
    }

    return ret;
}

RQ_EXPORT rq_object_mgr_t
rq_interpreter_get_object_mgr(rq_interpreter_t i)
{
    return i->object_mgr;
}

RQ_EXPORT rq_object_t
rq_interpreter_new_object(rq_interpreter_t i)
{
    return rq_object_alloc(i->object_mgr);
}

RQ_EXPORT rq_object_t 
rq_interpreter_symbol_find(rq_interpreter_t i, const char *id)
{
    return rq_object_mgr_symbol_find(i->object_mgr, id);
}

RQ_EXPORT void 
rq_interpreter_symbol_set(rq_interpreter_t i, const char *id, rq_object_t value)
{
    rq_object_t obj = rq_object_mgr_symbol_find(i->object_mgr, id);
    if (obj == rq_object_nil)
        rq_object_mgr_symbol_add(i->object_mgr, id, value);
    else
        rq_object_set_object(i->object_mgr, obj, value);
}

RQ_EXPORT void 
rq_interpreter_signal_error(
    rq_interpreter_t interpreter, 
    enum rq_error_severity severity, 
    const char *format, 
    ...
    )
{
    rq_interpreter_error_t interpreter_error;
    va_list l;
    char buf[4096];

    va_start(l, format);

    vsprintf(buf, format, l);

    interpreter_error = rq_interpreter_error_alloc(buf);
    rq_double_linked_list_append(interpreter->error_list, interpreter_error);

    interpreter->error_severity = severity;

    va_end(l);
}

static int
rq_interpreter_getc(struct rq_interpreter *i, rq_stream_t stream)
{
    /* Do we need more data from the stream? */
    if (i->parse_position == i->parse_max_position)
    {
        int num_bytes_to_read;
        int num_bytes_read;

        if (rq_stream_at_end(stream))
            return 0; /* We're finished! */

        /* do we need to grow the parse buffer? */
        if (i->parse_max_position == i->parse_buffer_len)
        {
            unsigned new_buffer_size = 
                i->parse_buffer_len + 
                RQ_INTERP_PARSER_PARSE_BUFFER_GROW_SIZE;
            i->parse_buffer = (char *)realloc(i->parse_buffer, new_buffer_size);
            i->parse_buffer_len = new_buffer_size;
        }

        num_bytes_to_read = i->parse_buffer_len - i->parse_max_position;
        num_bytes_read = rq_stream_read(stream, i->parse_buffer + i->parse_max_position, num_bytes_to_read);
        if (num_bytes_read > 0)
        {
            i->parse_max_position += num_bytes_read;
        }
    }

    return i->parse_buffer[i->parse_position++];
}

static void
rq_interpreter_ungetc(struct rq_interpreter *i)
{
    assert(i->parse_position > 0);
    i->parse_position--;
}

static void
add_char_to_token_buffer(struct rq_interpreter *i, char ch)
{
    if (i->token_position == i->token_buffer_len)
    {
        unsigned new_token_buffer_len = 
            i->token_buffer_len + RQ_INTERP_PARSER_TOKEN_BUFFER_GROW_SIZE;
        i->token_buffer = (char *)realloc(
            i->token_buffer, 
            new_token_buffer_len
            );
        i->token_buffer_len = new_token_buffer_len;
    }

    i->token_buffer[i->token_position] = ch;
    i->token_position++;
}

static int
rq_interpreter_gettoken(struct rq_interpreter *i, rq_stream_t stream)
{
    i->token_position = 0;
	while (1) 
    {
		int c = rq_interpreter_getc(i, stream);
		switch (c) 
        {
            case '\n':
                i->lineno++;
                break;

            case ' ': 
            case '\f': 
            case '\t': 
            case '\v': 
            case '\r':
                break;

            case ';':
                /* Comment: ";".*"\n" */
                while ((c = rq_interpreter_getc(i, stream)) != '\n')
                    ;
                i->lineno++;
                break;

            case '?':
                /* Character: "?". */
                c = rq_interpreter_getc(i, stream);
                sprintf(i->token_buffer, "%d", c);
                return RQ_INTERP_TOKEN_TYPE_INTEGER;

            case '-':
                /* Minus sign: "-". */
                c = rq_interpreter_getc(i, stream);
                if (!isdigit(c)) 
                {
                    rq_interpreter_ungetc(i);
                    c = '-';
                    goto got_id;
                }
                rq_interpreter_ungetc(i);
                c = '-';
                /* FALLTRHU */

            case '0':
            case '1': case '2': case '3':
            case '4': case '5': case '6':
            case '7': case '8': case '9':
            {
                int token_type = RQ_INTERP_TOKEN_TYPE_INTEGER;
                /* Integer: [0-9]+ */
                do 
                {
                    add_char_to_token_buffer(i, (char)c);
                    c = rq_interpreter_getc(i, stream);
                    if (c == '.')
                        token_type = RQ_INTERP_TOKEN_TYPE_DOUBLE;
                    else if (c == '-')
                        token_type = RQ_INTERP_TOKEN_TYPE_DATE;
                } 
                while (isdigit(c) || c == '.' || c == '-');
                rq_interpreter_ungetc(i);
                add_char_to_token_buffer(i, '\0');
                return token_type;
            }

          got_id:
            case '_': case '+': case '*': case '/': case '%':
            case '<': case '>': case '=': case '&':
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
            case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
            case 's': case 't': case 'u': case 'v': case 'w': case 'x':
            case 'y': case 'z':
            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
            case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
            case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
            case 'Y': case 'Z':
                /* Identifier: [-/+*%<>=&a-zA-Z_][-/+*%<>=&a-zA-Z_0-9]* */
                do 
                {
                    add_char_to_token_buffer(i, (char)c);
                    c = rq_interpreter_getc(i, stream);
                } 
                while (isalnum(c) || strchr("_-+*/%<>=&", c) != NULL);
                rq_interpreter_ungetc(i);
                add_char_to_token_buffer(i, '\0');
                return RQ_INTERP_TOKEN_TYPE_IDENTIFIER;

            case '"':
                /* String: "\""([^"]|"\\".)*"\"" */
                while ((c = rq_interpreter_getc(i, stream)) != '"' && c != 0) 
                {
                    if (c == '\\') 
                    {
                        c = rq_interpreter_getc(i, stream);
                        switch (c) 
                        {
                            case '\n': i->lineno++; break;
                            case 'a': add_char_to_token_buffer(i, '\a'); break;
                            case 'b': add_char_to_token_buffer(i, '\b'); break;
                            case 'f': add_char_to_token_buffer(i, '\f'); break;
                            case 'n': add_char_to_token_buffer(i, '\n'); break;
                            case 'r': add_char_to_token_buffer(i, '\r'); break;
                            case 't': add_char_to_token_buffer(i, '\t'); break;
                            case 'v': add_char_to_token_buffer(i, '\v'); break;
                            default: 
                                add_char_to_token_buffer(i, '\\'); 
                                add_char_to_token_buffer(i, (char)c);
                        }
                    } 
                    else 
                    {
                        if (c == '\n')
                            i->lineno++;
                        add_char_to_token_buffer(i, (char)c); 
                    }
                }
                add_char_to_token_buffer(i, '\0'); 
                return RQ_INTERP_TOKEN_TYPE_STRING;

            default:
                return c;
		}
	}
}

static rq_object_t
rq_interpreter_parse_form(struct rq_interpreter *interp, rq_stream_t stream)
{
	rq_object_t p;
    rq_object_t first = NULL;
    rq_object_t prev = NULL;

	while ((interp->token_type = rq_interpreter_gettoken(interp, stream)) != ')' && 
           interp->token_type != RQ_INTERP_TOKEN_TYPE_EOS) 
    {
		/*
		 * Parse a dotted pair notation.
		 */
		if (interp->token_type == '.') 
        {
			interp->token_type = rq_interpreter_gettoken(interp, stream);
			if (!prev)
			{
				rq_interpreter_signal_error(
                    interp,
                    RQ_ERROR_SEVERITY_CRITICAL,
                    "Error parsing form! unexpected '.'"
                    );
                return rq_object_nil;
			}

			rq_object_set_cons_cdr(prev, rq_interpreter_parse_object(interp, stream, 1));

			if ((interp->token_type = rq_interpreter_gettoken(interp, stream)) != ')')
			{
				rq_interpreter_signal_error(
                    interp,
                    RQ_ERROR_SEVERITY_CRITICAL,
                    "Error parsing form! expected ')'"
                    );
                return rq_object_nil;
			}
			break;
		}

		p = rq_object_alloc_cons(interp->object_mgr, rq_object_nil, rq_object_nil);

		if (!first)
			first = p;
		if (prev)
			rq_object_set_cons_cdr(prev, p);

		rq_object_set_cons_car(p, rq_interpreter_parse_object(interp, stream, 1));

		prev = p;
	}

	if (!first)
		return rq_object_nil;
	else
		return first;
}

static rq_object_t
rq_interpreter_parse_quote(struct rq_interpreter *i, rq_stream_t stream)
{
	rq_object_t p = rq_object_alloc(i->object_mgr);
    rq_object_t id = rq_object_alloc(i->object_mgr);
    rq_object_t q = rq_object_alloc(i->object_mgr);

    rq_object_set_identifier(id, "quote");
    rq_object_set_cons(q, rq_interpreter_parse_object(i, stream, 0), rq_object_nil);
    rq_object_set_cons(p, id, q);

	return p;
}

static rq_object_t
rq_interpreter_parse_object(struct rq_interpreter *i, rq_stream_t stream, int havetoken)
{
    rq_object_t p = rq_object_nil;

	if (!havetoken)
		i->token_type = rq_interpreter_gettoken(i, stream);

	switch (i->token_type)
    {
        case '(':
            p = rq_interpreter_parse_form(i, stream);
            break;

        case '\'':
            p = rq_interpreter_parse_quote(i, stream);
            break;

        case RQ_INTERP_TOKEN_TYPE_IDENTIFIER:
            if (!strcmp(i->token_buffer, "t"))
                p = rq_object_true;
            else if (!strcmp(i->token_buffer, "nil"))
                p = rq_object_nil;
            else 
            {
                p = rq_object_alloc_identifier(i->object_mgr, i->token_buffer);
            }
            break;

        case RQ_INTERP_TOKEN_TYPE_INTEGER:
            p = rq_object_alloc_integer(i->object_mgr, atoi(i->token_buffer));
            break;

        case RQ_INTERP_TOKEN_TYPE_DOUBLE:
            p = rq_object_alloc_double(i->object_mgr, atof(i->token_buffer));
            break;

        case RQ_INTERP_TOKEN_TYPE_DATE:
            p = rq_object_alloc_date(i->object_mgr, rq_date_parse(i->token_buffer, RQ_DATE_FORMAT_YMD));
            break;

        case RQ_INTERP_TOKEN_TYPE_STRING:
            p = rq_object_alloc_string(i->object_mgr, i->token_buffer);
            break;

        case 0:
            break;

        default:
        {
            rq_interpreter_signal_error(
                i, 
                RQ_ERROR_SEVERITY_CRITICAL, 
                "%d: unexpected character `%c'", 
                i->lineno, 
                i->token_type
                );
        }
        break;
	}

	return p;
}

RQ_EXPORT rq_object_t
rq_interpreter_parse(rq_interpreter_t i, rq_stream_t stream)
{
    rq_object_t objs = rq_object_nil;
    rq_object_t obj;

    if (!rq_stream_is_open(stream))
    {
        int err = rq_stream_open(stream);
        if (err != RQ_OK)
        {
            rq_interpreter_signal_error(
                i,
                RQ_ERROR_SEVERITY_CRITICAL,
                "Parser: Error opening stream"
                );
            return rq_object_nil;
        }
    }

    i->error_severity = RQ_ERROR_SEVERITY_NO_ERROR;
    i->lineno = 0;

    do
    {
        obj = rq_interpreter_parse_object(i, stream, 0);
        if (!rq_object_is_nil(obj))
        {
            if (rq_object_is_nil(objs))
                objs = rq_object_alloc_cons(i->object_mgr, obj, rq_object_nil);
            else
            {
                rq_object_t o = objs;
                while (!rq_object_is_nil(rq_object_get_cons_cdr(o)))
                    o = rq_object_get_cons_cdr(o);
                rq_object_set_cons_cdr(o, rq_object_alloc_cons(i->object_mgr, obj, rq_object_nil));
            }
        }
    }
    while (!rq_object_is_nil(obj));

    return objs;
}

RQ_EXPORT void
rq_interpreter_add_builtin(rq_interpreter_t i, const char *function_id, rq_object_t (*func)(void *, rq_object_t))
{
    rq_object_t builtin = rq_object_alloc_builtin(
        i->object_mgr, 
        func
        );
    rq_object_mgr_symbol_add(i->object_mgr, function_id, builtin);
}

RQ_EXPORT void 
rq_interpreter_garbage_collect(rq_interpreter_t i)
{
    rq_object_mgr_garbage_collect(i->object_mgr);
}

RQ_EXPORT void
rq_interpreter_variable_block_enter(rq_interpreter_t i)
{
    rq_object_mgr_symbol_table_push(i->object_mgr);
}

RQ_EXPORT void
rq_interpreter_variable_block_leave(rq_interpreter_t i)
{
    rq_object_mgr_symbol_table_pop(i->object_mgr);
}

RQ_EXPORT void
rq_interpreter_set_local(rq_interpreter_t i, const char *id, rq_object_t value)
{
    rq_object_mgr_symbol_set_local(i->object_mgr, id, value);
}
