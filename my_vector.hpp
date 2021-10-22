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

    vector_base(vector_base &&_x) noexcept { this->M_impl.M_swap_data(_x.M_impl); }

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

public:
    //构造函数
    vector();                                //默认构造函数。构造拥有默认构造的分配器的空容器。
    vector(size_type count, const T &value); //构造拥有 count 个有值 value 的元素的容器。
    explicit vector(size_type count);        //构造拥有个 count 默认插入的 T 实例的容器。不进行复制。
    template <typename InputIt>              //
    vector(InputIt first, InputIt last);     //构造拥有范围 [first, last) 内容的容器。
    vector(const vector &other);             //复制构造函数。构造拥有other内容的容器。
    vector(vector &&other) noexcept;         //移动构造函数。用移动语义构造拥有other内容的容器。移动后，保证other为empty() 。
    vector(std::initializer_list<T> init);   //构造拥有 initializer_list init 内容的容器。

    //析构函数
    ~vector(); //销毁vector。调用元素的析构函数，然后解分配所用的存储。注意，若元素是指针，则不销毁所指向的对象。

    //赋值运算符重载
    vector &operator=(const vector &other); //复制赋值运算符。以 other 的副本替换内容。
    vector &operator=(vector &&other) noexcept; //移动赋值运算符。用移动语义以 other 的内容替换内容（即从 other 移动 other 中的数据到此容器中）。之后 other 在合法但未指定的状态。
    vector &operator=(std::initializer_list<T> ilist); //以 initializer_list ilist 所标识者替换内容。

    //赋值函数
    void assign(size_type count, const T &value); //以 count 份 value 的副本替换内容。
    template <class InputIt>                      //
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

    void swap( vector& other );//将内容与 other 的交换。不在单独的元素上调用任何移动、复制或交换操作。所有迭代器和引用保持合法。尾后迭代器被非法化。
};
} // namespace my_stl