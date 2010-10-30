/**
 * @file
 *
 * A state machine.
 */
/*
** rq_state_machine.h
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
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef rq_state_machine_h
#define rq_state_machine_h

#include "rq_config.h"
#include "rq_defs.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

struct rq_state {
    unsigned id;

    struct rq_state **next_states;
    unsigned num_next_states;

    void *state_data;

    void (*enter_state_func)(void *global_data, void *input_data, struct rq_state *prev_state, struct rq_state *next_state);
    void (*leave_state_func)(void *global_data, void *input_data, struct rq_state *prev_state, struct rq_state *next_state);
};

struct rq_state_machine {
    struct rq_state **state_array;
    unsigned num_states;

    struct rq_state *cur_state;

    void *global_data;

    void (*free_global_data)(void *global_data);
    void (*free_state_data)(void *state_data);

    struct rq_state * (*next_state_cb)(void *global_data, void *input_data, struct rq_state *cur_state);
};


struct rq_state_machine *
rq_state_machine_alloc(
    unsigned num_states, 
    void *global_data,
    void (*free_global_data)(void *),
    void (*free_state_data)(void *),
    struct rq_state * (*next_state_cb)(void *global_data, void *input_data, struct rq_state *cur_state)
    );


void rq_state_machine_free(struct rq_state_machine *sm);

struct rq_state *
rq_state_machine_add_state(
    struct rq_state_machine *sm, 
    unsigned state_id, 
    void *state_data, 
    void (*enter_state_func)(void *global_data, void *input_data, struct rq_state *prev_state, struct rq_state *next_state),
    void (*leave_state_func)(void *global_data, void *input_data, struct rq_state *prev_state, struct rq_state *next_state),
    unsigned num_next_states, ...);


struct rq_state *rq_state_machine_next_state(struct rq_state_machine *sm, void *input_data);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
