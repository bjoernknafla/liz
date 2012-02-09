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
 *extern
 * Lookup table implementation.
 */

#include "liz_table.h"

#include "liz_assert.h"
#include "liz_allocator.h"



size_t
liz_table_memory_size_requirement(liz_int_t slot_capacity,
                                  liz_int_t data_slot_size,
                                  liz_int_t data_slot_alignment)
{
    LIZ_ASSERT((0 <= slot_capacity && slot_capacity <= liz_table_capacity_max()) && "slot_capacity must be positive and equal to or less than liz_table_capacity_max().");
    LIZ_ASSERT(0u == (data_slot_alignment & (data_slot_alignment - 1u)) && "Alignment must be a power of 2.");
    LIZ_ASSERT((0u == (data_slot_size % data_slot_alignment)) && "Data slot sizes need to be multiples of their alignment to stay aligned when stored contiguously.");
    
    size_t const table_alignment = sizeof(uint32_t);
    size_t const table_size = sizeof(liz_table_t);
    
    size_t const rooster_alignment = sizeof(liz_id_t);
    size_t const rooster_size = (slot_capacity + LIZ_TABLE_ROOSTER_SENTINEL_COUNT) * sizeof(liz_table_index_t);
    
    size_t const data_ids_alignment = sizeof(liz_id_t);
    size_t const data_ids_size = slot_capacity * sizeof(liz_id_t);
    
    size_t const data_alignment = data_slot_alignment;
    size_t const data_size = slot_capacity * data_slot_size;
    
    size_t result_size = table_size;
    result_size = liz_allocation_size_aggregate(table_alignment,
                                                result_size,
                                                rooster_alignment,
                                                rooster_size);
    
    result_size = liz_allocation_size_aggregate(table_alignment,
                                                result_size,
                                                data_ids_alignment,
                                                data_ids_size);
    
    result_size = liz_allocation_size_aggregate(table_alignment,
                                                result_size,
                                                data_alignment,
                                                data_size);
    
    /* Pad size so tables can be stored contiguously in memory and each instance
     * is aligned correctly.
     */
    result_size = liz_allocation_size_aggregate(table_alignment,
                                                result_size,
                                                table_alignment,
                                                0);
    
    return result_size;
}



liz_int_t
liz_table_capacity_max(void)
{
    return LIZ_TABLE_CAPACITY_MAX;
}



liz_table_t*
liz_table_create(liz_int_t const slot_capacity,
                 liz_int_t const data_slot_size,
                 liz_int_t data_slot_alignment,
                 void *allocator_context,
                 liz_alloc_func_t alloc_func)
{
    if (0 > slot_capacity 
        || LIZ_TABLE_CAPACITY_MAX < slot_capacity
        || 0 != data_slot_size % data_slot_alignment) {
        
        return NULL;
    }
    
    /* liz_table_validate_and_correct_data_alignment(&data_slot_alignment); */
    size_t const alloc_size = liz_table_memory_size_requirement(slot_capacity,
                                                                data_slot_size,
                                                                data_slot_alignment);
    
    liz_table_t *table = alloc_func(allocator_context,
                                    alloc_size);
    LIZ_ASSERT(0u == ((size_t)table & (LIZ_TABLE_ALIGNMENT - 1u)) && "Alignment of allocated memory less than required.");
    
    if (NULL != table) {
        
        liz_table_index_t *rooster = (liz_table_index_t *)(table + 1);
        LIZ_ASSERT(0 == ((uintptr_t)rooster) % LIZ_TABLE_ROOSTER_ALIGNMENT);
        
        liz_id_t *data_ids = (liz_id_t *)(rooster + slot_capacity + LIZ_TABLE_ROOSTER_SENTINEL_COUNT);
        LIZ_ASSERT(0 == ((uintptr_t)data_ids) % LIZ_TABLE_DATA_IDS_ALIGNMENT);
        
        char *unaligned_data_slots = (char *)(data_ids + slot_capacity);
        char *aligned_data_slots = unaligned_data_slots + liz_allocation_alignment_offset(unaligned_data_slots, data_slot_alignment);
        
        LIZ_ASSERT(0 == ((uintptr_t)aligned_data_slots) % data_slot_alignment);
        
        liz_table_init(table,
                       slot_capacity,
                       data_slot_size,
                       rooster,
                       data_ids,
                       aligned_data_slots);
    }
    
    return table;
}



