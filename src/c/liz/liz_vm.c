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

#include "liz_vm.h"

#include "liz_assert.h"
#include "liz_common.h"
#include "liz_common_internal.h"



#pragma mark Create and destroy vm



size_t
liz_vm_memory_size_requirement(liz_shape_specification_t const spec)
{
    // Size of struct.
    size_t vm_size = sizeof(liz_vm_t);
    
    // Size of shape atom indices.
    vm_size = liz_allocation_size_aggregate(LIZ_VM_ALIGNMENT, 
                                            vm_size,
                                            LIZ_SHAPE_ATOM_INDEX_ALIGNMENT,
                                            sizeof(uint16_t) * spec.persistent_state_change_capacity);
    vm_size = liz_allocation_size_aggregate(LIZ_VM_ALIGNMENT,
                                            vm_size,
                                            LIZ_SHAPE_ATOM_INDEX_ALIGNMENT, 
                                            sizeof(uint16_t) * spec.decider_state_capacity);
    vm_size = liz_allocation_size_aggregate(LIZ_VM_ALIGNMENT,
                                            vm_size,
                                            LIZ_SHAPE_ATOM_INDEX_ALIGNMENT,
                                            sizeof(uint16_t) * spec.action_state_capacity);
    
    // Size of states.
    vm_size = liz_allocation_size_aggregate(LIZ_VM_ALIGNMENT,
                                            vm_size,
                                            LIZ_PERSISTENT_STATE_ALIGNMENT, 
                                            sizeof(liz_persistent_state_t) * spec.persistent_state_change_capacity);
    vm_size = liz_allocation_size_aggregate(LIZ_VM_ALIGNMENT,
                                            vm_size,
                                            LIZ_DECIDER_STATE_ALIGNMENT,
                                            sizeof(uint16_t) * spec.decider_state_capacity);
    vm_size = liz_allocation_size_aggregate(LIZ_VM_ALIGNMENT,
                                            vm_size,
                                            LIZ_ACTION_STATE_ALIGNMENT,
                                            sizeof(uint8_t) * spec.action_state_capacity);
    
    // Size of action requests and guard stack.
    vm_size = liz_allocation_size_aggregate(LIZ_VM_ALIGNMENT,
                                            vm_size,
                                            LIZ_VM_DECIDER_GUARD_ALIGNMENT,
                                            sizeof(liz_vm_decider_guard_t) * spec.decider_guard_capacity);
    vm_size = liz_allocation_size_aggregate(LIZ_VM_ALIGNMENT,
                                            vm_size,
                                            LIZ_VM_ACTION_REQUEST_ALIGNMENT,
                                            sizeof(liz_vm_action_request_t) * spec.action_request_capacity);
    
    // Round up size to enable aligned packing in an array.
    vm_size = liz_allocation_size_aggregate(LIZ_VM_ALIGNMENT,
                                            vm_size,
                                            LIZ_VM_ALIGNMENT,
                                            0);
    
    return vm_size;
}



liz_vm_t*
liz_vm_create(liz_shape_specification_t const spec,
              void * LIZ_RESTRICT allocator_context,
              liz_alloc_func_t alloc_func)
{
    size_t const vm_size = liz_vm_memory_size_requirement(spec);
    liz_vm_t *vm = (liz_vm_t *)alloc_func(allocator_context, vm_size);
    
    if (!vm) {
        return NULL;
    }
    
    // Calculate aligned state addresses.
    char *ptr = (char *)vm + sizeof(liz_vm_t);
    ptr += liz_allocation_alignment_offset(ptr, LIZ_SHAPE_ATOM_INDEX_ALIGNMENT);
    uint16_t *persistent_state_change_shape_atom_indices = (uint16_t *)ptr;
    
    ptr += sizeof(uint16_t) * spec.persistent_state_change_capacity;
    ptr += liz_allocation_alignment_offset(ptr, LIZ_SHAPE_ATOM_INDEX_ALIGNMENT);
    uint16_t *decider_state_shape_atom_indices = (uint16_t *)ptr;
    
    ptr += sizeof(uint16_t) * spec.decider_state_capacity;
    ptr += liz_allocation_alignment_offset(ptr, LIZ_SHAPE_ATOM_INDEX_ALIGNMENT);
    uint16_t *action_state_shape_atom_indices = (uint16_t *)ptr;
    
    ptr += sizeof(uint16_t) * spec.action_state_capacity;
    ptr += liz_allocation_alignment_offset(ptr, LIZ_PERSISTENT_STATE_ALIGNMENT);
    liz_persistent_state_t *persistent_state_changes = (liz_persistent_state_t *)ptr;
    
    ptr += sizeof(liz_persistent_state_t) * spec.persistent_state_change_capacity;
    ptr += liz_allocation_alignment_offset(ptr, LIZ_DECIDER_STATE_ALIGNMENT);
    uint16_t *decider_states = (uint16_t *)ptr;
    
    ptr += sizeof(uint16_t) * spec.decider_state_capacity;
    ptr += liz_allocation_alignment_offset(ptr, LIZ_ACTION_STATE_ALIGNMENT);
    uint8_t *action_states = (uint8_t *)ptr;
    
    ptr += sizeof(uint8_t) * spec.action_state_capacity;
    ptr += liz_allocation_alignment_offset(ptr, LIZ_VM_DECIDER_GUARD_ALIGNMENT);
    liz_vm_decider_guard_t *decider_guards = (liz_vm_decider_guard_t *)ptr;
    
    ptr += sizeof(liz_vm_decider_guard_t) * spec.decider_guard_capacity;
    ptr += liz_allocation_alignment_offset(ptr, LIZ_VM_ACTION_REQUEST_ALIGNMENT);
    liz_vm_action_request_t *action_requests = (liz_vm_action_request_t *)ptr;
    
    liz_vm_init(vm,
                spec,
                persistent_state_change_shape_atom_indices,
                decider_state_shape_atom_indices,
                action_state_shape_atom_indices,
                persistent_state_changes,
                decider_states,
                action_states,
                decider_guards,
                action_requests);
    
    return vm;
}



void
liz_vm_destroy(liz_vm_t *vm,
               void * LIZ_RESTRICT allocator_context,
               liz_dealloc_func_t dealloc_func)
{
    dealloc_func(allocator_context, vm);
}



#pragma mark Update actor



bool
liz_vm_fulfills_shape_specification(liz_vm_t const *vm,
                                    liz_shape_specification_t const spec)
{
    bool result = true;
    result = result && spec.decider_state_capacity <= liz_lookaside_stack_capacity(&vm->decider_state_stack_header);
    result = result && spec.action_state_capacity <= liz_lookaside_stack_capacity(&vm->action_state_stack_header);
    result = result && spec.persistent_state_change_capacity <= liz_lookaside_stack_capacity(&vm->persistent_state_change_stack_header);
    result = result && spec.decider_guard_capacity <= liz_lookaside_stack_capacity(&vm->decider_guard_stack_header);
    result = result && spec.action_request_capacity <= liz_lookaside_double_stack_capacity(&vm->action_request_stack_header);
    
    return result;
}



void
liz_vm_update_actor(liz_vm_t *vm,
                    liz_vm_monitor_t *monitor,
                    void * LIZ_RESTRICT user_data_lookup_context,
                    liz_vm_user_data_lookup_func_t user_data_lookup_func,
                    liz_time_t const time,
                    liz_vm_actor_t *actor,
                    liz_vm_shape_t const *shape)
{
    LIZ_ASSERT(liz_vm_fulfills_shape_specification(vm, shape->spec));
    
    if (0u == shape->spec.shape_atom_count) {
        return;
    }
    
    liz_vm_reset(vm);
    
    void *actor_blackboard = user_data_lookup_func(user_data_lookup_context,
                                                   actor->header->user_data);

    vm->actor_random_number_seed = actor->header->placeholder_for_random_number_seed;
    
    while (LIZ_VM_NEXT_STEP(vm)) {
        vm->cmd = liz_vm_step_cmd(vm,
                                  monitor,
                                  actor_blackboard,
                                  time,
                                  actor,
                                  shape,
                                  vm->cmd);
    }
    
    liz_vm_extract_and_clear_actor_states(vm, actor, shape);
}



