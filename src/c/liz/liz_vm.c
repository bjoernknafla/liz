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



#pragma mark Create or destroy vm



size_t
liz_vm_memory_size(liz_shape_specification_t const spec)
{
    
}



liz_vm_t*
liz_vm_create(liz_shape_specification_t const spec,
              void * LIZ_RESTRICT allocator_context,
              liz_alloc_func_t alloc_func)
{
    
}



void
liz_vm_destroy(liz_vm_t *vm,
               void * LIZ_RESTRICT allocator_context,
               liz_dealloc_func_t dealloc_func)
{
    
}



#pragma mark Update actor



bool
liz_vm_fulfills_shape_specification(liz_vm_t const *vm,
                                    liz_shape_specification_t const spec)
{
    
}



void
liz_vm_update_actor(liz_vm_t *vm,
                    liz_vm_actor_t *actor,
                    liz_action_request_buffer_t *external_requests,
                    liz_vm_monitor_t *monitor,
                    liz_vm_shape_t const *shape,
                    void * LIZ_RESTRICT user_data_lookup_context,
                    liz_vm_user_data_lookup_func_t user_data_lookup_func)
{
    LIZ_ASSERT(liz_vm_fulfills_shape_specification(vm, shape->spec));
    
    if (0u == shape->spec.shape_atom_count) {
        return;
    }
    
    liz_vm_reset(vm, monitor);
    
    void *actor_blackboard = user_data_lookup_func(user_data_lookup_context,
                                                   actor->user_data);
    
    while (liz_vm_cmd_done != vm->cmd) {
        vm->cmd = liz_vm_step_cmd(vm,
                                  actor,
                                  actor_blackboard,
                                  monitor,
                                  shape,
                                  vm->cmd);
    }
}



void
liz_vm_cancel_actor(liz_vm_t *vm,
                    liz_vm_actor_t *actor,
                    liz_action_request_buffer_t *external_requests,
                    liz_vm_monitor_t *monitor,
                    liz_vm_shape_t const *shape,
                    void * LIZ_RESTRICT user_data_lookup_context,
                    liz_vm_user_data_lookup_func_t user_data_lookup_func)
{
    LIZ_ASSERT(liz_vm_fulfills_shape_specification(vm, shape->spec));
    
    liz_vm_reset(vm, monitor);
    
    void *actor_blackboard = user_data_lookup_func(user_data_lookup_context,
                                                   actor->user_data);
    
    vm->cmd = liz_vm_step_cmd(vm,
                              actor,
                              actor_blackboard,
                              monitor,
                              shape,
                              liz_vm_cmd_cleanup_and_actor_update);
    
    LIZ_ASSERT(liz_vm_cmd_done == vm->cmd);
}



void
liz_vm_extract_action_requests(liz_vm_t *vm,
                               liz_vm_actor_t *actor,
                               liz_vm_monitor_t *monitor,
                               liz_action_request_buffer_t *external_requests)
{
    
    
}



void
liz_vm_reset(liz_vm_t *vm,
             liz_vm_monitor_t *monitor)
{
    vm->shape_atom_index = 0;
    vm->actor_decider_state_index = 0;
    vm->actor_action_state_index = 0;
    vm->actor_persistent_state_index = 0;
    
    liz_lookaside_stack_clear(&vm->decider_state_stack_header);
    liz_lookaside_stack_clear(&vm->action_state_stack_header);
    liz_lookaside_double_stack_clear(&vm->action_request_stack_header);
    liz_lookaside_stack_clear(&vm->decider_guard_stack_header);
    
    vm->cancellation_range = (liz_vm_cancellation_range_t){0u, 0u};
    
    vm->cmd = liz_vm_cmd_invoke_node;
    vm->execution_state = liz_execution_state_launch;
    
    if (monitor) {
        monitor->flag_index = 0;
    }
}



#pragma mark Step actor update by hand


