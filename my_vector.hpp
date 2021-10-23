#include <iterator>
#include <cstdlib>
#include <stdexcept>
#include <type_traits>

namespace my_stl {
template <typename T>
class vector_iterator_base;

template <typename T>
class vector_iterator;

template <typename T>
class vector_const_iterator;

template <typename T>
class vector_base;

template <typename T>
class vector;

template <typename T>
class vector_iterator_base {
public:
    using self = vector_iterator_base<T>;
    using value_type = T;
    using const_pointer = const T *;
    using const_reference = const T &;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;

    friend class vector<T>;

protected:
    T *current = nullptr;

public:
    vector_iterator_base() = default;
    vector_iterator_base(const self &other) : current(other.current) {}
    vector_iterator_base(self &&other) noexcept : current(std::move(other.current)) {}
    explicit vector_iterator_base(T *&ptr) : current(ptr) {}
    explicit vector_iterator_base(T *&&ptr) : current(std::move(ptr)) {}

    virtual ~vector_iterator_base() = default;

    bool operator==(const self &other) const { return this->current == other.current; }
    bool operator==(const self &&other) const { return this->current == other.current; }
    template <typename iter>
    bool operator!=(iter &&other) const {
        return !operator==(std::forward<iter>(other));
    }

    vector_iterator_base &operator=(const self &other);
    vector_iterator_base &operator=(const self &&other) noexcept;

    const_reference operator*() const { return *current; }
    const_pointer operator->() const { return current; }
};
template <typename T>
vector_iterator_base<T> &vector_iterator_base<T>::operator=(const vector_iterator_base::self &other) {
    if (this == other) {
        return *this;
    }
    this->current = other.current;
    return *this;
}
template <typename T>
vector_iterator_base<T> &vector_iterator_base<T>::operator=(const vector_iterator_base::self &&other) noexcept {
    if (this == other) {
        return *this;
    }
    this->current = std::move(other.current);
    return *this;
}

template <typename T>
class vector_iterator : public vector_iterator_base<T> {
public:
    using pointer = T *;
    using reference = T &;
    using value_type = T;
    using self = vector_iterator;
    using const_pointer = const T *;
    using const_reference = const T &;
    using base = vector_iterator_base<T>;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;

    using vector_iterator_base<T>::current;

    //派生类构造函数
    vector_iterator() : base() {}
    explicit vector_iterator(pointer &ptr) : base(ptr) {}
    explicit vector_iterator(pointer &&ptr) : base(std::move(ptr)) {}
    vector_iterator(const self &other) : base(other) {}
    vector_iterator(self &&other) noexcept : base(std::move(other)) {}

    vector_iterator &operator=(const self &other) {
        base::operator=(other);
        return *this;
    }
    vector_iterator &operator=(const self &&other) noexcept {
        base::operator=(std::move(other));
        return *this;
    }
    vector_iterator &operator=(const vector_const_iterator<T> &other) {
        base::operator=(other);
        return *this;
    }
    vector_iterator &operator=(vector_const_iterator<T> &&other) {
        base::operator=(std::move(other));
        return *this;
    }

    reference operator*() { return *current; }
    pointer operator->() { return current; }

    self &operator++() {
        ++current;
        return *this;
    }

    self operator++(int) {
        auto temp = *this;
        ++current;
        return temp;
    }

    self &operator--() {
        --current;
        return *this;
    }

    self operator--(int) {
        auto temp = *this;
        --current;
        return temp;
    }

    reference &operator[](difference_type n) const noexcept { return current[n]; }

    self operator+(difference_type n) noexcept { return self(current + n); }

    self &operator+=(difference_type n) noexcept {
        current += n;
        return *this;
    }

    self operator-(difference_type n) noexcept { return self(current - n); }

    template <typename Iter>
    difference_type operator-(const Iter &&other) noexcept {
        return this->current - other.current;
    }

    self &operator-=(difference_type n) noexcept {
        current -= n;
        return *this;
    }
};

template <typename T>
class vector_const_iterator : public vector_iterator_base<T> {
public:
    using value_type = T;
    using pointer = const T *;
    using reference = const T &;
    using const_pointer = const T *;
    using const_reference = const T &;
    using self = vector_const_iterator;
    using base = vector_iterator_base<T>;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;