void
liz_vm_cancel_actor(liz_vm_t *vm,
                    liz_vm_monitor_t *monitor,
                    void * LIZ_RESTRICT user_data_lookup_context,
                    liz_vm_user_data_lookup_func_t user_data_lookup_func,
                    liz_time_t const time,
                    liz_vm_actor_t *actor,
                    liz_vm_shape_t const *shape)
{
    LIZ_ASSERT(liz_vm_fulfills_shape_specification(vm, shape->spec));
    
    liz_vm_reset(vm);
    
    void *actor_blackboard = user_data_lookup_func(user_data_lookup_context,
                                                   actor->header->user_data);
    
    vm->actor_random_number_seed = actor->header->placeholder_for_random_number_seed;
    vm->cmd = liz_vm_step_cmd(vm,
                              monitor,
                              actor_blackboard,
                              time,
                              actor,
                              shape,
                              liz_vm_cmd_cleanup);
    LIZ_ASSERT(liz_vm_cmd_done == vm->cmd);
    
    liz_vm_extract_and_clear_actor_states(vm, actor, shape);
}



void
liz_vm_extract_and_clear_action_requests(liz_vm_t *vm,
                                         liz_action_request_t *external_requests,
                                         liz_int_t const external_request_capacity,
                                         liz_id_t const actor_id)
{
    LIZ_ASSERT(liz_vm_cmd_done == vm->cmd && "Vm update must have been cleaned up and done before extracting action requests.");
    LIZ_ASSERT(external_request_capacity >= liz_lookaside_double_stack_count_all(&vm->action_request_stack_header));
    
    liz_int_t external_index = 0;
    
    // Extract cancellation requests.
    LIZ_ASSERT(liz_lookaside_double_stack_side_high == LIZ_VM_ACTION_REQUEST_STACK_SIDE_CANCEL
               && "Invalid assumption that cancellation requests are stored at the high side of the stack.");
    liz_int_t const cancel_top_index = liz_lookaside_double_stack_top_index(&vm->action_request_stack_header, LIZ_VM_ACTION_REQUEST_STACK_SIDE_CANCEL);
    for (liz_int_t i = liz_lookaside_double_stack_capacity(&vm->action_request_stack_header);
         i >= cancel_top_index; 
         --i) {

        external_requests[external_index++] = (liz_action_request_t){
            actor_id,
            vm->action_requests[i].action_id,
            vm->action_requests[i].resource,
            vm->action_requests[i].shape_item_index,
            liz_action_request_type_cancel
        };
    }
    
    // Extract launch requests.
    LIZ_ASSERT(liz_lookaside_double_stack_side_low == LIZ_VM_ACTION_REQUEST_STACK_SIDE_LAUNCH
               && "Invalid assumption that cancellation requests are stored at the low side of the stack.");
    liz_int_t const launch_top_index = liz_lookaside_double_stack_top_index(&vm->action_request_stack_header, LIZ_VM_ACTION_REQUEST_STACK_SIDE_LAUNCH);
    for (liz_int_t i = 0; i <= launch_top_index; ++i) {
        
        external_requests[external_index++] = (liz_action_request_t){
            actor_id,
            vm->action_requests[i].action_id,
            vm->action_requests[i].resource,
            vm->action_requests[i].shape_item_index,
            liz_action_request_type_launch
        };
    }
    
    liz_lookaside_double_stack_clear(&vm->action_request_stack_header);
}



void
liz_vm_reset(liz_vm_t *vm)
{
    vm->shape_atom_index = 0;
    vm->actor_decider_state_index = 0;
    vm->actor_action_state_index = 0;
    vm->actor_persistent_state_index = 0;
    
    vm->actor_random_number_seed = 0;
    
    liz_lookaside_stack_clear(&vm->decider_state_stack_header);
    liz_lookaside_stack_clear(&vm->action_state_stack_header);
    liz_lookaside_double_stack_clear(&vm->action_request_stack_header);
    liz_lookaside_stack_clear(&vm->decider_guard_stack_header);
    
    vm->cancellation_range = (liz_vm_cancellation_range_t){0u, 0u};
    
    vm->cmd = liz_vm_cmd_invoke_node;
    vm->execution_state = liz_execution_state_launch;
}



#pragma mark Step actor update by hand


liz_vm_cmd_t
liz_vm_step_cmd(liz_vm_t *vm,
                liz_vm_monitor_t *monitor,
                void * LIZ_RESTRICT actor_blackboard,
                liz_time_t const time,
                liz_vm_actor_t const *actor,
                liz_vm_shape_t const *shape,
                liz_vm_cmd_t const cmd)
{
    liz_vm_cmd_t next_cmd = liz_vm_cmd_done;
    
    switch (cmd) {
        case liz_vm_cmd_invoke_node:
            next_cmd = liz_vm_step_invoke_node(vm,
                                               monitor,
                                               actor_blackboard,
                                               time,
                                               actor,
                                               shape);
            break;
            
        case liz_vm_cmd_guard_decider:
            next_cmd = liz_vm_step_guard_decider(vm,
                                                 monitor,
                                                 actor_blackboard,
                                                 time,
                                                 actor,
                                                 shape);
            break;
            
        case liz_vm_cmd_cleanup:
            liz_vm_step_cleanup(vm,
                                monitor,
                                actor_blackboard,
                                time,
                                actor,
                                shape);
            
        case liz_vm_cmd_done:
            // Nothing to do. Call liz_vm_extract_action_requests yourself.
            next_cmd = liz_vm_cmd_done;
            break;
            
        case liz_vm_cmd_error:
            // An error occured because the shape stream representing a behavior
            // tree is malformed, e.g., a decider has no children.
            LIZ_ASSERT(0 && "Error - behavior tree is malformed.");
            
            // Fall through - errors are not handled and unhandled commands are
            //                errors.
            
        default:
            LIZ_ASSERT(0 && "Unhandled vm command.");
            next_cmd = liz_vm_cmd_error;
            break;
    }
    
    return next_cmd;
}



