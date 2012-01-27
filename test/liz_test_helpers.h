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
 * Test helper functions and data types.
 *
 * All in global namespace so C++ name lookup finds functions and operators
 * with liz global C namespace data types as parameters.
 */

#ifndef LIZ_liz_test_helpers_H
#define LIZ_liz_test_helpers_H


#include <cstdlib>
#include <sstream>
#include <vector>
#include <functional>

#include <unittestpp.h>


#include <liz/liz_lookaside_stack.h>
#include <liz/liz_lookaside_double_stack.h>
#include <liz/liz_common.h>
#include <liz/liz_common_internal.h>
#include <liz/liz_vm.h>





bool
operator==(liz_lookaside_stack_t const& lhs, 
           liz_lookaside_stack_t const& rhs);


bool
operator==(liz_lookaside_double_stack_t const& lhs, 
           liz_lookaside_double_stack_t const& rhs);


bool
operator==(liz_vm_cancellation_range_t const& lhs, 
           liz_vm_cancellation_range_t const& rhs);


bool
operator==(liz_shape_specification_t const& lhs,
           liz_shape_specification_t const& rhs);


bool
operator==(liz_action_request_t const& lhs,
           liz_action_request_t const& rhs);


bool
operator==(liz_action_state_update_t const& lhs,
           liz_action_state_update_t const& rhs);


bool
operator==(liz_vm_action_request_t const& lhs,
           liz_vm_action_request_t const& rhs);


bool
operator==(liz_vm_decider_guard_t const& lhs,
           liz_vm_decider_guard_t const& rhs);


bool
operator==(liz_vm_monitor_t const& lhs,
           liz_vm_monitor_t const& rhs);


bool
persistent_state_array_equals(liz_persistent_state_t const* lhs,
                              uint16_t const* lhs_shape_atom_indices,
                              liz_int_t const lhs_count,
                              liz_persistent_state_t const* rhs,
                              uint16_t const* rhs_shape_atom_indices,
                              liz_int_t const rhs_count,
                              liz_shape_atom_t const* shape_atoms,
                              liz_int_t const shape_atom_count);

class liz_persistent_state_comparator {
public:
    
    // Does not take over ownership or memory management of data.
    void 
    set(liz_persistent_state_t const* values,
        uint16_t const* keys,
        liz_int_t const count,
        liz_shape_atom_t const* shape_atoms,
        liz_int_t const shape_atom_count);    
    
    bool 
    operator==(liz_persistent_state_comparator const& other) const;    
    
    liz_persistent_state_t const* values_;
    uint16_t const* keys_;
    liz_int_t count_;
    liz_shape_atom_t const* shape_atoms_;
    liz_int_t shape_atom_count_;
};



template<typename T>
bool
array_equals(T const* lhs,
             liz_int_t const lhs_count,
             T const* rhs,
             liz_int_t const rhs_count) 
{
    if (lhs_count != rhs_count) {
        return false;
    }
    
    for (liz_int_t i = 0; i < lhs_count; ++i) {
        if (! (lhs[i] == rhs[i])) {
            return false;
        }
    }
    
    return true;
}


// Does not compare the blackboards or blackboard addresses.

bool
liz_vm_actor_equals(liz_vm_actor const& lhs,
                    liz_vm_actor const& rhs,
                    uint16_t const* persistent_state_shape_atom_indices,
                    liz_int_t const persistent_state_count,
                    liz_shape_atom_t const* shape_atoms,
                    liz_int_t const shape_atom_count);


// Does not compare the blackboards or blackboard addresses.
class liz_vm_actor_comparator {
public:
    void 
    set(liz_vm_actor_t const* actor, 
        liz_vm_shape_t const* shape);    
    
    bool 
    operator==(liz_vm_actor_comparator const& other) const;
    
    
    liz_vm_actor_t const* actor_;
    liz_vm_shape_t const* shape_;
};



bool
liz_vm_equals(liz_vm_t const& lhs, 
              liz_vm_t const& rhs,
              liz_shape_atom_t const* shape_atoms,
              liz_int_t const shape_atom_count);


bool
liz_vm_extractable_state_equals(liz_vm_t const& lhs, 
                                liz_vm_t const& rhs,
                                liz_shape_atom_t const* shape_atoms,
                                liz_int_t const shape_atom_count);

class liz_vm_comparator {
public:
    
    // Does not take over ownership or memory management of data.
    void 
    set(liz_vm_t const* vm,
        liz_shape_atom_t const* shape_atoms,
        liz_int_t const shape_atom_count);    
    
