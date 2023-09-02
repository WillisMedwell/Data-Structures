#include <Sov.hpp>
#include <gtest/gtest.h>


namespace TSov {
thread_local size_t element_constructions = 0;
thread_local size_t element_destructions = 0;

struct Element
 {
    int64_t dummy;
    bool has_been_destroyed = false;
    Element(int arg = 0)
    {
        dummy = arg;
        ++element_constructions;
    }
    Element(const Element& other)
    {
        dummy = other.dummy;
        ++element_constructions;
    }
    Element(Element&& other)
    {
        dummy = other.dummy;
        ++element_constructions;
    }
    ~Element()
    {
        if (has_been_destroyed) {
            throw std::runtime_error("destructor called twice");
        }
        has_been_destroyed = true;
        ++element_destructions;
    }
};
}
TEST(SovTest, Construction)
{
    Sov<int> sov(2);
    EXPECT_EQ(sov.size(), 0);
}
TEST(SovTest, PushConstRef)
{
    using namespace TSov;
    Sov<Element> sov(4);
    Element e0(0);
    Element e1(1);
    Element e2(2);
    Element e3(3);
    sov.pushBack(e0);
    sov.pushBack(e1);
    sov.pushBack(e2);
    sov.pushBack(e3);
    EXPECT_EQ(sov.field<0>()[0].dummy, 0);
    EXPECT_EQ(sov.field<0>()[1].dummy, 1);
    EXPECT_EQ(sov.field<0>()[2].dummy, 2);
    EXPECT_EQ(sov.field<0>()[3].dummy, 3);
}
TEST(SovTest, PushMove)
{
    using namespace TSov;
    Sov<Element> sov(4);
    sov.pushBack(Element { 0 });
    sov.pushBack(Element { 1 });
    sov.pushBack(Element { 2 });
    sov.pushBack(Element { 3 });
    EXPECT_EQ(sov.field<0>()[0].dummy, 0);
    EXPECT_EQ(sov.field<0>()[1].dummy, 1);
    EXPECT_EQ(sov.field<0>()[2].dummy, 2);
    EXPECT_EQ(sov.field<0>()[3].dummy, 3);
}
TEST(SovTest, PushConstRefWGrow)
{
    using namespace TSov;
    Sov<Element> sov(2);
    Element e0(0);
    Element e1(1);
    Element e2(2);
    Element e3(3);
    sov.pushBack(e0);
    sov.pushBack(e1);
    sov.pushBack(e2);
    sov.pushBack(e3);
    EXPECT_EQ(sov.field<0>()[0].dummy, 0);
    EXPECT_EQ(sov.field<0>()[1].dummy, 1);
    EXPECT_EQ(sov.field<0>()[2].dummy, 2);
    EXPECT_EQ(sov.field<0>()[3].dummy, 3);
}
TEST(SovTest, PushMoveWGrow)
{
    using namespace TSov;
    Sov<Element> sov(2);
    sov.pushBack(Element { 0 });
    sov.pushBack(Element { 1 });
    sov.pushBack(Element { 2 });
    sov.pushBack(Element { 3 });
    EXPECT_EQ(sov.field<0>()[0].dummy, 0);
    EXPECT_EQ(sov.field<0>()[1].dummy, 1);
    EXPECT_EQ(sov.field<0>()[2].dummy, 2);
    EXPECT_EQ(sov.field<0>()[3].dummy, 3);
}
TEST(SovTest, ProperElementConstruction)
{
    using namespace TSov;
    element_constructions = 0;
    element_destructions = 0;
    {
        Sov<Element> sov(2);
        Element e;
        sov.pushBack(e);
        sov.pushBack(Element {});
    }
    EXPECT_EQ(element_constructions, element_destructions);
}
TEST(SovTest, ProperElementConstructionWGrow)
{
    using namespace TSov;
    element_constructions = 0;
    element_destructions = 0;
    {
        Sov<Element> sov(2);
        Element e;
        sov.pushBack(e);
        sov.pushBack(Element {});
        sov.pushBack(e);
        sov.pushBack(Element {});
    }
    EXPECT_EQ(element_constructions, element_destructions);
}
TEST(SovTest, ElementAccess)
{
    Sov<uint8_t, std::string, uint16_t> sov(1);
    sov.pushBack(0, "first", 1);
    sov.pushBack(0, "second", 2);
    auto [a, b, c] = sov[0];
    EXPECT_EQ(a, 0);
    EXPECT_EQ(b, "first");
    EXPECT_EQ(c, 1);

    auto [d, e, f] = sov[1];
    EXPECT_EQ(d, 0);
    EXPECT_EQ(e, "second");
    EXPECT_EQ(f, 2);

    EXPECT_THROW(sov.at(2), std::out_of_range);
}
TEST(SovTest, StringElements)
{
    Sov<std::string> sov(1);

    std::string c = "cccccccccccccccccccc";
    std::string d = "dddddddddddddddddddd";

    sov.pushBack("aaaaaaaaaaaaaaaaaaaa");
    sov.pushBack("bbbbbbbbbbbbbbbbbbbb");
    sov.pushBack(c);
    sov.pushBack(d);

    c = "1";
    d = "2";

    EXPECT_EQ(sov.field<0>()[0], "aaaaaaaaaaaaaaaaaaaa");
    EXPECT_EQ(sov.field<0>()[1], "bbbbbbbbbbbbbbbbbbbb");
    EXPECT_EQ(sov.field<0>()[2], "cccccccccccccccccccc");
    EXPECT_EQ(sov.field<0>()[3], "dddddddddddddddddddd");
}

TEST(SovTest, hasField)
{
    Sov<std::string, int> sov(1);
    
    constexpr bool has_field = Sov<std::string>::hasField<int>();

    EXPECT_EQ(decltype(sov)::hasField<std::string>(), true);
    EXPECT_EQ(decltype(sov)::hasField<int>(), true);
    EXPECT_EQ(decltype(sov)::hasField<float>(), false);

    //constexpr bool has_string_field = SovHasField<decltype(sov), std::string>::value;
    //constexpr bool has_int_field = SovHasField<decltype(sov), int>::value;
    //constexpr bool has_float_field = SovHasField<decltype(sov), float>::value;

    //EXPECT_EQ(has_string_field, true);
    //EXPECT_EQ(has_int_field, true);
    //EXPECT_EQ(has_float_field, false);
}