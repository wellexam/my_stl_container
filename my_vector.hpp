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

    vector_base(size_t _n) : M_impl() { M_create_storage(_n); }

    vector_base(vector_base &&_x) { this->M_impl.M_swap_data(_x.M_impl); }

    ~vector_base() noexcept { M_deallocate(M_impl.M_start, M_impl.M_end_of_storage - M_impl.M_start); }

public:
    vector_impl M_impl;

    pointer M_allocate(size_t _n);

    void M_deallocate(pointer _p, size_t _n);

private:
    void M_create_storage(size_t _n) {
        this->M_impl._M_start = this->M_allocate(_n);
        this->M_impl._M_finish = this->M_impl._M_start;
        this->M_impl._M_end_of_storage = this->M_impl._M_start + _n;
    }
};

template <typename T>
class vector : vector_base<T> {
    using _Base = vector_base<T>;

public:
    using self = vector;
    using value_type = T;
    using _Base::pointer;
    using const_poinnter = const T *;
    using reference = T &;
    using const_reference = const T &;
    using iterator = vector_iterator<self>;
    using const_iterator = vector_const_iterator<self>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

protected:
    using _Base::M_impl;
    using _Base::M_allocate;
    using _Base::M_deallocate;
};
} // namespace my_stl