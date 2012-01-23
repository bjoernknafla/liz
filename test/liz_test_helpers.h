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

#ifndef LIZ_liz_test_helpers_H
#define LIZ_liz_test_helpers_H


#include <cassert>
#include <cstdlib>
#include <sstream>

#include <unittestpp.h>


#include <liz/liz_lookaside_stack.h>
#include <liz/liz_lookaside_double_stack.h>
#include <liz/liz_common.h>
#include <liz/liz_common_internal.h>
#include <liz/liz_vm.h>



namespace {
    
    
    inline
    bool
    operator==(liz_lookaside_stack_t const& lhs, 
               liz_lookaside_stack_t const& rhs)
    {
        bool const result = ((lhs.capacity == rhs.capacity) && (lhs.count == rhs.count));
        
        return result;
    }
    
    
    
    inline
    bool
    operator==(liz_lookaside_double_stack_t const& lhs, 
               liz_lookaside_double_stack_t const& rhs)
    {
        bool const result = ((lhs.capacity == rhs.capacity)
                             && (lhs.count_low == rhs.count_low)
                             && (lhs.count_high == rhs.count_high));
        
        return result;
    }
    
    
    
    inline
    bool
    operator==(liz_vm_cancellation_range_t const& lhs, 
               liz_vm_cancellation_range_t const& rhs)
    {    
        bool const result = (((liz_vm_cancellation_range_is_empty(lhs)) 
                              && (liz_vm_cancellation_range_is_empty(rhs))) 
                             || ((lhs.begin_index == rhs.begin_index)
                                 && (lhs.end_index == rhs.end_index)));
        
        return result;
    }
    
    
    
    inline
    bool
    operator==(liz_shape_specification_t const& lhs,
               liz_shape_specification_t const& rhs)
    {
        bool const result = ((lhs.shape_atom_count == rhs.shape_atom_count)
                             && (lhs.immediate_action_function_count == rhs.immediate_action_function_count)
                             && (lhs.persistent_state_count == rhs.persistent_state_count)
                             && (lhs.decider_state_capacity == rhs.decider_state_capacity)
                             && (lhs.action_state_capacity == rhs.action_state_capacity)
                             && (lhs.persistent_state_change_capacity == rhs.persistent_state_change_capacity)
                             && (lhs.decider_guard_capacity == rhs.decider_guard_capacity)
                             && (lhs.action_request_capacity == rhs.action_request_capacity));
        
        return result;
    }
    
    
    
    inline
    bool
    operator==(liz_action_request_t const& lhs,
               liz_action_request_t const& rhs)
    {
        bool const result = ((lhs.actor_id == rhs.actor_id)
                             && (lhs.action_id == rhs.action_id)
                             && (lhs.parameter == rhs.parameter)
                             && (lhs.shape_atom_index == rhs.shape_atom_index)
                             && (lhs.type == rhs.type));
        
        return result;
    }
    
    
    
    inline
    bool
    operator==(liz_action_state_update_t const& lhs,
               liz_action_state_update_t const& rhs)
    {
        bool const result = ((lhs.actor_id == rhs.actor_id)
                             && (lhs.shape_atom_index == rhs.shape_atom_index)
                             && (lhs.state == rhs.state));
        
        return result;
    }
    
    
    
    inline
    bool
    operator==(liz_vm_action_request_t const& lhs,
               liz_vm_action_request_t const& rhs)
    {
        bool const result = ((lhs.action_id == rhs.action_id)
                             && (lhs.resource_id == rhs.resource_id)
                             && (lhs.shape_atom_index == rhs.shape_atom_index));
        
        return result;
    }
    
    
    
    inline
    bool
    operator==(liz_vm_decider_guard_t const& lhs,
               liz_vm_decider_guard_t const& rhs)
    {
        bool const result = ((lhs.shape_atom_index == rhs.shape_atom_index)
                             && (lhs.end_index == rhs.end_index)
                             && (lhs.decider_state_rollback_marker == rhs.decider_state_rollback_marker)
                             && (lhs.action_launch_request_rollback_marker == rhs.action_launch_request_rollback_marker)
                             && (lhs.sequence_reached_child_index == rhs.sequence_reached_child_index)
                             && (lhs.concurrent_execution_state == rhs.concurrent_execution_state)
                             && (lhs.type == rhs.type));
        
        return result;
    }
    
    
    
    inline
    bool
    operator==(liz_vm_monitor_t const& lhs,
               liz_vm_monitor_t const& rhs)
    {
        bool const result = ((lhs.user_data == rhs.user_data)
                             && (lhs.func == rhs.func));
        
        return result;
    }
    
    
    
    inline
    bool
    persistent_state_array_equals(liz_persistent_state_t const* lhs,
                                  uint16_t const* lhs_shape_atom_indices,
                                  liz_int_t const lhs_count,
                                  liz_persistent_state_t const* rhs,
                                  uint16_t const* rhs_shape_atom_indices,
                                  liz_int_t const rhs_count,
                                  liz_shape_atom_t const* shape_atoms,
                                  liz_int_t const shape_atom_count)
    {
        if (lhs_count != rhs_count) {
            return false;
        }
        
        for (liz_int_t i = 0; i < lhs_count; ++i) {
            
            if (lhs_shape_atom_indices[i] != rhs_shape_atom_indices[i]) {
                return false;
            }
            
            assert(lhs_shape_atom_indices[i] < shape_atom_count);
            
            switch (shape_atoms[lhs_shape_atom_indices[i]].type_mask.type) {
                case liz_node_type_persistent_action:
                    if (lhs[i].persistent_action.state != rhs[i].persistent_action.state) {
                        return false;
                    }
                    
                    break;
                    
                default:
                    assert(false && "Unhandled node with associated persistent state.");
                    
                    // Trying something non-reliable, aka no guarantee that an 
                    // assignment to a union that is less than the union size 
                    // creates a defined memory pattern for the memory parts
                    // unovered.
                    if (lhs[i].size_and_alignment_dummy != rhs[i].size_and_alignment_dummy) {
                        return false;
                    }
                    
                    break;
            }
        }
        
        return true;
    }
    
    
    
    class liz_persistent_state_comparator {
    public:
        
        // Does not take over ownership or memory management of data.
        void set(liz_persistent_state_t const* values,
                 uint16_t const* keys,
                 liz_int_t const count,
                 liz_shape_atom_t const* shape_atoms,
                 liz_int_t const shape_atom_count)
        {
            values_ = values;
            keys_ = keys;
            count_ = count;
            shape_atoms_ = shape_atoms;
            shape_atom_count_ = shape_atom_count;
        }
        
        
        bool operator==(liz_persistent_state_comparator const& other) const
        {
            bool const result = ((shape_atoms_ == other.shape_atoms_)
                                 && (shape_atom_count_ == other.shape_atom_count_)
                                 && (persistent_state_array_equals(values_,
                                                                   keys_,
                                                                   count_,
                                                                   other.values_,
                                                                   other.keys_,
                                                                   other.count_,
                                                                   shape_atoms_,
                                                                   shape_atom_count_)));
            
            return result;
        }
        
        
        liz_persistent_state_t const* values_;
        uint16_t const* keys_;
        liz_int_t count_;
        liz_shape_atom_t const* shape_atoms_;
        liz_int_t shape_atom_count_;
    };
    
    
    
    template<typename T>
    bool
    array_equals(T const* lhs,
                 liz_int_t const lhs_count,
                 T const* rhs,
                 liz_int_t const rhs_count) 
    {
        if (lhs_count != rhs_count) {
            return false;
        }
        
        for (liz_int_t i = 0; i < lhs_count; ++i) {
            if (! (lhs[i] == rhs[i])) {
                return false;
            }
        }
        
        return true;
    }
    
    
    
