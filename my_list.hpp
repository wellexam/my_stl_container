#include <initializer_list>
#include <iterator>

template <typename T>
class list_node {
public:
    list_node *next;
    list_node *prev;
    T data;

    list_node() : next(nullptr), prev(nullptr){};
    explicit list_node(const T &_data) : data(_data){};
    list_node(const T &_data, list_node *_next, list_node *_prev) : data(_data), next(_next), prev(_prev){};
};

template <typename T, typename Ref, typename Ptr>
class list_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    using self = list_iterator<T, Ref, Ptr>;
    using iterator = list_iterator<T, T &, T *>;
    using const_iterator = list_iterator<T, const T &, const T *>;
    using value_type = typename std::iterator<std::bidirectional_iterator_tag, T>::value_type;
    using pointer = typename std::iterator<std::bidirectional_iterator_tag, T>::pointer;
    using reference = typename std::iterator<std::bidirectional_iterator_tag, T>::reference;
    using node = list_node<value_type>;

protected:
    node *current_node;

public:
    explicit list_iterator(node *_current_node) : current_node(_current_node) {}
    list_iterator() : current_node(nullptr) {}
    list_iterator(const iterator &other) : current_node(other.get_ptr()) {}

    bool operator==(const iterator &other) const { return this->current_node == other.current_node; }
    bool operator!=(const iterator &other) const { return !operator==(other); }
    list_iterator &operator=(const iterator &other);

    Ref operator*() const { return ((node *)current_node)->data; }
    Ptr operator->() const { return &(operator*()); }

    node *get_ptr() const { return current_node; }
    node *&get_ref_to_ptr() { return current_node; }

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

template <typename T, typename Ref, typename Ptr>
list_iterator<T, Ref, Ptr> &list_iterator<T, Ref, Ptr>::operator=(const list_iterator::iterator &other) {
    if (current_node == other.current_node) {
        return *this;
    }
    current_node = other.current_node;
    return *this;
}

template <typename T>
class list {
    using size_type = size_t;
    using value_type = T;
    using node = list_node<value_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = list_iterator<T, T &, T *>;
    using const_iterator = list_iterator<T, const T &, const T *>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    node *dummy_node;    //空白结点
    size_type list_size; //链表元素数量

public:
    //友元类
    // friend class iterator;
    // friend class const_iterator;

    //构造函数
    list();                                         // 默认构造函数。构造拥有默认构造的分配器的空容器
    list(size_type count, const value_type &value); // 构造拥有 count 个有值 value 的元素的容器
    // explicit list(size_type count);                 // 构造拥有个 count 默认插入的 T 实例的容器。不进行复制
    list(const list &other);             // 复制构造函数。构造拥有 other 内容的容器
    list(list &&other) noexcept;         // 移动构造函数。用移动语义构造拥有 other 内容的容器
    list(std::initializer_list<T> init); // 构造拥有 initializer_list init 内容的容器
    ~list();                             // 销毁 list 。调用元素的析构函数，然后解分配所用的存储。注意，若元素是指针，则不销毁所指向的对象

    list &operator=(const list &other); // 复制赋值运算符。以 other 的副本替换内容。
    list &operator=(list &&other) noexcept; // 移动赋值运算符。用移动语义以 other 的内容替换内容（即从 other 移动 other 中的数据到此容器中）。之后 other 在合法但未指定的状态。
    list &operator=(std::initializer_list<T> ilist); // 以 initializer_list ilist 所标识者替换内容。

    void assign(size_type count, const T &value); // 以 count 份 value 的副本替换内容。
    template <class InputIt>
    void assign(InputIt first, InputIt last);    // 以范围 [first, last) 中元素的副本替换内容。若任一参数是指向 *this 中的迭代器则行为未定义。
    void assign(std::initializer_list<T> ilist); // 以来自 initializer_list ilist 的元素替换内容

    //元素访问
    reference front();             //返回到容器首元素的引用。
    const_reference front() const; //返回到容器首元素的引用。
    reference back();              //返回到容器中最后一个元素的引用
    const_reference back() const;  //返回到容器中最后一个元素的引用

