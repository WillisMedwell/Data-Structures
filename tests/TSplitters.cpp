#include "Splitters.hpp"

#include <gtest/gtest.h>
#include <vector>

TEST(SplitByElementTest, Basic)
{
	std::vector<int> nums = { 1, 2, 3 };
	int i = 0;
	for (auto split : SplitByElement{ nums, 2 })
	{
		if (i == 0)
		{
			EXPECT_EQ(split.size(), 1);
			EXPECT_EQ(split.front(), 1);
			EXPECT_EQ(split.data(), nums.data());
		}
		else if (i == 1)
		{
			EXPECT_EQ(split.size(), 1);
			EXPECT_EQ(split.front(), 3);
			EXPECT_EQ(split.data(), &nums[2]);
		}
		else {
			EXPECT_FALSE(true);
		}
		i++;
	}
}

TEST(SplitByElementTest, StartDelim)
{
	std::vector<int> nums = { 2, 1, 2, 3 };
	int i = 0;
	for (auto split : SplitByElement{ nums, 2 })
	{
		if (i == 0)
		{
			EXPECT_EQ(split.size(), 1);
			EXPECT_EQ(split.front(), 1);
		}
		else if (i == 1)
		{
			EXPECT_EQ(split.size(), 1);
			EXPECT_EQ(split.front(), 3);
		}
		else {
			EXPECT_FALSE(true);
		}
		i++;
	}
}

TEST(SplitByElementTest, EndDelim)
{
	std::vector<int> nums = { 1, 2, 3, 2 };
	int i = 0;
	for (auto split : SplitByElement{ nums, 2 })
	{
		if (i == 0)
		{
			EXPECT_EQ(split.size(), 1);
			EXPECT_EQ(split.front(), 1);
		}
		else if (i == 1)
		{
			EXPECT_EQ(split.size(), 1);
			EXPECT_EQ(split.front(), 3);
		}
		else {
			EXPECT_FALSE(true);
		}
		i++;
	}
}

TEST(SplitByElementTest, MultiDelim)
{
	std::vector<int> nums = { 2, 2, 1, 2, 2, 2, 3, 2, 2, 2 };
	int i = 0;
	for (const auto split : SplitByElement{ nums, 2 })
	{
		if (i == 0)
		{
			EXPECT_EQ(split.size(), 1);
			EXPECT_EQ(split.front(), 1);
		}
		else if (i == 1)
		{
			EXPECT_EQ(split.size(), 1);
			EXPECT_EQ(split.front(), 3);
		}
		else {
			EXPECT_FALSE(true);
		}
		i++;
	}
}

TEST(SplitByElementTest, Indexing)
{
	std::vector<int> nums = { 2, 2, 1, 2, 2, 2, 3, 4, 2, 2, 2 };
	int i = 0;

	auto splitter = SplitByElement{ nums, 2 };
	EXPECT_EQ(splitter[0].size(), 1);
	EXPECT_EQ(splitter[0].front(), 1);
	EXPECT_EQ(splitter[1].size(), 2);
	EXPECT_EQ(splitter[1].front(), 3);
}

TEST(SplitByElementTest, StringView)
{
	auto sentence = std::string_view{"I went for a bike ride yesterday"};
	auto split_words = SplitByElement(sentence, ' ');
	auto words = std::vector<std::string_view>{"I", "went", "for", "a", "bike", "ride", "yesterday"};

	for(size_t i = 0; i < words.size(); ++i) {
		EXPECT_TRUE(std::ranges::equal(words[0], split_words[0]));
	}
}