    inline
    bool
    liz_vm_actor_equals(liz_vm_actor const& lhs,
                        liz_vm_actor const& rhs,
                        uint16_t const* persistent_state_shape_atom_indices,
                        liz_int_t const persistent_state_count,
                        liz_shape_atom_t const* shape_atoms,
                        liz_int_t const shape_atom_count)
    {
        
        bool result = (lhs.header->user_data == rhs.header->user_data
                       && lhs.header->placeholder_for_random_number_seed == rhs.header->placeholder_for_random_number_seed
                       && lhs.header->actor_id == rhs.header->actor_id
                       && lhs.header->decider_state_count == rhs.header->decider_state_count
                       && lhs.header->action_state_count == rhs.header->action_state_count
                       && persistent_state_array_equals(lhs.persistent_states,
                                                        persistent_state_shape_atom_indices,
                                                        persistent_state_count,
                                                        rhs.persistent_states,
                                                        persistent_state_shape_atom_indices,
                                                        persistent_state_count,
                                                        shape_atoms,
                                                        shape_atom_count)
                       && array_equals(lhs.decider_state_shape_atom_indices,
                                       lhs.header->decider_state_count,
                                       rhs.decider_state_shape_atom_indices,
                                       rhs.header->decider_state_count)
                       && array_equals(lhs.decider_states,
                                       lhs.header->decider_state_count,
                                       rhs.decider_states,
                                       rhs.header->decider_state_count)
                       && array_equals(lhs.action_state_shape_atom_indices,
                                       lhs.header->action_state_count,
                                       rhs.action_state_shape_atom_indices,
                                       rhs.header->action_state_count)
                       && array_equals(lhs.action_states,
                                       lhs.header->action_state_count,
                                       rhs.action_states,
                                       rhs.header->action_state_count)
                       );
        
        return result;
    }
    
    
    
    class liz_vm_actor_comparator {
    public:
        void set(liz_vm_actor_t const* actor, 
                 liz_vm_shape_t const* shape)
        {
            actor_ = actor;
            shape_ = shape;
        }
        
        
        bool operator==(liz_vm_actor_comparator const& other) 
        {
            bool result = (shape_ == other.shape_
                           && liz_vm_actor_equals(*actor_,
                                                  *other.actor_,
                                                  shape_->persistent_state_shape_atom_indices,
                                                  shape_->spec.persistent_state_count,
                                                  shape_->atoms,
                                                  shape_->spec.shape_atom_count)
                           );
            
            return result;
        }
        
        liz_vm_actor_t const* actor_;
        liz_vm_shape_t const* shape_;
        
    };
    
    
    inline
    bool
    liz_vm_equals(liz_vm_t const& lhs, 
                  liz_vm_t const& rhs,
                  liz_shape_atom_t const* shape_atoms,
                  liz_int_t const shape_atom_count)
    {
        bool const result = ((lhs.shape_atom_index == rhs.shape_atom_index)
                             && (lhs.actor_decider_state_index == rhs.actor_decider_state_index)
                             && (lhs.actor_action_state_index == rhs.actor_action_state_index)
                             && (lhs.actor_persistent_state_index == rhs.actor_persistent_state_index)
                             && (persistent_state_array_equals(lhs.persistent_state_changes, 
                                                               lhs.persistent_state_change_shape_atom_indices,
                                                               liz_lookaside_stack_count(&lhs.persistent_state_change_stack_header),
                                                               rhs.persistent_state_changes,
                                                               rhs.persistent_state_change_shape_atom_indices,
                                                               liz_lookaside_stack_count(&rhs.persistent_state_change_stack_header),
                                                               shape_atoms,
                                                               shape_atom_count))
                             && (array_equals(lhs.decider_state_shape_atom_indices,
                                              liz_lookaside_stack_count(&lhs.decider_state_stack_header),
                                              rhs.decider_state_shape_atom_indices,
                                              liz_lookaside_stack_count(&rhs.decider_state_stack_header)))
                             && (array_equals(lhs.action_state_shape_atom_indices,
                                              liz_lookaside_stack_count(&lhs.action_state_stack_header),
                                              rhs.action_state_shape_atom_indices,
                                              liz_lookaside_stack_count(&rhs.action_state_stack_header)))
                             && (array_equals(lhs.decider_states,
                                              liz_lookaside_stack_count(&lhs.decider_state_stack_header),
                                              rhs.decider_states,
                                              liz_lookaside_stack_count(&rhs.decider_state_stack_header)))
                             && (array_equals(lhs.action_states,
                                              liz_lookaside_stack_count(&lhs.action_state_stack_header),
                                              rhs.action_states,
                                              liz_lookaside_stack_count(&rhs.action_state_stack_header)))
                             && (array_equals(lhs.decider_guards,
                                              liz_lookaside_stack_count(&rhs.decider_guard_stack_header),
                                              rhs.decider_guards,
                                              liz_lookaside_stack_count(&rhs.decider_guard_stack_header)))
                             && (array_equals(lhs.action_requests,
                                              liz_lookaside_double_stack_count_low(&lhs.action_request_stack_header),
                                              rhs.action_requests,
                                              liz_lookaside_double_stack_count_low(&rhs.action_request_stack_header)))
                             && (array_equals(lhs.action_requests + liz_lookaside_double_stack_capacity(&lhs.action_request_stack_header) - liz_lookaside_double_stack_count_high(&lhs.action_request_stack_header),
                                              liz_lookaside_double_stack_count_high(&lhs.action_request_stack_header),
                                              rhs.action_requests + liz_lookaside_double_stack_capacity(&rhs.action_request_stack_header) - liz_lookaside_double_stack_count_high(&rhs.action_request_stack_header),
                                              liz_lookaside_double_stack_count_high(&rhs.action_request_stack_header)))
                             && (lhs.actor_random_number_seed == rhs.actor_random_number_seed)
                             && (lhs.persistent_state_change_stack_header == rhs.persistent_state_change_stack_header)
                             && (lhs.decider_state_stack_header == rhs.decider_state_stack_header)
                             && (lhs.action_state_stack_header == rhs.action_state_stack_header)
                             && (lhs.decider_guard_stack_header == rhs.decider_guard_stack_header)
                             && (lhs.action_request_stack_header == rhs.action_request_stack_header)
                             && (lhs.cancellation_range == rhs.cancellation_range)
                             && (lhs.cmd == rhs.cmd)
                             && (lhs.execution_state == rhs.execution_state));
        
        return result;
    }
    
    
    
    inline
    bool
    liz_vm_extractable_state_equals(liz_vm_t const& lhs, 
                                    liz_vm_t const& rhs,
                                    liz_shape_atom_t const* shape_atoms,
                                    liz_int_t const shape_atom_count)
    {
        bool const result = ((persistent_state_array_equals(lhs.persistent_state_changes, 
                                                            lhs.persistent_state_change_shape_atom_indices,
                                                            liz_lookaside_stack_count(&lhs.persistent_state_change_stack_header),
                                                            rhs.persistent_state_changes,
                                                            rhs.persistent_state_change_shape_atom_indices,
                                                            liz_lookaside_stack_count(&rhs.persistent_state_change_stack_header),
                                                            shape_atoms,
                                                            shape_atom_count))
                             && (array_equals(lhs.decider_state_shape_atom_indices,
                                              liz_lookaside_stack_count(&lhs.decider_state_stack_header),
                                              rhs.decider_state_shape_atom_indices,
                                              liz_lookaside_stack_count(&rhs.decider_state_stack_header)))
                             && (array_equals(lhs.action_state_shape_atom_indices,
                                              liz_lookaside_stack_count(&lhs.action_state_stack_header),
                                              rhs.action_state_shape_atom_indices,
                                              liz_lookaside_stack_count(&rhs.action_state_stack_header)))
                             && (array_equals(lhs.decider_states,
                                              liz_lookaside_stack_count(&lhs.decider_state_stack_header),
                                              rhs.decider_states,
                                              liz_lookaside_stack_count(&rhs.decider_state_stack_header)))
                             && (array_equals(lhs.action_states,
                                              liz_lookaside_stack_count(&lhs.action_state_stack_header),
                                              rhs.action_states,
                                              liz_lookaside_stack_count(&rhs.action_state_stack_header)))
                             && (array_equals(lhs.action_requests,
                                              liz_lookaside_double_stack_count_low(&lhs.action_request_stack_header),
                                              rhs.action_requests,
                                              liz_lookaside_double_stack_count_low(&rhs.action_request_stack_header)))
                             && (array_equals(lhs.action_requests + liz_lookaside_double_stack_capacity(&lhs.action_request_stack_header) - liz_lookaside_double_stack_count_high(&lhs.action_request_stack_header),
                                              liz_lookaside_double_stack_count_high(&lhs.action_request_stack_header),
                                              rhs.action_requests + liz_lookaside_double_stack_capacity(&rhs.action_request_stack_header) - liz_lookaside_double_stack_count_high(&rhs.action_request_stack_header),
                                              liz_lookaside_double_stack_count_high(&rhs.action_request_stack_header)))
                             && (lhs.actor_random_number_seed == rhs.actor_random_number_seed)
                             && (lhs.persistent_state_change_stack_header == rhs.persistent_state_change_stack_header)
                             && (lhs.decider_state_stack_header == rhs.decider_state_stack_header)
                             && (lhs.action_state_stack_header == rhs.action_state_stack_header)
                             && (lhs.action_request_stack_header == rhs.action_request_stack_header));
        
        return result;
    }
    
    
    
