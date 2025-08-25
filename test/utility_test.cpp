#include "gtest/gtest.h"
#include <mystl/utility.h>
#include <string>
#include <vector>

// 用于测试 move 语义的结构体
struct Moveable {
  bool moved_from = false;
  std::string data;

  Moveable(std::string d = "") : data(d) {}

  Moveable(const Moveable &) = delete;
  Moveable &operator=(const Moveable &) = delete;

  Moveable(Moveable &&other) noexcept {
    data = mystl::move(other.data);
    other.moved_from = true;
  }

  Moveable &operator=(Moveable &&other) noexcept {
    data = mystl::move(other.data);
    other.moved_from = true;
    return *this;
  }
};

TEST(UtilityTest, move) {
  // 测试左值
  int x = 42;
  int &&rref_x = mystl::move(x);
  EXPECT_EQ(&x, &rref_x);

  // 测试 move 构造
  Moveable a("hello");
  EXPECT_FALSE(a.moved_from);
  Moveable b(mystl::move(a));
  EXPECT_TRUE(a.moved_from);
  EXPECT_EQ(b.data, "hello");

  // 测试 move 赋值
  Moveable c("world");
  EXPECT_FALSE(c.moved_from);
  b = mystl::move(c);
  EXPECT_TRUE(c.moved_from);
  EXPECT_EQ(b.data, "world");
}

TEST(UtilityTest, swap) {
  // 测试基本类型
  int a = 1;
  int b = 2;
  mystl::swap(a, b);
  EXPECT_EQ(a, 2);
  EXPECT_EQ(b, 1);

  // 测试复杂类型
  std::string s1 = "hello";
  std::string s2 = "world";
  mystl::swap(s1, s2);
  EXPECT_EQ(s1, "world");
  EXPECT_EQ(s2, "hello");

  // 测试 STL 容器
  std::vector<int> v1 = {1, 2, 3};
  std::vector<int> v2 = {4, 5, 6};
  mystl::swap(v1, v2);
  EXPECT_EQ(v1, (std::vector<int>{4, 5, 6}));
  EXPECT_EQ(v2, (std::vector<int>{1, 2, 3}));
}