    //迭代器
    iterator begin() noexcept;                       //返回指向首元素的迭代器。
    const_iterator begin() const noexcept;           //返回指向首元素的迭代器。
    const_iterator cbegin() const noexcept;          //返回指向首元素的迭代器。
    iterator end() noexcept;                         //返回指向 list 末元素后一元素的迭代器。
    const_iterator end() const noexcept;             //返回指向 list 末元素后一元素的迭代器。
    const_iterator cend() const noexcept;            //返回指向 list 末元素后一元素的迭代器。
    reverse_iterator rbegin() noexcept;              //返回指向逆向 list 首元素的逆向迭代器。它对应非逆向 list 的末元素。
    const_reverse_iterator rbegin() const noexcept;  //返回指向逆向 list 首元素的逆向迭代器。它对应非逆向 list 的末元素。
    const_reverse_iterator crbegin() const noexcept; //返回指向逆向 list 首元素的逆向迭代器。它对应非逆向 list 的末元素。
    reverse_iterator rend() noexcept; //返回指向逆向 list 末元素后一元素的逆向迭代器。它对应非逆向 list 首元素的前一元素。此元素表现为占位符，试图访问它导致未定义行为。
    const_reverse_iterator rend() const noexcept; //返回指向逆向 list 末元素后一元素的逆向迭代器。它对应非逆向 list 首元素的前一元素。此元素表现为占位符，试图访问它导致未定义行为。
    const_reverse_iterator crend() const noexcept; //返回指向逆向 list 末元素后一元素的逆向迭代器。它对应非逆向 list 首元素的前一元素。此元素表现为占位符，试图访问它导致未定义行为。

    //容量
    bool empty() const noexcept;                           //检查容器是否无元素，即是否 begin() == end() 。
    size_type size() const noexcept { return list_size; }; //返回容器中的元素数，即 std::distance(begin(), end())。
    size_type max_size() const noexcept;                   //返回根据系统或库实现限制的容器可保有的元素最大数量，即对于最大容器的 std::distance(begin(), end()) 。

    //修改器
    void clear() noexcept; //从容器擦除所有元素。此调用后 size() 返回零。非法化任何指代所含元素的引用、指针或迭代器。任何尾后迭代器保持合法。
    iterator insert(const_iterator pos, const T &value);                  //插入元素到容器中的指定位置。
    iterator insert(const_iterator pos, T &&value);                       //插入元素到容器中的指定位置。
    iterator insert(const_iterator pos, size_type count, const T &value); //插入元素到容器中的指定位置。
    template <class InputIt>                                              //
    iterator insert(const_iterator pos, InputIt first, InputIt last);     //插入元素到容器中的指定位置。
    iterator insert(const_iterator pos, std::initializer_list<T> ilist);  //插入元素到容器中的指定位置。

    template <class... Args>
    iterator emplace(const_iterator pos, Args &&...args); //直接于 pos 前插入元素到容器中。

    iterator erase(const_iterator pos);                        //移除位于 pos 的元素。
    iterator erase(const_iterator first, const_iterator last); //移除范围 [first; last) 中的元素。

    void push_back(const T &value); //后附给定元素 value 到容器尾。 初始化新元素为 value 的副本。
    void push_back(T &&value);      //后附给定元素 value 到容器尾。移动 value 进新元素。

    template <class... Args>
    void emplace_back(Args &&...args); //添加新元素到容器尾。

    void pop_back(); //移除容器的末元素。

    void push_front(const T &value); //前附给定元素 value 到容器起始。
    void push_front(T &&value);      //前附给定元素 value 到容器起始。

    template <class... Args>
    void emplace_front(Args &&...args); //插入新元素到容器起始。

    void pop_front(); //移除容器首元素。指向被擦除元素的迭代器和引用被非法化。

    void resize(size_type count);                          //重设容器大小以容纳 count 个元素。
    void resize(size_type count, const value_type &value); //重设容器大小以容纳 count 个元素。

    void swap(list &other); //将内容与 other 的交换。不在单独的元素上调用任何移动、复制或交换操作。所有迭代器和引用保持合法。在操作后，保有此容器中尾后值的迭代器指代此容器或另一容器是未指定的。

    //操作
    void merge(list &other);                //归并二个已排序链表为一个。链表应以升序排序。
    void merge(list &&other);               //归并二个已排序链表为一个。链表应以升序排序。
    template <class Compare>                //
    void merge(list &other, Compare comp);  //归并二个已排序链表为一个。链表应以升序排序。
    template <class Compare>                //
    void merge(list &&other, Compare comp); //归并二个已排序链表为一个。链表应以升序排序。

