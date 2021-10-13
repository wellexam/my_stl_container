#include <initializer_list>

template <typename T>
class list_node {
    using pointer = list_node *;
    pointer next;
    pointer prev;
    T data;
};

template <typename T>
class list {
    using size_type = size_t;
    using value_type = T;
    using reference = value_type &;
    using const_reference = const value_type &;

private:
    list_node<value_type> dummy_node;
    size_type list_size;
    void empty_initialize();

public:
    list();                                         // 默认构造函数。构造拥有默认构造的分配器的空容器
    list(size_type count, const value_type &value); // 构造拥有 count 个有值 value 的元素的容器
    explicit list(size_type count);                 // 构造拥有个 count 默认插入的 T 实例的容器。不进行复制
    list(const list &other);                        // 复制构造函数。构造拥有 other 内容的容器
    list(list &&other);                             // 移动构造函数。用移动语义构造拥有 other 内容的容器
    list(std::initializer_list<T> init);            // 构造拥有 initializer_list init 内容的容器
    ~list();                                        // 销毁 list 。调用元素的析构函数，然后解分配所用的存储。注意，若元素是指针，则不销毁所指向的对象

    list &operator=(const list &other); // 复制赋值运算符。以 other 的副本替换内容。
    list &operator=(list &&other); // 移动赋值运算符。用移动语义以 other 的内容替换内容（即从 other 移动 other 中的数据到此容器中）。之后 other 在合法但未指定的状态。
    list &operator=(std::initializer_list<T> ilist); // 以 initializer_list ilist 所标识者替换内容。

    size_t size(){return list_size};
    // void list<T>::insert(list<T>::iterator position, list<T>::size_type n, const T &x);
};

template <typename T>
list<T>::list(){empty_initialize()};

template <typename T>
list<T>::list(size_type count, const value_type &value) {}

template <typename T>
void list<T>::empty_initialize() {
    dummy_node = new list_node<value_type>; //创建一个节点，就是调用配置器分配内存
    node->next = dummy_node;                //傀儡节点的前后都指向自己，形成一个环
    node->prev = dummy_node;
    list_size = 0;
}

template <typename T, typename Ref, typename Ptr>
class list_iterator {
    using self = list_iterator<T, Ref, Ptr>;
    using iterator = list_iterator<T, T &, T *>;
    using const_iterator = list_iterator<T, const T &, const T *>;
    using value_type = T;
    using pointer = Ptr;
    using reference = Ref;
    using node = list_node<value_type>;

private:
    pointer current_node;

public:
    list_iterator(node *_current_node) : current_node(_current_node) {}
    list_iterator() : current_node(nullptr) {}
    list_iterator(const iterator &other) : current_node(other.current_node) {}

    bool operator==(const iterator &other) const { return this->current_node == other.current_node; }
    bool operator!=(const iterator &other) const { return !operator==(this, other); }
    reference operator*() const { return ((node *)current_node)->data; }
    pointer operator->() const { return &(operator*()); }

    self &operator++() {
        this->current_node = this->current_node->next;
        return *this;
    }

    self operator++(int) {
        auto temp = *this;
        current_node = current_node->next;
        return temp;
    }

    self &operator--() {
        this->current_node = this->current_node->prev;
        return *this;
    }

    self operator--(int) {
        auto temp = *this;
        this->current_node = this->current_node->prev;
        return temp;
    }
};