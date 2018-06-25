#ifndef STRUCT_H
#define STRUCT_H

struct Test {
  char A;
  int B;
};

struct TestWithAttribute {
  char A;
  int B;
}__attribute__((packed));

struct TestWithBitFeild {
  char A;
  int space: 32;
  int B;
}__attribute__((packed));

#endif

