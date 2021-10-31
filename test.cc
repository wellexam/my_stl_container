#include "test.h"
#include "vector_test.h"
#include "list_test.h"
#include "my_any.hpp"
#include <any>
#include <iostream>
#include <string>
#include <vector>

//int main()
//{
//    std::cout << "Size of any: " << sizeof(mystl::any) << std::endl;
//    std::cout << "Size of size_t: " << sizeof(std::size_t) << std::endl;
//    std::cout << "Size of string: " << sizeof(std::string) << std::endl;
//    std::cout << "Size of vector: " << sizeof(std::vector<mystl::any>) << std::endl;
//    mystl::any a, b(10), c(std::string("Hello"));
//    mystl::any d(a), e(b), f(c);
//    a = 3.14;
//    d = a;
//    a.reset();
//    try {
//        std::cout << mystl::any_cast<double>(a) << std::endl;
//    } catch (std::exception& e){
//        std::cout << "bad_cast" << std::endl;
//    }
//    std::cout << mystl::any_cast<int>(b) << std::endl;
//    std::cout << mystl::any_cast<std::string>(c) << std::endl;
//    std::cout << mystl::any_cast<double>(d) << std::endl;
//    std::cout << mystl::any_cast<int>(e) << std::endl;
//    std::cout << mystl::any_cast<std::string>(f) << std::endl;
//    return 0;
//}

//int main()
//{
//    std::cout << "Size of any: " << sizeof(std::any) << std::endl;
//    std::cout << "Size of size_t: " << sizeof(std::size_t) << std::endl;
//    std::cout << "Size of string: " << sizeof(std::string) << std::endl;
//    std::cout << "Size of vector: " << sizeof(std::vector<std::any>) << std::endl;
//    std::any a, b(10), c(std::string("Hello"));
//    std::any d(a), e(b), f(c);
//    a = 3.14;
//    d = a;
//    std::cout << std::any_cast<double>(a) << std::endl;
//    std::cout << std::any_cast<int>(b) << std::endl;
//    std::cout << std::any_cast<std::string>(c) << std::endl;
//    std::cout << std::any_cast<double>(d) << std::endl;
//    std::cout << std::any_cast<int>(e) << std::endl;
//    std::cout << std::any_cast<std::string>(f) << std::endl;
//    return 0;
//}

int main() {
    //std::test::vector_test::vector_test();
    //std::test::list_test::list_test();
    mystl::any a;
    mystl::any b(1);
    mystl::any c(b);
    c.reset();
    a.emplace<mystl::vector<int>>({1,2,3,4,5});
    for (auto &i : mystl::any_cast<mystl::vector<int>>(a)) {
        std::cout << i << '\t';
    }
    std::cout << std::endl;
    c = a;
    for (auto &i : mystl::any_cast<mystl::vector<int>>(c)) {
        std::cout << i << '\t';
    }
    return 0;
}