    class liz_vm_comparator {
    public:
        
        // Does not take over ownership or memory management of data.
        void set(liz_vm_t const* vm,
                 liz_shape_atom_t const* shape_atoms,
                 liz_int_t const shape_atom_count)
        {
            vm_ = vm;
            shape_atoms_ = shape_atoms;
            shape_atom_count_ = shape_atom_count;
        }
        
        
        bool operator==(liz_vm_comparator const& other) const
        {
            bool const result = ((shape_atoms_ == other.shape_atoms_)
                                 && (shape_atom_count_ == other.shape_atom_count_)
                                 && (liz_vm_equals(*vm_,
                                                   *other.vm_,
                                                   shape_atoms_,
                                                   shape_atom_count_)));
            
            return result;
        }
        
        liz_vm_t const* vm_;
        liz_shape_atom_t const* shape_atoms_;
        liz_int_t shape_atom_count_;
    };
    
    
    
    class liz_vm_extractable_state_comparator {
    public:
        
        // Does not take over ownership or memory management of data.
        void set(liz_vm_t const* vm,
                 liz_shape_atom_t const* shape_atoms,
                 liz_int_t const shape_atom_count)
        {
            vm_ = vm;
            shape_atoms_ = shape_atoms;
            shape_atom_count_ = shape_atom_count;
        }
        
        
        bool operator==(liz_vm_extractable_state_comparator const& other) const
        {
            bool const result = ((shape_atoms_ == other.shape_atoms_)
                                 && (shape_atom_count_ == other.shape_atom_count_)
                                 && (liz_vm_extractable_state_equals(*vm_,
                                                                     *other.vm_,
                                                                     shape_atoms_,
                                                                     shape_atom_count_)));
            
            return result;
        }
        
        liz_vm_t const* vm_;
        liz_shape_atom_t const* shape_atoms_;
        liz_int_t shape_atom_count_;
    };
    
    
    
    
    static char const* LIZ_VM_PRINT_FIELD_SEPARATOR = "\n";
    
    inline
    char const*
    liz_action_request_type_to_c_str(liz_action_request_type_t const type)
    {
        char const* result = NULL;
        
        switch (type) {
            case liz_action_request_type_launch:
                result = "liz_action_request_type_launch";
                break;
            case liz_action_request_type_cancel:
                result = "liz_action_request_type_cancel";
                break;
            case liz_action_request_type_remap_shape_atom_index:
                result = "liz_action_request_type_remap_shape_atom_index";
                break;
            default:
                result = "unknown";
                break;
        }
        
        return result;
    }
    
    
    
    inline
    char const*
    liz_node_type_to_c_str(liz_node_type_t const type)
    {
        char const* result = NULL;
        
        switch (type) {
            case liz_node_type_immediate_action:
                result = "liz_node_type_immediate_action";
                break;
            case liz_node_type_deferred_action:
                result = "liz_node_type_deferred_action";
                break;
            case liz_node_type_persistent_action:
                result = "liz_node_type_persistent_action";
                break;
            case liz_node_type_sequence_decider:
                result = "liz_node_type_sequence_decider";
                break;
            case liz_node_type_dynamic_priority_decider:
                result = "liz_node_type_dynamic_priority_decider";
                break;
            case liz_node_type_concurrent_decider:
                result = "liz_node_type_concurrent_decider";
                break;
            default:
                result = "unknown";
                break;
        }
        
        return result;
    }
    
    
    inline
    char const*
    liz_execution_state_to_c_str(liz_execution_state_t const state)
    {
        char const* result = NULL;
        
        switch (state) {
            case liz_execution_state_launch:
                result = "liz_execution_state_launch";
                break;
            case liz_execution_state_running:
                result = "liz_execution_state_running";
                break;
            case liz_execution_state_success:
                result = "liz_execution_state_success";
                break;
            case liz_execution_state_fail:
                result = "liz_execution_state_fail";
                break;
            case liz_execution_state_cancel:
                result = "liz_execution_state_cancel";
                break;
            default:
                result = "unknown";
                break;
        }
        
        return result;
    }
    
    
    inline
    char const*
    liz_vm_cmd_to_c_str(liz_vm_cmd_t const cmd) 
    {
        char const* result = NULL;
        
        switch (cmd) {
            case liz_vm_cmd_invoke_node:
                result = "liz_vm_cmd_invoke_node";
                break;
            case liz_vm_cmd_guard_decider:
                result = "liz_vm_cmd_guard_decider";
                break;
            case liz_vm_cmd_cleanup:
                result = "liz_vm_cmd_cleanup";
                break;
            case liz_vm_cmd_done:
                result = "liz_vm_cmd_done";
                break;
            case liz_vm_cmd_error:
                result = "liz_vm_cmd_error";
                break;
            default:
                result = "unknown";
                break;
        }
        
        return result;
    }
    
    
    inline
    UnitTest::MemoryOutStream&
    operator<<(UnitTest::MemoryOutStream& mos, liz_vm_monitor_node_flag_t const flag)
    {
        char const* pre_seperator = "";
        
        if (0 == flag) {
            mos << "liz_vm_monitor_node_flag_ignore";
            
            pre_seperator = "|";
        }
        
        if (liz_vm_monitor_node_flag_enter_from_top & flag) {
            mos << pre_seperator;
            mos << "liz_vm_monitor_node_flag_enter_from_top";
            pre_seperator = "|";
        }
        
        if (liz_vm_monitor_node_flag_leave_to_top & flag) {
            mos << pre_seperator;
            mos << "liz_vm_monitor_node_flag_leave_to_top";
            pre_seperator = "|";
        }
        
        if (liz_vm_monitor_node_flag_enter_from_bottom & flag) {
            mos << pre_seperator;
            mos << "liz_vm_monitor_node_flag_enter_from_bottom";
            pre_seperator = "|";
        }
        
        if (liz_vm_monitor_node_flag_leave_to_bottom & flag) {
            mos << pre_seperator;
            mos << "liz_vm_monitor_node_flag_leave_to_bottom";
            pre_seperator = "|";
        }
        
        if (liz_vm_monitor_node_flag_cancel_action & flag) {
            mos << pre_seperator;
            mos << "liz_vm_monitor_node_flag_cancel_action";
            pre_seperator = "|";
        }
        
        if (liz_vm_monitor_node_flag_error & flag) {
            mos << pre_seperator;
            mos << "liz_vm_monitor_node_flag_error";
            pre_seperator = "|";
        }
        
        return mos;
    }
    
    
    inline 
    UnitTest::MemoryOutStream& 
    operator<<(UnitTest::MemoryOutStream& mos, liz_lookaside_stack_t const& stack) 
    {
        mos << "{capacity: " << stack.capacity;
        mos << " count: " << stack.count;
        mos << "}";
        
        return mos;
    }
    
    
    inline
    UnitTest::MemoryOutStream& 
    operator<<(UnitTest::MemoryOutStream& mos, liz_lookaside_double_stack_t const& stack) 
    {
        mos << "{capacity: " << stack.capacity;
        mos << " count_low: " << stack.count_low;
        mos << " count_high: " << stack.count_high;
        mos << "}";
        
        return mos;
    }
    
    
    inline
    UnitTest::MemoryOutStream&
    operator<<(UnitTest::MemoryOutStream& mos, liz_vm_cancellation_range_t const& cr) 
    {
        mos << "{begin_index: " << cr.begin_index;
        mos << " end_index: " << cr.end_index;
        mos << "}";
        
        return mos;
    }
    
    
    inline
    UnitTest::MemoryOutStream& 
    operator<<(UnitTest::MemoryOutStream& mos, liz_shape_specification_t const& spec) 
    {
        mos << "{shape_atom_count: " << spec.shape_atom_count;
        mos << " immediate_action_function_count: " << spec.immediate_action_function_count;
        mos << " persistent_state_count: " << spec.persistent_state_count;
        mos << " decider_state_capacity: " << spec.decider_state_capacity;
        mos << " action_state_capacity:" << spec.action_state_capacity;
        mos << " persistent_state_change_capacity: " << spec.persistent_state_change_capacity;
        mos << " decider_guard_Capacity: " << spec.decider_guard_capacity;
        mos << " action_request_capacity: " << spec.action_request_capacity;
        mos << "}";
        
        return mos;
    }
    
    
    inline
    UnitTest::MemoryOutStream& 
    operator<<(UnitTest::MemoryOutStream& mos, liz_action_request_t const& req) 
    {
        mos << "{actor_id: " << req.actor_id;
        mos << " action_id: " << req.action_id;
        mos << " parameter: " << req.parameter;
        mos << " shape_atom_index: " << req.shape_atom_index;
        mos << " type: " << liz_action_request_type_to_c_str(static_cast<liz_action_request_type_t>(req.type));
        mos << "}";
        
        return mos;
    }
    
    
    inline
    UnitTest::MemoryOutStream& 
    operator<<(UnitTest::MemoryOutStream& mos, liz_action_state_update_t const& update) 
    {
        mos << "{actor_id: " << update.actor_id;
        mos << " shape_atom_index: " << update.shape_atom_index;
        mos << " state: " << liz_execution_state_to_c_str(static_cast<liz_execution_state_t>(update.state));
        mos << "}";
        
        return mos;
    };
    
    
    
