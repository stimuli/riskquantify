/*
** rq_xml_util.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2009 Brett Hutley
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
#include "rq_xml_util.h"
#include "rq_error.h"
#include <assert.h>

RQ_EXPORT rq_error_code 
rq_xml_util_get_start_end_pos(rq_stream_t stream, const char *tag, long *start_pos, long *end_pos)
{
    enum cur_state {
        WAITING_FOR_START_TAG,
        IN_START_TAG,
        WAITING_FOR_END_TAG_BRACKET,
        WAITING_FOR_END_TAG_SLASH,
        IN_END_TAG
    } cur_state = WAITING_FOR_START_TAG;

    const char *next_char_ptr = tag;
    long last_start_pos = 0;

    /*
      OK, essentially the loop goes like this:

      1. wait for left angle bracket - set IN_START_TAG, set next_char_ptr to tag.
      2. if IN_START_TAG and next_char_ptr == '\0'
      .. then
      .... if we have a space or a closing angle bracket
      .... then
      ...... set WAITING_FOR_END_TAG_BRACKET
      .... else
      ...... set WAITING_FOR_START_TAG
    */

    while (1)
    {
        char ch;
        long cur_pos = rq_stream_tell(stream);

        if (rq_stream_read(stream, &ch, 1) != 1)
            break;

        switch (cur_state)
        {
            case WAITING_FOR_START_TAG:
            {
                if (ch == '<')
                {
                    last_start_pos = cur_pos;
                    next_char_ptr = tag;
                    cur_state = IN_START_TAG;
                }
                break;
            }
            
            case IN_START_TAG:
            {
                if (next_char_ptr == '\0')
                {
                    if (ch == ' ' || ch == '>')
                    {
                        /* we have a winner! */
                        *start_pos = last_start_pos;
                        cur_state = WAITING_FOR_END_TAG_BRACKET;
                    }
                    else
                    {
                        /* OK, this tag just starts with our target tag.
                           It could be a short-cut element, but we don't
                           currently handle that.
                         */
                        cur_state = WAITING_FOR_START_TAG;
                    }
                }
                else
                {
                    /* currently going through the tag string. */
                    if (ch == *next_char_ptr)
                    {
                        next_char_ptr++;
                    }
                    else
                    {
                        /* Not a match! */
                        cur_state = WAITING_FOR_START_TAG;
                    }
                }
                break;
            }

            case WAITING_FOR_END_TAG_BRACKET:
            {
                if (ch == '<')
                {
                    cur_state = WAITING_FOR_END_TAG_SLASH;
                }
                break;
            }

            case WAITING_FOR_END_TAG_SLASH:
            {
                if (ch == '/')
                {
                    next_char_ptr = tag;
                    cur_state = IN_END_TAG;
                }
                else
                    cur_state = WAITING_FOR_END_TAG_BRACKET;

                break;
            }

            case IN_END_TAG:
            {
                if (*next_char_ptr == '\0')
                {
                    if (ch == '>')
                    {
                        /* Great! Got the end tag. */
                        *end_pos = rq_stream_tell(stream);
                        return RQ_OK;
                    }
                    else
                        cur_state = WAITING_FOR_END_TAG_BRACKET;
                }
                else
                {
                    if (ch == *next_char_ptr)
                        next_char_ptr++;
                    else
                        cur_state = WAITING_FOR_END_TAG_BRACKET;
                }
                break;
            }

            default:
                assert(0);
        }
    }
}

