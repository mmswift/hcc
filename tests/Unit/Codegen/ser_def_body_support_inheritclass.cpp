// RUN: %cxxamp -emit-llvm -S -c %s -o -|%cppfilt|%FileCheck %s
// RUN: %gtest_amp %s -DUSING_GTEST=1 -o %t && %t
// XFAIL: *
#include <cstdlib> //for size_t
#include <iostream>
//Serialization object decl
namespace hc {
class Serialize {
 public:
  Serialize():x(0) {}
  void Append(size_t sz, const void *s) {
    x+=1;
  }
  int x;
};
template<typename T>
class gmac_array {
 public:
  __attribute__((annotate("serialize")))/* For compiler */
  void __cxxamp_serialize(Serialize& s) const {
    s.Append(0, NULL);
  }
  T t;
};
}
class base {
 public:
  __attribute__((annotate("serialize")))/* For compiler */
  void __cxxamp_serialize(hc::Serialize& s) const;
 private:
  hc::gmac_array<float> a;
  int i;
};
class derive:public base {
 public:
  __attribute__((annotate("serialize")))/* For compiler */
  void __cxxamp_serialize(hc::Serialize& s) const;
 private:
  float f;
  hc::gmac_array<float> b;
};

int kerker(void) [[cpu, hc]] {
  derive b1;
  hc::Serialize s;
  b1.__cxxamp_serialize(s);
  return 1;
}

// The definition should be generated by clang
// CHECK: define {{.*}}derive::__cxxamp_serialize
// CHECK: call {{.*}}void @base::__cxxamp_serialize(hc::Serialize&) const
// CHECK: }

#ifdef USING_GTEST
// Executable tests
#include <gtest/gtest.h>
TEST(Serialization, Call) {
  derive bl;
  hc::Serialize s;
  bl.__cxxamp_serialize(s);
  EXPECT_EQ(4, s.x);
}
#endif
