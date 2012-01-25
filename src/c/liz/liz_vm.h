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
 * Virtual machine kernel interpreting an actor's behavior tree shape.
 *
 * Vms working on different actors are independent of each other and do not 
 * affect each other - they can update in parallel if the actor blackboard and
 * user monitoring functions and context are also isolated from each other.
 *
 * Vms working on actor's based on the same behavior tree shape share read 
 * access to the shape data.
 *
 * Typical usage:
 * 1. Create vm(s) which fulfills the specifications of all actors and their 
 *    shapes to update.
 * 2. During each simulation step, for each actor:
 * 2.1. Independent of vm: update the actor action states and persistent action 
 *      stateswith changes from deferred action executing systems.
 * 2.2. Call update actor and then extract the action requests of a vm. 
 *      Call actor cancel to create cancel requests for all running deferred 
 *      actions.
 * 2.3. Independent of vm: execute the deferred and persistent actions in their
 *      respective systems and collect action updates.
 * 3. Destroy vm(s).
 *
 *
 * Define LIZ_VM_MONITOR_ENABLE to compile with runtime traversal monitoring.
 * Pass NULL for monitor to ignore traversal monitoring at runtime even with
 * defined LIZ_VM_MONITOR_ENABLE.
 *
 *
 * TODO: @todo Add a shape validation function to stop malformed behavior trees
 *             from being interpreted by the vm.
 *
 * TODO: @todo Decide if the vm and vm monitor should have an error state or 
 *             not and how to react to malformed shapes or invalid execution
 *             states feed into and returned from nodes.
 *
 * TODO: @todo The moment I add the actor clip and liz system always check 
 *             state changes updates passed in from the outside to have valid
 *             values.
 */

#ifndef LIZ_liz_vm_H
#define LIZ_liz_vm_H

#include <liz/liz_platform_types.h>
#include <liz/liz_platform_macros.h>
#include <liz/liz_assert.h>
#include <liz/liz_allocator.h>
#include <liz/liz_common.h>
#include <liz/liz_common_internal.h>
#include <liz/liz_lookaside_stack.h>
#include <liz/liz_lookaside_double_stack.h>


