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
 * Checks liz memory helper functions to:
 * - aggregate the size of data blobs with correct alignment, and
 * - calculate an offset from an unaligned memory address to an aligned address.
 */


#include <unittestpp.h>

#include <liz/liz_allocator.h>



SUITE(liz_allocator_test)
{
    TEST(default_dealloc_null)
    {
        // Nothing to check other than that no crash happens.
        
        void *default_allocator = NULL;
        liz_default_dealloc(default_allocator, NULL);
    }
    
    
    
    TEST(add_1_byte_alignment_to_1_byte_alignment)
    {
        size_t const start_alignment = 1;
        size_t const start_size = 3;
        size_t const added_alignment = 1;
        size_t const added_size = 2;
        size_t const expected_new_size = 5;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add_2_byte_alignment_to_1_byte_alignment)
    {
        size_t const start_alignment = 1;
        size_t const start_size = 3;
        size_t const added_alignment = 2;
        size_t const added_size = 2;
        size_t const expected_new_size = 6;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add_4_byte_alignment_to_1_byte_alignment)
    {
        size_t const start_alignment = 1;
        size_t const start_size = 3;
        size_t const added_alignment = 4;
        size_t const added_size = 8;
        size_t const expected_new_size = 14;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add_8_byte_alignment_to_1_byte_alignment)
    {
        size_t const start_alignment = 1;
        size_t const start_size = 3;
        size_t const added_alignment = 8;
        size_t const added_size = 0;
        size_t const expected_new_size = 10;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add_16_byte_alignment_to_1_byte_alignment)
    {
        size_t const start_alignment = 1;
        size_t const start_size = 3;
        size_t const added_alignment = 16;
        size_t const added_size = 32;
        size_t const expected_new_size = 50;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add1_byte_alignment_to_2_byte_alignment)
    {
        size_t const start_alignment = 2;
        size_t const start_size = 4;
        size_t const added_alignment = 1;
        size_t const added_size = 1;
        size_t const expected_new_size = 5;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add_2_byte_alignment_to_2_byte_alignment)
    {
        size_t const start_alignment = 2;
        size_t const start_size = 4;
        size_t const added_alignment = 2;
        size_t const added_size = 2;
        size_t const expected_new_size = 6;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add_4_byte_alignment_to_2_byte_alignment)
    {
        size_t const start_alignment = 2;
        size_t const start_size = 4;
        size_t const added_alignment = 4;
        size_t const added_size = 0;
        size_t const expected_new_size = 6;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add_8_byte_alignment_to_2_byte_alignment)
    {
        size_t const start_alignment = 2;
        size_t const start_size = 4;
        size_t const added_alignment = 8;
        size_t const added_size = 16;
        size_t const expected_new_size = 26;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add_16_byte_alignment_to_2_byte_alignment)
    {
        size_t const start_alignment = 2;
        size_t const start_size = 4;
        size_t const added_alignment = 16;
        size_t const added_size = 16;
        size_t const expected_new_size = 34;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add_1_byte_alignment_to_4_byte_alignment)
    {
        size_t const start_alignment = 4;
        size_t const start_size = 4;
        size_t const added_alignment = 1;
        size_t const added_size = 1;
        size_t const expected_new_size = 5;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    TEST(add_2_byte_alignment_to_4_byte_alignment)
    {
        size_t const start_alignment = 4;
        size_t const start_size = 4;
        size_t const added_alignment = 2;
        size_t const added_size = 4;
        size_t const expected_new_size = 8;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
        
    }
    
    
    
    TEST(add_4_byte_alignment_to_4_byte_alignment)
    {
        size_t const start_alignment = 4;
        size_t const start_size = 4;
        size_t const added_alignment = 4;
        size_t const added_size = 0;
        size_t const expected_new_size = 4;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
        
    }
    
    
    
    TEST(add_8_byte_alignment_to_4_byte_alignment)
    {
        size_t const start_alignment = 4;
        size_t const start_size = 4;
        size_t const added_alignment = 8;
        size_t const added_size = 16;
        size_t const expected_new_size = 24;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
        
    }
    
    
    
    TEST(add_16_byte_alignment_to_4_byte_alignment)
    {
        size_t const start_alignment = 4;
        size_t const start_size = 4;
        size_t const added_alignment = 16;
        size_t const added_size = 16;
        size_t const expected_new_size = 32;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
        
    }
    
    
    
    TEST(add_1_byte_alignment_to_8_byte_alignment)
    {
        size_t const start_alignment = 8;
        size_t const start_size = 16;
        size_t const added_alignment = 1;
        size_t const added_size = 1;
        size_t const expected_new_size = 17;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add_2_byte_alignment_to_8_byte_alignment)
    {
        size_t const start_alignment = 8;
        size_t const start_size = 16;
        size_t const added_alignment = 2;
        size_t const added_size = 0;
        size_t const expected_new_size = 16;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add_4_byte_alignment_to_8_byte_alignment)
    {
        size_t const start_alignment = 8;
        size_t const start_size = 16;
        size_t const added_alignment = 4;
        size_t const added_size = 8;
        size_t const expected_new_size = 24;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add_8_byte_alignment_to_8_byte_alignment)
    {
        size_t const start_alignment = 8;
        size_t const start_size = 16;
        size_t const added_alignment = 8;
        size_t const added_size = 32;
        size_t const expected_new_size = 48;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add_16_byte_alignment_to_8_byte_alignment)
    {
        size_t const start_alignment = 8;
        size_t const start_size = 16;
        size_t const added_alignment = 16;
        size_t const added_size = 16;
        size_t const expected_new_size = 40;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add_1_byte_alignment_to_16_byte_alignment)
    {
        size_t const start_alignment = 16;
        size_t const start_size = 16;
        size_t const added_alignment = 1;
        size_t const added_size = 1;
        size_t const expected_new_size = 17;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    TEST(add_2_byte_alignment_to_16_byte_alignment)
    {
        size_t const start_alignment = 16;
        size_t const start_size = 0;
        size_t const added_alignment = 2;
        size_t const added_size = 2;
        size_t const expected_new_size = 2;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    TEST(add_4_byte_alignment_to_16_byte_alignment)
    {
        size_t const start_alignment = 16;
        size_t const start_size = 16;
        size_t const added_alignment = 4;
        size_t const added_size = 0;
        size_t const expected_new_size = 16;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    TEST(add_8_byte_alignment_to_16_byte_alignment)
    {
        size_t const start_alignment = 16;
        size_t const start_size = 16;
        size_t const added_alignment = 8;
        size_t const added_size = 16;
        size_t const expected_new_size = 32;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(add_16_byte_alignment_to_16_byte_alignment)
    {
        size_t const start_alignment = 16;
        size_t const start_size = 16;
        size_t const added_alignment = 16;
        size_t const added_size = 48;
        size_t const expected_new_size = 64;
        
        size_t result = liz_allocation_size_aggregate(start_alignment,
                                                      start_size,
                                                      added_alignment,
                                                      added_size);
        
        CHECK_EQUAL(expected_new_size, result);
    }
    
    
    
    TEST(alignment_offset_for_1_byte)
    {
        size_t const start_addr = 1;
        void const *addr_ptr = (void const *)start_addr;
        
        size_t const alignment = 1;
        size_t const expected_offset = 0;
        
        size_t const result = liz_allocation_alignment_offset(addr_ptr, 
                                                              alignment);
        
        CHECK_EQUAL(expected_offset, result);
    }
    
    
    
    TEST(alignment_offset_for_2_bytes_needed)
    {
        size_t const start_addr = 1;
        void const *addr_ptr = (void const *)start_addr;
        
        size_t const alignment = 2;
        size_t const expected_offset = 1;
        
        size_t const result = liz_allocation_alignment_offset(addr_ptr, 
                                                              alignment);
        
        CHECK_EQUAL(expected_offset, result);
    }
    
    
    
    TEST(alignment_offset_for_2_bytes_unneeded)
    {
        size_t const start_addr = 4;
        void const *addr_ptr = (void const *)start_addr;
        
        size_t const alignment = 2;
        size_t const expected_offset = 0;
        
        size_t const result = liz_allocation_alignment_offset(addr_ptr, 
                                                              alignment);
        
        CHECK_EQUAL(expected_offset, result);
    }
    
    
    
    TEST(alignment_offset_for_4_bytes_needed)
    {
        size_t const start_addr = 3;
        void const *addr_ptr = (void const *)start_addr;
        
        size_t const alignment = 4;
        size_t const expected_offset = 1;
        
        size_t const result = liz_allocation_alignment_offset(addr_ptr, 
                                                              alignment);
        
        CHECK_EQUAL(expected_offset, result);
    }
    
    
    
    TEST(alignment_offset_for_4_bytes_unneeded)
    {
        size_t const start_addr = 8;
        void const *addr_ptr = (void const *)start_addr;
        
        size_t const alignment = 4;
        size_t const expected_offset = 0;
        
        size_t const result = liz_allocation_alignment_offset(addr_ptr, 
                                                              alignment);
        
        CHECK_EQUAL(expected_offset, result);
    }
    
    
    
    TEST(alignment_offset_for_8_bytes_needed)
    {
        size_t const start_addr = 4;
        void const *addr_ptr = (void const *)start_addr;
        
        size_t const alignment = 8;
        size_t const expected_offset = 4;
        
        size_t const result = liz_allocation_alignment_offset(addr_ptr, 
                                                              alignment);
        
        CHECK_EQUAL(expected_offset, result);
    }
    
    
    
    TEST(alignment_offset_for_8_bytes_unneeded)
    {
        size_t const start_addr = 8;
        void const *addr_ptr = (void const *)start_addr;
        
        size_t const alignment = 8;
        size_t const expected_offset = 0;
        
        size_t const result = liz_allocation_alignment_offset(addr_ptr, 
                                                              alignment);
        
        CHECK_EQUAL(expected_offset, result);
    }
    
    
    
    TEST(alignment_offset_for_16_bytes_needed)
    {
        size_t const start_addr = 8;
        void const *addr_ptr = (void const *)start_addr;
        
        size_t const alignment = 16;
        size_t const expected_offset = 8;
        
        size_t const result = liz_allocation_alignment_offset(addr_ptr, 
                                                              alignment);
        
        CHECK_EQUAL(expected_offset, result);
    }
    
    
    
    TEST(alignment_offset_for_16_bytes_unneeded)
    {
        size_t const start_addr = 32;
        void const *addr_ptr = (void const *)start_addr;
        
        size_t const alignment = 16;
        size_t const expected_offset = 0;
        
        size_t const result = liz_allocation_alignment_offset(addr_ptr, 
                                                              alignment);
        
        CHECK_EQUAL(expected_offset, result);
    }
    
} // SUITE(liz_allocator_test)
