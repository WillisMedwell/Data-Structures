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


    std::cout << alignof(std::string) <<'\n';
    std::cout << alignof(int) <<'\n';
    std::cout << alignof(uint16_t) << std::endl;

    std::cout << sizeof(std::string) <<'\n';
    std::cout << sizeof(int) <<'\n';
    std::cout << sizeof(uint16_t) << std::endl;


    Sov<uint8_t, std::string, int, uint16_t> sov(10);

    sov.pushBack(0, "first", 100, 1);
    sov.pushBack(0, "second", 200, 2);
    sov.pushBack(0, "third", 300, 3);
    sov.pushBack(0, "fourth", 400, 4);
    sov.pushBack(0, "fifth", 500, 5);

    // element access
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

    // reference to elements
    auto [a, f, s, t] = sov[2];

    // range based loops
    for(auto [a, b, c, d] : sov)
    {
        std::cout << a << " " << b << " " << (int)c << ", ";
    }

    return 0;
}