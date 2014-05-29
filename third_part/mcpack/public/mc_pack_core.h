#ifndef __MC_PACK_CORE_H
#define __MC_PACK_CORE_H

#include <stdlib.h>
#include <sys/types.h>
#include "mc_pack_def.h"

#define MC_PACK_VERSION (0x0000)

/**
 * @brief mc_pack_item_t
 *   ���ڱ����Ľӿ�
 *   �� type Ϊ array/object ʱ��value_size ������� item_count
 *   key Ϊ���֣�value Ϊָ�� array/object ��ָ��
 */

typedef struct _mc_v1pack_t {
	char tag[4];
	u_int item_count;
	u_int bytes_left;
	u_int cur_pos;
	u_int internal_item_count;
	u_short version;
	u_short reserved;
	char data[0];
} __attribute__((packed)) mc_v1pack_t;

/**
 * @brief init a pack
 *		��ָ���Ļ������ϳ�ʼ��һ�� datapack
 * @param [in] buf   : char*
 *		������
 * @param [in] buf_size   : u_int
 *		��������С��Ӧ���ٱ� sizeof(mc_v1pack_t) ��
 * @return  mc_v1pack_t*
 *		ָ�� pack �ṹ��ָ��
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 **/
mc_v1pack_t *mc_v1pack_create(char *buf, u_int buf_size);

/**
 * @brief get pack size
 *		ȡ�� pack �Ĵ�С��ֻ������ pack���������� array/object
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� pack �ṹ��ָ��
 * @return  int
 * @retval errno
 *		- >0 : pack size
 *		- <0 : failed
 **/
int mc_v1pack_get_size(const mc_v1pack_t * ppack);

/**
 * @brief get pack type
 *		ȡ�� pack ����
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� pack �ṹ��ָ��
 * @return  int
 * @retval
 *		- MC_PT_BAD
 *		- MC_PT_OBJ
 *		- MC_PT_ARR
 **/
int mc_v1pack_get_type(const mc_v1pack_t * ppack);

/**
 * @brief get item count of pack
 *		ȡ�� pack �� item count�������� array/object
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� pack �ṹ��ָ��
 * @return  int
 * @retval errno
 *		- >=0 : item count
 *		- <0 : failed
 **/
int mc_v1pack_get_item_count(const mc_v1pack_t * ppack);

/**
 * @brief init a array item in a pack
 *		��һ�� pack �д���һ�������͵� item
 * @param [in] ppack   : mc_v1pack_t*
 *		datapack ָ��
 * @param [in] name   : const char*
 *		��������ppack Ϊ����ʱ name ����Ϊ NULL
 * @return  mc_v1pack_t*
 *		ָ�� array ��ָ��
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 **/
mc_v1pack_t *mc_v1pack_put_array(mc_v1pack_t * ppack, const char *name);

/**
 * @brief get array from pack
 *		�� pack ��ȡ�� array ��ָ��
 * @param [in] ppack   : const mc_v1pack_t*
 *		datapack ָ��
 * @param [in] name   : const char*
 *		������
 * @return  mc_v1pack_t*
 *		ָ�� array ��ָ��
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 **/
mc_v1pack_t *mc_v1pack_get_array(const mc_v1pack_t * ppack, const char *name);

/**
 * @brief get array from array
 *		�� array ��ȡ���� array ��ָ��
 * @param [in] ppack   : const mc_v1pack_t*
 *		array ָ��
 * @param [in] arr_index   : u_int
 * @return  mc_v1pack_t*
 *		ָ�� array ��ָ��
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 **/
mc_v1pack_t *mc_v1pack_get_array_arr(const mc_v1pack_t * ppack, u_int arr_index);

/**
 * @brief init a object item in a pack
 *		��һ�� pack �д���һ���ṹ���͵� item
 * @param [in] ppack   : mc_v1pack_t*
 *		datapack ָ��
 * @param [in] name   : const char*
 *		�ṹ����ppack Ϊ����ʱ name ����Ϊ NULL
 * @return  mc_v1pack_t*
 *		ָ�� object ��ָ��
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 **/
mc_v1pack_t *mc_v1pack_put_object(mc_v1pack_t * ppack, const char *name);

