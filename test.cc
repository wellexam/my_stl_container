#include "my_list.hpp"
#include <iostream>

int main() {
    using std::cout;
    using std::endl;
    list<int> my_list_1(1, 0);
    list<bool> my_list_2(6, true);
    auto iter1 = my_list_1.end();
    for (int i = 1; i < 5; ++i) {
        auto temp = my_list_1.insert(iter1, i);
        cout << *temp << endl;
    }
    auto iter = my_list_1.begin();
    while (iter != my_list_1.end()) {
        cout << *iter << endl;
        ++iter;
    }
    my_list_1.clear();
    iter = my_list_1.begin();
    while (iter != my_list_1.end()) {
        cout << *iter << endl;
        ++iter;
    }
    //auto iter2 = my_list_1.begin();
    //auto iter3 = my_list_1.cbegin();
    //auto iter4 = my_list_1.cend();
    //// auto iter5 = my_list_1.rbegin();
    //++iter1;
    //cout << *iter1 << endl;
    //iter1++;
    //cout << *iter1 << endl;
    //++iter2;
    //cout << *iter2 << endl;
    //iter2++;
    //cout << *iter2 << endl;
    //++iter3;
    //cout << *iter3 << endl;
    //iter3++;
    //cout << *iter3 << endl;
    //++iter4;
    //cout << *iter4 << endl;
    //iter4++;
    //cout << *iter4 << endl;
    //auto temp = iter1++;
    //cout << (temp == iter1) << endl;
    //my_list_1.clear();
    //auto iter5 = my_list_1.begin();
    //auto iter6 = my_list_1.end();
    //cout << (iter5 == iter6) << endl;
}