liz_vm_cmd_t
liz_vm_step_invoke_node(liz_vm_t *vm,
                        liz_vm_monitor_t *monitor,
                        void * LIZ_RESTRICT actor_blackboard,
                        liz_time_t const time,
                        liz_vm_actor_t const *actor,
                        liz_vm_shape_t const *shape)
{
    LIZ_ASSERT(shape->spec.shape_atom_count > vm->shape_atom_index);
    
    liz_int_t const monitored_shape_atom_index = vm->shape_atom_index;
    liz_vm_monitor_node_flag_t traversal_direction = liz_vm_monitor_node_flag_enter_from_top;
    LIZ_VM_MONITOR_NODE(monitor,
                        monitored_shape_atom_index,
                        traversal_direction,
                        vm,
                        actor_blackboard, 
                        time,
                        actor,
                        shape);
    
    liz_vm_cmd_t next_cmd = liz_vm_cmd_error;
    
    switch (shape->atoms[vm->shape_atom_index].type_mask.type) {
        case liz_node_type_immediate_action:
            // Checks for invalid execution states internally.
            liz_vm_invoke_immediate_action(vm,
                                           actor_blackboard,
                                           time,
                                           actor,
                                           shape);
            
            // Traverse up to decider, alas its associated guard.
            next_cmd = liz_vm_cmd_guard_decider;
            traversal_direction = liz_vm_monitor_node_flag_leave_to_top;
            
            break;
            
        case liz_node_type_deferred_action:
            // Checks for invalid execution states internally.
            liz_vm_invoke_deferred_action(vm,
                                          actor,
                                          shape);
            
            // Traverse up to decider, alas its associated guard.
            next_cmd = liz_vm_cmd_guard_decider;
            traversal_direction = liz_vm_monitor_node_flag_leave_to_top;
            
            break;
            
        case liz_node_type_persistent_action:
            // Checks for invalid execution states internally.
            liz_vm_invoke_persistent_action(vm,
                                            actor,
                                            shape);
            
            // Traverse up to decider, alas its associated guard.
            next_cmd = liz_vm_cmd_guard_decider;
            traversal_direction = liz_vm_monitor_node_flag_leave_to_top;
            
            break;
            
        case liz_node_type_sequence_decider:
            liz_vm_invoke_sequence_decider(vm,
                                           actor,
                                           shape);
            
            // Traverse down to a decider child.
            next_cmd = liz_vm_cmd_invoke_node;
            traversal_direction = liz_vm_monitor_node_flag_leave_to_bottom;
            
            LIZ_VM_CATCH_CHILDLESS_DECIDER(vm, &next_cmd, &traversal_direction);
            
            break;
            
        case liz_node_type_dynamic_priority_decider:
            liz_vm_invoke_dynamic_priority_decider(vm,
                                                   actor,
                                                   shape);
            
            // Traverse down to a decider child.
            next_cmd = liz_vm_cmd_invoke_node;
            traversal_direction = liz_vm_monitor_node_flag_leave_to_bottom;
            
            LIZ_VM_CATCH_CHILDLESS_DECIDER(vm, &next_cmd, &traversal_direction);
            
            break;
            
        case liz_node_type_concurrent_decider:
            liz_vm_invoke_concurrent_decider(vm,
                                             actor,
                                             shape);
            
            // Traverse down to a decider child.
            next_cmd = liz_vm_cmd_invoke_node;
            traversal_direction = liz_vm_monitor_node_flag_leave_to_bottom;
            
            LIZ_VM_CATCH_CHILDLESS_DECIDER(vm, &next_cmd, &traversal_direction);
            
            break;
            
        default:
            assert(0 && "Unhandled node type.");
            next_cmd = liz_vm_cmd_error;
            traversal_direction = liz_vm_monitor_node_flag_error;
            break;
    }
    
    LIZ_VM_MONITOR_NODE(monitor,
                        monitored_shape_atom_index,
                        traversal_direction,
                        vm,
                        actor_blackboard, 
                        time,
                        actor,
                        shape);
    
    return next_cmd;
}



liz_vm_cmd_t
liz_vm_step_guard_decider(liz_vm_t *vm,
                          liz_vm_monitor_t *monitor,
                          void * LIZ_RESTRICT actor_blackboard,
                          liz_time_t const time,
                          liz_vm_actor_t const *actor,
                          liz_vm_shape_t const *shape)
{
    // An empty guard stack means that the root node has been left to its top,
    // alas the update is done.
    if (0 == liz_lookaside_stack_count(&vm->decider_guard_stack_header)) {
        return liz_vm_cmd_cleanup;
    }
    
    // Process decider node's child execution state, alas guard how to react.
    liz_vm_decider_guard_t const *top_guard = liz_vm_current_top_decider_guard(vm);
    liz_vm_monitor_node_flag_t traversal_direction = liz_vm_monitor_node_flag_enter_from_bottom;
    liz_int_t const monitored_shape_atom_index = top_guard->shape_atom_index;
    LIZ_VM_MONITOR_NODE(monitor,
                        monitored_shape_atom_index,
                        traversal_direction,
                        vm, 
                        actor_blackboard, 
                        time,
                        actor,
                        shape);
    
    switch (top_guard->type) {
        case liz_node_type_sequence_decider:
            liz_vm_guard_sequence_decider(vm);
            break;
            
        case liz_node_type_concurrent_decider:
            liz_vm_guard_concurrent_decider(vm);
            break;
            
        case liz_node_type_dynamic_priority_decider:
            liz_vm_guard_dynamic_priority_decider(vm);
            break;
            
        default:
            LIZ_ASSERT(0 && "Unhandled decider guard type.");
            break;
    }
    
    
    // Determine traversal direction and which cmd to run next.
    bool const traversing_up = top_guard->end_index <= vm->shape_atom_index;
    liz_vm_cmd_t next_cmd = liz_vm_cmd_error;
    
    if (traversing_up) {
        // Leave the decider node, alas the decider guard representing it and
        // prepare the vm for guarding the parent decider.
        liz_lookaside_stack_pop(&vm->decider_guard_stack_header);
        next_cmd = liz_vm_cmd_guard_decider;
        traversal_direction = liz_vm_monitor_node_flag_leave_to_top;
        
    } else { 
        // When switching from traversing up to traversing down again, first
        // cancel actions in marked cancellation range so new states won't mix
        // with abandoned ones on the following node invocation.
        liz_vm_cancel_actions_in_cancellation_range(vm,
                                                    monitor,
                                                    actor_blackboard,
                                                    time,
                                                    actor,
                                                    shape);
        
        next_cmd = liz_vm_cmd_invoke_node;
        traversal_direction = liz_vm_monitor_node_flag_leave_to_bottom;
    }
    
    LIZ_VM_MONITOR_NODE(monitor,
                        monitored_shape_atom_index,
                        traversal_direction,
                        vm, 
                        actor_blackboard, 
                        time,
                        actor,
                        shape);
    
    return next_cmd;
}



liz_vm_cmd_t
liz_vm_step_cleanup(liz_vm_t *vm,
                    liz_vm_monitor_t *monitor,
                    void * LIZ_RESTRICT actor_blackboard,
                    liz_time_t const time,
                    liz_vm_actor_t const *actor,
                    liz_vm_shape_t const *shape)
{
    LIZ_ASSERT(0 == liz_lookaside_stack_count(&vm->decider_guard_stack_header) 
               && "Decider guard stack must be empty before cleanup.");
    
    // If the last traversal steps were all up and then reached the root
    // before going down again, then there might be running actions to cancel.
    liz_vm_cancel_actions_in_cancellation_range(vm,
                                                monitor,
                                                actor_blackboard,
                                                time,
                                                actor,
                                                shape);
    
    // Reorder vm decider states.
    LIZ_ASSERT(0 == (((uintptr_t)vm->decider_guards) & (LIZ_VM_DECIDER_GUARD_ALIGNMENT - 1)) 
               && "Invalid assumption that decider guard stack is 16bit aligned.");
    LIZ_ASSERT(sizeof(*(vm->decider_guards)) >= (sizeof(*(vm->decider_states)) + sizeof(*(vm->decider_state_shape_atom_indices))) 
               && "Stack elements too small to store a decider state and its shape atom index.");
    liz_sort_values_for_keys_from_post_order_traversal(vm->decider_states,
                                                       vm->decider_state_shape_atom_indices,
                                                       sizeof(*(vm->decider_states)),
                                                       liz_lookaside_stack_count(&vm->decider_state_stack_header),
                                                       vm->decider_guards + sizeof(*(vm->decider_states)) * liz_lookaside_stack_count(&vm->decider_state_stack_header),
                                                       (uint16_t *)(vm->decider_guards),
                                                       liz_lookaside_stack_capacity(&vm->decider_guard_stack_header));
    
    // Reorder vm persistent state changes, establish the correct helper stack 
    // alignment.
    LIZ_ASSERT(0 == (((uintptr_t)vm->decider_guards) & (LIZ_VM_DECIDER_GUARD_ALIGNMENT - 1))
               && "Invalid assumption that decider guard stack is 16bit aligned.");
    LIZ_ASSERT(sizeof(*(vm->decider_guards)) >= (sizeof(*(vm->persistent_state_changes)) + sizeof(*(shape->persistent_state_shape_atom_indices)) + (LIZ_PERSISTENT_STATE_ALIGNMENT - LIZ_VM_DECIDER_GUARD_ALIGNMENT)) 
               && "Stack elements too small to store a persistent state change and its shape atom index.");
    
    char *aligned_persistent_state_stack = (char *)vm->decider_guards + sizeof(*(shape->persistent_state_shape_atom_indices)) * liz_lookaside_stack_capacity(&vm->decider_guard_stack_header);
    aligned_persistent_state_stack += liz_allocation_alignment_offset(aligned_persistent_state_stack, LIZ_PERSISTENT_STATE_ALIGNMENT);
    
    liz_sort_values_for_keys_from_post_order_traversal(vm->persistent_state_changes,
                                                       vm->persistent_state_change_shape_atom_indices, 
                                                       sizeof(*(vm->persistent_state_changes)),
                                                       liz_lookaside_stack_count(&vm->persistent_state_change_stack_header),
                                                       aligned_persistent_state_stack,
                                                       (uint16_t *)(vm->decider_guards),
                                                       liz_lookaside_stack_capacity(&vm->decider_guard_stack_header));
    
    // That's it. Action states and action launch requests should be in the
    // correct order while action cancel requests should be in the reversed 
    // correct order which is taken into account when extracting the requests.
    
    return liz_vm_cmd_done;
}



