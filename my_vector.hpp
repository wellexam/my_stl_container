#include <iterator>

namespace my_stl {
class vector_iterator_base;

template <typename my_vec>
class vector_iterator : public vector_iterator_base;

template <typename my_vec>
class vector_const_iterator : public vec_iterator_base;

template <typename T>
class vector_base;

template <typename T>
class vector : protected vector_base<T>;

template <typename T>
class vector_base {
public:
    using pointer = T *;

    class vector_impl {
        pointer M_start;
        pointer M_finish;
        pointer M_end_of_storage;

        vector_impl() : M_start(), M_finish(), M_end_of_storage() {}

        void M_swap_data(vector_impl &_x) {
            std::swap(M_start, _x.M_start);
            std::swap(M_finish, _x.M_finish);
            std::swap(M_end_of_storage, _x.M_end_of_storage);
        }
    };

public:
    vector_base() = default;

    vector_base(size_t _n) : _M_impl() { _M_create_storage(_n); }

    vector_base(vector_base &&_x) { this->M_impl.M_swap_data(_x.M_impl); }

    ~vector_base() _GLIBCXX_NOEXCEPT { _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start); }

public:
    vector_impl M_impl;

    pointer _M_allocate(size_t __n) {
        typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Tr;
        return __n != 0 ? _Tr::allocate(M_impl, __n) : pointer();
    }

    void _M_deallocate(pointer __p, size_t __n) {
        typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Tr;
        if (__p)
            _Tr::deallocate(M_impl, __p, __n);
    }

private:
    void _M_create_storage(size_t __n) {
        this->M_impl._M_start = this->_M_allocate(__n);
        this->M_impl._M_finish = this->M_impl._M_start;
        this->M_impl._M_end_of_storage = this->M_impl._M_start + __n;
    }
};
} // namespace my_stl