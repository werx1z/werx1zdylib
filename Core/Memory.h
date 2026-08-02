// Core/Memory.h
#pragma once
#include <mach/mach.h>
#include <mach/vm_map.h>
#include <dlfcn.h>
#include <cstdio>
#include <string>
#include <vector>

class Memory {
private:
    mach_port_t task;
    uintptr_t base_address;
    
    uintptr_t get_base_address() {
        Dl_info info;
        if (dladdr((void*)&get_base_address, &info)) {
            return (uintptr_t)info.dli_fbase;
        }
        return 0;
    }
    
public:
    Memory() {
        task = mach_task_self();
        base_address = get_base_address();
    }
    
    template<typename T>
    T read(uintptr_t address) {
        T value = 0;
        vm_size_t size = sizeof(T);
        mach_msg_type_number_t count = size;
        kern_return_t kr = vm_read_overwrite(task, address, size, (vm_address_t)&value, &count);
        if (kr != KERN_SUCCESS) {
            return 0;
        }
        return value;
    }
    
    template<typename T>
    void write(uintptr_t address, T value) {
        vm_size_t size = sizeof(T);
        vm_write(task, address, (vm_offset_t)&value, size);
    }
    
    uintptr_t read_ptr(uintptr_t address) {
        return read<uintptr_t>(address);
    }
    
    float read_float(uintptr_t address) {
        return read<float>(address);
    }
    
    int read_int(uintptr_t address) {
        return read<int>(address);
    }
    
    bool read_bool(uintptr_t address) {
        return read<bool>(address);
    }
    
    void read_string(uintptr_t address, char* buffer, size_t size) {
        vm_read_overwrite(task, address, size, (vm_address_t)buffer, (mach_msg_type_number_t*)&size);
    }
    
    void write_float(uintptr_t address, float value) {
        write<float>(address, value);
    }
    
    void write_int(uintptr_t address, int value) {
        write<int>(address, value);
    }
    
    uintptr_t get_base() { return base_address; }
};