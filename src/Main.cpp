#include "Map.hpp"
#include "Sov.hpp"
#include "Ecs.hpp"

#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

auto demoSov() -> void;

#include <vector>

struct Person
{
    std::string name;
};

struct Moving
{
    std::pair<float, float> position;
    std::pair<float, float> velocity;
};

struct MovingPerson
{
    std::string name;
    std::pair<float, float> position;
    std::pair<float, float> velocity;
};

int main()
{
    demoSov();

    auto p = EcsMetaprogramming::getCompositonOf<Person>();
    auto d = EcsMetaprogramming::getCompositonOf<Moving>();
    auto mp = EcsMetaprogramming::getCompositonOf<MovingPerson>();

    Ecs<Person, Moving, MovingPerson> ecs;

    auto thing = ecs.data;

    ecs.add(Person{});
    ecs.add(Moving{});
    ecs.add(MovingPerson{});


    return 0;
}





auto demoSov() -> void
{
    // can cause werid alignment which is handled.
    Sov<uint8_t, std::string, uint16_t> sov(1);

    sov.pushBack(0, "first", 1);
    sov.pushBack(0, "second", 2);

    // individual field access
    std::cout << "uint8_t[]\t= ";
    for (auto e : sov.field<0>()) {
        std::cout << (int)e << ' ';
    }

    std::cout << '\n'
              << "std::string[]\t= ";
    for (auto e : sov.field<1>()) {
        std::cout << e << ' ';
    }
    std::cout << '\n'
              << "uint16_t[]\t= ";
    for (auto e : sov.field<2>()) {
        std::cout << e << ' ';
    }
    std::cout << "\n\n";

    // reference to elements
    auto [field_1, field_2, field_3] = sov.at(1);

    field_1 = 11;
    field_2 = std::move(std::string("Hi"));
    field_3 = 0;

    // range based loops
    for (auto [a, b, c] : sov) {
        std::cout << "(" << (int)a << ", " << b << ", " << (int)c << ")\n";
    }
}