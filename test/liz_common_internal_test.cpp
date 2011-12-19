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


#include <unittestpp.h>

#include <liz/liz_common_internal.h>

#include <cassert>



SUITE(liz_common_internal_test)
{
    TEST(sort_decider_states_empty)
    {
        liz_int_t const state_count = 0;
        liz_int_t const stack_capacity = 0;
        
        uint16_t const expected_states[state_count] = {};
        uint16_t const expected_state_shape_atom_indices[state_count] = {};
        
        uint16_t states[state_count] = {};
        uint16_t state_shape_atom_indices[state_count] = {};
        
        uint16_t state_stack[stack_capacity] = {};
        uint16_t state_shape_atom_indices_stack[stack_capacity] = {};

        liz_sort_values_for_keys_from_post_order_traversal(states,
                                                           state_shape_atom_indices, 
                                                           sizeof(states[0]), 
                                                           state_count,
                                                           state_stack,
                                                           state_shape_atom_indices_stack,
                                                           stack_capacity);
        
        CHECK_ARRAY_EQUAL(expected_states, states, state_count);
        CHECK_ARRAY_EQUAL(expected_state_shape_atom_indices, state_shape_atom_indices, state_count);
    }
    
    
    
    TEST(sort_decider_states_single_element)
    {
        liz_int_t const state_count = 1;
        liz_int_t const stack_capacity = 0;
        
        uint16_t expected_states[state_count] = {42u};
        uint16_t expected_state_shape_atom_indices[state_count] = {7u};
        
        uint16_t states[state_count] = {42u};
        uint16_t state_shape_atom_indices[state_count] = {7u};
        
        uint16_t state_stack[stack_capacity] = {};
        uint16_t state_shape_atom_indices_stack[stack_capacity] = {};
        
        liz_sort_values_for_keys_from_post_order_traversal(states,
                                                           state_shape_atom_indices, 
                                                           sizeof(states[0]), 
                                                           state_count,
                                                           state_stack,
                                                           state_shape_atom_indices_stack,
                                                           stack_capacity);
        
        CHECK_ARRAY_EQUAL(expected_states, states, state_count);
        CHECK_ARRAY_EQUAL(expected_state_shape_atom_indices, state_shape_atom_indices, state_count);
    }
   
    
    
    TEST(sort_decider_states_already_ordered)
    {
        liz_int_t const state_count = 3;
        liz_int_t const stack_capacity = 1;
        
        uint16_t expected_states[state_count] = {5u, 4u, 3u};
        uint16_t expected_state_shape_atom_indices[state_count] = {0u, 1u, 2u};
        
        uint16_t states[state_count] = {5u, 4u, 3u};
        uint16_t state_shape_atom_indices[state_count] = {0u, 1u, 2u};
        
        uint16_t state_stack[stack_capacity] = {};
        uint16_t state_shape_atom_indices_stack[stack_capacity] = {};
        
        liz_sort_values_for_keys_from_post_order_traversal(states,
                                                           state_shape_atom_indices, 
                                                           sizeof(states[0]), 
                                                           state_count,
                                                           state_stack,
                                                           state_shape_atom_indices_stack,
                                                           stack_capacity);
        
        CHECK_ARRAY_EQUAL(expected_states, states, state_count);
        CHECK_ARRAY_EQUAL(expected_state_shape_atom_indices, state_shape_atom_indices, state_count);
    }
    
    
    
    TEST(sort_decider_states_single_sequence_unordered)
    {
        liz_int_t const state_count = 3;
        liz_int_t const stack_capacity = 2;
        
        uint16_t expected_states[state_count] = {103u, 102u, 101u};
        uint16_t expected_state_shape_atom_indices[state_count] = {7u, 23u, 42u};
        
        uint16_t states[state_count] = {101u, 102u, 103u};
        uint16_t state_shape_atom_indices[state_count] = {42u, 23u, 7u};
        
        uint16_t state_stack[stack_capacity] = {};
        uint16_t state_shape_atom_indices_stack[stack_capacity] = {};
        
        liz_sort_values_for_keys_from_post_order_traversal(states,
                                                           state_shape_atom_indices, 
                                                           sizeof(states[0]), 
                                                           state_count,
                                                           state_stack,
                                                           state_shape_atom_indices_stack,
                                                           stack_capacity);
        
        CHECK_ARRAY_EQUAL(expected_states, states, state_count);
        CHECK_ARRAY_EQUAL(expected_state_shape_atom_indices, state_shape_atom_indices, state_count);
    }
    
    
    
    TEST(sort_decider_states_single_sequence_unordered_without_first_key_as_last_element)
    {
        liz_int_t const state_count = 6;
        liz_int_t const stack_capacity = 3;
        
        uint16_t expected_states[state_count] = {105u, 104u, 103u, 102u, 101u, 100u};
        uint16_t expected_state_shape_atom_indices[state_count] = {1u, 2u, 3u, 4u, 6u, 7u};
        
        uint16_t states[state_count] = {105u, 104u, 102u, 103u, 100u, 101u};
        uint16_t state_shape_atom_indices[state_count] = {1u, 2u, 4u, 3u, 7u, 6u};
        
        uint16_t state_stack[stack_capacity] = {};
        uint16_t state_shape_atom_indices_stack[stack_capacity] = {};
        
        liz_sort_values_for_keys_from_post_order_traversal(states,
                                                           state_shape_atom_indices, 
                                                           sizeof(states[0]), 
                                                           state_count,
                                                           state_stack,
                                                           state_shape_atom_indices_stack,
                                                           stack_capacity);
        
        CHECK_ARRAY_EQUAL(expected_states, states, state_count);
        CHECK_ARRAY_EQUAL(expected_state_shape_atom_indices, state_shape_atom_indices, state_count);
    }
    
    
    
    TEST(sort_decider_states_sequences_in_sequence_unordered)
    {
        liz_int_t const state_count = 8;
        liz_int_t const stack_capacity = 3;
        
        uint16_t expected_states[state_count] = {107u, 106u, 105u, 104u, 103u, 102u, 101u, 100u};
        uint16_t expected_state_shape_atom_indices[state_count] = {0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u};
        
        uint16_t states[state_count] = {105u, 104u, 106u, 101u, 100u, 102u, 103u, 107u};
        uint16_t state_shape_atom_indices[state_count] = {2u, 3u, 1u, 6u, 7u, 5u, 4u, 0u};
        
        uint16_t state_stack[stack_capacity] = {};
        uint16_t state_shape_atom_indices_stack[stack_capacity] = {};
        
        liz_sort_values_for_keys_from_post_order_traversal(states,
                                                           state_shape_atom_indices, 
                                                           sizeof(states[0]), 
                                                           state_count,
                                                           state_stack,
                                                           state_shape_atom_indices_stack,
                                                           stack_capacity);
        
        CHECK_ARRAY_EQUAL(expected_states, states, state_count);
        CHECK_ARRAY_EQUAL(expected_state_shape_atom_indices, state_shape_atom_indices, state_count);
    }
    
    
    
} // SUITE(liz_common_internal_test)