void
liz_vm_extract_and_clear_actor_states(liz_vm_t *vm,
                                      liz_vm_actor_t *actor,
                                      liz_vm_shape_t const *shape)
{
    LIZ_ASSERT(liz_vm_cmd_done == vm->cmd
               && "Vm update must have been cleaned up and done before transmitting states to actor.");

    actor->header->placeholder_for_random_number_seed = vm->actor_random_number_seed;
    actor->header->decider_state_count = liz_lookaside_stack_count(&vm->decider_guard_stack_header);
    actor->header->action_state_count = liz_lookaside_stack_count(&vm->action_state_stack_header);
    
    liz_actor_header_t const actor_header = *(actor->header);
    
    liz_memcpy(actor->decider_states, 
               vm->decider_states,
               sizeof(*(actor->decider_states)) * actor_header.decider_state_count);
    liz_memcpy(actor->decider_state_shape_atom_indices,
               vm->decider_state_shape_atom_indices, 
               sizeof(*(actor->decider_state_shape_atom_indices)) * actor_header.decider_state_count);
    
    liz_memcpy(actor->action_states,
               vm->action_states,
               sizeof(*(actor->action_states)) * actor_header.action_state_count);
    liz_memcpy(actor->action_state_shape_atom_indices, 
               vm->action_state_shape_atom_indices,
               sizeof(*(actor->action_state_shape_atom_indices)) * actor_header.action_state_count);
    
    liz_apply_persistent_state_changes(actor->persistent_states,
                                       shape->persistent_state_shape_atom_indices,
                                       shape->spec.persistent_state_count,
                                       vm->persistent_state_changes,
                                       vm->persistent_state_change_shape_atom_indices,
                                       liz_lookaside_stack_count(&vm->persistent_state_change_stack_header));
    
    liz_lookaside_stack_clear(&vm->persistent_state_change_stack_header);
    liz_lookaside_stack_clear(&vm->decider_state_stack_header);
    liz_lookaside_stack_clear(&vm->action_state_stack_header);
}



#pragma mark Internals

#pragma mark Enter nodes from top



void
liz_vm_invoke_immediate_action(liz_vm_t *vm,
                               void * LIZ_RESTRICT actor_blackboard,
                               liz_time_t const time,
                               liz_vm_actor_t const *actor,
                               liz_vm_shape_t const *shape)
{
    LIZ_ASSERT(liz_node_type_immediate_action == (liz_node_type_t)(shape->atoms[vm->shape_atom_index].type_mask.type));
    
    // Determine the action state.
    liz_execution_state_t exec_state = liz_execution_state_launch;
    if (liz_seek_key(&vm->actor_action_state_index,
                     vm->shape_atom_index,
                     actor->action_state_shape_atom_indices,
                     shape->spec.shape_atom_count)) {
        
        exec_state = (liz_execution_state_t)actor->action_states[vm->actor_action_state_index];
        
        // TODO: @todo Remove check once a system is in place that checks and 
        //             rejects external invalid action state updates.
        LIZ_VM_CATCH_INVALID_PERSISTENT_AND_IMMEDIATE_ACTION_STATE(&exec_state);
        
        liz_vm_consume_state(&vm->actor_action_state_index,
                             vm->shape_atom_index,
                             actor->action_state_shape_atom_indices,
                             actor->header->action_state_count);
    }
    
    // Call the immediate action.
    exec_state = liz_vm_tick_immediate_action(&vm->actor_random_number_seed,
                                              actor_blackboard,
                                              time,
                                              exec_state,
                                              &shape->atoms[vm->shape_atom_index],
                                              shape->immediate_action_functions,
                                              shape->spec.immediate_action_function_count);
    
    // Catch invalid user supplied immediate action function return values.
    LIZ_VM_CATCH_INVALID_PERSISTENT_AND_IMMEDIATE_ACTION_STATE(&exec_state);
    
    // Store running state.
    if (liz_execution_state_running == exec_state) {
        liz_lookaside_stack_push(&vm->action_state_stack_header);
        liz_int_t const top_index = liz_lookaside_stack_top_index(&vm->action_state_stack_header);   
        
        vm->action_states[top_index] = (uint8_t)exec_state;
        vm->action_state_shape_atom_indices[top_index] = vm->shape_atom_index;
    }
    
    vm->execution_state = exec_state;
    vm->shape_atom_index += LIZ_NODE_SHAPE_ATOM_COUNT_IMMEDIATE_ACTION + 0u * LIZ_NODE_SHAPE_ATOM_COUNT_IMMEDIATE_ACTION_TWO_CHILDREN;
}



void
liz_vm_invoke_deferred_action(liz_vm_t *vm,
                              liz_vm_actor_t const *actor,
                              liz_vm_shape_t const *shape)
{
    LIZ_ASSERT(liz_node_type_deferred_action == (liz_node_type_t)(shape->atoms[vm->shape_atom_index].type_mask.type));
    
    liz_execution_state_t exec_state = liz_execution_state_launch;
    
    // Fetch state.
    if (liz_seek_key(&vm->actor_action_state_index,
                     vm->shape_atom_index,
                     actor->action_state_shape_atom_indices,
                     actor->header->action_state_count)) {

        exec_state = (liz_execution_state_t)(actor->action_states[vm->actor_action_state_index]);
        
        // TODO: @todo Remove check once a system is in place that checks and 
        //             rejects external invalid action state updates.
        LIZ_VM_CATCH_INVALID_DEFERRED_ACTION_STATE(&exec_state);
        
        liz_vm_consume_state(&vm->actor_action_state_index,
                             vm->shape_atom_index,
                             actor->action_state_shape_atom_indices,
                             actor->header->action_state_count);
        
        
        if (liz_execution_state_launch == exec_state
            || liz_execution_state_running == exec_state) {
            
            exec_state = liz_execution_state_running;
            
            // Emit running action state to remember the active deferred action.
            liz_lookaside_stack_push(&vm->action_state_stack_header);
            liz_int_t const action_state_top_index = liz_lookaside_stack_top_index(&vm->action_state_stack_header);
            vm->action_state_shape_atom_indices[action_state_top_index] = vm->shape_atom_index;
            vm->action_states[action_state_top_index] = (uint8_t)exec_state;
        } // Else: Deferred action terminated with success or fail. Nothing to do.
        
    } else {
        // Emit a launch request.
        liz_vm_launch_or_cancel_deferred_action(vm->action_requests,
                                                &vm->action_request_stack_header, 
                                                liz_execution_state_launch, 
                                                &shape->atoms[vm->shape_atom_index], 
                                                vm->shape_atom_index);
        /*
        liz_shape_atom_t const first_atom = shape->atoms[vm->shape_atom_index];
        liz_shape_atom_t const second_atom = shape->atoms[vm->shape_atom_index + 1];
        liz_lookaside_double_stack_push(&vm->action_request_stack_header, LIZ_VM_ACTION_REQUEST_STACK_SIDE_LAUNCH);
        liz_int_t const action_request_top_index = liz_lookaside_double_stack_top_index(&vm->action_request_stack_header, LIZ_VM_ACTION_REQUEST_STACK_SIDE_LAUNCH);
        vm->action_requests[action_request_top_index] = (liz_vm_action_request_t){
            second_atom.deferred_action_second.action_id,
            first_atom.deferred_action_first.resource_id,
            (uint16_t)vm->shape_atom_index
        };
         */
        
        // Emit a launch execution state. If a higher-up decider cancels this
        // action, then its action request is rolled back but no cancellation
        // request is additionally created.
        liz_lookaside_stack_push(&vm->action_state_stack_header);
        liz_int_t const action_state_top_index = liz_lookaside_stack_top_index(&vm->action_state_stack_header);
        vm->action_state_shape_atom_indices[action_state_top_index] = vm->shape_atom_index;
        vm->action_states[action_state_top_index] = (uint8_t)liz_execution_state_launch;
        
        exec_state = liz_execution_state_launch;
    }
    
    // Increment shape item cursor
    vm->shape_atom_index += LIZ_NODE_SHAPE_ATOM_COUNT_DEFERRED_ACTION + 0u * LIZ_NODE_SHAPE_ATOM_COUNT_DEFERRED_ACTION_TWO_CHILDREN;
    
    vm->execution_state = exec_state;
}



