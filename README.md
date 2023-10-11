![Test Badge](https://github.com/WillisMedwell/Data-Structures/actions/workflows/test.yml/badge.svg) ![Benchmark Badge](https://github.com/WillisMedwell/Data-Structures/actions/workflows/benchmark.yml/badge.svg)
# Useful Data Structures.
All are header implementations.

## Sov (Structure of Vectors)
Like a structure of arrays but with reallocation like std::vector. All memory is in one block making it very cache friendly. Heavy use of compile time hackery...
Requires C++20.

A Live comparison between an *object orientated* **std::vector** and *data orientated* **Sov** [here](https://willismedwell.github.io/Data-Structures/).

### Usage

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

## Functors
Honestly cannot believe the std doesn't support these. Literally blows my mind.

### Usage

```cpp
std::vector<int> range = { 1, 2, 3, 4, 5, 6};

std::ranges::find_if(range, IsEqualTo(4));
std::ranges::find_if(range, IsNotEqualTo(1));
std::ranges::any_of(range, IsGreaterThan(5));
std::ranges::all_of(range, IsLessThan(1));
```

### SplitByElement
Takes a contiguous range and returns spans into that range with no copying. 
- Range based loops = lazily evaluated.
- Indexing = on first index, are evaluated.

### Usage
```c++
std::string_view sentence = " I went for a run yesterday ";
for(std::span<const char> word_data : SplitByElement{sentence, ' ' })
{
    std::string_view word { word_data.begin(), word_data.end() };
    std::cout << word << ',';
}
// "I,went,for,a,run,yesterday,"
```

```c++
std::vector<int> nums = { 2, 1, 2, 2, 3, 4, 2 };

num_grouping = SplitByElement{nums, 2};

std::span<int> num_group0 = num_grouping[0];
// [ 1 ]
std::span<int> num_group1 = num_grouping[1];
// [ 3, 4 ]
```
