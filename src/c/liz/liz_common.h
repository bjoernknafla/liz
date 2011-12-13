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

#ifndef LIZ_liz_common_H
#define LIZ_liz_common_H


#include <liz/liz_platform_types.h>
#include <liz/liz_platform_macros.h>


#if defined(__cplusplus)
extern "C" {
#endif

    /** 
     * Generic type to store identies in 32 bits.
     */
    typedef uint32_t liz_id_t;
    
    
    /**
     * Execution states are passed to a node on invokation as an argument and
     * are returned by the node on leaving it.
     *
     * launch-argument  - start a beahvior that isn't running.
     * launch-returned  - tick or cancel the behavior during the next update.
     * running-argument - behavior returned running during the previous update.
     * running-returned - tick or cancel during the next update.
     * success-argument - deferred action termination notification from another 
     *                    system.
     * success-returned - behavior completed successfully.
     * fail-argument    - deferred action termination notification from another 
     *                    system.
     * fail-returned    - behavior completed unsuccessfully.
     *
     * Cancel state is only used during cancellation handling.
     * cancel-argument  - behavior should cancel and clean up side effects.
     * cancel-returned  - must only be returned when called with cancel-
     *                    argument.
     */
    typedef enum liz_execution_state {
        liz_execution_state_launch = 0,
        liz_execution_state_running,
        liz_execution_state_success,
        liz_execution_state_fail,
        liz_execution_state_cancel
    } liz_execution_state_t;
    
    
    
    /**
     * Function interface for actions to call immediately during behavior tree
     * traversal (in contrast to persistent and deferred actions).
     *
     * execution_request can be:
     * liz_execution_state_launch  - behavior tree traversal reaches node anew.
     * liz_execution_state_running - behavior tree traversal ticks the node 
     *                               succeedingly.
     * liz_execution_state_cancel  - behavior tree traversal cancels the action 
     *                               which might to cleanup the blackboard.
     *
     *
     * The function can return:
     * liz_execution_state_running - tick the action again during the next 
     *                               update if it is reached or cancel it 
     *                               otherwise.
     * liz_execution_state_success - action execution termined successfully.
     * liz_execution_state_success - action execution terminated with a failure.
     *
     * 
     * @attention Never return a launch state!
     *
     * @attention Only return a cancel state if execution_request is cancel,
     *            otherwise behavior is undefined.
     *
     *
     * TODO: @todo Decide if actor_blackboard should be const.
     * TODO: @todo Decide if to dissallow continuing/running immediate actions.
     */
    typedef liz_execution_state_t (*liz_immediate_action_func_t)(void *actor_blackboard,
                                                                 liz_execution_state_t execution_request);
    
    
    /**
     * Item counts and buffer sizes needed to store actor data and process a
     * shape.
     */
    typedef  struct liz_shape_specification {
        uint16_t shape_atom_count;
        uint16_t immediate_action_function_count;
        uint16_t persistent_state_count;
        
        uint16_t decider_state_capacity;
        uint16_t action_state_capacity;
        
        uint16_t action_request_capacity;
        uint16_t decider_guard_capacity;
        
    } liz_shape_specification_t;
    
    
    
    /**
     * Types of action requests send to other game systems.
     */
    typedef enum liz_action_request_type {
        liz_action_request_type_launch = 0,
        liz_action_request_type_cancel,
        liz_action_request_type_remap_shape_item_index
        
    } liz_action_request_type_t;
    
    
    /**
     * Request to launch or cancel a deferred action or to remap a running
     * actions shape atom index.
     *
     * actor_id         - identifies the sender and reply destination to send 
     *                    execution state changes.
     * action_id        - identifies the action and the system managing it.
     * parameter        - can be used to target a special resource associated
     *                    with the action to launch or cancel in the system, or
     *                    specifies the shape atom index value that should 
     *                    replace the old one in a remap request.
     * shape_atom_index - address of the node to reply to with execution state 
     *                    changes.
     * type             - kind of request: launch an action, cancel it, or
     *                    remap the shape atom index.
     */
    typedef struct liz_action_request {
        liz_id_t actor_id;
        
        uint32_t action_id;
        uint16_t parameter;
        
        uint16_t shape_atom_index;
        
        uint8_t type;
    } liz_action_request_t;
    
    
    /**
     * Receives action requests emitted by the vm.
     */
    typedef struct liz_action_request_buffer {
        size_t capacity;
        size_t count;
        
        liz_action_request_t *requests;
    } liz_action_request_buffer_t;

    
    
    /**
     * Systems notify actors about running action state changes via
     * liz_action_state_update_t messages.
     */
    typedef struct liz_action_state_update {
        liz_id_t actor_id;
        uint16_t shape_atom_index;
        uint8_t state;
    } liz_action_state_update_t;
    
    
    /**
     * Sorts an array of action state updates in ascending order according to:
     * 1. actor_id
     * 2. shape_atom_index.
     */
    void
    liz_action_state_update_sort(liz_action_state_update_t *state_changes,
                                 liz_int_t count);
    
    
    
#if defined(__cplusplus)
} /* extern "C" */
#endif


#endif /* LIZ_liz_common_H */
