/*
** rq_xml_node.c
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

#include "rq_xml_node.h"

RQ_EXPORT struct rq_xml_node *
rq_xml_node_alloc(enum rq_xml_node_type node_type)
{
    struct rq_xml_node *n = (struct rq_xml_node *)calloc(1, sizeof(struct rq_xml_node));
    n->node_type = node_type;

    return n;
}

RQ_EXPORT void 
rq_xml_node_free(struct rq_xml_node *nd)
{
    switch (nd->node_type)
    {
        case RQ_XML_NODE_TYPE_ELEMENT:
            if (nd->node.el.tag)
                free((char *)nd->node.el.tag);

            if (nd->node.el.first_child)
                rq_xml_node_free(nd->node.el.first_child);

            if (nd->node.el.first_att)
                rq_xml_node_free(nd->node.el.first_att);
            break;


        case RQ_XML_NODE_TYPE_ATTRIBUTE:
            if (nd->node.att.name)
                free((char *)nd->node.att.name);
            if (nd->node.att.value)
                free((char *)nd->node.att.value);
            break;

        case RQ_XML_NODE_TYPE_TEXT:
            if (nd->node.txt.text)
                free((char *)nd->node.txt.text);
            break;

        default:
            assert(0);
    }

    if (nd->next)
        rq_xml_node_free(nd->next);

    free(nd);
}

RQ_EXPORT void 
rq_xml_node_add_child(struct rq_xml_node *parent, struct rq_xml_node *child)
{
    child->node.el.parent = parent;

    if (parent->node.el.last_child)
        parent->node.el.last_child->next = child;
    else
        parent->node.el.first_child = child;

    parent->node.el.last_child = child;
    parent->node.el.num_children++;
}

RQ_EXPORT void 
rq_xml_node_add_sibling(struct rq_xml_node *brother, struct rq_xml_node *sister)
{
    struct rq_xml_node *n = brother;

    while (n->next)
        n = n->next;

    n->next = sister;
}

RQ_EXPORT struct rq_xml_node *
rq_xml_node_add_attribute(struct rq_xml_node *element, const char *name, const char *value)
{
    struct rq_xml_node *n = rq_xml_node_alloc(RQ_XML_NODE_TYPE_ATTRIBUTE);
    n->node.att.name = strdup(name);
    n->node.att.value = strdup(value);

    if (element->node.el.last_att)
        element->node.el.last_att->next = n;
    else
        element->node.el.first_att = n;

    element->node.el.last_att = n;

    return n;
}

RQ_EXPORT struct rq_xml_node *
rq_xml_node_add_text(struct rq_xml_node *element, const char *text)
{
    struct rq_xml_node *n = rq_xml_node_alloc(RQ_XML_NODE_TYPE_TEXT);
    n->node.txt.text = strdup(text);

    if (element->node.el.last_child)
        element->node.el.last_child->next = n;
    else
        element->node.el.first_child = n;

    element->node.el.last_child = n;
    
    return n;
}

static const char *
get_next_path_element(const char *xpath, char *tok_buf, unsigned tok_buf_len, unsigned *offset)
{
    int i;

    *tok_buf = '\0';
    *offset = 0;

    if (*xpath == '/')
    {
        xpath++;
    }

    for (i = 0; xpath[i] != '\0'; i++)
    {
        if (xpath[i] == '/' || xpath[i] == '[')
            break;

        tok_buf[i] = xpath[i];
    }

    tok_buf[i] = '\0';

    xpath += i;

    if (*xpath == '[')
    {
        unsigned j = 0;
        char num_buf[50];

        for (i = 1; xpath[i] != '\0'; i++)
        {
            if (xpath[i] == ']' || xpath[i] == '/')
                break;

            if (j == 49)
                break;

            if (j >= tok_buf_len)
            {
                tok_buf[tok_buf_len-1] = '\0';
                return NULL;
            }

            num_buf[j++] = xpath[i];
        }

        num_buf[j] = '\0';

        *offset = atoi(num_buf);

        xpath += i;
        if (*xpath == ']')
            xpath++;
    }

    return xpath;
}

RQ_EXPORT struct rq_xml_node *
rq_xml_node_find(struct rq_xml_node *root_node, const char *xpath)
{
    struct rq_xml_node *cur_node = root_node;
    char tok_buf[1024];
    unsigned num_nodes_traversed = 0;
	unsigned offset;

    while ((xpath = get_next_path_element(xpath, tok_buf, 1024, &offset)) != NULL)
    {
        if (*tok_buf == '@')
            cur_node = rq_xml_node_find_attribute(cur_node, tok_buf+1);
        else if (*tok_buf != '\0')
        {
            struct rq_xml_node *node_to_search = cur_node;
            if (num_nodes_traversed++ > 0)
                cur_node = cur_node->node.el.first_child;

            if (!cur_node)
                return NULL;

            if ((cur_node = rq_xml_node_find_element(cur_node, tok_buf, offset)) == NULL)
				return NULL;
        }
        else
            break;
    }

    return cur_node;
}

RQ_EXPORT struct rq_xml_node *
rq_xml_node_find_attribute(struct rq_xml_node *element, const char *attname)
{
    if (element->node_type == RQ_XML_NODE_TYPE_ELEMENT)
    {
        struct rq_xml_node *att;

        for (att = element->node.el.first_att; att; att = att->next)
            if (att->node.att.name && !strcmp(att->node.att.name, attname))
                return att;
    }

    return NULL;
}

RQ_EXPORT struct rq_xml_node *
rq_xml_node_find_element(struct rq_xml_node *element, const char *tagname, unsigned offset)
{
    unsigned found_count = 0;

    if (offset == 0) 
        offset = 1;

    for ( ; element; element = element->next)
        if (element->node_type == RQ_XML_NODE_TYPE_ELEMENT && element->node.el.tag && !strcmp(element->node.el.tag, tagname))
            if (++found_count == offset)
                return element;

    return NULL;
}

RQ_EXPORT void 
rq_xml_node_print(struct rq_xml_node *node)
{
    if (node)
    {
        switch (node->node_type)
        {
            case RQ_XML_NODE_TYPE_ELEMENT:
                printf("<%s", node->node.el.tag);
                if (node->node.el.first_att)
                    rq_xml_node_print(node->node.el.first_att);

                if (!node->node.el.first_child)
                    printf("/>");
                else
                {
					struct rq_xml_node *child;

                    printf(">");

					for (child = node->node.el.first_child; child; child = child->next)
						rq_xml_node_print(child);

                    printf("</%s>", node->node.el.tag);
                }

                break;

            case RQ_XML_NODE_TYPE_ATTRIBUTE:
                printf(" %s", node->node.att.name);
                if (node->node.att.value)
                    printf("='%s'", node->node.att.value);
                break;

            case RQ_XML_NODE_TYPE_TEXT:
                printf("%s", node->node.txt.text);
                break;
        }

        node = node->next;
    }
}

RQ_EXPORT unsigned
rq_xml_node_get_text(struct rq_xml_node *node, char *buf, unsigned max_buf_len)
{
    unsigned bytes_copied = 0;

    if (max_buf_len > 0)
    {
        if (node->node_type == RQ_XML_NODE_TYPE_ELEMENT)
        {
            struct rq_xml_node *child;
            for (child = node->node.el.first_child; child; child = child->next)
                if (child->node_type == RQ_XML_NODE_TYPE_TEXT)
                {
                    int i;
                    for (i = 0; child->node.txt.text[i] != '\0' && bytes_copied < max_buf_len; i++, bytes_copied++)
                        buf[bytes_copied] = child->node.txt.text[i];
                }
        }
    }

    if (bytes_copied < max_buf_len)
        buf[bytes_copied] = '\0';
    else if (max_buf_len > 0)
        buf[max_buf_len-1] = '\0';

    return bytes_copied;
}

RQ_EXPORT unsigned
rq_xml_node_find_text(struct rq_xml_node *root_node, const char *xpath, char *buf, unsigned max_buf_len)
{
    struct rq_xml_node *node = rq_xml_node_find(root_node, xpath);
    if (node)
        return rq_xml_node_get_text(node, buf, max_buf_len);
    else if (max_buf_len > 0)
        buf[0] = '\0';
    return 0;
}
