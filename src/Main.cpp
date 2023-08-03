#include "Map.hpp"
#include "Sov.hpp"
#include <numeric>
#include <ranges>
#include <string>

int main()
{
    Map<char, int> m;
    m.insert('a', 0);
    m.insert('b', 2);

    Sov<std::string, int, int> sov(10);

    sov.pushBack(0, 0, 5);
    sov.pushBack(1, 0, 4);
    sov.pushBack(2, 0, 3);
    sov.pushBack(3, 0, 2);
    sov.pushBack(4, 0, 1);


    

    for (auto e : sov.field<0>()) {
        std::cout << e << ' ';
    }
    std::cout << '\n';
    for (auto e : sov.field<1>()) {
        std::cout << e << ' ';
    }
    std::cout << '\n';
    for (auto e : sov.field<2>()) {
        std::cout << e << ' ';
    }

    auto [f, s, t] = sov[2];

    std::cout << f << " " << s << " " << t << '\n';

    for(auto [a, b, c] : sov)
    {
        std::cout << a << " " << b << " " << c << ", ";
    }

    return 0;
}