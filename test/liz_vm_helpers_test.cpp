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
 * Checks vm helper functions - mainly cancellation helpers.
 */

#include <unittestpp.h>

#include <cassert>
#include <vector>

#include <liz/liz_vm.h>

#include "liz_test_helpers.h"



SUITE(liz_vm_helpers_test)
{
    TEST(sort_decider_states_empty)
    {
        liz_int_t const state_count = 0;
        liz_int_t const stack_capacity = 0;
        std::size_t const state_alignment = 2;
        
        uint16_t const expected_states[state_count] = {};
        uint16_t const expected_state_shape_atom_indices[state_count] = {};
        
        uint16_t states[state_count] = {};
        uint16_t state_shape_atom_indices[state_count] = {};
        
        liz_vm_decider_guard_t stack_buffer[stack_capacity] = {};
        liz_lookaside_stack_t stack_header = liz_lookaside_stack_make(stack_capacity, 0);
        
        liz_vm_sort_values_for_keys_from_post_order_traversal(states,
                                                              state_shape_atom_indices, 
                                                              sizeof(states[0]),
                                                              state_alignment,
                                                              state_count,
                                                              stack_buffer,
                                                              &stack_header);
        
        CHECK_ARRAY_EQUAL(expected_states, states, state_count);
        CHECK_ARRAY_EQUAL(expected_state_shape_atom_indices, state_shape_atom_indices, state_count);
    }
    
    
    
    TEST(sort_decider_states_single_element)
    {
        liz_int_t const state_count = 1;
        liz_int_t const stack_capacity = 0;
        std::size_t const state_alignment = 2;
        
        uint16_t expected_states[state_count] = {42u};
        uint16_t expected_state_shape_atom_indices[state_count] = {7u};
        
        uint16_t states[state_count] = {42u};
        uint16_t state_shape_atom_indices[state_count] = {7u};
        
        liz_vm_decider_guard_t stack_buffer[stack_capacity] = {};
        liz_lookaside_stack_t stack_header = liz_lookaside_stack_make(stack_capacity, 0);
        
        liz_vm_sort_values_for_keys_from_post_order_traversal(states,
                                                              state_shape_atom_indices, 
                                                              sizeof(states[0]),
                                                              state_alignment,
                                                              state_count,
                                                              stack_buffer,
                                                              &stack_header);
        
        CHECK_ARRAY_EQUAL(expected_states, states, state_count);
        CHECK_ARRAY_EQUAL(expected_state_shape_atom_indices, state_shape_atom_indices, state_count);
    }
    
    
    
    TEST(sort_decider_states_already_ordered)
    {
        liz_int_t const state_count = 3;
        liz_int_t const stack_capacity = 1;
        std::size_t const state_alignment = 2;
        
        uint16_t expected_states[state_count] = {5u, 4u, 3u};
        uint16_t expected_state_shape_atom_indices[state_count] = {0u, 1u, 2u};
        
        uint16_t states[state_count] = {5u, 4u, 3u};
        uint16_t state_shape_atom_indices[state_count] = {0u, 1u, 2u};
        
        liz_vm_decider_guard_t stack_buffer[stack_capacity] = {};
        liz_lookaside_stack_t stack_header = liz_lookaside_stack_make(stack_capacity, 0);
        
        liz_vm_sort_values_for_keys_from_post_order_traversal(states,
                                                              state_shape_atom_indices, 
                                                              sizeof(states[0]),
                                                              state_alignment,
                                                              state_count,
                                                              stack_buffer,
                                                              &stack_header);
        
        CHECK_ARRAY_EQUAL(expected_states, states, state_count);
        CHECK_ARRAY_EQUAL(expected_state_shape_atom_indices, state_shape_atom_indices, state_count);
    }
    
    
    
    TEST(sort_decider_states_single_sequence_unordered)
    {
        liz_int_t const state_count = 3;
        liz_int_t const stack_capacity = 2;
        std::size_t const state_alignment = 2;
        
        uint16_t expected_states[state_count] = {103u, 102u, 101u};
        uint16_t expected_state_shape_atom_indices[state_count] = {7u, 23u, 42u};
        
        uint16_t states[state_count] = {101u, 102u, 103u};
        uint16_t state_shape_atom_indices[state_count] = {42u, 23u, 7u};
        
        liz_vm_decider_guard_t stack_buffer[stack_capacity] = {};
        liz_lookaside_stack_t stack_header = liz_lookaside_stack_make(stack_capacity, 0);
        
        liz_vm_sort_values_for_keys_from_post_order_traversal(states,
                                                              state_shape_atom_indices, 
                                                              sizeof(states[0]),
                                                              state_alignment,
                                                              state_count,
                                                              stack_buffer,
                                                              &stack_header);
        
        CHECK_ARRAY_EQUAL(expected_states, states, state_count);
        CHECK_ARRAY_EQUAL(expected_state_shape_atom_indices, state_shape_atom_indices, state_count);
    }
    
    
    
    TEST(sort_decider_states_single_sequence_unordered_without_first_key_as_last_element)
    {
        liz_int_t const state_count = 6;
        liz_int_t const stack_capacity = 3;
        std::size_t const state_alignment = 2;
        
        uint16_t expected_states[state_count] = {105u, 104u, 103u, 102u, 101u, 100u};
        uint16_t expected_state_shape_atom_indices[state_count] = {1u, 2u, 3u, 4u, 6u, 7u};
        
        uint16_t states[state_count] = {105u, 104u, 102u, 103u, 100u, 101u};
        uint16_t state_shape_atom_indices[state_count] = {1u, 2u, 4u, 3u, 7u, 6u};
        
        liz_vm_decider_guard_t stack_buffer[stack_capacity] = {};
        liz_lookaside_stack_t stack_header = liz_lookaside_stack_make(stack_capacity, 0);
        
        liz_vm_sort_values_for_keys_from_post_order_traversal(states,
                                                              state_shape_atom_indices, 
                                                              sizeof(states[0]),
                                                              state_alignment,
                                                              state_count,
                                                              stack_buffer,
                                                              &stack_header);
        
        CHECK_ARRAY_EQUAL(expected_states, states, state_count);
        CHECK_ARRAY_EQUAL(expected_state_shape_atom_indices, state_shape_atom_indices, state_count);
    }
    
    
    
    TEST(sort_decider_states_sequences_in_sequence_unordered)
    {
        liz_int_t const state_count = 8;
        liz_int_t const stack_capacity = 3;
        std::size_t const state_alignment = 2;
        
        uint16_t expected_states[state_count] = {107u, 106u, 105u, 104u, 103u, 102u, 101u, 100u};
        uint16_t expected_state_shape_atom_indices[state_count] = {0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u};
        
        uint16_t states[state_count] = {105u, 104u, 106u, 101u, 100u, 102u, 103u, 107u};
        uint16_t state_shape_atom_indices[state_count] = {2u, 3u, 1u, 6u, 7u, 5u, 4u, 0u};
        
        liz_vm_decider_guard_t stack_buffer[stack_capacity] = {};
        liz_lookaside_stack_t stack_header = liz_lookaside_stack_make(stack_capacity, 0);
        
        liz_vm_sort_values_for_keys_from_post_order_traversal(states,
                                                              state_shape_atom_indices, 
                                                              sizeof(states[0]),
                                                              state_alignment,
                                                              state_count,
                                                              stack_buffer,
                                                              &stack_header);
        
        CHECK_ARRAY_EQUAL(expected_states, states, state_count);
        CHECK_ARRAY_EQUAL(expected_state_shape_atom_indices, state_shape_atom_indices, state_count);
    }
    
    
    TEST(replace_an_empty_cancellation_range_with_another_empty_range) 
    {
        liz_vm_cancellation_range_t range = {0u, 0u};
        
        liz_vm_cancellation_range_adapt(&range, 7u, 7u);
        
        CHECK_EQUAL(7u, range.begin_index);
        CHECK_EQUAL(7u, range.end_index);
    }
    
    
    
    TEST(do_not_empty_cancellation_range)
    {
        liz_vm_cancellation_range_t range = {0u, 1u};
        
        liz_vm_cancellation_range_adapt(&range, 7u, 7u);
        
        CHECK_EQUAL(0u, range.begin_index);
        CHECK_EQUAL(1u, range.end_index);
    }
    
    
    
    TEST(grow_cancellation_range_to_the_left)
    {
        liz_vm_cancellation_range_t range = {3u, 5u};
        
        liz_vm_cancellation_range_adapt(&range, 4u, 7u);
        
        CHECK_EQUAL(3u, range.begin_index);
        CHECK_EQUAL(7u, range.end_index);
    }
    
    
    
    TEST(grow_cancellation_range_to_the_right)
    {
        liz_vm_cancellation_range_t range = {3u, 5u};
        
        liz_vm_cancellation_range_adapt(&range, 2u, 5u);
        
        CHECK_EQUAL(2u, range.begin_index);
        CHECK_EQUAL(5u, range.end_index);
    }
    
    
    
    TEST(grow_cancellation_range)
    {
        liz_vm_cancellation_range_t range = {3u, 5u};
        
        liz_vm_cancellation_range_adapt(&range, 1u, 7u);
        
        CHECK_EQUAL(1u, range.begin_index);
        CHECK_EQUAL(7u, range.end_index);
    }
    
    
    
    TEST(do_not_grow_cancellation_range) 
    {
        liz_vm_cancellation_range_t range = {3u, 4u};
        
        liz_vm_cancellation_range_adapt(&range, 3u, 3u);
        
        CHECK_EQUAL(3u, range.begin_index);
        CHECK_EQUAL(4u, range.end_index);
    }
    
  
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_current_update_launching_deferred_action)
    {
        push_shape_deferred_action(42 /* Action id */, 
                                   7 /* Resource id */);
        
        create_expected_result_and_proband_vms_for_shape();
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        0,
                                        1);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        0,
                                        1);
        
        push_vm_action_state(target_select_proband,
                             0 /* Shape atom index. */,
                             liz_execution_state_launch);
        
        liz_vm_cancel_running_actions_from_current_update(proband_vm,
                                                          NULL,
                                                          proband_blackboard, 
                                                          0 /* liz_time_t */, 
                                                          &proband_actor,
                                                          &shape);
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
    }
    
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_current_update_running_deferred_action)
    {
        push_shape_deferred_action(42 /* Action id */, 
                                   7 /* Resource id */);
        
        create_expected_result_and_proband_vms_for_shape();
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        0,
                                        1);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        0,
                                        1);
        
        push_vm_action_state(target_select_proband,
                             0 /* Shape atom index. */,
                             liz_execution_state_running);
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42 /* Action id */,
                                      7 /* Resrouce id */,
                                      0 /* Shape atom index */);
        
        liz_vm_cancel_running_actions_from_current_update(proband_vm,
                                                          NULL,
                                                          proband_blackboard, 
                                                          0 /* liz_time_t */, 
                                                          &proband_actor,
                                                          &shape);
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
    }
    
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_current_update_running_immediate_action)
    {
        push_shape_immediate_action(immediate_action_func_index_identity1);
        
        create_expected_result_and_proband_vms_for_shape();
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        0,
                                        1);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        0,
                                        1);
        
        push_vm_action_state(target_select_proband,
                             0 /* Shape atom index */,
                             liz_execution_state_running);
        
        expected_result_blackboard[immediate_action_func_index_identity1] = liz_execution_state_cancel;
        
        liz_vm_cancel_running_actions_from_current_update(proband_vm,
                                                          NULL,
                                                          proband_blackboard, 
                                                          0 /* liz_time_t */, 
                                                          &proband_actor,
                                                          &shape);
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
    }
    
    
    /* Commented out because running 
     * liz_vm_cancel_running_actions_from_current_update with an empty
     * cancellation range is not allowed.
     TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_current_update_empty_range)
     {
     push_shape_concurrent_decider(4 // Shape stream with 4 atoms.
     );
     push_shape_immediate_action(immediate_action_func_index_identity0);
     push_shape_deferred_action(42, // action_id
     7 // resource_id
     );
     
     create_expected_result_and_proband_vms_for_shape();
     
     liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
     0,
     0);
     liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
     0,
     0);
     
     
     push_vm_action_state(target_select_both, 
     1, // shape atom index
     liz_execution_state_running);
     push_vm_action_state(target_select_both, 
     2, // shape atom index
     liz_execution_state_running);
     
     expected_result_blackboard[immediate_action_func_index_identity0] = liz_execution_state_running;
     proband_blackboard[immediate_action_func_index_identity0] = liz_execution_state_running;
     
     liz_vm_cancel_running_actions_from_current_update(proband_vm,
     NULL,
     proband_blackboard, 
     0, // liz_time_t 
     &proband_actor,
     &shape);
     
     CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
     CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
     }
     */
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_current_update_no_states)
    {
        push_shape_concurrent_decider(4 // Shape stream with 4 atoms.
                                      );
        push_shape_immediate_action(immediate_action_func_index_identity0);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   ); // Adds two shape atoms.
        
        create_expected_result_and_proband_vms_for_shape();
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        0,
                                        4);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        0,
                                        4);
        
        /*
        push_vm_action_state(target_select_both, 
                             1, // shape atom index
                             liz_execution_state_running);
        push_vm_action_state(target_select_both, 
                             2, // shape atom index
                             liz_execution_state_running);
        */
        
        expected_result_blackboard[immediate_action_func_index_identity0] = liz_execution_state_running;
        proband_blackboard[immediate_action_func_index_identity0] = liz_execution_state_running;
        
        liz_vm_cancel_running_actions_from_current_update(proband_vm,
                                                          NULL,
                                                          proband_blackboard, 
                                                          0, // liz_time_t 
                                                          &proband_actor,
                                                          &shape);
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
    }
    
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_current_update_all_states)
    {
        push_shape_concurrent_decider(4 // Shape stream with 4 atoms.
                                      );
        push_shape_immediate_action(immediate_action_func_index_identity0);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   ); // Adds two shape atoms.
        
        create_expected_result_and_proband_vms_for_shape();
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        0,
                                        4);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        0,
                                        4);
        
        push_vm_action_state(target_select_proband, 
                             1, // shape atom index
                             liz_execution_state_running);
        push_vm_action_state(target_select_proband, 
                             2, // shape atom index
                             liz_execution_state_running);
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42, // action id
                                      7, // resource id
                                      2 // shape atom index
                                      );
        
        expected_result_blackboard[immediate_action_func_index_identity0] = liz_execution_state_cancel;
        proband_blackboard[immediate_action_func_index_identity0] = liz_execution_state_running;
        
        liz_vm_cancel_running_actions_from_current_update(proband_vm,
                                                          NULL,
                                                          proband_blackboard, 
                                                          0, // liz_time_t 
                                                          &proband_actor,
                                                          &shape);
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
    }
    
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_current_update_few_states)
    {
        push_shape_concurrent_decider(4 // Shape stream with 4 atoms.
                                      );
        push_shape_immediate_action(immediate_action_func_index_identity1);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   ); // Adds two shape atoms.
        
        create_expected_result_and_proband_vms_for_shape();
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        0,
                                        4);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        0,
                                        4);
        
        push_vm_action_state(target_select_proband, 
                             2, // shape atom index
                             liz_execution_state_running);
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42, // action id
                                      7, // resource id
                                      2 // shape atom index
                                      );
        
        liz_vm_cancel_running_actions_from_current_update(proband_vm,
                                                          NULL,
                                                          proband_blackboard, 
                                                          0, // liz_time_t 
                                                          &proband_actor,
                                                          &shape);
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, monitor_cancel_running_actions_from_current_update_few_states)
    {
        push_shape_concurrent_decider(4 // Shape stream with 4 atoms.
                                      );
        push_shape_immediate_action(immediate_action_func_index_identity1);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   ); // Adds two shape atoms.
        
        create_expected_result_and_proband_vms_for_shape();
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        2,
                                        4);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        2,
                                        4);
        
        push_vm_action_state(target_select_both, 
                             1, // shape atom index
                             liz_execution_state_running);
        
        push_vm_action_state(target_select_proband, 
                             2, // shape atom index
                             liz_execution_state_running);
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42, // action_id
                                      7, // resource_id
                                      2 //  shape_atom_index
                                      );
        
        
        liz_vm_monitor_log expected_log;
        