    inline
    UnitTest::MemoryOutStream& 
    operator<<(UnitTest::MemoryOutStream& mos, liz_vm_action_request_t const& req) 
    {
        mos << "{action_id: " << req.action_id;
        mos << " resource_id: " << req.resource_id;
        mos << " shape_atom_index: " << req.shape_atom_index;
        mos << "}";
        
        return mos;
    };
    
    
    
    inline
    UnitTest::MemoryOutStream& 
    operator<<(UnitTest::MemoryOutStream& mos, liz_vm_decider_guard_t const& guard) 
    {
        mos << "{ shape_atom_index: " << guard.shape_atom_index;
        mos << " end_index: " << guard.end_index;
        mos << " decider_state_rollback_marker: " << guard.decider_state_rollback_marker;
        mos << " action_launch_request_rollback_marker: " << guard. action_launch_request_rollback_marker;
        mos << " sequence_reached_child_index: " << guard.sequence_reached_child_index;
        mos << " concurrency_execution_state: " << guard.concurrent_execution_state;
        mos << " type: " << liz_node_type_to_c_str(static_cast<liz_node_type_t>(guard.type));
        mos << "}";
        
        return mos;
    };
    
    
    inline
    UnitTest::MemoryOutStream& 
    operator<<(UnitTest::MemoryOutStream& mos, liz_vm_monitor_t const& monitor) 
    {
        mos << "{ userdata: " << monitor.user_data;
        mos << " func: " << reinterpret_cast<void*>(monitor.func);
        mos << "}";
        
        return mos;
    };
    
    
    inline
    void
    persistent_state_array_print(UnitTest::MemoryOutStream& mos, 
                                 liz_persistent_state_t const* states,
                                 uint16_t const* shape_atom_indices,
                                 liz_int_t const count,
                                 liz_shape_atom_t const* shape_atoms,
                                 liz_int_t const shape_atom_count)
    {
        std::ostringstream os;
        os << "[";
        
        char const* separator = "";
        
        for (liz_int_t i = 0; i < count; ++i) {
            
            os << separator;
            
            liz_int_t const shape_atom_index = shape_atom_indices[i];
            
            if (shape_atom_index >= shape_atom_count) {
                os << " shape_atom_index: " << shape_atom_index << "out of range error";
            }
            
            switch (shape_atoms[shape_atom_index].type_mask.type) {
                case liz_node_type_persistent_action:
                    os << " shape_atom_index: " << shape_atom_index;
                    os << " shape atom type: " << liz_node_type_to_c_str(static_cast<liz_node_type_t>(shape_atoms[shape_atom_index].type_mask.type));
                    os << " state: " << liz_execution_state_to_c_str(static_cast<liz_execution_state_t>(states[i].persistent_action.state));
                    break;
                default:
                    os << " shape_atom_index: " << shape_atom_index;
                    os << " shape atom type: unknown type error.";
                    break;
            }
            
            separator = ", ";
        }
        
        os << "]";
        
        mos << os.str().c_str();
    }
    
    
    
    inline
    UnitTest::MemoryOutStream&
    operator<<(UnitTest::MemoryOutStream& mos, liz_persistent_state_comparator const& persistent_state_comparator)
    {
        persistent_state_array_print(mos,
                                     persistent_state_comparator.values_,
                                     persistent_state_comparator.keys_,
                                     persistent_state_comparator.count_,
                                     persistent_state_comparator.shape_atoms_,
                                     persistent_state_comparator.shape_atom_count_);
        
        return mos;
    }
    
    
    
    template<typename T>
    void
    array_print(UnitTest::MemoryOutStream& mos, 
                T const* values, 
                liz_int_t const count)
    {
        mos << "[";
        
        char const* separator = "";
        
        for (liz_int_t i = 0; i < count; ++i) {
            mos << separator;
            mos << values[i];
            separator = ", ";
        }
        
        mos << "]";
    }
    
    
    
    inline
    UnitTest::MemoryOutStream&
    operator<<(UnitTest::MemoryOutStream& mos, liz_actor_header_t const& actor_header)
    {
        mos << "{user_data: " << actor_header.user_data;
        mos << " placeholder_for_random_number_seed: " << actor_header.placeholder_for_random_number_seed;
        mos << " actor_id: " << actor_header.actor_id;
        mos << " decider_state_count: " << actor_header.decider_state_count;
        mos << " action_state_count: " << actor_header.action_state_count;
        
        return mos;
    }
    
    
    
    inline
    void
    liz_vm_actor_print(UnitTest::MemoryOutStream& mos,
                       liz_vm_actor_t const& actor,
                       uint16_t const* persistent_state_shape_atom_indices,
                       liz_int_t const persistent_state_count,
                       liz_shape_atom_t const* shape_atoms,
                       liz_int_t const shape_atom_count)
    {
        mos << "{header: " << *(actor.header);
        mos << " persistent_states (w/o indices): ";
        persistent_state_array_print(mos,
                                     actor.persistent_states,
                                     persistent_state_shape_atom_indices,
                                     persistent_state_count,
                                     shape_atoms,
                                     shape_atom_count);
        mos << " decider_state_shape_atom_indices: ";
        array_print(mos, 
                    actor.decider_state_shape_atom_indices, 
                    actor.header->decider_state_count);
        
        mos << " decider_states: ";
        array_print(mos, 
                    actor.decider_states, 
                    actor.header->decider_state_count);
        
        mos << " action_state_shape_atom_indices: ";
        array_print(mos,
                    actor.action_state_shape_atom_indices,
                    actor.header->action_state_count);
        
        mos << " action_states: ";
        array_print(mos,
                    actor.action_states,
                    actor.header->action_state_count);
        
        mos << "}";
    }
    
    
    
    inline
    UnitTest::MemoryOutStream&
    operator<<(UnitTest::MemoryOutStream& mos, 
               liz_vm_actor_comparator const& ac)
    {
        liz_vm_actor_print(mos,
                           *(ac.actor_),
                           ac.shape_->persistent_state_shape_atom_indices,
                           ac.shape_->spec.persistent_state_count,
                           ac.shape_->atoms,
                           ac.shape_->spec.shape_atom_count);
        
        return mos;
    }
    
    
    
