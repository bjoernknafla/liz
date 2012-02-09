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
 * Shallow wrappers around platform functions to ease finding, adapting, or
 * replacing.
 */

#ifndef LIZ_liz_platform_functions_H
#define LIZ_liz_platform_functions_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <liz/liz_platform_macros.h>



#if defined(__cplusplus)
extern "C" {
#endif

    
#define LIZ_ASSERT(expr) assert(expr)
    
    
    LIZ_INLINE static
    void*
    liz_malloc(size_t requested_bytes)
    {
        return malloc(requested_bytes);
    }
    
    
    LIZ_INLINE static
    void
    liz_free(void *ptr)
    {
        free(ptr);
    }
    
    
    LIZ_INLINE static
    void*
    liz_memmove(void *destination,
                void const *source,
                size_t bytes_to_move_count)
    {
        return memmove(destination, source, bytes_to_move_count);
    }
    
    
    LIZ_INLINE static
    void*
    liz_memcpy(void * LIZ_RESTRICT destination,
               void const * LIZ_RESTRICT source,
               size_t bytes_to_copy_count)
    {
        return memcpy(destination, source, bytes_to_copy_count);
    }
    
    
    LIZ_INLINE static
    void*
    liz_calloc(size_t element_count, 
               size_t element_size)
    {
        return calloc(element_count, element_size);
    }
    
    
    LIZ_INLINE static
    void*
    liz_memset(void *destination,
               int value_for_destination_bytes,
               size_t destination_byte_count)
    {
        return memset(destination, 
                      value_for_destination_bytes, 
                      destination_byte_count);
    }
    
    
    LIZ_INLINE static
    int
    liz_memcmp(void const *buffer0, 
               void const *buffer1,
               size_t buffer_byte_count)
    {
        return memcmp(buffer0, buffer1, buffer_byte_count);
    }
    
    
    
#if defined(__cplusplus)
} /* extern "C" */
#endif


#endif /* LIZ_liz_platform_functions_H */