void
liz_vm_invoke_persistent_action(liz_vm_t *vm,
                                liz_vm_actor_t const *actor,
                                liz_vm_shape_t const *shape)
{
    LIZ_ASSERT(liz_node_type_persistent_action == (liz_node_type_t)(shape->atoms[vm->shape_atom_index].type_mask.type));
    
    // Fetch state
    bool const state_found = liz_seek_key(&vm->actor_persistent_state_index,
                                          vm->shape_atom_index,
                                          shape->persistent_state_shape_atom_indices,
                                          shape->spec.persistent_state_count);
    (void)state_found;
    LIZ_ASSERT(state_found && "All persistent states must exist.");
    
    // Set and consume state.
    liz_execution_state_t exec_state = (liz_execution_state_t)(actor->persistent_states[vm->actor_persistent_state_index].persistent_action_state);
    
    // TODO: @todo Remove check once a system is in place that checks and 
    //             rejects external invalid action state updates.
    LIZ_VM_CATCH_INVALID_PERSISTENT_AND_IMMEDIATE_ACTION_STATE(&exec_state);
    
    liz_vm_consume_state(&vm->actor_persistent_state_index,
                         vm->shape_atom_index,
                         shape->persistent_state_shape_atom_indices,
                         shape->spec.persistent_state_count);
    
    // Increment shape atom cursor.
    vm->shape_atom_index += LIZ_NODE_SHAPE_ATOM_COUNT_PERSISTENT_ACTION + 0 * LIZ_NODE_SHAPE_ATOM_COUNT_PERSISTENT_ACTION_TWO_CHILDREN;
    vm->execution_state = exec_state;
}



void
liz_vm_invoke_sequence_decider(liz_vm_t *vm,
                               liz_vm_actor_t const *actor,
                               liz_vm_shape_t const *shape)
{
    liz_shape_atom_t const decider_atom = shape->atoms[vm->shape_atom_index];
    
    LIZ_ASSERT(liz_node_type_sequence_decider == (liz_node_type_t)(decider_atom.type_mask.type));

    // Caller checks and reacts to childless deciders.
    
    // Fetch state - in case of a sequence decider.
    LIZ_ASSERT(0u == LIZ_NODE_SHAPE_ATOM_COUNT_SEQUENCE_DECIDER_TWO_CHILDREN 
               && "Invalid assumption that the shape atom count per sequence decider child is zero. This is assumed because there is no efficient way to determine the number of children of a sequence decider.");
    uint16_t reached_child = vm->shape_atom_index + LIZ_NODE_SHAPE_ATOM_COUNT_SEQUENCE_DECIDER + 0 * LIZ_NODE_SHAPE_ATOM_COUNT_SEQUENCE_DECIDER_TWO_CHILDREN;
    
    if (liz_seek_key(&vm->actor_decider_state_index,
                     vm->shape_atom_index,
                     actor->decider_state_shape_atom_indices,
                     actor->header->decider_state_count)) {
        
        reached_child = actor->decider_states[vm->actor_decider_state_index];
        
        liz_vm_consume_state(&vm->actor_decider_state_index,
                             vm->shape_atom_index,
                             actor->decider_state_shape_atom_indices,
                             actor->header->decider_state_count);
    }
    
    // Set up and push guard.
    // Set the aggregate state to success - can not be set to launch as launch
    // and running are treated the same to simplify action state handling for
    // deferred actions. Alas: assume the best for the concurrent decider.    
    liz_lookaside_stack_push(&vm->decider_guard_stack_header);
    vm->decider_guards[liz_lookaside_stack_top_index(&vm->decider_guard_stack_header)] = (liz_vm_decider_guard_t){
        vm->shape_atom_index,
        vm->shape_atom_index + decider_atom.sequence_decider.end_offset,
        (uint16_t)liz_lookaside_stack_count(&vm->decider_state_stack_header),
        (uint16_t)liz_lookaside_double_stack_count(&vm->action_request_stack_header, LIZ_VM_ACTION_REQUEST_STACK_SIDE_LAUNCH),
        reached_child,
        0,
        decider_atom.sequence_decider.type,
        {0} // Padding.
    };
    
    vm->shape_atom_index = reached_child;
    
    // Set execution state to fail - no consequence when descending but no child
    // means going up and a decider which couldn't succeed through a child 
    // should fail.
    // Keep in mind that no decider child is an error!
    vm->execution_state = liz_execution_state_fail;
}



void
liz_vm_invoke_dynamic_priority_decider(liz_vm_t *vm,
                                       liz_vm_actor_t const *actor,
                                       liz_vm_shape_t const *shape)
{
    (void)actor;
    
    liz_shape_atom_t const decider_atom = shape->atoms[vm->shape_atom_index];
    
    LIZ_ASSERT(liz_node_type_dynamic_priority_decider == (liz_node_type_t)(decider_atom.type_mask.type));
    
    // Caller checks and reacts to childless deciders.
    
    // Set up and push guard.
    // Set the aggregate state to success - can not be set to launch as launch
    // and running are treated the same to simplify action state handling for
    // deferred actions. Alas: assume the best for the concurrent decider.    
    liz_lookaside_stack_push(&vm->decider_guard_stack_header);
    vm->decider_guards[liz_lookaside_stack_top_index(&vm->decider_guard_stack_header)] = (liz_vm_decider_guard_t){
        vm->shape_atom_index,
        vm->shape_atom_index + decider_atom.dynamic_priority_decider.end_offset,
        (uint16_t)liz_lookaside_stack_count(&vm->decider_state_stack_header),
        (uint16_t)liz_lookaside_double_stack_count(&vm->action_request_stack_header, LIZ_VM_ACTION_REQUEST_STACK_SIDE_LAUNCH),
        0,
        0,
        decider_atom.dynamic_priority_decider.type,
        {0} // Padding.
    };
    
    LIZ_ASSERT(0u == LIZ_NODE_SHAPE_ATOM_COUNT_DYNAMIC_PRIORITY_DECIDER_TWO_CHILDREN 
               && "Invalid assumption that the shape atom count per dynamic priority decider child is zero. This is assumed because there is no efficient way to determine the number of children of a dynamic priority decider.");
    vm->shape_atom_index += LIZ_NODE_SHAPE_ATOM_COUNT_DYNAMIC_PRIORITY_DECIDER + 0 * LIZ_NODE_SHAPE_ATOM_COUNT_DYNAMIC_PRIORITY_DECIDER_TWO_CHILDREN;
    
    // Set execution state to fail - no consequence when descending but no child
    // means going up and a decider which couldn't succeed through a child 
    // should fail.
    // Keep in mind that no decider child is an error!
    vm->execution_state = liz_execution_state_fail;
}



