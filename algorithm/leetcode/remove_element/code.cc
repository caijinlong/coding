
#include "base/public/common_head.h"

/*
Given an array and a value, remove all instances of that value in place and return the new length.

The order of elements can be changed. It doesn't matter what you leave beyond the new length.
*/

/*
Run Status: Accepted!
Program Runtime: 8 milli secs
Progress: 22/22 test cases passed.
Run Status: Accepted!
Program Runtime: 40 milli secs
Progress: 112/112 test cases passed.
*/
/*
一定要条理清晰，在不增加时间复杂度的前提下
多写几行代码是有必要的

*/
class Solution {
 public:
  int removeElement(int A[], int n, int elem) {
    int i = 0;
    int j = n - 1;
    while (i < j) {
      while (i < n && A[i] != elem) i++;
        while (j >= 0 && A[j] == elem) j--;
        if (i < j) {
          std::swap(A[i], A[j]);
        }
      }
      for (int i = 0; i < n; i++) {
        if (A[i] == elem) return i;
      }
      return n;
  }
};

namespace twice {
class Solution {
 public:
  int removeElement(int A[], int n, int elem) {
    int k = -1;
    for (int i = 0; i < n; i++) {
      if (A[i] != elem) A[++k] = A[i];
    }
    return k + 1;
  }
};
}  // namespace twice


int main(int argc, char** argv) {
  return 0;
}