    bool 
    operator==(liz_vm_comparator const& other) const;
    
    
    liz_vm_t const* vm_;
    liz_shape_atom_t const* shape_atoms_;
    liz_int_t shape_atom_count_;
};



class liz_vm_extractable_state_comparator {
public:
    
    // Does not take over ownership or memory management of data.
    void 
    set(liz_vm_t const* vm,
        liz_shape_atom_t const* shape_atoms,
        liz_int_t const shape_atom_count);    
    
    bool 
    operator==(liz_vm_extractable_state_comparator const& other) const;
    
    
    liz_vm_t const* vm_;
    liz_shape_atom_t const* shape_atoms_;
    liz_int_t shape_atom_count_;
};



struct liz_action_request_compare : std::binary_function<liz_action_request_t, liz_action_request_t, bool> {
    result_type operator()(first_argument_type const& lhs, 
                           second_argument_type const& rhs) const
    {
        if (lhs.type < rhs.type) {
            return true;
        }
        
        if (lhs.actor_id < rhs.actor_id) {
            return true;
        }
        
        if (lhs.shape_atom_index < rhs.shape_atom_index) {
            return true;
        }
        
        if (lhs.parameter < rhs.parameter) {
            return true;
        }
        
        return false;
    }
};



char const*
liz_action_request_type_to_c_str(liz_action_request_type_t const type);


char const*
liz_node_type_to_c_str(liz_node_type_t const type);


char const*
liz_execution_state_to_c_str(liz_execution_state_t const state);


char const*
liz_vm_cmd_to_c_str(liz_vm_cmd_t const cmd) ;


UnitTest::MemoryOutStream&
operator<<(UnitTest::MemoryOutStream& mos, liz_vm_monitor_node_flag_t const flag);


UnitTest::MemoryOutStream& 
operator<<(UnitTest::MemoryOutStream& mos, liz_lookaside_stack_t const& stack);


UnitTest::MemoryOutStream& 
operator<<(UnitTest::MemoryOutStream& mos, liz_lookaside_double_stack_t const& stack);


UnitTest::MemoryOutStream&
operator<<(UnitTest::MemoryOutStream& mos, liz_vm_cancellation_range_t const& cr);


UnitTest::MemoryOutStream& 
operator<<(UnitTest::MemoryOutStream& mos, liz_shape_specification_t const& spec);


UnitTest::MemoryOutStream& 
operator<<(UnitTest::MemoryOutStream& mos, liz_action_request_t const& req);


UnitTest::MemoryOutStream& 
operator<<(UnitTest::MemoryOutStream& mos, liz_action_state_update_t const& update);


UnitTest::MemoryOutStream& 
operator<<(UnitTest::MemoryOutStream& mos, liz_vm_action_request_t const& req);


UnitTest::MemoryOutStream& 
operator<<(UnitTest::MemoryOutStream& mos, liz_vm_decider_guard_t const& guard) ;


UnitTest::MemoryOutStream& 
operator<<(UnitTest::MemoryOutStream& mos, liz_vm_monitor_t const& monitor) ;


void
persistent_state_array_print(UnitTest::MemoryOutStream& mos, 
                             liz_persistent_state_t const* states,
                             uint16_t const* shape_atom_indices,
                             liz_int_t const count,
                             liz_shape_atom_t const* shape_atoms,
                             liz_int_t const shape_atom_count);



UnitTest::MemoryOutStream&
operator<<(UnitTest::MemoryOutStream& mos, liz_persistent_state_comparator const& persistent_state_comparator);


template<typename T>
void
array_print(UnitTest::MemoryOutStream& mos, 
            T const* values, 
            liz_int_t const count)
{
    mos << "[";
    
    char const* separator = "";
    
    for (liz_int_t i = 0; i < count; ++i) {
        mos << separator;
        mos << values[i];
        separator = ", ";
    }
    
    mos << "]";
}




UnitTest::MemoryOutStream&
operator<<(UnitTest::MemoryOutStream& mos, liz_actor_header_t const& actor_header);



void
liz_vm_actor_print(UnitTest::MemoryOutStream& mos,
                   liz_vm_actor_t const& actor,
                   uint16_t const* persistent_state_shape_atom_indices,
                   liz_int_t const persistent_state_count,
                   liz_shape_atom_t const* shape_atoms,
                   liz_int_t const shape_atom_count);



UnitTest::MemoryOutStream&
operator<<(UnitTest::MemoryOutStream& mos, 
           liz_vm_actor_comparator const& ac);


