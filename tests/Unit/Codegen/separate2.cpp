// RUN: %cxxamp -emit-llvm -S -c %s -o -|%FileCheck %s
extern "C" {
int foo(void) restrict(cpu, amp) {
	return 42;
}
int bar(void) restrict(amp) {
	return 43;
}
}

class baz {
 public:
  int bzzt(void) restrict(cpu) {
    return 44;
  }
  int cho(void) restrict(amp) {
    return 45;
  }
};
int kerker(void) restrict(amp,cpu) {
  baz b1;
  return b1.cho()+b1.bzzt();
}
// CHECK: foo
// CHECK-NOT: bar
// CHECK-NOT: {{define.*cho}}
// CHECK: {{define.*bzzt}} 