#if defined(__cplusplus)
extern "C" {
#endif

#pragma mark Types, enums, defines
    

    /** 
     * Vm commands representing a traversal step into a node, up or down the 
     * behavior tree, and the final vm cleanup.
     */
    typedef enum liz_vm_cmd {
        liz_vm_cmd_invoke_node = 0, /**< Tick a node entered from top. */
        liz_vm_cmd_guard_decider, /**< Guard a decider and traverse up or down. */
        liz_vm_cmd_cleanup, /**< Cancel if necessart and sort vm states for extraction. */
        liz_vm_cmd_done, /**< Run is done. Do not forget to extract the action requests yourself. */
        liz_vm_cmd_error /**< This must not happen, the behavior tree is malformed. */
    } liz_vm_cmd_t;
    
    
    
    /**
     * Provides direct access to actor data that might be stored in a data blob.
     */
    typedef struct liz_vm_actor {
        liz_actor_header_t *header;
        
        liz_persistent_state_t *persistent_states;
        
        uint16_t *decider_state_shape_atom_indices;
        uint16_t *decider_states;
        
        uint16_t *action_state_shape_atom_indices;
        uint8_t *action_states;
    } liz_vm_actor_t;
    
    
    
    /**
     * Provides direct access to shape data that might be stored in a data blob.
     */
    typedef struct liz_vm_shape {
        liz_shape_atom_t *atoms;
        uint16_t *persistent_state_shape_atom_indices;
        
        liz_immediate_action_func_t *immediate_action_functions;
        
        liz_shape_specification_t spec;
    } liz_vm_shape_t;
    
    
    
    /**
     * Assumed minimal alignment in bytes.
     */
#define LIZ_VM_ACTION_REQUEST_ALIGNMENT sizeof(uint32_t)
    
    
    
    /**
     * Action requests generated during an actor update by a vm.
     *
     * As a vm works on a single actor during an update it saves memory by not
     * storing the actor's id.
     *
     * TODO: @todo Decide if to use liz_action_request inside the vm instead to
     *             cut down type counts and to simplify request copying which
     *             removes the need for a local external request buffer on
     *             distributed memory machines.
     */
    typedef struct liz_vm_action_request {
        uint32_t action_id;
        uint16_t resource_id;
        uint16_t shape_atom_index;
    } liz_vm_action_request_t;
    
    
    
    /**
     * Assumed minimal alignment in bytes.
     */
#define LIZ_VM_DECIDER_GUARD_ALIGNMENT sizeof(uint16_t)
    
    
    
    /**
     * On entering a decider node from above the vm pushes a guard onto a stack
     * to keep track of it's traversal location in the behavior tree - the guard
     * is interpreted when traversal returns from its child nodes to handle the
     * original nodes traversal semantics.
     *
     * A guard stack is used to minimize the necessity to jump back in the 
     * shape atom stream to know the type of a node and to note a decider's 
     * state based on the returned execution state of its children.
     *
     * When adding rollback markers for immediately cancelable states and 
     * requestsalso add rollback handling to 
     * liz_vm_cancel_immediately_by_guard.
     */
    typedef struct liz_vm_decider_guard {
        uint16_t shape_atom_index;
        uint16_t end_index;
        
        uint16_t decider_state_rollback_marker;
        uint16_t action_launch_request_rollback_marker;
        
        // Next two fields are guard type dependent. Storing them in a union
        // complicates initialization so no union as long as there is enough
        // space.
        uint16_t sequence_reached_child_index;
        uint8_t concurrent_execution_state;

        uint8_t type;
        
        // 128 bit alas 16 byte padding to be usable as a reorder stack with
        // the right alignment for persistent state changes.
        char padding_to_16_bytes[4];
    } liz_vm_decider_guard_t;
    
    
    
    /**
     * Range of shape stream indices which might contain running actions to
     * cancel.
     */
    typedef struct liz_vm_cancellation_range {
        uint16_t begin_index;
        uint16_t end_index;
    } liz_vm_cancellation_range_t;
    
    
    
#define LIZ_VM_ACTION_REQUEST_STACK_SIDE_LAUNCH liz_lookaside_double_stack_side_low
#define LIZ_VM_ACTION_REQUEST_STACK_SIDE_CANCEL liz_lookaside_double_stack_side_high
    
    /**
     * Assumed minimal vm alignment based on alignment of first field on 32bit
     * platforms.
     */
#define LIZ_VM_ALIGNMENT sizeof(liz_int_t)
    
    /**
     *
     *
     * When adding states and requests to rollback immediately on cancellation 
     * add associated markers to liz_vm_decider_guard_t.
     */
    typedef struct liz_vm {
        liz_int_t shape_atom_index;
        liz_int_t actor_decider_state_index;
        liz_int_t actor_action_state_index;
        liz_int_t actor_persistent_state_index;
        
        uint16_t *persistent_state_change_shape_atom_indices;
        uint16_t *decider_state_shape_atom_indices;
        uint16_t *action_state_shape_atom_indices;
        
        liz_persistent_state_t *persistent_state_changes;
        uint16_t *decider_states;
        uint8_t *action_states;
        
        liz_vm_decider_guard_t *decider_guards;
        liz_vm_action_request_t *action_requests;
        
        liz_random_number_seed_t actor_random_number_seed;
        
        liz_lookaside_stack_t persistent_state_change_stack_header;
        liz_lookaside_stack_t decider_state_stack_header;
        liz_lookaside_stack_t action_state_stack_header;
        
        liz_lookaside_stack_t decider_guard_stack_header;
        liz_lookaside_double_stack_t action_request_stack_header;
        
        liz_vm_cancellation_range_t cancellation_range;
        
        liz_vm_cmd_t cmd;
        liz_execution_state_t execution_state;
    } liz_vm_t;
    
    
    
    typedef enum liz_vm_monitor_node_flag {
        liz_vm_monitor_node_flag_ignore = 0u,
        liz_vm_monitor_node_flag_enter_from_top = (1u << 0u),
        liz_vm_monitor_node_flag_leave_to_top = (1u << 1u),
        liz_vm_monitor_node_flag_enter_from_bottom = (1u << 2u),
        liz_vm_monitor_node_flag_leave_to_bottom = (1u << 3u),
        liz_vm_monitor_node_flag_cancel_action = (1u << 4u), 
        liz_vm_monitor_node_flag_error = (1u << 5u)
    } liz_vm_monitor_node_flag_t;
    

    
    /**
     * With LIZ_VM_MONITOR_ENABLE defined the monitor function is called 
     * during each traversal step into or out of a node.
     *
     * traversal_mask is exactly one of the values from liz_vm_monitor_node_flag 
     * excluding liz_vm_monitor_node_flag_ignore and 
     * liz_vm_monitor_node_flag_cancel_action during normal traversal.
     *
     * During cancellation handling liz_vm_monitor_node_flag_cancel_action is
     * OR-ed together with liz_vm_monitor_node_flag_enter_from_top or with
     * liz_vm_monitor_node_flag_leave_to_top.
     */
    typedef void (*liz_vm_monitor_func_t)(uintptr_t user_data,
                                          liz_uint_t const node_shape_atom_index,
                                          liz_uint_t const traversal_mask,
                                          liz_vm_t const *vm,
                                          void const * LIZ_RESTRICT actor_blackboard,
                                          liz_time_t const time,
                                          liz_vm_actor_t const *actor,
                                          liz_vm_shape_t const *shape);
    
    
    
    typedef struct liz_vm_monitor {
        uintptr_t user_data;
        liz_vm_monitor_func_t func;
    } liz_vm_monitor_t;
    
    
    
    /**
     * Stream of monitor breakpoints.
     *
     * Breakpoints are ordered for the shape atom index they monitor.
     * 
     * When the vm visits a node and monitoring is enabled and a breakpoint is
     * set for the node the flag is matched against the traversal direction
     * and if they match then the indexed breakpoint function is called.
     *
     * TODO: @todo Extract into a breakpoint monitor system.
     */
    /* Commented out - too complex and can be implemented on top of the simpler
     * monitoring scheme.
    typedef struct liz_vm_monitor_breakpoint_stream {
        uint16_t const *shape_atom_indices;
        uint16_t const *function_indices;
        liz_vm_monitor_node_flag_t const *flags;
        char const * const *labels;
        
        uint16_t count;
    } liz_vm_monitor_breakpoint_stream_t;
    
    
    
    typedef struct liz_vm_monitor_decider_guard_breakpoint {
        char const *label;
        uint16_t shape_atom_index;
        uint16_t end_index;
        uint16_t function_index;
        uint8_t flag;
    } liz_vm_monitor_decider_guard_breakpoint_t;
    */
    
    
    /** 
     * 
     *
     * user_data is passed to the breakpoint function when called.
     *
     * Create a monitor breakpoint stream per shape or, to monitor an individual
     * actor specially, per actor.
     *
     * When using multiple liz vms (in parallel) then create a monitor per vm.
     * Use user_data to store a vm identifier.
     *
     * TODO: @todo Extract into a breakpoint monitor system.
     */
    /* Commented out - too complex and can be implemented on top of the simpler
     * monitoring scheme.
    struct liz_vm_monitor {
        uintptr_t user_data;
        liz_vm_monitor_breakpoint_func_t const *breakpoint_functions;
        liz_vm_monitor_breakpoint_stream_t const *node_breakpoints;
        liz_vm_monitor_decider_guard_breakpoint_t *guard_breakpoints;
        liz_int_t breakpoint_index;
        liz_lookaside_stack_t guard_breakpoints_stack_header;
        uint16_t breakpoint_function_count;
    };
    */
    
    
    /**
     * Users can store data in an actor which is looked up via an also user
     * provided function at the beginning of an actor update by a vm.
     */
    typedef void* (*liz_vm_user_data_lookup_func_t)(void *context, 
                                                    uintptr_t user_data);
    
    
    
#pragma mark Create and destroy vm
    
    
    /**
     * Returns the memory size in bytes necessary to store a vm capable of
     * working on actors adhering to the shape specification spec.
     */
    size_t
    liz_vm_memory_size_requirement(liz_shape_specification_t spec);
    
    
    
    liz_vm_t*
    liz_vm_create(liz_shape_specification_t spec,
                  void * LIZ_RESTRICT allocator_context,
                  liz_alloc_func_t alloc_func);
    
    
    
    void
    liz_vm_destroy(liz_vm_t *vm,
                   void * LIZ_RESTRICT allocator_context,
                   liz_dealloc_func_t dealloc_func);
    
    
    
#pragma mark Update actor
    
    /**
     * Checks if the vm's buffers have enough capacity to process the 
     * specified behavior tree.
     */
    bool
    liz_vm_fulfills_shape_specification(liz_vm_t const *vm,
                                        liz_shape_specification_t spec);
    
    
    /**
     * Traverses an actor's behavior tree and updates the actor.
     *
     * @attention Resets vm as a first step - all non-extracted states and
     *            action requests are lost.
     *
     * Call liz_vm_extract_and_clear_actor_state afterwards to 
     * Call liz_vm_extract_and_clear_action_requests afterwards to get the 
     * requests.
     *
     * @attention Only call if  liz_vm_fulfills_shape_specification is true.
     */
    void
    liz_vm_update_actor(liz_vm_t *vm,
                        liz_vm_monitor_t *monitor,
                        void * LIZ_RESTRICT user_data_lookup_context,
                        liz_vm_user_data_lookup_func_t user_data_lookup_func,
                        liz_time_t const time,
                        liz_vm_actor_t const *actor,
                        liz_vm_shape_t const *shape);
    
    
    /**
     * Cancels running immediate actor and creates cancellation requests for its 
     * active deferred actions.
     *
     * @attention Resets vm as a first step - all non-extracted states and
     *            action requests are lost.
     *
     * Call liz_vm_extract_and_clear_actor_state afterwards to 
     * Call liz_vm_extract_and_clear_action_requests afterwards to get the 
     * requests.
     *
     * @attention Only call if  liz_vm_fulfills_shape_specification is true.
     */
    void
    liz_vm_cancel_actor(liz_vm_t *vm,
                        liz_vm_monitor_t *monitor,
                        void * LIZ_RESTRICT user_data_lookup_context,
                        liz_vm_user_data_lookup_func_t user_data_lookup_func,
                        liz_time_t const time,
                        liz_vm_actor_t const *actor,
                        liz_vm_shape_t const *shape);
    
    
    
    /**
     * Replaces actor's state with the state aggregated in vm.
     *
     * If the state in vm doesn't fit actor, then behavior is undefined.
     */
    void
    liz_vm_extract_actor_state(liz_vm_t const *vm,
                               liz_vm_actor_t *target_actor,
                               liz_vm_shape_t const *shape);
    
    
    /**
     * Copies the action launch and cancel requests of the last update from vm
     * to external_requests and returns the number of copied requests.
     *
     * TODO: @todo Change function to allow chunk-wise extraction of requests.
     *
     * TODO: @todo Add a way to just copy the launch or cancel requests and ways
     *             to query the different request counts.
     */
    liz_int_t
    liz_vm_extract_action_requests(liz_vm_t const *vm,
                                   liz_action_request_t *external_requests,
                                   liz_int_t const external_request_capacity,
                                   liz_id_t const actor_id);
    
    
    /**
     * Resets the vm, e.g., clears all buffers.
     * 
     * @attention If the monitor user data needs to be reset between vm runs,
     *            then reset it after or before updating/cancelling an actor
     *            with the vm, too.
     */
    void
    liz_vm_reset(liz_vm_t *vm);
    
    
    
#pragma mark Step actor update by hand
    
    /**
     * Just runs cmd and returns the next to run.
     *
     * Does not store the next command to run in vm.
     *
     * @attention Do not step invalid commands, e.g., do not call invoke 
     *            node if there is no valid node.
     */
    void
    liz_vm_step(liz_vm_t *vm,
                liz_vm_monitor_t *monitor,
                void * LIZ_RESTRICT actor_blackboard,
                liz_time_t const time,
                liz_vm_actor_t const *actor,
                liz_vm_shape_t const *shape);
    
    
    /**
     * Traverses into a node - an action or a decider - from the top.
     *
     * @attention Do not call without a valid node to invoke.
     */
    void
    liz_vm_step_invoke_node(liz_vm_t *vm,
                            liz_vm_monitor_t *monitor,
                            void * LIZ_RESTRICT actor_blackboard,
                            liz_time_t const time,
                            liz_vm_actor_t const *actor,
                            liz_vm_shape_t const *shape);
    
    
    /**
     * Re-enters a decider node from the bottom, alas calls the guard
     * associated with the decider.
     */
    void
    liz_vm_step_guard_decider(liz_vm_t *vm,
                              liz_vm_monitor_t *monitor,
                              void * LIZ_RESTRICT actor_blackboard,
                              liz_time_t const time,
                              liz_vm_actor_t const *actor,
                              liz_vm_shape_t const *shape);
    
    
    
    void
    liz_vm_step_cleanup(liz_vm_t *vm,
                        liz_vm_monitor_t *monitor,
                        void * LIZ_RESTRICT actor_blackboard,
                        liz_time_t const time,
                        liz_vm_actor_t const *actor,
                        liz_vm_shape_t const *shape);
    
    
    
#pragma mark Internals
    
#pragma mark Enter nodes from top
    
    
    
    void
    liz_vm_invoke_immediate_action(liz_vm_t *vm,
                                   void * LIZ_RESTRICT actor_blackboard,
                                   liz_time_t const time,
                                   liz_vm_actor_t const *actor,
                                   liz_vm_shape_t const *shape);
    
    
    void
    liz_vm_invoke_deferred_action(liz_vm_t *vm,
                                  liz_vm_actor_t const *actor,
                                  liz_vm_shape_t const *shape);
    
    void
    liz_vm_invoke_persistent_action(liz_vm_t *vm,
                                    liz_vm_actor_t const *actor,
                                    liz_vm_shape_t const *shape);
    
    

    void
    liz_vm_invoke_sequence_decider(liz_vm_t *vm,
                                   liz_vm_actor_t const *actor,
                                   liz_vm_shape_t const *shape);

    
    
    void
    liz_vm_invoke_dynamic_priority_decider(liz_vm_t *vm,
                                           liz_vm_actor_t const *actor,
                                           liz_vm_shape_t const *shape);
    
    
    
    void
    liz_vm_invoke_concurrent_decider(liz_vm_t *vm,
                                     liz_vm_actor_t const *actor,
                                     liz_vm_shape_t const *shape);
    
    
    
#pragma mark Re-enter deciders from bottom
    
    
    
    void
    liz_vm_guard_sequence_decider(liz_vm_t *vm);
    
    
    
    void
    liz_vm_guard_concurrent_decider(liz_vm_t *vm);
    
    
    
    void
    liz_vm_guard_dynamic_priority_decider(liz_vm_t *vm);
    
    
    
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
                liz_vm_action_request_t *action_requests);

    
    
    LIZ_INLINE static
    bool
    liz_vm_is_running(liz_vm_t const *vm) 
    {
        LIZ_ASSERT(liz_vm_cmd_error != vm->cmd
                   && "Behavior tree shape is malformed, or a user supplied immediate action function returns invalid execution states, or invalid external action states were not catched when set for an actor, or an implementation error triggered the error.");
        
        return (liz_vm_cmd_done != vm->cmd) && (liz_vm_cmd_error != vm->cmd);
    }
    
    
    
    /**
     * Cancels all of the following actions whose shape atom index is inside
     * the vm's cancellation range:
     * - the immediate and deferred actions with a running state from the 
     *   vm's action state buffer,
     * - all not yet visited immediate and deferred actions with a running or 
     *   launch state from the actor's action state buffer.
     *
     * This is the only place where shape stream traversal jumps back (to cancel
     * running actions just invoked by a concurrent node).
     * If jumping back in the stream shows up as a hotspot then rethink 
     * cancellation handling or store the action shape atoms together with their
     * action state in the vm.
     */
    void
    liz_vm_cancel_actions_in_cancellation_range(liz_vm_t *vm,
                                                liz_vm_monitor_t *monitor,
                                                void * LIZ_RESTRICT actor_blackboard,
                                                liz_time_t const time,
                                                liz_vm_actor_t const *actor,
                                                liz_vm_shape_t const *shape);
    
    
    
