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
 * Lookaside stack, lookaside means that it's the users job to provide and work 
 * on the collection of elements of the full stack while the lookaside structure 
 * is a header for bookkeeping.
 *
 * Minimizing return codes based on the article 
 * "Managing Decoupling Part 4 — The ID Lookup Table" by of Niklas Frykholm and
 * the accompanying discussions:
 * http://altdevblogaday.com/2011/09/23/managing-decoupling-part-4-the-id-lookup-table/
 * http://bitsquid.blogspot.com/2011/09/managing-decoupling-part-4-id-lookup.html
 *
 * It is written under the assumption that you should know that a call is
 * valid before calling. Know because you know the data or because you call an
 * empty or full check before (possibly in an assertion).
 * In debug builds triggering undefined behavior triggers assertions.
 */


#ifndef LIZ_liz_lookaside_stack_H
#define LIZ_liz_lookaside_stack_H


#include <liz/liz_platform_types.h>
#include <liz/liz_platform_macros.h>
#include <liz/liz_assert.h>


#if defined(__cplusplus)
extern "C" {
#endif
    
    
#define LIZ_LOOKASIDE_STACK_CAPACITY_MAX UINT16_MAX
        
    
    /**
     * Treat as opaque.
     * Implementation might and will change without warning.
     */
    typedef struct liz_lookaside_stack {
        uint16_t capacity;
        uint16_t count;
    } liz_lookaside_stack_t;
    
    
    
    
    
    LIZ_INLINE static
    liz_lookaside_stack_t 
    liz_lookaside_stack_make(liz_int_t capacity,
                             liz_int_t count)
    {
        LIZ_ASSERT(0 <= capacity);
        LIZ_ASSERT(count <=capacity);
        LIZ_ASSERT(capacity <= LIZ_LOOKASIDE_STACK_CAPACITY_MAX);
        
        return (liz_lookaside_stack_t){capacity, count};
    }
    
    
    
    LIZ_INLINE static
    bool
    liz_lookaside_stack_is_valid(liz_lookaside_stack_t const *stack)
    {
        return (stack->count >= 0 
                && stack->count <= stack->capacity);
    }

    
    
    /* Commented out because semantics are irritating in use and unsure if 
     * just testing for zero count isn't irritating for a zero capacity stack.
    LIZ_INLINE static
    bool
    liz_lookaside_stack_is_empty(liz_lookaside_stack_t const *stack)
    {
        return (0u == stack->count) && (0u < stack->capacity);
    }
    */
    
    
    
    LIZ_INLINE static
    bool
    liz_lookaside_stack_is_full(liz_lookaside_stack_t const *stack)
    {
        return stack->capacity == stack->count;
    }
    
    
    LIZ_INLINE static
    liz_int_t
    liz_lookaside_stack_capacity(liz_lookaside_stack_t const *stack)
    {
        return stack->capacity;
    }
    
    
    LIZ_INLINE static
    liz_int_t
    liz_lookaside_stack_count(liz_lookaside_stack_t const *stack)
    {
        return stack->count;
    }
    
    
    /**
     * Ensure that new_count isn't negative, or greater than capacity, or grows
     * the count so uninitialized elements are included, otherwise behavior is 
     * undefined.
     */
    LIZ_INLINE static
    void
    liz_lookaside_stack_set_count(liz_lookaside_stack_t *stack,
                                  liz_int_t new_count)
    {
        LIZ_ASSERT(0 <= new_count);
        LIZ_ASSERT(new_count <= stack->capacity);
        
        stack->count = new_count;
    }
    
    
    LIZ_INLINE static
    void
    liz_lookaside_stack_clear(liz_lookaside_stack_t *stack)
    {
        stack->count = 0u;
    }
    
    
    /**
     * Ensure that the stack isn't full before pushing elements onto it, 
     * otherwise behavior is undefined.
     */
    LIZ_INLINE static
    void
    liz_lookaside_stack_push(liz_lookaside_stack_t *stack)
    {
        LIZ_ASSERT(false == liz_lookaside_stack_is_full(stack));
        
        stack->count += 1u;
    }
    
    
    /**
     * Ensure that the stack isn't empty before popping elements from it, 
     * otherwise behavior is undefined.
     *
     * TODO: @todo Decide if to return a value (flag or count) if more elements
     *             are pop-able to enable use of just pop in a loop instead
     *             of needing pop and and emptiness check.
     */
    LIZ_INLINE static
    void
    liz_lookaside_stack_pop(liz_lookaside_stack_t *stack)
    {
        LIZ_ASSERT(0 != liz_lookaside_stack_count(stack));
        
        stack->count -= 1u;
    }
    
    
    
    /**
     * Returns the index of the top element of the stack.
     *
     * Ensure that the stack isn't empty, otherwise behavior is undefined.
     */
    LIZ_INLINE static
    liz_int_t
    liz_lookaside_stack_top_index(liz_lookaside_stack_t const *stack)
    {
        LIZ_ASSERT(0 != liz_lookaside_stack_count(stack));
        
        return ((liz_int_t)stack->count) - 1u;
    }
    
    
#if defined(__cplusplus)
} /* extern "C" */
#endif


#endif /* LIZ_liz_lookaside_stack_H */
