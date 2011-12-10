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
 * Checks that the char type is 8 bit in size (1 byte) and that 
 * liz_int_t and liz_uint_t have the system's register size in bits and are
 * defined with the correct signedness.
 *
 *
 * TODO: @todo Transform this runtime tests into compile time tests.
 */

#include <unittestpp.h>

#include <liz/liz_platform_types.h>

#include <climits>



SUITE(liz_platform_types_test) 
{

    TEST(byte_sizes)
    {
        CHECK_EQUAL(std::size_t(8), sizeof(char) * std::size_t(CHAR_BIT));
        
        CHECK_EQUAL(sizeof(liz_int_t), sizeof(ptrdiff_t));
        CHECK_EQUAL(sizeof(liz_uint_t), sizeof(size_t));
        CHECK_EQUAL(sizeof(liz_int_t), sizeof(liz_uint_t));
    }
    
    
    
    TEST(signedness)
    {
        CHECK(liz_int_t(liz_int_t(0) - 1) < liz_int_t(0));
        CHECK(liz_uint_t(liz_uint_t(0) - 1) > liz_uint_t(0));
    }
    
} // SUITE(liz_platform_types_test)



