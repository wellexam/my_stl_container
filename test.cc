#include "my_list.hpp"
#include <iostream>
#include <string>
#include <vector>

int main() {
    using std::cout;
    using std::endl;
    // list<int> shit;
    // auto iter = shit.rbegin();
    list<int> my_list{12312, 12312, 3123, 123, 12, 31, 23, 123, 213};
    auto iter = my_list.crbegin();
    while (iter != my_list.crend()) {
        cout << *iter << " ";
        ++iter;
    }
    cout << endl;
    list<int> list_1, lise_2(5, 1), list_3(5), list_4{1, 2, 3, 4, 5, 6}, list_5(list_4), list_6 = list_5, list_7 = {1, 2, 3, 4, 5, 6, 7};
    auto list_8(std::move(list_5));
    std::vector<list<int> *> vec{&list_1, &lise_2, &list_3, &list_4, &list_5, &list_6, &list_7, &list_8};
    list_4.emplace_back(29);
    cout << *(--list_4.cend()) << endl;
    auto temp = 56564;
    list_8.emplace_back(temp);
    cout << temp << endl;
    auto count = 1;
    for (auto &i : vec) {
        cout << count++ << ":";
        cout << i->front() << " ";
        for (auto &j : *i) {
            cout << j << " ";
        }
        cout << i->back() << " ";
        cout << endl;
    }
    list_4.swap(list_7);
    for (auto &i : list_4) {
        cout << i << " ";
    }
    cout << endl;
    for (auto &i : list_7) {
        cout << i << " ";
    }
    cout << endl;
    count = 1;
    for (auto &i : vec) {
        cout << count++ << ":";
        cout << "size = " << i->size() << "\t";
        i->pop_back();
        i->pop_front();
        for (auto &j : *i) {
            cout << j << " ";
        }
        cout << "\t";
        cout << "size = " << i->size();
        cout << endl;
    }
    for (auto &i : vec) {
        cout << count++ << ":";
        cout << "size = " << i->size() << "\t";
        i->resize(10);
        for (auto &j : *i) {
            cout << j << " ";
        }
        cout << "\t";
        cout << "size = " << i->size();
        cout << endl;
    }
}