void
liz_vm_print(UnitTest::MemoryOutStream& mos,
             liz_vm_t const& vm,
             liz_shape_atom_t const* shape_atoms,
             liz_int_t const shape_atom_count);


UnitTest::MemoryOutStream&
operator<<(UnitTest::MemoryOutStream& mos, liz_vm_comparator const& vm_comparator);



UnitTest::MemoryOutStream&
operator<<(UnitTest::MemoryOutStream& mos, liz_vm_extractable_state_comparator const& vm_comparator);



class counting_allocator {
public:
    
    counting_allocator();    
    
    ~counting_allocator();    
    
    void* 
    alloc(size_t bytes);
    
    void 
    dealloc(void* ptr);
    
    bool 
    is_balanced() const ;    
    
private:
    counting_allocator(counting_allocator const&); // =0;
    counting_allocator& operator=(counting_allocator const&); // =0;
    
private:
    static std::size_t const feeble_validation_token = 42;
    
    std::size_t requested_alloc_count;
    std::size_t requested_dealloc_count;
};



void* 
counting_alloc(void *allocator, size_t bytes);

void 
counting_dealloc(void *allocator, void *ptr);


struct liz_vm_monitor_log_entry {
    liz_uint_t node_shape_atom_index;
    liz_uint_t traversal_mask;
    liz_vm_t const *vm;
    void const* actor_blackboard;
    liz_time_t time;
    liz_vm_actor_t const *actor;
    liz_vm_shape_t const *shape;
};



bool
operator==(liz_vm_monitor_log_entry const& lhs, 
           liz_vm_monitor_log_entry const& rhs);

UnitTest::MemoryOutStream& 
operator<<(UnitTest::MemoryOutStream& mos, 
           liz_vm_monitor_log_entry const& entry);



typedef std::vector<liz_vm_monitor_log_entry> liz_vm_monitor_log;




UnitTest::MemoryOutStream& 
operator<<(UnitTest::MemoryOutStream& mos, liz_vm_monitor_log const& log);


void monitor_test_func(uintptr_t user_data,
                       liz_uint_t const node_shape_atom_index,
                       liz_uint_t const traversal_mask,
                       liz_vm_t const *vm,
                       void const * LIZ_RESTRICT actor_blackboard,
                       liz_time_t const time,
                       liz_vm_actor_t const *actor,
                       liz_vm_shape_t const *shape);



liz_execution_state_t
immediate_action_func_identity0(void* actor_blackboard,
                                liz_random_number_seed_t* rnd_seed,
                                liz_time_t time,
                                liz_execution_state_t execution_request);


liz_execution_state_t
immediate_action_func_identity1(void* actor_blackboard,
                                liz_random_number_seed_t* rnd_seed,
                                liz_time_t time,
                                liz_execution_state_t execution_request);


liz_execution_state_t
immediate_action_func_running2(void* actor_blackboard,
                               liz_random_number_seed_t* rnd_seed,
                               liz_time_t time,
                               liz_execution_state_t execution_request);


liz_execution_state_t
immediate_action_func_success3(void* actor_blackboard,
                               liz_random_number_seed_t* rnd_seed,
                               liz_time_t time,
                               liz_execution_state_t execution_request);


liz_execution_state_t
immediate_action_func_fail4(void* actor_blackboard,
                            liz_random_number_seed_t* rnd_seed,
                            liz_time_t time,
                            liz_execution_state_t execution_request);


liz_execution_state_t
immediate_action_func_cancel5(void* actor_blackboard,
                              liz_random_number_seed_t* rnd_seed,
                              liz_time_t time,
                              liz_execution_state_t execution_request);


class liz_vm_test_fixture {
public:
    
    enum immediate_action_func_index {
        immediate_action_func_index_identity0 = 0,
        immediate_action_func_index_identity1,
        immediate_action_func_index_running2,
        immediate_action_func_index_success3,
        immediate_action_func_index_fail4,
        immediate_action_func_index_cancel5
    };
    
    enum target_select {
        target_select_expected_result = 1,
        target_select_proband = 2,
        target_select_both = 3
    };
    
    
    liz_vm_test_fixture();    
    
    virtual ~liz_vm_test_fixture();    
    
    void grow_shape_atom_stream(std::size_t const new_size);    
    
    void push_shape_immediate_action(uint16_t const immediate_action_function_index);
    
    
    void push_shape_deferred_action(uint32_t const action_id,
                                    uint16_t const resource_id);
    
    
    void push_shape_persistent_action();
    
    
    