/**
 * @brief get object from pack
 *		�� pack ��ȡ�� object ��ָ��
 * @param [in] ppack   : const mc_v1pack_t*
 *		datapack ָ��
 * @param [in] name   : const char*
 *		�ṹ��
 * @return  mc_v1pack_t*
 *		ָ�� object ��ָ��
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 **/
mc_v1pack_t *mc_v1pack_get_object(const mc_v1pack_t * ppack, const char *name);

/**
 * @brief get object from array
 *		�� array ��ȡ�� object ��ָ��
 * @param [in] ppack   : const mc_v1pack_t*
 *		array ָ��
 * @param [in] arr_index   : u_int
 * @return  mc_v1pack_t*
 *		ָ�� object ��ָ��
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 **/
mc_v1pack_t *mc_v1pack_get_object_arr(const mc_v1pack_t * ppack, u_int arr_index);

/**
 * @brief check if a pointer is valid
 *		�ж�һ��ָ��ָ����Ƿ�Ϊ�Ϸ��� mc_v1pack_t �� array/object mc_pack_item_t
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� pack �� array/object ��ָ��
 * @param [in] buf_size   : u_int
 * @return  int
 * @retval validation
 *		- 1 : valid
 *		- 0 : invalid
 **/
int mc_v1pack_valid(const mc_v1pack_t * ppack, u_int buf_size);

/**
 * @brief add int32 into pack
 * @param [in] ppack   : mc_v1pack_t*
 *		ָ�� pack �� object/array ��ָ��
 * @param [in] name   : const char*
 *		�ֶ�����ppack Ϊ����ʱ name ����Ϊ NULL
 * @param [in] value   : mc_int32_t
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_put_int32(mc_v1pack_t * ppack, const char *name, mc_int32_t value);

/**
 * @brief
 * ����һ��32λ������
 * @param [in/out] ppack   : mc_v1pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] value   : float
 * @return  int
 * @retval
 **/
int mc_v1pack_put_float(mc_v1pack_t * ppack, const char *name, float value);

/**
 * @brief
 * �Ӷ���ȡ��һ��32λ������
 * @param [in/out] ppack   : const mc_v1pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] result   : float*
 * @return  int
 * @retval
 **/
int mc_v1pack_get_float(const mc_v1pack_t * ppack, const char *name, float * result);

/**
 * @brief
 * ��������ȡ��������
 * @param [in/out] ppack   : const mc_v1pack_t*
 * @param [in/out] arr_index   : u_int
 * @param [in/out] result   : float*
 * @return  int
 * @retval
 **/
int mc_v1pack_get_float_arr(const mc_v1pack_t * ppack, u_int arr_index, float * result);

/**
 * @brief
 * ����һ��64λ������
 * @param [in/out] ppack   : mc_v1pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] value   : double
 * @return  int
 * @retval
 **/
int mc_v1pack_put_double(mc_v1pack_t * ppack, const char *name, double value);

/**
 * @brief
 * �Ӷ�����ȡ��64λ������
 * @param [in/out] ppack   : const mc_v1pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] result   : double*
 * @return  int
 * @retval
 **/
int mc_v1pack_get_double(const mc_v1pack_t * ppack, const char *name, double * result);

/**
 * @brief
 * ��������ȡ��������
 * @param [in/out] ppack   : const mc_v1pack_t*
 * @param [in/out] arr_index   : u_int
 * @param [in/out] result   : double*
 * @return  int
 * @retval
 **/
int mc_v1pack_get_double_arr(const mc_v1pack_t * ppack, u_int arr_index, double * result);

/**
 * @brief
 * ����һ��boolֵ��1�ֽڳ���ע���ȡʱ����Ծ�����ֵ��������boolֵ
 * @param [in/out] ppack   : mc_v1pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] value   : mc_bool_t
 * @return  int
 * @retval
 **/
int mc_v1pack_put_bool(mc_v1pack_t * ppack, const char *name, mc_bool_t value);

