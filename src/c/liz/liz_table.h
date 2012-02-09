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
 * Table of pre-allocated size using an index array and a data array to map
 * ids to fixed sized data slots while keeping data tightly packed to allow  
 * processing without the need to jumping randomly around.
 *
 * Data placed in a data slot needs to store its id to enable tight data
 * packing on removal of slots. Therefore the user needs to supply an offset
 * to the id field in a data slot on creation of the table.
 *
 * To add data:
 * 1. Call add and keep the id.
 * 2. Call lookup with the id to get a pointer to the data slot.
 * 3. Use the raw data slot memory to place data into it and store the id
 *    in the data field which offset has been specified in the table creation
 *    process.
 *
 * All functions assume that you know, e.g., by design or by checking before a 
 * call, that calling them is safe. This minimizes the need for error return 
 * codes and checking them. See the second link to the aricle by Niklas Frykholm
 * for a discussion in the comment section about this.
 *
 * @attention The data handling isn't type safe and by placing too much data
 *            at a data slot address the following data slot or whatever lies
 *            in memory after the table will be trashed - undefined behavior
 *            follows.
 *
 *
 * Based on article by Niklas Frykohlm from BitSquid.se:
 *     http://bitsquid.blogspot.com/2011/09/managing-decoupling-part-4-id-lookup.html 
 *     and 
 *     http://altdevblogaday.com/2011/09/23/managing-decoupling-part-4-the-id-lookup-table/ 
 *     and especially the comment by Joerg Seebohn.
 *
 * Another inspiration is the rooster idea of Insomniac Games as presented
 * by Terrance Cohen:
 *     http://www.insomniacgames.com/a-dynamic-component-architecture-for-high-performance-gameplay/
 *
 */

#ifndef LIZ_liz_table_H
#define LIZ_liz_table_H


#include <liz/liz_platform_types.h>
#include <liz/liz_allocator.h>
#include <liz/liz_common.h>



