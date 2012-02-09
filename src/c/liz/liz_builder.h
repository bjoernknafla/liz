/*
 * Copyright (c) 2011, Bjoern Knafla
 * http://www.bjoernknafla.com/
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are 
 * met:
 *
 *   * Redistributions of source code must retain the above copyright 
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright 
 *     notice, this list of conditions and the following disclaimer in the 
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Bjoern Knafla nor the names of its contributors may
 *     be used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *
 * Builds schemes alas blueprints of behavior trees.
 *
 * Aggregates behavior tree schemes, one at a time, node by node.
 *
 * A scheme can be extracted and added to another scheme in construction.
 * Extract the shape atoms, persistent state shape atom indices, and shape
 * specification from a finished scheme to create a shape, behavior tree 
 * entities alas actors, and a virtual machine (VM) capable of interpreting
 * actors of the shape.
 */

#ifndef LIZ_liz_builder_H
#define LIZ_liz_builder_H


#include <liz/liz_platform_macros.h>
#include <liz/liz_allocator.h>


#if defined(__cplusplus)
extern "C" {
#endif

    
    typedef struct liz_builder_allocator {
        void *user_data;
        liz_alloc_func_t alloc_func;
        liz_dealloc_func_t dealloc_func;
        
    } liz_builder_allocator_t;
    
    typedef struct liz_builder liz_builder_t;
    
    
    /**
     * Allocates and initializes a builder using the allocator context and
     * allocation functions which are also used to set the builder instance
     * work allocator, e.g., to dynamically manage memory while building a 
     * behavior tree shape.
     */
    liz_builder_t*
    liz_builder_create(void * LIZ_RESTRICT allocator_context,
                       liz_alloc_func_t alloc_func,
                       liz_dealloc_func_t dealloc_func);
    
    /**
     * Frees all managed memory of builder via its work allocator and then
     * finalizes and frees builder via dealloc_func and allocator_context.
     */
    void
    liz_builder_destroy(liz_builder_t *builder,
                        void * LIZ_RESTRICT allocator_context,
                        liz_dealloc_func_t dealloc_func);
    
    
    void
    liz_builder_set_scheme_allocator(liz_builder_t *builder,
                                     liz_builder_allocator_t allocator);
    
    
    liz_builder_allocator_t
    liz_builder_scheme_allocator(liz_builder_t const *builder);
    
    
    
#if defined(__cplusplus)
} // extern "C"
#endif

#endif // LIZ_liz_builder_H