/**
 * @brief
 * �Ӷ����ж�ȡboolֵ
 * @param [in/out] ppack   : const mc_v1pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] value   : mc_bool_t*
 * @return  int
 * @retval
 **/
int mc_v1pack_get_bool(const mc_v1pack_t * ppack, const char *name, mc_bool_t * value);

/**
 * @brief
 * �������ж�ȡboolֵ
 * @param [in/out] ppack   : const mc_v1pack_t*
 * @param [in/out] arr_index   : u_int
 * @param [in/out] value   : mc_bool_t*
 * @return  int
 * @retval
 **/
int mc_v1pack_get_bool_arr(const mc_v1pack_t * ppack, u_int arr_index, mc_bool_t * value);

/**
 * @brief
 * ����һ����ֵ
 * @param [in/out] ppack   : mc_v1pack_t*
 * @param [in/out] name   : const char*
 * @return  int
 * @retval
 **/
int mc_v1pack_put_null(mc_v1pack_t * ppack, const char *name);

/**
 * @brief
 * �ж�һ��������ֶ��Ƿ�Ϊ��
 * @param [in/out] ppack   : const mc_v1pack_t*
 * @param [in/out] name   : const char*
 * @return  int
 * @retval
 **/
int mc_v1pack_is_null(const mc_v1pack_t * ppack, const char *name);

/**
 * @brief
 * �ж������һ����Ա�Ƿ�Ϊ��
 * @param [in/out] ppack   : const mc_v1pack_t*
 * @param [in/out] arr_index   : u_int
 * @return  int
 * @retval
 **/
int mc_v1pack_is_null_arr(const mc_v1pack_t * ppack, u_int arr_index);

/**
 * @brief
 * ��item��ȡ��floatֵ�����Զ�������ת������doubleתΪfloatʱ���жϷ�Χ
 * @param [in/out] pitem   : const mc_pack_item_t*
 * @param [in/out] res   : float*
 * @return  int
 * @retval
 **/
int mc_v1pack_get_float_from_item(const mc_pack_item_t *pitem, float *res);

/**
 * @brief
 * ��item��ȡ��doubleֵ
 * @param [in/out] pitem   : const mc_pack_item_t*
 * @param [in/out] res   : double*
 * @return  int
 * @retval
 **/
int mc_v1pack_get_double_from_item(const mc_pack_item_t *pitem, double *res);

