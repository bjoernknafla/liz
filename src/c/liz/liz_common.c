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

#include "liz_common.h"
#include "liz_common_internal.h"


#include "liz_platform_functions.h"
#include "liz_assert.h"
#include "liz_lookaside_stack.h"






liz_shape_specification_t
liz_shape_specification_merge_max(liz_shape_specification_t lhs,
                                  liz_shape_specification_t rhs)
{
    liz_shape_specification_t result;
    
    result.shape_atom_count = (uint16_t)liz_max_int(lhs.shape_atom_count, rhs.shape_atom_count);
    result.immediate_action_function_count = (uint16_t)liz_max_int(lhs.immediate_action_function_count, rhs.immediate_action_function_count);
    result.persistent_state_count = (uint16_t)liz_max_int(lhs.persistent_state_count, rhs.persistent_state_count);
    result.decider_guard_capacity = (uint16_t)liz_max_int(lhs.decider_guard_capacity, rhs.decider_guard_capacity);
    result.decider_state_capacity = (uint16_t)liz_max_int(lhs.decider_state_capacity, rhs.decider_state_capacity);
    result.action_state_capacity = (uint16_t)liz_max_int(lhs.action_state_capacity, rhs.action_state_capacity);
    result.action_request_capacity = (uint16_t)liz_max_int(lhs.action_request_capacity, rhs.action_request_capacity);
    
    return result;
}



void
liz_apply_persistent_state_changes(liz_persistent_state_t * LIZ_RESTRICT persistent_states,
                                   uint16_t *  LIZ_RESTRICT  persistent_state_shape_atom_indices,
                                   liz_int_t persistent_state_count,
                                   liz_persistent_state_t const * LIZ_RESTRICT persistent_state_changes,
                                   uint16_t const * LIZ_RESTRICT persistent_state_change_shape_atom_indices,
                                   liz_int_t persistent_state_change_count)
{
    liz_int_t apply_index = 0;
    
    for (liz_int_t ci = 0; i < persistent_state_change_count; ++i) {
        
        bool const found = liz_seek_key(&apply_index,
                                        persistent_state_change_shape_atom_indices[i],
                                        persistent_state_shape_atom_indices,
                                        persistent_state_count);
        (void)found;
        LIZ_ASSERT(found && "Indexed persistent state must exist.");
        
        persistent_states[apply_index] = persistent_state_changes[ci];
    }
}




/*
void
liz_sort_decider_states_for_indices(uint16_t * LIZ_RESTRICT destination_states,
                                    uint16_t * LIZ_RESTRICT destination_shape_atom_indices,
                                    uint16_t const * LIZ_RESTRICT unordered_states,
                                    uint16_t const * LIZ_RESTRICT unordered_shape_atom_indices,
                                    liz_int_t count)
{*/
    /* decider states are associated with the index of their shape atom -
     * all ordering and before- and after-relationships depend on this stored 
     * item index.
     *
     * decider states are emitted during traversal of a behavior tree.
     * The state of inner nodes is emitted after the states of it's 
     * children sub-trees because only then the inner node
     * knows which child has been the last to run and what execution state it
     * returned.
     *
     * As sequences might contain other sequences whose children are only
     * actions and as long as empty inner nodes (inner nodes with zero children) 
     * are tolerated, there will be item index sequences of increasing values,
     * decreasing values, and all different mixes of them.
     *
     * The following algorithm traverses the unordered states and shape atom
     * indices from their back to front. The beginning of the destination states
     * and shape atom indices is used as a helper stack to push values onto 
     * (states and indices whose atom index is smaller than that of their 
     * predecessor state).
     * State atom indices on the stack effectively grow from the beginning to
     * the top of the stack. The stack is popped the moment a smaller atom 
     * index is found in the unordered buffer. The popped element is then placed
     * at its final position in the target buffers.
     */
    /* 
    LIZ_ASSERT(0 <= count);
    
    // Traverse the arrays from back to front.
    liz_int_t dest_stack_pop_index = -1;
    liz_int_t dest_index = count - 1;
    liz_int_t src_index = count - 1;
    
    // Loop until the first element is in place in the destination arrays.
    while (src_index >= 0) {
        
        LIZ_ASSERT(dest_stack_pop_index < dest_index && "The stack at the start of the destination arrays must not overlap with the values entered from the back to their final location.");
        
        // Pop and store stack entries with larger shape atom indices than found
        // next in the entries to order.
        while ((dest_stack_pop_index >= 0) 
               && (destination_shape_atom_indices[dest_stack_pop_index] > unordered_shape_atom_indices[src_index])) {
            
            destination_states[dest_index] = destination_states[dest_stack_pop_index];
            destination_shape_atom_indices[dest_index] = destination_shape_atom_indices[dest_stack_pop_index];
            --dest_stack_pop_index;
            --dest_index;
        }
        

        if ((src_index > 0)
            && (unordered_shape_atom_indices[src_index - 1] > unordered_shape_atom_indices[src_index])) {
            
            ++dest_stack_pop_index;
            destination_states[dest_stack_pop_index] = unordered_states[src_index];
            destination_shape_atom_indices[dest_stack_pop_index]= unordered_shape_atom_indices[src_index];
            
        } else {
            
            LIZ_ASSERT(src_index >= 0);
            
            destination_states[dest_index] = unordered_states[src_index];
            destination_shape_atom_indices[dest_index] = unordered_shape_atom_indices[src_index];
            --dest_index;
        }
        
        --src_index;
    }
}
*/


