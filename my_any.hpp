#pragma once
#include <cstdlib>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <iostream>

namespace mystl {
class any;

class any {
    class var_base;
    template <class T>
    class var;

public:
    template <class T>
    friend T any_cast(const any &operand);

    template <class T>
    friend T any_cast(any &operand);

    template <class T>
    friend T any_cast(any &&operand);

    template <class T>
    friend const T *any_cast(const any *operand) noexcept;

    template <class T>
    friend T *any_cast(any *operand) noexcept;

    constexpr any() noexcept = default; // 构造空对象。
    any(const any &other);     // 复制 other 的内容进新实例，从而任何内容的类型和值都等于构造函数调用前的 other 所拥有者，或者若 other 为空则内容为空。
    any(any &&other) noexcept; // 移动 other 的内容进新实例，从而任何内容的类型和值都等于构造函数调用前的 other 所拥有者，或者若 other 为空则内容为空。
    template <typename ValueType, typename = std::enable_if_t<!std::is_same<any, std::decay_t<ValueType>>::value>>
    any(ValueType &&value); //构造对象，其初始内容为 std::decay_t<ValueType> 类型对象，从 std::forward<ValueType>(value) 直接初始化它。
    template <typename ValueType, typename... Args>
    explicit any(std::in_place_type_t<ValueType>, Args &&...args); //构造对象，其初始内容为 std::decay_t<ValueType> 类型对象，从 std::forward<Args>(args)... 直接非列表初始化它。
    template <typename ValueType, typename U, typename... Args>
    explicit any(std::in_place_type_t<ValueType>, std::initializer_list<U> il,
                 Args &&...args); //构造对象，其初始内容为 std::decay_t<ValueType> 类型对象，从 il, std::forward<Args>(args)... 直接非列表初始化它。

    any &operator=(const any &rhs);     // 以复制 rhs 的状态赋值，如同用 any(rhs).swap(*this) 。
    any &operator=(any &&rhs) noexcept; // 以移动 rhs 的状态赋值，如同用 any(std::move(rhs)).swap(*this) 。赋值后 rhs 留在合法但未指定的状态。
    template <typename ValueType, typename = std::enable_if_t<!std::is_same<any, std::decay_t<ValueType>>::value>> // 以 rhs 的类型和值赋值，如同用 any(std::forward<ValueType>(rhs)).swap(*this) 。
    any &operator=(ValueType &&rhs); // 此重载仅若 std::decay_t<ValueType> 与 any 不是同一类型且 std::is_copy_constructible_v<std::decay_t<ValueType>> 为 true才参与重载决议。

    ~any();

    template <class ValueType, class... Args>
    std::decay_t<ValueType> &emplace(Args &&...args);
    template <class ValueType, class U, class... Args>
    std::decay_t<ValueType> &emplace(std::initializer_list<U> il, Args &&...args);

    void reset() noexcept;

    void swap(any &other) noexcept;

    [[nodiscard]] bool has_value() const noexcept;

    [[nodiscard]] const std::type_info &type() const noexcept;

protected:
    var_base *ptr = nullptr;
    std::type_info *info = const_cast<std::type_info *>(&typeid(void));

private:
    class var_base {
    public:
        var_base() = default;
        virtual ~var_base() = default;
    };

    template <class T>
    class var : private var_base {
    public:
        T data;

        var(T &_data) : data(_data) {}
        var(T &&_data) : data(std::move(_data)) {}

        ~var() { delete data; }

        var *copy_constructor() const { return new var<T>(this->data); }
    };
};
} // namespace mystl