int mc_v1pack_get_bool_from_item(const mc_pack_item_t *pitem, mc_bool_t *res);
/**
 * @brief add int32 into pack
 * @param [in] ppack   : mc_v1pack_t*
 *		ָ�� pack �� object/array ��ָ��
 * @param [in] name   : const char*
 *		�ֶ�����ppack Ϊ����ʱ name ����Ϊ NULL
 * @param [in] value   : mc_int32_t
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
/**
 * @brief get int32 from pack
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� pack �� object ��ָ��
 * @param [in] name   : const char*
 * @param [out] result   : mc_int32_t*
 *		���
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_get_int32(const mc_v1pack_t * ppack, const char *name, mc_int32_t * result);

/**
 * @brief add uint32 into pack
 *
 * @param [in] ppack   : mc_v1pack_t*
 *		ָ�� pack �� object ��ָ��
 * @param [in] name   : const char*
 * @param [in] value   : mc_uint32_t
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_put_uint32(mc_v1pack_t * ppack, const char *name, mc_uint32_t value);

/**
 * @brief get uint32 from pack
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� pack �� object/array ��ָ��
 * @param [in] name   : const char*
 *		�ֶ�����ppack Ϊ����ʱ name ����Ϊ NULL
 * @param [out] result   : mc_uint32_t*
 *		���
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_get_uint32(const mc_v1pack_t * ppack, const char *name, mc_uint32_t * result);

/**
 * @brief add uint64 into pack
 *
 * @param [in] ppack   : mc_v1pack_t*
 *		ָ�� pack �� object ��ָ��
 * @param [in] name   : const char*
 *		�ֶ�����ppack Ϊ����ʱ name ����Ϊ NULL
 * @param [in] value   : mc_int64_t
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_put_int64(mc_v1pack_t * ppack, const char *name, mc_int64_t value);

/**
 * @brief get int64 from pack
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� pack �� object ��ָ��
 * @param [in] name   : const char*
 * @param [out] result   : mc_int64_t*
 *		���
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_get_int64(const mc_v1pack_t * ppack, const char *name, mc_int64_t * result);

/**
 * @brief add uint64 into pack
 * @param [in] ppack   : mc_v1pack_t*
 *		ָ�� pack �� object/array ��ָ��
 * @param [in] name   : const char*
 *		�ֶ�����ppack Ϊ����ʱ name ����Ϊ NULL
 * @param [in] value   : mc_uint64_t
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_put_uint64(mc_v1pack_t * ppack, const char *name, mc_uint64_t value);

/**
 * @brief get uint64 from pack
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� pack �� object ��ָ��
 * @param [in] name   : const char*
 * @param [out] result   : mc_uint64_t*
 *		���
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_get_uint64(const mc_v1pack_t * ppack, const char *name, mc_uint64_t * result);

/**
 * @brief add a string into pack
 *		�� pack �м����ַ������Ὣ��β�� '\\0' Ҳ����
 * @param [in] ppack   : mc_v1pack_t*
 *		ָ�� pack �� object/array ��ָ��
 * @param [in] name   : const char*
 *		�ֶ�����ppack Ϊ����ʱ name ����Ϊ NULL
 * @param [in] value   : const char*
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_put_str(mc_v1pack_t * ppack, const char *name, const char *value);

/**
 * @brief add a string into pack
 *		�� value ��ǰ n ���ֽڼ��� pack �У����Զ��� '\\0'
 * @param [in] ppack   : mc_v1pack_t*
 *		ָ�� pack �� object/array ��ָ��
 * @param [in] name   : const char*
 *		�ֶ�����ppack Ϊ����ʱ name ����Ϊ NULL
 * @param [in] value   : const char*
 * @param [in] len   : u_int
 *		����
 * @return  int
 * @retval   errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_put_nstr(mc_v1pack_t * ppack, const char *name, const char *value, u_int len);

/**
 * @brief get a string from pack
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� pack �� object ��ָ��
 * @param [in] name   : const char*
 * @return  const char*
 *		ָ�� string ��ָ��
 * @retval  MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 **/
const char *mc_v1pack_get_str(const mc_v1pack_t * ppack, const char *name);

/**
 * @brief get a string from pack with default value
 *		ȡ���ַ��������δ�ҵ�������Ĭ��ֵ
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� pack �� object ��ָ��
 * @param [in] name   : const char*
 * @param [in] def_value   : const char*
 *		Ĭ��ֵ�����δ�ҵ�������def_value
 * @return  const char*
 *		ָ�� string ��ָ��
 * @retval  MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 **/
const char *mc_v1pack_get_str_def(const mc_v1pack_t * ppack, const char *name, const char *def_value);

/**
 * @brief add raw data into pack
 * @param [in] ppack   : mc_v1pack_t*
 *		ָ�� pack �� object/array ��ָ��
 * @param [in] name   : const char*
 *		�ֶ�����ppack Ϊ����ʱ name ����Ϊ NULL
 * @param [in] rawdata   : const void*
 * @param [in] size   : u_int
 *		raw data ��С
 * @return  int
 *		- 0 : succeed
 *		- <0 : failed
 * @retval errno
 **/
int mc_v1pack_put_raw(mc_v1pack_t * ppack, const char *name, const void *rawdata, u_int size);

/**
 * @brief get raw data from pack
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� pack �� object ��ָ��
 * @param [in] name   : const char*
 * @param [out] size   : u_int*
 *		raw data �Ĵ�С
 * @return  const void*
 *		ָ�� raw data ��ָ��
 * @retval   MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 **/
const void *mc_v1pack_get_raw(const mc_v1pack_t * ppack, const char *name, u_int *size);

