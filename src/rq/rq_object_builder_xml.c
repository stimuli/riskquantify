/*
** rq_object_builder_xml.c
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
#include "rq_object_builder_xml.h"
#include "rq_xml_parser.h"
#include "rq_double_linked_list.h"
#include "rq_object.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


enum rq_next_value_type {
    NEXT_VALUE_TYPE_NONE,
    NEXT_VALUE_TYPE_STRING,
    NEXT_VALUE_TYPE_INTEGER,
    NEXT_VALUE_TYPE_DOUBLE,
    NEXT_VALUE_TYPE_DATE
};

struct rq_object_builder_xml_el_node {
    const char *object_name;
    void *object;
    rq_object_schema_t object_schema;
};

struct rq_object_builder_xml {
    rq_object_schema_mgr_t object_schema_mgr;
    rq_double_linked_list_t object_list;
    int started_construction;
    const char *object_path;
    const char *object_type_name;
    const char *current_path_ptr;
    unsigned current_path_depth;
    char *namebuf;
    unsigned namebuflen;
    enum rq_next_value_type next_value_type;
    struct rq_object_builder_xml_el_node *current_node;

    int finished;
    void *built_object;
};

const char * const rq_object_builder_xml_type_name = "XMLObjectBuilder";

static void
rq_object_builder_xml_el_node_free(void *p)
{
    struct rq_object_builder_xml_el_node *n = (struct rq_object_builder_xml_el_node *)p;

    if (n->object_name)
       RQ_FREE((char *)n->object_name);
   RQ_FREE(n);
}

static void
rq_object_builder_xml_free(void *p)
{
    struct rq_object_builder_xml *obx = (struct rq_object_builder_xml *)p;
    if (obx)
    {
        rq_double_linked_list_free(obx->object_list);

        if (obx->object_type_name)
           RQ_FREE((char *)obx->object_type_name);
       RQ_FREE(obx);
    }
}

void 
rq_object_builder_xml_callback(
    void *callback_data, 
    enum rq_xml_parser_parse_event_type parse_event_type, 
    const char *name, 
    const char *value
    )
{
    struct rq_object_builder_xml *obx = (struct rq_object_builder_xml *)callback_data;
    short process_event = 0;

    if (obx->finished)
        return;

    if (!obx->started_construction)
    {
        if (parse_event_type == RQ_XML_PARSER_PARSE_EVENT_TYPE_ENTERING_ELEMENT &&
            !memcmp(obx->current_path_ptr, name, strlen(name)) &&
            (obx->current_path_ptr[strlen(name)] == '/' || 
             obx->current_path_ptr[strlen(name)] == '\0')
            )
        {
            if (obx->current_path_ptr[strlen(name)] == '/')
            {
                obx->current_path_ptr = obx->current_path_ptr + strlen(name) + 1;
                obx->current_path_depth++;
            }
            else if (obx->current_path_ptr[strlen(name)] == '\0')
            {
                obx->started_construction = 1;
                process_event = 1;
            }
        }
        else if (parse_event_type == RQ_XML_PARSER_PARSE_EVENT_TYPE_LEAVING_ELEMENT &&
                 obx->current_path_depth > 0)
        {
            if (obx->current_path_ptr > obx->object_path && 
                *(obx->current_path_ptr-1) == '/')
                obx->current_path_ptr -= 2;
            while (obx->current_path_ptr > obx->object_path)
            {
                if (*(obx->current_path_ptr-1) == '/')
                    break;
                obx->current_path_depth--;
            }
        }
    }
    else
        process_event = 1;

    if (process_event)
    {
        switch (parse_event_type)
        {
            case RQ_XML_PARSER_PARSE_EVENT_TYPE_ENTERING_ELEMENT:
            {
                const char *object_type_name = NULL;

                if (rq_double_linked_list_size(obx->object_list) > 0)
                {
                    rq_double_linked_list_iterator_t dll_it = rq_double_linked_list_rbegin(obx->object_list);
                    struct rq_object_builder_xml_el_node *node = 
                        (struct rq_object_builder_xml_el_node *)rq_double_linked_list_iterator_get_data(dll_it);

                    rq_object_schema_node_t osn = rq_object_schema_find_property(node->object_schema, name);
                    if (osn)
                        object_type_name = rq_object_schema_node_get_type_name(osn);
                }
                else
                    object_type_name = obx->object_type_name;

                if (object_type_name)
                {
                    if (!strcmp(object_type_name, "string"))
                        obx->next_value_type = NEXT_VALUE_TYPE_STRING;
                    else if (!strcmp(object_type_name, "integer"))
                        obx->next_value_type = NEXT_VALUE_TYPE_INTEGER;
                    else if (!strcmp(object_type_name, "double"))
                        obx->next_value_type = NEXT_VALUE_TYPE_DOUBLE;
                    else if (!strcmp(object_type_name, "date"))
                        obx->next_value_type = NEXT_VALUE_TYPE_DATE;
                    else
                    {
                        rq_object_schema_t object_schema = 
                            rq_object_schema_mgr_find(obx->object_schema_mgr, object_type_name);
                        if (object_schema)
                        {
                            struct rq_object_builder_xml_el_node *node = 
                                (struct rq_object_builder_xml_el_node *)
                                RQ_MALLOC(sizeof(struct rq_object_builder_xml_el_node));

                            obx->current_node = node;

                            node->object_name = RQ_STRDUP(name);
                            node->object = rq_object_schema_construct_object(object_schema);
                            node->object_schema = object_schema;
                            rq_double_linked_list_append(obx->object_list, node);
                        }
                    }
                }

            }
            break;

            case RQ_XML_PARSER_PARSE_EVENT_TYPE_LEAVING_ELEMENT:
            {
                rq_double_linked_list_iterator_t dll_it = rq_double_linked_list_rbegin(obx->object_list);
                /* if (rq_double_linked_list_at_rend(dll_it))
                   do error processing... 
                */
                struct rq_object_builder_xml_el_node *child_node = 
                    (struct rq_object_builder_xml_el_node *)rq_double_linked_list_iterator_get_data(dll_it);

                obx->next_value_type = NEXT_VALUE_TYPE_NONE;

                if (!strcmp(child_node->object_name, name))
                {
                    struct rq_object_builder_xml_el_node *parent_node = NULL;

                    dll_it = rq_double_linked_list_iterator_decr(dll_it);
                    parent_node = 
                        (struct rq_object_builder_xml_el_node *)rq_double_linked_list_iterator_get_data(dll_it);

                    if (!parent_node)
                    {
                        obx->finished = 1;
                        obx->built_object = child_node->object;
                        obx->current_node = NULL;
                    }
                    else
                    {
                        rq_object_schema_set_value_object(parent_node->object_schema, parent_node->object, name, child_node->object);
                        obx->current_node = parent_node;
                    }

                    dll_it = rq_double_linked_list_rbegin(obx->object_list);
                    rq_double_linked_list_erase(obx->object_list, dll_it);
                }
            }
            break;

            case RQ_XML_PARSER_PARSE_EVENT_TYPE_GOT_ATTRIBUTE_VALUE:
            {
                if (obx->current_node != NULL)
                {
                    rq_object_schema_t object_schema = obx->current_node->object_schema;
                    unsigned namelen = (unsigned)strlen(name);
                    if (namelen >= obx->namebuflen)
                    {
                        obx->namebuf = (char *)realloc(obx->namebuf, namelen+1);
                        obx->namebuflen = namelen+1;
                    }

                    strcpy(obx->namebuf, "@");
                    strcat(obx->namebuf, name);
                    rq_object_schema_set_value_string(object_schema, obx->current_node->object, obx->namebuf, value);

                }
            }
            break;

            case RQ_XML_PARSER_PARSE_EVENT_TYPE_GOT_ELEMENT_VALUE:
            {
                if (obx->current_node != NULL)
                {
                    int process_value = (obx->next_value_type != NEXT_VALUE_TYPE_NONE);
                    const char *property_name = name;
                    enum rq_next_value_type next_value_type = obx->next_value_type;
                    rq_object_schema_t object_schema = obx->current_node->object_schema;

                    if (!process_value)
                    {
                        if (!strcmp(obx->current_node->object_name, name))
                        {
                            rq_object_schema_node_t node = 
                                rq_object_schema_find_property(object_schema, ".");
                            if (node)
                            {
                                process_value = 1;
                                property_name = ".";

                                if (!strcmp(rq_object_schema_node_get_type_name(node), "string"))
                                    next_value_type = NEXT_VALUE_TYPE_STRING;
                                else if (!strcmp(rq_object_schema_node_get_type_name(node), "integer"))
                                    next_value_type = NEXT_VALUE_TYPE_INTEGER;
                                else if (!strcmp(rq_object_schema_node_get_type_name(node), "double"))
                                    next_value_type = NEXT_VALUE_TYPE_DOUBLE;
                                else if (!strcmp(rq_object_schema_node_get_type_name(node), "date"))
                                    next_value_type = NEXT_VALUE_TYPE_DATE;
                                else
                                    process_value = 0;
                            }
                        }
                    }

                    if (process_value)
                    {
                        switch (next_value_type)
                        {
                            case NEXT_VALUE_TYPE_STRING:
                                rq_object_schema_set_value_string(object_schema, obx->current_node->object, ".", value);
                                break;

                            case NEXT_VALUE_TYPE_INTEGER:
                            {
                                long l = atol(value);
                                rq_object_schema_set_value_integer(object_schema, obx->current_node->object, ".", l);
                            }
                            break;

                            case NEXT_VALUE_TYPE_DOUBLE:
                            {
                                double d = atof(value);
                                rq_object_schema_set_value_double(object_schema, obx->current_node->object, ".", d);
                            }
                            break;

                            case NEXT_VALUE_TYPE_DATE:
                            {
                                rq_date d = rq_date_parse(value, RQ_DATE_FORMAT_YMD);
                                rq_object_schema_set_value_date(object_schema, obx->current_node->object, ".", d);
                            }
                            break;
                        }
                    }
                }
            }
            break;
        }
    }
}


