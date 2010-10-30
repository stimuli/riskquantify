/**
 * \file rq_object.h
 * \author Brett Hutley
 *
 * \brief A risk quantify object acts kinda like a variant type in the
 * Windoze world, and kinda like a Lisp CONS object in the real world
 * ;)
 */
/*
** rq_object.h
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
#ifndef rq_object_h
#define rq_object_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_object_schema.h"
#include "rq_array.h"
#include "rq_tree_rb.h"
#include "rq_double_linked_list.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

struct rq_object_mgr;

enum rq_object_type {
    RQ_OBJECT_TYPE_NIL = 0,
    RQ_OBJECT_TYPE_TRUE = 1,
    RQ_OBJECT_TYPE_INTEGER = 2,
    RQ_OBJECT_TYPE_DOUBLE = 3,
    RQ_OBJECT_TYPE_STRING = 4,
    RQ_OBJECT_TYPE_DATE = 5,
    RQ_OBJECT_TYPE_CONS = 6,
    RQ_OBJECT_TYPE_IDENTIFIER = 7,
    RQ_OBJECT_TYPE_RQOBJECT = 8,
    RQ_OBJECT_TYPE_BUILTIN = 9, /* an inbuilt function */
    RQ_OBJECT_TYPE_TREE = 10,
    RQ_OBJECT_TYPE_VECTOR = 11,
    RQ_OBJECT_TYPE_ERROR = 50
};

typedef struct rq_object {
    enum rq_object_type object_type;

    unsigned short gc_id;

    union {
        long i;
        double d;
        const char *s;
        rq_date date;
        struct {
            struct rq_object *car;
            struct rq_object *cdr;
        } cons;
        struct {
            void *object;
            rq_object_schema_t object_schema;
        } rqobject;
        rq_tree_rb_t tree;
        rq_array_t vector;
        struct rq_object *(*func)(void *, struct rq_object *);
    } value;
} *rq_object_t;

/* The object pool */
struct rq_object_pool {
    unsigned int num_objects;
    unsigned int current_offset;
    struct rq_object *pool;
};

/* 
   The object manager
   ==================

   Memory Management:
   There is at least one object pool. When a new object is allocated it
   is removed from the free_list and put on the used list. If the free_list
   is empty, then an object is grabbed from the current object pool. If
   the current object pool is used up, a new object pool is created and
   added to the list.

   Garbage Collection:
   Firstly the gc_id is incremented in the object manager. Then all the
   objects in each symbol table is tagged with the gc_id. Then the used
   list is traversed and any object not containing the current gc_id is
   removed, cleared and linked to the beginning of the free list.
*/

typedef struct rq_object_mgr {
    unsigned short gc_id;
    rq_double_linked_list_t used_list;
    struct rq_object *free_list;
    rq_array_t object_pools;
    rq_double_linked_list_t symbol_tables; /* a list of symbol tables */
} *rq_object_mgr_t;

/**
 * A globally available nil object.
 */
extern rq_object_t rq_object_nil;

/**
 * A globally available true object
 */
extern rq_object_t rq_object_true;

/**
 * Determine if an rq_object is NULL. (ie if the object
 * hasn't been allocated).
 */
RQ_EXPORT int rq_object_is_null(rq_object_t obj);

/**
 * test the object to see if it is nil - either the same
 * as the rq_object_nil global, or has a NIL type.
 */
RQ_EXPORT int rq_object_is_nil(rq_object_t obj);

/**
 * Allocate an object which initially has a NIL type
 */
RQ_EXPORT rq_object_t rq_object_alloc(struct rq_object_mgr * object_mgr);

/**
 * Allocate an integer object
 */
RQ_EXPORT rq_object_t rq_object_alloc_integer(struct rq_object_mgr * object_mgr, long i);

/**
 * Allocate a double object
 */
RQ_EXPORT rq_object_t rq_object_alloc_double(struct rq_object_mgr * object_mgr, double d);

/**
 * Allocate a string object
 */
RQ_EXPORT rq_object_t rq_object_alloc_string(struct rq_object_mgr * object_mgr, const char *s);

/**
 * Allocate a date object
 */
RQ_EXPORT rq_object_t rq_object_alloc_date(struct rq_object_mgr * object_mgr, rq_date date);

/**
 * Allocate a CONS object
 */
RQ_EXPORT rq_object_t rq_object_alloc_cons(struct rq_object_mgr * object_mgr, rq_object_t car, rq_object_t cdr);

/**
 * Allocate an Identifier type object
 */
RQ_EXPORT rq_object_t rq_object_alloc_identifier(struct rq_object_mgr * object_mgr, const char *id);

/**
 * Allocate an RQObject type object
 */
