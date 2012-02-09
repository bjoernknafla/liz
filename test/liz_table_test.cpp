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
 *
 */


#include <unittestpp.h>


#include "liz_test_helpers.h"

#include <liz/liz_table.h>


SUITE(liz_table_test)
{
    /* Platform dependent, not really testable.
     TEST(precalculated_sizes_4_byte_data_size)
     {
     CHECK_EQUAL((size_t)32, liz_table_memory_size_requirement(0, 4, 4));
     CHECK_EQUAL((size_t)48, liz_table_memory_size_requirement(1, 4, 4));
     }
     
     
     
     TEST(precalculated_sizes_8_byte_data_size)
     {
     CHECK_EQUAL((size_t)32, liz_table_memory_size_requirement(0, 8, 4));
     CHECK_EQUAL((size_t)52, liz_table_memory_size_requirement(1, 8, 4));
     CHECK_EQUAL((size_t)56, liz_table_memory_size_requirement(1, 8, 8));
     }
     
     
     TEST(precalculated_sizes_16_byte_data_size)
     {
     CHECK_EQUAL((size_t)44, liz_table_memory_size_requirement(0, 16, 16));
     CHECK_EQUAL((size_t)72, liz_table_memory_size_requirement(1, 16, 16));
     CHECK_EQUAL((size_t)88, liz_table_memory_size_requirement(1, 32, 16));
     }
     */
    
    TEST(destroy_null_must_not_crash)
    {
        counting_allocator allocator;
        
        liz_table_destroy(NULL, &allocator, counting_dealloc);
        CHECK(true);
    }
    
    
    TEST(create_and_destroy)
    {
        
        counting_allocator allocator;
        
        
        liz_table_t *table = liz_table_create(1, 1, 1, &allocator, counting_alloc);
        CHECK(NULL != table);
        
        CHECK_EQUAL(0, liz_table_count(table));
        CHECK_EQUAL(1, liz_table_capacity(table));
        
        liz_table_destroy(table, &allocator, counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    TEST(create_and_detroy_empty_table)
    {
        
        counting_allocator allocator;
        
        
        liz_table_t *table = liz_table_create(0, 1, 1, &allocator, counting_alloc);
        CHECK(NULL != table);
        
        CHECK_EQUAL(0, liz_table_count(table));
        CHECK_EQUAL(0, liz_table_capacity(table));
        
        liz_table_destroy(table, &allocator, counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    TEST(try_to_create_table_with_too_much_capacity)
    {
        counting_allocator allocator;
        
        liz_uint_t const capacity = liz_table_capacity_max() + 1;
        liz_table_t *table = liz_table_create(capacity, 1, 1, &allocator, counting_alloc);
        CHECK(NULL == table);
        
        liz_table_destroy(table, &allocator, counting_dealloc);
        
        
        CHECK(allocator.is_balanced());
    }
    
    
    TEST(create_table_with_max_capacity)
    {
        counting_allocator allocator;
        
        liz_uint_t const capacity = liz_table_capacity_max();
        liz_table_t *table = liz_table_create(capacity, 1, 1, &allocator, counting_alloc);
        CHECK(NULL != table);
        
        liz_table_destroy(table, &allocator, counting_dealloc);
        
        
        CHECK(allocator.is_balanced());
    }
    
    
    TEST(add_one)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 2;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t id = liz_table_add(table);
        
        CHECK_EQUAL(true, liz_table_contains(table, id));
        CHECK_EQUAL(1, liz_table_count(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    TEST(add_one_to_table_with_capacity_of_one)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 1;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t id = liz_table_add(table);
        
        CHECK_EQUAL(true, liz_table_contains(table, id));
        CHECK_EQUAL(1, liz_table_count(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    TEST(add_then_remove)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 2;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t id = liz_table_add(table);
        
        liz_table_remove(table, id);
        
        CHECK_EQUAL(false, liz_table_contains(table, id));
        CHECK_EQUAL(0, liz_table_count(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    TEST(add_one_then_remove_with_table_with_capacity_of_one)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 1;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t id = liz_table_add(table);
        liz_table_remove(table, id);
        
        CHECK_EQUAL(false, liz_table_contains(table, id));
        CHECK_EQUAL(0, liz_table_count(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    TEST(add_remove_add)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 2;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        liz_table_remove(table, id0);
        liz_id_t const id1 = liz_table_add(table);
        
        CHECK_EQUAL(false, liz_table_contains(table, id0));
        CHECK_EQUAL(true, liz_table_contains(table, id1));
        CHECK_EQUAL(1, liz_table_count(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    
    TEST(add_remove_add_with_capacity_of_one)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 1;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        liz_table_remove(table, id0);
        liz_id_t const id1 = liz_table_add(table);
        
        CHECK_EQUAL(false, liz_table_contains(table, id0));
        CHECK_EQUAL(true, liz_table_contains(table, id1));
        CHECK_EQUAL(1, liz_table_count(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    
    TEST(add_add_remove_first)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 3;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        liz_id_t const id1 = liz_table_add(table);
        liz_table_remove(table, id0);
        
        
        CHECK_EQUAL(false, liz_table_contains(table, id0));
        CHECK_EQUAL(true, liz_table_contains(table, id1));
        CHECK_EQUAL(1, liz_table_count(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    TEST(add_add_remove_second)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 3;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        liz_id_t const id1 = liz_table_add(table);
        liz_table_remove(table, id1);
        
        
        CHECK_EQUAL(true, liz_table_contains(table, id0));
        CHECK_EQUAL(false, liz_table_contains(table, id1));
        CHECK_EQUAL(1, liz_table_count(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    
    TEST(add_add_remove_first_add)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 3;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        liz_id_t const id1 = liz_table_add(table);
        liz_table_remove(table, id0);
        liz_id_t const id2 = liz_table_add(table);
        
        
        CHECK_EQUAL(false, liz_table_contains(table, id0));
        CHECK_EQUAL(true, liz_table_contains(table, id1));
        CHECK_EQUAL(true, liz_table_contains(table, id2));
        CHECK_EQUAL(2, liz_table_count(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    TEST(add_add_remove_second_add)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 3;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        liz_id_t const id1 = liz_table_add(table);
        liz_table_remove(table, id1);
        liz_id_t const id2 = liz_table_add(table);
        
        
        CHECK_EQUAL(true, liz_table_contains(table, id0));
        CHECK_EQUAL(false, liz_table_contains(table, id1));
        CHECK_EQUAL(true, liz_table_contains(table, id2));
        CHECK_EQUAL(2, liz_table_count(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    
    
    
    TEST(add_add_remove_first_with_capacity_of_two)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 2;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        liz_id_t const id1 = liz_table_add(table);
        liz_table_remove(table, id0);
        
        
        CHECK_EQUAL(false, liz_table_contains(table, id0));
        CHECK_EQUAL(true, liz_table_contains(table, id1));
        CHECK_EQUAL(1, liz_table_count(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    TEST(add_add_remove_second_with_capacity_of_two)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 2;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        liz_id_t const id1 = liz_table_add(table);
        liz_table_remove(table, id1);
        
        
        CHECK_EQUAL(true, liz_table_contains(table, id0));
        CHECK_EQUAL(false, liz_table_contains(table, id1));
        CHECK_EQUAL(1, liz_table_count(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    
    TEST(add_add_remove_first_add_with_capacity_of_two)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 2;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        liz_id_t const id1 = liz_table_add(table);
        liz_table_remove(table, id0);
        liz_id_t const id2 = liz_table_add(table);
        
        
        CHECK_EQUAL(false, liz_table_contains(table, id0));
        CHECK_EQUAL(true, liz_table_contains(table, id1));
        CHECK_EQUAL(true, liz_table_contains(table, id2));
        CHECK_EQUAL(2, liz_table_count(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    TEST(add_add_remove_second_add_with_capacity_of_two)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 2;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        liz_id_t const id1 = liz_table_add(table);
        liz_table_remove(table, id1);
        liz_id_t const id2 = liz_table_add(table);
        
        
        CHECK_EQUAL(true, liz_table_contains(table, id0));
        CHECK_EQUAL(false, liz_table_contains(table, id1));
        CHECK_EQUAL(true, liz_table_contains(table, id2));
        CHECK_EQUAL(2, liz_table_count(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    TEST(empty_and_full_capacity_of_one_empty)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 1;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        
        // CHECK_EQUAL(true, liz_table_is_empty(table));
        CHECK_EQUAL(0u, liz_table_count(table));
        CHECK_EQUAL(false, liz_table_is_full(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        assert(allocator.is_balanced());
    }
    
    
    TEST(empty_and_full_capacity_of_one_full)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 1;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        (void)liz_table_add(table);
        
        // CHECK_EQUAL(false, liz_table_is_empty(table));
        CHECK_EQUAL(true, liz_table_is_full(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        assert(allocator.is_balanced());
    }
    
    
    
    TEST(empty_and_full_capacity_of_zero)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 0;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        
        // CHECK_EQUAL(false, liz_table_is_empty(table));
        CHECK_EQUAL(true, liz_table_is_full(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        assert(allocator.is_balanced());
    }
    
    
    TEST(empty_and_full_capacity_of_two_empty)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 2;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        
        // CHECK_EQUAL(true, liz_table_is_empty(table));
        CHECK_EQUAL(false, liz_table_is_full(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        assert(allocator.is_balanced());
    }
    
    
    TEST(empty_and_full_capacity_of_two_not_empty)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 2;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        (void)liz_table_add(table);
        
        // CHECK_EQUAL(false, liz_table_is_empty(table));
        CHECK_EQUAL(false, liz_table_is_full(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        assert(allocator.is_balanced());
    }
    
    
    TEST(empty_and_full_capacity_of_two_full)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 2;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        (void)liz_table_add(table);
        (void)liz_table_add(table);
        
        // CHECK_EQUAL(false, liz_table_is_empty(table));
        CHECK_EQUAL(true, liz_table_is_full(table));
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        assert(allocator.is_balanced());
    }
    
    
    TEST(add_lookup)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 3;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        
        data *lookup0 = (data *)liz_table_lookup(table, id0);
        lookup0->value = 42;
        data store0 = *lookup0;
        
        data data0 = *(data *)liz_table_data(table, 0);
        liz_id_t const data_id0 = liz_table_data_id(table, 0);
        
        CHECK_EQUAL(store0.value, data0.value);
        CHECK_EQUAL(id0, data_id0);
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        assert(allocator.is_balanced());
    }
    
    
    TEST(add_add_lookup_lookup)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 3;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        liz_id_t const id1 = liz_table_add(table);
        
        data *lookup0 = (data *)liz_table_lookup(table, id0);
        lookup0->value = 42;
        data store0 = *lookup0;
        
        data *lookup1 = (data *)liz_table_lookup(table, id1);
        lookup1->value = 23;
        data store1 = *lookup1;
        
        data data0 = *(data *)liz_table_data(table, 0);
        liz_id_t const data_id0 = liz_table_data_id(table, 0);
        
        data data1 = *(data *)liz_table_data(table, 1);
        liz_id_t const data_id1 = liz_table_data_id(table, 1);
        
        
        CHECK_EQUAL(store0.value, data0.value);
        CHECK_EQUAL(id0, data_id0);
        CHECK_EQUAL(store1.value, data1.value);
        CHECK_EQUAL(id1, data_id1);
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        assert(allocator.is_balanced());
    }
    
    
    
    TEST(add_add_remove_first_lookup_second)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 3;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        liz_id_t const id1 = liz_table_add(table);
        liz_table_remove(table, id0);
        
        data *lookup1 = (data *)liz_table_lookup(table, id1);
        lookup1->value = 23;
        data store1 = *lookup1;
        
        data data1 = *(data *)liz_table_data(table, 0);
        liz_id_t const data_id1 = liz_table_data_id(table, 0);
        
        CHECK_EQUAL(store1.value, data1.value);
        CHECK_EQUAL(id1, data_id1);
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        assert(allocator.is_balanced());
    }
    
    
    
    TEST(add_add_remove_second_lookup_first)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 3;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        liz_id_t const id1 = liz_table_add(table);
        liz_table_remove(table, id1);
        
        data *lookup0 = (data *)liz_table_lookup(table, id0);
        lookup0->value = 42;
        data store0 = *lookup0;
        
        data data0 = *(data *)liz_table_data(table, 0);
        liz_id_t const data_id0 = liz_table_data_id(table, 0);
        
        CHECK_EQUAL(store0.value, data0.value);
        CHECK_EQUAL(id0, data_id0);
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        assert(allocator.is_balanced());
    }
    
    
    
    TEST(add_add_lookup_remove_first)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 3;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        liz_id_t const id1 = liz_table_add(table);
        
        
        data *lookup1 = (data *)liz_table_lookup(table, id1);
        lookup1->value = 23;
        data store1 = *lookup1;
        
        liz_table_remove(table, id0);
        
        data data1 = *(data *)liz_table_data(table, 0);
        liz_id_t const data_id1 = liz_table_data_id(table, 0);
        
        
        CHECK_EQUAL(store1.value, data1.value);
        CHECK_EQUAL(id1, data_id1);
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        assert(allocator.is_balanced());
    }
    
    
    
    TEST(add_lookup_add_remove_second)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 3;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        
        data *lookup0 = (data *)liz_table_lookup(table, id0);
        lookup0->value = 42;
        data store0 = *lookup0;
        
        
        liz_id_t const id1 = liz_table_add(table);
        liz_table_remove(table, id1);
        
        
        data data0 = *(data *)liz_table_data(table, 0);
        liz_id_t const data_id0 = liz_table_data_id(table, 0);
        
        CHECK_EQUAL(store0.value, data0.value);
        CHECK_EQUAL(id0, data_id0);
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        assert(allocator.is_balanced());
    }
    
    
    
    
    
    TEST(add_add_lookup_remove_first_add_lookup)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 3;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        liz_id_t const id1 = liz_table_add(table);
        
        
        data *lookup1 = (data *)liz_table_lookup(table, id1);
        lookup1->value = 23;
        data store1 = *lookup1;
        
        liz_table_remove(table, id0);
        
        liz_id_t const id2 = liz_table_add(table);
        
        data *lookup2 = (data *)liz_table_lookup(table, id2);
        lookup2->value = 999;
        data store2 = *lookup2;
        
        
        data data1 = *(data *)liz_table_data(table, 0);
        liz_id_t const data_id1 = liz_table_data_id(table, 0);
        
        data data2 = *(data *)liz_table_data(table, 1);
        liz_id_t const data_id2 = liz_table_data_id(table, 1);
        
        
        CHECK_EQUAL(store1.value, data1.value);
        CHECK_EQUAL(id1, data_id1);
        CHECK_EQUAL(store2.value, data2.value);
        CHECK_EQUAL(id2, data_id2);
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        assert(allocator.is_balanced());
    }
    
    
    
    TEST(add_lookup_add_remove_second_add_lookup)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        liz_uint_t const capacity = 3;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data),
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        assert(NULL != table);
        
        liz_id_t const id0 = liz_table_add(table);
        
        data *lookup0 = (data *)liz_table_lookup(table, id0);
        lookup0->value = 42;
        data store0 = *lookup0;
        
        
        liz_id_t const id1 = liz_table_add(table);
        liz_table_remove(table, id1);
        
        liz_id_t const id2 = liz_table_add(table);
        data *lookup2 = (data *)liz_table_lookup(table, id2);
        lookup2->value = 999;
        data store2 = *lookup2;
        
        
        data data0 = *(data *)liz_table_data(table, 0);
        liz_id_t const data_id0 = liz_table_data_id(table, 0);
        
        data data2 = *(data *)liz_table_data(table, 1);
        liz_id_t const data_id2 = liz_table_data_id(table, 1);
        
        CHECK_EQUAL(store0.value, data0.value);
        CHECK_EQUAL(id0, data_id0);
        CHECK_EQUAL(store2.value, data2.value);
        CHECK_EQUAL(id2, data_id2);
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        assert(allocator.is_balanced());
    }
    
    
    TEST(id_version_increment_does_not_change_id_index)
    {
        counting_allocator allocator;
        
        struct data {
            int32_t value;
        };
        
        
        liz_uint_t const capacity = 1;
        liz_table_t *table = liz_table_create(capacity,
                                              sizeof(data), 
                                              sizeof(int32_t),
                                              &allocator,
                                              counting_alloc);
        
        liz_id_t id = liz_table_add(table);
        liz_table_remove(table, id);
        id = liz_table_add(table);
        
        liz_table_remove(table, id);
        id = liz_table_add(table);
        
        uint32_t version_mirror = LIZ_TABLE_ROOSTER_ID_VERSION_INCREMENT;
        
        while (0 != version_mirror) {
            /* Table uses a sentinel element so removal and adding alternates
             * between two index slots so both need to be incremented up to the
             * overflow.
             */
            for (int i = 0; i < 2; ++i) {
                liz_table_remove(table, id);
                id = liz_table_add(table);
            }
            version_mirror += LIZ_TABLE_ROOSTER_ID_VERSION_INCREMENT;
        }
        
        CHECK_EQUAL(0u, id);
        CHECK_EQUAL(0u, version_mirror);
        
        
        liz_table_destroy(table,
                          &allocator,
                          counting_dealloc);
        
        assert(allocator.is_balanced());
    }
    
    
    
} // SUITE(liz_table_test)
