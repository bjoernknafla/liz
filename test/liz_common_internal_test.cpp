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

#include <liz/liz_common.h>
#include <liz/liz_common_internal.h>

#include <cassert>

#include "liz_test_helpers.h"



SUITE(liz_common_internal_test)
{
    TEST(merge_lesser_with_greater_shape_specification)
    {
        liz_shape_specification_t lesser = {
            1, 2, 3, 4, 5, 6, 7, 8
        };
        liz_shape_specification_t greater = {
            2, 3, 4, 5, 6, 7, 8, 9
        };
        
        liz_shape_specification_t const expected_result_spec = greater;
        
        liz_shape_specification_t const proband_spec = liz_shape_specification_merge_max(lesser, greater);
        
        CHECK_EQUAL(expected_result_spec, proband_spec);
    }
    
    
    TEST(merge_two_shape_specifications)
    {
        liz_shape_specification_t lesser = {
            1, 2, 3, 4, 5, 6, 7, 8
        };
        liz_shape_specification_t greater = {
            1, 3, 2, 8, 0, 7, 4, 9
        };
        
        liz_shape_specification_t const expected_result_spec = {
            1, 3, 3, 8, 5, 7, 7, 9
        };
        
        liz_shape_specification_t const proband_spec = liz_shape_specification_merge_max(lesser, greater);
        
        CHECK_EQUAL(expected_result_spec, proband_spec);
    }
    
    
    TEST(apply_no_persistent_state_changes_to_no_persistent_states)
    {
        // Just called to ensure that it does not crash.
        liz_apply_persistent_state_changes(NULL, NULL, 0, NULL, NULL, 0);
    }
    
    
    namespace {
        
        class apply_persistent_state_changes_fixture {
        public:
            
            apply_persistent_state_changes_fixture()
            :   shape_atoms()
            ,   expected_states()
            ,   expected_state_shape_atom_indices()
            ,   proband_states()
            ,   proband_state_shape_atom_indices()
            ,   change_count(0)
            ,   state_changes()
            ,   state_change_shape_atom_indices()
            ,   expected_result_comparator()
            ,   proband_comparator()
            {
                {
                    liz_int_t shape_atom_index = 0;
                    liz_shape_atom_stream_add_concurrent_decider(shape_atoms, &shape_atom_index, state_count + 1, state_count + 1);
                    for (liz_int_t i = 0; i < state_count; ++i) {
                        liz_shape_atom_stream_add_persistent_action(shape_atoms, &shape_atom_index, state_count + 1);
                    }
                }
                
                init_persistent_states(expected_states,
                                       expected_state_shape_atom_indices,
                                       state_count,
                                       1, // start index
                                       liz_execution_state_launch);
                
                init_persistent_states(proband_states,
                                       proband_state_shape_atom_indices,
                                       state_count,
                                       1, // start index
                                       liz_execution_state_launch);
                
                expected_result_comparator.set(expected_states,
                                               expected_state_shape_atom_indices,
                                               state_count,
                                               shape_atoms,
                                               state_count + 1);
                
                proband_comparator.set(proband_states,
                                       proband_state_shape_atom_indices,
                                       state_count,
                                       shape_atoms,
                                       state_count + 1);
                
            }
            
            
            virtual ~apply_persistent_state_changes_fixture()
            {
                // Nothing to do.
            }
            
            
            void set_expected_result_persistent_action_state(uint16_t const persistent_state_index,
                                                             uint16_t const shape_atom_index,
                                                             liz_execution_state_t const exec_state)
            {
                assert(persistent_state_index < state_count);
                assert(0 != shape_atom_index);
                assert(state_count >= shape_atom_index);
                
                expected_states[persistent_state_index].persistent_action.state = static_cast<uint8_t>(exec_state);
                expected_state_shape_atom_indices[persistent_state_index] = shape_atom_index;
            }
            
            
            void push_persistent_action_state_change(uint16_t const shape_atom_index,
                                                     liz_execution_state const exec_state)
            {
                assert(0 < shape_atom_index);
                assert(state_count >= shape_atom_index);
                assert(change_count < state_count);
                
                state_changes[change_count].persistent_action.state = static_cast<uint8_t>(exec_state);
                state_change_shape_atom_indices[change_count] = shape_atom_index;
                
                ++change_count;
            }
            
            
            static liz_int_t const state_count = 3;
            
            liz_shape_atom_t shape_atoms[state_count + 1];
            
            liz_persistent_state_t expected_states[state_count];
            uint16_t expected_state_shape_atom_indices[state_count];
            
            liz_persistent_state_t proband_states[state_count];
            uint16_t proband_state_shape_atom_indices[state_count];
            
            
            liz_int_t change_count;
            liz_persistent_state_t state_changes[state_count];
            uint16_t state_change_shape_atom_indices[state_count];
            
            liz_persistent_state_comparator expected_result_comparator;
            liz_persistent_state_comparator proband_comparator;
            
        private:
            apply_persistent_state_changes_fixture(apply_persistent_state_changes_fixture const&); // = 0
            apply_persistent_state_changes_fixture& operator=(apply_persistent_state_changes_fixture const&); // = 0
            
            
            void init_persistent_states(liz_persistent_state_t *states, 
                                        uint16_t *shape_atom_indices, 
                                        liz_int_t const count, 
                                        uint16_t const start_index, 
                                        liz_execution_state_t const init_state)
            {
                for (liz_int_t i = 0; i < count; ++i) {
                    states[i].persistent_action.state = static_cast<uint8_t>(init_state);
                    shape_atom_indices[i] = start_index + i;
                }
            }
        };
        
        liz_int_t const apply_persistent_state_changes_fixture::state_count;
        
        
    } // anonymous namespace
    
    
    
    TEST_FIXTURE(apply_persistent_state_changes_fixture, apply_empty_persistent_state_changes)
    {
        liz_apply_persistent_state_changes(proband_states,
                                           proband_state_shape_atom_indices, 
                                           state_count,
                                           state_changes,
                                           state_change_shape_atom_indices, 
                                           change_count);
        
        CHECK_EQUAL(expected_result_comparator, proband_comparator);
    }
    
    
    TEST_FIXTURE(apply_persistent_state_changes_fixture, apply_one_persistent_state_change)
    {
        set_expected_result_persistent_action_state(1, 2, liz_execution_state_success);
        push_persistent_action_state_change(2, liz_execution_state_success);
        
        liz_apply_persistent_state_changes(proband_states,
                                           proband_state_shape_atom_indices, 
                                           state_count,
                                           state_changes,
                                           state_change_shape_atom_indices, 
                                           change_count);
        
        CHECK_EQUAL(expected_result_comparator, proband_comparator);
    }
    
    
    TEST_FIXTURE(apply_persistent_state_changes_fixture, apply_persistent_state_changes_to_all_states)
    {
        set_expected_result_persistent_action_state(0, 1, liz_execution_state_running);
        set_expected_result_persistent_action_state(1, 2, liz_execution_state_success);
        set_expected_result_persistent_action_state(2, 3, liz_execution_state_fail);
        
        push_persistent_action_state_change(1, liz_execution_state_running);
        push_persistent_action_state_change(2, liz_execution_state_success);
        push_persistent_action_state_change(3, liz_execution_state_fail);
        
        liz_apply_persistent_state_changes(proband_states,
                                           proband_state_shape_atom_indices, 
                                           state_count,
                                           state_changes,
                                           state_change_shape_atom_indices, 
                                           change_count);
        
        CHECK_EQUAL(expected_result_comparator, proband_comparator);
    }
    
} // SUITE(liz_common_internal_test)



