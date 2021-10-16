#include "my_list.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <list>

class shit {
public:
    int data1 = 0;
    int data2 = 0;

    bool operator<(shit &other) const { return this->data1 < other.data1; }

    shit() = default;
    shit(int _data1, int _data2) : data1(_data1), data2(_data2) {}
    ~shit() = default;
};

bool cmp(shit &a, shit &b) {
    return a.data1 < b.data1;
}

int main() {
    using std::cout;
    using std::endl;
    std::list<int> std_list{1, 2, 3, 5, 3, 2, 7};
    std_list.sort();
    my_stl::list<shit> my_list{shit(1, 3), shit(2, 3), shit(5, 3), shit(2, 7)};
    my_list.sort(cmp);
    auto iter = my_list.begin();
    while (iter != my_list.end()) {
        cout << iter->data1 << " " << iter->data2 << "\t";
        ++iter;
    }
    cout << endl;
    my_stl::list<int> list_1, lise_2(5, 1), list_3(5), list_4{1, 2, 3, 4, 5, 6}, list_5(list_4), list_6 = list_5, list_7 = {1, 2, 3, 4, 5, 6, 7};
    auto list_8(std::move(list_5));
    std::vector<my_stl::list<int> *> vec{&list_1, &lise_2, &list_3, &list_4, &list_5, &list_6, &list_7, &list_8};
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