    inline
    void
    liz_vm_print(UnitTest::MemoryOutStream& mos,
                 liz_vm_t const& vm,
                 liz_shape_atom_t const* shape_atoms,
                 liz_int_t const shape_atom_count)
    {
        mos << LIZ_VM_PRINT_FIELD_SEPARATOR << "{" << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " shape_atom_index: " << vm.shape_atom_index << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " actor_decider_state_index: " << vm.actor_decider_state_index << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " actor_action_state_index: " << vm.actor_action_state_index << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " actor_persistent_state_index: " << vm.actor_persistent_state_index << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " persistent_state_change_stack_header: " << vm.persistent_state_change_stack_header << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " persistent state changes (indices and states): ";
        persistent_state_array_print(mos,
                                     vm.persistent_state_changes,
                                     vm.persistent_state_change_shape_atom_indices,
                                     liz_lookaside_stack_count(&vm.persistent_state_change_stack_header),
                                     shape_atoms, 
                                     shape_atom_count);
        mos << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " decider_state_stack_header: " << vm.decider_state_stack_header << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " decider_states: ";
        array_print(mos, 
                    vm.decider_states, 
                    liz_lookaside_stack_count(&vm.decider_state_stack_header));
        mos << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " decider_state_shape_atom_indices: ";
        array_print(mos,
                    vm.decider_state_shape_atom_indices, 
                    liz_lookaside_stack_count(&vm.decider_state_stack_header));
        mos << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " action_state_stack_header: " << vm.action_state_stack_header << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " action_states: [";
        
        char const* separator = "";
        for (liz_int_t i = 0; i < liz_lookaside_stack_count(&vm.action_state_stack_header); ++i) {
            mos << separator;
            mos << liz_execution_state_to_c_str(static_cast<liz_execution_state_t>(vm.action_states[i]));
            separator = ", ";
        }
        mos << "]" << LIZ_VM_PRINT_FIELD_SEPARATOR;
        
        mos << " action_state_shape_atom_indices: ";
        array_print(mos,
                    vm.action_state_shape_atom_indices,
                    liz_lookaside_stack_count(&vm.action_state_stack_header));
        mos << LIZ_VM_PRINT_FIELD_SEPARATOR;
        
        mos << " decider_guard_stack_header: " << vm.decider_guard_stack_header << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " decider_guards: ";
        array_print(mos,
                    vm.decider_guards,
                    liz_lookaside_stack_count(&vm.decider_guard_stack_header));
        mos << LIZ_VM_PRINT_FIELD_SEPARATOR;
        
        mos << " action_request_stack_header: " << vm.action_request_stack_header << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " action launch requests: ";
        array_print(mos,
                    vm.action_requests,
                    liz_lookaside_double_stack_count(&vm.action_request_stack_header,
                                                     LIZ_VM_ACTION_REQUEST_STACK_SIDE_LAUNCH));
        mos << LIZ_VM_PRINT_FIELD_SEPARATOR;
        
        mos << " action cancel requests: ";
        if (0 < liz_lookaside_double_stack_count(&vm.action_request_stack_header,
                                                 LIZ_VM_ACTION_REQUEST_STACK_SIDE_CANCEL)) {
            array_print(mos,
                        vm.action_requests + liz_lookaside_double_stack_top_index(&vm.action_request_stack_header,
                                                                                  LIZ_VM_ACTION_REQUEST_STACK_SIDE_CANCEL),
                        liz_lookaside_double_stack_count(&vm.action_request_stack_header,
                                                         LIZ_VM_ACTION_REQUEST_STACK_SIDE_CANCEL));
        } else {
            mos << "[]";
        }
        mos << LIZ_VM_PRINT_FIELD_SEPARATOR;
        
        mos << " actor_random_number_seed: " << vm.actor_random_number_seed << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " cancellation_range: " << vm.cancellation_range << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " cmd: " << liz_vm_cmd_to_c_str(static_cast<liz_vm_cmd_t>(vm.cmd)) << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << " execution_state: " << liz_execution_state_to_c_str(static_cast<liz_execution_state_t>(vm.execution_state)) << LIZ_VM_PRINT_FIELD_SEPARATOR;
        mos << "}" << LIZ_VM_PRINT_FIELD_SEPARATOR;
    }
    
    
    
    inline
    UnitTest::MemoryOutStream&
    operator<<(UnitTest::MemoryOutStream& mos, liz_vm_comparator const& vm_comparator)
    {
        liz_vm_print(mos,
                     *vm_comparator.vm_,
                     vm_comparator.shape_atoms_,
                     vm_comparator.shape_atom_count_);
        
        return mos;
    }
    
    
    
    inline
    UnitTest::MemoryOutStream&
    operator<<(UnitTest::MemoryOutStream& mos, liz_vm_extractable_state_comparator const& vm_comparator)
    {
        liz_vm_print(mos,
                     *vm_comparator.vm_,
                     vm_comparator.shape_atoms_,
                     vm_comparator.shape_atom_count_);
        
        return mos;
    }
    
    
    
    
    class counting_allocator {
    public:
        
        counting_allocator()
        :   requested_alloc_count(0)
        ,   requested_dealloc_count(0)
        {}
        
        
        ~counting_allocator()
        {
            assert(is_balanced());
        }
        
        
        void* 
        alloc(size_t bytes)
        {
            void* tmp = std::malloc(bytes + 2 * sizeof(std::size_t));
            
            if (NULL != tmp) {
                std::size_t* header = static_cast<std::size_t*>(tmp);
                *header = bytes;
                ++header;
                *header = feeble_validation_token;
                
                this->requested_alloc_count += bytes;
                
                tmp = (++header);
            }
            
            return tmp;
        }
        
        void 
        dealloc(void* ptr)
        {
            if (NULL != ptr) {
                std::size_t* header = static_cast<std::size_t*>(ptr);
                header = header - 2;
                
                if (feeble_validation_token == *(header + 1)) {
                    requested_dealloc_count += *header;
                    
                    
                    std::free(header);
                    
                } else {
                    assert(0 && "Error - ptr allocated by different allocator or invalidated.");
                }
            }
        }
        
        bool 
        is_balanced() const 
        {
            return requested_alloc_count == requested_dealloc_count;
        }
        
        
    private:
        counting_allocator(counting_allocator const&); // =0;
        counting_allocator& operator=(counting_allocator const&); // =0;
        
    private:
        static std::size_t const feeble_validation_token = 42;
        
        std::size_t requested_alloc_count;
        std::size_t requested_dealloc_count;
    };
    
    
    inline 
    void* 
    counting_alloc(void *allocator, size_t bytes)
    {
        counting_allocator* ctxt = static_cast<counting_allocator*>(allocator);
        return ctxt->alloc(bytes);
    }
    
    inline 
    void 
    counting_dealloc(void *allocator, void *ptr)
    {
        counting_allocator* ctxt = static_cast<counting_allocator*>(allocator);
        ctxt->dealloc(ptr);
    }
    
    
    
    
    liz_execution_state_t
    immediate_action_func_identity0(void* actor_blackboard,
                                    liz_random_number_seed_t* rnd_seed,
                                    liz_time_t time,
                                    liz_execution_state_t execution_request)
    {
        liz_int_t const func_specific_index = 0;
        
        (void)rnd_seed;
        (void)time;
        
        liz_execution_state_t* states = static_cast<liz_execution_state_t*>(actor_blackboard);
        states[func_specific_index] = execution_request;
        
        return execution_request;
    }
    
    
    liz_execution_state_t
    immediate_action_func_identity1(void* actor_blackboard,
                                    liz_random_number_seed_t* rnd_seed,
                                    liz_time_t time,
                                    liz_execution_state_t execution_request)
    {
        liz_int_t const func_specific_index = 1;
        
        (void)rnd_seed;
        (void)time;
        
        liz_execution_state_t* states = static_cast<liz_execution_state_t*>(actor_blackboard);
        states[func_specific_index] = execution_request;
        
        return execution_request;
    }
    
    
    liz_execution_state_t
    immediate_action_func_running2(void* actor_blackboard,
                                   liz_random_number_seed_t* rnd_seed,
                                   liz_time_t time,
                                   liz_execution_state_t execution_request)
    {
        liz_int_t const func_specific_index = 2;
        execution_request = liz_execution_state_running;
        
        (void)rnd_seed;
        (void)time;
        
        liz_execution_state_t* states = static_cast<liz_execution_state_t*>(actor_blackboard);
        states[func_specific_index] = execution_request;
        
        return execution_request;
    }
    
    
    liz_execution_state_t
    immediate_action_func_success3(void* actor_blackboard,
                                   liz_random_number_seed_t* rnd_seed,
                                   liz_time_t time,
                                   liz_execution_state_t execution_request)
    {
        liz_int_t const func_specific_index = 3;
        execution_request = liz_execution_state_success;
        
        (void)rnd_seed;
        (void)time;
        
        liz_execution_state_t* states = static_cast<liz_execution_state_t*>(actor_blackboard);
        states[func_specific_index] = execution_request;
        
        return execution_request;
    }
    
    
    liz_execution_state_t
    immediate_action_func_fail4(void* actor_blackboard,
                                liz_random_number_seed_t* rnd_seed,
                                liz_time_t time,
                                liz_execution_state_t execution_request)
    {
        liz_int_t const func_specific_index = 4;
        execution_request = liz_execution_state_fail;
        
        (void)rnd_seed;
        (void)time;
        
        liz_execution_state_t* states = static_cast<liz_execution_state_t*>(actor_blackboard);
        states[func_specific_index] = execution_request;
        
        return execution_request;
    }
    
    
    liz_execution_state_t
    immediate_action_func_cancel5(void* actor_blackboard,
                                  liz_random_number_seed_t* rnd_seed,
                                  liz_time_t time,
                                  liz_execution_state_t execution_request)
    {
        liz_int_t const func_specific_index = 5;
        execution_request = liz_execution_state_cancel;
        
        (void)rnd_seed;
        (void)time;
        
        liz_execution_state_t* states = static_cast<liz_execution_state_t*>(actor_blackboard);
        states[func_specific_index] = execution_request;
        
        return execution_request;
    }
    
    
    class liz_vm_test_fixture {
    public:
        