    //派生类构造函数
    vector_const_iterator() : base() {}
    explicit vector_const_iterator(T *&ptr) : base(ptr) {}
    explicit vector_const_iterator(T *&&ptr) : base(std::move(ptr)) {}
    vector_const_iterator(const self &other) : base(other) {}
    vector_const_iterator(self &&other) noexcept : base(std::move(other)) {}
    vector_const_iterator(const vector_iterator<T> &other) : base(other) {}
    vector_const_iterator(vector_iterator<T> &&other) noexcept : base(std::move(other)) {}

protected:
    using vector_iterator_base<T>::current;

public:
    vector_const_iterator &operator=(const self &other) {
        base::operator=(other);
        return *this;
    }
    vector_const_iterator &operator=(const self &&other) noexcept {
        base::operator=(std::move(other));
        return *this;
    }
    vector_const_iterator &operator=(const vector_iterator<T> &other) {
        base::operator=(other);
        return *this;
    }
    vector_const_iterator &operator=(vector_iterator<T> &&other) {
        base::operator=(std::move(other));
        return *this;
    }

    self &operator++() {
        ++current;
        return *this;
    }

    self operator++(int) {
        auto temp = *this;
        ++current;
        return temp;
    }

    self &operator--() {
        --current;
        return *this;
    }

    self operator--(int) {
        auto temp = *this;
        --current;
        return temp;
    }

    reference &operator[](difference_type n) const noexcept { return current[n]; }

    self operator+(difference_type n) noexcept { return self(current + n); }

    self &operator+=(difference_type n) noexcept {
        current += n;
        return *this;
    }

    //减号运算符重载实现，整数版本。
    template <typename tp>
    self sub_impl(tp n, std::true_type) const noexcept {
        return self(current - n);
    }

    //减号运算符重载实现，迭代器版本。
    template <typename Iter>
    difference_type sub_impl(Iter &&other, std::false_type) const noexcept {
        return this->current - other.current;
    }

    //减号运算符重载分发。
    template <typename tp>
    decltype(auto) operator-(tp &&n) const noexcept {
        return sub_impl(std::forward<tp>(n), std::is_integral<std::remove_reference_t<tp>>());
    }

    self &operator-=(difference_type n) noexcept {
        current -= n;
        return *this;
    }
};

template <typename T>
class vector_base {
public:
    using pointer = T *;

    class vector_impl {
    public:
        pointer M_start = nullptr;
        pointer M_finish = nullptr;
        pointer M_end_of_storage = nullptr;

        vector_impl() = default;

        void M_swap_data(vector_impl &_x) {
            std::swap(M_start, _x.M_start);
            std::swap(M_finish, _x.M_finish);
            std::swap(M_end_of_storage, _x.M_end_of_storage);
        }
    };

public:
    vector_base() : M_impl() {}

    explicit vector_base(size_t _n) : M_impl() { M_create_storage(_n); }

    vector_base(vector_base &&_x) noexcept { this->M_impl.M_swap_data(_x.M_impl); }

    virtual ~vector_base() noexcept { M_deallocate(M_impl.M_start, M_impl.M_end_of_storage - M_impl.M_start); }

public:
    vector_impl M_impl;

    pointer M_allocate(size_t _n);

    void M_deallocate(pointer _p, size_t _n);

private:
    void M_create_storage(size_t _n) {
        this->M_impl.M_start = this->M_allocate(_n);
        this->M_impl.M_finish = this->M_impl.M_start;
        this->M_impl.M_end_of_storage = this->M_impl.M_start + _n;
    }
};

template <typename T>
typename vector_base<T>::pointer vector_base<T>::M_allocate(size_t _n) {
    auto ptr = static_cast<pointer>(std::malloc(sizeof(T) * _n));
    return ptr;
}

template <typename T>
void vector_base<T>::M_deallocate(vector_base::pointer _p, size_t _n) {
    std::free(_p);
}

template <typename T>
class vector : vector_base<T> {
    using Base = vector_base<T>;

public:
    using self = vector;
    using value_type = T;
    using typename Base::pointer;
    using const_poinnter = const T *;
    using reference = T &;
    using const_reference = const T &;
    using iterator = vector_iterator<T>;
    using const_iterator = vector_const_iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

protected:
    using Base::M_impl;
    using Base::M_allocate;
    using Base::M_deallocate;

public:
    //构造函数
    vector() : Base() {}                     //默认构造函数。构造拥有默认构造的分配器的空容器。
    vector(size_type count, const T &value); //构造拥有 count 个有值 value 的元素的容器。
    explicit vector(size_type count);        //构造拥有个 count 默认插入的 T 实例的容器。不进行复制。
    template <typename InputIt>
    vector(InputIt first, InputIt last);                        //构造拥有范围 [first, last) 内容的容器。
    vector(const vector &other);                                //复制构造函数。构造拥有other内容的容器。
    vector(vector &&other) noexcept : Base(std::move(other)) {} //移动构造函数。用移动语义构造拥有other内容的容器。移动后，保证other为empty() 。
    vector(std::initializer_list<T> init);                      //构造拥有 initializer_list init 内容的容器。

