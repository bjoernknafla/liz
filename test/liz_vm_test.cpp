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
 * Checks vm functions.
 */

#include <unittestpp.h>

#include <cassert>
#include <vector>

#include <liz/liz_common.h>
#include <liz/liz_common_internal.h>
#include <liz/liz_vm.h>

#include "liz_test_helpers.h"



SUITE(liz_vm_test)
{
    TEST(create_and_detroy_vm)
    {
        counting_allocator allocator;
        
        liz_shape_specification_t vm_spec = {
            2,
            1,
            0,
            1,
            1,
            0,
            1,
            0
        };        
        liz_vm_t *vm = liz_vm_create(vm_spec,
                                     &allocator,
                                     counting_alloc);
        
        CHECK(NULL != vm);
        
        liz_vm_destroy(vm,
                       &allocator,
                       counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    TEST(vm_does_not_fulfill_shape_requirements)
    {
        counting_allocator allocator;
        
        liz_shape_specification_t vm_spec = {
            2,
            1,
            0,
            1,
            1,
            0,
            1,
            0
        };
        
        liz_shape_specification_t too_large_spec = {
            2,
            0,
            0,
            1,
            1,
            0,
            1,
            1
        };
        
        liz_vm_t *vm = liz_vm_create(vm_spec,
                                     &allocator,
                                     counting_alloc);
        
        
        CHECK(false == liz_vm_fulfills_shape_specification(vm, too_large_spec));
        
        
        liz_vm_destroy(vm,
                       &allocator,
                       counting_dealloc);
    }
    
    TEST(vm_fulfills_shape_requirements)
    {
        counting_allocator allocator;
        
        liz_shape_specification_t vm_spec = {
            3,
            1,
            0,
            1,
            2,
            0,
            1,
            1
        };
        
        liz_shape_specification_t lesser_spec = {
            2,
            0,
            0,
            1,
            1,
            0,
            1,
            1
        };
        
        liz_vm_t *vm = liz_vm_create(vm_spec,
                                     &allocator,
                                     counting_alloc);
        
        
        CHECK(liz_vm_fulfills_shape_specification(vm, lesser_spec));
        
        
        liz_vm_destroy(vm,
                       &allocator,
                       counting_dealloc);
    }
    
    
    namespace {
        
        
        liz_vm_monitor_t *monitor_null = NULL;
        void* user_data_lookup_context_null = NULL;
        liz_time_t const update_time_zero = 0;
        
        
        
        void*
        idenity_user_data_lookup_func(void *context,
                                      uintptr_t user_data)
        {
            (void)context;
            return reinterpret_cast<void*>(user_data);
        }
        
        
    } // anonymous namespace
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_immediate_action)
    {
        push_shape_immediate_action(immediate_action_func_index_running2);
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_vm_action_state(target_select_expected_result,
                             0, // shape_atom_index
                             liz_execution_state_running);
        
        expected_result_blackboard[immediate_action_func_index_running2] = liz_execution_state_running;
        
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_running, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, run_immediate_action)
    {
        push_shape_immediate_action(immediate_action_func_index_identity0);
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(0, // shape_atom_index
                                liz_execution_state_running);
        
        push_vm_action_state(target_select_expected_result,
                             0, // shape_atom_index
                             liz_execution_state_running);
        
        expected_result_blackboard[immediate_action_func_index_identity0] = liz_execution_state_running;
        
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_running, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, terminate_successful_immediate_action)
    {
        push_shape_immediate_action(immediate_action_func_index_success3);
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(0, // shape_atom_index
                                liz_execution_state_running);
        
        expected_result_blackboard[immediate_action_func_index_success3] = liz_execution_state_success;
        
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_success, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, terminate_failing_immediate_action)
    {
        push_shape_immediate_action(immediate_action_func_index_fail4);
        
        create_expected_result_and_proband_vms_for_shape();
        
        expected_result_blackboard[immediate_action_func_index_fail4] = liz_execution_state_fail;
        
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_fail, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_deferred_action)
    {
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_vm_action_state(target_select_expected_result,
                             0, // shape_atom_index
                             liz_execution_state_launch);
        
        push_vm_action_launch_request(target_select_expected_result,
                                      42, // action_id
                                      7, // resource_id
                                      0 // shape_atom_index
                                      );
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_launch, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, run_deferred_action_launched_before)
    {
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_vm_action_state(target_select_both,
                             0, // shape_atom_index
                             liz_execution_state_running);
        
        push_actor_action_state(0, // shape_atom_index
                                liz_execution_state_launch);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_running, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, run_deferred_action_running_before)
    {
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_vm_action_state(target_select_both,
                             0, // shape_atom_index
                             liz_execution_state_running);
        
        push_actor_action_state(0, // shape_atom_index
                                liz_execution_state_running);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_running, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, terminate_successful_deferred_action)
    {
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(0, // shape_atom_index
                                liz_execution_state_success);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_success, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, terminate_failing_deferred_action)
    {
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(0, // shape_atom_index
                                liz_execution_state_fail);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_fail, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, tick_persistent_action)
    {
        push_shape_persistent_action();
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(0, // index of persistent state in actor
                                   0, // shape_atom_index
                                   liz_execution_state_success);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_success, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_successfully_terminating_sequence_decider)
    {
        push_shape_sequence_decider(2 // shape atom end offset
                                    );
        push_shape_persistent_action();
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_success, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_terminating_sequence_decider_failing_on_first_child)
    {
        push_shape_sequence_decider(5 // shape atom end offset
                                    );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_success3);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_fail);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_fail, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_terminating_sequence_decider_failing_on_middle_child)
    {
        push_shape_sequence_decider(5 // shape atom end offset
                                    );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_fail4);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        expected_result_blackboard[immediate_action_func_index_fail4] = liz_execution_state_fail;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_fail, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_terminating_sequence_decider_failing_on_last_child)
    {
        push_shape_sequence_decider(5 // shape atom end offset
                                    );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_success3);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        push_actor_action_state(3, // shape_atom_index
                                liz_execution_state_fail);
        
        expected_result_blackboard[immediate_action_func_index_success3] = liz_execution_state_success;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_fail, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_sequence_decider_with_first_child_returning_running)
    {
        push_shape_sequence_decider(5 // shape atom end offset
                                    );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_success3);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_running);
        
        push_actor_action_state(3, // shape_atom_index
                                liz_execution_state_fail);
        
        push_vm_decider_state(target_select_expected_result,
                              0, // shape_atom_index
                              1 // reached child state
                              );
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_running, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_sequence_decider_with_middle_child_returning_running)
    {
        push_shape_sequence_decider(5 // shape atom end offset
                                    );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_running2);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        push_actor_action_state(3, // shape_atom_index
                                liz_execution_state_fail);
        
        push_vm_decider_state(target_select_expected_result,
                              0, // shape_atom_index
                              2 // reached child state
                              );
        
        push_vm_action_state(target_select_expected_result,
                             2,
                             liz_execution_state_running);
        
        expected_result_blackboard[immediate_action_func_index_running2] = liz_execution_state_running;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_running, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_sequence_decider_with_last_child_returning_running)
    {
        push_shape_sequence_decider(5 // shape atom end offset
                                    );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_success3);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        push_actor_action_state(3, // shape_atom_index
                                liz_execution_state_running);
        
        push_vm_decider_state(target_select_expected_result,
                              0, // shape_atom_index
                              3 // reached child state
                              );
        
        push_vm_action_state(target_select_expected_result,
                             3,
                             liz_execution_state_running);
        
        expected_result_blackboard[immediate_action_func_index_success3] = liz_execution_state_success;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_running, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, run_successfully_terminating_sequence_decider)
    {
        push_shape_sequence_decider(5 // shape atom end offset
                                    );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_success3);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        push_actor_action_state(3, // shape_atom_index
                                liz_execution_state_success);
        
        expected_result_blackboard[immediate_action_func_index_success3] = liz_execution_state_success;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_success, proband_vm->execution_state);
    }
    
    
    
    TEST(launch_dynamic_priority_decider_with_first_child_succeeding)
    {
        
    }
    
    
    TEST(launch_dynamic_priority_decider_with_middle_child_succeeding)
    {
        
    }
    
    
    TEST(launch_dynamic_priority_decider_with_last_child_succeeding)
    {
        
    }
    
    
    TEST(launch_dynamic_priority_decider_with_all_children_failing)
    {
        
    }
    
    TEST(launch_dynamic_priority_decider_with_first_child_returning_running)
    {
        
    }
    
    
    TEST(launch_dynamic_priority_decider_with_middle_child_returning_running)
    {
        
    }
    
    
    TEST(launch_dynamic_priority_decider_with_last_child_returning_running)
    {
        
    }
    
    
    TEST(run_dynamic_priority_decider_a_child_succeeding)
    {
        
    }
    
    
    TEST(launch_succeeding_concurrent_decider)
    {
        
    }
    
    
    TEST(launch_concurrent_decider_with_first_child_failing)
    {
        
    }
    
    
    TEST(launch_concurrent_decider_with_middle_child_failing)
    {
        
    }
    
    
    TEST(launch_concurrent_decider_with_last_child_failing)
    {
        
    }
    
    
    TEST(launch_concurrent_decider_with_all_children_succeeding)
    {
        
    }
    
    TEST(launch_concurrent_decider_with_a_child_returning_running)
    {
        
    }
    
    
    TEST(launch_concurrent_decider_with_all_children_returning_running)
    {
        
    }
    
    
    TEST(launch_concurrent_decider_with_succeeding_running_and_failing_children)
    {
        
    }
    
    TEST(run_succeeding_concurrent_decider)
    {
        
    }
    
    
    TEST(cancel_without_running_actions)
    {
        
    }
    
    
    TEST(cancel_immediate_action)
    {
        
    }
    
    
    TEST(cancel_launched_deferred_action)
    {
        
    }
    
    
    TEST(cancel_running_deferred_action)
    {
        
    }
    
    
    TEST(cancel_multiple_active_actions)
    {
        
    }
    
    
    TEST(extract_empty_actor_state)
    {
        
    }
    
    
    TEST(extract_capacity_sized_actor_state)
    {
        
    }
    
    
    TEST(extract_empty_requests)
    {
        
    }
    
    TEST(extract_action_launch_requests)
    {
        
    }
    
    
    TEST(extract_action_cancel_requests)
    {
        
    }
    
    
    TEST(extract_action_requests)
    {
        
    }
    
    
    TEST(extract_full_capacity_action_requests)
    {
        
    }
    
    
    TEST(monitor_action_enter)
    {
        
    }
    
    
    TEST(monitor_action_leave)
    {
        
    }
    
    
    TEST(monitor_action_cancellation)
    {
        
    }
    
    
    TEST(monitor_decider_enter_from_top)
    {
        
    }
    
    
    TEST(monitor_decider_enter_from_bottom)
    {
        
    }
    
    
    TEST(monitor_decider_leave_to_bottom)
    {
        
    }
    
    
    TEST(monitor_decider_leave_to_top)
    {
        
    }
    
} // SUITE(liz_vm_test)