        enum immediate_action_func_index {
            immediate_action_func_index_identity0 = 0,
            immediate_action_func_index_identity1,
            immediate_action_func_index_running2,
            immediate_action_func_index_success3,
            immediate_action_func_index_fail4,
            immediate_action_func_index_cancel5,
        };
        
        enum target_select {
            target_select_expected_result = 1,
            target_select_proband = 2,
            target_select_both = 3
        };
        
        
        liz_vm_test_fixture()
        :   allocator()
        ,   shape()
        ,   actor()
        ,   expected_result_vm(NULL)
        ,   proband_vm(NULL)
        ,   shape_atoms()
        ,   shape_immediate_action_functions(shape_immediate_action_function_count)
        ,   shape_persistent_state_shape_atom_indices()
        ,   actor_header()
        ,   actor_persistent_states()
        ,   actor_decider_state_shape_atom_indices()
        ,   actor_decider_states()
        ,   actor_action_state_shape_atom_indices()
        ,   actor_action_states()
        ,   expected_result_blackboard()
        ,   proband_blackboard()
        ,   expected_result_vm_comparator()
        ,   proband_vm_comparator()
        ,   expected_result_vm_extractable_state_comparator()
        ,   proband_vm_extractable_state_comparator()
        {
            shape_immediate_action_functions[immediate_action_func_index_identity0] = immediate_action_func_identity0;
            shape_immediate_action_functions[immediate_action_func_index_identity1] = immediate_action_func_identity1;
            shape_immediate_action_functions[immediate_action_func_index_running2] = immediate_action_func_running2;
            shape_immediate_action_functions[immediate_action_func_index_success3] = immediate_action_func_success3;
            shape_immediate_action_functions[immediate_action_func_index_fail4] = immediate_action_func_fail4;
            shape_immediate_action_functions[immediate_action_func_index_cancel5] = immediate_action_func_cancel5;
            
            shape.immediate_action_functions = &shape_immediate_action_functions[0];
            
            shape.spec.immediate_action_function_count = shape_immediate_action_function_count;
            
            actor_header.user_data = reinterpret_cast<uintptr_t>(proband_blackboard);
            actor_header.placeholder_for_random_number_seed = 0;
            actor_header.actor_id = 0;
            actor_header.decider_state_count = 0;
            actor_header.action_state_count = 0;
            actor.header = &actor_header;
        }
        
        
        virtual ~liz_vm_test_fixture()
        {
            liz_vm_destroy(expected_result_vm,
                           &allocator,
                           counting_dealloc);
            
            
            liz_vm_destroy(proband_vm,
                           &allocator,
                           counting_dealloc);
            
            expected_result_vm = NULL;
            proband_vm = NULL;
            
            assert(allocator.is_balanced());
        }
        
        
        void grow_shape_atom_stream(std::size_t const new_size) 
        {
            if (new_size > shape_atoms.size()) {
                shape_atoms.resize(new_size);
            }
        }
        
        
        void push_shape_immediate_action(uint16_t const immediate_action_function_index)
        {
            assert(shape.spec.immediate_action_function_count > immediate_action_function_index);
            
            liz_int_t insertion_index = shape.spec.shape_atom_count;
            
            shape.spec.shape_atom_count += LIZ_NODE_SHAPE_ATOM_COUNT_IMMEDIATE_ACTION + 0 * LIZ_NODE_SHAPE_ATOM_COUNT_IMMEDIATE_ACTION_TWO_CHILDREN;
            
            grow_shape_atom_stream(shape.spec.shape_atom_count);
            
            liz_shape_atom_stream_add_immediate_action(&shape_atoms[0],
                                                       &insertion_index,
                                                       shape.spec.shape_atom_count,
                                                       immediate_action_function_index);
            
            // Relink to the vector storage in case the resize needed to
            // create a larger internal array and destroyed the smaller old 
            // one.
            shape.atoms = &shape_atoms[0];
            
            
            
            // Highly simplified shape spec calculation - no optimization 
            // for size in memory.
            assert(LIZ_COUNT_MAX > shape.spec.action_state_capacity);
            shape.spec.action_state_capacity += 1;
            
            // Filling state and shape atom index containers up with zeros.
            // The actor and the vms manage the prepared storage afterwards
            // on their own.
            actor_action_state_shape_atom_indices.push_back(0);
            actor_action_states.push_back(0);
            
            // Relink to the vector storage in case the push back needed to
            // create a larger internal array and destroyed the smaller old 
            // one.
            actor.action_state_shape_atom_indices = &actor_action_state_shape_atom_indices[0];
            actor.action_states = &actor_action_states[0];
        }
        
        
        void push_shape_deferred_action(uint32_t const action_id,
                                        uint16_t const resource_id)
        {
            liz_int_t insertion_index = shape.spec.shape_atom_count;
            
            shape.spec.shape_atom_count += LIZ_NODE_SHAPE_ATOM_COUNT_DEFERRED_ACTION + 0 * LIZ_NODE_SHAPE_ATOM_COUNT_DEFERRED_ACTION_TWO_CHILDREN;
            
            grow_shape_atom_stream(shape.spec.shape_atom_count);
            
            liz_shape_atom_stream_add_deferred_action(&shape_atoms[0],
                                                      &insertion_index,
                                                      shape.spec.shape_atom_count,
                                                      action_id,
                                                      resource_id);
            
            // Relink to the vector storage in case the resize needed to
            // create a larger internal array and destroyed the smaller old 
            // one.
            shape.atoms = &shape_atoms[0];
            
            
            
            // Highly simplified shape spec calculation - no optimization 
            // for size in memory.
            assert(LIZ_COUNT_MAX > shape.spec.action_state_capacity);
            shape.spec.action_state_capacity += 1u;
            
            assert(LIZ_COUNT_MAX > shape.spec.action_request_capacity);
            shape.spec.action_request_capacity += 1u;
            
            // Filling state and shape atom index containers up with zeros.
            // The actor and the vms manage the prepared storage afterwards
            // on their own.
            actor_action_state_shape_atom_indices.push_back(0);
            actor_action_states.push_back(0);
            
            // Relink to the vector storage in case the push back needed to
            // create a larger internal array and destroyed the smaller old 
            // one.
            actor.action_state_shape_atom_indices = &actor_action_state_shape_atom_indices[0];
            actor.action_states = &actor_action_states[0];
        }
        
        
        void push_shape_persistent_action()
        {
            liz_int_t insertion_index = shape.spec.shape_atom_count;
            liz_int_t const persistent_action_shape_atom_index = shape.spec.shape_atom_count;
            
            shape.spec.shape_atom_count += LIZ_NODE_SHAPE_ATOM_COUNT_PERSISTENT_ACTION + 0 * LIZ_NODE_SHAPE_ATOM_COUNT_PERSISTENT_ACTION_TWO_CHILDREN;
            
            grow_shape_atom_stream(shape.spec.shape_atom_count);
            
            liz_shape_atom_stream_add_persistent_action(&shape_atoms[0],
                                                        &insertion_index,
                                                        shape.spec.shape_atom_count);
            
            // Relink to the vector storage in case the resize needed to
            // create a larger internal array and destroyed the smaller old 
            // one.
            shape.atoms = &shape_atoms[0];
            
            assert(LIZ_COUNT_MAX > shape.spec.persistent_state_count);
            shape.spec.persistent_state_count += 1u;
            
            shape_persistent_state_shape_atom_indices.push_back(static_cast<uint16_t>(persistent_action_shape_atom_index));
            
            // Relink to the vector storage in case the push back needed to
            // create a larger internal array and destroyed the smaller old 
            // one.
            shape.persistent_state_shape_atom_indices = &shape_persistent_state_shape_atom_indices[0];
            
            
            actor_persistent_states.resize(shape.spec.persistent_state_count);
            
            // Relink to the vector storage in case the push back needed to
            // create a larger internal array and destroyed the smaller old 
            // one.
            actor.persistent_states = &actor_persistent_states[0];
        }
        
        
        