static void *
rq_object_builder_xml_build(void *p, rq_stream_t stream, rq_object_schema_mgr_t schema_mgr, const char *object_path, const char *object_type_name)
{
    struct rq_object_builder_xml obx;
    rq_xml_parser_t parser = rq_xml_parser_alloc();

    obx.object_schema_mgr = schema_mgr;
    obx.object_path = object_path;
    obx.object_type_name = object_type_name;
    obx.current_path_ptr = object_path;
    if (*obx.current_path_ptr == '/')
        obx.current_path_ptr++;

    obx.namebuf = (char *)RQ_MALLOC(256);
    obx.namebuflen = 256;
    obx.next_value_type = NEXT_VALUE_TYPE_NONE;

    obx.started_construction = 0;
    obx.object_list = rq_double_linked_list_alloc(rq_object_builder_xml_el_node_free);
    obx.finished = 0;
    obx.built_object = NULL;

    obx.current_node = NULL;

    rq_xml_parser_set_callback_data(
        parser, 
        &obx
        );

    rq_xml_parser_set_callback(
        parser,
        rq_object_builder_xml_callback
        );

    rq_xml_parser_parse(parser, stream);

    rq_double_linked_list_free(obx.object_list);
    rq_xml_parser_free(parser);

   RQ_FREE(obx.namebuf);

    return obx.built_object;
}

RQ_EXPORT rq_object_builder_t 
rq_object_builder_xml_alloc()
{
    return _rq_object_builder_alloc(
        rq_object_builder_xml_type_name,
        NULL,
        rq_object_builder_xml_free,
        rq_object_builder_xml_build
        );
}

