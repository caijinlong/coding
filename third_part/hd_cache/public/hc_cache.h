#ifndef HC_CACHE_H_
#define HC_CACHE_H_

#include "cache_base.h"
#include <stdio.h>
#include <fcntl.h>
#include "base/public/basictypes.h"

#define BYTES_PER_MB 1048576 //  1024 * 1024
#define BYTES_PER_KB 1024
#define MAX_FILE_NUM 20000
#define HAVE_NOASSISTANT 0x200
#define HAVE_ASSISTANT 0x100
#define NOT_USE 0x0
#define USING_IN_WRITE 0x1
#define USING_IN_READ 0x2
#define NEED_DELETE 0x3
#define NEED_PHYSICAL_DELETE 0x4
#define HC_NULL 0xFFFFFFFF
#define MAX_FILE_SIZE 2000
#define PATH_SIZE 1000

struct hc_hashtable {
	u_int hashnum;
	u_int *table;
};
// �ڵ�ṹ
struct hc_indexnode {
	// 64λǩ��
	u_int sign1;
	u_int sign2;
	// λ����Ϣ
	u_int file_offset; // �ļ�λ��
	u_int pos_offset; // �ļ��е�λ��
	u_int length; // ����
	// hash chain
	u_int before;
	u_int next;
	// file chain
	u_int left;
	u_int right;
	// ���еĴ���
	u_int click_time;
};
// hc_cache�е��ļ��ṹ
struct hc_file {
	int handle;			// ���
	int status;			// ״̬
	u_int entry;		// ���
	u_int tail; 		// ���ļ��е����һ���ڵ�
	u_int free_size;	// ���е��ļ���С
	int node_num_inlist;// node�б��еĽڵ���
	int thread_in_read;	// ���ڶ����ļ����߳���
	char *buf;			// mmap��ָ��
	time_t est_time;	// ����ʱ��
};
// file list�ṹ
struct hc_filelist {
	u_int file_size;		// ÿ���ļ��Ĵ�С(Bytes)
	int file_num;			// �ļ�����
	int oldest_offset;		// ���ϵ��ļ�λ��
	int newest_offset;		// ���µ��ļ�λ��
	hc_file * files;			// �ļ�����ָ��
};
//  cacheͳ����Ϣ
struct hc_cache_stat_t {
	int add_count;
	int seek_count;
	int hit_count;
	int remove_count;
};
// hc_cache�ṹ
struct hc_cache {   
	char file_name[PATH_SIZE];	// ʹ�õ��ļ���
	u_int capacity;					// �ڵ����
	hc_hashtable hashtable;			// hash��
	hc_indexnode *nodelist;			// �ڵ����
	hc_filelist filelist;			// �ļ��б�
	u_int rs_head;					// ����ջ��
	u_int in_curpos;				// ��ǰλ��
	u_int total_count;				// ����
	int use_mode;					// ʹ��ģʽ
	time_t valid_time;				// ��Чʱ��
	hc_cache_stat_t cache_stat;
	pthread_mutex_t hc_mutex_count;	// ������
};

/*
 *  @brief [create and initialize a hc_cache structure]
 *  @param capacity [number of nodes of the hc_cache]
 *  @param file_size [the size of each file in the hc_cache(unit:MB)]
 *  @param file_num [the number of file the hc_cache uses]
 *  @param use_mode [use mode of the hc_cache]
 *  @param char *file_name [file_name including path of the files]
 *  @param valid_time [timeout for cache files(unit_size:s)]
 *  @return pointer of the hc_cache: NULL for error; others for success
 */ 
hc_cache *hc_create_cache(u_int capacity, int file_size, int file_num,
		int use_mode, char *file_name, time_t valid_time);

/*			  
 *  @brief [destroy and free a hc_cache structure]
 *  @param pcache [pointer of the hc_cache]
 *  @return: this function always returns RT_NOTICE_PROCESSED for success
 */ 
int hc_free_cache(hc_cache *pcache);

/*
 * @brief [remove a item by sign from hc_cache]
 * @param pcache [pointer of the hc_cache]
 * @param sign [pointer of the 64bit sign of the item]
 * @param click_time [output the click times of the removed item if it's not NULL]
 * @return: RT_NOTICE_PROCESSED for success, others for failure
 */
