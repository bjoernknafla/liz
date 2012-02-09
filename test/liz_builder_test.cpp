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
 * Test functionality to build a behavior tree at runtime in code.
 */
#include <unittestpp.h>

#include <cassert>

#include <liz/liz_common.h>
#include <liz/liz_common_internal.h>
#include <liz/liz_builder.h> 

#include "liz_test_helpers.h"

/*
typedef struct liz_shape {
    
} liz_shape_t;


typedef struct liz_actor {
    
} liz_actor_t;


typedef struct liz_actor_clip {
    
} liz_actor_clip_t;

typedef struct liz_work {
    
    
    
} liz_work_t;




SUITE(liz_builder_test)
{
    TEST(create_and_destroy)
    {
        counting_allocator allocator;
        
        liz_builder_t* builder = liz_builder_create(&allocator, 
                                                    counting_alloc,
                                                    counting_dealloc);
        
        liz_builder_destroy(builder,
                            &allocator,
                            counting_dealloc);
        
        CHECK(allocator.is_balanced());
    }
    
    
    TEST(create_and_destroy_with_allocator_arg_and_not_with_work_allocator)
    {
        counting_allocator builder_create_allocator;
        counting_allocator work_allocator;
        
        liz_builder_t* builder = liz_builder_create(&builder_create_allocator, 
                                                    counting_alloc,
                                                    counting_dealloc);
        
        liz_builder_set_scheme_allocator(builder,
                                         (liz_builder_allocator_t){
                                             &work_allocator,
                                             counting_alloc,
                                             counting_dealloc
                                         });
        
        liz_builder_destroy(builder,
                            &builder_create_allocator,
                            counting_dealloc);
        
        CHECK(builder_create_allocator.is_balanced());
        CHECK(work_allocator.is_balanced());
    }
    
    
    TEST(immediate_action_shape_spec)
    {
        liz_int_t const immediate_action_function_count = 1;
        
        liz_builder_begin_scheme(builder,
                                 max_immediate_action_function_index);
        {
            liz_builder_append_immediate_action(builder,
                                                immediate_action_function_count);
        }
        liz_builder_end_scheme(builder);
        
        liz_shape_specification_t const expected_spec = {
            1, // shape_atom_count
            immediate_action_function_count,
            0, // persistent_state_count
            0, // decider_state_capacity
            1, // action_state_capacity
            0, // persistent_state_change_capacity
            0, // decider_guard_capacity
            0  // action_request_capacity
        };
        
        liz_shape_specification_t const spec = liz_builder_shape_specification(builder);
        
        CHECK_EQUAL(expected_spec, spec);
    }
    
    
    TEST(immediate_action_shape)
    {
        
        
        liz_scheme_t *scheme = liz_scheme_create(liz_builder_shape_specification(builder),
                                                 &allocator,
                                                 counting_alloc);
        
        liz_scheme_destory(scheme,
                           &allocator,
                           dealloc_func);
        
        
        liz_builder_extract_shape(builder,
                                  &shape.spec,
                                  shape.atoms,
                                  shape_atom_capacity,
                                  shape.persistent_state_shape_atom_indices,
                                  persistent_state_count);
        
        CHECK_EQUAL(expected_shape, shape);
    }
    
}
*/
