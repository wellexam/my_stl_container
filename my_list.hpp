template <typename T>
class __list_node {
    using void_pointer = __list_node *;
    // typedef __list_node* void_pointer;
    void_pointer next; // 为void*类型，其实可以是__list_node<T>*
    void_pointer prev;
    T data; //存储数据
};

template <typename T, typename Ref, typename Ptr>
class _iterator {
    using link_type = __list_node<T> *;
    link_type node; // 这个迭代器指向的节点
    // 迭代器构造函数
    __list_iterator(link_type x) : node(x) {}
    __list_iterator() {}
    __list_iterator(const iterator &x) : node(x.node) {}
    // 迭代器必要的行为实现
    bool operator==(const self &x) const { return node == x.node; }
    bool operator!=(const self &x) const { return node != x.node; }
    //迭起器取值，取得是节点的值
    reference operator*() const { return (*node).data; }
#ifndef __SGI_STL_NO_ARROW_OPERATOR
    //成员调用操作符
    pointer operator->() const { return &(operator*()); }
#endif /* __SGI_STL_NO_ARROW_OPERATOR */
    // 先加１，再返回，类似与++i
    self &operator++() {
        node = (link_type)((*node).next);
        return *this;
    }
    //先加１，然后返回加１前的数据，类似与i++
    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }
    // 先减１，再返回，类似于--i
    self &operator--() {
        node = (link_type)((*node).prev);
        return *this;
    }
    //先减１，再返回减１前的数据，类似于i--
    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }
};

template <typename T>
class my_list {
    using size_type = size_t;
    using value_type = T;

private:
    __list_node<value_type> node;
    size_type list_size;
    void empty_initialize();

public:
    my_list();
    my_list(size_type count, const value_type &value);
    ~my_list();

    size_t size(){return list_size};
    void my_list<T>::insert(my_list<T>::iterator position, my_list<T>::size_type n, const T &x);
};

template <typename T>
my_list<T>::my_list(){empty_initialize()};

template <typename T>
my_list<T>::my_list(size_type count, const value_type &value) {}

template <typename T>
void my_list<T>::empty_initialize() {
    node = new __list_node<value_type>; //创建一个节点，就是调用配置器分配内存
    node->next = node;                  //傀儡节点的前后都指向自己，形成一个环
    node->prev = node;
    list_size = 0;
}

template <class T>
void my_list<T>::insert(iterator position, my_list<T>::size_type n, const T& x) {
  for ( ; n > 0; --n)
    insert(position, x);
}

template <class T, class Alloc> template <class InputIterator>
void list<T, Alloc>::insert(iterator position,
                            InputIterator first, InputIterator last) {
  for ( ; first != last; ++first)
    insert(position, *first);
}

template <typename T>
void fill_initialize(size_type n, const T &value) {
    empty_initialize();
    __STL_TRY { insert(begin(), n, value); }
    __STL_UNWIND(clear(); put_node(node));
}

template <typename T>
void range_initialize(InputIterator first, InputIterator last) {
    empty_initialize();
    __STL_TRY { insert(begin(), first, last); }
    __STL_UNWIND(clear(); put_node(node));
}