#include "my_list.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <list>

class shit {
public:
    int data1 = 0;
    int data2 = 0;

    bool operator<(const shit &other) const { return this->data1 < other.data1; }
    bool operator==(const shit &other) const { return this->data1 == other.data1 && this->data2 == other.data2; }

    shit() = default;
    shit(int _data1, int _data2) : data1(_data1), data2(_data2) {}
    ~shit() = default;
};

bool cmp(const shit &a, const shit &b) {
    return a.data1 < b.data1;
}

bool equal(const shit &a, const shit &b) {
    return a == b;
}

bool p(const shit &a) {
    return a == shit(5, 3);
}

int main() {
    using std::cout;
    using std::endl;
    my_stl::list<int> list;
    list.assign({1, 2, 3, 5, 3, 2, 7});
    for (auto &i : list) {
        cout << i << " ";
    }
    std::list<int> std_list{1, 2, 3, 5, 3, 2, 7};
    std_list.sort();
    my_stl::list<shit> my_list{shit(1, 3), shit(2, 3), shit(5, 3), shit(2, 7)};
    my_stl::list<shit> my_list_2{shit(1, 4), shit(2, 4), shit(5, 4), shit(2, 4), shit(4, 1), shit(6, 3), shit(2, 4)};
    my_list.splice(my_list.end(), std::move(my_list_2), ++my_list_2.begin(), --(--my_list_2.end()));
    for (auto &i : my_list) {
       cout << i.data1 << " " << i.data2 << "\t";
    }
    cout << my_list.size();
    cout << endl;
    my_list.sort();
    my_list_2.unique(equal);
    my_list_2.sort();
    for (auto &i : my_list_2) {
       cout << i.data1 << " " << i.data2 << "\t";
    }
    cout << endl;
    auto my_list_3(my_list_2);
    my_list_3.sort();
    //my_list_2.unique(equal);
    for (auto &i : my_list_2) {
       cout << i.data1 << " " << i.data2 << "\t";
    }
    cout << endl;
    for (auto &i : my_list_3) {
       cout << i.data1 << " " << i.data2 << "\t";
    }
    my_stl::swap(my_list_2, my_list_3);
    cout << endl;for (auto &i : my_list_2) {
       cout << i.data1 << " " << i.data2 << "\t";
    }
    cout << endl;
    //my_list_3.sort();
    for (auto &i : my_list_3) {
       cout << i.data1 << " " << i.data2 << "\t";
    }
    cout << endl;
    cout << (my_list_2 < my_list_3) << " " << (my_list_2 > my_list_3) << " " << (my_list_2 <= my_list_3) << " " << (my_list_2 >= my_list_3);
    cout << my_list_2.size();
    cout << endl;
    my_list.reverse();
    cout << "reversed ";
    for (auto &i : my_list) {
       cout << i.data1 << " " << i.data2 << "\t";
    }
    cout << endl;
    my_list.sort();
    my_list.merge(std::move(my_list_2), cmp);
    for (auto &i : my_list) {
       cout << i.data1 << " " << i.data2 << "\t";
    }
    my_list.sort();
    my_list.remove_if(p);
    cout << endl;
    cout << "removed ";
    for (auto &i : my_list) {
       cout << i.data1 << " " << i.data2 << "\t";
    }
    // cout << endl;
    // for (auto &i : my_list_2) {
    //     cout << i.data1 << " " << i.data2 << "\t";
    // }
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