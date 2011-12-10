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

#include "liz_allocator.h"

#include "liz_platform_functions.h"
#include "liz_assert.h"


void*
liz_default_alloc(void *allocator_context, size_t requested_bytes)
{
    (void)allocator_context;
    
    return liz_malloc(requested_bytes);
}



void 
liz_default_dealloc(void * LIZ_RESTRICT allocator_context, void * LIZ_RESTRICT ptr)
{
    (void)allocator_context;
    liz_free(ptr);
}



size_t 
liz_allocation_size_aggregate(size_t const start_alignment, 
                              size_t const previous_size_in_bytes, 
                              size_t const alignment_for_addition,
                              size_t const min_additional_size_in_bytes)
{
    LIZ_ASSERT(0u < start_alignment && "Alignment must be a power of two.");
    LIZ_ASSERT(0u < alignment_for_addition && "Alignment must be a power of two.");
    LIZ_ASSERT(0u == (start_alignment & (start_alignment - 1u)) && "Alignment must be a power of two.");
    LIZ_ASSERT(0u == (alignment_for_addition & (alignment_for_addition - 1u)) && "Alignment must be a power of two.");
    
    size_t aggregated_size = previous_size_in_bytes;
    
    size_t const diff_to_start_alignment = aggregated_size % start_alignment;
    size_t const relative_end_alignment = start_alignment + diff_to_start_alignment;
    size_t const alignment_mismatch = relative_end_alignment % alignment_for_addition;
    size_t const preventive_alignment_correction_padding = alignment_mismatch ? alignment_for_addition - alignment_mismatch : 0u;
    
    aggregated_size += preventive_alignment_correction_padding + min_additional_size_in_bytes;
    
    return aggregated_size;
}



size_t
liz_allocation_alignment_offset(void const *memory,
                                size_t const required_alignment)
{
    LIZ_ASSERT(0u < required_alignment && "Alignment must be a power of two.");
    LIZ_ASSERT(0u == (required_alignment & (required_alignment - 1u)) && "Alignment must be a power of two.");
    
    size_t const unaligned_addr = (size_t)memory;
    size_t const alignment_mismatch = unaligned_addr % required_alignment;
    size_t const alignment_correction_offset = alignment_mismatch ? required_alignment - alignment_mismatch : 0u;
    
    return alignment_correction_offset;
}



