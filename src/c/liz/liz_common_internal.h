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
 * Common types and helper functions used to implement liz.
 *
 * TODO: @todo Check and ensure that all uses of unions, e.g., 
 *             liz_persistent_state_t and liz_shape_atom_t initialize/set the 
 *             padding to zero to increase the possibility for comparisons
 *             without knowing the type of the union (which is not portable
 *             or standard conform to be true).
 */

#ifndef LIZ_liz_common_internal_H
#define LIZ_liz_common_internal_H


#include <liz/liz_common.h>


#if defined(__cplusplus)
extern "C" {
#endif
  
    
#define LIZ_SHAPE_ATOM_INDEX_ALIGNMENT sizeof(uint16_t)
#define LIZ_DECIDER_STATE_ALIGNMENT sizeof(uint16_t)
#define LIZ_ACTION_STATE_ALIGNMENT sizeof(uint8_t)
    
    
    
    /**
     * Behavior tree node types.
     */
    typedef enum liz_node_type {
        liz_node_type_immediate_action = 0,
        liz_node_type_deferred_action = 1,
        liz_node_type_persistent_action = 2,
        liz_node_type_action_max_id = 2,
        liz_node_type_sequence_decider,
        liz_node_type_dynamic_priority_decider,
        liz_node_type_concurrent_decider
    } liz_node_type_t;
    
    
    
#define LIZ_NODE_SHAPE_ATOM_COUNT_IMMEDIATE_ACTION 1
#define LIZ_NODE_SHAPE_ATOM_COUNT_IMMEDIATE_ACTION_TWO_CHILDREN 0   
#define LIZ_NODE_SHAPE_ATOM_COUNT_DEFERRED_ACTION 2
#define LIZ_NODE_SHAPE_ATOM_COUNT_DEFERRED_ACTION_TWO_CHILDREN 0
#define LIZ_NODE_SHAPE_ATOM_COUNT_PERSISTENT_ACTION 1
#define LIZ_NODE_SHAPE_ATOM_COUNT_PERSISTENT_ACTION_TWO_CHILDREN 0 
#define LIZ_NODE_SHAPE_ATOM_COUNT_SEQUENCE_DECIDER 1
#define LIZ_NODE_SHAPE_ATOM_COUNT_SEQUENCE_DECIDER_TWO_CHILDREN 0
#define LIZ_NODE_SHAPE_ATOM_COUNT_DYNAMIC_PRIORITY_DECIDER 1
#define LIZ_NODE_SHAPE_ATOM_COUNT_DYNAMIC_PRIORITY_DECIDER_TWO_CHILDREN 0
#define LIZ_NODE_SHAPE_ATOM_COUNT_CONCURRENT_DECIDER 1
#define LIZ_NODE_SHAPE_ATOM_COUNT_CONCURRENT_DECIDER_TWO_CHILDREN 0
#define LIZ_NODE_SHAPE_ATOM_COUNT_PROBABILITY_DECIDER 2
#define LIZ_NODE_SHAPE_ATOM_COUNT_PROBABILITY_DECIDER_TWO_CHILDREN 3
    
    
    /**
     * Liz flattens the static structure or shape of behavior trees via a 
     * pre-order traversal into a stream of liz_shape_atom elements.
     *
     * Due to the flattening a sub-behavior is represented by a sub-stream alas 
     * a contiguous range of shape atoms inside the shape stream.
     *
     * A behavior tree node might be represented by one or multiple shape atoms.
     *
     * Most nodes are represented by a fixed number of atoms but it is possible 
     * that the number of atoms needed to represent a node is dependent upon the
     * number of its sub-behaviors, e.g., a probability decider encodes its
     * type and the offset to its sub-stream end in one atom, the next atom 
     * hold the number of direct children and the offset to the first child, 
     * then the probability ranges is stored for each child, followed by 
     * atoms that hold the offsets for two children.
     */
    typedef union liz_shape_atom {
        uint32_t size_and_alignment_dummy;
        struct {
            uint8_t type;
            uint8_t padding_dummy;
            uint16_t content_dummy;
        } type_mask;
        struct {
            uint8_t type;
            uint8_t padding;
            uint16_t function_index;
        } immediate_action;
        struct {
            uint8_t type;
            uint8_t padding;
            uint16_t resource_id;
        } deferred_action_first; // Followed by a deferred_action_second atom.
        struct {
            uint32_t action_id;
        } deferred_action_second;
        struct {
            uint8_t type;
            char padding[3];
        } persistent_action;
        struct {
            uint8_t type;
            uint8_t padding;
            uint16_t end_offset;
        } sequence_decider;
        struct {
            uint8_t type;
            uint8_t padding;
            uint16_t end_offset;
        } dynamic_priority_decider;
        struct {
            uint8_t type;
            uint8_t padding;
            uint16_t end_offset;
        } concurrent_decider;
        struct {
            uint8_t type;
            uint8_t padding;
            uint16_t end_offset;
        } probability_decider_header_first; // Followed by probability_decider_header_second
        struct {
            uint16_t child_count;
            uint16_t first_child_end_offset;
        } probability_decider_header_second; // Followed by child_count x probability_decider_child_probability and (child_count / 2) x probability_decider_child_offsets
        struct {
            float probability;
        } probability_decider_child_probability;
        struct {
            uint16_t end_offset0;
            uint16_t end_offset1;
        } probability_decider_child_offsets;
        
    } liz_shape_atom_t;

    
    
    typedef struct liz_actor_clip {
        size_t memory_size;
        uintptr_t memory;
    } liz_actor_clip_t;
    
    
    
    typedef struct liz_actor_clip_header {
        uintptr_t user_data;
        size_t capacity;
        size_t count;
        size_t actor_size;
        liz_id_t clip_id;
        liz_id_t shape_id;
    } liz_actor_clip_header_t;

    
    
    /**
     *
     *
     * TODO: @todo Add random number seed when implementing the probability 
     *             decider.
     */
    typedef struct liz_actor_header {
        uintptr_t user_data;
        liz_int_t placeholder_for_random_number_seed;
        liz_id_t actor_id;
        uint16_t decider_state_count;
        uint16_t action_state_count;
    } liz_actor_header_t;
    
    

    // Assumed minimal alignment in bytes.
#define LIZ_PERSISTENT_STATE_ALIGNMENT 2
  
    
    
    typedef union liz_persistent_state {
        uint64_t size_and_alignment_dummy;
        struct {
            uint8_t state;
            char padding[7];
        } persistent_action;
    } liz_persistent_state_t;
    
    
    
    LIZ_INLINE static
    liz_int_t
    liz_min(liz_int_t const lhs, liz_int_t const rhs) 
    {
        return lhs < rhs ? lhs : rhs;
    }
    
    
    
    LIZ_INLINE static
    liz_int_t 
    liz_max(liz_int_t const lhs, liz_int_t const rhs) 
    {
        return lhs > rhs ? lhs : rhs;
    }    
    
    
    
    void
    liz_shape_atom_stream_add_immediate_action(liz_shape_atom_t *atoms,
                                               liz_int_t *index,
                                               liz_int_t capacity,
                                               uint16_t immediate_action_function_index);
    
    
    
    void
    liz_shape_atom_stream_add_deferred_action(liz_shape_atom_t *atoms,
                                              liz_int_t *index,
                                              liz_int_t capacity,
                                              uint32_t action_id,
                                              uint16_t resource_id);
    
    
    
    void
    liz_shape_atom_stream_add_persistent_action(liz_shape_atom_t *atoms,
                                                liz_int_t *index,
                                                liz_int_t capacity);
    
    
    
    void
    liz_shape_atom_stream_add_sequence_decider(liz_shape_atom_t *atoms,
                                               liz_int_t *index,
                                               liz_int_t capacity,
                                               uint16_t end_offset);
    
    
    
    void
    liz_shape_atom_stream_add_dynamic_priority_decider(liz_shape_atom_t *atoms,
                                                       liz_int_t *index,
                                                       liz_int_t capacity,
                                                       uint16_t end_offset);
    
    
    
    void
    liz_shape_atom_stream_add_concurrent_decider(liz_shape_atom_t *atoms,
                                                 liz_int_t *index,
                                                 liz_int_t capacity,
                                                 uint16_t end_offset);
    
    
    
    liz_shape_specification_t
    liz_shape_specification_merge_max(liz_shape_specification_t lhs,
                                      liz_shape_specification_t rhs);
    
    
    
    void
    liz_apply_persistent_state_changes(liz_persistent_state_t * LIZ_RESTRICT persistent_states,
                                       uint16_t const *  LIZ_RESTRICT  persistent_state_shape_atom_indices,
                                       liz_int_t persistent_state_count,
                                       liz_persistent_state_t const * LIZ_RESTRICT persistent_state_changes,
                                       uint16_t const * LIZ_RESTRICT persistent_state_change_shape_atom_indices,
                                       liz_int_t persistent_state_change_count);
    
    
    /**
     * Copies the entries of unordered states and unordered shape atom indices
     * sorted for atom index into the respective destination states and 
     * destination shape atom indices.
     *
     * All arrays must have at least count elements, otherwise behavior is
     * undefined.
     *
     * count must be greater or equal to @c 0, otherwise behavior is 
     * undefined.
     *
     * The arrays must not overlap, otherwise behavior is undefined.
     */
    /* Replaced by the more generic liz_sort_values_for_keys_from_post_order_traversal
    void
    liz_sort_decider_states_for_indices(uint16_t * LIZ_RESTRICT destination_states,
                                        uint16_t * LIZ_RESTRICT destination_shape_atom_indices,
                                        uint16_t const * LIZ_RESTRICT unordered_states,
                                        uint16_t const * LIZ_RESTRICT unordered_shape_atom_indices,
                                        liz_int_t count);
    */
    
    
    /**
     * Reorders keys and their associated values which are in a pre-order 
     * traversal order according to increasing keys.
     *
     * stack_capacity must be at least as large as the tree whose traversal
     * generated the key and value sequence.
     * value_reoder_stack must be correctly aligned to store values, otherwise
     * behavios is undefined.
     */
    /* Refactored to minimize errors when calculating stack addresses, renamed,
     * and moved to vm.h.
    void
    liz_sort_values_for_keys_from_post_order_traversal(void * LIZ_RESTRICT values,
                                                       uint16_t * LIZ_RESTRICT keys,
                                                       size_t const value_size_in_bytes,
                                                       liz_int_t const key_value_count,
                                                       void * LIZ_RESTRICT value_reorder_stack,
                                                       uint16_t * LIZ_RESTRICT key_reorder_stack,
                                                       liz_int_t const stack_capacity);
    */
    
    
    /**
     * Searches for key_to_find in keys, beginning from the index pointed to by 
     * cursor.
     *
     * Returns true of key_to_find is found, otherwise returns false.
     *
     * If key_to_find is found then the value pointed to by cursor represents the
     * index of the first match.
     *
     * If key_to_find isn't found then cursor points to an index of the first 
     * greater key or to key_count if no greater key than key_to_find exists.
     */
    bool
    liz_seek_key(liz_int_t *cursor,
                 uint16_t const key_to_find,
                 uint16_t const *keys,
                 uint16_t const key_count);
    
    
    
#if defined(__cplusplus)
} /* extern "C" */
#endif


#endif /* LIZ_liz_common_internal_H */