    //析构函数
    ~vector(); //销毁vector。调用元素的析构函数，然后解分配所用的存储。注意，若元素是指针，则不销毁所指向的对象。

    //赋值运算符重载
    vector &operator=(const vector &other); //复制赋值运算符。以 other 的副本替换内容。
    vector &operator=(vector &&other) noexcept; //移动赋值运算符。用移动语义以 other 的内容替换内容（即从 other 移动 other 中的数据到此容器中）。之后 other 在合法但未指定的状态。
    vector &operator=(std::initializer_list<T> ilist); //以 initializer_list ilist 所标识者替换内容。

    //赋值函数
    void assign(size_type count, const T &value); //以 count 份 value 的副本替换内容。
    template <typename InputIt>                   //
    void assign(InputIt first, InputIt last);     //以范围 [first, last) 中元素的副本替换内容。若任一参数是指向 *this 中的迭代器则行为未定义。
    void assign(std::initializer_list<T> ilist);  //以来自 initializer_list ilist 的元素替换内容。

    //元素访问
    reference at(size_type pos);             //返回位于指定位置 pos 的元素的引用，有边界检查。若 pos 不在容器范围内，则抛出 std::out_of_range 类型的异常。
    const_reference at(size_type pos) const; //返回位于指定位置 pos 的元素的引用，有边界检查。若 pos 不在容器范围内，则抛出 std::out_of_range 类型的异常。

    reference operator[](size_type pos);             //返回位于指定位置 pos 的元素的引用。不进行边界检查。
    const_reference operator[](size_type pos) const; //返回位于指定位置 pos 的元素的引用。不进行边界检查。

    reference front();             //返回到容器首元素的引用。
    const_reference front() const; //返回到容器首元素的引用。

    reference back();             //返回到容器中最后一个元素的引用。
    const_reference back() const; //返回到容器中最后一个元素的引用。

    T *data() noexcept;             //返回指向作为元素存储工作的底层数组的指针。
    const T *data() const noexcept; //返回指向作为元素存储工作的底层数组的指针。

    //迭代器
    iterator begin() noexcept;              //返回指向 vector 首元素的迭代器。
    const_iterator begin() const noexcept;  //返回指向 vector 首元素的迭代器。
    const_iterator cbegin() const noexcept; //返回指向 vector 首元素的迭代器。

    iterator end() noexcept;              //返回指向 vector 末元素后一元素的迭代器。
    const_iterator end() const noexcept;  //返回指向 vector 末元素后一元素的迭代器。
    const_iterator cend() const noexcept; //返回指向 vector 末元素后一元素的迭代器。

    reverse_iterator rbegin() noexcept;              //返回指向逆向 vector 首元素的逆向迭代器。它对应非逆向 vector 的末元素。
    const_reverse_iterator rbegin() const noexcept;  //返回指向逆向 vector 首元素的逆向迭代器。它对应非逆向 vector 的末元素。
    const_reverse_iterator crbegin() const noexcept; //返回指向逆向 vector 首元素的逆向迭代器。它对应非逆向 vector 的末元素。

