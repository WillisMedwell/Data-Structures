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

int main()
{
    demoSov();

    Ecs2<Component::Components, Player, Ball, Id> ecs2;
    Player p1{{"willis", "computer science"}, {}};
    Player p2{{"will", "housemate"}, {}};
    Ball b1{};
    Id id{{"william", "comp sci"}};
    ecs2.add(p1);
    ecs2.add(p2);
    ecs2.add(b1);
    ecs2.add(id);

    ecs2.printNamable();
    std::cout <<  ecs2.storageLayout() << '\n';

    // can iterate over individual components, or can iterate over Entity types.
    ecs2.begin<Component::Description>();
    ecs2.begin<Player>();
    //ecs2.begin<int>(); // this will fail.

    //auto a = std::views::all(, ecs2.end<Player>()});
    using IteratorType = decltype(ecs2.begin<Player>());
    //auto r = std::ranges::subrange<IteratorType, IteratorType>{ecs2.begin<Player>(), ecs2.end<Player>()};


    //for(const auto [des, kin] : std::ranges::subrange{)
    //{
    //    std::cout << des.name << " " << des.info << '\n';
    //}

    //auto r = std::ranges::to<

    

    for(auto i = ecs2.begin<Player>(); i != ecs2.end<Player>(); ++i)
    {
        const auto [description, kinematics] = *i;
        std::cout << description.name << " " << description.info << '\n';
    }

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