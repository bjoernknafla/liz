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
 * Lookaside stack with two sides, low and high, to push to or pop from, sharing 
 * the stack's capacity.
 *
 * Lookaside means that it's the users job to provide and work on the collection 
 * of elements of the full stack while the lookaside structure is more of a 
 * header for bookkeeping.
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


#ifndef BT_liz_lookaside_double_stack_H
#define BT_liz_lookaside_double_stack_H


#include <liz/liz_platform_types.h>
#include <liz/liz_platform_macros.h>
#include <liz/liz_assert.h>


#if defined(__cplusplus)
extern "C" {
#endif
    
    
    
#define LIZ_LOOKASIDE_DOUBLE_STACK_CAPACITY_MAX UINT16_MAX
    
    
    
    typedef struct liz_lookaside_double_stack_s {
        uint16_t capacity;
        uint16_t count_low;
        uint16_t count_high;
    } liz_lookaside_double_stack_t;
    
    
    
    typedef enum liz_lookaside_double_stack_side {
        liz_lookaside_double_stack_side_low,
        liz_lookaside_double_stack_side_high
    } liz_lookaside_double_stack_side_t;
    
    
    
    LIZ_INLINE static
    liz_lookaside_double_stack_t
    liz_lookaside_double_stack_make(liz_int_t capacity,
                                    liz_int_t low_side_element_count,
                                    liz_int_t high_side_element_count)
    {
        LIZ_ASSERT(0 <= capacity);
        LIZ_ASSERT(LIZ_LOOKASIDE_DOUBLE_STACK_CAPACITY_MAX >= capacity);
        LIZ_ASSERT(low_side_element_count + high_side_element_count <= capacity);
     
        return (liz_lookaside_double_stack_t){
            (uint16_t)capacity,
            (uint16_t)low_side_element_count,
            (uint16_t)high_side_element_count
        };
    }
    
    
    
    LIZ_INLINE static
    bool
    liz_lookaside_double_stack_is_valid(liz_lookaside_double_stack_t const *stack)
    {
        liz_int_t const count_low = stack->count_low;
        liz_int_t const count_high = stack->count_high;
        liz_int_t const capacity = stack->capacity;
        
        if (count_low > capacity
            || count_high > capacity
            || count_low + count_high > capacity) {
            
            return false;
        }
        
        return true;
    }
    
    
    
    /* Commented out because semantics are irritating in use and unsure if 
     * just testing for zero count isn't irritating for a zero capacity stack.
    LIZ_INLINE static
    bool
    liz_lookaside_double_stack_is_empty(liz_lookaside_double_stack_t const *stack)
    {
        return (0u == stack->count_low) && (0u == stack->count_high) && (0u < stack->capacity);
    }
    */
    
    
    
    LIZ_INLINE static
    bool
    liz_lookaside_double_stack_is_full(liz_lookaside_double_stack_t const *stack)
    {
        return stack->capacity == stack->count_low + stack->count_high;
    }
    
    
    LIZ_INLINE static
    liz_int_t
    liz_lookaside_double_stack_capacity(liz_lookaside_double_stack_t const *stack)
    {
        return stack->capacity;
    }
    
    
    LIZ_INLINE static
    liz_int_t
    liz_lookaside_double_stack_count_all(liz_lookaside_double_stack_t const *stack)
    {
        return stack->count_low + stack->count_high;
    }
    
    
    LIZ_INLINE static
    liz_int_t
    liz_lookaside_double_stack_count_low(liz_lookaside_double_stack_t const *stack)
    {
        return stack->count_low;
    }

    
    
    LIZ_INLINE static
    liz_int_t
    liz_lookaside_double_stack_count_high(liz_lookaside_double_stack_t const *stack)
    {
        return stack->count_high;
    }

    
    
    LIZ_INLINE static
    liz_int_t
    liz_lookaside_double_stack_count(liz_lookaside_double_stack_t const *stack,
                                     liz_lookaside_double_stack_side_t side)
    {
        liz_int_t count;
        
        switch (side) {
            case liz_lookaside_double_stack_side_low:
                count = liz_lookaside_double_stack_count_low(stack);
                break;
            case liz_lookaside_double_stack_side_high:
                count = liz_lookaside_double_stack_count_high(stack);
                break;
            default:
                LIZ_ASSERT(0 && "Invalid side value.");
                count = stack->count_low + stack->count_high;
                break;
        }
        
        return count;
    }
    
    
    /**
     * Ensure that new_low_count isn't negative, or overlaps the stack's 
     * high part, or grows the count so uninitialized elements are included,
     * otherwise behavior is undefined.
     */
    LIZ_INLINE static
    void
    liz_lookaside_double_stack_set_count_low(liz_lookaside_double_stack_t *stack,
                                             liz_int_t new_count_low)
    {
        LIZ_ASSERT(0 <= new_count_low);
        LIZ_ASSERT(new_count_low + stack->count_high <= stack->capacity);
        
        stack->count_low = new_count_low;
    }
    
    
    /**
     * Ensure that new_high_count isn't negative, or overlaps the stack's 
     * low part, or grows the count so uninitialized elements are included,
     * otherwise behavior is undefined.
     */
    LIZ_INLINE static
    void
    liz_lookaside_double_stack_set_count_high(liz_lookaside_double_stack_t *stack,
                                              liz_int_t new_count_high)
    {
        LIZ_ASSERT(0 <= new_count_high);
        LIZ_ASSERT(new_count_high + stack->count_low <= stack->capacity);
        
        stack->count_high = new_count_high;
    }
    
    
    /**
     * Ensure that new_count isn't negative, or overlaps the stack's 
     * opposite part, or grows the count so uninitialized elements are included,
     * otherwise behavior is undefined.
     */
    LIZ_INLINE static
    void
    liz_lookaside_double_stack_set_count(liz_lookaside_double_stack_t *stack,
                                         liz_int_t new_count,
                                         liz_lookaside_double_stack_side_t side)
    {
        switch (side) {
            case liz_lookaside_double_stack_side_low:
                liz_lookaside_double_stack_set_count_low(stack, new_count);
                break;
            case liz_lookaside_double_stack_side_high:
                liz_lookaside_double_stack_set_count_high(stack, new_count);
                break;
            default:
                LIZ_ASSERT(0 && "Invalid side value.");
                break;
        }
    }

    
    
    
    LIZ_INLINE static
    void
    liz_lookaside_double_stack_clear(liz_lookaside_double_stack_t *stack)
    {
        stack->count_low = 0u;
        stack->count_high = 0u;
    }

    
    
    /**
     * Ensure that the stack isn't full before pushing elements onto it, 
     * otherwise behavior is undefined.
     */
    LIZ_INLINE static
    void
    liz_lookaside_double_stack_push_low(liz_lookaside_double_stack_t *stack)
    {
        LIZ_ASSERT(!liz_lookaside_double_stack_is_full(stack));
        
        stack->count_low += 1u;
    }
    
    
    /**
     * Ensure that the stack isn't full before pushing elements onto it, 
     * otherwise behavior is undefined.
     */
    LIZ_INLINE static
    void
    liz_lookaside_double_stack_push_high(liz_lookaside_double_stack_t *stack)
    {
        LIZ_ASSERT(!liz_lookaside_double_stack_is_full(stack));
        
        stack->count_high += 1u;    
    }
    
    /**
     * Ensure that the stack isn't full before pushing elements onto it, 
     * otherwise behavior is undefined.
     */
    LIZ_INLINE static
    void
    liz_lookaside_double_stack_push(liz_lookaside_double_stack_t *stack,
                                    liz_lookaside_double_stack_side_t side)
    {     
        switch (side) {
            case liz_lookaside_double_stack_side_low:
                liz_lookaside_double_stack_push_low(stack);
                break;
            case liz_lookaside_double_stack_side_high:
                liz_lookaside_double_stack_push_high(stack);
                break;
            default:
                LIZ_ASSERT(0 && "Invalid side value.");
                break;
        }
    }

    
    
    /**
     * Ensure that the stack's low part isn't empty before popping elements from
     * it, otherwise behavior is undefined.
     *
     * TODO: @todo Decide if to return a value (flag or count) if more elements
     *             are pop-able to enable use of just pop in a loop instead
     *             of needing pop and and emptiness check.
     */
    LIZ_INLINE static
    void
    liz_lookaside_double_stack_pop_low(liz_lookaside_double_stack_t *stack)
    {
        LIZ_ASSERT(0u != liz_lookaside_double_stack_count_low(stack));
        
        stack->count_low -= 1u;
    }

    
    
    /**
     * Ensure that the stack's high part isn't empty before popping elements
     * from it, otherwise behavior is undefined.
     *
     * TODO: @todo Decide if to return a value (flag or count) if more elements
     *             are pop-able to enable use of just pop in a loop instead
     *             of needing pop and and emptiness check.
     */
    LIZ_INLINE static
    void
    liz_lookaside_double_stack_pop_high(liz_lookaside_double_stack_t *stack)
    {
        LIZ_ASSERT(0u != liz_lookaside_double_stack_count_high(stack));
        
        stack->count_high -= 1u;
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
    liz_lookaside_double_stack_pop(liz_lookaside_double_stack_t *stack,
                                   liz_lookaside_double_stack_side_t side)
    {
        switch (side) {
            case liz_lookaside_double_stack_side_low:
                liz_lookaside_double_stack_pop_low(stack);
                break;
            case liz_lookaside_double_stack_side_high:
                liz_lookaside_double_stack_pop_high(stack);
                break;
            default:
                LIZ_ASSERT(0 && "Invalid side value.");
                break;
        }
    }
    
    
    
    /**
     * Returns the index of the top element on the low part of the stack.
     *
     * Ensure that the low stack isn't empty, otherwise behavior is undefined.
     */
    LIZ_INLINE static
    liz_int_t
    liz_lookaside_double_stack_top_index_low(liz_lookaside_double_stack_t const *stack)
    {
        LIZ_ASSERT(0 != liz_lookaside_double_stack_count_low(stack));
        
        return stack->count_low - 1u;
    }
    
    /**
     * Returns the index of the top element on the high part of the stack.
     *
     * Ensure that the high stack isn't empty, otherwise behavior is undefined.
     */
    LIZ_INLINE static
    liz_int_t
    liz_lookaside_double_stack_top_index_high(liz_lookaside_double_stack_t const *stack)
    {
        LIZ_ASSERT(0 != liz_lookaside_double_stack_count_high(stack));
        
        return stack->capacity - stack->count_high;
    }
    
    
    /**
     * Calls liz_lookaside_double_stack_top_index_low or 
     * liz_lookaside_double_stack_top_index_high based on the value of side.
     */
    LIZ_INLINE static
    liz_int_t
    liz_lookaside_double_stack_top_index(liz_lookaside_double_stack_t const *stack,
                                         liz_lookaside_double_stack_side_t const side)
    {
        liz_int_t result = stack->capacity;
        
        switch (side) {
            case liz_lookaside_double_stack_side_low:
                result = liz_lookaside_double_stack_top_index_low(stack);
                break;
            case liz_lookaside_double_stack_side_high:
                result = liz_lookaside_double_stack_top_index_high(stack);
                break;
            default:
                LIZ_ASSERT(0 && "Invalid side value.");
                break;
        }
        
        return result;
    }

    
    
#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* BT_liz_lookaside_double_stack_H */