        void push_shape_sequence_decider(uint16_t const sub_stream_end_offset)
        {
            assert(shape.spec.shape_atom_count + sub_stream_end_offset > LIZ_NODE_SHAPE_ATOM_COUNT_SEQUENCE_DECIDER + 0 * LIZ_NODE_SHAPE_ATOM_COUNT_SEQUENCE_DECIDER_TWO_CHILDREN);
            
            liz_int_t insertion_index = shape.spec.shape_atom_count;
            
            shape.spec.shape_atom_count += LIZ_NODE_SHAPE_ATOM_COUNT_SEQUENCE_DECIDER + 0 * LIZ_NODE_SHAPE_ATOM_COUNT_SEQUENCE_DECIDER_TWO_CHILDREN;
            
            grow_shape_atom_stream(insertion_index + sub_stream_end_offset);
            
            liz_shape_atom_stream_add_sequence_decider(&shape_atoms[0],
                                                       &insertion_index,
                                                       static_cast<liz_int_t>(shape_atoms.size()),
                                                       sub_stream_end_offset);
            
            // Relink to the vector storage in case the resize needed to
            // create a larger internal array and destroyed the smaller old 
            // one.
            shape.atoms = &shape_atoms[0];
            
            assert(LIZ_COUNT_MAX > shape.spec.decider_state_capacity);
            shape.spec.decider_state_capacity += 1u;
            
            assert(LIZ_COUNT_MAX > shape.spec.decider_guard_capacity);
            shape.spec.decider_guard_capacity += 1u;
            
            actor_decider_states.resize(shape.spec.decider_state_capacity);
            actor_decider_state_shape_atom_indices.resize(shape.spec.decider_state_capacity);
            
            // Relink to the vector storage in case the resize needed to
            // create a larger internal array and destroyed the smaller old 
            // one.
            actor.decider_states = &actor_decider_states[0];
            actor.decider_state_shape_atom_indices = &actor_decider_state_shape_atom_indices[0];
        }
        
        
        
        void push_shape_dynamic_priority_decider(uint16_t const sub_stream_end_offset)
        {
            assert(shape.spec.shape_atom_count + sub_stream_end_offset > LIZ_NODE_SHAPE_ATOM_COUNT_DYNAMIC_PRIORITY_DECIDER + 0 * LIZ_NODE_SHAPE_ATOM_COUNT_DYNAMIC_PRIORITY_DECIDER_TWO_CHILDREN);
            
            liz_int_t insertion_index = shape.spec.shape_atom_count;
            
            shape.spec.shape_atom_count += LIZ_NODE_SHAPE_ATOM_COUNT_DYNAMIC_PRIORITY_DECIDER + 0 * LIZ_NODE_SHAPE_ATOM_COUNT_DYNAMIC_PRIORITY_DECIDER_TWO_CHILDREN;
            
            grow_shape_atom_stream(insertion_index + sub_stream_end_offset);
            
            liz_shape_atom_stream_add_dynamic_priority_decider(&shape_atoms[0],
                                                               &insertion_index,
                                                               static_cast<liz_int_t>(shape_atoms.size()),
                                                               sub_stream_end_offset);
            
            // Relink to the vector storage in case the resize needed to
            // create a larger internal array and destroyed the smaller old 
            // one.
            shape.atoms = &shape_atoms[0];
            
            assert(LIZ_COUNT_MAX > shape.spec.decider_guard_capacity);
            shape.spec.decider_guard_capacity += 1u;
        }
        
        
        void push_shape_concurrent_decider(uint16_t const sub_stream_end_offset)
        {
            assert(shape.spec.shape_atom_count + sub_stream_end_offset > LIZ_NODE_SHAPE_ATOM_COUNT_CONCURRENT_DECIDER + 0 * LIZ_NODE_SHAPE_ATOM_COUNT_CONCURRENT_DECIDER_TWO_CHILDREN);
            
            liz_int_t insertion_index = shape.spec.shape_atom_count;
            
            shape.spec.shape_atom_count += LIZ_NODE_SHAPE_ATOM_COUNT_CONCURRENT_DECIDER + 0 * LIZ_NODE_SHAPE_ATOM_COUNT_CONCURRENT_DECIDER_TWO_CHILDREN;
            
            grow_shape_atom_stream(insertion_index + sub_stream_end_offset);
            
            liz_shape_atom_stream_add_concurrent_decider(&shape_atoms[0],
                                                         &insertion_index,
                                                         static_cast<liz_int_t>(shape_atoms.size()),
                                                         sub_stream_end_offset);
            
            // Relink to the vector storage in case the resize needed to
            // create a larger internal array and destroyed the smaller old 
            // one.
            shape.atoms = &shape_atoms[0];
            
            assert(LIZ_COUNT_MAX > shape.spec.decider_guard_capacity);
            shape.spec.decider_guard_capacity += 1u;
        }
        
        
        
        
        
        
        void push_actor_action_state(uint16_t const shape_atom_index,
                                     uint8_t const state)
        {
            assert(actor_action_states.size() == actor_action_state_shape_atom_indices.size());
            assert(shape.spec.action_state_capacity == actor_action_states.capacity());
            assert(actor_action_state_shape_atom_indices.capacity() == shape.spec.action_state_capacity);
            
            actor_action_states[actor_header.action_state_count] = state;
            actor_action_state_shape_atom_indices[actor_header.action_state_count] = shape_atom_index;
            
            assert(actor_header.action_state_count < shape.spec.action_state_capacity);
            actor_header.action_state_count += 1u;
        }
        
        
        
        void push_actor_decider_state(uint16_t const shape_atom_index,
                                      uint16_t const state)
        {
            assert(actor_decider_states.size() == actor_decider_state_shape_atom_indices.size());
            assert(shape.spec.decider_state_capacity == actor_decider_states.capacity());
            assert(actor_decider_state_shape_atom_indices.capacity() == shape.spec.decider_state_capacity);
            
            actor_decider_states[actor_header.decider_state_count] = state;
            actor_decider_state_shape_atom_indices[actor_header.decider_state_count] = shape_atom_index;
            
            assert(actor_header.decider_state_count < shape.spec.decider_state_capacity);
            actor_header.decider_state_count += 1u;
        }
        
        
        void set_actor_persistent_state(uint16_t const persistent_state_index,
                                        uint16_t const shape_atom_index,
                                        liz_execution_state_t const exec_state)
        {
            assert(actor_persistent_states.size() == shape_persistent_state_shape_atom_indices.size());
            assert(shape.spec.persistent_state_count == actor_persistent_states.size());
            assert(actor_persistent_states.capacity() == shape.spec.persistent_state_count);
            assert(persistent_state_index < shape.spec.persistent_state_count);
            assert(shape_atom_index < shape.spec.shape_atom_count);
            assert(shape_persistent_state_shape_atom_indices[persistent_state_index] == shape_atom_index);
            
            actor_persistent_states[persistent_state_index].persistent_action.state = static_cast<uint8_t>(exec_state);
        }
        
        
        // Create the expected result and the tested vm after establishing
        // the shape (and don't call the method again for the instance of 
        // cancel_test_fixture).
        void create_expected_result_and_proband_vms_for_shape()
        {
            assert(NULL == expected_result_vm);
            assert(NULL == proband_vm);
            assert(shape.spec.shape_atom_count == shape_atoms.size() && "A decider reserved more shape atom capacity than is actually used which might hint at a malformed behavior tree.");
            
            expected_result_vm = liz_vm_create(shape.spec,
                                               &allocator,
                                               counting_alloc);
            
            proband_vm = liz_vm_create(shape.spec,
                                       &allocator,
                                       counting_alloc);
            
            expected_result_vm_comparator.set(expected_result_vm,
                                              shape.atoms,
                                              shape.spec.shape_atom_count);
            proband_vm_comparator.set(proband_vm,
                                      shape.atoms,
                                      shape.spec.shape_atom_count);
            
            expected_result_vm_extractable_state_comparator.set(expected_result_vm,
                                                                shape.atoms,
                                                                shape.spec.shape_atom_count);
            proband_vm_extractable_state_comparator.set(proband_vm,
                                                        shape.atoms,
                                                        shape.spec.shape_atom_count);
        }
        
        
        
