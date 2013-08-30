// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yanliangcui@yunrang.com (Yanliang Cui)
// 整取零还的定长内存池
// 使用环境：
// 1, 一次分配大块内存,使用者分区域分别释放
// 2, 单线程分配，多线程释放
//

#ifndef UTIL_YMEMPOOL_SAMF_MEMPOOL_SAMF_MEMPOOL_H_
#define UTIL_YMEMPOOL_SAMF_MEMPOOL_SAMF_MEMPOOL_H_

#include <vector>
#include <deque>
#include <utility>
#include "base/public/logging.h"
#include "../public/flag_set.h"

namespace util {
namespace mempool {

class SamfMempool {
 public:
  // item_size:record大小,当为2的幂时free速度会提高一倍,建议总是将其设置成2的幂
  // item_per_block：每次fetch时返回的内存含有的record数目
  // fast_and_wasted:当为true时,会速度更快但浪费内存
  // block_per_alloc:内存池内部每次申请的block数
  SamfMempool(size_t item_size,
              int item_per_blcok,
              bool fast_and_wasted = true,
              int block_per_alloc = kDftBlockPerAlloc);

  virtual ~SamfMempool();

  void* FetchBlock();
  void  FreeItem(void* item);
  void  FreeItems(void* item, int num_item);
  void  FreeBlock(void* block);

 private:
  typedef std::deque<std::pair<FlagSet, char*> > BlockInfoDeque;
  
  struct BlockHeader {
    int magic_num;
    std::pair<FlagSet, char*>* block_info;
  };

  static const int kBlockHeaderSize = sizeof(BlockHeader);
  static const int kSamfMagic = 952712580;
  static const int kDftBlockPerAlloc = 100;

  BlockHeader* GetBlockHeader(char* block) {
    return reinterpret_cast<BlockHeader*>(block);
  }

  void AllocNewBlocks();
  bool RecycleBlocks();

 private:
  size_t item_size_;
  int item_per_block_;
  int block_per_alloc_;
  uint64 mask_;
  bool fast_and_wasted_;
  bool itemsize_is_2exp_;
  int itemsize_2exp_;

  size_t attach_block_size_;
  size_t size_per_alloc_;
  BlockInfoDeque block_info_;
  std::vector<char*> blocks_;
  std::vector<char*> raw_mems_;
};

}  // namespace ymempool
}  // namespace util

#endif  // UTIL_YMEMPOOL_SAMF_MEMPOOL_SAMF_MEMPOOL_H_