/**
 * @brief get int32 from array
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� array ��ָ��
 * @param [in] arr_index   : u_int
 * @param [out] result   : mc_int32_t*
 *		���
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_get_int32_arr(const mc_v1pack_t * ppack, u_int arr_index, mc_int32_t * result);

/**
 * @brief get uint32 from array
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� array ��ָ��
 * @param [in] arr_index   : u_int
 * @param [out] result   : mc_uint32_t*
 *		���
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_get_uint32_arr(const mc_v1pack_t * ppack, u_int arr_index, mc_uint32_t * result);

/**
 * @brief get int64 from array
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� array ��ָ��
 * @param [in] arr_index   : u_int
 * @param [out] result   : mc_int64_t*
 *		���
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_get_int64_arr(const mc_v1pack_t * ppack, u_int arr_index, mc_int64_t * result);

/**
 * @brief get uint64 from array
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� array ��ָ��
 * @param [in] arr_index   : u_int
 * @param [out] result   : mc_uint64_t*
 *		���
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_get_uint64_arr(const mc_v1pack_t * ppack, u_int arr_index, mc_uint64_t * result);

/**
 * @brief get string from array
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� array ��ָ��
 * @param [in] arr_index   : u_int
 * @return  const char*
 *		ָ�� string ��ָ��
 * @retval   MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 **/
const char *mc_v1pack_get_str_arr(const mc_v1pack_t * ppack, u_int arr_index);

/**
 * @brief get string from array with default value
 *		�������±�ȡ���ַ��������δ�ҵ�������Ĭ��ֵ
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� array ��ָ��
 * @param [in] arr_index   : u_int
 * @param [in] def_value   : const char*
 *		Ĭ��ֵ�����δ�ҵ�������def_value
 * @return  const char*
 *		ָ�� string ��ָ��
 * @retval  MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 **/
const char *mc_v1pack_get_str_arr_def(const mc_v1pack_t * ppack, u_int arr_index, const char *def_value);

/**
 * @brief get raw data from array
 * @param [in] ppack   : const mc_v1pack_t*
 *		ָ�� array ��ָ��
 * @param [in] arr_index   : u_int
 * @param [out] size   : u_int*
 *		raw data �Ĵ�С
 * @return  const void*
 *		ָ�� raw data ��ָ��
 * @retval  MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 **/
const void *mc_v1pack_get_raw_arr(const mc_v1pack_t * ppack, u_int arr_index, u_int *size);

/**
 * @brief get first item
 * @param [in] ppack   : const mc_v1pack_t*
 * @param [out] pitem   : mc_pack_item_t*
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_first_item(const mc_v1pack_t * ppack, mc_pack_item_t * pitem);

/**
 * @brief get next item
 * @param [in] pitem_now   : const mc_pack_item_t*
 * @param [out] pitem_out   : mc_pack_item_t*
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_next_item(const mc_pack_item_t * pitem_now, mc_pack_item_t * pitem_out);

/**
 * @brief get item by index
 * @param [in] ppack   : const mc_v1pack_t*
 * @param [in] idx   : u_int
 * @param [out] pitem   : mc_pack_item_t*
 * @return  int
 * @retval errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_get_by_idx(const mc_v1pack_t * ppack, u_int idx, mc_pack_item_t * pitem);

/**
 * @brief close a pack
 *		���� pack ǰ������ô˺������� pack ��ʣ��ռ���Ϊ 0
 *
 * @param [in] ppack   : mc_v1pack_t*
 * @return  int
 * @retval  errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_close(mc_v1pack_t * ppack);

/**
 * @brief reopen a pack
 *		���´�һ�� close �˵� pack
 * @param [in] ppack   : mc_v1pack_t*
 * @param [in] buf_size   : u_int
 * @return  int
 * @retval  errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_reopen(mc_v1pack_t * ppack, u_int buf_size);

/**
 * @brief copy item from one pack to another
 *		��һ�� pack �п����������һ�� pack
 * @param [in] src_pack   : const mc_v1pack_t*
 *		������ pack/array/object
 * @param [in] dst_pack   : mc_v1pack_t*
 *		������ pack/array/object�����ͱ����� src_pack һ��
 * @param [in] name   : const char*
 *		���Ϊ NULL���� copy ������Ԫ��
 * @return  int
 * @retval  errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_copy_item(const mc_v1pack_t * src_pack, mc_v1pack_t * dst_pack, const char *name);

/**
 * @brief copy item from one array to another
 * @param [in] src_arr   : const mc_v1pack_t*
 *		source array
 * @param [in] dst_arr   : mc_v1pack_t*
 *		destination array
 * @param [in] idx   : u_int
 *		array index
 * @return  int
 * @retval   errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_copy_item_arr(const mc_v1pack_t * src_arr, mc_v1pack_t * dst_arr, u_int idx);

/**
 * @brief get item by name
 * @param [in] ppack   : const mc_v1pack_t*
 * @param [in] name   : const char*
 * @param [out] pitem   : mc_pack_item_t*
 * @return  int
 * @retval   errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_get_item(const mc_v1pack_t * ppack, const char *name, mc_pack_item_t *pitem);

/**
 * @brief get item by array index
 * @param [in] ppack   : const mc_v1pack_t*
 * @param [in] idx   : u_int
 * @param [out] pitem   : mc_pack_item_t*
 * @return  int
 * @retval   errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_get_item_arr(const mc_v1pack_t * ppack, u_int idx, mc_pack_item_t *pitem);

/**
 * @brief reset a pack
 * @param [in] ppack   : mc_v1pack_t*
 * @return  int
 * @retval  errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_reset(mc_v1pack_t *ppack);

/**
 * @brief callback function for mc_v1pack_foreach
 * @param [in] pitem   : const mc_pack_item_t*
 *		one item in pack
 * @param [in] arg   : void*
 *		argument for callback
 * @return  int
 * @retval
 *		- <0 : stop and return to caller of foreach
 **/