void
liz_vm_invoke_concurrent_decider(liz_vm_t *vm,
                                 liz_vm_actor_t const *actor,
                                 liz_vm_shape_t const *shape)
{
    (void)actor;
    
    liz_shape_atom_t const decider_atom = shape->atoms[vm->shape_atom_index];
    
    LIZ_ASSERT(liz_node_type_concurrent_decider == (liz_node_type_t)(decider_atom.type_mask.type));
    
    // Caller checks and reacts to childless deciders.
    
    // Set up and push guard.
    // Set the aggregate state to success - can not be set to launch as launch
    // and running are treated the same to simplify action state handling for
    // deferred actions. Alas: assume the best for the concurrent decider.    
    liz_lookaside_stack_push(&vm->decider_guard_stack_header);
    vm->decider_guards[liz_lookaside_stack_top_index(&vm->decider_guard_stack_header)] = (liz_vm_decider_guard_t){
        vm->shape_atom_index,
        vm->shape_atom_index + decider_atom.concurrent_decider.end_offset,
        (uint16_t)liz_lookaside_stack_count(&vm->decider_state_stack_header),
        (uint16_t)liz_lookaside_double_stack_count(&vm->action_request_stack_header, LIZ_VM_ACTION_REQUEST_STACK_SIDE_LAUNCH),
        0,
        (uint8_t)liz_execution_state_success,
        decider_atom.concurrent_decider.type,
        {0} // Padding.
    };
    
    LIZ_ASSERT(0u == LIZ_NODE_SHAPE_ATOM_COUNT_CONCURRENT_DECIDER_TWO_CHILDREN 
               && "Invalid assumption that the shape atom count per concurrent decider child is zero. This is assumed because there is no efficient way to determine the number of children of a concurrent decider.");
    vm->shape_atom_index += LIZ_NODE_SHAPE_ATOM_COUNT_CONCURRENT_DECIDER + 0 * LIZ_NODE_SHAPE_ATOM_COUNT_CONCURRENT_DECIDER_TWO_CHILDREN;
    
    // Set execution state to fail - no consequence when descending but no child
    // means going up and a decider which couldn't succeed through a child 
    // should fail.
    // Keep in mind that no decider child is an error!
    vm->execution_state = liz_execution_state_fail;
}



#pragma mark Re-enter deciders from bottom



void
liz_vm_guard_sequence_decider(liz_vm_t *vm)
{
    liz_vm_decider_guard_t *guard = liz_vm_current_top_decider_guard(vm);
    LIZ_ASSERT(liz_node_type_sequence_decider == guard->type);
        
    switch (vm->execution_state) {
        case liz_execution_state_launch: // Fall through, shortcut for deferred action handling.
        case liz_execution_state_running:
        {
            // Child is running so sequence remembers last reached child in
            // emitted state, returns running, too, and leaves its sub-stream.
            LIZ_ASSERT(!liz_lookaside_stack_is_full(&vm->decider_state_stack_header) 
                       && "Vm decider state buffer is smaller than specified by the shape or the shape specification is wrong.");
            liz_lookaside_stack_push(&vm->decider_state_stack_header);
            liz_int_t const top_index = liz_lookaside_stack_top_index(&vm->decider_state_stack_header);
            
            vm->decider_states[top_index] = guard->sequence_reached_child_index;
            vm->decider_state_shape_atom_indices[top_index] = guard->shape_atom_index;
            
            vm->shape_atom_index = (uint16_t)guard->end_index;
            break;
        }
            
        case liz_execution_state_success:
            // Child succeeded, on to the next child, remember reached child.
            guard->sequence_reached_child_index = (uint16_t)vm->shape_atom_index; 
            LIZ_ASSERT(vm->decider_state_stack_header.count == guard->decider_state_rollback_marker);
            break;
            
        case liz_execution_state_fail:
            // Child failed, sequence fails, too, and leaves its sub-stream.
            vm->shape_atom_index = guard->end_index;
            LIZ_ASSERT(vm->decider_state_stack_header.count == guard->decider_state_rollback_marker);
            break;
            
        default:
            LIZ_ASSERT(0 && "Unhandled execution state.");
            break;
    }
}



void
liz_vm_guard_concurrent_decider(liz_vm_t *vm)
{
    liz_vm_decider_guard_t *guard = liz_vm_current_top_decider_guard(vm);
    LIZ_ASSERT(liz_node_type_concurrent_decider == guard->type);
        
    switch (vm->execution_state) {
        case liz_execution_state_launch: // Fall through, shortcut for deferred action handling.
        case liz_execution_state_running:
            // Child is running and turns the whole concurrent decider to run
            // (as long as no later child fails). Go on and invoke the remaining
            // children.
            guard->concurrent_execution_state = (uint8_t)liz_execution_state_running;            
            break;
            
        case liz_execution_state_success:
            // Child succeeded, on to the next child.
            // For the case that this was the last child of the decider, re-
            // establish the aggregated state. It starts as success and when
            // switched to running sticks to it as long as no child fails.
            vm->execution_state = guard->concurrent_execution_state;
            break;
            
        case liz_execution_state_fail:
            // Child failed, concurrent decider fails immediately, too. Running 
            // children (invoked during this update and not yet invoked but 
            // running from the previous update) are cancelled. 
            // Leave the decider sub-stream.
            liz_vm_rollback_immediately_cancelable_states_and_requests(vm, guard);
            liz_vm_cancellation_range_adapt(&vm->cancellation_range,
                                            guard->shape_atom_index,
                                            guard->end_index);
            
            vm->shape_atom_index = guard->end_index;
            break;
            
        default:
            LIZ_ASSERT(0 && "Unhandled execution state.");
            break;
    }
}



void
liz_vm_guard_dynamic_priority_decider(liz_vm_t *vm)
{
    liz_vm_decider_guard_t *guard = liz_vm_current_top_decider_guard(vm);
    LIZ_ASSERT(liz_node_type_dynamic_priority_decider == guard->type);
        
    switch (vm->execution_state) {
        case liz_execution_state_launch: // Fall through, shortcut for deferred action handling.
        case liz_execution_state_running: // Fall through.
        case liz_execution_state_success:
            // Child succeeded or is running and turns the whole priority 
            // decider to this state.
            // Action from lower-priority child that are in a running state from
            // the previous update need to be cancelled.
            // Leave the decider sub-stream.
            liz_vm_cancellation_range_adapt(&vm->cancellation_range,
                                            vm->shape_atom_index,
                                            guard->end_index);
            vm->shape_atom_index = guard->end_index;
            break;
            
        case liz_execution_state_fail:
            // Child failed, go on and tick the next one.            
            // If this was the last child, then the whole decider fails.
            // Nothing to do.
            break;
            
        default:
            LIZ_ASSERT(0 && "Unhandled execution state.");
            break;
    }
}



#pragma mark Helpers