void
liz_table_destroy(liz_table_t *table,
                  void *allocator_context,
                  liz_dealloc_func_t dealloc_func)
{
    if (NULL != table) {
        
        liz_table_finalize(table);
        dealloc_func(allocator_context, table);
    }
}



void
liz_table_init(liz_table_t *table, 
               liz_int_t const slot_capacity,
               liz_int_t const data_slot_size,
               liz_table_index_t *rooster,
               liz_id_t *data_ids,
               void *data_slots)
{    
    LIZ_ASSERT((0 <= slot_capacity && slot_capacity <= liz_table_capacity_max()) && "slot_capacity must be positive and equal to or less than liz_table_capacity_max().");
    
    table->count = 0u;
    table->capacity = (uint32_t)slot_capacity;
    table->data_slot_size = (uint32_t)data_slot_size;
    table->freelist_dequeue_index = 0u;
    /* Rooster has one slot more than capacity for a sentinel. */
    table->freelist_enqueue_index = (uint32_t)slot_capacity;
    table->rooster_offset = ((char *)rooster) - ((char *)table);
    table->data_ids_offset = ((char *)data_ids) - ((char *)table);
    table->data_slots_offset = ((char *)data_slots) - ((char *)table);
    
    /* Establish the FIFO freelist. */
    /* Rooster is one slot bigger than requested to store a sentinel. */
    for (liz_int_t i = 0; i  < slot_capacity + LIZ_TABLE_ROOSTER_SENTINEL_COUNT; ++i) {
        liz_table_index_t *rooster_slot = ((liz_table_index_t *)(((char *)table) + table->rooster_offset)) + i;
        
        rooster_slot->versioned_id = (uint32_t)i;
        rooster_slot->indirection_index = ((uint32_t)i + 1u);
    }
}



void
liz_table_finalize(liz_table_t *table)
{
    table->count = 0u;
    table->capacity = 0u;
    table->data_slot_size = 0u;
    table->freelist_dequeue_index = 0u;
    table->freelist_enqueue_index = 0u;
    table->rooster_offset = 0u;
    table->data_ids_offset = 0u;
    table->data_slots_offset = 0u;
}


/*
 bool
 liz_table_is_empty(liz_table_t const *table)
 {
 return 0u == table->count && 0u != table->capacity;
 }
 */


bool
liz_table_is_full(liz_table_t const *table)
{
    return table->count == table->capacity;
}



liz_int_t
liz_table_count(liz_table_t const *table)
{
    return table->count;
}



liz_int_t
liz_table_capacity(liz_table_t const *table)
{
    return table->capacity;
}



liz_id_t
liz_table_add(liz_table_t *table)
{
    LIZ_ASSERT(false == liz_table_is_full(table));
    
    uint32_t const table_count = table->count;
    liz_table_index_t *rooster_slot = ((liz_table_index_t *)(((char *)table) + table->rooster_offset)) + table->freelist_dequeue_index;
    liz_id_t *data_ids_slot = (liz_id_t *)(((char *)table) + table->data_ids_offset) + table_count;
    
    table->freelist_dequeue_index = rooster_slot->indirection_index;
    rooster_slot->indirection_index = table_count;
    *data_ids_slot = rooster_slot->versioned_id;
    
    table->count = table_count + 1u;
    
    return *data_ids_slot;
}



