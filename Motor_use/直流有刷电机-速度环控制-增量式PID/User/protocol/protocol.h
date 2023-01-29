
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/
#include "stm32f1xx.h"
#include "./usart/bsp_debug_usart.h"

#ifdef _cplusplus
extern "C" {
#endif   

/* 鏁版嵁鎺ユ敹缂撳啿鍖哄ぇ�?? */
#define PROT_FRAME_LEN_RECV  128

/* 鏍￠獙鏁版嵁鐨勯暱搴? */
#define PROT_FRAME_LEN_CHECKSUM    1

/* 鏁版嵁澶寸粨鏋勪�? */
typedef __packed struct
{
  uint32_t head;    // 鍖呭�?
  uint8_t ch;       // 閫氶�?
  uint32_t len;     // 鍖呴暱搴?
  uint8_t cmd;      // 鍛戒�?
//  uint8_t sum;      // 鏍￠獙鍜?
  
}packet_head_t;

#define FRAME_HEADER     0x59485A53    // 甯уご

/* 閫氶亾瀹忓畾涔? */
#define CURVES_CH1      0x01
#define CURVES_CH2      0x02
#define CURVES_CH3      0x03
#define CURVES_CH4      0x04
#define CURVES_CH5      0x05

/* 鎸囦�?(涓嬩綅鏈? -> 涓婁綅鏈?) */
#define SEND_TARGET_CMD      0x01     // 鍙戦€佷笂浣嶆満閫氶亾鐨勭洰鏍囧�??
#define SEND_FACT_CMD        0x02     // 鍙戦€侀€氶亾瀹為檯鍊?
#define SEND_P_I_D_CMD       0x03     // 鍙戦�?? PID 鍊硷紙鍚屾涓婁綅鏈烘樉绀虹殑鍊硷�?
#define SEND_START_CMD       0x04     // 鍙戦€佸惎鍔ㄦ寚浠わ紙鍚屾涓婁綅鏈烘寜閽姸鎬侊級
#define SEND_STOP_CMD        0x05     // 鍙戦€佸仠姝㈡寚浠わ紙鍚屾涓婁綅鏈烘寜閽姸鎬侊級
#define SEND_PERIOD_CMD      0x06     // 鍙戦€佸懆鏈燂紙鍚屾涓婁綅鏈烘樉绀虹殑鍊硷�?

/* 鎸囦�?(涓婁綅鏈? -> 涓嬩綅鏈?) */
#define SET_P_I_D_CMD        0x10     // 璁剧�? PID �??
#define SET_TARGET_CMD       0x11     // 璁剧疆鐩爣�??
#define START_CMD            0x12     // 鍚姩鎸囦护
#define STOP_CMD             0x13     // 鍋滄鎸囦护
#define RESET_CMD            0x14     // 澶嶄綅鎸囦护
#define SET_PERIOD_CMD       0x15     // 璁剧疆鍛ㄦ湡

/* 绌烘寚浠? */
#define CMD_NONE             0xFF     // 绌烘寚浠?

/* 绱㈠紩鍊煎畯瀹氫�? */
#define HEAD_INDEX_VAL       0x3u     // 鍖呭ご绱㈠紩鍊硷�?4瀛楄妭锛?
#define CHX_INDEX_VAL        0x4u     // 閫氶亾绱㈠紩鍊硷�?1瀛楄妭锛?
#define LEN_INDEX_VAL        0x5u     // 鍖呴暱绱㈠紩鍊硷�?4瀛楄妭锛?
#define CMD_INDEX_VAL        0x9u     // 鍛戒护绱㈠紩鍊硷�?1瀛楄妭锛?

#define EXCHANGE_H_L_BIT(data)      ((((data) << 24) & 0xFF000000) |\
                                     (((data) <<  8) & 0x00FF0000) |\
                                     (((data) >>  8) & 0x0000FF00) |\
                                     (((data) >> 24) & 0x000000FF))     // 浜ゆ崲楂樹綆瀛楄�?

#define COMPOUND_32BIT(data)        (((*(data-0) << 24) & 0xFF000000) |\
                                     ((*(data-1) << 16) & 0x00FF0000) |\
                                     ((*(data-2) <<  8) & 0x0000FF00) |\
                                     ((*(data-3) <<  0) & 0x000000FF))      // 鍚堟垚涓轰竴涓�?
                                     
/**
 * @brief   鎺ユ敹鏁版嵁澶勭�?
 * @param   *data:  瑕佽绠楃殑鏁版嵁鐨勬暟�??.
 * @param   data_len: 鏁版嵁鐨勫ぇ�??
 * @return  void.
 */
void protocol_data_recv(uint8_t *data, uint16_t data_len);

/**
 * @brief   鍒濆鍖栨帴鏀跺崗�??
 * @param   void
 * @return  鍒濆鍖栫粨�??.
 */
int32_t protocol_init(void);

/**
 * @brief   鎺ユ敹鐨勬暟鎹鐞?
 * @param   void
 * @return  -1锛氭病鏈夋壘鍒颁竴涓纭殑鍛戒护.
 */
int8_t receiving_process(void);

/**
  * @brief 璁剧疆涓婁綅鏈虹殑鍊?
  * @param cmd锛氬懡浠?
  * @param ch: 鏇茬嚎閫氶亾
  * @param data锛氬弬鏁版寚�??
  * @param num锛氬弬鏁颁釜�??
  * @retval �??
  */
void set_computer_value(uint8_t cmd, uint8_t ch, void *data, uint8_t num);

#ifdef _cplusplus
}
#endif   

#endif