#if defined(LIZ_VM_MONITOR_ENABLE)
        expected_log.push_back((liz_vm_monitor_log_entry) {
            2,
            liz_vm_monitor_node_flag_enter_from_top | liz_vm_monitor_node_flag_cancel_action,
            proband_vm,
            proband_blackboard,
            0, // liz_time_t
            &proband_actor,
            &shape
        });
        expected_log.push_back((liz_vm_monitor_log_entry) {
            2,
            liz_vm_monitor_node_flag_leave_to_top | liz_vm_monitor_node_flag_cancel_action,
            proband_vm,
            proband_blackboard,
            0, // liz_time_t
            &proband_actor,
            &shape
        });
#endif
        
        liz_vm_monitor_log proband_log;
        
        liz_vm_monitor_t monitor = {
            reinterpret_cast<uintptr_t>(&proband_log),
            monitor_test_func
        };
        
        liz_vm_cancel_running_actions_from_current_update(proband_vm,
                                                          &monitor,
                                                          proband_blackboard, 
                                                          0, // liz_time_t 
                                                          &proband_actor,
                                                          &shape);
        
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
        CHECK_EQUAL(expected_log, proband_log);
    }
    
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_previous_update_launching_deferred_action)
    {
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        push_actor_action_state(target_select_proband,
                                0, // shape_atom_index
                                liz_execution_state_launch);
        
        create_expected_result_and_proband_vms_for_shape();
        
        
        expected_result_vm->actor_action_state_index = 1;
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42, // action_id
                                      7, // resource_id
                                      0 // shape_atom_index
                                      );
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        0,
                                        1);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        0,
                                        1);
        
        
        liz_vm_cancel_launched_and_running_actions_from_previous_update(proband_vm,
                                                                        NULL,
                                                                        proband_blackboard, 
                                                                        0, // liz_time_t 
                                                                        &proband_actor,
                                                                        &shape);
        
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
        
    }
    
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_previous_update_running_deferred_action)
    {
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        push_actor_action_state(target_select_proband,
                                0, // shape_atom_index
                                liz_execution_state_running);
        
        create_expected_result_and_proband_vms_for_shape();
        
        
        expected_result_vm->actor_action_state_index = 1;
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42, // action_id
                                      7, // resource_id
                                      0 // shape_atom_index
                                      );
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        0,
                                        1);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        0,
                                        1);
        
        
        liz_vm_cancel_launched_and_running_actions_from_previous_update(proband_vm,
                                                                        NULL,
                                                                        proband_blackboard, 
                                                                        0, // liz_time_t 
                                                                        &proband_actor,
                                                                        &shape);
        
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
    }
    
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_previous_update_running_immediate_action)
    {
        push_shape_immediate_action(immediate_action_func_index_identity1);
        
        push_actor_action_state(target_select_proband,
                                0, // shape_atom_index
                                liz_execution_state_running);
        
        create_expected_result_and_proband_vms_for_shape();
        
        
        expected_result_vm->actor_action_state_index = 1;
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        0,
                                        1);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        0,
                                        1);
        
        expected_result_blackboard[immediate_action_func_index_identity1] = liz_execution_state_cancel;
        
        
        liz_vm_cancel_launched_and_running_actions_from_previous_update(proband_vm,
                                                                        NULL,
                                                                        proband_blackboard, 
                                                                        0, // liz_time_t 
                                                                        &proband_actor,
                                                                        &shape);
        
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
    }
    
    
    /**
     * Calling liz_vm_cancel_launched_and_running_actions_from_previous_update
     * with an empty cancellation range results in undefined behavior.
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_previous_update_empty_range)
    {
        CHECK(false);
    }
    */
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_previous_update_no_states)
    {
        push_shape_concurrent_decider(4 // Shape stream with 4 atoms.
                                      );
        push_shape_immediate_action(immediate_action_func_index_identity0);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   ); // Adds two shape atoms.
        
        create_expected_result_and_proband_vms_for_shape();
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        0,
                                        4);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        0,
                                        4);
        
        expected_result_blackboard[immediate_action_func_index_identity0] = liz_execution_state_running;
        proband_blackboard[immediate_action_func_index_identity0] = liz_execution_state_running;
        
        liz_vm_cancel_launched_and_running_actions_from_previous_update(proband_vm,
                                                                        NULL,
                                                                        proband_blackboard, 
                                                                        0, // liz_time_t 
                                                                        &proband_actor,
                                                                        &shape);
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
    }
    
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_previous_update_no_states_to_index_left)
    {
        push_shape_concurrent_decider(4 // Shape stream with 4 atoms.
                                      );
        push_shape_immediate_action(immediate_action_func_index_identity0);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   ); // Adds two shape atoms.
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(target_select_proband,
                                1, // shape atom index
                                liz_execution_state_running);
        
        expected_result_vm->actor_action_state_index = 1;
        proband_vm->actor_action_state_index = 1;
        
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        0,
                                        4);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        0,
                                        4);
        
        expected_result_blackboard[immediate_action_func_index_identity0] = liz_execution_state_running;
        proband_blackboard[immediate_action_func_index_identity0] = liz_execution_state_running;
        
        liz_vm_cancel_launched_and_running_actions_from_previous_update(proband_vm,
                                                                        NULL,
                                                                        proband_blackboard, 
                                                                        0, // liz_time_t 
                                                                        &proband_actor,
                                                                        &shape);
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
    }
    
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_previous_update_all_states)
    {
        push_shape_concurrent_decider(4 // Shape stream with 4 atoms.
                                      );
        push_shape_immediate_action(immediate_action_func_index_identity1);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   ); // Adds two shape atoms.
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(target_select_proband,
                                1, // shape_atom_index
                                liz_execution_state_running);
        push_actor_action_state(target_select_proband,
                                2, // shape_atom_index
                                liz_execution_state_running);
        
        expected_result_vm->actor_action_state_index = 2;
        
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        0,
                                        4);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        0,
                                        4);
        
        expected_result_blackboard[immediate_action_func_index_identity1] = liz_execution_state_cancel;
        proband_blackboard[immediate_action_func_index_identity1] = liz_execution_state_running;
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42, // action_id
                                      7, // resource_id
                                      2 // shape_atom_index
                                      );
        
        liz_vm_cancel_launched_and_running_actions_from_previous_update(proband_vm,
                                                                        NULL,
                                                                        proband_blackboard, 
                                                                        0, // liz_time_t 
                                                                        &proband_actor,
                                                                        &shape);
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
    }
    
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_previous_update_few_states_to_index_left)
    {
        push_shape_concurrent_decider(4 // Shape stream with 4 atoms.
                                      );
        push_shape_immediate_action(immediate_action_func_index_identity1);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   ); // Adds two shape atoms.
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(target_select_proband,
                                1, // shape_atom_index
                                liz_execution_state_running);
        push_actor_action_state(target_select_proband,
                                2, // shape_atom_index
                                liz_execution_state_running);
        
        expected_result_vm->actor_action_state_index = 2;
        proband_vm->actor_action_state_index = 1;
        
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        0,
                                        4);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        0,
                                        4);
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42, // action_id
                                      7, // resource_id
                                      2 // shape_atom_index
                                      );
        
        liz_vm_cancel_launched_and_running_actions_from_previous_update(proband_vm,
                                                                        NULL,
                                                                        proband_blackboard, 
                                                                        0, // liz_time_t 
                                                                        &proband_actor,
                                                                        &shape);
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
    }
    
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_actions_from_previous_update_few_states_in_range)
    {
        push_shape_concurrent_decider(4 // Shape stream with 4 atoms.
                                      );
        push_shape_immediate_action(immediate_action_func_index_identity1);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   ); // Adds two shape atoms.
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(target_select_proband,
                                1, // shape_atom_index
                                liz_execution_state_running);
        push_actor_action_state(target_select_proband,
                                2, // shape_atom_index
                                liz_execution_state_running);
        
        expected_result_vm->actor_action_state_index = 1;
        
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        0,
                                        2);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        0,
                                        2);
        
        expected_result_blackboard[immediate_action_func_index_identity1] = liz_execution_state_cancel;
        proband_blackboard[immediate_action_func_index_identity1] = liz_execution_state_running;
        
        
        liz_vm_cancel_launched_and_running_actions_from_previous_update(proband_vm,
                                                                        NULL,
                                                                        proband_blackboard, 
                                                                        0, // liz_time_t 
                                                                        &proband_actor,
                                                                        &shape);
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);   
    }
    
    
    
    TEST_FIXTURE(liz_vm_test_fixture, monitor_cancel_running_actions_from_previous_update_few_states)
    {
        push_shape_concurrent_decider(4 // Shape stream with 4 atoms.
                                      );
        push_shape_immediate_action(immediate_action_func_index_identity1);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   ); // Adds two shape atoms.
        
        create_expected_result_and_proband_vms_for_shape();
        
        liz_vm_cancellation_range_adapt(&expected_result_vm->cancellation_range,
                                        2,
                                        4);
        liz_vm_cancellation_range_adapt(&proband_vm->cancellation_range,
                                        2,
                                        4);
        
        push_actor_action_state(target_select_proband,
                                1, // shape atom index
                                liz_execution_state_running);
        
        push_actor_action_state(target_select_proband,
                                2, // shape atom index
                                liz_execution_state_running);
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42, // action_id
                                      7, // resource_id
                                      2 //  shape_atom_index
                                      );
        
        
        expected_result_vm->actor_action_state_index = 2;

        push_monitor_log_entry(target_select_expected_result, 
                               2, 
                               liz_vm_monitor_node_flag_enter_from_top | liz_vm_monitor_node_flag_cancel_action, 
                               0);
        push_monitor_log_entry(target_select_expected_result, 
                               2, 
                               liz_vm_monitor_node_flag_leave_to_top | liz_vm_monitor_node_flag_cancel_action, 
                               0);
        
        
        
        liz_vm_cancel_launched_and_running_actions_from_previous_update(proband_vm,
                                                                        monitor,
                                                                        proband_blackboard, 
                                                                        0, // liz_time_t 
                                                                        &proband_actor,
                                                                        &shape);
        
        
        CHECK_EQUAL(expected_result_vm_comparator, proband_vm_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, proband_blackboard, shape_immediate_action_function_count);
        CHECK_EQUAL(expected_result_monitor_log, proband_monitor_log);
    }
}