    //从一个 list 转移元素给另一个。不复制或移动元素，仅重指向链表结点的内部指针。
    void splice(const_iterator pos, list &other); //从 other 转移所有元素到 *this 中。元素被插入到 pos 所指向的元素之前。操作后容器 other 变为空。若 other 与 *this 指代同一对象则行为未定义。
    void splice(const_iterator pos, list &&other); //从 other 转移所有元素到 *this 中。元素被插入到 pos 所指向的元素之前。操作后容器 other 变为空。若 other 与 *this 指代同一对象则行为未定义。
    void splice(const_iterator pos, list &other, const_iterator it);  // 从 other 转移 it 所指向的元素到 *this 。元素被插入到 pos 所指向的元素之前。
    void splice(const_iterator pos, list &&other, const_iterator it); // 从 other 转移 it 所指向的元素到 *this 。元素被插入到 pos 所指向的元素之前。
    void splice(const_iterator pos, list &other, const_iterator first,
                const_iterator last); //从 other 转移范围 [first, last) 中的元素到 *this 。元素被插入到 pos 所指向的元素之前。若 pos 是范围 [first,last) 中的迭代器则行为未定义。
    void splice(const_iterator pos, list &&other, const_iterator first,
                const_iterator last); //从 other 转移范围 [first, last) 中的元素到 *this 。元素被插入到 pos 所指向的元素之前。若 pos 是范围 [first,last) 中的迭代器则行为未定义。

    void remove(const T &value);      //移除所有等于 value 的元素
    template <class UnaryPredicate>   //
    void remove_if(UnaryPredicate p); //移除所有谓词 p 对它返回 true 的元素。

    void reverse() noexcept; //逆转容器中的元素顺序。不非法化任何引用或迭代器。

    void unique();                   //从容器移除所有相继的重复元素。只留下相等元素组中的第一个元素。
    template <class BinaryPredicate> //
    void unique(BinaryPredicate p);  //从容器移除所有相继的重复元素。只留下相等元素组中的第一个元素。

    void sort();             //以升序排序元素。保持相等元素的顺序。
    template <class Compare> //
    void sort(Compare comp); //以升序排序元素。保持相等元素的顺序。
};

template <typename T>
list<T>::list() {
    dummy_node = new list_node<value_type>; //创建一个节点，就是调用配置器分配内存
    dummy_node->next = dummy_node;          //傀儡节点的前后都指向自己，形成一个环
    dummy_node->prev = dummy_node;
    list_size = 0;
}

template <typename T>
list<T>::list(size_type count, const value_type &value) {
    list_size = count;
    dummy_node = new list_node<value_type>;
    auto temp_pointer = dummy_node;
    while (count--) {
        temp_pointer->next = new list_node<value_type>(value, nullptr, temp_pointer);
        temp_pointer = temp_pointer->next;
    }
    temp_pointer->next = dummy_node;
    dummy_node->prev = temp_pointer;
}

template <typename T>
list<T>::list(const list &other) {
    list_size = other.list_size;
    auto iter = other.cbegin();
    dummy_node = new list_node<value_type>;
    node *temp_pointer = dummy_node;
    while (iter != other.cend()) {
        temp_pointer->next = new list_node<value_type>(*iter, nullptr, temp_pointer);
        temp_pointer = temp_pointer->next;
        ++iter;
    }
    temp_pointer->next = dummy_node;
    dummy_node->prev = temp_pointer;
}

template <typename T>
list<T>::list(list &&other) noexcept {
    this->dummy_node = other.dummy_node;
    other.dummy_node = new list_node<value_type>;
    other.dummy_node->next = other.dummy_node;
    other.dummy_node->prev = other.dummy_node;
    other.list_size = 0;
    list_size = other.list_size;
}

template <typename T>
void list<T>::clear() noexcept {
    auto iter = this->begin();
    while (iter != this->end()) {
        auto temp = iter.get_ptr();
        ++iter;
        delete temp;
    }
    dummy_node->next = dummy_node;
    dummy_node->prev = dummy_node;
    this->list_size = 0;
}

