#pragma once

#include <iostream>

template <typename T>
class scoped_ptr {
private:
    T* ptr_;
public:
    typedef T element_type;
    explicit scoped_ptr(T* ptr = nullptr): ptr_(ptr) {}
    scoped_ptr(const scoped_ptr<T>& tmp) = delete;
    scoped_ptr<T>& operator = (const scoped_ptr<T>& tmp) = delete;
    scoped_ptr(scoped_ptr<T>&& tmp) = delete;
    scoped_ptr<T>& operator = (scoped_ptr<T>&& tmp) = delete;
    T* get() const{return ptr_;}
    T& operator*() const { return *ptr_;}
    T* operator -> () const { return ptr_;}
    void reset(T* ptr = nullptr) {
        delete ptr_;
        ptr_ = ptr;
    }
    T* release() {
        auto a = ptr_;
        ptr_ = nullptr;
        return a;
    }
    explicit operator bool() const {
        return ptr_ != nullptr;
    }
    ~scoped_ptr() {
        std::cout << "~scoped_ptr" << std::endl;

        delete ptr_;
    }
};