    reverse_iterator rend() noexcept; //返回指向逆向 vector 末元素后一元素的逆向迭代器。它对应非逆向 vector 首元素的前一元素。此元素表现为占位符，试图访问它导致未定义行为。
    const_reverse_iterator rend() const noexcept; //返回指向逆向 vector 末元素后一元素的逆向迭代器。它对应非逆向 vector 首元素的前一元素。此元素表现为占位符，试图访问它导致未定义行为。
    const_reverse_iterator crend() const noexcept; //返回指向逆向 vector 末元素后一元素的逆向迭代器。它对应非逆向 vector 首元素的前一元素。此元素表现为占位符，试图访问它导致未定义行为。

    //容量
    bool empty() const noexcept;         //检查容器是否无元素，即是否 begin() == end() 。
    size_type size() const noexcept;     //返回容器中的元素数，即 std::distance(begin(), end()) 。
    size_type max_size() const noexcept; //返回根据系统或库实现限制的容器可保有的元素最大数量，即对于最大容器的 std::distance(begin(), end()) 。
    size_type capacity() const noexcept; //返回容器当前已为之分配空间的元素数。
    void shrink_to_fit();                //请求移除未使用的容量。

    //修改器
    void clear() noexcept; //从容器擦除所有元素。此调用后 size() 返回零。

    iterator insert(const_iterator pos, const T &value);                  //在pos前插入value。
    iterator insert(const_iterator pos, T &&value);                       //在pos前插入value。
    iterator insert(const_iterator pos, size_type count, const T &value); //在pos前插入value的count个副本。
    template <typename InputIt>                                           //
    iterator insert(const_iterator pos, InputIt first, InputIt last);     //在pos前插入来自范围[first, last)的元素。
    iterator insert(const_iterator pos, std::initializer_list<T> ilist);  //在pos前插入来自ilist的元素。

    template <class... Args>
    iterator emplace(const_iterator pos, Args &&...args); //直接于pos前插入元素到容器中。

    iterator erase(const_iterator pos);                        //移除位于pos的元素。
    iterator erase(const_iterator first, const_iterator last); //移除范围[first; last)中的元素。

    void push_back(const T &value); //后附给定元素value到容器尾。
    void push_back(T &&value);      //移动value进新元素。

    template <class... Args>
    void emplace_back(Args &&...args); //添加新元素到容器尾。

    void pop_back(); //移除容器的末元素。

    void resize(size_type count);                          //重设容器大小以容纳count个元素。若当前大小小于count，则后附额外的默认插入的元素。
    void resize(size_type count, const value_type &value); //重设容器大小以容纳count个元素。若当前大小小于count，则后附额外的value的副本。