void
liz_vm_init(liz_vm_t *vm,
            liz_shape_specification_t const spec,
            uint16_t *persistent_state_change_shape_atom_indices,
            uint16_t *decider_state_shape_atom_indices,
            uint16_t *action_state_shape_atom_indices,
            liz_persistent_state_t *persistent_state_changes,
            uint16_t *decider_states,
            uint8_t *action_states,
            liz_vm_decider_guard_t *decider_guards,
            liz_vm_action_request_t *action_requests)
{
    vm->persistent_state_change_shape_atom_indices = persistent_state_change_shape_atom_indices;
    vm->decider_state_shape_atom_indices = decider_state_shape_atom_indices;
    vm->action_state_shape_atom_indices = action_state_shape_atom_indices;
    
    vm->persistent_state_changes = persistent_state_changes;
    vm->decider_states = decider_states;
    vm->action_states = action_states;
    
    vm->decider_guards = decider_guards;
    vm->action_requests = action_requests;
    
    vm->actor_random_number_seed = 0;
    
    vm->persistent_state_change_stack_header = liz_lookaside_stack_make(spec.persistent_state_change_capacity, 0);
    vm->decider_state_stack_header = liz_lookaside_stack_make(spec.decider_state_capacity, 0);
    vm->action_state_stack_header = liz_lookaside_stack_make(spec.action_state_capacity, 0);
    
    vm->decider_guard_stack_header = liz_lookaside_stack_make(spec.decider_guard_capacity, 0);
    vm->action_request_stack_header = liz_lookaside_double_stack_make(spec.action_request_capacity, 0, 0);
    
    liz_vm_reset(vm);
}



void
liz_vm_cancel_actions_in_cancellation_range(liz_vm_t *vm,
                                            liz_vm_monitor_t *monitor,
                                            void * LIZ_RESTRICT actor_blackboard,
                                            liz_time_t const time,
                                            liz_vm_actor_t const *actor,
                                            liz_vm_shape_t const *shape)
{
    if (liz_vm_cancellation_range_is_empty(vm->cancellation_range)) {
        return;
    }
   
    liz_vm_cancel_running_actions_from_current_update(vm,
                                                      monitor,
                                                      actor_blackboard,
                                                      time,
                                                      actor,
                                                      shape);
    
    // Running this after the jump back to have linear shape atom stream 
    // iteration during cancellation.
    liz_vm_cancel_launched_and_running_actions_from_previous_update(vm,
                                                                    monitor,
                                                                    actor_blackboard,
                                                                    time,
                                                                    actor,
                                                                    shape);
    
    // Clear alas empty the cancellation range.
    vm->cancellation_range = (liz_vm_cancellation_range_t){
        vm->shape_atom_index,
        vm->shape_atom_index
    };
}



/**
 * TODO: @todo Extract and complete to create a breakpoint monitor on top of the 
 *             simpler monitor concept.
 */
/*
void
liz_vm_monitor_node(uintptr_t user_data,
                    uint16_t const node_shape_atom_index,
                    liz_uint_t const traversal_direction,
                    liz_vm_t const *vm,
                    void const * LIZ_RESTRICT actor_blackboard,
                    liz_vm_actor_t const *actor,
                    liz_vm_shape_t const *shape)
{
    liz_vm_breakpoint_monitor_t *monitor = (liz_vm_breakpoint_monitor_t *)user_data;
    
    LIZ_ASSERT(0 < monitor->breakpoint_function_count);
    
    liz_vm_monitor_breakpoint_stream_t const *breakpoints = monitor->node_breakpoints;
    
    if (NULL == monitor
        || NULL == breakpoints) {
        
        return;
    }

    
    if (liz_vm_monitor_node_flag_cancel_action & traversal_direction) {
        
        liz_uint_t const cancel_mask = traversal_direction - liz_vm_monitor_node_flag_cancel_action;
        
        // Cancellation will lead to the end of the cancelled sub-stream, 
        // reusing the breakpoint index to enable an efficient cancellation 
        // treatment.
        monitor->breakpoint_index = 0;
        if (!liz_seek_key(&monitor->breakpoint_index, 
                          node_shape_atom_index,
                          breakpoints->shape_atom_indices,
                          breakpoints->count)) {
         
            return;
        }
        
        // Introduce a helper function for this and reuse it below!
        if (traversal_direction & breakpoints->flags[monitor->breakpoint_index]) {
            liz_int_t const func_index = breakpoints->function_indices[monitor->breakpoint_index];
            LIZ_ASSERT(func_index < monitor->breakpoint_function_count);
            
            monitor->breakpoint_functions[func_index]((uintptr_t)monitor,
                                                      node_shape_atom_index,
                                                      traversal_direction,
                                                      vm,
                                                      actor_blackboard,
                                                      actor,
                                                      shape);
        }
        
        
        return;
    }


    switch (traversal_direction) {
        case liz_vm_monitor_node_flag_enter_from_top:
            // Invoke a node, all vm data identify the node.
            
            if (liz_seek_key(&monitor->breakpoint_index,
                             node_shape_atom_index,
                             breakpoints->shape_atom_indices,
                             breakpoints->count)) {
                
                liz_vm_breakpoint_monitor_trigger_breakpoint(monitor,
                                                             node_shape_atom_index,
                                                             traversal_direction,
                                                             vm,
                                                             actor_blackboard,
                                                             actor,
                                                             shape);
                
                if (shape->atoms[node_shape_atom_index].type_mask.type > liz_node_type_action_max_id
                    && ~((liz_uint_t)(liz_vm_monitor_node_flag_cancel_action | liz_vm_monitor_node_flag_enter_from_top)) & breakpoints->flags[monitor->breakppoint_index]) {
                    
                    // Push breakpoint guard onto breakpoint guard stack.
                    
                }
                
                
            }
            
            break;
        case liz_vm_monitor_node_flag_enter_from_bottom:
            // Entering a guard from the bottom - see vm stack. Check if guarding this shape atom index.
            break;
        case liz_vm_monitor_node_flag_leave_to_top:
            // Leaving an node or a guard. Pop monitor stack if it matches the shape atom index.
            break;
        case liz_vm_monitor_node_flag_leave_to_bottom:
            // Leaving decider node or its guard. In both cases guard is on stack.
            break;
        default:
            LIZ_ASSERT(0 && "Unhandled traversal direction.");
            break;
    }
    
    
#error Only push a guard breakpoint to the stack if the stack isn't full!
}
*/



void
liz_vm_cancellation_range_adapt(liz_vm_cancellation_range_t *cancellation_range,
                                uint16_t const new_begin_index,
                                uint16_t const new_end_index)
{
    LIZ_ASSERT(new_begin_index <= new_end_index);
    
    liz_vm_cancellation_range_t range = *cancellation_range;
    
    if (liz_vm_cancellation_range_is_empty(range)) {
        
        range.begin_index = new_begin_index;
        range.end_index = new_end_index;
        
    } else if (new_begin_index < new_end_index) {
        // Only non-empty new begin/end pairs adapt a non-empty range.
        range.begin_index = (uint16_t)liz_min(range.begin_index, new_begin_index);
        range.end_index = (uint16_t)liz_max(range.end_index, new_end_index);
    }
    
    *cancellation_range = range;
}



liz_execution_state_t
liz_vm_launch_or_cancel_deferred_action(liz_vm_action_request_t *action_requests,
                                        liz_lookaside_double_stack_t *action_request_stack_header,
                                        liz_execution_state_t const execution_request,
                                        liz_shape_atom_t const *action_begin_shape_atom,
                                        liz_int_t const shape_atom_index)
{
    LIZ_ASSERT(liz_execution_state_launch == execution_request 
               || liz_execution_state_cancel == execution_request);
    
    // Consume two shape atoms per deferred action;
    liz_shape_atom_t const *second_atom = action_begin_shape_atom + 1;
    
    liz_lookaside_double_stack_side_t launch_or_cancel_side = LIZ_VM_ACTION_REQUEST_STACK_SIDE_LAUNCH;
    
    if (liz_execution_state_cancel) {
        launch_or_cancel_side = LIZ_VM_ACTION_REQUEST_STACK_SIDE_CANCEL;
    }
    
    liz_lookaside_double_stack_push(action_request_stack_header,
                                    launch_or_cancel_side);
    liz_int_t const top_index = liz_lookaside_double_stack_top_index(action_request_stack_header,
                                                                     launch_or_cancel_side);
    action_requests[top_index] = (liz_vm_action_request_t){
        second_atom->deferred_action_second.action_id,
        action_begin_shape_atom->deferred_action_first.resource_id,
        shape_atom_index
    };
    
    return execution_request;
}



