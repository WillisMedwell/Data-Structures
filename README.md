![Test Badge](https://github.com/WillisMedwell/Data-Structures/actions/workflows/test.yml/badge.svg) ![Benchmark Badge](https://github.com/WillisMedwell/Data-Structures/actions/workflows/benchmark.yml/badge.svg)
# Data Structures
All are header implementations.

## Sov (Structure of Vectors)
Like a structure of arrays but with reallocation like std::vector. All memory is in one block making it very cache friendly. Heavy use of compile time hackery...
Requires C++20.

A Live comparison between an *object orientated* **std::vector** and *data orientated* **Sov** [here](https://willismedwell.github.io/Data-Structures/).

```cpp
Sov<uint8_t, std::string, uint16_t> sov;

sov.pushBack(0, "first", 1);
sov.pushBack(0, "second", 2);

// get individual field (like using std::get<>)
for (auto e : sov.field<0>()) {
    std::cout << (int)e << ' ';
}
for (auto e : sov.field<uint16_t>()) {
    std::cout << (int)e << ' ';
}

// range based loop (takes it as a reference)
for (const auto& [a, b, c] : sov) {
    std::cout << "(" << (int)a << ", " << b << ", " << (int)c << ")\n";
}

// reference to elements
auto [field_1, field_2, field_3] = sov[2];
```