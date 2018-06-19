#include <stdio.h>
#include "structs_test.h"

int main()
{
  char str[10] = {'A', 0, 0, 0, 0, 'B', 0, 0, 0, 0};

  struct Test* ptr = str;
  //без директивы __attribute__ ожидаем, что компилятор сделает выравнивание, и разместит int B сместив тот на 4 байта.
  printf("Withou using __attribute__ :\n");
  printf("In structure field A stored: %c (ASCI: %d)\nin structure field B stored: %c (ASCI: %d)\n\n",ptr->A, ptr->A, ptr->B, ptr->B );
  //ptr = NULL;
  //с директивой все пойдет друг за другом, следовательно в int B ожидаем 0.
  struct TestWithAttribute* ptr_1 = str;
  printf("With __attribute__ :\n");
  printf("In structure field A stored: %c (ASCI: %d)\nin structure field B stored: %c (ASCI: %d)\n\n",ptr_1->A, ptr_1->A, ptr_1->B, ptr_1->B );
  //сместив ptr на 1 получим ASCI код символа B, а в случае с ptr_1(для варианта с директивой) сместим его на 4.
  printf("Just shift ptr in test structure and print field B: ");
  ptr = str + 1;
  printf("%d\n", ptr->B);

  printf("Just shift ptr_1 in test with attribute structure and print field B: ");
  ptr_1 = str+4;
  printf("%d\n", ptr_1->B);
  //с использованием битовых полей заполним наше "пространство" из нулей, организовав смещение на 4 байта.
  printf("\nArtificial filling of zeros by means of bit fields\n");
  struct TestWithBitFeild* ptr_2 = str;
  printf("%d = %c \n%d = %c\n", ptr_2->A, ptr_2->A, ptr_2->B, ptr_2->B);

  return 0;
}