liz_vm_cmd_t
liz_vm_step_cmd(liz_vm_t *vm,
                liz_vm_actor_t *actor,
                void * LIZ_RESTRICT actor_blackboard,
                liz_vm_monitor_t *monitor,
                liz_vm_shape_t const *shape,
                liz_vm_cmd_t const cmd)
{
    liz_vm_cmd_t next_cmd = liz_vm_cmd_done;
    
    switch (cmd) {
        case liz_vm_cmd_invoke_node:
            next_cmd = liz_vm_step_invoke_node(vm,
                                               actor_blackboard,
                                               actor,
                                               monitor,
                                               shape);
            break;
            
        case liz_vm_cmd_guard_and_traverse:
            next_cmd = liz_vm_step_guard_and_traverse(vm,
                                                      actor_blackboard,
                                                      actor,
                                                      monitor,
                                                      shape);
            break;
            
        case liz_vm_cmd_cleanup_and_actor_update:
            next_cmd = liz_vm_step_cleanup_and_actor_update(vm,
                                                            actor_blackboard,
                                                            actor,
                                                            monitor,
                                                            shape);
            break;
            
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
                        void * LIZ_RESTRICT actor_blackboard,
                        liz_vm_actor_t *actor,
                        liz_vm_monitor_t *monitor,
                        liz_vm_shape_t const *shape)
{
    LIZ_ASSERT(shape->spec.shape_atom_count > vm->shape_atom_index);
    
    liz_vm_monitor_node_flag_t traversal_direction = liz_vm_monitor_node_flag_enter_from_top;
    LIZ_VM_MONITOR_NODE(monitor,
                        traversal_direction,
                        vm,
                        actor_blackboard,
                        actor,
                        shape);
    
    liz_vm_cmd_t next_cmd = liz_vm_cmd_error;
    
    switch (liz_vm_current_node_type(vm, shape)) {
        case liz_node_type_immediate_action:
            liz_vm_invoke_immediate_action(vm,
                                           actor_blackboard,
                                           actor,
                                           shape);
            
            // Traverse up to decider, alas its associated guard.
            next_cmd = liz_vm_cmd_guard_and_traverse;
            traversal_direction = liz_vm_monitor_node_flag_leave_to_top;
            
            break;
            
        case liz_node_type_deferred_action:
            liz_vm_invoke_deferred_action(vm,
                                          actor,
                                          shape);
            
            // Traverse up to decider, alas its associated guard.
            next_cmd = liz_vm_cmd_guard_and_traverse;
            traversal_direction = liz_vm_monitor_node_flag_leave_to_top;
            
            break;
            
        case liz_node_type_persistent_action:
            liz_vm_invoke_persistent_action(vm,
                                            actor,
                                            shape);
            
            // Traverse up to decider, alas its associated guard.
            next_cmd = liz_vm_cmd_guard_and_traverse;
            traversal_direction = liz_vm_monitor_node_flag_leave_to_top;
            
            break;
            
        case liz_node_type_sequence_decider: // Fall through
        case liz_node_type_dynamic_priority_decider: // Fall through
        case liz_node_type_concurrent_decider:
            liz_vm_invoke_common_decider(vm,
                                         actor,
                                         shape);
            
            // Traverse down to a decider child.
            next_cmd = liz_vm_cmd_invoke_node;
            traversal_direction = liz_vm_monitor_node_flag_leave_to_bottom;
            
            break;
            
        default:
            assert(0 && "Unhandled node type.");
            break;
    }
    
    (void)traversal_direction; // Inhibit compiler warning if monitoring is off.
    LIZ_VM_MONITOR_NODE(monitor, 
                        traversal_direction,
                        vm,
                        actor_blackboard,
                        actor,
                        shape);
    
    return next_cmd;
}



liz_vm_cmd_t
liz_vm_step_guard_and_traverse(liz_vm_t *vm,
                               void * LIZ_RESTRICT actor_blackboard,
                               liz_vm_actor_t *actor,
                               liz_vm_monitor_t *monitor,
                               liz_vm_shape_t const *shape)
{
    // An empty guard stack means that the root node has been left to its top,
    // alas the update is done.
    if (liz_lookaside_stack_is_empty(&vm->decider_guard_stack_header)) {
        return liz_vm_cmd_cleanup_and_actor_update;
    }
    
    // Process decider node's child execution state, alas guard how to react.
    liz_vm_decider_guard_t const *top_guard = liz_vm_current_top_decider_guard(vm);
    liz_vm_monitor_node_flag_t traversal_direction = liz_vm_monitor_node_flag_enter_from_bottom;
    
    LIZ_VM_MONITOR_NODE(monitor,
                        traversal_direction,
                        vm, 
                        actor_blackboard,
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
        next_cmd = liz_vm_cmd_guard_and_traverse;
        traversal_direction = liz_vm_monitor_node_flag_leave_to_top;
        
    } else { 
        // When switching from traversing up to traversing down again, first
        // Cancel actions in marked cancellation range so new states won't mix
        // with abandoned ones on the following node invocation.
        liz_vm_cancel_actions_in_cancellation_range(vm,
                                                    actor_blackboard,
                                                    actor,
                                                    monitor,
                                                    shape);
        
        next_cmd = liz_vm_cmd_invoke_node;
        traversal_direction = liz_vm_monitor_node_flag_leave_to_bottom;
    }
    
    (void)traversal_direction; // Inhibit compiler warning if monitoring is off.
    LIZ_VM_MONITOR_NODE(monitor,
                        traversal_direction,
                        vm, 
                        actor_blackboard,
                        actor,
                        shape);
    
    return next_cmd;
}



liz_vm_cmd_t
liz_vm_step_cleanup_and_actor_update(liz_vm_t *vm,
                                     void * LIZ_RESTRICT actor_blackboard,
                                     liz_vm_actor_t *actor,
                                     liz_vm_monitor_t *monitor,
                                     liz_vm_shape_t const *shape)
{
    
}



#pragma mark Internals

#pragma mark Enter nodes from top



void
liz_vm_invoke_immediate_action(liz_vm_t *vm,
                               void * LIZ_RESTRICT actor_blackboard,
                               liz_vm_actor_t const *actor,
                               liz_vm_shape_t const *shape)
{
    liz_shape_atom_t const *node = &shape->atoms[vm->shape_atom_index];
    
    // Determine the action state.
    liz_execution_state_t exec_state = liz_execution_state_launch;
    if (liz_find_key(&vm->actor_action_state_index,
                     vm->shape_atom_index,
                     liz_vm_actor_action_state_shape_atom_indices(actor),
                     shape->spec.shape_atom_count)) {
        
        uint8_t const *actor_action_states = liz_vm_actor_action_states(actor); 
        exec_state = (liz_execution_state_t)actor_action_states[vm->actor_action_state_index];
        
        // If action fails then cancellation shouldn't try to cancel it.
        vm->actor_action_state_index += 1u;
    }
    
    // Call the immediate action.
    exec_state = liz_vm_tick_immediate_action(actor_blackboard,
                                              exec_state,
                                              node,
                                              shape->immediate_action_functions,
                                              shape->spec.immediate_action_function_count);
    LIZ_ASSERT(liz_execution_state_cancel != exec_state && "Immediate actions must not return liz_execution_state_cancel when not called with it.");
    
    // Store running state.
    if (liz_execution_state_running == exec_state) {
        liz_lookaside_stack_push(&vm->action_state_stack_header);
        liz_int_t const top_index = liz_lookaside_stack_top_index(&vm->action_state_stack_header);   
        
        vm->action_states[top_index] = (uint8_t)exec_state;
        vm->action_state_shape_atom_indices[top_index] = vm->shape_atom_index;
    }
    
    vm->execution_state = exec_state;
    vm->cmd = liz_vm_cmd_guard_and_traverse;
    vm->shape_atom_index += 1u;
}



void
liz_vm_invoke_deferred_action(liz_vm_t *vm,
                              liz_vm_actor_t const *actor,
                              liz_vm_shape_t const *shape)
{
    // Fetch state.
    
    // Push request if no state exists, push launch and set state to 
    // launch, otherwise push running and switch state to running.
    
    // Set state.
    
    // Monitor ascent out of node.
    
    // Next cmd = guard_and_traverse
    
    // Increment shape item cursor
}



void
liz_vm_invoke_persistent_action(liz_vm_t *vm,
                                liz_vm_actor_t const *actor,
                                liz_vm_shape_t const *shape)
{
    // Fetch state
    
    // Set state.
    
    // Monitor ascent out of node.
    
    // Next cmd = guard_and_traverse
    
    // Increment shape item cursor
}



void
liz_vm_invoke_common_decider(liz_vm_t *vm,
                             liz_vm_actor_t const *actor,
                             liz_vm_shape_t const *shape)
{
    // Return guard_and_traverse if guard is empty, otherwise return invoke.
    
    // Fetch state.
    
    // Set up and push guard, advance shape item cursor.
    
    // Set state to fail - no consequence when descending but w/o a 
    // child the ascent means a fail.
    
    // Next cmd = traverse
    
    // Set the aggregate state to success - can not be set to launch as launch
    // and running are treated the same to simplify action state handling for
    // deferred actions. Alas: assume the best for the concurrent decider.
    
}



#pragma mark Re-enter decider from bottom



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
            
            vm->decider_states[top_index] = guard->sequence_child_index;
            vm->decider_state_shape_atom_indices[top_index] = guard->shape_atom_index;
            
            vm->shape_atom_index = (uint16_t)guard->end_index;
            break;
        }
            
        case liz_execution_state_success:
            // Child succeeded, on to the next child, remember reached child.
            guard->sequence_child_index = (uint16_t)vm->shape_atom_index; 
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
            guard->concurrent_aggregated_state = (uint8_t)liz_execution_state_running;            
            break;
            
        case liz_execution_state_success:
            // Child succeeded, on to the next child.
            // For the case that this was the last child of the decider, re-
            // establish the aggregated state. It starts as success and when
            // switched to running sticks to it as long as no child fails.
            vm->execution_state = guard->concurrent_aggregated_state;
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
liz_vm_cancel_actions_in_cancellation_range(liz_vm_t *vm,
                                            void * LIZ_RESTRICT actor_blackboard,
                                            liz_vm_actor_t const *actor,
                                            liz_vm_monitor_t *monitor,
                                            liz_vm_shape_t const *shape)
{
    if (liz_vm_cancellation_range_is_empty(vm->cancellation_range)) {
        return;
    }
   
    liz_vm_cancel_running_actions_from_current_update(vm,
                                                      actor_blackboard,
                                                      monitor,
                                                      shape);
    
    // Running this after the jump back to have linear shape atom stream 
    // iteration during cancellation.
    liz_vm_cancel_launched_and_running_actions_from_previous_update(vm,
                                                                    actor_blackboard,
                                                                    actor,
                                                                    monitor,
                                                                    shape);
    
    // Clear alas empty the cancellation range.
    vm->cancellation_range = (liz_vm_cancellation_range_t){
        vm->shape_atom_index,
        vm->shape_atom_index
    };
}