#if defined(LIZ_VM_MONITOR_ENABLE)
#   define LIZ_VM_MONITOR_NODE(monitor, shape_item_index, traversal_mask, vm, actor_blackboard, time, actor, shape) \
    liz_vm_monitor_node(monitor, shape_item_index, traversal_mask, vm, actor_blackboard, time, actor, shape);
#else
#   define LIZ_VM_MONITOR_NODE(monitor, shape_item_index, traversal_mask, vm, actor_blackboard, time, actor, shape) \
    do { \
        (void)monitor; \
        (void)shape_item_index; \
        (void)traversal_mask; \
        (void)vm; \
        (void)actor_blackboard; \
        (void)time; \
        (void)actor; \
        (void)shape; \
    } while (0)
#endif
    
    
    
    LIZ_INLINE static
    void
    liz_vm_monitor_node(liz_vm_monitor_t *monitor,
                        liz_uint_t const node_shape_atom_index,
                        liz_uint_t const traversal_mask,
                        liz_vm_t const *vm,
                        void const * LIZ_RESTRICT actor_blackboard,
                        liz_time_t const time,
                        liz_vm_actor_t const *actor,
                        liz_vm_shape_t const *shape)
    {
        if (NULL != monitor) {
            monitor->func(monitor->user_data,
                          node_shape_atom_index,
                          traversal_mask,
                          vm,
                          actor_blackboard,
                          time,
                          actor,
                          shape);
        }
    }
    
    
    
    LIZ_INLINE static
    liz_vm_decider_guard_t*
    liz_vm_current_top_decider_guard(liz_vm_t *vm)
    {
        LIZ_ASSERT(0 != liz_lookaside_stack_count(&vm->decider_guard_stack_header));
        
        liz_int_t const top_index = liz_lookaside_stack_top_index(&vm->decider_guard_stack_header);
        return &vm->decider_guards[top_index];
    }
    
    
    
    /**
     * Centralized way to detect and treat malformed behavior trees, alas 
     * deciders (inner nodes or branches) without children - it checks if a just
     * invoked decider has no children and might assert or might change cmd and
     * traversal_mask to show or repair the error.
     *
     * I am not sure if a childless decider should lead to an assertion or
     * error, or if a failing child should be simulated to keep traversal going.
     * Via this macro I can change my mind easily...
     */
