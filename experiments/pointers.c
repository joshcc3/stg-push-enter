#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


bool member(int array_ptr[], int value) {

    return false;
}

void example_inspecting_lvalue() {
   int *arr = malloc(sizeof(int)*10);
   printf("This is an lvalue: %d", *arr);
}

void example_inspecting_ptrs() {
  int (*p2)[3];

  p2 = malloc(sizeof(int)*3);

  printf("Values inside p: p2 = %p, *p2 = %p, *p2[0] = %p, p2[0] = %p, p2[0][0] = %p, p2[0][1] = %p, *p2[1] = %p, p2[1][0] = %p, p2[1][1] = %p\n", \
         p2, (*p2), &(*p2)[0], p2[0], &p2[0][0], &p2[0][1], &(*p2)[1], &p2[1][0], &p2[1][0]);


}


void example_inspecting_refs() {

    int v = 10;
    int *p = &v;
//    int &r1;
    int *&p2;  //  p2 is a reference to a pointer   to an int.

//    int &r1 = &v;
//    int &r2 = &p;
//
//    printf("Refs: p=%p, r1=%p, r2=%p\n", p, r1, r2);

}


int example_assigning_ptrs() {

  int (*p2)[3];
  int p3[3][3];
  p3[0][0] = 1001;
  p2 = malloc(sizeof(int)*9);
  // Does not work - you must use constant expressions to initialize arrays, (what is a constant - I have not formal defn yet)
  // int p4[3] = *p2;


  p2[0][0] = 10;
  printf("1. Should be 10, Value at p2[0][0]%d\n", p2[0][0]);

  *p2[0] = 11;
  printf("2. Should be 11, Value at p2[0][0]%d\n", p2[0][0]);

  // Does not work - you cannot assign to an array (you can assign any other lvalue)
//  *p2 = p3[0];
//  printf("3. Should be 1001, Value at p2[0][0]%d\n", p2[0][0]);

  // Does not work for the same reason above
//  *p2 = p4;
//  printf("4. Should be 11, Value at p2[0][0]%d\n", p2[0][0]);

  *p2[0] = 12;
  printf("4. Should be 12, Value at p2[0][0]%d\n", p2[0][0]);


}

