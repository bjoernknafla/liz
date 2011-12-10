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
 * Pass NULL for monitor to disable traversal monitoring at runtime.
 *
 *
 * TODO: @todo Flesh out monitoring, it's currently merely a placeholder.
 */

#ifndef LIZ_liz_vm_H
#define LIZ_liz_vm_H

#include <liz/liz_platform_types.h>
#include <liz/liz_platform_macros.h>
#include <liz/liz_common.h>
#include <liz/liz_common_internal.h>
#include <liz/liz_lookaside_stack.h>
#include <liz/liz_lookaside_double_stack.h>


#if defined(__cplusplus)
extern "C" {
#endif

#pragma mark Types, enums, defines
    
    
#if defined(LIZ_VM_MONITOR_ENABLE)
#   define LIZ_VM_MONITOR_NODE(monitor, mask, vm, actor_blackboard, actor, shape) liz_vm_monitor_node(monitor, mask, vm, actor_blackboard, actor, shape);
#else
#   define LIZ_VM_MONITOR_NODE(monitor, mask, vm, actor_blackboard, actor, shape) /* Nothing to do. */
#endif
    
    
    /** 
     *
     *
     * TODO: @todo Decide if to add a start command that checks if the shape
     *             has a node to invoke.
     *
     * TODO: @todo Profile if it makes sense to put invoke behind the guard and
     *             traverse command because they happen more frequently during
     *             ascending through the tree.
     */
    typedef enum liz_vm_cmd {
        liz_vm_cmd_invoke_node = 0, /**< Tick a node entered from top. */
        liz_vm_cmd_guard_and_traverse, /**< Guard a decider and traverse up or down. */
        liz_vm_cleanup_and_actor_update, /**< Cleanup the vm and copy the new states to the actor. */
        liz_vm_cmd_done, /**< Run is done. Do not forget to extract the action requests yourself. */
        liz_vm_error /**< This must not happen, the behavior tree is malformed. */
    } liz_vm_cmd_t;
    
    
    
    typedef struct liz_vm_actor {
        uintptr_t user_data;
        liz_id_t actor_id;
        
        /* Capacities are specified in the associated shape. */
        uint16_t decider_state_count;
        uint16_t action_state_count;
        
        uintptr_t persistent_states_offset;
        uintptr_t decider_state_shape_atom_indices_offset;
        uintptr_t decider_states_offset;
        uintptr_t action_state_shape_atom_indices_offset;
        uintptr_t action_states_offset;
    } liz_vm_actor_t;
    
    
    
    typedef struct liz_vm_shape {
        liz_shape_atom_t *atoms;
        uint16_t *persistent_state_shape_atom_indices;
        
        liz_immediate_action_func_t *immediate_action_functions;
        
        liz_shape_specification_t spec;
    } liz_vm_shape_t;
    
    
    
    /**
     *
     *
     * TODO: @todo Decide if to use liz_action_request inside the vm instead to
     *             cut down type counts and to simplify request copying which
     *             removes the need for a local external request buffer on
     *             distributed memory machines.
     */
    typedef struct liz_vm_action_request {
        uint32_t action_id;
        uint16_t resource;
        uint16_t shape_item_index;
    } liz_vm_action_request_t;
    
    
    
    typedef struct liz_vm_decider_guard {
        uint16_t shape_atom_index;
        uint16_t end_index;
        uint16_t sequence_child_index;
        
        uint16_t decider_state_rollback_marker;
        uint16_t action_launch_request_rollback_marker;
        
        uint8_t concurrent_aggregated_state;
        
        uint8_t monitor_node_flag;
        uint8_t type;
        
        char padding_to_128_bit[3];
    } liz_vm_decider_guard_t;
    
    
    
    typedef struct liz_vm_cancellation_range {
        uint16_t begin_index;
        uint16_t end_index;
    } liz_vm_cancellation_range_t;
    
    
    
    struct liz_vm {
        liz_int_t shape_atom_index;
        liz_int_t actor_decider_state_index;
        liz_int_t actor_action_state_index;
        liz_int_t actor_persistent_state_index;
        
        uint16_t *decider_state_shape_atom_indices;
        uint16_t *action_state_shape_atom_indices;
        
        uint16_t *decider_states;
        uint8_t *action_states;
        
        liz_vm_action_request_t *action_requests;
        liz_vm_decider_guard_t *decider_guards;
        
        liz_lookaside_stack_t decider_state_stack_header;
        liz_lookaside_stack_t action_state_stack_header;
        liz_lookaside_double_stack_t action_request_stack_header;
        liz_lookaside_stack_t decider_guard_stack_header;
        
        liz_vm_cancellation_range_t cancellation_range;
        
        liz_vm_cmd_t cmd;
        liz_execution_state_t execution_state;
    };
    
    
    
    typedef enum liz_vm_monitor_node_flag {
        liz_vm_monitor_node_flag_ignore = 0u,
        liz_vm_monitor_node_flag_enter_from_top = (1u << 0u),
        liz_vm_monitor_node_flag_leave_to_top = (1u << 1u),
        liz_vm_monitor_node_flag_enter_from_bottom = (1u << 2u),
        liz_vm_monitor_node_flag_leave_to_bottom = (1u << 3u)
        
    } liz_vm_monitor_node_flag_t;
    
    
    
    /** 
     *
     *
     * TODO: @todo Extend to contain breakpoints for the shape and for an actor.
     *
     * TODO: @todo Add an array for node names/descriptions to ease debugging.
     */
    struct liz_vm_monitor {
        void *context;
        
        liz_vm_monitor_node_func_t *breakpoint_funcs;
        uint16_t *node_flag_shape_atom_index;
        uint8_t *node_flags;
        
        liz_int_t flag_index;
    };
    
    
    
    typedef void (*liz_vm_monitor_node_func_t)(liz_vm_monitor_t *monitor,
                                               liz_vm_t const *vm,
                                               void const * LIZ_RESTRICT blackboard,
                                               liz_vm_actor_t const *actor,
                                               liz_vm_shape_t const *shape);
    
    
    
    typedef void* (*liz_vm_user_data_lookup_func_t)(void *context, 
                                                    uintptr_t user_data);
    
    
    
#pragma mark Create or destroy vm
    
    
    
    size_t
    liz_vm_memory_size(liz_shape_specification_t spec);
    
    
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
     * Call liz_vm_extract_action_requests to get the requests.
     *
     * @attention Only call if  liz_vm_fulfills_shape_specification is true.
     */
    void
    liz_vm_update_actor(liz_vm_t *vm,
                        liz_vm_actor_t *actor,
                        liz_action_request_buffer_t *external_requests,
                        liz_vm_monitor_t *monitor,
                        liz_vm_shape_t const *shape,
                        void * LIZ_RESTRICT user_data_lookup_context,
                        liz_vm_user_data_lookup_func_t user_data_lookup_func);
    
    
    /**
     * Creates cancellation requests for all running actions of actor.
     *
     * Call liz_vm_extract_action_requests to get the requests.
     *
     * @attention Only call if  liz_vm_fulfills_shape_specification is true.
     */
    void
    liz_vm_cancel_actor(liz_vm_t *vm,
                        liz_vm_actor_t *actor,
                        liz_action_request_buffer_t *external_requests,
                        liz_vm_monitor_t *monitor,
                        liz_vm_shape_t const *shape,
                        void * LIZ_RESTRICT user_data_lookup_context,
                        liz_vm_user_data_lookup_func_t user_data_lookup_func);
    
    
    /**
     * Copies the action launch and cancel requests of the last update to
     * external_requests.
     */
    void
    liz_vm_extract_action_requests(liz_vm_t *vm,
                                   liz_vm_actor_t *actor,
                                   liz_vm_monitor_t *monitor,
                                   liz_action_request_buffer_t *external_requests);
    
    
    /**
     * Resets the vm, e.g., clears all buffers.
     */
    void
    liz_vm_reset(liz_vm_t *vm,
                 liz_vm_monitor_t *monitor);
    
    
    
#pragma mark Step actor update by hand
    
    /**
     * Just runs cmd and returns the next to run.
     *
     * Does not store the next command to run in vm.
     *
     * @attention Do not step invalid commands, e.g., do not call invoke 
     *            node if there is no valid node.
     */
    liz_vm_cmd_t
    liz_vm_step_cmd(liz_vm_t *vm,
                    liz_vm_actor_t *actor,
                    void * LIZ_RESTRICT actor_blackboard,
                    liz_vm_monitor_t *monitor,
                    liz_vm_shape_t const *shape,
                    liz_vm_cmd_t const cmd);
    
    
    /**
     * Traverses into a node - an action or a decider - from the top.
     *
     * @attention Do not call without a valid node to invoke.
     */
    liz_vm_cmd_t
    liz_vm_step_invoke_node(liz_vm_t *vm,
                            void * LIZ_RESTRICT actor_blackboard,
                            liz_vm_actor_t *actor,
                            liz_vm_monitor_t *monitor,
                            liz_vm_shape_t const *shape);
    
    
    /**
     * Re-enters a decider node from the bottom, alas calls the guard
     * associated with the decider.
     */
    liz_vm_cmd_t
    liz_vm_step_guard_and_traverse(liz_vm_t *vm,
                                   void * LIZ_RESTRICT actor_blackboard,
                                   liz_vm_actor_t *actor,
                                   liz_vm_monitor_t *monitor,
                                   liz_vm_shape_t const *shape);
    
    
    
    liz_vm_cmd_t
    liz_vm_step_cleanup_and_actor_update(liz_vm_t *vm,
                                         void * LIZ_RESTRICT actor_blackboard,
                                         liz_vm_actor_t *actor,
                                         liz_vm_monitor_t *monitor,
                                         liz_vm_shape_t const *shape);
    
    
    
#pragma mark Internals
    
#pragma mark Enter nodes from top
    
    
    
    void
    liz_vm_invoke_immediate_action(liz_vm_t *vm,
                                   void * LIZ_RESTRICT actor_blackboard,
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
    
    
    /**
     * Covers sequence, concurrent, and dynamic priority decider.
     */
    void
    liz_vm_invoke_common_decider(liz_vm_t *vm,
                                 liz_vm_actor_t const *actor,
                                 liz_vm_shape_t const *shape);
    
    
    
#pragma mark Re-enter decider from bottom
    
    
    
    void
    liz_vm_guard_sequence_decider(liz_vm_t *vm);
    
    
    
    void
    liz_vm_guard_concurrent_decider(liz_vm_t *vm);
    
    
    
    void
    liz_vm_guard_dynamic_priority_decider(liz_vm_t *vm);
    
    
    
#pragma mark Helpers
    
    
    
    void
    liz_vm_cancel_actions(liz_vm_t *vm,
                          void * LIZ_RESTRICT actor_blackboard,
                          liz_vm_actor_t *actor,
                          liz_vm_monitor_t *monitor,
                          liz_vm_shape_t const *shape);
    
    
    
    void
    liz_vm_monitor_node(liz_vm_monitor_t *monitor,
                        unsigned int mask,
                        liz_vm_t const *vm,
                        void const * LIZ_RESTRICT blackboard,
                        liz_vm_actor_t const *actor,
                        liz_vm_shape_t const *shape);
    
    
    
    LIZ_INLINE static
    liz_persistent_state_t*
    liz_vm_actor_persistent_states(liz_vm_actor_t *actor)
    {
        return (liz_persistent_state_t *)(((char *)actor) + actor->persistent_states_offset);
    }
    
    
    
    LIZ_INLINE static
    uint16_t*
    liz_vm_actor_decider_state_shape_atom_indices(liz_vm_actor_t *actor)
    {
        return (uint16_t *)(((char *)actor) + actor->decider_state_shape_atom_indices_offset); 
    }
    
    
    
    LIZ_INLINE static
    uint16_t*
    liz_vm_actor_decider_states(liz_vm_actor_t *actor)
    {
        return (uint16_t *)(((char *)actor) + actor->decider_states_offset); 
    }
    
    
    
    LIZ_INLINE static
    uint16_t*
    liz_vm_actor_action_state_shape_atom_indices(liz_vm_actor_t *actor)
    {
        return (uint16_t *)(((char *)actor) + actor->action_state_shape_atom_indices_offset); 
    }
    
    
    
    LIZ_INLINE static
    uint8_t*
    liz_vm_actor_action_states(liz_vm_actor_t *actor)
    {
        return (uint8_t *)(((char *)actor) + actor->action_states_offset); 
    }
    
    
    
    LIZ_INLINE static
    liz_node_type_t
    liz_vm_current_node_type(liz_vm_t const *vm,
                             liz_vm_shape_t const *shape)
    {
        LIZ_ASSERT(shape->spec.shape_atom_count > vm->shape_atom_index);
        
        return (liz_node_type_t)(shape->atoms[vm->shape_atom_index].type_mask.type);
    }
    
    
    LIZ_INLINE static
    liz_vm_decider_guard_t*
    liz_vm_current_top_decider_guard(liz_vm_t *vm)
    {
        liz_int_t const top_index = liz_lookaside_stack_top_index(&vm->decider_guard_stack_header);
        return &vm->decider_guards[top_index];
    }

    
    
#if defined(__cplusplus)
} /* extern "C" */
#endif


#endif /* LIZ_liz_vm_H */