template <typename T>
list<T>::~list() {
    this->clear();
    delete this->dummy_node;
}

template <typename T>
list<T> &list<T>::operator=(const list &other) {
    if (this == &other) {
        return *this;
    }
    this->clear();
    auto const_iter = other.cbegin();
    node *temp_pointer = dummy_node;
    while (const_iter != other.cend()) {
        temp_pointer->next = new list_node<value_type>(*const_iter, nullptr, temp_pointer);
        temp_pointer = temp_pointer->next;
        ++const_iter;
    }
    temp_pointer->next = dummy_node;
    dummy_node->prev = temp_pointer;
    this->list_size = other.list_size;
    return *this;
}

template <typename T>
list<T> &list<T>::operator=(list &&other) noexcept {
    this->clear();
    delete this->dummy_node;
    this->dummy_node = other.dummy_node;
    this->list_size = other.list_size;
    other.dummy_node = new list_node<value_type>;
    other.dummy_node->next = other.dummy_node;
    other.dummy_node->prev = other.dummy_node;
    other.list_size = 0;
    return *this;
}

template <typename T>
void list<T>::assign(size_type count, const T &value) {
    this->clear();
    list_size = count;
    auto temp_pointer = dummy_node;
    while (count--) {
        temp_pointer->next = new list_node<value_type>(value, nullptr, temp_pointer);
        temp_pointer = temp_pointer->next;
    }
    temp_pointer->next = dummy_node;
    dummy_node->prev = temp_pointer;
}

template <typename T>
T &list<T>::front() {
    return *(this->begin());
}

template <typename T>
const T &list<T>::front() const {
    return *(this->cbegin());
}

template <typename T>
T &list<T>::back() {
    return *(--(this->end()));
}

template <typename T>
const T &list<T>::back() const {
    return *(--(this->cend()));
}

template <typename T>
typename list<T>::iterator list<T>::begin() noexcept {
    iterator temp(this->dummy_node->next);
    return temp;
}

template <typename T>
typename list<T>::const_iterator list<T>::begin() const noexcept {
    const_iterator temp(this->dummy_node->next);
    return temp;
}

template <typename T>
typename list<T>::const_iterator list<T>::cbegin() const noexcept {
    const_iterator temp(this->dummy_node->next);
    return temp;
}

template <typename T>
typename list<T>::iterator list<T>::end() noexcept {
    iterator temp(this->dummy_node);
    return temp;
}

template <typename T>
typename list<T>::const_iterator list<T>::end() const noexcept {
    const_iterator temp(this->dummy_node);
    return temp;
}

template <typename T>
typename list<T>::const_iterator list<T>::cend() const noexcept {
    const_iterator temp(this->dummy_node);
    return temp;
}

template <typename T>
typename list<T>::reverse_iterator list<T>::rbegin() noexcept {
    reverse_iterator temp(iterator(this->dummy_node));
    return temp;
}

template <typename T>
typename list<T>::const_reverse_iterator list<T>::rbegin() const noexcept {
    const_reverse_iterator temp(const_iterator(this->dummy_node));
    return temp;
}

template <typename T>
typename list<T>::const_reverse_iterator list<T>::crbegin() const noexcept {
    const_reverse_iterator temp(const_iterator(this->dummy_node));
    return temp;
}

template <typename T>
typename list<T>::reverse_iterator list<T>::rend() noexcept {
    reverse_iterator temp(iterator(this->dummy_node->next));
    return temp;
}

template <typename T>
typename list<T>::const_reverse_iterator list<T>::rend() const noexcept {
    const_reverse_iterator temp(const_iterator(this->dummy_node->next));
    return temp;
}

template <typename T>
typename list<T>::const_reverse_iterator list<T>::crend() const noexcept {
    const_reverse_iterator temp(const_iterator(this->dummy_node->next));
    return temp;
}
template <typename T>
bool list<T>::empty() const noexcept {
    return !list_size;
}
template <typename T>
typename list<T>::iterator list<T>::insert(list::const_iterator pos, const T &value) {
    auto current = pos.get_ptr();
    auto prev = current->prev;
    auto new_node = new node(value, current, prev);
    prev->next = new_node;
    current->prev = new_node;
    ++list_size;
    return iterator(new_node);
}
