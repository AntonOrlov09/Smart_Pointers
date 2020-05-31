#include <iostream>
#include "scoped_ptr.hpp"
#include <cassert>
#include "shared_ptr .hpp"

using namespace std;

// Тут простой тест с примером использования
struct foo {
    int field1;
    int field2;
};

void bar() {
    scoped_ptr<foo> foo_ptr(new foo {0, 111});
    cout << (*foo_ptr).field1 << " "
              << foo_ptr->field1 << " "
              << foo_ptr.get() << " "
              << endl;

    if (foo_ptr) foo_ptr->field1 += 1;

//    foo_ptr = std::move(foo_ptr); // - compilation fails
//    auto foo_ptr2(foo_ptr);// - compilation fails
//    foo_ptr = foo_ptr; // - compilation fails
//    bool status = foo_ptr; // - compilation fails
} // foo_ptr уничтожен. Оператор delete вызван автоматически.

static void test_scoped_ptr() {
    static_assert(
            !std::is_convertible<int *, scoped_ptr<int>>::value,
            "scoped ptr should not have implicit constructor from pointer!");

    static_assert(!std::is_copy_constructible<scoped_ptr<int>>::value,
                  "scoped ptr should not be copiable");

    static_assert(!std::is_move_constructible<scoped_ptr<int>>::value,
                  "scoped ptr should not be movable");

    static_assert(!std::is_copy_assignable<scoped_ptr<int>>::value,
                  "scoped ptr should not be copiable by operator=");

    static_assert(!std::is_move_assignable<scoped_ptr<int>>::value,
                  "scoped ptr should not be movable by operator=");

    static_assert(std::is_same<scoped_ptr<int>::element_type, int>::value,
                  "scoped ptr should contain element_type");

    static_assert(
            is_constructible<bool, scoped_ptr<int>>::value // explicit conversion
            && !is_convertible<scoped_ptr<int>,
                    bool>::value, // implicit conversion
            "scoped ptr should convertible to the bool, but not implicitly");

    {
        scoped_ptr<int> empty_ptr;

        assert(!empty_ptr);
        assert(empty_ptr.get() == nullptr);
    }

    {
        struct entity {
            int f1;
            std::string f2;
        };

        static_assert(std::is_same<scoped_ptr<entity>::element_type, entity>::value,
                      "scoped ptr should contain element_type");

        scoped_ptr<entity> const ptr{new entity{10, "hello"}};

        assert(ptr->f1 == 10);
        assert(ptr->f2 == "hello");

        assert((*ptr).f1 == 10);
        assert((*ptr).f2 == "hello");

        assert(ptr.get()->f1 == 10);
        assert(ptr.get()->f2 == "hello");
    }

    {
        scoped_ptr<int> ptr{new int{10}};
        assert(*ptr == 10);
        assert(ptr);

        ptr.reset();
        assert(ptr.get() == nullptr);
        assert(!ptr);

        ptr.reset(new int{20});
        assert(*ptr == 20);
    }

    {
        int *i = new int{10};
        scoped_ptr<int> ptr{i};

        assert(i == ptr.release());

        delete i;
    }
}

static void test_shared_ptr() {
    static_assert(
            !std::is_convertible<int *, shared_ptr<int>>::value,
            "shared ptr should not have implicit constructor from pointer!");

    static_assert(std::is_same<shared_ptr<int>::element_type, int>::value,
                  "shared ptr should contain element_type");

    static_assert(
            std::is_constructible<bool, shared_ptr<int>>::value // explicit conversion
                                                        && !std::is_convertible<shared_ptr<int>,
            bool>::value, // implicit conversion
            "shared ptr should convertible to the bool, but not implicitly");

    {
        shared_ptr<int> empty_ptr;

        assert(!empty_ptr);
        assert(empty_ptr.get() == nullptr);
    }

    {
        struct entity {
            int f1;
            std::string f2;
        };

        static_assert(std::is_same<shared_ptr<entity>::element_type, entity>::value,
                      "shared ptr should contain element_type");

        shared_ptr<entity> const ptr{new entity{10, "hello"}};

        assert(ptr->f1 == 10);
        assert(ptr->f2 == "hello");

        assert((*ptr).f1 == 10);
        assert((*ptr).f2 == "hello");

        assert(ptr.get()->f1 == 10);
        assert(ptr.get()->f2 == "hello");
    }

    {
        shared_ptr<int> ptr{new int{10}};
        assert(*ptr == 10);
        assert(ptr);

        ptr.reset();
        assert(ptr.get() == nullptr);
        assert(!ptr);

        ptr.reset(new int{20});
        assert(*ptr == 20);
    }

    {
        shared_ptr<int> ptr{new int{10}};

        ptr = static_cast<shared_ptr<int> const &>(ptr);

        {
            shared_ptr<int> other{ptr};

            assert(other.get() == ptr.get());
            assert(&(*other) == &(*ptr));
        }

        {
            shared_ptr<int> other;

            other = ptr;

            assert(other.get() == ptr.get());
            assert(&(*other) == &(*ptr));
        }
    }

    {
        shared_ptr<int> first{new int{10}};
        shared_ptr<int> second{first};

        first.reset();
        assert(first.get() != second.get());

        first = second;
        assert(first.get() == second.get());

        second.reset(new int{20});
        assert(first.get() != second.get());

        second = first;
        assert(first.get() == second.get());
    }
    {
        int trigger = 42;
        struct A {
            int *a_;
            explicit A(int *a) : a_(a) {}
            ~A() { *a_ = 0; }
        };

        {
            shared_ptr<A> first{new A{&trigger}};
            auto second = first;
            auto third = second;
            auto fourth = third;
            auto fifth = second;

            assert(first.get() == first.get());
            assert(*first->a_ == 42);
            assert(*fifth->a_ == 42);
        }
        assert(trigger == 0);
    }
}

void bar2() {
    shared_ptr<foo> foo_shptr(new foo {888, 999});
    shared_ptr<foo> foo_shptr2 = foo_shptr;
    assert(foo_shptr.get() == foo_shptr2.get());
    cout << foo_shptr->field1
              << " "
              << foo_shptr2->field1
              << endl;

// плохое использование
// приводи к двойному удалению, так как теперь два умных указателя
// "владеют" одним обхектом, не зная друг о друге

}
int main() {
    bar();
    test_scoped_ptr();
    bar2();
    test_shared_ptr();
    return 0;
}
