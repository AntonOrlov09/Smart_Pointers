#pragma once

#include <iostream>

template <typename T>
class shared_ptr {
private:
    T* ptr_; // сам сырой указатель
    int* count_; // счетчик ссылок. Скорее всего это указатель на int :)
public:
    typedef T element_type;
    explicit shared_ptr(T *ptr = nullptr) : ptr_(ptr), count_(new int{1}) {} // явный конструктор для оборачивания указателя

    shared_ptr(const shared_ptr &sharedPtr) : ptr_(sharedPtr.ptr_), count_(sharedPtr.count_) {
        (*count_)++;
    } // конструктор копирования, который инкрементирует счетчик ссылок  и сохраняет указатель


    shared_ptr& operator=(const shared_ptr& shared_ptr) {
        if (this != &shared_ptr) {
            if (count_ != nullptr) {
                (*count_)--;
                if (*count_ == 0) {
                    delete ptr_;
                    delete count_;
                }
            }
            ptr_ = shared_ptr.ptr_;
            count_ = shared_ptr.count_;
            (*count_)++;
        }
        return *this;
    }  // присваивание копированием



    T* get() const { return ptr_; } // метод get чтобы взять указатель

    T* operator->() const { return ptr_; } // оператор -> чтобы взять указатель.

    T& operator*() const { return *ptr_; } // оператор * разыменования указателя и получения ссылки на объект T

    // заменяет хранимый указатель на другой
    void reset(T *other = nullptr) {
        if (count_!=nullptr){
            (*count_)--;
            if (*count_ == 0) {
                delete ptr_;
                delete count_;
            }
        }
        count_= nullptr;
        if (other != nullptr) {
            count_ = new int{1};
        }
        ptr_ = other;
    }
    // это оператор, чтобы можно было писать if (sptr) { ... }.
    explicit operator bool() const { return ptr_ != nullptr; }

    ~shared_ptr() {
        (*count_)--;
        if (*count_ == 0) {
            delete ptr_;
            delete count_;
        }
        std::cout << "~shared_ptr" << std::endl;
    } // деструктор (уменьшает счетчик ссылок.
    //   Если видит, что счетчик ссылок == 0, то уничтожает данные и по самому указателю.
};