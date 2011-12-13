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
 */

#ifndef LIZ_liz_common_internal_H
#define LIZ_liz_common_internal_H


#include <liz/liz_common.h>


#if defined(__cplusplus)
extern "C" {
#endif
    
    
    typedef enum liz_node_type {
        liz_node_type_immediate_action = 0,
        liz_node_type_deferred_action,
        liz_node_type_persistent_action,
        liz_node_type_sequence_decider,
        liz_node_type_dynamic_priority_decider,
        liz_node_type_concurrent_decider
    } liz_node_type_t;
    
    
    
    typedef union liz_shape_atom {
        uint32_t size_and_alignment_dummy;
        struct {
            uint16_t dummy;
            uint8_t dymmy;
            uint8_t type;
        } type_mask;
        struct {
            uint16_t function_index;
            uint8_t padding;
            uint8_t type;
        } immediate_action;
        struct {
            uint16_t resource;
            uint8_t padding;
            uint8_t type;
        } deferred_action_first;
        struct {
            uint32_t action_id;
        } deferred_action_second;
        struct {
            char padding[3];
            uint8_t type;
        } persistent_action;
        struct {
            uint16_t end_index;
            uint8_t padding;
            uint8_t type;
        } common_decider; /* Used by sequence, priority, concurrent decider. */
        struct {
            uint16_t end_index;
            uint8_t padding;
            uint8_t type;
        } probability_decider_header_first;
        struct {
            uint16_t direct_child_count;
            uint16_t padding;
        } probability_decider_header_second;
        struct {
            float probability;
        } probability_decider_child_probability;
        struct {
            uint16_t shape_atom_index;
            uint16_t padding;
        } probability_decider_child_index;
        
    } liz_shape_atom_t;
    
    
    
    typedef union liz_persistent_state {
        
        uint64_t size_and_alignment_dummy;
        uint8_t persistent_action_state;
        
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
    
    
    
    liz_shape_specification_t
    liz_shape_specification_merge_max(liz_shape_specification_t lhs,
                                      liz_shape_specification_t rhs);
    
    
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
    void
    liz_decider_states_sort(uint16_t * LIZ_RESTRICT destination_states,
                            uint16_t * LIZ_RESTRICT destination_shape_atom_indices,
                            uint16_t const * LIZ_RESTRICT unordered_states,
                            uint16_t const * LIZ_RESTRICT unordered_shape_atom_indices,
                            liz_int_t count);
    
    
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
    liz_find_key(liz_int_t *cursor,
                 uint16_t const key_to_find,
                 uint16_t const *keys,
                 uint16_t const key_count);
    
    
    
#if defined(__cplusplus)
} /* extern "C" */
#endif


#endif /* LIZ_liz_common_internal_H */