#define LIZ_VM_CATCH_CHILDLESS_DECIDER(vm, cmd, traversal_mask) liz_vm_catch_childless_decider(vm, cmd, traversal_mask)
    
    
    
    LIZ_INLINE static
    void
    liz_vm_catch_childless_decider(liz_vm_t *vm,
                                   liz_vm_cmd_t *cmd,
                                   liz_vm_monitor_node_flag_t *traversal_direction)
    {
        if (liz_vm_current_top_decider_guard(vm)->end_index == vm->shape_atom_index) {
            // Assert on the error.
            LIZ_ASSERT(0 && "Childless decider equals a malformed beahvior tree shape.");
            
            // Repair the error and go on.
            vm->execution_state = liz_execution_state_fail;
            *cmd = liz_vm_cmd_guard_decider;
            (void)traversal_direction;
            
            // Detect and show the error.
            // vm->execution_state = liz_execution_state_fail;
            // *next_cmd = liz_vm_cmd_error;
            // *traversal_direction = liz_vm_monitor_node_flag_error;
        }        
    }
    
    
    
#define LIZ_VM_CATCH_INVALID_PERSISTENT_AND_IMMEDIATE_ACTION_STATE(exec_state) liz_vm_catch_invalid_persistent_and_immediate_action_state(exec_state)
    
    
    
    /** 
     * Called to check and handle bad persistent action states and
     * immediate action return states during node invokation.
     */
    LIZ_INLINE static
    void
    liz_vm_catch_invalid_persistent_and_immediate_action_state(liz_execution_state_t *exec_state)
    {
        LIZ_ASSERT((liz_execution_state_running == *exec_state 
                    || liz_execution_state_success == *exec_state 
                    || liz_execution_state_fail == *exec_state) 
                   && "Invalid execution state for persistent/immediate action.");
     
        switch (*exec_state) {
            case liz_execution_state_launch:
                *exec_state = liz_execution_state_running;
                break;
            case liz_execution_state_cancel:
                *exec_state = liz_execution_state_fail;
                break;
            default:
                // Everything is all right, nothing to do.
                break;
        }
    }
    
    
    