void
liz_table_remove(liz_table_t *table,
                 liz_id_t const id)
{
    LIZ_ASSERT(0u != liz_table_count(table));
    LIZ_ASSERT(true == liz_table_contains(table, id));
    
    
    liz_table_index_t *rooster = (liz_table_index_t *)(((char *)table) + table->rooster_offset);
    liz_id_t *data_ids = (liz_id_t *)(((char *)table) + table->data_ids_offset);
    
    liz_uint_t const rooster_index = id & LIZ_TABLE_ID_TO_ROOSTER_INDEX_MASK;
    liz_uint_t const data_slot_index = rooster[rooster_index].indirection_index;
    
    /* Update freelist and increase the versioning to invalidate the id. */
    rooster[table->freelist_enqueue_index].indirection_index = (uint32_t)rooster_index;
    table->freelist_enqueue_index = (uint32_t)rooster_index;
    
    /* Increment versioning and handle overflows. 
     liz_uint32_t const incremented_version = (id + BT_TABLE_ROOSTER_ID_VERSION_INCREMENT) & ~BT_TABLE_ID_TO_ROOSTER_INDEX_MASK;
     rooster[rooster_index].versioned_id = (liz_uint32_t)rooster_index + incremented_version;
     */
    
    rooster[rooster_index].versioned_id += LIZ_TABLE_ROOSTER_ID_VERSION_INCREMENT;
    
    
    /* Get last data_slot, its data_id, update the data_id and move the slot. */
    liz_uint_t const defrag_data_slot_index = --(table->count);
    
    liz_id_t const defrag_data_id = data_ids[defrag_data_slot_index];
    data_ids[data_slot_index] = defrag_data_id;
    rooster[defrag_data_id & LIZ_TABLE_ID_TO_ROOSTER_INDEX_MASK].indirection_index = (uint32_t)data_slot_index;
    
    liz_uint_t const data_slot_size = table->data_slot_size;
    char *data = ((char *)table) + table->data_slots_offset;
    char *data_slot = data + data_slot_index * data_slot_size;
    char *defrag_data_slot = data + defrag_data_slot_index * data_slot_size;
    
    /* Slots don't overlap or are identical if the last one is removed.
     * Unfortunately the behavior of memcpy is undefined in the last case...
     */
    liz_memmove(data_slot, defrag_data_slot, data_slot_size);
    
    /* Commented out - byte-wise copying might be too slow, hoping for 
     * optimizations in memmove...
     *
     * for (liz_uint_t i = 0; i < table->data_slot_size; ++i) {
     *     data_slot[i] = defrag_data_slot[i];
     * }
     */
}



bool
liz_table_contains(liz_table_t const *table,
                   liz_id_t const id)
{
    liz_uint_t const rooster_index = id & LIZ_TABLE_ID_TO_ROOSTER_INDEX_MASK;    
    liz_table_index_t *rooster = (liz_table_index_t *)(((char *)table) + table->rooster_offset);    
    
    return rooster_index <= table->capacity && id == rooster[rooster_index].versioned_id;
}



void*
liz_table_lookup(liz_table_t *table,
                 liz_id_t const id)
{
    LIZ_ASSERT(true == liz_table_contains(table, id));
    
    liz_table_index_t *rooster_slot = ((liz_table_index_t *)(((char *)table) + table->rooster_offset)) + (id & LIZ_TABLE_ID_TO_ROOSTER_INDEX_MASK);
    liz_uint_t const data_slot_index = rooster_slot->indirection_index;
    void *data_slot = ((char *)table) + table->data_slots_offset + data_slot_index * table->data_slot_size;
    
    return data_slot;
}



void*
liz_table_data(liz_table_t *table,
               liz_int_t const index)
{
    LIZ_ASSERT(index < table->count);
    
    return ((char *)table) + table->data_slots_offset + index * table->data_slot_size;
}



liz_id_t
liz_table_data_id(liz_table_t const *table,
                  liz_int_t const index)
{
    LIZ_ASSERT(index < table->count);
    
    liz_id_t const *data_ids = (liz_id_t *)(((char *)table) + table->data_ids_offset);
    
    return data_ids[index];
}