    void push_shape_sequence_decider(uint16_t const sub_stream_end_offset);
    
    
    
    void push_shape_dynamic_priority_decider(uint16_t const sub_stream_end_offset);
    
    
    void push_shape_concurrent_decider(uint16_t const sub_stream_end_offset);
    
    
    
    void push_actor_action_state(target_select const target_vm,
                                 uint16_t const shape_atom_index,
                                 uint8_t const state);
    
    void push_actor_decider_state(target_select const target_vm,
                                  uint16_t const shape_atom_index,
                                  uint16_t const state);    
    
    void set_actor_persistent_state(target_select const target_vm,
                                    uint16_t const persistent_state_index,
                                    uint16_t const shape_atom_index,
                                    liz_execution_state_t const exec_state);
    
    
    // Create the expected result and the tested vm after establishing
    // the shape (and don't call the method again for the instance of 
    // cancel_test_fixture).
    void create_expected_result_and_proband_vms_for_shape();
    
    
    
    void push_vm_action_state(target_select const target_vm,
                              uint16_t const shape_atom_index,
                              uint8_t const state);
    
    
    void push_vm_action_launch_request(target_select const target_vm,
                                       uint32_t const action_id,
                                       uint16_t const resource_id,
                                       uint16_t const shape_atom_index);
    
    
    void push_vm_action_cancel_request(target_select const target_vm,
                                       uint32_t const action_id,
                                       uint16_t const resource_id,
                                       uint16_t const shape_atom_index);
    
    
    
    void push_vm_persistent_action_state_change(target_select const target_vm,
                                                uint16_t const shape_atom_index,
                                                liz_execution_state const exec_state);
    
    
    void push_vm_decider_state(target_select const target_vm,
                               uint16_t const shape_atom_index,
                               uint16_t const state);
    
    
    void push_monitor_log_entry(target_select const target_log,
                                uint16_t const shape_atom_index,
                                liz_uint_t const monitor_mask,
                                liz_time_t const time);
    
    
    
    counting_allocator allocator;
    
    liz_vm_shape_t shape;
    liz_vm_actor_t expected_result_actor;
    liz_vm_actor_t proband_actor;
    
    liz_vm_t* expected_result_vm;
    liz_vm_t* proband_vm;
    
    static int const shape_immediate_action_function_count = 6;
    
    std::vector<liz_shape_atom_t> shape_atoms;
    std::vector<liz_immediate_action_func_t> shape_immediate_action_functions;
    std::vector<uint16_t> shape_persistent_state_shape_atom_indices;
    
    liz_actor_header_t expected_result_actor_header;
    std::vector<liz_persistent_state_t> expected_result_actor_persistent_states;
    std::vector<uint16_t> expected_result_actor_decider_state_shape_atom_indices;
    std::vector<uint16_t> expected_result_actor_decider_states;
    std::vector<uint16_t> expected_result_actor_action_state_shape_atom_indices;
    std::vector<uint8_t> expected_result_actor_action_states;
    
    liz_actor_header_t proband_actor_header;
    std::vector<liz_persistent_state_t> proband_actor_persistent_states;
    std::vector<uint16_t> proband_actor_decider_state_shape_atom_indices;
    std::vector<uint16_t> proband_actor_decider_states;
    std::vector<uint16_t> proband_actor_action_state_shape_atom_indices;
    std::vector<uint8_t> proband_actor_action_states;
    
    liz_execution_state_t expected_result_blackboard[shape_immediate_action_function_count];
    liz_execution_state_t proband_blackboard[shape_immediate_action_function_count];
    
    
    liz_vm_actor_comparator expected_result_actor_comparator;
    liz_vm_actor_comparator proband_actor_comparator;
    
    liz_vm_comparator expected_result_vm_comparator;
    liz_vm_comparator proband_vm_comparator;
    
    liz_vm_extractable_state_comparator expected_result_vm_extractable_state_comparator;
    liz_vm_extractable_state_comparator proband_vm_extractable_state_comparator;
    
    liz_vm_monitor_log expected_result_monitor_log;
    liz_vm_monitor_log proband_monitor_log;
    
    liz_vm_monitor_t proband_monitor;
    liz_vm_monitor_t* monitor;
    
private:
    liz_vm_test_fixture(liz_vm_test_fixture const&); // = 0
    liz_vm_test_fixture& operator=(liz_vm_test_fixture const&); // = 0
};



#endif /* LIZ_liz_test_helpers_H */