void
liz_sort_values_for_keys_from_post_order_traversal(void * LIZ_RESTRICT values,
                                                   uint16_t * LIZ_RESTRICT keys,
                                                   size_t const value_size_in_bytes,
                                                   liz_int_t const key_value_count,
                                                   void * LIZ_RESTRICT value_reorder_stack,
                                                   uint16_t * LIZ_RESTRICT key_reorder_stack,
                                                   liz_int_t const stack_capacity)
{
    liz_lookaside_stack_t reorder_stack = liz_lookaside_stack_make(stack_capacity,
    
    liz_int_t kv_read_index = key_value_count - 1;
    liz_int_t kv_write_index = key_value_count - 1;
    liz_int_t reorder_stack_top_index = -1;
    
    
    while (0 < kv_read_index) {
        LIZ_ASSERT(kv_read_index <= kv_write_index && "Writing must not overtake reading.");
        LIZ_ASSERT(keys[kv_read_index] != key_reorder_stack[reorder_stack_top_index] && "Keys must be unique.")
        
        if (!liz_lookaside_stack_is_empty(&reorder_stack) 
            && keys[kv_read_index] < key_reorder_stack[reorder_stack_top_index]) {
            
            // Move the key and value from the stack in place.
            keys[kv_write_index] = key_reorder_stack[reorder_stack_top_index];
            liz_memcpy(values + value_size_in_bytes * kv_write_index,
                       value_reorder_stack + value_size_in_bytes * reorder_stack_top_index,
                       value_size_in_bytes);
            
            liz_lookaside_stack_pop(&reorder_stack);
            reorder_stack_top_index = liz_lookaside_stack_top_index(&reorder_stack);
            
            --kv_write_index;
        } else if (keys[kv_read_index - 1] < keys[kv_read_index]) {
            // Key and value are greater than their predecessor, so save the 
            // roundtrip to push and then pop them from the stack and move them
            // to their final position immediately.
            // This step also prevents pushing of leaves for which there might
            // be no space on the stack.
            keys[kv_write_index] = keys[kv_read_index];
            
            // read and write indices are equal for an already sorted list, 
            // therefore use memmove instead of memcpy.
            liz_memmove(values + value_size_in_bytes * kv_write_index,
                        values + value_size_in_bytes * kv_read_index,
                        value_size_in_bytes);
            
            --kv_read_index;
            --kv_write_index;
        } else {
            // Push the key and its value onto the stack.
            liz_lookaside_stack_push(&reorder_stack);
            reorder_stack_top_index = liz_lookaside_stack_top_index(&reorder_stack);
            
            key_reorder_stack[reorder_stack_top_index] = keys[kv_read_index];
            liz_memcpy(value_reorder_stack + value_size_in_bytes * reorder_stack_top_index, 
                       value_size_in_bytes * kv_read_index, 
                       value_size_in_bytes);
            
            --kv_read_index;
        }
    }

    // All keys and values read, write what's left on the stack back.
    LIZ_ASSERT(kv_write_index + 1 == liz_lookaside_stack_count(&reorder_stack));
    liz_memcpy(keys, 
               key_reorder_stack, 
               sizeof(keys[0]) * liz_lookaside_stack_count(&reorder_stack));
    liz_memcpy(values, 
               value_reorder_stack,
               value_size_in_bytes * liz_lookaside_stack_count(&reorder_stack));
}



bool
liz_seek_key(liz_int_t *cursor,
             uint16_t const key_to_find,
             uint16_t const *keys,
             uint16_t const key_count)
{
    for (liz_int_t i = *cursor; i < key_count; ++i) {
        
        if (key_to_find > keys[i]) {
            continue;
        } else if (key_to_find  < keys[i]) {
            *cursor = i;
            return false;
        } else {
            *cursor = i;
            return true;
        }
    }
    
    *cursor = key_count;
    
    return false;
}