void
liz_vm_cancel_immediate_or_deferred_action(liz_random_number_seed_t *rnd_seed,
                                           void * LIZ_RESTRICT actor_blackboard,
                                           liz_vm_action_request_t *deferred_action_requests,
                                           liz_lookaside_double_stack_t *deferred_action_request_stack_header,
                                           liz_time_t const time,
                                           liz_shape_atom_t const *action_begin_shape_atom,
                                           liz_int_t const shape_atom_index,
                                           liz_immediate_action_func_t const *immediate_action_functions,
                                           liz_int_t const immediate_action_function_count)
{
    switch (action_begin_shape_atom->type_mask.type) {
        case liz_node_type_immediate_action:
        {
            liz_execution_state_t exec_state = liz_vm_tick_immediate_action(rnd_seed,
                                                                            actor_blackboard,
                                                                            time,
                                                                            liz_execution_state_cancel,
                                                                            action_begin_shape_atom,
                                                                            immediate_action_functions,
                                                                            immediate_action_function_count);
            LIZ_ASSERT(liz_execution_state_cancel == exec_state);
            (void)exec_state;
            
            break;
        }
            
        case liz_node_type_deferred_action:
        {
            liz_execution_state_t exec_state = liz_vm_launch_or_cancel_deferred_action(deferred_action_requests,
                                                                                       deferred_action_request_stack_header,
                                                                                       liz_execution_state_cancel,
                                                                                       action_begin_shape_atom,
                                                                                       shape_atom_index);
            LIZ_ASSERT(liz_execution_state_cancel == exec_state);
            (void)exec_state;
            
            break;
        }
            
        default:
            LIZ_ASSERT(0 && "Unexpected node type for cancellation.");
            break;
    }
}



void
liz_vm_cancel_running_actions_from_current_update(liz_vm_t *vm,
                                                  liz_vm_monitor_t *monitor,
                                                  void * LIZ_RESTRICT actor_blackboard,
                                                  liz_time_t const time,
                                                  liz_vm_actor_t const *actor,
                                                  liz_vm_shape_t const *shape)
{
    liz_vm_cancellation_range_t const range = vm->cancellation_range;
    liz_int_t first_action_index = 0;
    
    // If this shows up as a hotspot implement and test a backward key search.
    (void)liz_seek_key(&first_action_index,
                       range.begin_index,
                       vm->action_state_shape_atom_indices ,
                       liz_lookaside_stack_count(&vm->action_state_stack_header));
    
    for (liz_int_t i = first_action_index; i < liz_lookaside_stack_count(&vm->action_state_stack_header); ++i) {
        LIZ_ASSERT(range.begin_index <= vm->action_state_shape_atom_indices[i] 
                   && "Vm action states must be in increasing shape atom index order.");
        LIZ_ASSERT(range.end_index > vm->action_state_shape_atom_indices[i] 
                   && "Actions must be cancelled before invoking new actions outside of the cancellation range.");
        LIZ_ASSERT(liz_execution_state_launch == (liz_execution_state_t)(vm->action_states[i])
                   || liz_execution_state_running == (liz_execution_state_t)(vm->action_states[i])
                   && "Termination states must not be stored for the next update.");
        
        
        if (liz_execution_state_running == (liz_execution_state_t)(vm->action_states[i])) {
            LIZ_VM_MONITOR_NODE(monitor,
                                vm->action_state_shape_atom_indices[i],
                                (liz_uint_t)(liz_vm_monitor_node_flag_cancel_action 
                                             | liz_vm_monitor_node_flag_enter_from_top),
                                vm, 
                                actor_blackboard,  
                                time,
                                actor, 
                                shape);
            
            liz_vm_cancel_immediate_or_deferred_action(&vm->actor_random_number_seed,
                                                       actor_blackboard,
                                                       vm->action_requests,
                                                       &vm->action_request_stack_header,
                                                       time,
                                                       &shape->atoms[vm->action_state_shape_atom_indices[i]],
                                                       vm->action_state_shape_atom_indices[i],
                                                       shape->immediate_action_functions,
                                                       shape->spec.immediate_action_function_count);
            
            LIZ_VM_MONITOR_NODE(monitor,
                                vm->action_state_shape_atom_indices[i] ,
                                (liz_uint_t)(liz_vm_monitor_node_flag_cancel_action 
                                             | liz_vm_monitor_node_flag_leave_to_top),
                                vm, 
                                actor_blackboard,  
                                time,
                                actor, 
                                shape);
        }
        
        // Action launch requests from deferred actions invoked during the 
        // current update have already been rolled back by the decider
        // guards and need no further treatment.
        // No other action than deferred action are allowed to return a launch
        // state.
    }
    
    // Rollback to remove the new states just cancelled.
    // If liz_find_key did not find any states after the range begin index, then
    // first_action_index marks the end of the action state stack and setting
    // it as the count doesn't change the stack.
    liz_lookaside_stack_set_count(&vm->action_state_stack_header,
                                  first_action_index);
}



void
liz_vm_cancel_launched_and_running_actions_from_previous_update(liz_vm_t *vm,
                                                                liz_vm_monitor_t *monitor,
                                                                void * LIZ_RESTRICT actor_blackboard,
                                                                liz_time_t const time,
                                                                liz_vm_actor_t const *actor,
                                                                liz_vm_shape_t const *shape)
{
    liz_vm_cancellation_range_t const range = vm->cancellation_range;
    liz_int_t actor_action_index = vm->actor_action_state_index;
    liz_int_t const actor_action_state_count = actor->header->action_state_count;
    (void)liz_seek_key(&actor_action_index, 
                       range.begin_index,
                       actor->action_state_shape_atom_indices, 
                       actor_action_state_count);
    
    for (; actor_action_index < actor_action_state_count; ++actor_action_index) {
        
        liz_int_t const shape_atom_index = actor->action_state_shape_atom_indices[actor_action_index];
        if (range.end_index <= shape_atom_index) {
            break;
        }
        
        liz_execution_state_t const action_state = (liz_execution_state_t)(actor->action_states[actor_action_index]);
        if (liz_execution_state_launch == action_state
            || liz_execution_state_running == action_state) {
            
            // Only cancel running or previously launched and not yet terminated
            // actions.
            LIZ_VM_MONITOR_NODE(monitor,
                                actor->action_state_shape_atom_indices[actor_action_index] ,
                                (liz_uint_t)(liz_vm_monitor_node_flag_cancel_action 
                                             | liz_vm_monitor_node_flag_enter_from_top),
                                vm, 
                                actor_blackboard,  
                                time,
                                actor, 
                                shape);
            
            liz_vm_cancel_immediate_or_deferred_action(&vm->actor_random_number_seed,
                                                       actor_blackboard,
                                                       vm->action_requests,
                                                       &vm->action_request_stack_header,
                                                       time,
                                                       &shape->atoms[shape_atom_index],
                                                       shape_atom_index,
                                                       shape->immediate_action_functions,
                                                       shape->spec.immediate_action_function_count);
            
            LIZ_VM_MONITOR_NODE(monitor,
                                actor->action_state_shape_atom_indices[actor_action_index] ,
                                (liz_uint_t)(liz_vm_monitor_node_flag_cancel_action 
                                             | liz_vm_monitor_node_flag_leave_to_top),
                                vm, 
                                actor_blackboard, 
                                time,
                                actor, 
                                shape);
        }
    }
    
    // Set the vm's actor action state index forward to not read the cancelled
    // action states again during the current update.
    vm->actor_action_state_index = actor_action_index;
}



