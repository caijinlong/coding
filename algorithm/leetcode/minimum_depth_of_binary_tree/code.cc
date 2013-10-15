// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 21:35:52
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 13/13 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 60 milli secs
 * Progress: 41/41 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

int MinDeep(TreeNode * root) {
  if (root == NULL) return 0;
  if (root->left == NULL && root->right == NULL ) return 1;

  int min = -1;
  if (root->left != NULL) {
    min = MinDeep(root->left);
  }
  if (root->right != NULL) {
    int t = MinDeep(root->right);
    min = (min == -1) ? t : std::min(t, min);
  }

  return min + 1;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}