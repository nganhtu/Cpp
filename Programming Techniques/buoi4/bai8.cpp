// #include <iostream>
// #include <map>

// using namespace std;

template <class T>
map<T, double> fuzzy_set_union(const map<T, double> &a, const map<T, double> &b)
{
    map<T, double> res = a;
    for (pair<T, double> i : b)
    {
        if (res.count(i.first))
        {
            res[i.first] = max(res.find(i.first)->second, i.second);
        }
        else
        {
            res.insert(i);
        }
    }

    return res;
}

template <class T>
map<T, double> fuzzy_set_intersection(const map<T, double> &a, const map<T, double> &b)
{
    map<T, double> res;
    for (pair<T, double> i : a)
    {
        if (b.count(i.first))
        {
            res.insert({i.first, min(i.second, b.find(i.first)->second)});
        }
    }

    return res;
}

// template<class T>
// void print_fuzzy_set(const std::map<T, double> &a) {
//     cout << "{ ";
//     for (const auto &x : a) {
//         std::cout << "(" << x.first << ", " << x.second << ") ";
//     }
//     cout << "}";
//     std::cout << std::endl;
// }

// int main() {
//     std::map<int, double> a = {{1, 0.2}, {2, 0.5}, {3, 1}, {4, 0.6}, {5, 0.7}};
//     std::map<int, double> b = {{1, 0.5}, {2, 0.4}, {4, 0.9}, {5, 0.4}, {6, 1}};
//     std::cout << "A = "; print_fuzzy_set(a);
//     std::cout << "B = "; print_fuzzy_set(b);
//     std::map<int, double> c = fuzzy_set_union(a, b);
//     std::map<int, double> d = fuzzy_set_intersection(a, b);
//     std::cout << "Union: "; print_fuzzy_set(c);
//     std::cout << "Intersection: "; print_fuzzy_set(d);
// }
