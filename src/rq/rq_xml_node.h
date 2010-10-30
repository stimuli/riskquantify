/*
** rq_xml_node.h
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
#ifndef rq_xml_node_h
#define rq_xml_node_h

#include "rq_config.h"
#include "rq_defs.h"

enum rq_xml_node_type {
    RQ_XML_NODE_TYPE_ELEMENT,
    RQ_XML_NODE_TYPE_ATTRIBUTE,
    RQ_XML_NODE_TYPE_TEXT
};

struct rq_xml_node {
    enum rq_xml_node_type node_type;

    struct rq_xml_node *next; /**< next node at this level. */
    
    union {
        struct {
            const char *name;
            const char *value;
        } att;

        struct {
            const char *tag;
            struct rq_xml_node *parent;

            struct rq_xml_node *first_child;
            struct rq_xml_node *last_child;
            unsigned num_children;

            struct rq_xml_node *first_att;
            struct rq_xml_node *last_att;
        } el;

        struct {
            const char *text;
        } txt;
    } node;
};

RQ_EXPORT struct rq_xml_node *rq_xml_node_alloc(enum rq_xml_node_type type);

RQ_EXPORT void rq_xml_node_free(struct rq_xml_node *nd);

RQ_EXPORT void rq_xml_node_add_child(struct rq_xml_node *parent, struct rq_xml_node *child);

RQ_EXPORT void rq_xml_node_add_sibling(struct rq_xml_node *brother, struct rq_xml_node *sister);

RQ_EXPORT struct rq_xml_node *rq_xml_node_add_attribute(struct rq_xml_node *element, const char *name, const char *value);

RQ_EXPORT struct rq_xml_node *rq_xml_node_add_text(struct rq_xml_node *element, const char *text);

RQ_EXPORT struct rq_xml_node *rq_xml_node_find(struct rq_xml_node *root_node, const char *xpath);

RQ_EXPORT struct rq_xml_node *rq_xml_node_find_attribute(struct rq_xml_node *element, const char *attname);

RQ_EXPORT struct rq_xml_node *rq_xml_node_find_element(struct rq_xml_node *element, const char *tagname, unsigned offset);

RQ_EXPORT void rq_xml_node_print(struct rq_xml_node *root_node);

RQ_EXPORT unsigned rq_xml_node_get_text(struct rq_xml_node *node, char *buf, unsigned max_buf_len);

RQ_EXPORT unsigned rq_xml_node_find_text(struct rq_xml_node *root_node, const char *xpath, char *buf, unsigned max_buf_len);

#endif
