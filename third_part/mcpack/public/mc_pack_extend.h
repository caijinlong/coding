#ifndef  __MC_PACK_EX_H_
#define  __MC_PACK_EX_H_

#include <stdarg.h>

#include "mc_pack_c.h"

/**
 * @brief get error info string by errno
 * @param [in] err   : int
 *		errno
 * @return  const char*
 *		error info string
 * @retval
 *		��������δ���壬���� "MCP: Unknow error"
 **/
const char *mc_pack_perror(int err);
const char *mc_pack_perror_ptr(const mc_pack_t * ptr);

/**
 * @brief get type info string by type
 * @param [in] type   : int
 *		type, see mc_pack_type_t
 * @return  const char*
 *		type info string
 * @retval
 *		���δ���壬���� "unknown"
 **/
const char *mc_pack_ptype(int type);

/**
 * @brief compare string value in pack
 *		�Ƚ� pack �е��ַ��������ִ�Сд
 * @param [in] ppack   : const mc_pack_t*
 * @param [in] name   : const char*
 * @param [in] value   : const char*
 * @return  int
 * @retval  errno
 *		- 0 : equal
 *		- 1 : not equal
 *		- <0 : failed
 **/
int mc_pack_cmp_str(const mc_pack_t *ppack, const char *name, const char *value);

/**
 * @brief compare string value in pack
 *		�Ƚ� pack �е��ַ����������ִ�Сд
 *
 * @param [in] ppack   : const mc_pack_t*
 * @param [in] name   : const char*
 * @param [in] value   : const char*
 * @return  int
 * @retval  errno
 *		- 0 : equal
 *		- 1 : not equal
 *		- <0 : failed
 **/
int mc_pack_casecmp_str(const mc_pack_t *ppack, const char *name, const char *value);

/**
 * @brief create a pack from a buffer & put something like putf
 * @param [in] buf   : char*
 *		buffer
 * @param [in] buf_size   : u_int
 *		size of buffer
 * @param [in] fmt   : const char*
 *		"key:value.key:value" ��ʽ
 *		key �������ַ����� %s
 *		value ȡֵ %s %d %u %lld %llu ������ printf һ��
 * @return  mc_pack_t*
 * @retval  pack
 *		- !NULL : pack
 *		- NULL : failed
 **/
mc_pack_t *
mc_pack_creatf(int version, char *buf, u_int buf_size, char * tmp, u_int tmp_size, const char *fmt, ...);

/**
 * @brief ���� put item
 * @param [in] ppack   : mc_pack_t*
 * @param [in] fmt   : const char*
 *		"key:value.key:value" ��ʽ
 *		key �������ַ����� %s
 *		value ȡֵ %s %d %u %lld %llu ������ printf һ��
 *		value ȡֵ %p%u ʱ���� raw pointer �� size
 * @return  int
 * @retval   errno
 *		- >=0 : �ɹ� put �ĸ���
 *		- <0 : failed
 **/
int mc_pack_putf(mc_pack_t *ppack, const char *fmt, ...) __attribute__((format(printf, 2, 3)));

/**
 * @brief same as mc_pack_putf, except for using va_list arg
 * @param [in] ppack   : mc_pack_t*
 * @param [in] fmt   : const char*
 * @param [in] arg   : va_list
 * @return  int
 * @retval   errno
 *		- >=0 : �ɹ� put �ĸ���
 *		- <0 : failed
 **/
int mc_pack_vputf(mc_pack_t *ppack, const char *fmt, va_list arg);

/**
 * @brief ���� get item
 * @param [in] ppack   : const mc_pack_t*
 * @param [in] fmt   : const char*
 *		"key:value.key:value" ��ʽ
 *		key ���ַ���
 *		value ȡֵ %p %d %u %lld %llu ������ scanf һ��
 *		ʹ�� %p ��ȡ�ַ��������� char** ����
 *		ʹ�� %p%u ��ȡ raw������ char** �� u_int*
 * @return  int
 * @retval   errno
 *		- >=0 : �ɹ� get �ĸ���
 *		- <0 : failed
 **/
int mc_pack_getf(const mc_pack_t *ppack, const char *fmt, ...) __attribute__((format(scanf, 2, 3)));

/**
 * @brief same as mc_pack_getf, except for using va_list arg
 * @param [in] ppack   : const mc_pack_t*
 * @param [in] fmt   : const char*
 * @param [in] arg   : va_list
 * @return  int
 * @retval   errno
 *		- >=0 : �ɹ� get �ĸ���
 *		- <0 : failed
 **/
int mc_pack_vgetf(const mc_pack_t *ppack, const char *fmt, va_list arg);

/**
 * @brief pack to string
 *		�� pack ���������Ϊ key:value ��ʽ���ַ���
 * @param [in] ppack   : const mc_pack_t*
 * @param [out] str   : char*
 *		������ַ���������
 * @param [in] size   : u_int
 * @return  int
 * @retval	 errno
 *		- 0 : succeed
 *		- <0 : failed
 **/
int mc_pack_pack2str(const mc_pack_t *ppack, char *str, u_int size);

/**
 * @brief get last sub name
 * @param [in] key   : const char*
 *		pointer to full name
 * @return  const char*
 *		pointer to last sub name
 * @retval
 **/
const char *mc_pack_get_subkey(const char *key);
#endif  //__MC_PACK_EX_H_