void
liz_vm_monitor_node(liz_vm_monitor_t *monitor,
                    unsigned int mask,
                    liz_vm_t const *vm,
                    void const * LIZ_RESTRICT blackboard,
                    liz_vm_actor_t const *actor,
                    liz_vm_shape_t const *shape)
{

}



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
        action_begin_shape_atom->deferred_action_first.resource,
        shape_atom_index
    };
    
    return execution_request;
}



void
liz_vm_cancel_immediate_or_deferred_action(void * LIZ_RESTRICT actor_blackboard,
                                           liz_vm_action_request_t *deferred_action_requests,
                                           liz_lookaside_double_stack_t *deferred_action_request_stack_header,
                                           liz_shape_atom_t const *action_begin_shape_atom,
                                           liz_int_t const shape_atom_index,
                                           liz_immediate_action_func_t const *immediate_action_functions,
                                           liz_int_t const immediate_action_function_count)
{
    switch (action_begin_shape_atom->type_mask.type) {
        case liz_node_type_immediate_action:
        {
            liz_execution_state_t exec_state = liz_vm_tick_immediate_action(actor_blackboard,
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
                                                  void * LIZ_RESTRICT actor_blackboard,
                                                  liz_vm_monitor_t *monitor,
                                                  liz_vm_shape_t const *shape)
{
    liz_vm_cancellation_range_t const range = vm->cancellation_range;
    liz_int_t first_action_index = 0;
    
    // If this shows up as a hotspot implement and test a backward key search.
    (void)liz_find_key(&first_action_index,
                       range.begin_index,
                       vm->action_state_shape_atom_indices ,
                       liz_lookaside_stack_count(&vm->action_state_stack_header));
    
    for (liz_int_t i = first_action_index; i < liz_lookaside_stack_count(&vm->action_state_stack_header); ++i) {
        LIZ_ASSERT(range.begin_index <= vm->action_state_shape_atom_indices[i] 
                   && "Vm action states must be in increasing shape atom index order.");
        LIZ_ASSERT(range.end_index > vm->action_state_shape_atom_indices[i] 
                   && "Actions must be cancelled before invoking new actions outside of the cancellation range.");
        
        if (liz_execution_state_launch == (liz_execution_state_t)(vm->action_states[i])) {
            // Action launch requests from deferred actions invoked during the 
            // current update have already been rolled back by the decider
            // guards and neat no further treatment.
            continue;
        }
        
        liz_vm_cancel_immediate_or_deferred_action(actor_blackboard,
                                                   vm->action_requests,
                                                   &vm->action_request_stack_header,
                                                   &shape->atoms[vm->action_state_shape_atom_indices[i]],
                                                   vm->action_state_shape_atom_indices[i],
                                                   shape->immediate_action_functions,
                                                   shape->spec.immediate_action_function_count);
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
                                                                void * LIZ_RESTRICT actor_blackboard,
                                                                liz_vm_actor_t const *actor,
                                                                liz_vm_monitor_t *monitor,
                                                                liz_vm_shape_t const *shape)
{
    liz_vm_cancellation_range_t const range = vm->cancellation_range;
    uint16_t const *actor_action_state_shape_atom_indices = liz_vm_actor_action_state_shape_atom_indices(actor);
    
    liz_int_t actor_action_index = vm->actor_action_state_index;
    (void)liz_find_key(&actor_action_index, 
                       range.begin_index,
                       actor_action_state_shape_atom_indices, 
                       actor->action_state_count);
    
    for (; actor_action_index < actor->action_state_count; ++actor_action_index) {
        
        liz_int_t const shape_atom_index = actor_action_state_shape_atom_indices[actor_action_index];
        if (range.end_index <= shape_atom_index) {
            break;
        }
        
        uint8_t const *actor_action_states = liz_vm_actor_action_states(actor);
        liz_execution_state_t const action_state = (liz_execution_state_t)(actor_action_states[actor_action_index]);
        if (liz_execution_state_launch != action_state
            && liz_execution_state_running != action_state) {
            // Only cancel yet not visited launched or running actions.
            continue;
        }
        
        liz_vm_cancel_immediate_or_deferred_action(actor_blackboard,
                                                   vm->action_requests,
                                                   &vm->action_request_stack_header,
                                                   &shape->atoms[shape_atom_index],
                                                   shape_atom_index,
                                                   shape->immediate_action_functions,
                                                   shape->spec.immediate_action_function_count);
    }
    
    // Set the vm's actor action state index forward to not read the cancelled
    // action states again during the current update.
    vm->actor_action_state_index = actor_action_index;
}