typedef int (*mc_v1pack_foreach_callback)(const mc_pack_item_t *pitem, void *arg);
typedef int (*mc_v1pack_foreach_callback_ex)(const mc_pack_item_t *pitem, void *arg,full_key_t *key);

/**
 * @brief for each item in pack do sth.
 * @param [in] ppack   : const mc_v1pack_t*
 * @param [in] cb   : mc_v1pack_foreach_callback
 *		callback function
 * @param [in] arg   : void*
 *		argument for callback
 * @return  int
 * @retval
 *		- <0 : err returned by cb
 *		- >=0 : sum of retval of cb
 **/
int mc_v1pack_foreach(const mc_v1pack_t *ppack, mc_v1pack_foreach_callback cb, void *arg);
int mc_v1pack_foreach_ex(const mc_v1pack_t *ppack, mc_v1pack_foreach_callback_ex cb, void *arg,full_key_t *key);

/**
 * @brief get int32 from item
 * @param [in] pitem   : const mc_pack_item_t*
 * @param [out] res   : mc_int32_t*
 * @return  int
 * @retval
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_get_int32_from_item(const mc_pack_item_t *pitem, mc_int32_t *res);

/**
 * @brief get uint32 from item
 * @param [in] pitem   : const mc_pack_item_t*
 * @param [out] res   : mc_uint32_t*
 * @return  int
 * @retval
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_get_uint32_from_item(const mc_pack_item_t *pitem, mc_uint32_t *res);

/**
 * @brief get int64 from item
 * @param [in] pitem   : const mc_pack_item_t*
 * @param [out] res   : mc_int64_t*
 * @return  int
 * @retval
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_get_int64_from_item(const mc_pack_item_t *pitem, mc_int64_t *res);

/**
 * @brief get uint64 from item
 * @param [in] pitem   : const mc_pack_item_t*
 * @param [out] res   : mc_uint64_t*
 * @return  int
 * @retval
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_v1pack_get_uint64_from_item(const mc_pack_item_t *pitem, mc_uint64_t *res);

/**
 * @brief put item into pack
 * @param [in] pack   : mc_v1pack_t*
 * @param [in] pitem   : const mc_pack_item_t*
 * @return  int
 * @retval
 *		- 0 : succeed
 *		- <0 : failed
 * @note
 *	��ȡ item.key ����ĩ�����֣����Զ������� pack �����
 **/
int mc_v1pack_put_item(mc_v1pack_t *pack, const mc_pack_item_t *pitem);

#endif	//__MC_PACK_CORE_H