RQ_EXPORT rq_object_t rq_object_alloc_rqobject(struct rq_object_mgr * object_mgr, void *object, rq_object_schema_t object_schema);

/**
 * Allocate a 'Builtin' type object
 */
RQ_EXPORT rq_object_t rq_object_alloc_builtin(struct rq_object_mgr * object_mgr, rq_object_t (*func)(void *, rq_object_t));

/**
 * Allocate a tree type object
 */
RQ_EXPORT rq_object_t rq_object_alloc_tree(struct rq_object_mgr * object_mgr);

/**
 * Allocate a vector type object
 */
RQ_EXPORT rq_object_t rq_object_alloc_vector(struct rq_object_mgr * object_mgr);

/**
 * Clear an object, freeing it's memory, and setting it's
 * type to NIL
 */
RQ_EXPORT void rq_object_clear(rq_object_t obj);

/**
 * Deep copy the passed in object
 */
RQ_EXPORT rq_object_t rq_object_clone(struct rq_object_mgr * object_mgr, rq_object_t obj);

/**
 * Reset the object to be NIL WITHOUT releasing
 * the memory managed by this object. Used if we want
 * to acquire the management of the contents.
 */
RQ_EXPORT void rq_object_reset(rq_object_t obj);

/**
 * Get an object's type
 */
RQ_EXPORT enum rq_object_type rq_object_get_object_type(rq_object_t obj);

/**
 * Get the integer value of the object
 */
RQ_EXPORT int rq_object_get_integer(rq_object_t obj);

/**
 * Get the double value of the object
 */
RQ_EXPORT double rq_object_get_double(rq_object_t obj);

/**
 * Get the string value of the object
 */
RQ_EXPORT const char *rq_object_get_string(rq_object_t obj);

/**
 * Get the date value of the object
 */
RQ_EXPORT rq_date rq_object_get_date(rq_object_t obj);

/**
 * Get the car of the cons value of the object
 */
RQ_EXPORT rq_object_t rq_object_get_cons_car(rq_object_t obj);

/**
 * Get the cdr of the cons value of the object
 */
RQ_EXPORT rq_object_t rq_object_get_cons_cdr(rq_object_t obj);

/**
 * Get the identifier string value from the object
 */
RQ_EXPORT const char *rq_object_get_identifier(rq_object_t obj);

/**
 * Get the RQOBJECT value from the object
 */
RQ_EXPORT void *rq_object_get_rqobject(rq_object_t obj);

/**
 * Call the builtin function contained in the object obj, passing 
 * it the object manager and the arguments specified by args.
 */
RQ_EXPORT rq_object_t rq_object_call_builtin(rq_object_t obj, void *interpreter, rq_object_t args);

/**
 * Set the value of an object to be a copy of the other object
 */
RQ_EXPORT void rq_object_set_object(struct rq_object_mgr * object_mgr, rq_object_t obj, rq_object_t value);

/**
 * Set the value of the object to be the int value, first
 * clearing any existing data in the object.
 */
RQ_EXPORT void rq_object_set_integer(rq_object_t obj, int value);

/**
 * Set the value of the object to be the double value, first
 * clearing any existing data in the object.
 */
RQ_EXPORT void rq_object_set_double(rq_object_t obj, double value);

/**
 * Set the value of the object to be the string value, first
 * clearing any existing data in the object.
 */
RQ_EXPORT void rq_object_set_string(rq_object_t obj, const char *value);

/**
 * Set the value of the object to be the date value, first
 * clearing any existing data in the object.
 */
RQ_EXPORT void rq_object_set_date(rq_object_t obj, rq_date value);

/**
 * Set the value of the object to be the cons represented
 * by the car and cdr values passed as arguments, first
 * clearing any existing data in the object.
 */
RQ_EXPORT void rq_object_set_cons(rq_object_t obj, rq_object_t car, rq_object_t cdr);

/**
 * Set the car cell of the CONS object passed as the first
 * parameter. This function asserts if the first object isn't
 * of type CONS.
 */
RQ_EXPORT void rq_object_set_cons_car(rq_object_t obj, rq_object_t car);

/**
 * Set the cdr cell of the CONS object passed as the first
 * parameter. This function asserts if the first object isn't
 * of type CONS.
 */
RQ_EXPORT void rq_object_set_cons_cdr(rq_object_t obj, rq_object_t cdr);

/**
 * Set the value of the object to be the identifier value, first
 * clearing any existing data in the object.
 */
RQ_EXPORT void rq_object_set_identifier(rq_object_t obj, const char *value);

/**
 * Set the value of the object to be the rqobject pointed to by 
 * the variable rqobject, and represented by the schema object_schema.
 * Clear any existing data in the object first.
 */
RQ_EXPORT void rq_object_set_rqobject(rq_object_t obj, void *rqobject, rq_object_schema_t object_schema);