        void push_vm_action_state(target_select const target_vm,
                                  uint16_t const shape_atom_index,
                                  uint8_t const state)
        {
            if (target_vm == target_select_expected_result 
                || target_vm == target_select_both) {
                
                liz_vm_t *vm = expected_result_vm;
                
                liz_lookaside_stack_push(&vm->action_state_stack_header);
                liz_int_t const top_index = liz_lookaside_stack_top_index(&vm->action_state_stack_header);
                
                vm->action_states[top_index] = state;
                vm->action_state_shape_atom_indices[top_index] = shape_atom_index;
            }
            
            if (target_vm == target_select_proband 
                || target_vm == target_select_both) {
                
                liz_vm_t *vm = proband_vm;
                
                liz_lookaside_stack_push(&vm->action_state_stack_header);
                liz_int_t const top_index = liz_lookaside_stack_top_index(&vm->action_state_stack_header);
                
                vm->action_states[top_index] = state;
                vm->action_state_shape_atom_indices[top_index] = shape_atom_index;
            }
        }
        
        
        void push_vm_action_launch_request(target_select const target_vm,
                                           uint32_t const action_id,
                                           uint16_t const resource_id,
                                           uint16_t const shape_atom_index)
        {
            if (target_vm == target_select_expected_result 
                || target_vm == target_select_both) {
                
                liz_vm_t *vm = expected_result_vm;
                
                liz_lookaside_double_stack_push(&vm->action_request_stack_header, 
                                                LIZ_VM_ACTION_REQUEST_STACK_SIDE_LAUNCH);
                liz_int_t const top_index = liz_lookaside_double_stack_top_index(&vm->action_request_stack_header, LIZ_VM_ACTION_REQUEST_STACK_SIDE_LAUNCH);
                
                vm->action_requests[top_index] = (liz_vm_action_request_t){
                    action_id,
                    resource_id,
                    shape_atom_index
                };
            }
            
            if (target_vm == target_select_proband 
                || target_vm == target_select_both) {
                
                liz_vm_t *vm = proband_vm;
                
                liz_lookaside_double_stack_push(&vm->action_request_stack_header, 
                                                LIZ_VM_ACTION_REQUEST_STACK_SIDE_LAUNCH);
                liz_int_t const top_index = liz_lookaside_double_stack_top_index(&vm->action_request_stack_header, LIZ_VM_ACTION_REQUEST_STACK_SIDE_LAUNCH);
                
                vm->action_requests[top_index] = (liz_vm_action_request_t){
                    action_id,
                    resource_id,
                    shape_atom_index
                };
            }
        }
        
        
        void push_vm_action_cancel_request(target_select const target_vm,
                                           uint32_t const action_id,
                                           uint16_t const resource_id,
                                           uint16_t const shape_atom_index)
        {
            if (target_vm == target_select_expected_result 
                || target_vm == target_select_both) {
                
                liz_vm_t *vm = expected_result_vm;
                
                liz_lookaside_double_stack_push(&vm->action_request_stack_header, 
                                                LIZ_VM_ACTION_REQUEST_STACK_SIDE_CANCEL);
                liz_int_t const top_index = liz_lookaside_double_stack_top_index(&vm->action_request_stack_header, LIZ_VM_ACTION_REQUEST_STACK_SIDE_CANCEL);
                
                vm->action_requests[top_index] = (liz_vm_action_request_t){
                    action_id,
                    resource_id,
                    shape_atom_index
                };
            }
            
            if (target_vm == target_select_proband 
                || target_vm == target_select_both) {
                
                liz_vm_t *vm = proband_vm;
                
                liz_lookaside_double_stack_push(&vm->action_request_stack_header, 
                                                LIZ_VM_ACTION_REQUEST_STACK_SIDE_CANCEL);
                liz_int_t const top_index = liz_lookaside_double_stack_top_index(&vm->action_request_stack_header, LIZ_VM_ACTION_REQUEST_STACK_SIDE_CANCEL);
                
                vm->action_requests[top_index] = (liz_vm_action_request_t){
                    action_id,
                    resource_id,
                    shape_atom_index
                };
            }
        }
        
        
        
        void push_vm_persistent_action_state_change(target_select const target_vm,
                                                    uint16_t const shape_atom_index,
                                                    liz_execution_state const exec_state)
        {
            assert(shape_atom_index < shape.spec.shape_atom_count);
            
            if (target_select_expected_result == target_vm
                || target_select_both == target_vm) {
                
                liz_lookaside_stack_push(&expected_result_vm->persistent_state_change_stack_header);
                liz_int_t const top_index = liz_lookaside_stack_top_index(&expected_result_vm->persistent_state_change_stack_header);
                
                expected_result_vm->persistent_state_changes[top_index].persistent_action.state = static_cast<uint8_t>(exec_state);
                expected_result_vm->persistent_state_change_shape_atom_indices[top_index] = shape_atom_index;
                
                assert(0 == top_index || expected_result_vm->persistent_state_change_shape_atom_indices[top_index - 1] < shape_atom_index);
            }
            
            if (target_select_proband == target_vm
                || target_select_both == target_vm) {
                
                liz_lookaside_stack_push(&proband_vm->persistent_state_change_stack_header);
                liz_int_t const top_index = liz_lookaside_stack_top_index(&proband_vm->persistent_state_change_stack_header);
                
                proband_vm->persistent_state_changes[top_index].persistent_action.state = static_cast<uint8_t>(exec_state);
                proband_vm->persistent_state_change_shape_atom_indices[top_index] = shape_atom_index;
                
                assert(0 == top_index || proband_vm->persistent_state_change_shape_atom_indices[top_index - 1] < shape_atom_index);
            }
            
        }
        
        
        void push_vm_decider_state(target_select const target_vm,
                                   uint16_t const shape_atom_index,
                                   uint16_t const state)
        {
            assert(shape_atom_index < shape.spec.shape_atom_count);
            
            if (target_select_expected_result == target_vm
                || target_select_both == target_vm) {

                liz_vm_t *vm = expected_result_vm;
                liz_lookaside_stack_t *stack_header = &vm->decider_state_stack_header;
                liz_lookaside_stack_push(stack_header);
                liz_int_t const top_index = liz_lookaside_stack_top_index(stack_header);
                
                vm->decider_state_shape_atom_indices[top_index] = shape_atom_index;
                vm->decider_states[top_index] = state;
            }
            
            if (target_select_proband == target_vm
                || target_select_both == target_vm) {
 
                liz_vm_t *vm = proband_vm;
                liz_lookaside_stack_t *stack_header = &vm->decider_state_stack_header;
                liz_lookaside_stack_push(stack_header);
                liz_int_t const top_index = liz_lookaside_stack_top_index(stack_header);
                
                vm->decider_state_shape_atom_indices[top_index] = shape_atom_index;
                vm->decider_states[top_index] = state;
            }
        }
        
        
        counting_allocator allocator;
        
        liz_vm_shape_t shape;
        liz_vm_actor_t actor;
        
        liz_vm_t* expected_result_vm;
        liz_vm_t* proband_vm;
        
        static int const shape_immediate_action_function_count = 6;
        
        std::vector<liz_shape_atom_t> shape_atoms;
        std::vector<liz_immediate_action_func_t> shape_immediate_action_functions;
        std::vector<uint16_t> shape_persistent_state_shape_atom_indices;
        
        liz_actor_header_t actor_header;
        std::vector<liz_persistent_state_t> actor_persistent_states;
        std::vector<uint16_t> actor_decider_state_shape_atom_indices;
        std::vector<uint16_t> actor_decider_states;
        std::vector<uint16_t> actor_action_state_shape_atom_indices;
        std::vector<uint8_t> actor_action_states;
        
        liz_execution_state_t expected_result_blackboard[shape_immediate_action_function_count];
        liz_execution_state_t proband_blackboard[shape_immediate_action_function_count];
        
        
        liz_vm_comparator expected_result_vm_comparator;
        liz_vm_comparator proband_vm_comparator;
        
        liz_vm_extractable_state_comparator expected_result_vm_extractable_state_comparator;
        liz_vm_extractable_state_comparator proband_vm_extractable_state_comparator;
        
    private:
        liz_vm_test_fixture(liz_vm_test_fixture const&); // = 0
        liz_vm_test_fixture& operator=(liz_vm_test_fixture const&); // = 0
    };
    
    
    
    int const liz_vm_test_fixture::shape_immediate_action_function_count;
    
} // Anonymous namespace


#endif /* LIZ_liz_test_helpers_H */
