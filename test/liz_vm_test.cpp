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
#include <set>

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
                            &proband_actor,
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
        
        push_actor_action_state(target_select_proband,
                                0, // shape_atom_index
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
                            &proband_actor,
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
        
        push_actor_action_state(target_select_proband, 
                                0, // shape_atom_index
                                liz_execution_state_running);
        
        expected_result_blackboard[immediate_action_func_index_success3] = liz_execution_state_success;
        
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
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
                            &proband_actor,
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
                            &proband_actor,
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
        
        push_actor_action_state(target_select_proband,
                                0, // shape_atom_index
                                liz_execution_state_launch);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
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
        
        push_actor_action_state(target_select_proband,
                                0, // shape_atom_index
                                liz_execution_state_running);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
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
        
        push_actor_action_state(target_select_proband,
                                0, // shape_atom_index
                                liz_execution_state_success);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
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
        
        push_actor_action_state(target_select_proband,
                                0, // shape_atom_index
                                liz_execution_state_fail);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
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
        
        set_actor_persistent_state(target_select_proband,
                                   0, // index of persistent state in actor
                                   0, // shape_atom_index
                                   liz_execution_state_success);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
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
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
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
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_fail);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
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
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        expected_result_blackboard[immediate_action_func_index_fail4] = liz_execution_state_fail;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
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
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
                                liz_execution_state_fail);
        
        expected_result_blackboard[immediate_action_func_index_success3] = liz_execution_state_success;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
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
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_running);
        
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
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
                            &proband_actor,
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
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
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
                            &proband_actor,
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
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
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
                            &proband_actor,
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
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
                                liz_execution_state_success);
        
        expected_result_blackboard[immediate_action_func_index_success3] = liz_execution_state_success;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_success, proband_vm->execution_state);
    }
    
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_dynamic_priority_decider_with_first_child_succeeding)
    {
        push_shape_dynamic_priority_decider(5 // shape atom end offset
                                            );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_success3);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_success, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_dynamic_priority_decider_with_first_child_succeeding_cancelling_previous_child)
    {
        push_shape_dynamic_priority_decider(5 // shape atom end offset
                                            );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_identity0);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        push_actor_action_state(target_select_proband,
                                2, // shape_atom_index 
                                liz_execution_state_running);
        
        expected_result_blackboard[immediate_action_func_index_identity0] = liz_execution_state_cancel;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_success, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_dynamic_priority_decider_with_middle_child_succeeding)
    {
        push_shape_dynamic_priority_decider(5 // shape atom end offset
                                            );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_success3);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_fail);
        
        expected_result_blackboard[immediate_action_func_index_success3] = liz_execution_state_success;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_success, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_dynamic_priority_decider_with_middle_child_succeeding_and_cancelling_previous_child)
    {
        push_shape_dynamic_priority_decider(5 // shape atom end offset
                                            );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_success3);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_fail);
        
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index 
                                liz_execution_state_launch);
        
        expected_result_blackboard[immediate_action_func_index_success3] = liz_execution_state_success;
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42, // action_id 
                                      7, // resource_id
                                      3 // shape_atom_index
                                      );
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_success, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_dynamic_priority_decider_with_last_child_succeeding)
    {
        push_shape_dynamic_priority_decider(5 // shape atom end offset
                                            );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_fail4);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_fail);
        
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
                                liz_execution_state_success);
        
        expected_result_blackboard[immediate_action_func_index_fail4] = liz_execution_state_fail;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_success, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_dynamic_priority_decider_with_all_children_failing)
    {
        push_shape_dynamic_priority_decider(5 // shape atom end offset
                                            );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_fail4);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_fail);
        
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
                                liz_execution_state_fail);
        
        expected_result_blackboard[immediate_action_func_index_fail4] = liz_execution_state_fail;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_fail, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_dynamic_priority_decider_with_first_child_returning_running)
    {
        push_shape_dynamic_priority_decider(5 // shape atom end offset
                                            );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_fail4);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_running);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_running, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_dynamic_priority_decider_with_first_child_returning_running_and_cancelling_previous_child)
    {
        push_shape_dynamic_priority_decider(5 // shape atom end offset
                                            );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_identity1);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_running);
        
        push_actor_action_state(target_select_proband,
                                2, // shape_atom_index 
                                liz_execution_state_running);
        
        expected_result_blackboard[immediate_action_func_index_identity1] = liz_execution_state_cancel;
        
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_running, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_dynamic_priority_decider_with_middle_child_returning_running)
    {
        push_shape_dynamic_priority_decider(5 // shape atom end offset
                                            );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_running2);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_fail);
        
        push_vm_action_state(target_select_expected_result,
                             2, // shape_atom_index
                             liz_execution_state_running);
        
        expected_result_blackboard[immediate_action_func_index_running2] = liz_execution_state_running;
        
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_running, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_dynamic_priority_decider_with_middle_child_returning_running_and_cancelling_previous_child)
    {
        push_shape_dynamic_priority_decider(5 // shape atom end offset
                                            );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_running2);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_fail);
        
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
                                liz_execution_state_running);
        
        push_vm_action_state(target_select_expected_result,
                             2, // shape_atom_index
                             liz_execution_state_running);
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42, // action_id
                                      7, // resource_id
                                      3 // shape_atom_index
                                      );
        
        expected_result_blackboard[immediate_action_func_index_running2] = liz_execution_state_running;
        
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_running, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_dynamic_priority_decider_with_last_child_returning_running)
    {
        push_shape_dynamic_priority_decider(5 // shape atom end offset
                                            );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_fail4);
        push_shape_deferred_action(42, // action_ud
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index
                                   1, // shape_atom_index
                                   liz_execution_state_fail);
        
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
                                liz_execution_state_launch);
        
        push_vm_action_state(target_select_expected_result,
                             3, // shape_atom_index
                             liz_execution_state_running);
        
        expected_result_blackboard[immediate_action_func_index_fail4] = liz_execution_state_fail;
        
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_running, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_succeeding_concurrent_decider)
    {
        push_shape_concurrent_decider(2 // shape atom end offset
                                      );
        
        push_shape_persistent_action();
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband, 
                                   0, // persistent state index 
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_success, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_concurrent_decider_with_first_child_failing)
    {
        push_shape_concurrent_decider(5 // shape atom end offset
                                      );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_fail4);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband, 
                                   0, // persistent state index 
                                   1, // shape_atom_index
                                   liz_execution_state_fail);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_fail, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_concurrent_decider_with_first_child_failing_and_cancel_previous_children)
    {
        push_shape_concurrent_decider(5 // shape atom end offset
                                      );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_identity1);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband, 
                                   0, // persistent state index 
                                   1, // shape_atom_index
                                   liz_execution_state_fail);
        
        push_actor_action_state(target_select_proband,
                                2, // shape_atom_index
                                liz_execution_state_running);
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
                                liz_execution_state_launch);
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42, // action_id
                                      7, // resource_id
                                      3 // shape_atom_index
                                      );
        
        expected_result_blackboard[immediate_action_func_index_identity1] = liz_execution_state_cancel;
        
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_fail, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_concurrent_decider_with_first_child_failing_and_cancel_running_previous_children)
    {
        push_shape_concurrent_decider(5 // shape atom end offset
                                      );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_identity1);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband, 
                                   0, // persistent state index 
                                   1, // shape_atom_index
                                   liz_execution_state_fail);
        
        push_actor_action_state(target_select_proband,
                                2, // shape_atom_index
                                liz_execution_state_running);
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
                                liz_execution_state_success);
        
        expected_result_blackboard[immediate_action_func_index_identity1] = liz_execution_state_cancel;
        
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_fail, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_concurrent_decider_with_middle_child_failing)
    {
        push_shape_concurrent_decider(5 // shape atom end offset
                                      );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_fail4);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index 
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        expected_result_blackboard[immediate_action_func_index_fail4] = liz_execution_state_fail;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_fail, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_concurrent_decider_with_middle_child_failing_cancel_current_and_previously_running_children)
    {
        push_shape_concurrent_decider(5 // shape atom end offset
                                      );
        push_shape_immediate_action(immediate_action_func_index_identity0);

        push_shape_immediate_action(immediate_action_func_index_fail4);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(target_select_proband,
                                1, // shape_atom_index
                                liz_execution_state_running);
        push_actor_action_state(target_select_proband,
                                2, // shape_atom_index
                                liz_execution_state_running);
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
                                liz_execution_state_running);
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42, // action_id 
                                      7, // resource_id 
                                      3 // shape_atom_index
                                      );
        
        expected_result_blackboard[immediate_action_func_index_identity0] = liz_execution_state_cancel;
        expected_result_blackboard[immediate_action_func_index_fail4] = liz_execution_state_fail;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_fail, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_concurrent_decider_with_last_child_failing)
    {
        push_shape_concurrent_decider(5 // shape atom end offset
                                      );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_success3);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband, 
                                   0, // persistent state index
                                   1, // shape_atom_index 
                                   liz_execution_state_success);
        
        push_actor_action_state(target_select_proband,
                                2, // shape_atom_index
                                liz_execution_state_running);
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
                                liz_execution_state_fail);
        
        expected_result_blackboard[immediate_action_func_index_success3] = liz_execution_state_success;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_fail, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_concurrent_decider_with_last_child_failing_cancel_currently_running_children)
    {
        push_shape_concurrent_decider(6 // shape atom end offset
                                      );
        push_shape_deferred_action(23, // action_id
                                   32 // resource_id
                                   );
        push_shape_immediate_action(immediate_action_func_index_success3);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(target_select_proband,
                                1, // shape_atom_index
                                liz_execution_state_running);
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
                                liz_execution_state_running);
        push_actor_action_state(target_select_proband,
                                4, // shape_atom_index
                                liz_execution_state_fail);
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      23, // action_id
                                      32, // resource_id
                                      1 // shape_atom_index
                                      );
        
        expected_result_blackboard[immediate_action_func_index_success3] = liz_execution_state_success;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_fail, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_concurrent_decider_with_all_children_succeeding)
    {
        push_shape_concurrent_decider(5 // shape atom end offset
                                      );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_success3);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index 
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
                                liz_execution_state_success);
        
        
        expected_result_blackboard[immediate_action_func_index_success3] = liz_execution_state_success;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_success, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_concurrent_decider_with_a_child_returning_running)
    {
        push_shape_concurrent_decider(5 // shape atom end offset
                                      );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_running2);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index 
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
                                liz_execution_state_success);
        
        push_vm_action_state(target_select_expected_result,
                             2, // shape_atom_index
                             liz_execution_state_running);
        
        
        expected_result_blackboard[immediate_action_func_index_running2] = liz_execution_state_running;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_running, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_concurrent_decider_with_all_children_returning_running)
    {
        push_shape_concurrent_decider(5 // shape atom end offset
                                      );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_running2);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband, 
                                   0, // persistent state index 
                                   1, // shape_atom_index
                                   liz_execution_state_running);
        
        push_vm_action_state(target_select_expected_result,
                             2, // shape_atom_index
                             liz_execution_state_running);
        push_vm_action_state(target_select_expected_result,
                             3, // shape_atom_index
                             liz_execution_state_launch);
        push_vm_action_launch_request(target_select_expected_result,
                                      42, // action_id 
                                      7, // resource_id
                                      3 // shape_atom_index
                                      );
        
        
        expected_result_blackboard[immediate_action_func_index_running2] = liz_execution_state_running;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_launch, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_concurrent_decider_with_succeeding_running_and_failing_children)
    {
        push_shape_concurrent_decider(5 // shape atom end offset
                                      );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_identity0);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index 
                                   1, // shape_atom_index
                                   liz_execution_state_success);
        
        push_actor_action_state(target_select_proband,
                                2, // shape_atom_index
                                liz_execution_state_running);
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
                                liz_execution_state_fail);
        
        
        expected_result_blackboard[immediate_action_func_index_identity0] = liz_execution_state_cancel;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_fail, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, launch_concurrent_decider_with_running_succeeding_and_failing_children)
    {
        push_shape_concurrent_decider(5 // shape atom end offset
                                      );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_identity0);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index 
                                   1, // shape_atom_index
                                   liz_execution_state_running);
        
        push_actor_action_state(target_select_proband,
                                2, // shape_atom_index
                                liz_execution_state_success);
        push_actor_action_state(target_select_proband,
                                3, // shape_atom_index
                                liz_execution_state_fail);
        
        
        expected_result_blackboard[immediate_action_func_index_identity0] = liz_execution_state_success;
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_fail, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, multi_level_bt_complex_fail_running_launching_cancelling)
    {
        push_shape_dynamic_priority_decider(19); // shape_atom_index 0
        {
            // Fails.
            push_shape_concurrent_decider(5); // shape_atom_index 1
            {
                // Runs - is cancelled.
                push_shape_sequence_decider(3); // shape_atom_index 2
                {
                    // Runs - is cancelled.
                    push_shape_deferred_action(10, 1); // shape_atom_index 3-4
                }
                
                // Fails.
                push_shape_immediate_action(immediate_action_func_index_fail4); // shape_atom_index 5
            }
            
            // Runs now and therefore cancels previously running lower-priority
            // child.
            push_shape_concurrent_decider(5); // shape_atom_index 6
            {
                // Running.
                push_shape_sequence_decider(2); // shape_atom_index 7
                {
                    // Running.
                    push_shape_immediate_action(immediate_action_func_index_running2); // shape_atom_index 8
                }
                
                // Launching.
                push_shape_deferred_action(20, 2); // shape_atom_index 9-10
            }
            
            // Ran previously and would succeed now.
            push_shape_dynamic_priority_decider(8); // shape_atom_index 11
            {
                // Failed previously would crash the program if called now.
                push_shape_immediate_action(immediate_action_func_index_cancel5); // shape_atom_index 12
                
                push_shape_sequence_decider(6); // shape_atom_index 13
                {
                    // Succeeded previously.
                    push_shape_persistent_action(); // shape_atom_index 14
                    
                    // Ran previously - is cancelled.
                    push_shape_concurrent_decider(4); // shape_atom_index 15
                    {
                        // Ran previously - is cancelled.
                        push_shape_deferred_action(30, 3); // shape_atom_index 16-17
                     
                        // Ran previously - is cancelled.
                        push_shape_immediate_action(immediate_action_func_index_identity1); // shape_atom_index 18
                    }
                }
            }
        }
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband, 0, 14, liz_execution_state_success);
        push_actor_decider_state(target_select_proband, 13, 15);
        push_actor_action_state(target_select_proband, 16, liz_execution_state_running);
        push_actor_action_state(target_select_proband, 18, liz_execution_state_running);
        
        expected_result_blackboard[immediate_action_func_index_fail4] = liz_execution_state_fail;
        expected_result_blackboard[immediate_action_func_index_running2] = liz_execution_state_running;
        expected_result_blackboard[immediate_action_func_index_identity1] = liz_execution_state_cancel;
        
        push_vm_decider_state(target_select_expected_result, 7, 8);
        push_vm_action_state(target_select_expected_result, 8, liz_execution_state_running);
        push_vm_action_state(target_select_expected_result, 9, liz_execution_state_launch);
        push_vm_action_launch_request(target_select_expected_result, 20, 2, 9);
        push_vm_action_cancel_request(target_select_expected_result, 30, 3, 16);
        
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
        CHECK_EQUAL(liz_execution_state_launch, proband_vm->execution_state);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_without_running_actions)
    {
        push_shape_concurrent_decider(5 // shape atom end offset
                                      );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_identity0);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband, 
                                   0, // persistent state index 
                                   1, // shape_atom_index
                                   liz_execution_state_running);
        
        push_actor_action_state(target_select_proband, 
                                2, // shape_atom_index
                                liz_execution_state_success);
        push_actor_action_state(target_select_proband, 
                                3, // shape_atom_index
                                liz_execution_state_success);
        
        liz_vm_cancel_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_immediate_action)
    {
        push_shape_immediate_action(immediate_action_func_index_identity0);
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(target_select_proband, 
                                0, // shape_atom_index
                                liz_execution_state_running);
        
        expected_result_blackboard[immediate_action_func_index_identity0] = liz_execution_state_cancel;
        
        liz_vm_cancel_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_launched_deferred_action)
    {
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(target_select_proband, 
                                0, // shape_atom_index
                                liz_execution_state_launch);
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42, // action_id
                                      7, // resource_id
                                      0 // shape_atom_index
                                      );
        
        liz_vm_cancel_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_running_deferred_action)
    {
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(target_select_proband, 
                                0, // shape_atom_index
                                liz_execution_state_running);
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42, // action_id
                                      7, // resource_id
                                      0 // shape_atom_index
                                      );
        
        liz_vm_cancel_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, cancel_multiple_active_actions)
    {
        push_shape_concurrent_decider(5 // shape atom end offset
                                      );
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_identity0);
        push_shape_deferred_action(42, // action_id
                                   7 // resource_id
                                   );
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband,
                                   0, // persistent state index 
                                   1, // shape_atom_index
                                   liz_execution_state_running);
        
        push_actor_action_state(target_select_proband, 
                                2, // shape_atom_index
                                liz_execution_state_running);
        push_actor_action_state(target_select_proband, 
                                3, // shape_atom_index
                                liz_execution_state_running);
        
        expected_result_blackboard[immediate_action_func_index_identity0] = liz_execution_state_cancel;
        
        push_vm_action_cancel_request(target_select_expected_result,
                                      42, // action_id
                                      7, // resource_id
                                      3 // shape_atom_index
                                      );
        
        liz_vm_cancel_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_vm_extractable_state_comparator, 
                    proband_vm_extractable_state_comparator);
        CHECK_ARRAY_EQUAL(expected_result_blackboard, 
                          proband_blackboard, 
                          shape_immediate_action_function_count);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, monitor_deferred_action_traversal)
    {
        push_shape_deferred_action(42, 7);
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_enter_from_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_leave_to_top, update_time_zero);
        
        liz_vm_update_actor(proband_vm,
                            monitor,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_monitor_log, proband_monitor_log);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, monitor_deferred_action_cancellation)
    {
        push_shape_deferred_action(42, 7);
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(target_select_proband, 0, liz_execution_state_launch);
        
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_enter_from_top | liz_vm_monitor_node_flag_cancel_action, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_leave_to_top | liz_vm_monitor_node_flag_cancel_action, update_time_zero);

        
        liz_vm_cancel_actor(proband_vm,
                            monitor,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_monitor_log, proband_monitor_log);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, monitor_immediate_action_traversal)
    {
        push_shape_immediate_action(immediate_action_func_index_success3);
        
        create_expected_result_and_proband_vms_for_shape();
       
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_enter_from_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_leave_to_top, update_time_zero);
        
        liz_vm_update_actor(proband_vm,
                            monitor,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_monitor_log, proband_monitor_log);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, monitor_immediate_action_cancellation)
    {
        push_shape_immediate_action(immediate_action_func_index_identity0);        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(target_select_proband, 0, liz_execution_state_running);
        
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_enter_from_top | liz_vm_monitor_node_flag_cancel_action, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_leave_to_top | liz_vm_monitor_node_flag_cancel_action, update_time_zero);
                
        liz_vm_cancel_actor(proband_vm,
                            monitor,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_monitor_log, proband_monitor_log);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, monitor_persistent_action_traversal)
    {
        push_shape_persistent_action();
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_proband, 0, 0, liz_execution_state_running);
        
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_enter_from_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_leave_to_top, update_time_zero);
        
        liz_vm_update_actor(proband_vm,
                            monitor,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_monitor_log, proband_monitor_log);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, monitor_sequence_decider_traversal)
    {
        push_shape_sequence_decider(3);
        push_shape_immediate_action(immediate_action_func_index_success3);
        push_shape_immediate_action(immediate_action_func_index_success3);
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_enter_from_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_leave_to_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 1, liz_vm_monitor_node_flag_enter_from_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 1, liz_vm_monitor_node_flag_leave_to_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_enter_from_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_leave_to_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 2, liz_vm_monitor_node_flag_enter_from_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 2, liz_vm_monitor_node_flag_leave_to_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_enter_from_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_leave_to_top, update_time_zero);
        
        liz_vm_update_actor(proband_vm,
                            monitor,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_monitor_log, proband_monitor_log);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, monitor_dynamic_priority_decider_traversal)
    {
        push_shape_dynamic_priority_decider(3);
        push_shape_immediate_action(immediate_action_func_index_success3);
        push_shape_immediate_action(immediate_action_func_index_identity1);
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(target_select_proband, 2, liz_execution_state_running);
        
        
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_enter_from_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_leave_to_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 1, liz_vm_monitor_node_flag_enter_from_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 1, liz_vm_monitor_node_flag_leave_to_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_enter_from_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_leave_to_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 2, liz_vm_monitor_node_flag_enter_from_top | liz_vm_monitor_node_flag_cancel_action, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 2, liz_vm_monitor_node_flag_leave_to_top | liz_vm_monitor_node_flag_cancel_action, update_time_zero);
        
        liz_vm_update_actor(proband_vm,
                            monitor,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_monitor_log, proband_monitor_log);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, monitor_concurrent_decider_traversal)
    {
        push_shape_concurrent_decider(3);
        push_shape_immediate_action(immediate_action_func_index_identity0);
        push_shape_immediate_action(immediate_action_func_index_identity1);
        
        create_expected_result_and_proband_vms_for_shape();

        push_actor_action_state(target_select_proband, 1, liz_execution_state_running);
        push_actor_action_state(target_select_proband, 2, liz_execution_state_fail);
        
        
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_enter_from_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_leave_to_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 1, liz_vm_monitor_node_flag_enter_from_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 1, liz_vm_monitor_node_flag_leave_to_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_enter_from_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_leave_to_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 2, liz_vm_monitor_node_flag_enter_from_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 2, liz_vm_monitor_node_flag_leave_to_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_enter_from_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_leave_to_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 1, liz_vm_monitor_node_flag_enter_from_top | liz_vm_monitor_node_flag_cancel_action, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 1, liz_vm_monitor_node_flag_leave_to_top | liz_vm_monitor_node_flag_cancel_action, update_time_zero);
        
        liz_vm_update_actor(proband_vm,
                            monitor,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_monitor_log, proband_monitor_log);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, monitor_deeper_behavior_tree_traversal)
    {
        push_shape_dynamic_priority_decider(4);
        {
            push_shape_concurrent_decider(3);
            {
                push_shape_sequence_decider(2);
                {
                    push_shape_immediate_action(immediate_action_func_index_success3);
                }
            }
        }
        
        create_expected_result_and_proband_vms_for_shape(); 
        
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_enter_from_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_leave_to_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 1, liz_vm_monitor_node_flag_enter_from_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 1, liz_vm_monitor_node_flag_leave_to_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 2, liz_vm_monitor_node_flag_enter_from_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 2, liz_vm_monitor_node_flag_leave_to_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 3, liz_vm_monitor_node_flag_enter_from_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 3, liz_vm_monitor_node_flag_leave_to_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 2, liz_vm_monitor_node_flag_enter_from_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 2, liz_vm_monitor_node_flag_leave_to_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 1, liz_vm_monitor_node_flag_enter_from_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 1, liz_vm_monitor_node_flag_leave_to_top, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_enter_from_bottom, update_time_zero);
        push_monitor_log_entry(target_select_expected_result, 0, liz_vm_monitor_node_flag_leave_to_top, update_time_zero);
        
        liz_vm_update_actor(proband_vm,
                            monitor,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        CHECK_EQUAL(expected_result_monitor_log, proband_monitor_log);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, extract_empty_actor_state)
    {
        push_shape_sequence_decider(5);
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_identity0);
        push_shape_deferred_action(42, 7);
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_decider_state(target_select_proband, 0, 3);
        push_actor_action_state(target_select_proband, 3, liz_execution_state_success);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        liz_vm_extract_actor_state(proband_vm, &proband_actor, &shape);
        
        CHECK_EQUAL(expected_result_actor_comparator, proband_actor_comparator);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, extract_capacity_sized_actor_state)
    {
        push_shape_concurrent_decider(5);
        push_shape_persistent_action();
        push_shape_immediate_action(immediate_action_func_index_running2);
        push_shape_deferred_action(42, 7);
        
        create_expected_result_and_proband_vms_for_shape();
        
        set_actor_persistent_state(target_select_both, 0, 1, liz_execution_state_running);
        push_actor_action_state(target_select_both, 2, liz_execution_state_running);
        
        push_actor_action_state(target_select_expected_result, 3, liz_execution_state_launch);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        liz_vm_extract_actor_state(proband_vm, &proband_actor, &shape);
        
        CHECK_EQUAL(expected_result_actor_comparator, proband_actor_comparator);
    }
    
    
    
    TEST_FIXTURE(liz_vm_test_fixture, extract_empty_requests)
    {
        push_shape_immediate_action(immediate_action_func_index_success3);
        
        create_expected_result_and_proband_vms_for_shape();
        
        liz_id_t const actor_id = 5;
        liz_int_t const request_capacity = 3;
        liz_action_request_t expected_result[request_capacity] = {};
        liz_action_request_t proband[request_capacity] = {};
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        liz_int_t const expected_result_extract_count = 0;
        liz_int_t const extract_count = liz_vm_extract_action_requests(proband_vm,
                                                                       proband, 
                                                                       request_capacity, actor_id);
        
        std::set<liz_action_request_t, liz_action_request_compare> expected_result_set(&expected_result[0], &expected_result[expected_result_extract_count]);
        
        std::set<liz_action_request_t, liz_action_request_compare> proband_set(&proband[0], &proband[extract_count]);
        
        CHECK_EQUAL(expected_result_extract_count, extract_count);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, extract_action_launch_requests)
    {
        push_shape_concurrent_decider(6);
        {
            push_shape_deferred_action(11, 1);
            push_shape_immediate_action(immediate_action_func_index_success3);
            push_shape_deferred_action(13, 3);
        }
        
        create_expected_result_and_proband_vms_for_shape();
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        liz_id_t const actor_id = 5;
        liz_int_t const request_capacity = 3;
        liz_action_request_t expected_result[request_capacity] = {
            {
                actor_id,
                11,
                1,
                1,
                liz_action_request_type_launch
            },
            {
                actor_id,
                13,
                3,
                3,
                liz_action_request_type_launch
            }
        };
        liz_action_request_t proband[request_capacity] = {};
        
        liz_int_t const expected_result_extract_count = 2;
        liz_int_t const extract_count = liz_vm_extract_action_requests(proband_vm,
                                                                       proband, 
                                                                       request_capacity, actor_id);
        
        std::set<liz_action_request_t, liz_action_request_compare> expected_result_set(&expected_result[0], &expected_result[expected_result_extract_count]);
        
        std::set<liz_action_request_t, liz_action_request_compare> proband_set(&proband[0], &proband[extract_count]);
        
        CHECK_EQUAL(expected_result_extract_count, extract_count);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, extract_action_cancel_requests)
    {
        push_shape_concurrent_decider(6);
        {
            push_shape_deferred_action(11, 1);
            push_shape_immediate_action(immediate_action_func_index_fail4);
            push_shape_deferred_action(13, 3);
        }
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(target_select_proband, 4, liz_execution_state_running);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        liz_id_t const actor_id = 5;
        liz_int_t const request_capacity = 3;
        liz_action_request_t expected_result[request_capacity] = {
            {
                actor_id,
                13,
                3,
                4,
                liz_action_request_type_cancel
            }
        };
        liz_action_request_t proband[request_capacity] = {};
        
        liz_int_t const expected_result_extract_count = 1;
        liz_int_t const extract_count = liz_vm_extract_action_requests(proband_vm,
                                                                       proband, 
                                                                       request_capacity, actor_id);
        
        std::set<liz_action_request_t, liz_action_request_compare> expected_result_set(&expected_result[0], &expected_result[expected_result_extract_count]);
        
        std::set<liz_action_request_t, liz_action_request_compare> proband_set(&proband[0], &proband[extract_count]);
        
        CHECK_EQUAL(expected_result_extract_count, extract_count);
    }
    
    
    TEST_FIXTURE(liz_vm_test_fixture, extract_full_capacity_action_requests)
    {
        push_shape_dynamic_priority_decider(8);
        {
            push_shape_deferred_action(11, 1);
            push_shape_concurrent_decider(5);
            {
                push_shape_deferred_action(12, 2);
                push_shape_deferred_action(13, 3);
            }
        }
        
        create_expected_result_and_proband_vms_for_shape();
        
        push_actor_action_state(target_select_proband, 4, liz_execution_state_running);
        push_actor_action_state(target_select_proband, 6, liz_execution_state_running);
        
        liz_vm_update_actor(proband_vm,
                            monitor_null,
                            user_data_lookup_context_null,
                            idenity_user_data_lookup_func,
                            update_time_zero,
                            &proband_actor,
                            &shape);
        
        liz_id_t const actor_id = 5;
        liz_int_t const request_capacity = 3;
        liz_action_request_t expected_result[request_capacity] = {
            {
                actor_id,
                11,
                1,
                1,
                liz_action_request_type_launch
            },
            {
                actor_id,
                12,
                2,
                4,
                liz_action_request_type_cancel
            },
            {
                actor_id,
                13,
                3,
                6,
                liz_action_request_type_cancel
            }
        };
        liz_action_request_t proband[request_capacity] = {};
        
        liz_int_t const expected_result_extract_count = 3;
        liz_int_t const extract_count = liz_vm_extract_action_requests(proband_vm,
                                                                       proband, 
                                                                       request_capacity, actor_id);
        
        std::set<liz_action_request_t, liz_action_request_compare> expected_result_set(&expected_result[0], &expected_result[expected_result_extract_count]);
        
        std::set<liz_action_request_t, liz_action_request_compare> proband_set(&proband[0], &proband[extract_count]);
        
        CHECK_EQUAL(expected_result_extract_count, extract_count);
    }
    
} // SUITE(liz_vm_test)