/**
 * Set the function pointer of the builtin object to be the func value, 
 * first clearing any existing data in the object.
 */
RQ_EXPORT void rq_object_set_builtin(rq_object_t obj, rq_object_t (*func)(void *, rq_object_t ));

/**
 * Aquire the RQOBJECT value from the object. By making this
 * call you are taking ownership of the RQOBJECT and become
 * responsible for deleting it. This call also resets the type
 * of the rq_object_t object to be NIL.
 */
RQ_EXPORT void *rq_object_acquire_rqobject(rq_object_t obj);

/**
 * Get the type name of this RQOBJECT.
 */
RQ_EXPORT const char *rq_object_get_rqobject_type_name(rq_object_t obj);

/**
 * Get the object schema of this RQOBJECT.
 */
RQ_EXPORT rq_object_schema_t rq_object_get_rqobject_schema(rq_object_t obj);

/**
 * Coerce the value of the object to an integer
 */
RQ_EXPORT long rq_object_coerce_integer(rq_object_t obj);

/**
 * Coerce the value of the object to a double
 */
RQ_EXPORT double rq_object_coerce_double(rq_object_t obj);

/**
 * Coerce the value of the object to a string
 */
RQ_EXPORT const char *rq_object_coerce_string(rq_object_t obj);

/**
 * Coerce the value of the object to a date
 */
RQ_EXPORT rq_date rq_object_coerce_date(rq_object_t obj);

/**
 * Coerce the value of the object to an identifier
 * @return returns NULL is the object can't be coerced to an
 * identifier.
 */
RQ_EXPORT const char *rq_object_coerce_identifier(rq_object_t obj);

/**
 * Count the number of items in a cons list
 *
 * @return the number of items in the list, or -1 if the object passed
 *  in is not a CONS.
 */
RQ_EXPORT int rq_object_list_size(rq_object_t obj);

/**
 * Return the number of objects contained by this object
 * vector.
 */
RQ_EXPORT unsigned int rq_object_vector_size(rq_object_t obj);

/**
 * Get the object at a particular zero-based offset in this
 * object vector
 */
RQ_EXPORT rq_object_t rq_object_vector_get_at(rq_object_t obj, unsigned int i);

/**
 * Add a new object to the object vector.
 */
RQ_EXPORT void rq_object_vector_add(rq_object_t obj, rq_object_t child);

/**
 * Find an object within an object tree
 */
RQ_EXPORT rq_object_t rq_object_tree_find(rq_object_t obj, rq_object_t key);

/**
 * Get the list of key objects out of the tree. The key objects are
 * cloned and then add to a list that is returned in an object of type
 * RQ_OBJECT_TYPE_VECTOR, and should be freed by the caller.  
 */
RQ_EXPORT rq_object_t rq_object_tree_keys(struct rq_object_mgr * object_mgr, rq_object_t obj);

/** Add an object to the tree.
 * Add an object to the tree. The object to add must be of type
 * RQ_OBJECT_TYPE_CONS or the function will assert. The car of the cons
 * is the key, and the cdr of the cons is the value to be inserted.
 */
RQ_EXPORT void rq_object_tree_add(rq_object_t obj, rq_object_t cons);

/** Clear all the objects from the tree.
 *
 */
RQ_EXPORT void rq_object_tree_clear(rq_object_t obj);

/**
 * Allocate a new object manager.
 */
RQ_EXPORT struct rq_object_mgr * rq_object_mgr_alloc();

/**
 * Free an allocated object manager.
 */
RQ_EXPORT void rq_object_mgr_free(struct rq_object_mgr * object_mgr);

/**
 * Add a symbol to the symbol table
 */
RQ_EXPORT void rq_object_mgr_symbol_add(struct rq_object_mgr * object_mgr, const char *id, rq_object_t obj);

/**
 * Find a symbol within the symbol tables that the object manager 
 * manages.
 */
RQ_EXPORT rq_object_t rq_object_mgr_symbol_find(struct rq_object_mgr * object_mgr, const char *id);

/**
 * Set a symbol in the last symbol table in the symbol table stack
 */
RQ_EXPORT void rq_object_mgr_symbol_set_local(struct rq_object_mgr * object_mgr, const char *id, rq_object_t value);

/**
 * Garbage collect any unreferenced objects
 */
RQ_EXPORT void rq_object_mgr_garbage_collect(struct rq_object_mgr * object_mgr);

/**
 * Push a new symbol table onto the symbol table stack
 */
RQ_EXPORT void rq_object_mgr_symbol_table_push(struct rq_object_mgr * object_mgr);

/**
 * Pop the last symbol table from the symbol table stack
 */
RQ_EXPORT void rq_object_mgr_symbol_table_pop(struct rq_object_mgr * object_mgr);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
