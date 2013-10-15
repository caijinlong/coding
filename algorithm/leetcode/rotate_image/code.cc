// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-25 20:47:53
// File  : code.cc
// Brief :
/*
 *Run Status: Accepted!
 Program Runtime: 8 milli secs
 Progress: 3/3 test cases passed.
 Run Status: Accepted!
 Program Runtime: 28 milli secs
 Progress: 20/20 test cases passed.
 *
 * */

#include <vector>
#include <iostream>
#include "base/public/logging.h"


namespace algorithm {

void RotateSub(std::vector<std::vector<int> > & v, int idx, int n) {
  LOG(INFO) << "n:" << n;
  if (n == 1) return;
  int size = n - 1;
  for (int i = 0; i < n - 1; i++) {  // Note from 0 to n-2
    // LOG(INFO) << "n:" << n << " i :" << i << " idx:" << idx;
    int foo = v[idx + i][idx];
    v[idx + i][idx] = v[idx + size][idx + i];
    v[idx + size][idx + i] = v[idx + size - i][idx + size];
    v[idx + size - i][idx + size] = v[idx][idx + size - i];
    v[idx][idx + size - i] = foo;
  }
}

void Rotate(std::vector<std::vector<int> > & v) {
  int n = v.size();
  for (int i = 0; i < v.size() / 2; i++) {
     RotateSub(v, i, n);
     n -= 2;
  }
}

void Out(std::vector<std::vector<int> > & v) {
  for (int i = 0; i < v.size(); i++) {
    for (int j = 0; j < v.size(); j++) {
      std::cout << v[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

}  // namesapce algorithm


// http://hawstein.com/posts/1.6.html

/*
 * 顺时针旋转90度方法：1. 交换主对角线元素
 *                     2. 交换第j列与第n-1-j列的元素
 * 
 * 逆时针旋转90度方法：1. 交换主对角线元素
 *                     2. 交换第i行与第n-1-i行的元素
 *
 * */

namespace NB {
using namespace std;
// clockwise
void Rotate(vector<vector<int> > &matrix) {
  int n = matrix.size();
  for (int i = 0; i < n/2; ++i) {
    for (int j = i; j < n-1-i; ++j){
      int t = matrix[i][j];
      matrix[i][j] = matrix[n-1-j][i];
      matrix[n-1-j][i] = matrix[n-1-i][n-1-j];
      matrix[n-1-i][n-1-j] = matrix[j][n-1-i];
      matrix[j][n-1-i] = t;
    }
  }
}

// unclockwise
void RotateR(std::vector<std::vector<int> > & v) {
  for (int i = 0; i < v.size(); i++) {
    for (int j = i + 1; j < v.size(); j++) {
      std::swap(v[i][j], v[j][i]);
    }
  }
  for (int i = 0; i < v.size() / 2; i++) {
    for (int j = 0; j < v.size(); j++) {
      std::swap(v[i][j], v[v.size() - 1 - i][j]);
    }
  }
}

}  // namespce NB

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::vector<int> > matrix;
  std::vector<int> foo;
  int n = 6;
  for (int i = 0; i < n; i++) {
    foo.push_back(i);
  }
  for (int i = 0; i < n; i++) {
    matrix.push_back(foo);
  }
  Out(matrix);
  HERE(INFO);
  NB::RotateR(matrix);
  Out(matrix);
  return 0;
}