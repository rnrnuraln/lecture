#include <iostream>

using namespace std;

int main()
{
  const int a = 1;
  int b = a;
  int* const p = &b;
  int* q = p;
  const int* r = &a;
}