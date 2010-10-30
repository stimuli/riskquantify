/*
** rq_dom_parser.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2008-2009 Brett Hutley
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "rq_dom_parser.h"
#include "rq_xml_parser.h"

struct rq_dom_parser {
    struct rq_xml_node *head_element;

    struct rq_xml_node *cur_element;
};

void 
dom_parser_cb(
    void *dom_parser_ptr, 
    enum rq_xml_parser_parse_event_type parse_event_type, 
    const char *p1, 
    const char *p2, 
    const char *p3)
{
    struct rq_dom_parser *dom_parser = (struct rq_dom_parser *)dom_parser_ptr;

    switch (parse_event_type)
    {
        case RQ_XML_PARSER_PARSE_EVENT_TYPE_ENTERING_ELEMENT:
        {
            struct rq_xml_node *n = rq_xml_node_alloc(RQ_XML_NODE_TYPE_ELEMENT);
            n->node.el.tag = strdup(p1);
            if (dom_parser->cur_element)
            {
                rq_xml_node_add_child(dom_parser->cur_element, n);
            }
            else if (dom_parser->head_element)
            {
                /* OK, what we have here is multiple nodes as the root of this
                   document, which is actually illegal but we'll support it
                   anyway.
                */
                rq_xml_node_add_sibling(dom_parser->head_element, n);
            }
            else
                dom_parser->head_element = n;

            dom_parser->cur_element = n;

            break;
        }

        case RQ_XML_PARSER_PARSE_EVENT_TYPE_LEAVING_ELEMENT:
            if (dom_parser->cur_element)
            {
                /* This should never actually be NULL, since we
                   can't really leave an element we never entered, right?
                */
                dom_parser->cur_element = dom_parser->cur_element->node.el.parent;
            }
            break;

        case RQ_XML_PARSER_PARSE_EVENT_TYPE_GOT_ATTRIBUTE_VALUE:
            if (dom_parser->cur_element)
            {
                /* This should never actually be NULL, since we
                   can't really have an attribute without a parent element.
                */
                rq_xml_node_add_attribute(dom_parser->cur_element, p2, p3);
            }
            break;

        case RQ_XML_PARSER_PARSE_EVENT_TYPE_GOT_ELEMENT_VALUE:
            if (dom_parser->cur_element)
                rq_xml_node_add_text(dom_parser->cur_element, p2);

            break;

        default:
            assert(0);
    }
}

RQ_EXPORT struct rq_xml_node *
rq_dom_parser_parse_stream(rq_stream_t stream)
{
    rq_xml_parser_t parser = rq_xml_parser_alloc();

    struct rq_dom_parser *dom_parser = (struct rq_dom_parser *)calloc(1, sizeof(struct rq_dom_parser));

	struct rq_xml_node *ret_node = NULL;

    rq_xml_parser_init(parser);
    rq_xml_parser_set_callback_data(parser, dom_parser);
    rq_xml_parser_set_callback(parser, dom_parser_cb);

    rq_xml_parser_parse(parser, stream);

	ret_node = dom_parser->head_element;

    free(dom_parser);

    rq_xml_parser_free(parser);

	return ret_node;
}