/*
namespace mystl {
class any;

template <typename T>
void destroy_impl(void *ptr) {
    delete static_cast<std::remove_cv_t<std::remove_reference_t<T>> *>(ptr);
}

template <typename T>
void *construct_impl(void *ptr) {
    auto casted_ptr = static_cast<const std::remove_cv_t<std::remove_reference_t<T>> *>(ptr);
    return new std::remove_reference_t<T>(std::forward<const std::remove_reference_t<T>>(*casted_ptr));
}

void swap(any &lhs, any &rhs) noexcept;

class any {
protected:
    using destory_ptr = void (*)(void *const);
    using construct_ptr = void *(*)(void *const);

    std::type_info *info = const_cast<std::type_info *>(&typeid(void));
    void *ptr = nullptr;
    destory_ptr destroy_fun = nullptr;
    construct_ptr construct_fun = nullptr;

public:
    template <class T>
    friend T any_cast(const any &operand);

    template <class T>
    friend T any_cast(any &operand);

    template <class T>
    friend T any_cast(any &&operand);

    template <class T>
    friend const T *any_cast(const any *operand) noexcept;

    template <class T>
    friend T *any_cast(any *operand) noexcept;

    constexpr any() noexcept = default; // 构造空对象。
    any(const any &other);     // 复制 other 的内容进新实例，从而任何内容的类型和值都等于构造函数调用前的 other 所拥有者，或者若 other 为空则内容为空。
    any(any &&other) noexcept; // 移动 other 的内容进新实例，从而任何内容的类型和值都等于构造函数调用前的 other 所拥有者，或者若 other 为空则内容为空。
    template <typename ValueType, typename = std::enable_if_t<!std::is_same<any, std::decay_t<ValueType>>::value>>
    any(ValueType &&value); //构造对象，其初始内容为 std::decay_t<ValueType> 类型对象，从 std::forward<ValueType>(value) 直接初始化它。
    template <typename ValueType, typename... Args>
    explicit any(std::in_place_type_t<ValueType>, Args &&...args); //构造对象，其初始内容为 std::decay_t<ValueType> 类型对象，从 std::forward<Args>(args)... 直接非列表初始化它。
    template <typename ValueType, typename U, typename... Args>
    explicit any(std::in_place_type_t<ValueType>, std::initializer_list<U> il,
                 Args &&...args); //构造对象，其初始内容为 std::decay_t<ValueType> 类型对象，从 il, std::forward<Args>(args)... 直接非列表初始化它。

    any &operator=(const any &rhs);     // 以复制 rhs 的状态赋值，如同用 any(rhs).swap(*this) 。
    any &operator=(any &&rhs) noexcept; // 以移动 rhs 的状态赋值，如同用 any(std::move(rhs)).swap(*this) 。赋值后 rhs 留在合法但未指定的状态。
    template <typename ValueType, typename = std::enable_if_t<!std::is_same<any, std::decay_t<ValueType>>::value>> // 以 rhs 的类型和值赋值，如同用 any(std::forward<ValueType>(rhs)).swap(*this) 。
    any &operator=(ValueType &&rhs); // 此重载仅若 std::decay_t<ValueType> 与 any 不是同一类型且 std::is_copy_constructible_v<std::decay_t<ValueType>> 为 true才参与重载决议。

    ~any();

    template <class ValueType, class... Args>
    std::decay_t<ValueType> &emplace(Args &&...args);
    template <class ValueType, class U, class... Args>
    std::decay_t<ValueType> &emplace(std::initializer_list<U> il, Args &&...args);

    void reset() noexcept;

    void swap(any &other) noexcept;

    [[nodiscard]] bool has_value() const noexcept;

    [[nodiscard]] const std::type_info &type() const noexcept;
};
any::any(const any &other) {
    this->info = other.info;
    if (*this->info != typeid(void)) {
        this->destroy_fun = other.destroy_fun;
        this->construct_fun = other.construct_fun;
        this->ptr = this->construct_fun(other.ptr);
    }
}
any::any(any &&other) noexcept {
    this->info = std::move_if_noexcept(other.info);
    if (*this->info != typeid(void)) {
        this->destroy_fun = other.destroy_fun;
        this->ptr = other.ptr;
    }
}
template <typename ValueType, typename>
any::any(ValueType &&value) {
    this->info = const_cast<std::type_info *>(&typeid(ValueType));
    this->destroy_fun = &destroy_impl<ValueType>;
    this->construct_fun = &construct_impl<ValueType>;
    this->ptr = new std::remove_reference_t<ValueType>(std::forward<ValueType>(value));
}
template <typename ValueType, typename... Args>
any::any(std::in_place_type_t<ValueType>, Args &&...args) {
    this->info = const_cast<std::type_info *>(&typeid(ValueType));
    this->destroy_fun = &destroy_impl<ValueType>;
    this->construct_fun = &construct_impl<ValueType>;
    this->ptr = new ValueType(std::forward<Args>(args)...);
}

template <typename ValueType, typename U, typename... Args>
any::any(std::in_place_type_t<ValueType>, std::initializer_list<U> il, Args &&...args) {
    this->info = const_cast<std::type_info *>(&typeid(ValueType));
    this->destroy_fun = &destroy_impl<ValueType>;
    this->construct_fun = &construct_impl<ValueType>;
    this->ptr = new ValueType(std::forward<std::initializer_list<U>>(il), std::forward<Args>(args)...);
}

any::~any() {
    this->reset();
}

any &any::operator=(const any &rhs) {
    if (this->ptr == rhs.ptr) {
        return *this;
    }
    any(rhs).swap(*this);
    return *this;
}

any &any::operator=(any &&rhs) noexcept {
    if (this->ptr == rhs.ptr) {
        return *this;
    }
    any(std::move(rhs)).swap(*this);
    return *this;
}

template <typename ValueType, typename>
any &any::operator=(ValueType &&rhs) {
    any(std::forward<ValueType>(rhs)).swap(*this);
    return *this;
}

void any::reset() noexcept {
    if (!(*(this->info) == typeid(void))) {
        this->destroy_fun(this->ptr);
        this->ptr = nullptr;
        this->info = const_cast<std::type_info *>(&typeid(void));
    }
}
void any::swap(any &other) noexcept {
    std::swap(this->ptr, other.ptr);
    std::swap(this->info, other.info);
    std::swap(this->destroy_fun, other.destroy_fun);
    std::swap(this->construct_fun, other.construct_fun);
}
template <class ValueType, class... Args>
std::decay_t<ValueType> &any::emplace(Args &&...args) {
    this->reset();
    this->info = const_cast<std::type_info *>(&typeid(ValueType));
    this->destroy_fun = &destroy_impl<ValueType>;
    this->construct_fun = &construct_impl<ValueType>;
    this->ptr = new ValueType(std::forward<Args>(args)...);
    return *static_cast<ValueType *>(this->ptr);
}
template <class ValueType, class U, class... Args>
std::decay_t<ValueType> &any::emplace(std::initializer_list<U> il, Args &&...args) {
    this->reset();
    this->info = const_cast<std::type_info *>(&typeid(ValueType));
    this->destroy_fun = &destroy_impl<ValueType>;
    this->construct_fun = &construct_impl<ValueType>;
    this->ptr = new ValueType(std::forward<std::initializer_list<U>>(il), std::forward<Args>(args)...);
    return *static_cast<ValueType *>(this->ptr);
}
bool any::has_value() const noexcept {
    return (*(this->info) != typeid(void));
}
const std::type_info &any::type() const noexcept {
    return *this->info;
}

void swap(any &lhs, any &rhs) noexcept {
    lhs.swap(rhs);
}
template <class T>
T any_cast(const any &operand) {
    if (typeid(T) != operand.type()) {
        throw std::bad_cast();
    }
    return static_cast<T>(*any_cast<std::remove_cv_t<std::remove_reference_t<T>>>(&operand));
}
template <class T>
T any_cast(any &operand) {
    if (typeid(T) != operand.type()) {
        throw std::bad_cast();
    }
    return static_cast<T>(*any_cast<std::remove_cv_t<std::remove_reference_t<T>>>(&operand));
}
template <class T>
T any_cast(any &&operand) {
    if (typeid(T) != operand.type()) {
        throw std::bad_cast();
    }
    return static_cast<T>(std::move(*any_cast<std::remove_cv_t<std::remove_reference_t<T>>>(&operand)));
}
template <class T>
const T *any_cast(const any *operand) noexcept {
    if (typeid(T) != operand->type() || !operand) {
        return nullptr;
    }
    return static_cast<const T *>(operand->ptr);
}
template <class T>
T *any_cast(any *operand) noexcept {
    if (typeid(T) != operand->type() || !operand) {
        return nullptr;
    }
    return static_cast<T *>(operand->ptr);
}
template <class T, class... Args>
any make_any(Args &&...args) {
    return any(std::in_place_type<T>, std::forward<Args>(args)...);
}
template <class T, class U, class... Args>
any make_any(std::initializer_list<U> il, Args &&...args) {
    return any(std::in_place_type<T>, il, std::forward<Args>(args)...);
}
} // namespace mystl
 */