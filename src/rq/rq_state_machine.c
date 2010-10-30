/*
** rq_state_machine.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2006-2008 Brett Hutley
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
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
** USA
*/
#include "rq_state_machine.h"
#include <stdlib.h>
#include <stdarg.h>

struct rq_state_machine *
rq_state_machine_alloc(
    unsigned num_states, 
    void *global_data,
    void (*free_global_data)(void *),
    void (*free_state_data)(void *),
    struct rq_state * (*next_state_cb)(void *global_data, void *input_data, struct rq_state *cur_state)
    )
{
    struct rq_state_machine *sm = (struct rq_state_machine *)RQ_CALLOC(1, sizeof(struct rq_state_machine));
    int i;

    sm->state_array = (struct rq_state **)RQ_CALLOC(num_states, sizeof(struct rq_state *));

    for (i = 0; i < num_states; i++)
    {
        sm->state_array[i] = (struct rq_state *)RQ_CALLOC(1, sizeof(struct rq_state));
        sm->state_array[i]->id = i;
    }

    sm->num_states = num_states;
    sm->global_data = global_data;
    sm->free_global_data = free_global_data;
    sm->next_state_cb = next_state_cb;

    return sm;
}

void
rq_state_machine_free(struct rq_state_machine *sm)
{
    int i;

    for (i = 0; i < sm->num_states; i++)
    {
        if (sm->state_array[i])
        {
            if (sm->state_array[i]->state_data && sm->free_state_data)
                (*sm->free_state_data)(sm->state_array[i]->state_data);

            RQ_FREE(sm->state_array[i]);
        }
    }

    RQ_FREE(sm->state_array);

    if (sm->global_data && sm->free_global_data)
        (*sm->free_global_data)(sm->global_data);
}

struct rq_state *
rq_state_machine_add_state(
    struct rq_state_machine *sm, 
    unsigned state_id, 
    void *state_data, 
    void (*enter_state_func)(void *global_data, void *input_data, struct rq_state *prev_state, struct rq_state *next_state),
    void (*leave_state_func)(void *global_data, void *input_data, struct rq_state *prev_state, struct rq_state *next_state),
    unsigned num_next_states, ...)
{
    struct rq_state *state = sm->state_array[state_id];
    va_list arglist;

    va_start(arglist, num_next_states);

    state->state_data = state_data;
    state->enter_state_func = enter_state_func;
    state->leave_state_func = leave_state_func;

    if (num_next_states)
    {
        int i;

        state->num_next_states = num_next_states;
        state->next_states = (struct rq_state **)RQ_CALLOC(num_next_states, sizeof(struct rq_state *));
        for (i = 0; i < num_next_states; i++)
        {
            unsigned id = va_arg(arglist, unsigned);
            state->next_states[i] = sm->state_array[id];
        }
    }

    va_end(arglist);

    return state;
}

struct rq_state *
rq_state_machine_next_state(struct rq_state_machine *sm, void *input_data)
{
    struct rq_state *state = (*sm->next_state_cb)(sm->global_data, input_data, sm->cur_state);
    if (state && state != sm->cur_state)
    {
        if (sm->cur_state->leave_state_func)
            (*sm->cur_state->leave_state_func)(sm->global_data, input_data, sm->cur_state, state);
        if (state->enter_state_func)
            (*state->enter_state_func)(sm->global_data, input_data, sm->cur_state, state);
        sm->cur_state = state;
    }

    return sm->cur_state;
}