    void swap(vector &other); //将内容与 other 的交换。不在单独的元素上调用任何移动、复制或交换操作。所有迭代器和引用保持合法。尾后迭代器被非法化。
protected:
    /// Safety check used only from at().
    void M_range_check(size_type _n) const {
        if (_n >= this->size())
            throw std::out_of_range("Out of range.");
    }
};

template <typename T>
vector<T>::vector(size_type count, const T &value) : Base(count * 2) {
    while (count--) {
        new (M_impl.M_finish) T(value);
        ++M_impl.M_finish;
    }
}
template <typename T>
vector<T>::vector(vector::size_type count) : Base(count * 2) {
    new (M_impl.M_finish) T[count];
    M_impl.M_finish += count;
}

template <typename T>
template <typename InputIt>
vector<T>::vector(InputIt first, InputIt last) : Base((last - first) * 2) {
    while (first != last) {
        new (M_impl.M_finish) T(*first);
        ++M_impl.M_finish;
        ++first;
    }
}

template <typename T>
vector<T>::vector(const vector &other) : Base(other.size() * 2) {
    auto iter = other.cbegin();
    while (iter != other.cend()) {
        new (M_impl.M_finish) T(*iter);
        ++M_impl.M_finish;
        ++iter;
    }
}

template <typename T>
vector<T>::vector(std::initializer_list<T> init) : Base(init.size() * 2) {
    for (auto &i : init) {
        new (M_impl.M_finish) T(std::move(i));
        ++M_impl.M_finish;
    }
}
template <typename T>
vector<T>::~vector() {
    this->clear();
}

template <typename T>
vector<T> &vector<T>::operator=(const vector &other) {
    if (this->M_impl.M_finish == other.M_impl.M_finish) {
        return *this;
    }
    this->clear();
    if (this->capacity() < other.capacity()) {
        this->resize(other.capacity() * 2);
    }
    auto iter = other.cbegin();
    while (iter != other.cend()) {
        new (M_impl.M_finish) T(*iter);
        ++M_impl.M_finish;
        ++iter;
    }
    return *this;
}

template <typename T>
vector<T> &vector<T>::operator=(vector &&other) noexcept {
    if (this->M_impl.M_finish == other.M_impl.M_finish) {
        return *this;
    }
    vector temp;
    this->M_impl.M_swap_data(temp.M_impl);
    this->M_impl.M_swap_data(other.M_impl);
    return *this;
}
template <typename T>
vector<T> &vector<T>::operator=(std::initializer_list<T> ilist) {
    this->clear();
    if (ilist.size() > this->capacity()) {
        this->resize(ilist.size() * 2);
    }
    for (auto &i : ilist) {
        new (M_impl.M_finish) T(std::move(i));
        ++M_impl.M_finish;
    }
    return *this;
}

template <typename T>
void vector<T>::assign(size_type count, const T &value) {
    this->clear();
    if (this->capacity() < count) {
        this->resize(count * 2);
    }
    while (count--) {
        new (M_impl.M_finish) T(value);
        ++M_impl.M_finish;
    }
}

template <typename T>
template <typename InputIt>
void vector<T>::assign(InputIt first, InputIt last) {
    this->clear();
    auto count = last - first;
    if (this->capacity() < count) {
        this->resize(count * 2);
    }
    while (first != last) {
        new (M_impl.M_finish) T(*first);
        ++M_impl.M_finish;
        ++first;
    }
}
template <typename T>
void vector<T>::assign(std::initializer_list<T> ilist) {
    this->clear();
    if (ilist.size() > this->capacity()) {
        this->resize(ilist.size() * 2);
    }
    for (auto &i : ilist) {
        new (M_impl.M_finish) T(std::move(i));
        ++M_impl.M_finish;
    }
}

template <typename T>
typename vector<T>::reference vector<T>::at(vector::size_type pos) {
    this->M_range_check(pos);
    return (*this)[pos];
}

template <typename T>
typename vector<T>::const_reference vector<T>::at(vector::size_type pos) const {
    this->M_range_check(pos);
    return (*this)[pos];
}

template <typename T>
typename vector<T>::reference vector<T>::operator[](vector::size_type pos) {
    return *(this->M_impl.M_start + pos);
}

template <typename T>
typename vector<T>::const_reference vector<T>::operator[](vector::size_type pos) const {
    return *(this->M_impl.M_start + pos);
}

template <typename T>
typename vector<T>::reference vector<T>::front() {
    return *begin();
}

template <typename T>
typename vector<T>::const_reference vector<T>::front() const {
    return *begin();
}

template <typename T>
typename vector<T>::reference vector<T>::back() {
    return *(end() - 1);
}

template <typename T>
typename vector<T>::const_reference vector<T>::back() const {
    return *(end() - 1);
}

template <typename T>
T *vector<T>::data() noexcept {
    return this->_M_impl._M_start;
}

template <typename T>
const T *vector<T>::data() const noexcept {
    return this->_M_impl._M_start;
}

template <typename T>
void vector<T>::clear() noexcept {
    while (M_impl.M_finish != M_impl.M_start) {
        --M_impl.M_finish;
        M_impl.M_finish->~T();
    }
}

template <typename T>
typename vector<T>::iterator vector<T>::begin() noexcept {
    return iterator(const_cast<T *>(M_impl.M_start));
}

template <typename T>
typename vector<T>::const_iterator vector<T>::begin() const noexcept {
    return const_iterator(const_cast<T *>(M_impl.M_start));
}

template <typename T>
typename vector<T>::const_iterator vector<T>::cbegin() const noexcept {
    return const_iterator(const_cast<T *>(M_impl.M_start));
}

template <typename T>
typename vector<T>::iterator vector<T>::end() noexcept {
    return iterator(const_cast<T *>(M_impl.M_finish));
}

template <typename T>
typename vector<T>::const_iterator vector<T>::end() const noexcept {
    return const_iterator(const_cast<T *>(M_impl.M_finish));
}
template <typename T>
typename vector<T>::const_iterator vector<T>::cend() const noexcept {
    return const_iterator(const_cast<T *>(M_impl.M_finish));
}
template <typename T>
typename vector<T>::reverse_iterator vector<T>::rbegin() noexcept {
    return std::reverse_iterator<iterator>(end());
}

template <typename T>
typename vector<T>::const_reverse_iterator vector<T>::rbegin() const noexcept {
    return std::reverse_iterator<const_iterator>(cend());
}

template <typename T>
typename vector<T>::const_reverse_iterator vector<T>::crbegin() const noexcept {
    return std::reverse_iterator<const_iterator>(cend());
}

template <typename T>
typename vector<T>::reverse_iterator vector<T>::rend() noexcept {
    return std::reverse_iterator<iterator>(begin());
}

template <typename T>
typename vector<T>::const_reverse_iterator vector<T>::rend() const noexcept {
    return std::reverse_iterator<const_iterator>(cbegin());
}

template <typename T>
typename vector<T>::const_reverse_iterator vector<T>::crend() const noexcept {
    return std::reverse_iterator<const_iterator>(cbegin());
}

template <typename T>
bool vector<T>::empty() const noexcept {
    return M_impl.M_finish == M_impl.M_start;
}

template <typename T>
typename vector<T>::size_type vector<T>::size() const noexcept {
    return M_impl.M_finish - M_impl.M_start;
}

template <typename T>
typename vector<T>::size_type vector<T>::capacity() const noexcept {
    return M_impl.M_end_of_storage - M_impl.M_start;
}

template <typename T>
void vector<T>::shrink_to_fit() {
    auto size = this->size();
    if (size == capacity()) {
        return;
    }
    M_impl.M_start = static_cast<pointer>(realloc(M_impl.M_start, size));
    M_impl.M_finish = M_impl.M_start + size;
    M_impl.M_end_of_storage = M_impl.M_finish;
}

template <typename T>
void vector<T>::resize(vector::size_type count) {
    auto size = this->size();
    if (size == capacity()) {
        return;
    }
    M_impl.M_start = static_cast<pointer>(realloc(M_impl.M_start, count));
    M_impl.M_finish = M_impl.M_start + size;
    M_impl.M_end_of_storage = M_impl.M_start + count;
    while (M_impl.M_finish != M_impl.M_end_of_storage) {
        new (M_impl.M_finish) T;
        ++M_impl.M_finish;
    }
}

template <typename T>
void vector<T>::resize(vector::size_type count, const value_type &value) {
    auto size = this->size();
    if (size == capacity()) {
        return;
    }
    M_impl.M_start = static_cast<pointer>(realloc(M_impl.M_start, count));
    M_impl.M_finish = M_impl.M_start + size;
    M_impl.M_end_of_storage = M_impl.M_start + count;
    while (M_impl.M_finish != M_impl.M_end_of_storage) {
        new (M_impl.M_finish) T(value);
        ++M_impl.M_finish;
    }
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(const_iterator pos, const T &value) {
    if (M_impl.M_finish != M_impl.M_end_of_storage) {
        auto pos_ptr = pos.current - 1;
        auto now_ptr = M_impl.M_finish;
        ++M_impl.M_finish;
        while (now_ptr != pos_ptr) {
            new (now_ptr) T(std::move(*(now_ptr - 1)));
            --now_ptr;
        }
        new (pos_ptr) T(value);
        return iterator(pos_ptr);
    } else {
        auto pos_dif = pos.current - M_impl.M_start;
        auto new_size = this->size() * 2;
        M_impl.M_start = static_cast<pointer>(realloc(M_impl.M_start, new_size));
        M_impl.M_finish = M_impl.M_start + new_size / 2;
        M_impl.M_end_of_storage = M_impl.M_start + new_size;
        auto now_ptr = M_impl.M_finish;
        ++M_impl.M_finish;
        auto pos_ptr = M_impl.M_start + pos_dif;
        while (now_ptr != pos_ptr) {
            new (now_ptr) T(std::move(*(now_ptr - 1)));
            --now_ptr;
        }
        new (pos_ptr) T(value);
        return iterator(pos_ptr);
    }
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(const_iterator pos, T &&value) {
    if (M_impl.M_finish != M_impl.M_end_of_storage) {
        auto pos_ptr = pos.current - 1;
        auto now_ptr = M_impl.M_finish;
        ++M_impl.M_finish;
        while (now_ptr != pos_ptr) {
            new (now_ptr) T(std::move(*(now_ptr - 1)));
            --now_ptr;
        }
        ++pos_ptr;
        new (pos_ptr) T(std::move(value));
        return iterator(pos_ptr);
    } else {
        auto pos_dif = pos.current - M_impl.M_start;
        auto new_size = this->size() * 2;
        M_impl.M_start = static_cast<pointer>(realloc(M_impl.M_start, new_size));
        M_impl.M_finish = M_impl.M_start + new_size / 2;
        M_impl.M_end_of_storage = M_impl.M_start + new_size;
        auto now_ptr = M_impl.M_finish;
        ++M_impl.M_finish;
        auto pos_ptr = M_impl.M_start + pos_dif - 1;
        while (now_ptr != pos_ptr) {
            new (now_ptr) T(std::move(*(now_ptr - 1)));
            --now_ptr;
        }
        ++pos_ptr;
        new (pos_ptr) T(std::move(value));
        return iterator(pos_ptr.m);
    }
}
template <typename T>
typename vector<T>::iterator vector<T>::insert(const_iterator pos, size_type count, const T &value) {
    if (count <= 0) {
        return pos;
    }
    if (M_impl.M_end_of_storage - M_impl.M_finish >= count) {
        auto pos_ptr = pos.current - 1;
        M_impl.M_finish += count;
        auto now_ptr = M_impl.M_finish - 1;
        while (now_ptr - count != pos_ptr) {
            new (now_ptr) T(std::move(*(now_ptr - count)));
            --now_ptr;
        }
        ++pos_ptr;
        auto temp = pos_ptr;
        while (count--) {
            new (pos_ptr) T(value);
            ++pos_ptr;
        }
        return iterator(temp);
    } else {
        auto pos_dif = pos.current - M_impl.M_start;
        auto size = this->size();
        M_impl.M_start = static_cast<pointer>(realloc(M_impl.M_start, (size + count) * 2));
        M_impl.M_finish = M_impl.M_start + size;
        M_impl.M_end_of_storage = M_impl.M_start + (size + count) * 2;
        M_impl.M_finish += count;
        auto now_ptr = M_impl.M_finish - 1;
        auto pos_ptr = M_impl.M_start + pos_dif - 1;
        while (now_ptr - count != pos_ptr) {
            new (now_ptr) T(std::move(*(now_ptr - count)));
            --now_ptr;
        }
        ++pos_ptr;
        auto temp = pos_ptr;
        while (count--) {
            new (pos_ptr) T(value);
            ++pos_ptr;
        }
        return iterator(temp);
    }
}

// template <typename T>
// typename vector<T>::iterator vector<T>::insert(vector::const_iterator pos, const T &value) {
//     if (M_impl.M_finish != M_impl.M_end_of_storage) {
//         new (M_impl.M_finish) T(value);
//         ++M_impl.M_finish;
//     } else {
//         auto new_size = this->size() * 2;
//         M_impl.M_start = static_cast<pointer>(realloc(M_impl.M_start, new_size));
//         M_impl.M_finish = M_impl.M_start + new_size / 2;
//         M_impl.M_end_of_storage = M_impl.M_start + new_size;
//         new (M_impl.M_finish) T(value);
//         ++M_impl.M_finish;
//     }
// }
// template <typename T>
// typename vector<T>::iterator vector<T>::insert(vector::const_iterator pos, T &&value) {
//     if (M_impl.M_finish != M_impl.M_end_of_storage) {
//         new (M_impl.M_finish) T(std::move(value));
//         ++M_impl.M_finish;
//     } else {
//         auto new_size = this->size() * 2;
//         M_impl.M_start = static_cast<pointer>(realloc(M_impl.M_start, new_size));
//         M_impl.M_finish = M_impl.M_start + new_size / 2;
//         M_impl.M_end_of_storage = M_impl.M_start + new_size;
//         new (M_impl.M_finish) T(std::move(value));
//         ++M_impl.M_finish;
//     }
// }
} // namespace my_stl