int hc_removeitem(hc_cache *pcache, u_int *sign, int *click_time);

/*
 * @brief [add a item to hc_cache]
 * @param pcache [pointer of the hc_cache]
 * @param sign [pointer of the 64bit sign of the item]
 * @param pdata [pointer of the data of the item]
 * @param size [the size of item, unitsize: bit]
 * @return: RT_NOTICE_PROCESSED for success, others for failure
 */
int hc_additem(hc_cache *pcache, u_int *sign, void *pdata, u_int size);

/*
 * @brief [read a item from hc_cache]
 * @param pcache [pointer of the hc_cache]
 * @param sign [pointer of the 64bit sign of the item]
 * @param pdata [pointer of the data of the item]
 * @param size [the size of item, unitsize: bit]
 * @param fact_size [output the fact size of the seeked item if fact_size != NULL]
 * @param click_time [output the number of the item seekd if click_time != NULL]
 * @return: RT_NOTICE_PROCESSED for success, others for failure
 */
int hc_seekitem(hc_cache *pcache, u_int *sign, 
		void *pdata, u_int size, u_int *fact_size, int *click_time);

/*
 * @brief [seek item from hc_cache and only return info]
 * @param pcache [pointer of the hc_cache]
 * @param sign [pointer of the 64bit sign of the item]
 * @param size [the size of item, unitsize: bit]
 * @param fact_size [output the fact size of the seeked item]
 * @param file_offset [output the offset of the files that the item saved]
 * @param pos_offset [output the offset of the item in the file]
 * @param click_time [output the number of the item seekd if click_time != NULL]
 * @return: RT_NOTICE_PROCESSED for success, others for failure
 */
int hc_seekitem_r(hc_cache *pcache, u_int *sign, u_int size, 
		u_int *fact_size, u_int *handle, u_int *offset, int *click_time);

/*
 *  @brief [reset click time of the node assigned by sign]
 *  @param pcache [pointer of the hc_cache]
 *  @param sign [pointer of the 64bit sign of the item]
 *  return RT_NOTICE_PROCESSED for success, others for failure
 */
int hc_reset_click_time(hc_cache *pcache, u_int *sign);

/*
 * @brief [modify a item that in hc_cache]
 * @param pcache [pointer of the hc_cache]
 * @param sign [pointer of the 64bit sign of the item]
 * @param pdata [pointer of the data of the item]
 * @param size [the size of item, unitsize: bit]
 * @return: RT_NOTICE_PROCESSED for success, others for failure
 */
int hc_modifyitem(hc_cache *pcache, u_int *sign, void *pdata, u_int size);

/*
 * @brief [clear the hc_cache]
 * @param pcache [pointer of the hc_cache]
 * @return: this function always returns RT_NOTICE_PROCESSED for success
 */
int hc_clean_cache(hc_cache *pcache);

/**
 * @brief dump cache to file
 * @param[in] pcache    pointer to cache
 * @param[in] file_name  file name
 * @return   
 *  @li @c RT_NOTICE_PROCESSED        on success
 *  @li @c RT_NOTICE_NONE_PROCESSED   on failure
 */
int hc_dump_cache(hc_cache* pcache, char *file_name);

/**
 * load cache from file
 * @param file_name     file name
 * @param max_interval   cache expire time
 * @return   
 *  @li a pointer to the cache  on success
 *  @li @c NULL                 on failure
 */
hc_cache* hc_load_cache(char *file_name, time_t max_interval);

/**
 * remove expired files. should be called periodically.
 * @param hc_cache      pointer to cache
 * @param expired_file_name_buf[out] buffer holding the name of expired file
 * @param buf_len		length of buffer
 * @return
 *  @li RT_NOTICE_PROCESSED if any file removed
 *  @li RT_NOTICE_NONE_PROCESSED if no file removed
 * @note this function DO NOT physically unlink the file from file system. 
 *	The name of expired file to be deleted is returned instead so that the caller 
 *	can unlink the file without holding the mutex.
 */
int hc_cache_clean_expired_files(hc_cache *cache, int *expired_file_offset);
#endif
