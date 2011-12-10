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
 * Test liz_lookaside_stack.
 */


#include <liz/liz_lookaside_stack.h>

#include <cassert>
#include <climits>

#include <unittestpp.h>



SUITE(liz_lookaside_stack)
{
    TEST(empty_stack_make)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(2, 0);
        
        CHECK_EQUAL(0, liz_lookaside_stack_count(&stack));
        CHECK_EQUAL(2, liz_lookaside_stack_capacity(&stack));
        CHECK_EQUAL(true, liz_lookaside_stack_is_valid(&stack));
    }
    
    
    
    TEST(zero_capacity_stack_make)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(0, 0);
        
        CHECK_EQUAL(0, liz_lookaside_stack_count(&stack));
        CHECK_EQUAL(0, liz_lookaside_stack_capacity(&stack));
        CHECK_EQUAL(true, liz_lookaside_stack_is_valid(&stack));
    }
    
    
    
    TEST(empty_stack_count)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 0);
        
        CHECK_EQUAL(0, liz_lookaside_stack_count(&stack));
    }
    
    
    
    TEST(zero_capacity_stack_count)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(0, 0);
        
        CHECK_EQUAL(0, liz_lookaside_stack_count(&stack));
    }
    
    
    
    TEST(stack_count)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 1);
        
        CHECK_EQUAL(1, liz_lookaside_stack_count(&stack));
    }

    
    
    
    TEST(full_stack_count) 
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 4);
        
        CHECK_EQUAL(4, liz_lookaside_stack_count(&stack));
    }
    
    
    
    
    
    TEST(empty_stack_is_empty)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 0);
        
        CHECK_EQUAL(true, liz_lookaside_stack_is_empty(&stack));
    }
    
    
    TEST(zero_capacity_stack_is_not_empty)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(0, 0);
        
        CHECK_EQUAL(false, liz_lookaside_stack_is_empty(&stack));
    }
    
    
    TEST(non_empty_stack_is_not_empty)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 1);
        
        CHECK_EQUAL(false, liz_lookaside_stack_is_empty(&stack));
    }
    
    
    TEST(full_stack_is_not_empty)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 4);
        
        CHECK_EQUAL(false, liz_lookaside_stack_is_empty(&stack));
    }
    
    
    TEST(empty_stack_is_not_full) 
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 0);
        
        CHECK_EQUAL(false, liz_lookaside_stack_is_full(&stack));
    }
    
    
    TEST(zero_capacityu_stack_is_full) 
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(0, 0);
        
        CHECK_EQUAL(true, liz_lookaside_stack_is_full(&stack));
    }
    
    
    TEST(non_full_stack_is_not_full)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 3);
        
        CHECK_EQUAL(false, liz_lookaside_stack_is_full(&stack));
    }
        
    
    TEST(full_stack_is_full)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 4);
        
        CHECK_EQUAL(true, liz_lookaside_stack_is_full(&stack));
    }
    
    
    TEST(clear_empty_stack)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 0);
        
        liz_lookaside_stack_clear(&stack);
        
        CHECK_EQUAL(true, liz_lookaside_stack_is_empty(&stack));
        CHECK_EQUAL(4, liz_lookaside_stack_capacity(&stack));
    }
    
    
    TEST(clear_zero_capacity_stack)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(0, 0);
        
        liz_lookaside_stack_clear(&stack);
        
        CHECK_EQUAL(false, liz_lookaside_stack_is_empty(&stack));
        CHECK_EQUAL(0, liz_lookaside_stack_capacity(&stack));
    }
    
    
    TEST(clear_non_empty_stack)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 2);
        
        liz_lookaside_stack_clear(&stack);
        
        
        CHECK_EQUAL(true, liz_lookaside_stack_is_empty(&stack));
        CHECK_EQUAL(4, liz_lookaside_stack_capacity(&stack));
    }
    
    
    TEST(clear_full_stack)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 4);
        
        liz_lookaside_stack_clear(&stack);
        
        
        CHECK_EQUAL(true, liz_lookaside_stack_is_empty(&stack));
        CHECK_EQUAL(4, liz_lookaside_stack_capacity(&stack));
    }
    
    
    
    TEST(empty_stack_push)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 0);
        
        liz_lookaside_stack_push(&stack);
        
        CHECK_EQUAL(1, liz_lookaside_stack_count(&stack));
        CHECK_EQUAL(4, liz_lookaside_stack_capacity(&stack));
    }
    
    
    TEST(non_empty_stack_push)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 3);
        
        liz_lookaside_stack_push(&stack);
        
        CHECK_EQUAL(4, liz_lookaside_stack_count(&stack));
        CHECK_EQUAL(4, liz_lookaside_stack_capacity(&stack));
    }
    
    
    
    
    TEST(one_element_stack_pop)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 1);
        
        liz_lookaside_stack_pop(&stack);
        
        CHECK_EQUAL(0, liz_lookaside_stack_count(&stack));
        CHECK_EQUAL(4, liz_lookaside_stack_capacity(&stack));
    }
    
    
    TEST(full_stack_pop)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 4);
        
        liz_lookaside_stack_pop(&stack);
        
        CHECK_EQUAL(3, liz_lookaside_stack_count(&stack));
        CHECK_EQUAL(4, liz_lookaside_stack_capacity(&stack));
    }
        
    
    TEST(set_count)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 2);
        
        liz_lookaside_stack_set_count(&stack, 1);
        
        CHECK_EQUAL(1, liz_lookaside_stack_count(&stack));
        CHECK_EQUAL(4, liz_lookaside_stack_capacity(&stack));
    }
    
    /* It's undefined behavior to call top_index on an empty stack.
    TEST(zero_capacity_stack_top_index)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(0, 0);
        
        liz_int_t index = liz_lookaside_stack_top_index(&stack);
        
        CHECK_EQUAL(BT_LOOKASIDE_STACK_INDEX_INVALID, index);
    }
    
    
    TEST(empty_stack_top_index)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 0);
        
        liz_int_t index = liz_lookaside_stack_top_index(&stack);
        
        CHECK_EQUAL(BT_LOOKASIDE_STACK_INDEX_INVALID, index);
    }
    */
        
    TEST(few_elements_stack_top_index)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 2);
        
        liz_int_t index = liz_lookaside_stack_top_index(&stack);
        
        CHECK_EQUAL(1, index);
    }
    
    
    TEST(full_element_stack_top_index)
    {
        liz_lookaside_stack_t stack = liz_lookaside_stack_make(4, 4);
        
        liz_int_t index = liz_lookaside_stack_top_index(&stack);
        
        CHECK_EQUAL(3, index);
    }
    
} // SUITE(liz_traversal_leave_stack)


