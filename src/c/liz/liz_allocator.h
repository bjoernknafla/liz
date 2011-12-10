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

#ifndef LIZ_liz_allocator_H
#define LIZ_liz_allocator_H


#include <liz/liz_platform_types.h>
#include <liz/liz_platform_macros.h>


#if defined(__cplusplus)
extern "C" {
#endif
    
    
    typedef void* (*liz_alloc_func_t)(void *context, size_t requested_bytes);
    
    typedef void (*liz_dealloc_func_t)(void * LIZ_RESTRICT context, 
                                       void * LIZ_RESTRICT ptr);
    
    
    
    /**
     * Ignores allocator_context and simply calls the platforms's malloc 
     * function.
     */
    void*
    liz_default_alloc(void * LIZ_RESTRICTallocator_context, 
                      size_t const requested_bytes);
    
    
    /**
     * Ignores allocator_context and simply calls the platform's free function.
     */
    void 
    liz_default_dealloc(void * LIZ_RESTRICT allocator_context, 
                        void * LIZ_RESTRICT ptr);
    
    
    /**
     * Function to a aggregate multiple allocation requests into a single 
     * request.
     * 
     * The comprehensive count of bytes to allocate is returned.
     *
     * Each aggregated request specifies its alignment and minimal size 
     * requirement. As padding might be necessary to align the request offset
     * inside allocated memory more than the requested minimal additional size
     * might be added to allow correction padding.
     *
     * Call with a min_additional_size_in_bytes of zero for a returned size
     * that enables contiguous storage of correctly aligned allocations.
     *
     * Alignment values must be a power of two, e.g., not zero, otherwise
     * behavior is undefined.
     */
    size_t 
    liz_allocation_size_aggregate(size_t start_alignment, 
                                  size_t previous_size_in_bytes, 
                                  size_t alignment_for_addition,
                                  size_t min_additional_size_in_bytes);
    
    /**
     * Returns the necessary offset from memory to fulfill the allocation 
     * requirement.
     */
    size_t
    liz_allocation_alignment_offset(void const *memory,
                                    size_t required_alignment);
    
    
    
#if defined(__cplusplus)
} /* extern "C" */
#endif


#endif /* BT_bt_allocator_H */
