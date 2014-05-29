#ifndef  __MC_PACK_JSON_H_
#define  __MC_PACK_JSON_H_

#include "mc_pack_c.h"

/**
 * @brief convert json text to pack
 * @param [in] txt   : const char*
 * @param [in] pack   : mc_pack_t*
 * @return  int
 * @retval   errno
 *	- 0 : succeed
 *	- <0 : failed
 *	- >0 : position of parse failure, start from 1
 **/
int mc_pack_json2pack(const char *txt, mc_pack_t *pack);

/**
 * @brief convert pack to json text
 * raw will be ignored
 * @param [in] pack   : const mc_pack_t*
 * @param [out] txt_buf   : char*
 * @param [in] buf_size   : u_int
 * @return  int
 * @retval   errno
 *	- 0 : succeed
 *	- <0 : failed
 **/
int mc_pack_pack2json(const mc_pack_t *pack, char *txt_buf, u_int buf_size);

/**
 * @brief
 * ʹ��json��ʽ��mcpack��ӡ���ն�
 * @param [in/out] src   : const mc_pack_t*
 * @return  int
 * @retval
 **/
int mc_pack_print(const mc_pack_t * src);

/**
 * @brief
 * ʹ��json��ʽ�Ѵ���mcpack��buffeer��ӡ���ն�
 * @param [in/out] packbuf   : const char*
 * @param [in/out] length   : unsigned int
 * @return  int
 * @retval
 **/
int mc_pack_printbuf(const char * packbuf, unsigned int length);

#endif  //__MC_PACK_TEXT_H_
