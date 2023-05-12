/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
  * @file    app_msg.h
  * @brief   Message definition for user application task.
  * @note    If new message types/sub types are to be added, add to the end of enumeration.
  *          The orders are NOT to changed.
  * @author  Lory
  * @date    2017.2.9
  * @version v1.0
*********************************************************************************************************
*/
/*============================================================================*
  *                     Define to prevent recursive inclusion
  *============================================================================*/

#ifndef _APP_MSG_H_
#define _APP_MSG_H_

/*============================================================================*
  *                               Header Files
  *============================================================================*/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup APP_MSG APP Message
  * @brief message definition for user application task
  * @{
  */

/*============================================================================*
  *                                   Types
  *============================================================================*/
/** @defgroup APP_MSG_Exported_Types APP Message Exported Types
  * @{
    */

/**  @brief Event type definitions.*/
typedef enum
{
    EVENT_GAP_MSG = 0x01,  /**< message from gap layer for stack */
    EVENT_IO_TO_APP = 0x02, /**< message from IO to user application */
    EVENT_MULTI_SPK_TIMER = 0x03,
    EVENT_IODRIVER_TO_APP = 0x05,
    EVENT_DSP_MSG = 0x06,
    EVENT_UART_TX = 0x08,
    EVENT_UART_RX = 0x09,
    EVENT_GAP_TIMER = 0x0A,
} T_EVENT_TYPE;

/**  @brief IO type definitions for IO message, may extend as requested */
typedef enum
{
    IO_MSG_TYPE_BT_STATUS,  /**< BT status change with subtype @ref GAP_MSG_TYPE */
    IO_MSG_TYPE_KEYSCAN,    /**< Key scan message with subtype @ref T_IO_MSG_KEYSCAN */
    IO_MSG_TYPE_QDECODE,    /**< subtype to be defined */
    IO_MSG_TYPE_UART,       /**< Uart message with subtype @ref T_IO_MSG_UART */
    IO_MSG_TYPE_KEYPAD,     /**< subtype to be defined */
    IO_MSG_TYPE_IR,         /**< subtype to be defined */
    IO_MSG_TYPE_GDMA,       /**< subtype to be defined */
    IO_MSG_TYPE_ADC,        /**< subtype to be defined */
    IO_MSG_TYPE_D3DG,       /**< subtype to be defined */
    IO_MSG_TYPE_SPI,        /**< subtype to be defined */
    IO_MSG_TYPE_MOUSE_BUTTON,   /**< subtype to be defined */
    IO_MSG_TYPE_GPIO,       /**< Gpio message with subtype @ref T_IO_MSG_GPIO*/
    IO_MSG_TYPE_MOUSE_SENSOR,   /**< subtype to be defined */
    IO_MSG_TYPE_TIMER,      /**< App timer message with subtype @ref T_IO_MSG_TIMER */
    IO_MSG_TYPE_WRISTBNAD,  /**< wristband message with subtype @ref T_IO_MSG_WRISTBAND */
    IO_MSG_TYPE_MESH_STATUS,    /**< subtype to be defined */
    IO_MSG_TYPE_KEYBOARD_BUTTON, /**< subtype to be defined */
    IO_MSG_TYPE_ANCS,            /**< ANCS message*/
    IO_MSG_TYPE_IR_LEARN_DATA,       /**< ir learn data message*/
    IO_MSG_TYPE_IR_LEARN_STOP,       /**< ir learn stop message*/
    IO_MSG_TYPE_IR_SEND_REPEAT_CODE, /**< ir send repeat code message*/
    IO_MSG_TYPE_IR_SEND_COMPLETE,    /**< ir send complete message*/
    IO_MSG_TYPE_BAT_LPC,        /**< lpc send low power message*/
    IO_MSG_TYPE_BAT_DETECT,     /**< BAT adc detect battery value*/
    IO_MSG_TYPE_AUDIO,          /**< Audio message with subtype @ref T_IO_MSG_TYPE_AUDIO*/
    IO_MSG_TYPE_RESET_WDG_TIMER, /**< reset watch dog timer*/
} T_IO_MSG_TYPE;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_AUDIO type */
typedef enum
{
    IO_MSG_AUDIO_INPUT_BUFF_READY,
    IO_MSG_AUDIO_PROCESS_DONE,
    IO_MSG_AUDIO_TIMEOUT,
} T_IO_MSG_TYPE_AUDIO;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_KEYSCAN type */
typedef enum
{
    IO_MSG_KEYSCAN_RX_PKT        = 1, /**< Keyscan RX data event */
    IO_MSG_KEYSCAN_MAX           = 2, /**<  */
    IO_MSG_KEYSCAN_ALLKEYRELEASE = 3, /**< All keys are released event */
    IO_MSG_KEYSCAN_STUCK         = 4, /**<  key stuck message */
} T_IO_MSG_KEYSCAN;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_UART type */
typedef enum
{
    IO_MSG_UART_RX                     = 1,
    IO_MSG_UART_RX_TIMEOUT             = 2,
    IO_MSG_UART_RX_OVERFLOW            = 3,
    IO_MSG_UART_RX_TIMEOUT_OVERFLOW    = 4,
    IO_MSG_UART_RX_EMPTY               = 5,
} T_IO_MSG_UART;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_GPIO type */
typedef enum
{
    IO_MSG_GPIO_KEY,               /**< KEY GPIO event */
    IO_MSG_GPIO_LINE_IN,           /**< LINE IN event */
    IO_MSG_GPIO_NFC,               /**< NFC event */
    IO_MSG_GPIO_UART_WAKE_UP,      /**< UART WAKE UP event */
    IO_MSG_GPIO_CHARGER,           /**< CHARGER event */
} T_IO_MSG_GPIO;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_TIMER type */
typedef enum
{
    IO_MSG_TIMER_ALARM,
    IO_MSG_TIMER_RWS
} T_IO_MSG_TIMER;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_WRISTBNAD type */
typedef enum
{
    IO_MSG_BWPS_TX_VALUE,
    IO_MSG_RTC_TIMEROUT_WALL_CLOCK,
    IO_MSG_SENSOR_WAKE_UP,
    IO_MSG_LED_TWINKLE,
    IO_MSG_MOTOR_VIBRATE,
    IO_MSG_CHARGER_STATE,
    IO_MSG_RTC_LOW_BATTERY_RESTORE,
    IO_MSG_RTC_ALARM,
    IO_MSG_POWER_OFF,
    IO_MSG_POWER_ON,
    IO_MSG_HRS_EVENT,
    IO_MSG_SENSOR_MOTION_INTERRUPT,
    IO_MSG_UART_CMD_DEBUG,
    IO_MSG_HRS_TIMEOUT_HANDLE,
    IO_MSG_UPDATE_CONPARA,
    IO_MSG_SHORT_BUTTON,
    IO_MSG_LONG_BUTTON,
    IO_MSG_UP_TOUCH,
    IO_MSG_DOWN_TOUCH,
    IO_MSG_LEFT_TOUCH,
    IO_MSG_RIGHT_TOUCH,
    IO_MSG_UART_GPS,
    IO_MSG_UART_DEBUG_RX,
    IO_MSG_WAS_RX_VALUE,
    IO_MSG_WAS_ENABLE_CCCD,
} T_IO_MSG_WRISTBAND;

/**  @brief IO message definition for communications between tasks*/
typedef struct
{
    uint16_t type;
    uint16_t subtype;
    union
    {
        uint32_t  param;
        void     *buf;
    } u;
} T_IO_MSG;

/** @} */ /* End of group APP_MSG_Exported_Types */

/** @} */ /* End of group APP_MSG */

#ifdef __cplusplus
}
#endif

#endif /* _APP_MSG_H_ */