#define LIZ_VM_CATCH_INVALID_DEFERRED_ACTION_STATE(exec_state) liz_vm_catch_invalid_deferred_action_state(exec_state)
    
    
    
    LIZ_INLINE static
    void
    liz_vm_catch_invalid_deferred_action_state(liz_execution_state_t *exec_state)
    {
        LIZ_ASSERT(liz_execution_state_cancel != *exec_state 
                   && "Invalid execution state for deferred action.");
        
        if (liz_execution_state_cancel == *exec_state) {
            *exec_state = liz_execution_state_fail;
        }
    }

    
    
    LIZ_INLINE static
    bool
    liz_vm_cancellation_range_is_empty(liz_vm_cancellation_range_t const range)
    {
        bool const result = range.begin_index == range.end_index;
        
        return result;
    }
    
    
    
    /**
     * Replaces an empty range or grows a non-empty range if the new indices lie 
     * outside of the range but never shrinks it.
     *
     * new_begin_index must be less or equal to new_end_index, otherwise 
     * behavior is undefined.
     */
    void
    liz_vm_cancellation_range_adapt(liz_vm_cancellation_range_t *cancellation_range,
                                    uint16_t const new_begin_index,
                                    uint16_t const new_end_index);
    
    
    /**
     * Determines if shape_atom_index references an immediate or deferred action
     * and calles the related cancellation function, see
     * liz_vm_tick_immediate_action and liz_vm_launch_or_cancel_deferred_action.
     */
    void
    liz_vm_cancel_immediate_or_deferred_action(void * LIZ_RESTRICT actor_blackboard,
                                               liz_random_number_seed_t *rnd_seed,
                                               liz_vm_action_request_t *deferred_action_requests,
                                               liz_lookaside_double_stack_t *deferrec_action_request_stack_header,
                                               liz_time_t const time,
                                               liz_shape_atom_t const *shape_atoms,
                                               liz_int_t const shape_atom_index,
                                               liz_immediate_action_func_t const *immediate_action_functions,
                                               liz_int_t const immediate_action_function_count);
    
    
    /**
     * Tick, a.k.a. launch, run, terminate, or cancel an immediate action.
     */
    liz_execution_state_t
    liz_vm_tick_immediate_action(void * LIZ_RESTRICT actor_blackboard,
                                 liz_random_number_seed_t *rnd_seed,
                                 liz_time_t const time,
                                 liz_execution_state_t const execution_request,
                                 liz_shape_atom_t const *first_shape_atom_of_node_in_stream,
                                 liz_immediate_action_func_t const *immediate_action_functions,
                                 liz_int_t const immediate_action_function_count);
    
    
    
    /**
     * Call to emit an action launch or cancel request for a deferred action.
     */
    liz_execution_state_t
    liz_vm_launch_or_cancel_deferred_action(liz_vm_action_request_t *action_requests,
                                            liz_lookaside_double_stack_t *action_request_stack_header,
                                            liz_execution_state_t const execution_request,
                                            liz_shape_atom_t const *shape_atoms,
                                            liz_int_t const shape_atom_index);
    
    
    
    /**
     * Called by a guard to roll back action launch requests and decider states 
     * generated by guard children when the guard decides to cancel its 
     * children.
     *
     * This form of cancellation happens immediately while running actions
     * belonging to different guards are cancelled together just before the next
     * action can be called, a.k.a. before behavior tree traversal descends 
     * after ascending out of guarded nodes.
     */
    LIZ_INLINE static
    void
    liz_vm_cancel_immediately_by_guard(liz_vm_t *vm, 
                                       liz_vm_decider_guard_t const *guard)
    {
        liz_lookaside_stack_set_count(&vm->decider_state_stack_header, 
                                      guard->decider_state_rollback_marker);
        
        liz_lookaside_double_stack_set_count(&vm->action_request_stack_header, 
                                             guard->action_launch_request_rollback_marker, 
                                             LIZ_VM_ACTION_REQUEST_STACK_SIDE_LAUNCH);
    }
    
    
    
    /**
     * Cancel immediate and deferred actions that returned a running state
     * during the current update.
     *
     * @attention Do not call with an empty cancellation range in vm.
     */
    void
    liz_vm_cancel_running_actions_from_current_update(liz_vm_t *vm,
                                                      liz_vm_monitor_t *monitor,
                                                      void * LIZ_RESTRICT actor_blackboard,
                                                      liz_time_t const time,
                                                      liz_vm_actor_t const *actor,
                                                      liz_vm_shape_t const *shape);
    
    
    
    /**
     * Cancel immediate and deferred action with a running (or launching) state
     * from the previous update that have not been invoked during the current
     * update.
     *
     * @attention Do not call with an empty cancellation range in vm.
     */
    void
    liz_vm_cancel_launched_and_running_actions_from_previous_update(liz_vm_t *vm,
                                                                    liz_vm_monitor_t *monitor,
                                                                    void * LIZ_RESTRICT actor_blackboard,
                                                                    liz_time_t const time,
                                                                    liz_vm_actor_t const *actor,
                                                                    liz_vm_shape_t const *shape);
    
    
    /**
     * Call after extraction of a successfully seeked key.
     *
     * Increments the cursor, alas consumes the state it indexes. This is done
     * to prevent cancellation of an action with a fail state because the action
     * already terminated.
     */
    LIZ_INLINE static
    void
    liz_vm_consume_state(liz_int_t *cursor,
                         uint16_t const shape_atom_index,
                         uint16_t const *shape_atom_indices,
                         liz_int_t const shape_atom_index_count)
    {
        LIZ_ASSERT(*cursor < shape_atom_index_count);
        LIZ_ASSERT(shape_atom_index == shape_atom_indices[*cursor]);
        
        (void)shape_atom_index;
        (void)shape_atom_indices;
        (void)shape_atom_index_count;
        
        *cursor += 1;
    }
    
    
    
    /**
     * Reorders keys and their associated values which are in a pre-order 
     * traversal order according to increasing keys.
     */
    void
    liz_vm_sort_values_for_keys_from_post_order_traversal(void * LIZ_RESTRICT values,
                                                          uint16_t * LIZ_RESTRICT keys,
                                                          size_t const value_size_in_bytes,
                                                          size_t const value_alignment_in_bytes,
                                                          liz_int_t const key_value_count,
                                                          liz_vm_decider_guard_t *decider_guard_stack_buffer,
                                                          liz_lookaside_stack_t *decider_guard_stack_header);
    
#if defined(__cplusplus)
} /* extern "C" */
#endif


#endif /* LIZ_liz_vm_H */