#if defined(__cplusplus)
extern "C" {
#endif
    
    
    
    
#define LIZ_TABLE_ID_TO_ROOSTER_INDEX_MASK 0xFFFFu
#define LIZ_TABLE_ROOSTER_ID_VERSION_INCREMENT 0x10000u
#define LIZ_TABLE_ROOSTER_SENTINEL_COUNT 1u
    
    /* Not advertising the seemingly possible 32bit max size as memory might not
     * be available when combinaing size requirements of rooster and data slots.
     */
#define LIZ_TABLE_CAPACITY_MAX (UINT16_MAX - LIZ_TABLE_ROOSTER_SENTINEL_COUNT)
    
    /* Alignment of first field of liz_table_t. */
#define LIZ_TABLE_ALIGNMENT sizeof(uint32_t)
#define LIZ_TABLE_ROOSTER_ALIGNMENT sizeof(liz_id_t)
#define LIZ_TABLE_DATA_IDS_ALIGNMENT sizeof(liz_id_t) 
    
    
    
    
    
    
    /**
     * Id lookup table for data slots - treat as opaque.
     *
     * On creation more memory then the size of the data structure is allocated
     * and the id-to-data, data-to-id, and data slot arrays are placed in memory
     * and their offset from the table's memory address is stored.
     *
     * Data placed in data slots is assumed to be copyable byte per byte without
     * changing its identity or becoming invalid.
     */
    typedef struct liz_table_s {
        
        /* Size calculation and table creation code assumes that the first
         * struct field is a uint32_t.
         */
        uint32_t count;
        uint32_t capacity;
        uint32_t data_slot_size;
        
        /* FIFO queue inside the rooster for unused ids to keep version wear off 
         * minimal.
         */
        uint32_t freelist_dequeue_index;
        uint32_t freelist_enqueue_index;
        
        /* Offsets store the offset between the address of the table and the
         * memory address of the specific data.
         *
         * TODO: @todo Decide if to use sized types here to enable platform-
         *             independent storage of the offset. As long as table data
         *             is stored right behind the table struct, and as long
         *             as the max capacity is the max 16 bit value (minus 
         *             sentinel count), and the rooster and id sizes are small,
         *             as long the data offset will fit easily into a 32bit 
         *             integer. Based on the slot size a greater sized integer 
         *             might be necessary to access the end of the data slots.
         *             However, use of the init function can place the data 
         *             buffers wherever. As long as the pure binary blob saving
         *             of liz data is only used for one platform, as long the 
         *             size of the table does not need to be platform- 
         *             independent.
         */
        liz_int_t rooster_offset;
        liz_int_t data_ids_offset;
        liz_int_t data_slots_offset;        
    } liz_table_t;
    
    
    
    /**
     * Index encoding a versioned id and an index that points to a data slot or,
     * if the table index is on the FIFO freelist, to the next table_index 
     * to use when adding new data to the table.
     */ 
    typedef struct liz_table_index {
        liz_id_t versioned_id;
        uint32_t indirection_index; /* Index to data or next rooster slot. */
    } liz_table_index_t;
    
    
    /**
     * slot_capacity is the number of slots for data.
     * data_slot_size is the sie of data per slot in units of bytes.
     * See liz_table_create for alignment and size requirements.
     *
     * slot_capacity must be equal to or less than liz_table_capacity_max(),
     * otherwise behavior is undefined.
     */
    size_t
    liz_table_memory_size_requirement(liz_int_t slot_capacity,
                                      liz_int_t data_slot_size,
                                      liz_int_t data_slot_alignment);
    
    
    /**
     * Maximum number of slots a table can hold if enough memory is available.
     */
    liz_int_t
    liz_table_capacity_max(void);
    
    /**
     * Allocates the required memory for a table with slot_capacity data slots
     * and the specified data_slot_alignment by calling alloc_func with 
     * allocator_context.
     *
     * slot_capacity must be equal to or less than liz_table_capacity_max(),
     * otherwise behavior is undefined.
     *
     * Takes ownership for the memory allocated but does not care for memory
     * that might be pointed to from user data placed in data slots.
     *
     * data_slot_alignment is in bytes and must be a power of 2. If lesser than 
     * 4 bytes it is set to 4.
     *
     * Data slot sizes need to be multiples of their alignment to stay aligned
     * when stored contiguously, e.g., one ofter the other, tightly packed.
     *
     * Data alignment and data size are in units of bytes.
     *
     * Returns NULL if slot_capacity exceeds liz_table_capacity_max, of if the 
     * data size is too large, or if not enough memory is allocatable at the 
     * moment.
     *
     * @attention Use the same or interchangeable allocator contexts for 
     *            creation and destruction.
     */
    liz_table_t*
    liz_table_create(liz_int_t slot_capacity,
                     liz_int_t data_slot_size,
                     liz_int_t data_slot_alignment,
                     void *allocator_context,
                     liz_alloc_func_t alloc_func);
    
    /**
     * Deallocates the memory of table without calling any finalization or 
     * destruction method on the data itself.
     *
     * Accepts NULL as a value for table.
     */
    void
    liz_table_destroy(liz_table_t *table,
                      void *allocator_context,
                      liz_dealloc_func_t dealloc_func);
    
    
    /**
     *
     * 
     *
     * slot_capacity must be equal to or less than liz_table_capacity_max(),
     * otherwise behavior is undefined.
     */
    void
    liz_table_init(liz_table_t *table, 
                   liz_int_t const slot_capacity,
                   liz_int_t const data_slot_size_in_bytes,
                   liz_table_index_t *rooster,
                   liz_id_t *data_ids,
                   void *data_slots);
    
    
    void
    liz_table_finalize(liz_table_t *table);
    
    
    /**
     * Returns BT_TRUE if the table has data slots which are all empty, 
     * otherwise returns BT_FALSE.
     *
     * A table with zero capacity always returns BT_FALSE.
     */
    /* Commented out, check the count and available capacity by yourself.
     bool
     liz_table_is_empty(liz_table_t const *table);
     */
    
    /**
     * Returns true if no empty data slots are available, otherwise returns
     * false.
     */
    bool
    liz_table_is_full(liz_table_t const *table);
    
    
    liz_int_t
    liz_table_count(liz_table_t const *table);
    
    liz_int_t
    liz_table_capacity(liz_table_t const *table);
    
    /**
     * Registers a data slot on table and returns the slot id.
     *
     * Lookup the data slot via the id to actually store data in it.
     * 
     * @attention Don't call when table is full, otherwise behavior is
     *            undefined.
     */
    liz_id_t
    liz_table_add(liz_table_t *table);
    
    /**
     * Unregisters the data slot associated with id and reshuffles data behind
     * the scene to keep it packed tightly.
     * 
     * The data slot previously associated with id mustn't be accessed directly.
     *
     * @attention Don't pass in an invalid, e.g., not contained, id, otherwise
     *            behavior is undefined.
     */
    void
    liz_table_remove(liz_table_t *table,
                     liz_id_t id);
    
    /**
     * Returns true of the table contains the id, otherwise returns false.
     */
    bool
    liz_table_contains(liz_table_t const *table,
                       liz_id_t id);
    
    /**
     * Returns a pointer to the data slot associated with id.
     *
     * The data slot pointer becomes invalid when calling remove or clear after
     * a lookup. Adding doesn't invalidate a lookup result.
     *
     * @attention Don't call with an id that isn't contained, otherwise behavior
     *            is undefined.
     */
    void*
    liz_table_lookup(liz_table_t *table,
                     liz_id_t id);
    
    /**
     * Returns a pointer to the indexed data slot.
     *
     * Data is packed tigthly and continuously to minimize cache trashing when 
     * processing it linearly.
     *
     * Use together with liz_table_data_id to get the id associated with
     * a data slot.
     *
     * index must be less than liz_table_count, otherwise behavior is undefined.
     *
     * @attention Removal of data changes data placement in slots. Interleaving
     *            removal and liz_table_data accesses will skip data slots.
     *            Run liz_table_data from the end of the table and also remove 
     *            from the end to be save.
     */
    void*
    liz_table_data(liz_table_t *table,
                   liz_int_t index);
    
    /**
     * Companion function for liz_table_data_begin
     */
    liz_id_t
    liz_table_data_id(liz_table_t const *table,
                      liz_int_t index);
    
    
    
#if defined(__cplusplus)
} /* extern "C" */
#endif


#endif /* LIZ_liz_table_H */
