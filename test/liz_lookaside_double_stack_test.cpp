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
 *
 */


#include <liz/liz_lookaside_double_stack.h>

#include <cassert>
#include <climits>

#include <unittestpp.h>



SUITE(liz_lookaside_double_stack_test)
{
    TEST(make_empty_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(2, 0, 0);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(2, liz_lookaside_double_stack_capacity(&stack));
        CHECK_EQUAL(true, liz_lookaside_double_stack_is_valid(&stack));
    }
    
    
    
    TEST(make_zero_capacity_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(0, 0, 0);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_capacity(&stack));
        CHECK_EQUAL(true, liz_lookaside_double_stack_is_valid(&stack));
    }
    
    
    
    TEST(count_all_empty_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 0);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_all(&stack));
    }
    
    
    
    TEST(count_all_zero_capacity_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(0, 0, 0);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_all(&stack));
    }
    
    
    
    TEST(count_all_low_entries_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 0);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_all(&stack));
    }
    
    
    
    TEST(count_all_high_entries_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 3);
        
        CHECK_EQUAL(3, liz_lookaside_double_stack_count_all(&stack));
    }
    
    
    
    TEST(count_all_low_and_high_entries_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 1);
        
        CHECK_EQUAL(3, liz_lookaside_double_stack_count_all(&stack));
    }
    
    
    TEST(count_all_full_stack) 
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 3, 1);
        
        CHECK_EQUAL(4, liz_lookaside_double_stack_count_all(&stack));
    }
    
    
    
    
    
    
    TEST(count_low_empty_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 0);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
    }
    
    
    
    TEST(count_low_zero_capacity_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(0, 0, 0);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
    }
    
    
    
    TEST(count_low_low_entries_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 0);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_low(&stack));
    }
    
    
    
    TEST(count_low_high_entries_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 3);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
    }
    
    
    
    TEST(count_low_low_and_high_entries_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 1);
        
        CHECK_EQUAL(2, liz_lookaside_double_stack_count_low(&stack));
    }
    
    
    TEST(count_low_full_stack) 
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 3, 1);
        
        CHECK_EQUAL(3, liz_lookaside_double_stack_count_low(&stack));
    }
    
    
    
    
    
    TEST(count_high_empty_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 0);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
    }
    
    
    
    TEST(count_high_zero_capacity_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(0, 0, 0);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
    }
    
    
    
    TEST(count_high_low_entries_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 0);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
    }
    
    
    
    TEST(count_high_high_entries_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 3);
        
        CHECK_EQUAL(3, liz_lookaside_double_stack_count_high(&stack));
    }
    
    
    
    TEST(count_high_low_and_high_entries_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 1);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_high(&stack));
    }
    
    
    TEST(count_high_full_stack) 
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 3, 1);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_high(&stack));
    }
    
    
    
    
    
    TEST(count_empty_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 0);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count(&stack, liz_lookaside_double_stack_side_low));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count(&stack, liz_lookaside_double_stack_side_high));
    }
    
    
    
    TEST(count_zero_capacity_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(0, 0, 0);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count(&stack, liz_lookaside_double_stack_side_low));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count(&stack, liz_lookaside_double_stack_side_high));
    }
    
    
    
    TEST(count_low_entries_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 0);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count(&stack, liz_lookaside_double_stack_side_low));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count(&stack, liz_lookaside_double_stack_side_high));
    }
    
    
    
    TEST(count_high_entries_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 3);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count(&stack, liz_lookaside_double_stack_side_low));
        CHECK_EQUAL(3, liz_lookaside_double_stack_count(&stack, liz_lookaside_double_stack_side_high));
    }
    
    
    
    TEST(count_low_and_high_entries_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 1);
        
        CHECK_EQUAL(2, liz_lookaside_double_stack_count(&stack, liz_lookaside_double_stack_side_low));
        CHECK_EQUAL(1, liz_lookaside_double_stack_count(&stack, liz_lookaside_double_stack_side_high));
    }
    
    
    TEST(count_full_stack) 
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 3, 1);
        
        CHECK_EQUAL(3, liz_lookaside_double_stack_count(&stack, liz_lookaside_double_stack_side_low));
        CHECK_EQUAL(1, liz_lookaside_double_stack_count(&stack, liz_lookaside_double_stack_side_high));
    }
    
    
    
    TEST(empty_stack_is_empty)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 0);
        
        CHECK_EQUAL(true, liz_lookaside_double_stack_is_empty(&stack));
    }
    
    
    TEST(zero_capacity_stack_is_not_empty)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(0, 0, 0);
        
        CHECK_EQUAL(false, liz_lookaside_double_stack_is_empty(&stack));
    }
    
    
    TEST(low_non_empty_stack_is_not_empty)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 0);
        
        CHECK_EQUAL(false, liz_lookaside_double_stack_is_empty(&stack));
    }
    
    
    TEST(high_non_empty_stack_is_not_empty)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 2);
        
        CHECK_EQUAL(false, liz_lookaside_double_stack_is_empty(&stack));
    }
    
    
    TEST(low_and_high_non_empty_stack_is_not_empty)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 1);
        
        CHECK_EQUAL(false, liz_lookaside_double_stack_is_empty(&stack));
    }
    
    
    TEST(full_stack_is_not_empty)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 2);
        
        CHECK_EQUAL(false, liz_lookaside_double_stack_is_empty(&stack));
    }
    
    
    TEST(empty_stack_is_not_full) 
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 0);
        
        CHECK_EQUAL(false, liz_lookaside_double_stack_is_full(&stack));
    }
    
    
    TEST(zero_capacityu_stack_is_full) 
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(0, 0, 0);
        
        CHECK_EQUAL(true, liz_lookaside_double_stack_is_full(&stack));
    }
    
    
    TEST(low_non_full_stack_is_not_full)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 3, 0);
        
        CHECK_EQUAL(false, liz_lookaside_double_stack_is_full(&stack));
    }
    
    
    TEST(high_non_full_stack_is_not_full)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 2);
        
        CHECK_EQUAL(false, liz_lookaside_double_stack_is_full(&stack));
    }
    
    
    TEST(low_and_high_non_full_stack_is_not_full)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 1);
        
        CHECK_EQUAL(false, liz_lookaside_double_stack_is_full(&stack));
    }
    
    
    TEST(low_full_stack_is_full)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 4);
        
        CHECK_EQUAL(true, liz_lookaside_double_stack_is_full(&stack));
    }
    
    
    
    TEST(high_full_stack_is_full)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 4);
        
        CHECK_EQUAL(true, liz_lookaside_double_stack_is_full(&stack));
    }
    
    
    TEST(low_and_high_full_stack_is_full)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 2);
        
        CHECK_EQUAL(true, liz_lookaside_double_stack_is_full(&stack));
    }
    
    
    TEST(clear_empty_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 0);
        
        liz_lookaside_double_stack_clear(&stack);
        
        CHECK_EQUAL(true, liz_lookaside_double_stack_is_empty(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(clear_zero_capacity_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(0, 0, 0);
        
        liz_lookaside_double_stack_clear(&stack);
        
        CHECK_EQUAL(false, liz_lookaside_double_stack_is_empty(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(clear_non_empty_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 1);
        
        liz_lookaside_double_stack_clear(&stack);
        
        
        CHECK_EQUAL(true, liz_lookaside_double_stack_is_empty(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(clear_full_stack)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 2);
        
        liz_lookaside_double_stack_clear(&stack);
        
        
        CHECK_EQUAL(true, liz_lookaside_double_stack_is_empty(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    
    TEST(empty_stack_push_low)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 0);
        
        liz_lookaside_double_stack_push_low(&stack);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(non_empty_low_stack_push_low)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 3, 0);
        
        liz_lookaside_double_stack_push_low(&stack);
        
        CHECK_EQUAL(4, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(non_empty_high_stack_push_low)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 3);
        
        liz_lookaside_double_stack_push_low(&stack);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(3, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(non_empty_stack_push_low)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 1);
        
        liz_lookaside_double_stack_push_low(&stack);
        
        CHECK_EQUAL(2, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    
    
    
    TEST(empty_stack_push_high)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 0);
        
        liz_lookaside_double_stack_push_high(&stack);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(non_empty_low_stack_push_high)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 3, 0);
        
        liz_lookaside_double_stack_push_high(&stack);
        
        CHECK_EQUAL(3, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(non_empty_high_stack_push_high)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 3);
        
        liz_lookaside_double_stack_push_high(&stack);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(non_empty_stack_push_high)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 1);
        
        liz_lookaside_double_stack_push_high(&stack);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(2, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(empty_stack_push_low_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 0);
        
        liz_lookaside_double_stack_push(&stack, liz_lookaside_double_stack_side_low);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(non_empty_low_stack_push_low_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 3, 0);
        
        liz_lookaside_double_stack_push(&stack, liz_lookaside_double_stack_side_low);
        
        CHECK_EQUAL(4, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(non_empty_high_stack_push_low_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 3);
        
        liz_lookaside_double_stack_push(&stack, liz_lookaside_double_stack_side_low);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(3, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(non_empty_stack_push_low_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 1);
        
        liz_lookaside_double_stack_push(&stack, liz_lookaside_double_stack_side_low);
        
        CHECK_EQUAL(2, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    
    TEST(empty_stack_push_high_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 0);
        
        liz_lookaside_double_stack_push(&stack, liz_lookaside_double_stack_side_high);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(non_empty_low_stack_push_high_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 3, 0);
        
        liz_lookaside_double_stack_push(&stack, liz_lookaside_double_stack_side_high);
        
        CHECK_EQUAL(3, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(non_empty_high_stack_push_high_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 3);
        
        liz_lookaside_double_stack_push(&stack, liz_lookaside_double_stack_side_high);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(non_empty_stack_push_high_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 1);
        
        liz_lookaside_double_stack_push(&stack, liz_lookaside_double_stack_side_high);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(2, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(one_low_element_stack_pop_low)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 0);
        
        liz_lookaside_double_stack_pop_low(&stack);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(full_low_element_stack_pop_low)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 4, 0);
        
        liz_lookaside_double_stack_pop_low(&stack);
        
        CHECK_EQUAL(3, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(one_low_element_a_few_high_elements_pop_low)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 2);
        
        liz_lookaside_double_stack_pop_low(&stack);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(2, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(full_stack_pop_low)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 3, 1);
        
        liz_lookaside_double_stack_pop_low(&stack);
        
        CHECK_EQUAL(2, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    
    
    TEST(one_high_element_stack_pop_high)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 1);
        
        liz_lookaside_double_stack_pop_high(&stack);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(full_high_element_stack_pop_high)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 4);
        
        liz_lookaside_double_stack_pop_high(&stack);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(3, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(few_low_element_one_high_elements_pop_high)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 1);
        
        liz_lookaside_double_stack_pop_high(&stack);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(full_stack_pop_high)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 2);
        
        liz_lookaside_double_stack_pop_high(&stack);
        
        CHECK_EQUAL(2, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    
    
    
    
    TEST(one_low_element_stack_pop_low_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 0);
        
        liz_lookaside_double_stack_pop(&stack, liz_lookaside_double_stack_side_low);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(full_low_element_stack_pop_low_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 4, 0);
        
        liz_lookaside_double_stack_pop(&stack, liz_lookaside_double_stack_side_low);
        
        CHECK_EQUAL(3, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(one_low_element_a_few_high_elements_pop_low_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 2);
        
        liz_lookaside_double_stack_pop(&stack, liz_lookaside_double_stack_side_low);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(2, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(full_stack_pop_low_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 3, 1);
        
        liz_lookaside_double_stack_pop(&stack, liz_lookaside_double_stack_side_low);
        
        CHECK_EQUAL(2, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    
    
    TEST(one_high_element_stack_pop_high_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 1);
        
        liz_lookaside_double_stack_pop(&stack, liz_lookaside_double_stack_side_high);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(full_high_element_stack_pop_high_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 4);
        
        liz_lookaside_double_stack_pop(&stack, liz_lookaside_double_stack_side_high);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(3, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(few_low_element_one_high_elements_pop_high_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 1);
        
        liz_lookaside_double_stack_pop(&stack, liz_lookaside_double_stack_side_high);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(full_stack_pop_high_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 2);
        
        liz_lookaside_double_stack_pop(&stack, liz_lookaside_double_stack_side_high);
        
        CHECK_EQUAL(2, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    
    TEST(set_count_low)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 2);
        
        liz_lookaside_double_stack_set_count_low(&stack,
                                                 1);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(2, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(set_count_hight)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 2);
        
        liz_lookaside_double_stack_set_count_high(&stack,
                                                  0);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(set_count_low_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 2);
        
        liz_lookaside_double_stack_set_count(&stack,
                                             0,
                                             liz_lookaside_double_stack_side_low);
        
        CHECK_EQUAL(0, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(2, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    
    TEST(set_count_high_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 2);
        
        liz_lookaside_double_stack_set_count(&stack,
                                             1,
                                             liz_lookaside_double_stack_side_high);
        
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_low(&stack));
        CHECK_EQUAL(1, liz_lookaside_double_stack_count_high(&stack));
        CHECK_EQUAL(4, liz_lookaside_double_stack_capacity(&stack));
    }
    
    /* It's undefined behavior to call top_index on an empty stack.
    TEST(zero_capacity_stack_top_index_low)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(0, 0, 0);
        
        liz_int_t index = liz_lookaside_double_stack_top_index_low(&stack);
        
        CHECK_EQUAL(LIZ_LOOKASIDE_DOUBLE_STACK_INDEX_INVALID, index);
    }
    
    
    TEST(empty_stack_top_index_low)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 0);
        
        liz_int_t index = liz_lookaside_double_stack_top_index_low(&stack);
        
        CHECK_EQUAL(LIZ_LOOKASIDE_DOUBLE_STACK_INDEX_INVALID, index);
    }
    
    
    TEST(empty_low_part_stack_top_index_low)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 1);
        
        liz_int_t index = liz_lookaside_double_stack_top_index_low(&stack);
        
        CHECK_EQUAL(LIZ_LOOKASIDE_DOUBLE_STACK_INDEX_INVALID, index);
    }
    */
    
    TEST(few_low_elements_stack_top_index_low)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 0);
        
        liz_int_t index = liz_lookaside_double_stack_top_index_low(&stack);
        
        CHECK_EQUAL(1, index);
    }
    
    
    TEST(full_low_element_stack_top_index_low)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 4, 0);
        
        liz_int_t index = liz_lookaside_double_stack_top_index_low(&stack);
        
        CHECK_EQUAL(3, index);
    }
    
    
    TEST(stack_top_index_low)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 3);
        
        liz_int_t index = liz_lookaside_double_stack_top_index_low(&stack);
        
        CHECK_EQUAL(0, index);
    }
    
    
    
    /* It's undefined behavior to call top_index on an empty stack.
    TEST(zero_capacity_stack_top_index_high)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(0, 0, 0);
        
        liz_int_t index = liz_lookaside_double_stack_top_index_high(&stack);
        
        CHECK_EQUAL(LIZ_LOOKASIDE_DOUBLE_STACK_INDEX_INVALID, index);
    }
    
    
    TEST(empty_stack_top_index_high)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 4);
        
        liz_int_t index = liz_lookaside_double_stack_top_index_high(&stack);
        
        CHECK_EQUAL(LIZ_LOOKASIDE_DOUBLE_STACK_INDEX_INVALID, index);
    }
    
    
    TEST(empty_high_part_stack_top_index_high)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 0);
        
        liz_int_t index = liz_lookaside_double_stack_top_index_high(&stack);
        
        CHECK_EQUAL(LIZ_LOOKASIDE_DOUBLE_STACK_INDEX_INVALID, index);
    }
    */
    
    TEST(few_high_elements_stack_top_index_high)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 3);
        
        liz_int_t index = liz_lookaside_double_stack_top_index_high(&stack);
        
        CHECK_EQUAL(1, index);
    }
    
    
    TEST(full_high_element_stack_top_index_high)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 4);
        
        liz_int_t index = liz_lookaside_double_stack_top_index_high(&stack);
        
        CHECK_EQUAL(0, index);
    }
    
    
    TEST(stack_top_index_high)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 3);
        
        liz_int_t index = liz_lookaside_double_stack_top_index_high(&stack);
        
        CHECK_EQUAL(1, index);
    }
    
    
    /* It's undefined behavior to call top_index on an empty stack.
    TEST(zero_capacity_stack_top_index_low_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(0, 0, 0);
        
        liz_int_t index = liz_lookaside_double_stack_top_index(&stack,
                                                               liz_lookaside_double_stack_side_low);
        
        CHECK_EQUAL(LIZ_LOOKASIDE_DOUBLE_STACK_INDEX_INVALID, index);
    }
    
    
    TEST(empty_stack_top_index_low_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 0);
        
        liz_int_t index = liz_lookaside_double_stack_top_index(&stack,
                                                               liz_lookaside_double_stack_side_low);
        
        CHECK_EQUAL(LIZ_LOOKASIDE_DOUBLE_STACK_INDEX_INVALID, index);
    }
    
    
    TEST(empty_low_part_stack_top_index_low_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 1);
        
        liz_int_t index = liz_lookaside_double_stack_top_index(&stack,
                                                               liz_lookaside_double_stack_side_low);
        
        CHECK_EQUAL(LIZ_LOOKASIDE_DOUBLE_STACK_INDEX_INVALID, index);
    }
    */
    
    TEST(few_low_elements_stack_top_index_low_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 2, 0);
        
        liz_int_t index = liz_lookaside_double_stack_top_index(&stack,
                                                               liz_lookaside_double_stack_side_low);
        
        CHECK_EQUAL(1, index);
    }
    
    
    TEST(full_low_element_stack_top_index_low_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 4, 0);
        
        liz_int_t index = liz_lookaside_double_stack_top_index(&stack,
                                                               liz_lookaside_double_stack_side_low);
        
        CHECK_EQUAL(3, index);
    }
    
    
    TEST(stack_top_index_low_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 3);
        
        liz_int_t index = liz_lookaside_double_stack_top_index(&stack,
                                                               liz_lookaside_double_stack_side_low);
        
        CHECK_EQUAL(0, index);
    }
    
    
    
    /* It's undefined behavior to call top_index on an empty stack.
    TEST(zero_capacity_stack_top_index_high_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(0, 0, 0);
        
        liz_int_t index = liz_lookaside_double_stack_top_index(&stack,
                                                               liz_lookaside_double_stack_side_high);
        
        CHECK_EQUAL(LIZ_LOOKASIDE_DOUBLE_STACK_INDEX_INVALID, index);
    }
    
    
    TEST(empty_stack_top_index_high_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 0);
        
        liz_int_t index = liz_lookaside_double_stack_top_index(&stack,
                                                               liz_lookaside_double_stack_side_high);
        
        CHECK_EQUAL(LIZ_LOOKASIDE_DOUBLE_STACK_INDEX_INVALID, index);
    }
    
    
    TEST(empty_highw_part_stack_top_index_high_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 0);
        
        liz_int_t index = liz_lookaside_double_stack_top_index(&stack,
                                                               liz_lookaside_double_stack_side_high);
        
        CHECK_EQUAL(LIZ_LOOKASIDE_DOUBLE_STACK_INDEX_INVALID, index);
    }
    */
    
    TEST(few_high_elements_stack_top_index_high_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 3);
        
        liz_int_t index = liz_lookaside_double_stack_top_index(&stack,
                                                               liz_lookaside_double_stack_side_high);
        
        CHECK_EQUAL(1, index);
    }
    
    
    TEST(full_high_element_stack_top_index_high_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 0, 4);
        
        liz_int_t index = liz_lookaside_double_stack_top_index(&stack,
                                                               liz_lookaside_double_stack_side_high);
        
        CHECK_EQUAL(0, index);
    }
    
    
    TEST(stack_top_index_high_case)
    {
        liz_lookaside_double_stack_t stack = liz_lookaside_double_stack_make(4, 1, 3);
        
        liz_int_t index = liz_lookaside_double_stack_top_index(&stack,
                                                               liz_lookaside_double_stack_side_high);
        
        CHECK_EQUAL(1, index);
    }
} // SUITE(liz_lookaside_double_stack_test)

