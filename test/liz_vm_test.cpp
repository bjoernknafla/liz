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
 * Checks the vm.
 */

#include <unittestpp.h>

#include <liz/liz_vm.h>

SUITE(liz_vm_test)
{
    TEST(replace_an_empty_cancellation_range_with_another_empty_range) 
    {
        liz_vm_cancellation_range_t range = {0u, 0u};
        
        liz_vm_cancellation_range_adapt(&range, 7u, 7u);
        
        CHECK_EQUAL(7u, range.begin_index);
        CHECK_EQUAL(7u, range.end_index);
    }
    
    
    TEST(do_not_empty_cancellation_range)
    {
        liz_vm_cancellation_range_t range = {0u, 1u};
        
        liz_vm_cancellation_range_adapt(&range, 7u, 7u);
        
        CHECK_EQUAL(0u, range.begin_index);
        CHECK_EQUAL(1u, range.end_index);
    }
    
    TEST(grow_cancellation_range_to_the_left)
    {
        liz_vm_cancellation_range_t range = {3u, 5u};
        
        liz_vm_cancellation_range_adapt(&range, 4u, 7u);
        
        CHECK_EQUAL(3u, range.begin_index);
        CHECK_EQUAL(7u, range.end_index);
    }
    
    
    
    TEST(grow_cancellation_range_to_the_right)
    {
        liz_vm_cancellation_range_t range = {3u, 5u};
        
        liz_vm_cancellation_range_adapt(&range, 2u, 5u);
        
        CHECK_EQUAL(2u, range.begin_index);
        CHECK_EQUAL(5u, range.end_index);
    }
    
    
    TEST(grow_cancellation_range)
    {
        liz_vm_cancellation_range_t range = {3u, 5u};
        
        liz_vm_cancellation_range_adapt(&range, 1u, 7u);
        
        CHECK_EQUAL(1u, range.begin_index);
        CHECK_EQUAL(7u, range.end_index);
    }
    
    
    TEST(do_not_grow_cancellation_range) 
    {
        liz_vm_cancellation_range_t range = {3u, 4u};
        
        liz_vm_cancellation_range_adapt(&range, 3u, 3u);
        
        CHECK_EQUAL(3u, range.begin_index);
        CHECK_EQUAL(4u, range.end_index);
    }
}
