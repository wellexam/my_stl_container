#include <iostream>
#include <vector>
#include "my_vector.hpp"

int main() {
    using std::cout;
    using std::endl;
    my_stl::vector<int> s{1,2,3,4,5};
    for (auto const &i : s) {
        cout << i << " ";
    }
    cout << endl;
    cout << s.empty() << "\t";
    cout << s.size() << "\t";
    cout << s.capacity() << "\t";
    s.shrink_to_fit();
    cout << s.capacity() << "\t";
    cout << endl;
    auto end = s.cend() - (++s.cbegin());
    cout << end;
}