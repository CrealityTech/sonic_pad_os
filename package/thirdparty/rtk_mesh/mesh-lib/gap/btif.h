/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BTIF_H_
#define _BTIF_H_

#include <stdint.h>
#include <stdbool.h>
#include <bt_types.h>


#ifdef __cplusplus
extern "C" {
#endif


#define F_BT_LE_PRIVACY_SUPPORT		1
/**
 * \addtogroup  STACK      STACK
 * \defgroup    BTIF       BTIF
 *
 * \brief   Define parameters and structures used in Bluetooth stack APIs or messages.
 * \details The BTIF module provide parameters and structure definitions that
 *          can be used in Bluetooth stack APIs and upstream messages that Bluetooth
 *          stack sent to APP. APIs such as release btif message buffers are also
 *          provided here.\n
 *
 * \ingroup     STACK
 */

/**
 *
 * \ingroup     BTIF
 */
#define BTIF_NAME_LENGTH                     40

#define BTIF_ERR                            GAP_ERR

#define BTIF_SUCCESS                        GAP_SUCCESS
#define BTIF_ACCEPT                         GAP_ACCEPT
#define BTIF_ERR_REJECT                     GAP_ERR_REJECT
#define BTIF_ERR_NO_RESOURCE                GAP_ERR_NO_RESOURCE
#define BTIF_ERR_INVALID_PARAM              GAP_ERR_INVALID_PARAM
#define BTIF_ERR_INVALID_STATE              GAP_ERR_INVALID_STATE
#define BTIF_ERR_CONN_DISCONNECT            GAP_ERR_CONN_DISCONNECT
#define BTIF_ERR_CONN_LOST                  GAP_ERR_CONN_LOST
#define BTIF_ERR_AUTHEN_FAIL                GAP_ERR_AUTHEN_FAIL
#define BTIF_ERR_INIT_TOUT                  GAP_ERR_INIT_TOUT
#define BTIF_ERR_INIT_OUT_OF_SYNC           GAP_ERR_INIT_OUT_OF_SYNC
#define BTIF_ERR_INIT_HARDWARE_ERROR        GAP_ERR_INIT_HARDWARE_ERROR
#define BTIF_ERR_LOWER_LAYER_ERROR          GAP_ERR_LOWER_LAYER_ERROR
#define BTIF_ERR_ADDR_NOT_RESOLVED          GAP_ERR_ADDR_NOT_RESOLVED
#define BTIF_ERR_TOUT                       GAP_ERR_TOUT

#define BTIF_ERR_UNSPECIFIED                GAP_ERR_UNSPECIFIED
#define BTIF_ERR_NOT_SUPPORTED              GAP_ERR_NOT_SUPPORTED

/**
 * btif.h
 *
 * \name  BTIF_AUTHEN_FLAGS
 * \brief Btif authentication support flags.
 * \anchor  BTIF_AUTHEN_FLAGS
 */
/**
 * \ingroup     BTIF
 */
#define BTIF_AUTHEN_BONDING                  0x0001
#define BTIF_AUTHEN_GENERAL_BONDING          0x0002
#define BTIF_AUTHEN_MITM                     0x0004
#define BTIF_AUTHEN_SC                       0x0008
#define BTIF_AUTHEN_KEY_PRESS                0x0010
#define BTIF_AUTHEN_FORCE_BONDING            0x0100
#define BTIF_AUTHEN_SC_ONLY                  0x0200
#define BTIF_AUTHEN_SC_BR                    0x0400

#define BTIF_LE_MAX_EXTENDED_SCAN_PHY_NUM    2
#define BTIF_LE_MAX_EXTENDED_ADV_SETS_NUM    4//max is 63 by core spec
#define BTIF_LE_MAX_INIT_PHYS_NUM            3

#define LE_ADV_CHANNEL_37                    0x01   /**< Enable channel 37 */
#define LE_ADV_CHANNEL_38                    0x02   /**< Enable channel 38 */
#define LE_ADV_CHANNEL_39                    0x04   /**< Enable channel 39 */
#define LE_ADV_CHANNEL_ALL                   0x07   /**< Enable all channels */

/* PairingReq / PairingRsp initKeyDist/respKeyDist */
#define BTIF_LE_SMP_DIST_ENC_KEY             0x01
#define BTIF_LE_SMP_DIST_ID_KEY              0x02
#define BTIF_LE_SMP_DIST_SIGN_KEY            0x04
#define BTIF_LE_SMP_DIST_LINK_KEY            0x08

#define BTIF_LE_CONN_INIT_PHY_1M             0x01
#define BTIF_LE_CONN_INIT_PHY_2M             0x02
#define BTIF_LE_CONN_INIT_PHY_CODED          0x04

#define BTIF_LE_TX_POWER_UNKNOWN             127
#define BTIF_USE_LE_FIXED_DISPLAY_VALUE      0x80000000

/**
If extended advertising PDU types are being used (bit 4 = 0) then:
1. The advertisement shall not be both connectable and scannable.
2. High duty cycle directed connectable advertising shall not be used (bit 3 = 0).
*/
#define BTIF_ADV_EVENT_PROP_CONNECTABLE_ADV     0x01
#define BTIF_ADV_EVENT_PROP_SCANNABLE_ADV       0x02
#define BTIF_ADV_EVENT_PROP_DIRECTED_ADV        0x04
#define BTIF_ADV_EVENT_PROP_HDC_DIRECTED_ADV    0x08
#define BTIF_ADV_EVENT_PROP_USE_LEGACY_ADV      0x10
#define BTIF_ADV_EVENT_PROP_OMIT_ADV_ADDR       0x20
#define BTIF_ADV_EVENT_PROP_INCLUDE_TX_POWER    0x40

#define BTIF_EXT_ADV_REPORT_CONNECTABLE_ADV     0x01
#define BTIF_EXT_ADV_REPORT_SCANNABLE_ADV       0x02
#define BTIF_EXT_ADV_REPORT_DIRECTED_ADV        0x04
#define BTIF_EXT_ADV_REPORT_SCAN_RESPONSE       0x08
#define BTIF_EXT_ADV_REPORT_USE_LEGACY_ADV      0x10

#define BTIF_LE_SCAN_PHY_LE_1M                  0x01
#define BTIF_LE_SCAN_PHY_LE_CODED               0x04

#define BTIF_LE_ALL_PHY_NO_PREFERENCE_TX        0x01
#define BTIF_LE_ALL_PHY_NO_PREFERENCE_RX        0x02

#define BTIF_LE_TX_PHY_PREFERENCE_1M            0x01
#define BTIF_LE_TX_PHY_PREFERENCE_2M            0x02
#define BTIF_LE_TX_PHY_PREFERENCE_CODED         0x04

#define BTIF_LE_RX_PHY_PREFERENCE_1M            0x01
#define BTIF_LE_RX_PHY_PREFERENCE_2M            0x02
#define BTIF_LE_RX_PHY_PREFERENCE_CODED         0x04

/**
 * btif.h
 *
 * \name  BTIF_BR_SEC
 * \brief Btif BR/EDR security requirement flags.
 * \anchor  BTIF_BR_SEC
 */
/**
 * \ingroup     BTIF
 */
#define BTIF_BR_SEC_OUTGOING            0x01    /* define requirement for outgoing connection*/
#define BTIF_BR_SEC_AUTHEN              0x02
#define BTIF_BR_SEC_MITM                0x04    /* mitm required, only possible if authentication is set */
#define BTIF_BR_SEC_AUTHOR              0x08    /* link establishment requires explicit user level authorization */
#define BTIF_BR_SEC_ENCRYPT             0x10    /* activate encryption on this link, only possible if authentication is set*/
#define BTIF_BR_SEC_SECURE_CONNECTIONS  0x20

/**
 * btif.h
 *
 * \name  BTIF_BR_LINK_POLICY
 * \brief Btif BR/EDR link policy.
 * \anchor  BTIF_BR_LINK_POLICY
 */
/**
 * \ingroup     BTIF
 */
#define BTIF_BR_LINK_POLICY_DISABLE_ALL             0x00
#define BTIF_BR_LINK_POLICY_ENABLE_ROLE_SWITCH      0x01
#define BTIF_BR_LINK_POLICY_ENABLE_SNIFF_MODE       0x04

/**
 * btif.h
 *
 * \name  BTIF_BR_ACL_PKT_TYPE
 * \brief Btif BR/EDR ACL link packet type.
 * \anchor  BTIF_BR_ACL_PKT_TYPE
 */
/**
 * \ingroup     BTIF
 */
#define BTIF_PKT_TYPE_NO_2DH1           0x0002
#define BTIF_PKT_TYPE_NO_3DH1           0x0004
#define BTIF_PKT_TYPE_DM1               0x0008
#define BTIF_PKT_TYPE_DH1               0x0010
#define BTIF_PKT_TYPE_NO_2DH3           0x0100
#define BTIF_PKT_TYPE_NO_3DH3           0x0200
#define BTIF_PKT_TYPE_DM3               0x0400
#define BTIF_PKT_TYPE_DH3               0x0800
#define BTIF_PKT_TYPE_NO_2DH5           0x1000
#define BTIF_PKT_TYPE_NO_3DH5           0x2000
#define BTIF_PKT_TYPE_DM5               0x4000
#define BTIF_PKT_TYPE_DH5               0x8000

/**
 * btif.h
 *
 * \name  BTIF_BR_L2CAP_MODE
 * \brief Btif BR/EDR L2CAP mode.
 * \anchor  BTIF_BR_L2CAP_MODE
 */
/**
 * \ingroup     BTIF
 */
#define BTIF_L2C_MODE_BASIC             (1 << 0x00)    /**< Basic L2CAP mode */
#define BTIF_L2C_MODE_ERTM              (1 << 0x03)    /**< Enhanced Retransmission mode */

/**
 * btif.h
 *
 * \name  BTIF_BR_L2CAP_REJ_MODE
 * \brief Btif BR/EDR L2CAP rej mode in ERTM.
 * \anchor  BTIF_BR_L2CAP_REJ_MODE
 */
/**
 * \ingroup     BTIF
 */
#define BTIF_L2C_ERTM_REJ               0x00
#define BTIF_L2C_ERTM_SREJ              0x01

#define BTIF_AIR_MODE_U_LAW             0
#define BTIF_AIR_MODE_A_LAW             1
#define BTIF_AIR_MODE_CVSD              2
#define BTIF_AIR_MODE_TRANSPARENT       3

typedef enum
{
    BTIF_CAUSE_SUCCESS              = BTIF_SUCCESS,
    BTIF_CAUSE_ACCEPT               = (BTIF_ERR | BTIF_ACCEPT),
    BTIF_CAUSE_REJECT               = (BTIF_ERR | BTIF_ERR_REJECT),
    BTIF_CAUSE_RESOURCE_ERROR       = (BTIF_ERR | BTIF_ERR_NO_RESOURCE),
    BTIF_CAUSE_INVALID_PARAM        = (BTIF_ERR | BTIF_ERR_INVALID_PARAM),
    BTIF_CAUSE_INVALID_STATE        = (BTIF_ERR | BTIF_ERR_INVALID_STATE),
    BTIF_CAUSE_CONN_DISCONNECT      = (BTIF_ERR | BTIF_ERR_CONN_DISCONNECT),
    BTIF_CAUSE_CONN_LOST            = (BTIF_ERR | BTIF_ERR_CONN_LOST),
    BTIF_CAUSE_AUTHEN_FAIL          = (BTIF_ERR | BTIF_ERR_AUTHEN_FAIL),
    BTIF_CAUSE_INIT_TOUT            = (BTIF_ERR | BTIF_ERR_INIT_TOUT),
    BTIF_CAUSE_INIT_OUT_OF_SYNC     = (BTIF_ERR | BTIF_ERR_INIT_OUT_OF_SYNC),
    BTIF_CAUSE_INIT_HARDWARE_ERROR  = (BTIF_ERR | BTIF_ERR_INIT_HARDWARE_ERROR),
    BTIF_CAUSE_LOWER_LAYER_ERROR    = (BTIF_ERR | BTIF_ERR_LOWER_LAYER_ERROR),
    BTIF_CAUSE_ADDR_NOT_RESOLVED    = (BTIF_ERR | BTIF_ERR_ADDR_NOT_RESOLVED),
    BTIF_CAUSE_TOUT                 = (BTIF_ERR | BTIF_ERR_TOUT),

    BTIF_CAUSE_UNSPECIFIED          = (BTIF_ERR | BTIF_ERR_UNSPECIFIED),
    BTIF_CAUSE_NOT_SUPPORTED        = (BTIF_ERR | BTIF_ERR_NOT_SUPPORTED),
} T_BTIF_CAUSE;

/**
 * btif.h
 *
 * \brief    Btif key type.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_KEY_COMBINATION           = 0x00, /**< Combination key. */
    BTIF_KEY_LOCAL_UNIT            = 0x01, /**< Local unit key. */
    BTIF_KEY_REMOTE_UNIT           = 0x02, /**< Remote unit key. */
    BTIF_KEY_DEBUG_COMBINATION     = 0x03, /**< Debug combination key. */
    BTIF_KEY_UNAUTHEN              = 0x04, /**< SSP generated link key without MITM protection. */
    BTIF_KEY_AUTHEN                = 0x05, /**< SSP generated link key with MITM protection. */
    BTIF_KEY_CHANGED_COMBINATION   = 0x06, /**< Changed combination key. */
    BTIF_KEY_UNAUTHEN_P256         = 0x07, /**< Security Connections generated link key without MITM protection. */
    BTIF_KEY_AUTHEN_P256           = 0x08, /**< Security Connections link key with MITM protection. */
    BTIF_KEY_LE_LOCAL_LTK          = 0x11, /**< BLE Long Term Key Blob. */
    BTIF_KEY_LE_REMOTE_LTK         = 0x12, /**< BLE Long Term Key Blob. */
    BTIF_KEY_LE_LOCAL_IRK          = 0x13, /**< BLE Identity Resolving Key. */
    BTIF_KEY_LE_REMOTE_IRK         = 0x14, /**< BLE Identity Resolving Key. */
    BTIF_KEY_LE_LOCAL_CSRK         = 0x15, /**< BLE Local CSR Key. */
    BTIF_KEY_LE_REMOTE_CSRK        = 0x16, /**< BLE Remote CSR Key. */
    BTIF_KEY_REQ_BR                = 0x80, /**< Only used to request a BR/EDR link key. */
    BTIF_KEY_DELETE                = 0xFF  /**< Link key is no longer valid and should be deleted. */
} T_BTIF_KEY_TYPE;

typedef enum
{
    BTIF_REMOTE_ADDR_LE_PUBLIC          = 0x00,
    BTIF_REMOTE_ADDR_LE_RANDOM          = 0x01,
    BTIF_REMOTE_ADDR_LE_PUBLIC_IDENTITY = 0x02,
    BTIF_REMOTE_ADDR_LE_RANDOM_IDENTITY = 0x03,
    BTIF_REMOTE_ADDR_ANY                = 0x20,
    BTIF_REMOTE_ADDR_LE_ANONYMOUS       = 0xff,
} T_BTIF_REMOTE_ADDR_TYPE;

typedef enum
{
    BTIF_DEV_CFG_OP_NAME = 0x10,
    BTIF_DEV_CFG_OP_DID_EIR,
    BTIF_DEV_CFG_OP_EXT_EIR,
    BTIF_DEV_CFG_OP_PAGE_SCAN,
    BTIF_DEV_CFG_OP_INQUIRY_SCAN,
    BTIF_DEV_CFG_OP_INQUIRY_MODE,
    BTIF_DEV_CFG_OP_LINK_POLICY,
    BTIF_DEV_CFG_OP_BT_MODE,
    BTIF_DEV_CFG_OP_ACCEPT_CONN_ROLE
} T_BTIF_DEV_CFG_OPCODE;

typedef enum
{
    BTIF_ACL_CONN_ACTIVE        = 0x01,
    BTIF_ACL_CONN_SNIFF         = 0x02,
    BTIF_ACL_AUTHEN_START       = 0x03,
    BTIF_ACL_AUTHEN_SUCCESS     = 0x04,
    BTIF_ACL_AUTHEN_FAIL        = 0x05,
    BTIF_ACL_CONN_ENCRYPTED     = 0x06,
    BTIF_ACL_CONN_DISCONN       = 0x07,
    BTIF_ACL_CONN_NOT_ENCRYPTED = 0x08,
    BTIF_ACL_ADDR_RESOLVED      = 0x09,
    BTIF_ACL_ROLE_MASTER        = 0x0A,
    BTIF_ACL_ROLE_SLAVE         = 0x0B,
    BTIF_ACL_CONN_SNIFF_SUBRATE = 0x0C,
    BTIF_ACL_CONN_FAIL          = 0x0D,
    BTIF_ACL_CONN_SUCCESS       = 0x0E,
    BTIF_ACL_CONN_READY         = 0x0F,
    BTIF_ACL_CONN_BR_SC_START   = 0x10,
    BTIF_ACL_CONN_BR_SC_CMPL    = 0x11
} T_BTIF_ACL_STATUS;

typedef struct
{
    T_BTIF_KEY_TYPE     key_type;
    uint8_t             key_size;
    uint8_t             encrypt_type;
    uint16_t            cause;
} T_BTIF_ACL_STATUS_PARAM_AUTHEN;

typedef struct
{
    uint16_t             cause;
} T_BTIF_ACL_STATUS_PARAM_BR_SC_CMPL;

typedef struct
{
    uint8_t                 bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE remote_addr_type;
} T_BTIF_ACL_STATUS_PARAM_RESOLVE;

typedef struct
{
    uint16_t    interval;
} T_BTIF_ACL_STATUS_PARAM_SNIFF;

typedef struct
{
    uint16_t    max_tx_latency;
    uint16_t    max_rx_latency;
    uint16_t    min_remote_tout;
    uint16_t    min_local_tout;
} T_BTIF_ACL_STATUS_PARAM_SNIFF_SUBRATE;

typedef struct
{
    uint16_t    cause;
} T_BTIF_ACL_CONNECTION_DISCONNECT;

typedef struct
{
    uint16_t    cause;
} T_BTIF_ACL_CONNECTION_FAIL;

typedef struct
{
    uint16_t    handle;
} T_BTIF_ACL_CONNECTION_SUCCESS;

typedef struct
{
    uint16_t    handle;
} T_BTIF_ACL_CONNECTION_READY;

typedef union
{
    T_BTIF_ACL_CONNECTION_SUCCESS           conn_success;
    T_BTIF_ACL_CONNECTION_READY             conn_ready;
    T_BTIF_ACL_CONNECTION_FAIL              conn_fail;
    T_BTIF_ACL_CONNECTION_DISCONNECT        conn_disconn;
    T_BTIF_ACL_STATUS_PARAM_AUTHEN          authen;
    T_BTIF_ACL_STATUS_PARAM_RESOLVE         resolve;
    T_BTIF_ACL_STATUS_PARAM_SNIFF           sniff;
    T_BTIF_ACL_STATUS_PARAM_SNIFF_SUBRATE   sniff_subrate;
    T_BTIF_ACL_STATUS_PARAM_BR_SC_CMPL      br_sc_cmpl;
} T_BTIF_ACL_STATUS_PARAM;

typedef enum
{
    BTIF_PASSKEY_ENTRY_STARTED,     /* the passkey key entry protocol procedure is started */
    BTIF_PASSKEY_DIGIT_ENTERED,     /* a digit is entered by the remote user */
    BTIF_PASSKEY_DIGIT_ERASED,      /* a digit is erased by the remote user */
    BTIF_PASSKEY_CLEARED,           /* the display is cleared by the remote user */
    BTIF_PASSKEY_ENTRY_CMPL,        /* the passkey key entry protocol procedure is completed  */
} T_BTIF_KEYPRESS_NOTIF_TYPE;

/** @brief define indentify address type */
typedef enum
{
    BTIF_IDENT_ADDR_PUBLIC      = 0x00,    /*  low energy public address. */
    BTIF_IDENT_ADDR_RAND        = 0x01,    /*  low energy random address. */
} T_BTIF_IDENT_ADDR_TYPE;

/** @brief define local adress type */
typedef enum
{
    BTIF_LOCAL_ADDR_LE_PUBLIC           = 0x00,    /**<  Bluetooth low energy public address. */
    BTIF_LOCAL_ADDR_LE_RANDOM           = 0x01,    /**<  Bluetooth low energy random address. */
    BTIF_LOCAL_ADDR_LE_RAP_OR_PUBLIC    = 0x02,    /**<  Bluetooth low energy random address. */
    BTIF_LOCAL_ADDR_LE_RAP_OR_RAND      = 0x03     /**<  Bluetooth low energy random address. */
} T_BTIF_LOCAL_ADDR_TYPE;

typedef enum
{
    BTIF_ADDR_LE_RAND_STATIC            = 0x00,
    BTIF_ADDR_LE_RAND_NON_RESOLVABLE    = 0x01,
    BTIF_ADDR_LE_RAND_RESOLVABLE        = 0x02
} T_BTIF_ADDR_RAND;

typedef enum
{
    BTIF_PDU_TYPE_ANY           = 0x00,
    BTIF_PDU_TYPE_NOTIFICATION  = 0x01,
    BTIF_PDU_TYPE_INDICATION    = 0x02
} T_BTIF_GATT_PDU_TYPE;

typedef enum
{
    BTIF_GATT_DISCOVERY_SERVICE = 0x01,     /**< all primary services */
    BTIF_GATT_DISCOVERY_SERVICE_BY_UUID,    /**< service by UUID      */
    BTIF_GATT_DISCOVERY_CHARACTER,          /**< all characteristics  */
    BTIF_GATT_DISCOVERY_CHARACTER_DESC,     /**< all characteristic descriptors   */
    BTIF_GATT_DISCOVERY_RELATIONSHIP        /**< relationship (included services) */
} T_BTIF_GATT_DISCOVERY_TYPE;

typedef struct
{
    uint16_t    att_handle;
    uint16_t    end_group_handle;
    uint16_t    uuid16;
} T_BTIF_GATT_GENERIC_ELEM16;

typedef struct
{
    uint16_t    att_handle;
    uint16_t    end_group_handle;
    uint8_t     uuid128[16];
} T_BTIF_GATT_GENERIC_ELEM128;

typedef T_BTIF_GATT_GENERIC_ELEM16  T_BTIF_GATT_SERVICE_ELEM16;
typedef T_BTIF_GATT_GENERIC_ELEM128 T_BTIF_GATT_SERVICE_ELEM128;

typedef struct
{
    uint16_t    att_handle;
    uint16_t    end_group_handle;
} T_BTIF_GATT_SERVICE_BY_UUID_ELEM;

typedef struct
{
    uint16_t    decl_handle;
    uint16_t    properties;            /**< high nibble is reserved */
    uint16_t    value_handle;
    uint16_t    uuid16;
} T_BTIF_GATT_CHARACT_ELEM16;

typedef struct
{
    uint16_t    decl_handle;
    uint16_t    properties;            /**< high nibble is reserved */
    uint16_t    value_handle;
    uint8_t     uuid128[16];
} T_BTIF_GATT_CHARACT_ELEM128;

typedef struct
{
    uint16_t    handle;
    uint16_t    uuid16;
} T_BTIF_GATT_CHARACT_DESC_ELEM16;

typedef struct
{
    uint16_t    handle;
    uint8_t     uuid128[16];
} T_BTIF_GATT_CHARACT_DESC_ELEM128;

typedef struct
{
    uint16_t    decl_handle;
    uint16_t    att_handle;
    uint16_t    end_group_handle;
    uint16_t    uuid16;
} T_BTIF_GATT_RELATION_ELEM16;

typedef struct
{
    uint16_t    decl_handle;
    uint16_t    att_handle;
    uint16_t    end_group_handle;
    uint8_t     uuid128[16];
} T_BTIF_GATT_RELATION_ELEM128;

typedef enum
{
    BTIF_GATT_READ_TYPE_BASIC = 0x01,   /**< ATT "Read (Blob) Request"  */
    BTIF_GATT_READ_TYPE_BY_UUID         /**< ATT "Read By Type Request" */
} T_BTIF_GATT_READ_TYPE;

typedef enum
{
    BTIF_GATT_WRITE_TYPE_REQ = 0x01,    /**<  ATT "Write Request"  */
    BTIF_GATT_WRITE_TYPE_CMD,           /**<  ATT "Write Command"  */
    BTIF_GATT_WRITE_TYPE_PREP,          /**<  ATT "Prepare Write Request"  */
    BTIF_GATT_WRITE_TYPE_SIGNED_CMD     /**<  ATT "Signed Write Command"  */
} T_BTIF_GATT_WRITE_TYPE;

typedef enum
{
    BTIF_GATT_STORE_OP_GET_CCC_BITS,            /**< get CCC Bits for one <bd_addr,bdtype> */
    BTIF_GATT_STORE_OP_SET_CCC_BITS,            /**< set CCC Bits for one <bd_addr,bdtype> */
} T_BTIF_GATT_STORE_OPCODE;

typedef enum
{
    BTIF_LE_ADV_MODE_DISABLED = 0,
    BTIF_LE_ADV_MODE_ENABLED
} T_BTIF_LE_ADV_MODE;

typedef enum
{
    BTIF_LE_EXT_ADV_MODE_DISABLED = 0,
    BTIF_LE_EXT_ADV_MODE_ENABLED
} T_BTIF_LE_EXT_ADV_MODE;

typedef enum
{
    BTIF_LE_PERIODIC_ADV_MODE_DISABLED = 0,
    BTIF_LE_PERIODIC_ADV_MODE_ENABLED
} T_BTIF_LE_PERIODIC_ADV_MODE;

typedef enum
{
    BTIF_LE_AE_LEGACY_ADV_CONN_N_SCAN_UNDIRECTED  = 0x13,
    BTIF_LE_AE_LEGACY_ADV_CONN_LOW_DUTY_DIRECTED  = 0x15,
    BTIF_LE_AE_LEGACY_ADV_CONN_HIGH_DUTY_DIRECTED = 0x1D,
    BTIF_LE_AE_LEGACY_ADV_SCAN_UNDIRECTED         = 0x12,
    BTIF_LE_AE_LEGACY_ADV_NON_SCAN_NON_CONN       = 0x10,
} T_BTIF_LE_AE_LEGACY_ADV_PROPERTY;

typedef enum
{
    BTIF_LE_PRIM_ADV_PHY_LE_1M      = 1,
    BTIF_LE_PRIM_ADV_PHY_LE_CODED   = 3
} T_BTIF_LE_PRIM_ADV_PHY_TYPE;

typedef enum
{
    BTIF_LE_ADV_TYPE_UNDIRECTED = 0,
    BTIF_LE_ADV_TYPE_DIRECTED_HIGH_DUTY,
    BTIF_LE_ADV_TYPE_SCANNABLE,
    BTIF_LE_ADV_TYPE_NON_CONNECTABLE,
    BTIF_LE_ADV_TYPE_DIRECTED_LOW_DUTY
} T_BTIF_LE_ADV_TYPE;

typedef enum
{
    BTIF_LE_SCAN_REQ_NOTIFY_DISABLE = 0,
    BTIF_LE_SCAN_REQ_NOTIFY_ENABLE  = 1
} T_BTIF_LE_SCAN_REQ_NOTIFY_TYPE;

/** This enum type describes adv type for BTIF_MSG_LE_SCAN_INFO. */
typedef enum
{
    BTIF_LE_ADV_EVT_TYPE_UNDIRECTED = 0,    /**<  Connectable  undirected advertising. */
    BTIF_LE_ADV_EVT_TYPE_DIRECTED   = 1,    /**<  Connectable directed advertising. */
    BTIF_LE_ADV_EVT_TYPE_SCANNABLE  = 2,    /**<  Scanable undirected advertising. */
    BTIF_LE_ADV_EVT_TYPE_NON_CONNECTABLE,   /**<  Nonconnectable undirected advertising. */
    BTIF_LE_ADV_EVT_TYPE_SCAN_RSP           /**<  scan response. */
} T_BTIF_LE_ADV_EVT_TYPE;

typedef enum
{
    BTIF_LE_ADV_EVT_TYPE_CONNECTABLE_DIRECTED = 0x01,   /*  Connectable  undirected advertising */
} T_BTIF_LE_DIRECT_ADV_EVT_TYPE;

typedef enum
{
    BTIF_LE_ADV_FILTER_ANY = 0,
    BTIF_LE_ADV_FILTER_WHITE_LIST_SCAN,
    BTIF_LE_ADV_FILTER_WHITE_LIST_CONN,
    BTIF_LE_ADV_FILTER_WHITE_LIST_ALL,
} T_BTIF_LE_ADV_FILTER_POLICY;

typedef enum
{
    BTIF_LE_SCAN_FILTER_ANY = 0,
    BTIF_LE_SCAN_FILTER_WHITE_LIST,
    BTIF_LE_SCAN_FILTER_ANY_RPA,
    BTIF_LE_SCAN_FILTER_WHITE_LIST_RPA,
} T_BTIF_LE_SCAN_FILTER_POLICY;

typedef enum
{
    BTIF_LE_DATA_TYPE_ADV = 0,
    BTIF_LE_DATA_TYPE_SCAN_RSP
} T_BTIF_LE_ADV_DATA_TYPE;

typedef enum
{
    BTIF_LE_DATA_TYPE_EXT_ADV = 0,
    BTIF_LE_DATA_TYPE_EXT_SCAN_RSP
} T_BTIF_LE_EXT_ADV_DATA_TYPE;

typedef enum
{
    BTIF_LE_ADV_FRAG_OP_INTERMEDIATE_FRAG   = 0,
    BTIF_LE_ADV_FRAG_OP_FIRST_FRAG          = 1,
    BTIF_LE_ADV_FRAG_OP_LAST_FRAG           = 2,
    BTIF_LE_ADV_FRAG_OP_CMPL_ADV_DATA       = 3,
    BTIF_LE_ADV_FRAG_OP_UNCHANGE_UPDATE_DID = 4,
} T_BTIF_LE_ADV_FRAG_OP_TYPE;

typedef enum
{
    BTIF_LE_ADV_FRAG_PREFER_MAY_FRAG_ALL    = 0,
    BTIF_LE_ADV_FRAG_PREFER_NOT_OR_MIN_FRAG = 1
} T_BTIF_LE_ADV_FRAG_PREFERENCE_TYPE;

typedef enum
{
    BTIF_LE_SCAN_MODE_DISABLED  = 0,
    BTIF_LE_SCAN_MODE_ENABLED   = 1,
} T_BTIF_LE_SCAN_MODE;

typedef enum
{
    BTIF_LE_SCAN_DUPLICATE_FILTER_DISABLED = 0,
    BTIF_LE_SCAN_DUPLICATE_FILTER_ENABLED = 1,
    BTIF_LE_SCAN_DUPLICATE_FILTER_ENABLED_RESET_FOR_EACH_PERIOD = 2,
} T_BTIF_LE_SCAN_FILTER_DUPLICATES;

typedef enum
{
    BTIF_LE_SCAN_TYPE_PASSIVE,
    BTIF_LE_SCAN_TYPE_ACTIVE
} T_BTIF_LE_SCAN_TYPE;

typedef enum
{
    BTIF_LE_WHITE_LIST_OP_CLEAR = 0,
    BTIF_LE_WHITE_LIST_OP_ADD,
    BTIF_LE_WHITE_LIST_OP_REMOVE
} T_BTIF_LE_WHITE_LIST_OP;

typedef enum
{
    BTIF_LE_ADV_SET_OP_CLEAR = 0,
    BTIF_LE_ADV_SET_OP_ADD,
    BTIF_LE_ADV_SET_OP_REMOVE
} T_BTIF_LE_ADV_SET_OP;

typedef enum
{
    BTIF_DIRECT_ADDR_LE_PUBLIC              = 0x00,
    BTIF_DIRECT_ADDR_LE_RAND                = 0x01,
    BTIF_DIRECT_ADDR_LE_PUBLIC_IDENT        = 0x02,
    BTIF_DIRECT_ADDR_LE_RAND_IDENT          = 0x03,
    BTIF_DIRECT_ADDR_LE_RAND_UNABLE_RESOLV  = 0xFE
} T_BTIF_DIRECT_ADDR_TYPE;

typedef struct
{
    T_BTIF_LE_SCAN_TYPE scan_type;
    uint16_t            scan_interval;
    uint16_t            scan_window;
} T_BTIF_LE_EXT_SCAN_PARAM;

typedef struct
{
    uint8_t     adv_handle;
    uint16_t    duration;
    uint8_t     max_ext_adv_evt;
} T_BTIF_LE_EXT_ADV_SET_PARAM;

typedef enum
{
    BTIF_LE_PERIODIC_ADV_LIST_OP_CLEAR = 0,
    BTIF_LE_PERIODIC_ADV_LIST_OP_ADD,
    BTIF_LE_PERIODIC_ADV_LIST_OP_REMOVE
} T_BTIF_LE_PERIODIC_ADV_LIST_OP;

typedef enum
{
    BTIF_LE_RESOLVING_LIST_OP_CLEAR = 0,
    BTIF_LE_RESOLVING_LIST_OP_ADD,
    BTIF_LE_RESOLVING_LIST_OP_REMOVE
} T_BTIF_LE_RESOLVING_LIST_OP;

typedef enum
{
    BTIF_LE_NETWORK_PRIVACY = 0,
    BTIF_LE_DEVICE_PRIVACY
} T_BTIF_LE_PRIVACY_MODE;

typedef enum
{
    BTIF_LE_PHY_1M       = 0x01,
    BTIF_LE_PHY_2M       = 0x02,
    BTIF_LE_PHY_CODED    = 0x03
} T_BTIF_LE_PHY_TYPE;

typedef enum
{
    BTIF_LE_PHY_OPTIONS_LE_CODED_PREFERENCE_NO   = 0,
    BTIF_LE_PHY_OPTIONS_LE_CODED_PREFERENCE_S2   = 1,
    BTIF_LE_PHY_OPTIONS_LE_CODED_PREFERENCE_S8   = 2
} T_BTIF_LE_PHY_OPTIONS;

typedef struct
{
    uint16_t        scan_interval;
    uint16_t        scan_window;
    uint16_t        conn_interval_min;
    uint16_t        conn_interval_max;
    uint16_t        conn_latency;
    uint16_t        supv_tout;
    uint16_t        ce_len_min;
    uint16_t        ce_len_max;
} T_BTIF_LE_CONN_REQ_PARAM;

typedef enum
{
    T_BTIF_SERVICE_APP_DEF      = 0,   /**< application provides services */
} T_BTIF_GATT_SERVICE_ID;

typedef enum
{
    BTIF_L2C_LE_CONN_ACCEPT                       = L2C_LE_CREDIT_CONN_SUCCESS,
    BTIF_L2C_LE_CONN_PSM_NOT_SUPPORTED            = (L2C_ERR | L2C_LE_CREDIT_RSP_PSM_NOT_SUPPORTED),
    BTIF_L2C_LE_CONN_NO_RESOURCES                 = (L2C_ERR | L2C_LE_CREDIT_RSP_NO_RESOURCES),
    BTIF_L2C_LE_CONN_INSUFFICIENT_AUTHEN          = (L2C_ERR | L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHEN),
    BTIF_L2C_LE_CONN_INSUFFICIENT_AUTHOR          = (L2C_ERR | L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHOR),
    BTIF_L2C_LE_CONN_INSUFFICIENT_ENCRYP_KEY_SIZE = (L2C_ERR | L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP_KEY_SIZE),
    BTIF_L2C_LE_CONN_INSUFFICIENT_ENCRYP          = (L2C_ERR | L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP),
    BTIF_L2C_LE_CONN_INVAlID_SOURCE_CID           = (L2C_ERR | L2C_LE_CREDIT_RSP_INVALID_SOURCE_CID),
    BTIF_L2C_LE_CONN_SOURCE_CID_ALREADY_ALLOC     = (L2C_ERR | L2C_LE_CREDIT_RSP_SOURCE_CID_ALREADY_ALLOC)
} T_BTIF_L2C_LE_CONN_STATUS;

typedef enum
{
    BTIF_LE_SECURITY_NONE,
    BTIF_LE_SECURITY_UNAUTHEN_ENCRYT,
    BTIF_LE_SECURITY_AUTHEN_ENCRYT,
    BTIF_LE_SECURITY_UNAUTHEN_DATA_SIGN,
    BTIF_LE_SECURITY_AUTHEN_DATA_SIGN,
    BTIF_LE_SECURITY_AUTHOR
} T_BTIF_LE_SECURITY_MODE;

/**
 * btif.h
 *
 * \brief    Definition of input/output capabilities.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_IOCAP_DISPLAY_ONLY,       /**< only a Display present, no Keyboard or Yes/No Keys */
    BTIF_IOCAP_DISPLAY_YES_NO,     /**< Display and Yes/No Keys present */
    BTIF_IOCAP_KEYBOARD_ONLY,      /**< only a Keyboard present, no Display */
    BTIF_IOCAP_NO_IO,              /**< no input/output capabilities */
    BTIF_IOCAP_KEYBOARD_DISPLAY    /**< BLE: Keyboard and Display present */
} T_BTIF_IOCAP;

/**
 * btif.h
 *
 * \brief    BR/EDR bluetooth mode.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_BT_MODE21_DISABLED,    /**< disable all Bluetooth 2.1 functionality */
    BTIF_BT_MODE21_ENABLED      /**< enable Bluetooth 2.1 functionality */
} T_BTIF_BT_MODE;

/**
 * btif.h
 *
 * \brief    BR/EDR radio mode.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_RADIO_VISIBLE_CONNECTABLE  = 0x01,  /**< inquiry scan and page scan enabled on local device */
    BTIF_RADIO_VISIBLE              = 0x02,  /**< inquiry scan enabled, no page scan */
    BTIF_RADIO_CONNECTABLE          = 0x03,  /**< page scan enabled, no inquiry scan */
    BTIF_RADIO_NON_DISCVBL          = 0x04,  /**< no inquiry scan and page scan disabled */
    BTIF_RADIO_DEEP_SLEEP           = 0x05,
    BTIF_RADIO_OFF                  = 0x06
} T_BTIF_RADIO_MODE;

typedef enum
{
    BTIF_BR_NO_SCAN = 0,
    BTIF_BR_INQUIRY_SCAN,
    BTIF_BR_PAGE_SCAN,
    BTIF_BR_INQUIRY_AND_PAGE_SCAN,
} T_BTIF_BR_PAGE_SCAN_MODE;

typedef enum
{
    BTIF_BR_PAGE_SCAN_STANDARD  = 0,
    BTIF_BR_PAGE_SCAN_INTERLACED,
} T_BTIF_BR_PAGE_SCAN_TYPE;

typedef enum
{
    BTIF_BR_INQUIRY_SCAN_STANDARD = 0,
    BTIF_BR_INQUIRY_SCAN_INTERLACED,
} T_BTIF_BR_INQUIRY_SCAN_TYPE;

typedef enum
{
    BTIF_BR_STANDARD_INQUIRY_RESULT = 0,
    BTIF_BR_INQUIRY_RESULT_WITH_RSSI,
    BTIF_BR_EXTENDED_INQUIRY_RESULT,
} T_BTIF_BR_INQUIRY_MODE;

typedef uint8_t T_BTIF_BR_LINK_POLICY;

typedef enum
{
    BTIF_BR_LINK_ROLE_MASTER,
    BTIF_BR_LINK_ROLE_SLAVE
} T_BTIF_BR_LINK_ROLE;

typedef enum
{
    BTIF_ACL_CFG_LINK_POLICY,
    BTIF_ACL_CFG_LINK_SUPV_TOUT,
    BTIF_ACL_CFG_LINK_ROLE,
    BTIF_ACL_CFG_PKT_TYPE,
    BTIF_ACL_CFG_ENTER_SNIFF_MODE,
    BTIF_ACL_CFG_EXIT_SNIFF_MODE,
} T_BTIF_ACL_CFG_OPCODE;

typedef struct
{
    T_BTIF_BR_LINK_POLICY   policy;
} T_BTIF_ACL_CFG_PARAM_LINK_POLICY;

typedef struct
{
    uint16_t                tout;
} T_BTIF_ACL_CFG_PARAM_LINK_SUPV_TOUT;

typedef struct
{
    T_BTIF_BR_LINK_ROLE      role;
} T_BTIF_ACL_CFG_PARAM_LINK_ROLE;

typedef struct
{
    uint16_t    min_interval;
    uint16_t    max_interval;
    uint16_t    sniff_attempt;
    uint16_t    sniff_tout;
    uint16_t    max_latency;
    uint16_t    min_remote_tout;
    uint16_t    min_local_tout;
} T_BTIF_ACL_CFG_PARAM_SNIFF_MODE;

typedef struct
{
    uint16_t        type;
} T_BTIF_ACL_CFG_PKT_TYPE;

typedef union
{
    T_BTIF_ACL_CFG_PARAM_LINK_POLICY    link_policy;
    T_BTIF_ACL_CFG_PARAM_LINK_SUPV_TOUT supv_tout;
    T_BTIF_ACL_CFG_PARAM_LINK_ROLE      link_role;
    T_BTIF_ACL_CFG_PARAM_SNIFF_MODE     sniff_mode;
    T_BTIF_ACL_CFG_PKT_TYPE             pkt_type;
} T_BTIF_ACL_CFG_PARAM;

typedef enum
{
    BTIF_UUID16,
    BTIF_UUID32,
    BTIF_UUID128
} T_BTIF_UUID_TYPE;

typedef union
{
    uint16_t    uuid_16;
    uint32_t    uuid_32;
    uint8_t     uuid_128[16];
} T_BTIF_UUID_DATA;

typedef enum
{
    BTIF_SDP_DISCOV_TYPE_BR,
    BTIF_SDP_DISCOV_TYPE_DID
} T_BTIF_SDP_DISCOV_TYPE;

typedef enum
{
    BTIF_L2C_CONN_ACCEPT               = L2C_CONN_ACCEPT,
    BTIF_L2C_CONN_PENDING              = (L2C_ERR | L2C_ERR_PENDING),
    BTIF_L2C_CONN_INVALID_PSM          = (L2C_ERR | L2C_ERR_INVALID_PSM),
    BTIF_L2C_CONN_SECURITY_BLOCK       = (L2C_ERR | L2C_ERR_SECURITY_BLOCK),
    BTIF_L2C_CONN_NO_RESOURCE          = (L2C_ERR | L2C_ERR_NO_RESOURCE),
    BTIF_L2C_CONN_INVALID_PARAM        = (L2C_ERR | L2C_ERR_INVALID_PARAM),
    BTIF_L2C_CONN_INVALID_SOURCE_CID   = (L2C_ERR | L2C_ERR_INVALID_SOURCE_CID),
    BTIF_L2C_CONN_SOURCE_CID_ALLOCATED = (L2C_ERR | L2C_ERR_SOURCE_CID_ALLOCATED)
} T_BTIF_L2C_CONN_CAUSE;

typedef enum
{
    BTIF_SCO_PKT_STATUS_OK           = 0,
    BTIF_SCO_PKT_STATUS_INVALID      = 1,
    BTIF_SCO_PKT_STATUS_NO_DATA      = 2,
    BTIF_SCO_PKT_STATUS_PARTIAL_LOST = 3
} T_BTIF_SCO_PKT_STATUS;

/**type definitions */
typedef enum
{
    BTIF_MSG_REGISTER_REQ = 0x01,
    BTIF_MSG_REGISTER_RSP,
    BTIF_MSG_RELEASE_REQ,
    BTIF_MSG_RELEASE_RSP,
    BTIF_MSG_ACT_INFO,
    BTIF_MSG_DEV_CFG_REQ,
    BTIF_MSG_DEV_CFG_RSP,

    BTIF_MSG_ACL_STATUS_INFO,
    BTIF_MSG_ACL_PRIORITY_SET_REQ,
    BTIF_MSG_ACL_PRIORITY_SET_RSP,          /**< Response of setting acl priority. */

    BTIF_MSG_READ_RSSI_REQ,
    BTIF_MSG_READ_RSSI_RSP,
    BTIF_MSG_VENDOR_CMD_REQ,
    BTIF_MSG_VENDOR_CMD_RSP,
    BTIF_MSG_VENDOR_EVT_INFO,

    BTIF_MSG_PAIRABLE_MODE_SET_REQ,
    BTIF_MSG_PAIRABLE_MODE_SET_RSP,
    BTIF_MSG_USER_PASSKEY_REQ_IND,
    BTIF_MSG_USER_PASSKEY_REQ_CFM,
    BTIF_MSG_USER_PASSKEY_REQ_REPLY_REQ,
    BTIF_MSG_USER_PASSKEY_REQ_REPLY_RSP,
    BTIF_MSG_USER_PASSKEY_NOTIF_INFO,
    BTIF_MSG_AUTHEN_RESULT_IND,
    BTIF_MSG_AUTHEN_RESULT_CFM,
    BTIF_MSG_AUTHEN_KEY_REQ_IND,
    BTIF_MSG_AUTHEN_KEY_REQ_CFM,
    BTIF_MSG_USER_CFM_REQ_IND,
    BTIF_MSG_USER_CFM_REQ_CFM,
    BTIF_MSG_KEYPRESS_NOTIF_REQ,
    BTIF_MSG_KEYPRESS_NOTIF_RSP,
    BTIF_MSG_KEYPRESS_NOTIF_INFO,
    BTIF_MSG_HW_ERROR_INFO,

    BTIF_MSG_SW_RESET_REQ,
    BTIF_MSG_SW_RESET_RSP,

    BTIF_MSG_REMOTE_OOB_REQ_IND = 0x0100,
    BTIF_MSG_REMOTE_OOB_REQ_CFM,

    /** GATT ---*/
    /** server */
    BTIF_MSG_GATT_SRV_REG_REQ,
    BTIF_MSG_GATT_SRV_REG_RSP,
    BTIF_MSG_GATT_ATTR_UPDATE_REQ,
    BTIF_MSG_GATT_ATTR_UPDATE_RSP,
    BTIF_MSG_GATT_ATTR_UPDATE_STATUS_IND,
    BTIF_MSG_GATT_ATTR_UPDATE_STATUS_CFM,
    BTIF_MSG_GATT_ATTR_READ_IND,
    BTIF_MSG_GATT_ATTR_READ_CFM,
    BTIF_MSG_GATT_ATTR_WRITE_REQ_IND,
    BTIF_MSG_GATT_ATTR_WRITE_REQ_CFM,
    BTIF_MSG_GATT_ATTR_PREP_WRITE_IND,
    BTIF_MSG_GATT_ATTR_PREP_WRITE_CFM,
    BTIF_MSG_GATT_ATTR_EXEC_WRITE_IND,
    BTIF_MSG_GATT_ATTR_EXEC_WRITE_CFM,
    BTIF_MSG_GATT_ATTR_WRITE_CMD_INFO,//0x110
    BTIF_MSG_GATT_ATTR_CCCD_INFO,
    /** client */
    BTIF_MSG_GATT_DISCOVERY_REQ,
    BTIF_MSG_GATT_DISCOVERY_RSP,
    BTIF_MSG_GATT_DISCOVERY_IND,
    BTIF_MSG_GATT_DISCOVERY_CFM,
    BTIF_MSG_GATT_ATTR_READ_REQ,
    BTIF_MSG_GATT_ATTR_READ_RSP,
    BTIF_MSG_GATT_ATTR_READ_MULTI_REQ,
    BTIF_MSG_GATT_ATTR_READ_MULTI_RSP,
    BTIF_MSG_GATT_ATTR_WRITE_REQ,
    BTIF_MSG_GATT_ATTR_WRITE_RSP,
    BTIF_MSG_GATT_ATTR_PREP_WRITE_REQ,
    BTIF_MSG_GATT_ATTR_PREP_WRITE_RSP,
    BTIF_MSG_GATT_ATTR_EXEC_WRITE_REQ,
    BTIF_MSG_GATT_ATTR_EXEC_WRITE_RSP,

    BTIF_MSG_GATT_ATTR_IND,//0x120
    BTIF_MSG_GATT_ATTR_CFM,
    BTIF_MSG_GATT_ATTR_NOTIF_INFO,

    /** common */
    BTIF_MSG_GATT_SECURITY_REQ,
    BTIF_MSG_GATT_SECURITY_RSP,
    BTIF_MSG_GATT_SERVER_STORE_IND,
    BTIF_MSG_GATT_SERVER_STORE_CFM,
    BTIF_MSG_GATT_MTU_SIZE_INFO,
    /** end of GATT ---*/

    BTIF_MSG_LE_CONN_REQ,
    BTIF_MSG_LE_CONN_RSP,
    BTIF_MSG_LE_CONN_IND,
    BTIF_MSG_LE_CONN_CFM,
    BTIF_MSG_LE_CONN_CMPL_INFO,
    BTIF_MSG_LE_DISCONN_REQ,
    BTIF_MSG_LE_DISCONN_RSP,
    BTIF_MSG_LE_DISCONN_IND,
    BTIF_MSG_LE_DISCONN_CFM,//0x130

    BTIF_MSG_LE_ADV_REQ,
    BTIF_MSG_LE_ADV_RSP,
    BTIF_MSG_LE_ADV_PARAM_SET_REQ,
    BTIF_MSG_LE_ADV_PARAM_SET_RSP,
    BTIF_MSG_LE_ADV_DATA_SET_REQ,
    BTIF_MSG_LE_ADV_DATA_SET_RSP,
    BTIF_MSG_LE_SCAN_REQ,
    BTIF_MSG_LE_SCAN_RSP,
    BTIF_MSG_LE_SCAN_PARAM_SET_REQ,
    BTIF_MSG_LE_SCAN_PARAM_SET_RSP,
    BTIF_MSG_LE_SCAN_INFO,
    BTIF_MSG_LE_MODIFY_WHITE_LIST_REQ,
    BTIF_MSG_LE_MODIFY_WHITE_LIST_RSP,
    BTIF_MSG_LE_CONN_UPDATE_REQ,
    BTIF_MSG_LE_CONN_UPDATE_RSP,
    BTIF_MSG_LE_CONN_UPDATE_IND,//0x140
    BTIF_MSG_LE_CONN_UPDATE_CFM,
    BTIF_MSG_LE_CONN_PARAM_INFO,
    BTIF_MSG_LE_CONN_PARAM_REQ_EVT_INFO,
    BTIF_MSG_LE_CONN_PARAM_REQ_REPLY_REQ,
    BTIF_MSG_LE_CONN_PARAM_REQ_REPLY_RSP,
    BTIF_MSG_LE_CONN_PARAM_REQ_NEG_REPLY_REQ,
    BTIF_MSG_LE_CONN_PARAM_REQ_NEG_REPLY_RSP,

    BTIF_MSG_LE_CREDIT_BASED_CONN_REQ,
    BTIF_MSG_LE_CREDIT_BASED_CONN_RSP,
    BTIF_MSG_LE_CREDIT_BASED_CONN_IND,
    BTIF_MSG_LE_CREDIT_BASED_CONN_CFM,
    BTIF_MSG_LE_CREDIT_BASED_DISCONN_REQ,
    BTIF_MSG_LE_CREDIT_BASED_DISCONN_RSP,
    BTIF_MSG_LE_CREDIT_BASED_DISCONN_IND,
    BTIF_MSG_LE_CREDIT_BASED_DISCONN_CFM,
    BTIF_MSG_LE_SEND_CREDIT_REQ,//0x150
    BTIF_MSG_LE_SEND_CREDIT_RSP,
    BTIF_MSG_LE_CREDIT_BASED_DATA_REQ,
    BTIF_MSG_LE_CREDIT_BASED_DATA_RSP,
    BTIF_MSG_LE_CREDIT_BASED_DATA_IND,
    BTIF_MSG_LE_CREDIT_BASED_DATA_CFM,
    BTIF_MSG_LE_CREDIT_BASED_CONN_CMPL_INFO,
    BTIF_MSG_LE_CREDIT_BASED_CONN_CREDIT_INFO,
    BTIF_MSG_LE_CREDIT_BASED_SECURITY_REG_REQ,
    BTIF_MSG_LE_CREDIT_BASED_SECURITY_REG_RSP,
    BTIF_MSG_LE_CREDIT_BASED_PSM_REG_REQ,
    BTIF_MSG_LE_CREDIT_BASED_PSM_REG_RSP,
    BTIF_MSG_LE_SET_CHANN_CLASSIF_REQ,
    BTIF_MSG_LE_SET_CHANN_CLASSIF_RSP,
    BTIF_MSG_LE_READ_CHANN_MAP_REQ,
    BTIF_MSG_LE_READ_CHANN_MAP_RSP,
    BTIF_MSG_LE_RECEIVER_TEST_REQ,
    BTIF_MSG_LE_RECEIVER_TEST_RSP,
    BTIF_MSG_LE_TRANSMITTER_TEST_REQ,//0x162
    BTIF_MSG_LE_TRANSMITTER_TEST_RSP,
    BTIF_MSG_LE_TEST_END_REQ,
    BTIF_MSG_LE_TEST_END_RSP,
    BTIF_MSG_LE_READ_ADV_TX_POWER_REQ,
    BTIF_MSG_LE_READ_ADV_TX_POWER_RSP,
    BTIF_MSG_LE_SET_RAND_ADDR_REQ,
    BTIF_MSG_LE_SET_RAND_ADDR_RSP,
    BTIF_MSG_LE_READ_MAX_DATA_LEN_REQ,
    BTIF_MSG_LE_READ_MAX_DATA_LEN_RSP,
    BTIF_MSG_LE_READ_DEFAULT_DATA_LEN_REQ,
    BTIF_MSG_LE_READ_DEFAULT_DATA_LEN_RSP,
    BTIF_MSG_LE_WRITE_DEFAULT_DATA_LEN_REQ,
    BTIF_MSG_LE_WRITE_DEFAULT_DATA_LEN_RSP,
    BTIF_MSG_LE_SET_DATA_LEN_REQ,//0x172
    BTIF_MSG_LE_SET_DATA_LEN_RSP,
    BTIF_MSG_LE_DATA_LEN_CHANGE_INFO,
    BTIF_MSG_LE_MODIFY_RESOLV_LIST_REQ,
    BTIF_MSG_LE_MODIFY_RESOLV_LIST_RSP,
    BTIF_MSG_LE_READ_PEER_RESOLV_ADDR_REQ,
    BTIF_MSG_LE_READ_PEER_RESOLV_ADDR_RSP,
    BTIF_MSG_LE_READ_LOCAL_RESOLV_ADDR_REQ,
    BTIF_MSG_LE_READ_LOCAL_RESOLV_ADDR_RSP,
    BTIF_MSG_LE_SET_RESOLUTION_ENABLE_REQ,
    BTIF_MSG_LE_SET_RESOLUTION_ENABLE_RSP,
    BTIF_MSG_LE_SET_RESOLV_PRIV_ADDR_TOUT_REQ,
    BTIF_MSG_LE_SET_RESOLV_PRIV_ADDR_TOUT_RSP,
    BTIF_MSG_LE_CONFIG_LOCAL_IRK_REQ,
    BTIF_MSG_LE_CONFIG_LOCAL_IRK_RSP,
    BTIF_MSG_LE_SET_PRIVACY_MODE_REQ,
    BTIF_MSG_LE_SET_PRIVACY_MODE_RSP,//0x182
    BTIF_MSG_LE_DIRECT_ADV_INFO,
    BTIF_MSG_LE_HIGH_DUTY_ADV_TOUT_INFO,

    BTIF_MSG_LE_SET_ADV_SET_RAND_ADDR_REQ,
    BTIF_MSG_LE_SET_ADV_SET_RAND_ADDR_RSP,

    BTIF_MSG_LE_EXT_ADV_PARAM_SET_REQ,
    BTIF_MSG_LE_EXT_ADV_PARAM_SET_RSP,
    BTIF_MSG_LE_EXT_ADV_DATA_SET_REQ,
    BTIF_MSG_LE_EXT_ADV_DATA_SET_RSP,
    BTIF_MSG_LE_EXT_ADV_ENABLE_REQ,
    BTIF_MSG_LE_EXT_ADV_ENABLE_RSP,

    BTIF_MSG_LE_EXT_SCAN_PARAM_SET_REQ,
    BTIF_MSG_LE_EXT_SCAN_PARAM_SET_RSP,
    BTIF_MSG_LE_EXT_SCAN_REQ,
    BTIF_MSG_LE_EXT_SCAN_RSP,//0x190

    BTIF_MSG_LE_SET_DEFAULT_PHY_REQ,
    BTIF_MSG_LE_SET_DEFAULT_PHY_RSP,

    BTIF_MSG_LE_SET_PHY_REQ,
    BTIF_MSG_LE_SET_PHY_RSP,

    BTIF_MSG_LE_ENHANCED_RECEIVER_TEST_REQ,
    BTIF_MSG_LE_ENHANCED_RECEIVER_TEST_RSP,
    BTIF_MSG_LE_ENHANCED_TRANSMITTER_TEST_REQ,
    BTIF_MSG_LE_ENHANCED_TRANSMITTER_TEST_RSP,

    BTIF_MSG_LE_MODIFY_PERIODIC_ADV_LIST_REQ,
    BTIF_MSG_LE_MODIFY_PERIODIC_ADV_LIST_RSP,

    BTIF_MSG_LE_READ_RF_PATH_COMPENSATION_REQ,
    BTIF_MSG_LE_READ_RF_PATH_COMPENSATION_RSP,

    BTIF_MSG_LE_WRITE_RF_PATH_COMPENSATION_REQ,
    BTIF_MSG_LE_WRITE_RF_PATH_COMPENSATION_RSP,

    BTIF_MSG_LE_MODIFY_ADV_SET_REQ,
    BTIF_MSG_LE_MODIFY_ADV_SET_RSP,//0x1A0

    BTIF_MSG_LE_SET_PERIODIC_ADV_PARAM_REQ,
    BTIF_MSG_LE_SET_PERIODIC_ADV_PARAM_RSP,
    BTIF_MSG_LE_SET_PERIODIC_ADV_DATA_REQ,
    BTIF_MSG_LE_SET_PERIODIC_ADV_DATA_RSP,
    BTIF_MSG_LE_SET_PERIODIC_ADV_ENABLE_REQ,
    BTIF_MSG_LE_SET_PERIODIC_ADV_ENABLE_RSP,
    BTIF_MSG_LE_PERIODIC_ADV_CREATE_SYNC_REQ,
    BTIF_MSG_LE_PERIODIC_ADV_CREATE_SYNC_RSP,
    BTIF_MSG_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL_REQ,
    BTIF_MSG_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL_RSP,
    BTIF_MSG_LE_PERIODIC_ADV_TERMINATE_SYNC_REQ,
    BTIF_MSG_LE_PERIODIC_ADV_TERMINATE_SYNC_RSP,

    BTIF_MSG_LE_PHY_UPDATE_INFO,
    BTIF_MSG_LE_EXT_ADV_REPORT_INFO,
    BTIF_MSG_LE_PERIODIC_ADV_SYNC_ESTABLISHED_INFO,
    BTIF_MSG_LE_PERIODIC_ADV_REPORT_INFO,//0x1B0
    BTIF_MSG_LE_PERIODIC_ADV_SYNC_LOST_INFO,
    BTIF_MSG_LE_SCAN_TIMEOUT_INFO,
    BTIF_MSG_LE_ADV_SET_TERMINATED_INFO,
    BTIF_MSG_LE_SCAN_REQ_RECEIVED_INFO,
    BTIF_MSG_LE_LOCAL_DEV_INFO,
    BTIF_MSG_LE_ENABLE_EXT_ADV_MODE_REQ,
    BTIF_MSG_LE_ENABLE_EXT_ADV_MODE_RSP,
    BTIF_MSG_LE_REMOTE_FEATS_INFO,
    BTIF_MSG_GATT_ATTR_WRITE_CMD_SIGNED_INFO,
    BTIF_MSG_LE_SIGNED_STATUS_INFO,

    BTIF_MSG_JUST_WORK_REQ_IND,
    BTIF_MSG_JUST_WORK_REQ_CFM,

    BTIF_MSG_USER_PASSKEY_NOTIF_IND,
    BTIF_MSG_USER_PASSKEY_NOTIF_CFM,

    BTIF_MSG_GATT_SRV2_REG_REQ,

#if F_BT_LE_6_0_AOA_AOD_SUPPORT
    BTIF_MSG_LE_SET_SUPPL_TEST_PARAMS_REQ = 0x0300,
    BTIF_MSG_LE_SET_SUPPL_TEST_PARAMS_RSP,
    BTIF_MSG_LE_SET_CONNLESS_SUPPL_TRANSMIT_PARAMS_REQ,
    BTIF_MSG_LE_SET_CONNLESS_SUPPL_TRANSMIT_PARAMS_RSP,
    BTIF_MSG_LE_SET_CONNLESS_SUPPL_TRANSMIT_ENABLE_REQ,
    BTIF_MSG_LE_SET_CONNLESS_SUPPL_TRANSMIT_ENABLE_RSP,
    BTIF_MSG_LE_SET_CONNLESS_IQ_SAMPLING_ENABLE_REQ,
    BTIF_MSG_LE_SET_CONNLESS_IQ_SAMPLING_ENABLE_RSP,
    BTIF_MSG_LE_SET_CONN_SUPPL_PARAMS_REQ,
    BTIF_MSG_LE_SET_CONN_SUPPL_PARAMS_RSP,
    BTIF_MSG_LE_CONN_SUPPL_REQUEST_ENABLE_REQ,
    BTIF_MSG_LE_CONN_SUPPL_REQUEST_ENABLE_RSP,
    BTIF_MSG_LE_CONN_SUPPL_RESPONSE_ENABLE_REQ,
    BTIF_MSG_LE_CONN_SUPPL_RESPONSE_ENABLE_RSP,
    BTIF_MSG_LE_READ_ANTENNA_INFORMATION_REQ,
    BTIF_MSG_LE_READ_ANTENNA_INFORMATION_RSP,

    BTIF_MSG_LE_CONNLESS_IQ_REPORT_INFO,
    BTIF_MSG_LE_CONN_IQ_REPORT_INFO,
    BTIF_MSG_LE_SUPPL_REQUEST_FAILED_INFO,
#endif

    BTIF_MSG_RADIO_MODE_SET_REQ = 0x1000,
    BTIF_MSG_RADIO_MODE_SET_RSP,

    BTIF_MSG_INQUIRY_REQ,
    BTIF_MSG_INQUIRY_RSP,
    BTIF_MSG_INQUIRY_RESULT_INFO,
    BTIF_MSG_INQUIRY_CMPL_INFO,
    BTIF_MSG_INQUIRY_CANCEL_REQ,
    BTIF_MSG_INQUIRY_CANCEL_RSP,

    BTIF_MSG_REMOTE_NAME_REQ,
    BTIF_MSG_REMOTE_NAME_RSP,

    BTIF_MSG_SDP_DISCOV_REQ,
    BTIF_MSG_SDP_DISCOV_RSP,
    BTIF_MSG_SDP_ATTR_IND,
    BTIF_MSG_SDP_ATTR_CFM,
    BTIF_MSG_DID_ATTR_IND,
    BTIF_MSG_DID_ATTR_CFM,
    BTIF_MSG_SDP_DISCOV_CMPL_INFO,
    BTIF_MSG_STOP_SDP_DISCOV_REQ,
    BTIF_MSG_STOP_SDP_DISCOV_RSP,

    BTIF_MSG_AUTHEN_REQ,
    BTIF_MSG_AUTHEN_RSP,
    BTIF_MSG_AUTHOR_REQ_IND,
    BTIF_MSG_AUTHOR_REQ_CFM,
    BTIF_MSG_PIN_CODE_REQ_IND,
    BTIF_MSG_PIN_CODE_REQ_CFM,
    BTIF_MSG_LEGACY_REMOTE_OOB_REQ_IND,
    BTIF_MSG_LEGACY_REMOTE_OOB_REQ_CFM,
    BTIF_MSG_LOCAL_OOB_REQ,
    BTIF_MSG_LOCAL_OOB_RSP,

    BTIF_MSG_ACL_CFG_REQ,
    BTIF_MSG_ACL_CFG_RSP,

    BTIF_MSG_SDP_REG_REQ,
    BTIF_MSG_SDP_REG_RSP,
    BTIF_MSG_L2C_PROTO_REG_REQ,
    BTIF_MSG_L2C_PROTO_REG_RSP,
    BTIF_MSG_L2C_CONN_REQ,
    BTIF_MSG_L2C_CONN_RSP,
    BTIF_MSG_L2C_CONN_IND,
    BTIF_MSG_L2C_CONN_CFM,
    BTIF_MSG_L2C_CONN_CMPL_INFO,
    BTIF_MSG_L2C_DATA_REQ,
    BTIF_MSG_L2C_DATA_RSP,
    BTIF_MSG_L2C_DATA_IND,
    BTIF_MSG_L2C_DATA_CFM,
    BTIF_MSG_L2C_DISCONN_REQ,
    BTIF_MSG_L2C_DISCONN_RSP,
    BTIF_MSG_L2C_DISCONN_IND,
    BTIF_MSG_L2C_DISCONN_CFM,
    BTIF_MSG_L2C_SET_LOCAL_BUSY_REQ,
    BTIF_MSG_L2C_SET_LOCAL_BUSY_RSP,

    BTIF_MSG_L2C_SEC_REG_REQ,
    BTIF_MSG_L2C_SEC_REG_RSP,
    BTIF_MSG_RFC_AUTHEN_REQ,
    BTIF_MSG_RFC_AUTHEN_RSP,

    BTIF_MSG_SCO_CONN_REQ,
    BTIF_MSG_SCO_CONN_RSP,
    BTIF_MSG_SCO_CONN_IND,
    BTIF_MSG_SCO_CONN_CFM,
    BTIF_MSG_SCO_CONN_CMPL_INFO,
    BTIF_MSG_SCO_DATA_REQ,
    BTIF_MSG_SCO_DATA_RSP,
    BTIF_MSG_SCO_DATA_IND,
    BTIF_MSG_SCO_DATA_CFM,
    BTIF_MSG_SCO_DISCONN_REQ,
    BTIF_MSG_SCO_DISCONN_RSP,
    BTIF_MSG_SCO_DISCONN_IND,
    BTIF_MSG_SCO_DISCONN_CFM,

    BTIF_MSG_ACL_DISCONN_REQ,
    BTIF_MSG_ACL_DISCONN_RSP,

    BTIF_MSG_SETUP_QOS_REQ,
    BTIF_MSG_SETUP_QOS_RSP,

    /*connectionless slave broadcast*/
#if F_BT_CSB_SUPPORT
    BTIF_MSG_CSB_SET_REQ,
    BTIF_MSG_CSB_RECEIVE_SET_REQ,
    BTIF_MSG_CSB_DATA_IND,
#endif

    BTIF_MSG_IDLE,
} T_BTIF_MSG_TYPE;

typedef struct
{
    uint32_t    version;
    uint16_t    cause;
} T_BTIF_REGISTER_RSP;

typedef struct
{
    uint16_t    cause;
} T_BTIF_RELEASE_RSP;

typedef struct
{
    uint8_t     local_addr[6];
    uint16_t    cause;
} T_BTIF_ACT_INFO;

typedef struct
{
    T_BTIF_DEV_CFG_OPCODE   op;
    uint16_t                cause;
} T_BTIF_DEV_CFG_SET_RSP;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    T_BTIF_ACL_STATUS           status;
    T_BTIF_ACL_STATUS_PARAM     p;
} T_BTIF_ACL_STATUS_INFO;

typedef struct
{
    uint8_t         bd_addr[6];
    uint8_t         conn_type; /* 0: BREDR ACL; 1: SCO; 2: LE */
    bool            is_high_priority;
    uint16_t        cause;
} T_BTIF_ACL_PRIORITY_SET_RSP;

typedef struct
{
    uint8_t                 bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE remote_addr_type;
    uint16_t                cause;
    int8_t                  rssi;
} T_BTIF_READ_RSSI_RSP;

typedef struct
{
    uint16_t    command;
    uint16_t    cause;
    bool        is_cmpl_evt;
    uint8_t     param_len;
    uint8_t     param[1];
} T_BTIF_VENDOR_CMD_RSP;

typedef struct
{
    uint8_t     param_len;
    uint8_t     param[1];
} T_BTIF_VENDOR_EVT_INFO;

typedef struct
{
    uint16_t    cause;
} T_BTIF_PAIRABLE_MODE_SET_RSP;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    bool                        key_press;
} T_BTIF_USER_PASSKEY_REQ_IND;

typedef struct
{
    uint16_t    cause;
} T_BTIF_USER_PASSKEY_REQ_REPLY_RSP;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    uint32_t                    display_value;
} T_BTIF_USER_PASSKEY_NOTIF_INFO;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    uint32_t                    display_value;
} T_BTIF_USER_PASSKEY_NOTIF_IND;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
} T_BTIF_JUST_WORK_REQ_IND;


typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    uint8_t                     key_len;
    T_BTIF_KEY_TYPE             key_type;           /**< Key type. */
    uint8_t                     link_key[28];
    uint16_t                    cause;
} T_BTIF_AUTHEN_RESULT_IND;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    T_BTIF_KEY_TYPE             key_type;           /**< Requested Key type. */
} T_BTIF_AUTHEN_KEY_REQ_IND;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    uint32_t                    display_value;
} T_BTIF_USER_CFM_REQ_IND;

typedef struct
{
    uint16_t                    cause;
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
} T_BTIF_KEYPRESS_NOTIF_RSP;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    T_BTIF_KEYPRESS_NOTIF_TYPE  event_type;
} T_BTIF_KEYPRESS_NOTIF_INFO;

typedef struct
{
    uint8_t     hw_code;
} T_BTIF_HW_ERROR_INFO;

typedef struct
{
    uint8_t                 bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE addr_type;
} T_BTIF_REMOTE_OOB_REQ_IND;

typedef struct
{
    void       *p_srv_handle;
    uint16_t    cause;
} T_BTIF_GATT_SRV_REG_RSP;

typedef struct
{
    void          *p_srv_handle;
    void          *p_req_handle;
    uint16_t       link_id;
    uint16_t       attr_index;
    uint16_t       cause;
} T_BTIF_GATT_ATTR_UPDATE_RSP;

typedef struct
{
    void                       *p_srv_handle;
    void                       *p_req_handle;
    uint16_t                    link_id;
    uint16_t                    attr_index;
    uint16_t                    cause;
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
} T_BTIF_GATT_ATTR_UPDATE_STATUS_IND;

typedef struct
{
    uint16_t       link_id;
    void          *p_srv_handle;
    uint16_t       attr_index;
    uint16_t       read_offset;
} T_BTIF_GATT_ATTR_READ_IND;

typedef struct
{
    uint16_t       link_id;
    void          *p_srv_handle;
    uint16_t       attr_index;
    uint16_t       attr_len;
    uint16_t       handle;
    uint16_t       write_offset;
    uint16_t       gap;
    uint8_t        data[1];
} T_BTIF_GATT_ATTR_WRITE_IND;

typedef struct
{
    uint16_t    link_id;
    uint8_t     flags;      /**<  0x00-Cancel all writes, 0x01-write all values */
} T_BTIF_GATT_ATTR_EXEC_WRITE_IND;

typedef struct
{
    uint16_t       link_id;
    void          *p_srv_handle;
    uint16_t       count;              /**< nbr. of attr_index/CCCD value pairs */
    uint16_t       gap;                /**< offset of first pair in data[]       */
    uint8_t        data[1];
} T_BTIF_GATT_ATTR_CCCD_INFO;

typedef struct
{
    uint16_t                    link_id;
    T_BTIF_GATT_DISCOVERY_TYPE  type;
    uint16_t                    cause;
} T_BTIF_GATT_DISCOVERY_RSP;

typedef struct
{
    uint16_t                    link_id;
    T_BTIF_GATT_DISCOVERY_TYPE  type;
    uint16_t                    cause;
    uint16_t                    elem_cnt;
    uint16_t                    elem_len;
    uint16_t                    gap;
    uint8_t                     list[1];
} T_BTIF_GATT_DISCOVERY_IND;

typedef struct
{
    uint16_t                link_id;
    T_BTIF_GATT_READ_TYPE   read_type;
    uint16_t                cause;
    uint16_t                read_offset;
    uint16_t                total_len;
    uint16_t                attr_len;
    uint16_t                num_handle;
    uint16_t                gap;
    uint8_t                 handles[1];
} T_BTIF_GATT_ATTR_READ_RSP;

typedef struct
{
    uint16_t        link_id;        /**<  local link ID. */
    uint16_t        cause;          /**<  More detailed result information. */
    uint16_t        total_len;      /**<  Total number of bytes stored in the value[] array. */
    uint16_t        gap;            /**<  offset of first handle in value[] */
    uint8_t         value[1];       /**<  Array of attribute values read. */
} T_BTIF_GATT_ATTR_READ_MULTI_RSP;

typedef struct
{
    uint16_t                link_id;
    T_BTIF_GATT_WRITE_TYPE  write_type;
    uint16_t                attr_handle;
    uint16_t                cause;
    uint16_t                attr_len;       /**< attrib. value length */
    uint16_t                gap;            /**< offset of attrib. value in data[] */
    uint8_t                 data[1];
} T_BTIF_GATT_ATTR_WRITE_RSP;

typedef struct
{
    uint16_t    link_id;        /**<  local link ID.  */
    uint16_t    cause;          /**<  More detailed result information. */
    uint16_t    write_offset;   /**<  The offset of the first octet to be written.  */
    uint16_t    attr_len;       /**<  attrib. value length.  */
    uint16_t    gap;            /**<  offset of attrib. value in data[].  */
    uint8_t     data[1];        /**<  attrib. value in data[].  */
} T_BTIF_GATT_ATTR_PREP_WRITE_RSP;

typedef struct
{
    uint16_t    link_id;    /**<  local link ID.  */
    uint16_t    cause;      /**<  More detailed result information. */
} T_BTIF_GATT_ATTR_EXEC_WRITE_RSP;

typedef struct
{
    uint16_t       link_id;
    uint16_t       attr_handle;
    uint16_t       attr_len;        /**< attrib. value length */
    uint16_t       gap;             /**< offset of attrib. value in data[] */
    uint8_t        data[1];
} T_BTIF_GATT_ATTR_IND;

typedef T_BTIF_GATT_ATTR_IND T_BTIF_GATT_ATTR_NOTIF_INFO;

typedef struct
{
    uint16_t            link_id;
    T_BTIF_KEY_TYPE     key_type;
    uint8_t             key_size;
    uint16_t            cause;
} T_BTIF_GATT_SECURITY_RSP;

typedef struct
{
    T_BTIF_GATT_STORE_OPCODE    op;
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    uint8_t                     bd_addr[6];
    uint8_t                     data_len;
    uint8_t                    *p_data;
} T_BTIF_GATT_SERVER_STORE_IND;

typedef struct
{
    uint16_t       link_id;
    uint16_t       mtu_size;
} T_BTIF_GATT_MTU_SIZE_INFO;

typedef struct
{
    uint16_t       link_id;
    uint16_t       cause;
    bool           update_local;
    uint32_t       local_sign_count;
    uint32_t       remote_sign_count;
} T_BTIF_LE_SIGNED_STATUS_INFO;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    uint16_t                    link_id;
    uint16_t                    cause;
} T_BTIF_LE_CONN_RSP;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
}  T_BTIF_LE_CONN_IND;

typedef struct
{
    uint16_t                    cause;
    uint16_t                    link_id;
    uint16_t                    mtu_size;
    uint8_t                     ds_credits;
    T_BTIF_LOCAL_ADDR_TYPE      own_addr_type;
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    uint8_t                     bd_addr[6];
    uint16_t                    interval;
    uint16_t                    latency;
    uint16_t                    supv_tout;
    T_BTIF_LE_PHY_TYPE          tx_phy;
    T_BTIF_LE_PHY_TYPE          rx_phy;
    uint8_t                     csa;
} T_BTIF_LE_CONN_CMPL_INFO;

#if F_BT_LE_READ_REMOTE_FEATS
typedef struct
{
    uint16_t                    cause;
    uint16_t                    link_id;
    uint8_t                     remote_feats[8];
} T_BTIF_LE_REMOTE_FEATS_INFO;
#endif

typedef struct
{
    uint16_t        link_id;
    uint16_t        cause;
    uint16_t        reason;
} T_BTIF_LE_DISCONN_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        cause;
} T_BTIF_LE_DISCONN_IND;

typedef struct
{
    T_BTIF_LE_ADV_MODE      adv_mode;
    uint16_t                cause;
} T_BTIF_LE_ADV_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_ADV_PARAM_SET_RSP;

typedef struct
{
    T_BTIF_LE_ADV_DATA_TYPE data_type;
    uint16_t                cause;
} T_BTIF_LE_ADV_DATA_SET_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_SCAN_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_SCAN_PARAM_SET_RSP;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    T_BTIF_LE_ADV_EVT_TYPE      adv_type;
    int8_t                      rssi;
    uint8_t                     data_len;
    uint8_t                     data[31];
} T_BTIF_LE_SCAN_INFO;

typedef struct
{
    T_BTIF_LE_WHITE_LIST_OP     operation;
    uint16_t                    cause;
} T_BTIF_LE_MODIFY_WHITE_LIST_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        cause;
} T_BTIF_LE_CONN_UPDATE_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        min_interval;
    uint16_t        max_interval;
    uint16_t        latency;
    uint16_t        supv_tout;
} T_BTIF_LE_CONN_UPDATE_IND;

typedef struct
{
    uint16_t        link_id;
    uint16_t        interval;
    uint16_t        latency;
    uint16_t        supv_tout;
} T_BTIF_LE_CONN_PARAM_INFO;

typedef struct
{
    uint16_t        link_id;
    uint16_t        min_interval;
    uint16_t        max_interval;
    uint16_t        latency;
    uint16_t        supv_tout;
} T_BTIF_LE_CONN_PARAM_REQ_EVT_INFO;

typedef struct
{
    uint16_t        cause;
    uint16_t        link_id;
} T_BTIF_LE_CONN_PARAM_REQ_REPLY_RSP;

typedef struct
{
    uint16_t        cause;
    uint16_t        link_id;
} T_BTIF_LE_CONN_PARAM_REQ_NEG_REPLY_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
    uint16_t        cause;
} T_BTIF_LE_CREDIT_BASED_CONN_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
} T_BTIF_LE_CREDIT_BASED_CONN_IND;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
    uint16_t        cause;
} T_BTIF_LE_CREDIT_BASED_DISCONN_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
    uint16_t        cause;
} T_BTIF_LE_CREDIT_BASED_DISCONN_IND;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
    uint16_t        cause;
} T_BTIF_LE_SEND_CREDIT_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
    uint16_t        cause;
} T_BTIF_LE_CREDIT_BASED_DATA_RSP;

typedef struct
{
    uint16_t        link_id;        /**<  local link ID.  */
    uint16_t        channel;        /**<  channel ID  */
    uint16_t        value_len;      /**<  value length  */
    uint16_t        gap;            /**<  offset of value in data[].  */
    uint8_t         data[1];        /**<  value.  */
} T_BTIF_LE_CREDIT_BASED_DATA_IND;

typedef struct
{
    uint16_t        cause;
    uint16_t        link_id;            /**<  local link ID.  */
    uint16_t        channel;            /**<  channel ID  */
    uint16_t        remote_mtu;         /**<  remote Maximum Transmission Unit */
    uint16_t        remote_credits;     /**<  remote initial credits*/
    uint16_t        max_ds_credits;     /**<  max downstream credits */
} T_BTIF_LE_CREDIT_BASED_CONN_CMPL_INFO;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
} T_BTIF_LE_CREDIT_BASED_CONN_CREDIT_INFO;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_CREDIT_BASED_SECURITY_REG_RSP;

typedef struct
{
    uint16_t        le_psm;
    uint16_t        cause;
} T_BTIF_LE_CREDIT_BASED_PSM_REG_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_SET_CHANN_CLASSIF_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        cause;
    uint8_t         chann_map[5];
} T_BTIF_LE_READ_CHANN_MAP_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_RECEIVER_TEST_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_TRANSMITTER_TEST_RSP;

typedef struct
{
    uint16_t        cause;
    uint16_t        num_pkts;
} T_BTIF_LE_TEST_END_RSP;

typedef struct
{
    uint16_t        cause;
    uint8_t         tx_power_level;
} T_BTIF_LE_READ_ADV_TX_POWER_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_SET_RAND_ADDR_RSP;

typedef struct
{
    uint16_t        cause;
    uint16_t        max_tx_oct;
    uint16_t        max_tx_time;
    uint16_t        max_rx_oct;
    uint16_t        max_rx_time;
} T_BTIF_LE_READ_MAX_DATA_LEN_RSP;

typedef struct
{
    uint16_t        cause;
    uint16_t        max_tx_oct;
    uint16_t        max_tx_time;
} T_BTIF_LE_READ_DEFAULT_DATA_LEN_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_WRITE_DEFAULT_DATA_LEN_RSP;

typedef struct
{
    uint16_t        cause;
    uint16_t        link_id;
} T_BTIF_LE_SET_DATA_LEN_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        max_tx_oct;
    uint16_t        max_tx_time;
    uint16_t        max_rx_oct;
    uint16_t        max_rx_time;
} T_BTIF_LE_DATA_LEN_CHANGE_INFO;

typedef struct
{
    T_BTIF_LE_RESOLVING_LIST_OP operation;
    uint16_t                    cause;
} T_BTIF_LE_MODIFY_RESOLV_LIST_RSP;

typedef struct
{
    uint16_t    cause;
    uint8_t     peer_resolv_addr[6];
} T_BTIF_LE_READ_PEER_RESOLV_ADDR_RSP;

typedef struct
{
    uint16_t    cause;
    uint8_t     local_resolv_addr[6];
} T_BTIF_LE_READ_LOCAL_RESOLV_ADDR_RSP;

typedef struct
{
    uint16_t    cause;
} T_BTIF_LE_SET_RESOLUTION_ENABLE_RSP;

typedef struct
{
    uint16_t    cause;
} T_BTIF_LE_SET_RESOLV_PRIV_ADDR_TOUT_RSP;

typedef struct
{
    uint16_t    cause;
    bool        generate;
    uint8_t     local_irk[16];
} T_BTIF_LE_CONFIG_LOCAL_IRK_RSP;

typedef struct
{
    uint16_t    cause;
} T_BTIF_LE_SET_PRIVACY_MODE_RSP;

typedef struct
{
    uint8_t                         bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE         remote_addr_type;
    uint8_t                         direct_bd_addr[6];
    T_BTIF_DIRECT_ADDR_TYPE         direct_addr_type;
    T_BTIF_LE_DIRECT_ADV_EVT_TYPE   direct_adv_type;
    int8_t                          rssi;
} T_BTIF_LE_DIRECT_ADV_INFO;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_HIGH_DUTY_ADV_TIMEOUT_INFO;
#if F_BT_LE_5_0_SUPPORT
#if F_BT_LE_5_0_AE_ADV_SUPPORT
typedef struct
{
    uint16_t        cause;
    uint8_t         adv_handle;
    uint8_t         rand_addr[6];
} T_BTIF_LE_SET_ADV_SET_RAND_ADDR_RSP;

typedef struct
{
    int8_t          selected_tx_power;
    uint16_t        cause;
} T_BTIF_LE_EXT_ADV_PARAM_SET_RSP;

typedef struct
{
    T_BTIF_LE_EXT_ADV_DATA_TYPE data_type;
    uint16_t                         cause;
} T_BTIF_LE_EXT_ADV_DATA_SET_RSP;

typedef struct
{
    T_BTIF_LE_EXT_ADV_MODE mode;
    uint16_t                    cause;
} T_BTIF_LE_EXT_ADV_ENABLE_RSP;
#endif

#if F_BT_LE_5_0_AE_SCAN_SUPPORT
typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_EXT_SCAN_PARAM_SET_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_EXT_SCAN_RSP;
#endif

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_SET_DEFAULT_PHY_RSP;

typedef struct
{
    uint16_t        cause;
    uint16_t        link_id;
} T_BTIF_LE_SET_PHY_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_ENHANCED_RECEIVER_TEST_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_ENHANCED_TRANSMITTER_TEST_RSP;

typedef struct
{
    T_BTIF_LE_PERIODIC_ADV_LIST_OP  op;
    uint16_t                        cause;
} T_BTIF_LE_MODIFY_PERIODIC_ADV_LIST_RSP;

typedef struct
{
    uint16_t    cause;
    int8_t      rf_tx_path_compensation;
    int8_t      rf_rx_path_compensation;
} T_BTIF_LE_READ_RF_PATH_COMPENSATION_RSP;

typedef struct
{
    uint16_t                cause;
} T_BTIF_LE_WRITE_RF_PATH_COMPENSATION_RSP;

typedef struct
{
    T_BTIF_LE_ADV_SET_OP    op;
    uint16_t                cause;
} T_BTIF_LE_MODIFY_ADV_SET_RSP;

typedef struct
{
    uint16_t                cause;
} T_BTIF_LE_SET_PERIODIC_ADV_PARAM_RSP;

typedef struct
{
    uint16_t                cause;
} T_BTIF_LE_SET_PERIODIC_ADV_DATA_RSP;

typedef struct
{
    uint16_t                cause;
} T_BTIF_LE_SET_PERIODIC_ADV_ENABLE_RSP;

typedef struct
{
    uint16_t                cause;
} T_BTIF_LE_PERIODIC_ADV_CREATE_SYNC_RSP;

typedef struct
{
    uint16_t                cause;
} T_BTIF_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL_RSP;

typedef struct
{
    uint16_t                cause;
} T_BTIF_LE_PERIODIC_ADV_TERMINATE_SYNC_RSP;

typedef struct
{
    uint16_t            cause;
    uint16_t            link_id;
    T_BTIF_LE_PHY_TYPE  tx_phy;
    T_BTIF_LE_PHY_TYPE  rx_phy;
} T_BTIF_LE_PHY_UPDATE_INFO;

typedef enum
{
    BTIF_ADV_EVENT_TYPE_DATA_STATUS_COMPLETE = 0x00,
    BTIF_ADV_EVENT_TYPE_DATA_STATUS_MORE     = 0x01,
    BTIF_ADV_EVENT_TYPE_DATA_STATUS_TRUNCATED = 0x02,
    BTIF_ADV_EVENT_TYPE_DATA_STATUS_RFU      = 0x03
} T_BTIF_EXT_ADV_EVT_TYPE_DATA_STATUS;

typedef struct
{
    uint16_t                            event_type;
    T_BTIF_EXT_ADV_EVT_TYPE_DATA_STATUS data_status;
    T_BTIF_REMOTE_ADDR_TYPE             addr_type;
    uint8_t                             bd_addr[6];
    T_BTIF_LE_PRIM_ADV_PHY_TYPE         primary_phy;
    uint8_t                             secondary_phy;
    uint8_t                             adv_sid;
    int8_t                              tx_power;
    int8_t                              rssi;
    uint16_t                            peri_adv_interval;
    T_BTIF_DIRECT_ADDR_TYPE             direct_addr_type;
    uint8_t                             direct_addr[6];
    uint8_t                             data_len;
    uint8_t                            *p_data; //max 229 bytes
} T_BTIF_LE_EXT_ADV_REPORT_INFO;

typedef struct
{
    uint16_t            cause;
    uint8_t             sync_handle;
    uint8_t             advertising_sid;
    uint8_t             advertiser_address_type;
    uint8_t             advertiser_address[6];
    T_BTIF_LE_PHY_TYPE  advertiser_phy;
    uint8_t             periodic_advertising_interval;
    uint8_t             advertiser_clock_accuracy;
} T_BTIF_LE_PERIODIC_ADV_SYNC_ESTABLISHED_INFO;

typedef struct
{
    uint16_t    sync_handle;
    int8_t      tx_power;
    int8_t      rssi;
    uint8_t     rfu;
    uint8_t     data_status;
    uint8_t     data_len;
    uint8_t    *p_data;
} T_BTIF_LE_PERIODIC_ADV_REPORT_INFO;

typedef struct
{
    uint8_t     sync_handle;
} T_BTIF_LE_PERIODIC_ADV_SYNC_LOST_INFO;

typedef struct
{
    uint16_t    cause;
    uint8_t     adv_handle;
    uint16_t    conn_handle;
    uint8_t     num_cmpl_ext_adv_evt;
} T_BTIF_LE_ADV_SET_TERMINATED_INFO;

typedef struct
{
    uint8_t     adv_handle;
    uint8_t     scanner_addr_type;
    uint8_t     scanner_addr[6];
} T_BTIF_LE_SCAN_REQ_RECEIVED_INFO;

typedef struct
{
    uint16_t    cause;
    uint16_t    le_max_ext_adv_len;
    uint8_t     le_max_num_of_adv_sets;
    uint8_t     le_periodic_adv_list_size;
} T_BTIF_LE_ENABLE_EXT_ADV_MODE_RSP;
#endif
typedef struct
{
    uint8_t     le_local_feats[8];
    uint16_t    le_ds_pool_id;
    uint16_t    le_ds_data_offset;
    uint8_t     le_ds_credits;
    uint8_t     le_max_wl_size;
#if F_BT_LE_PRIVACY_SUPPORT
    uint8_t     le_resolvable_list_size;
#endif
#if F_BT_LE_5_0_SUPPORT
    int8_t      le_min_tx_power;
    int8_t      le_max_tx_power;
#endif
} T_BTIF_LE_LOCAL_DEV_INFO;




typedef struct
{
    uint16_t    cause;
} T_BTIF_RADIO_MODE_SET_RSP;

typedef struct
{
    uint16_t    cause;
} T_BTIF_INQUIRY_RSP;

typedef struct
{
    uint8_t     bd_addr[6];
    uint32_t    cod;
    int8_t      rssi;
    uint8_t     name[BTIF_NAME_LENGTH];
} T_BTIF_INQUIRY_RESULT_INFO;

typedef struct
{
    uint16_t    cause;
} T_BTIF_INQUIRY_CMPL_INFO;

typedef struct
{
    uint16_t    cause;
} T_BTIF_INQUIRY_CANCEL_RSP;

typedef struct
{
    uint16_t    cause;
    uint8_t     bd_addr[6];
    uint8_t     name[BTIF_NAME_LENGTH];
} T_BTIF_REMOTE_NAME_RSP;

typedef struct
{
    uint8_t         bd_addr[6];
    uint16_t        cause;
} T_BTIF_SDP_DISCOV_RSP;

typedef struct
{
    uint8_t                 bd_addr[6];
    uint8_t                 server_channel;
    uint8_t                 supported_repos;
    uint16_t                l2c_psm;
    T_BTIF_UUID_TYPE        srv_class_uuid_type;
    T_BTIF_UUID_DATA        srv_class_uuid_data;
    uint16_t                remote_version;
    uint16_t                supported_feat;
    uint8_t                *p_ext_data;
} T_BTIF_SDP_ATTR_IND;

typedef struct
{
    uint16_t        specific_id;
    uint16_t        vendor_id;
    uint16_t        vendor_src;
    uint16_t        product_id;
    uint16_t        version;
    uint8_t         bd_addr[6];
} T_BTIF_DID_ATTR_IND;

typedef struct
{
    uint8_t         bd_addr[6];
    uint16_t        cause;
} T_BTIF_SDP_DISCOV_CMPL_INFO;

typedef struct
{
    uint8_t         bd_addr[6];
    uint16_t        cause;
} T_BTIF_STOP_SDP_DISCOV_RSP;

typedef struct
{
    uint8_t         bd_addr[6];
    uint16_t        cause;
} T_BTIF_AUTHEN_RSP;

typedef struct
{
    uint16_t        psm;                    /**< authorize protocol psm */
    uint16_t        server_chann;           /**< server channel for rfcomm, 0 for l2cap */
    uint16_t        uuid;                   /**< uuid to connect, used for outgoing */
    uint8_t         bd_addr[6];
    uint8_t         outgoing;               /**< 1:outgoing, 0:incoming */
} T_BTIF_AUTHOR_REQ_IND;

typedef struct
{
    uint8_t         bd_addr[6];
} T_BTIF_PIN_CODE_REQ_IND;

typedef struct
{
    uint8_t         bd_addr[6];
} T_BTIF_LEGACY_REMOTE_OOB_REQ_IND;

typedef struct
{
    uint8_t         data_c[16];      /**< Hash C value */
    uint8_t         data_r[16];      /**< Randomizer R value */
    uint16_t        cause;
} T_BTIF_LOCAL_OOB_RSP;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_ACL_CFG_OPCODE       op;
    uint16_t                    cause;
} T_BTIF_ACL_CFG_RSP;

typedef struct
{
    uint16_t    cause;
} T_BTIF_SDP_REG_RSP;

typedef struct
{
    uint16_t    psm;
    uint16_t    proto_id;
    uint16_t    cause;
} T_BTIF_L2C_PROTO_REG_RSP;

typedef struct
{
    uint16_t    cid;
    uint16_t    proto_id;
    uint16_t    cause;
    uint8_t     bd_addr[6];
} T_BTIF_L2C_CONN_RSP;

typedef struct
{
    uint16_t    cid;
    uint16_t    proto_id;
    uint8_t     bd_addr[6];
} T_BTIF_L2C_CONN_IND;

typedef struct
{
    uint16_t    cause;
    uint16_t    cid;
    uint16_t    remote_mtu;
    uint16_t    proto_id;
    uint16_t    ds_data_offset;
    uint8_t     bd_addr[6];
} T_BTIF_L2C_CONN_CMPL_INFO;

typedef struct
{
    uint16_t    proto_id; /**< Protocol ID assigned by upper layer. */
    uint16_t    cid;      /**< Local CID assigned by Bluetooth stack. */
    uint16_t    length;   /**< Length of L2CAP data. */
    uint16_t    gap;      /**< Offset from data parameter to the real L2CAP data. */
    uint8_t     data[1];  /**< The rest of message which contains real L2CAP data at offset of gap. */
} T_BTIF_L2C_DATA_IND;

typedef struct
{
    uint16_t    cid;
    uint16_t    proto_id;
    uint16_t    cause;
} T_BTIF_L2C_DISCONN_RSP;

typedef struct
{
    uint16_t    cid;
    uint16_t    proto_id;
    uint16_t    cause;
} T_BTIF_L2C_DISCONN_IND;

typedef struct
{
    uint16_t    cause;
} T_BTIF_L2C_SET_LOCAL_BUSY_RSP;

typedef struct
{
    uint16_t    psm;
    uint16_t    server_chann;
    uint8_t     active;
    uint16_t    uuid;
    uint16_t    cause;
} T_BTIF_L2C_SEC_REG_RSP;

typedef struct
{
    uint16_t    chann;
    uint16_t    dlci;
    uint16_t    cause;
    uint8_t     outgoing;
    uint8_t     bd_addr[6];
} T_BTIF_RFC_AUTHEN_RSP;

typedef struct
{
    uint16_t    cause;
    uint8_t     bd_addr[6];
} T_BTIF_SCO_CONN_RSP;

typedef struct
{
    uint8_t     bd_addr[6];
    uint8_t     is_esco;
} T_BTIF_SCO_CONN_IND;

typedef struct
{
    uint16_t    handle;
    uint16_t    cause;
    uint8_t     is_esco;
    uint8_t     air_mode;
    uint16_t    tx_length;
    uint16_t    rx_length;
    uint8_t     bd_addr[6];
} T_BTIF_SCO_CONN_CMPL_INFO;

typedef struct
{
    uint16_t                handle;
    T_BTIF_SCO_PKT_STATUS   status;
    void                   *p_buf;
    uint16_t                len;
} T_BTIF_SCO_DATA_IND;

typedef struct
{
    uint16_t    handle;
    uint16_t    cause;
    uint16_t    reason;
} T_BTIF_SCO_DISCONN_RSP;

typedef struct
{
    uint16_t    handle;
    uint16_t    cause;
} T_BTIF_SCO_DISCONN_IND;

typedef struct
{
    uint16_t    cause;
    uint16_t    reason;
    uint8_t     bd_addr[6];
} T_BTIF_ACL_DISCONN_RSP;

typedef struct
{
    uint16_t    cause;
    uint16_t    handle;
} T_BTIF_SETUP_QOS_RSP;



typedef enum
{
    SMP_OOB_INVALID_TYPE,
    SMP_OOB_PEER,
    SMP_OOB_LOCAL,
    SMP_OOB_BOTH
} T_BTIF_OOB_DATA_TYPE;

typedef struct
{
    uint8_t     present;
    uint8_t     rand[16];
    uint8_t     confirm[16];
    uint8_t     bd_addr_to[6];
    uint8_t     local_sk[32];
    uint8_t     local_pk[64];
} T_BTIF_LOCAL_OOB_DATA;

typedef struct
{
    uint8_t     present;
    uint8_t     rand[16];
    uint8_t     confirm[16];
    uint8_t     bd_addr_from[6];
} T_BTIF_PEER_OOB_DATA;

typedef struct
{
    T_BTIF_LOCAL_OOB_DATA   loc_oob_data;
    T_BTIF_PEER_OOB_DATA  peer_oob_data;
} T_SMP_SC_OOB_DATA;

#if F_BT_LE_6_0_AOA_AOD_SUPPORT

typedef struct
{
    uint16_t    cause;
    bool        is_receiver;
} T_BTIF_LE_SET_SUPPL_TEST_PARAMS_RSP;


typedef struct
{
    uint16_t cause;
    uint16_t sync_handle;
} T_BTIF_LE_SET_CONNLESS_IQ_SAMPLING_ENABLE_RSP;

typedef struct
{
    uint16_t    cause;
    bool        is_receiver;
} T_BTIF_LE_SET_CONN_SUPPL_PARAMS_RSP;

typedef struct
{
    uint16_t    cause;
} T_BTIF_LE_CONN_SUPPL_REQUEST_ENABLE_RSP;

typedef struct
{
    uint16_t    cause;
} T_BTIF_LE_CONN_SUPPL_RESPONSE_ENABLE_RSP;

typedef struct
{
    uint16_t cause;
    uint8_t supported_sampling_rates;
    uint8_t num_of_antennae;
    uint8_t max_suppl_length;
} T_BTIF_LE_READ_ANTENNA_INFO_RSP;



typedef struct
{
    uint16_t sync_handle;
    uint8_t  channel_index;
    uint8_t  suppl_type;
    uint8_t  slot_durations;
    uint8_t  packet_status;
    uint8_t  sample_count; //0x09 ?C 0x52
    uint8_t  iq_sample[82 + 82];
} T_BTIF_LE_CONNLESS_IQ_REPORT_INFO;

typedef struct
{
    uint16_t conn_handle;
    uint8_t  rx_phy;
    uint8_t  data_chan_index;
    uint8_t  rssi;
    uint8_t  suppl_type;
    uint8_t  slot_durations;
    uint8_t  packet_status;
    uint8_t  sample_count;//0x09 ?C 0x52
    uint8_t  iq_sample[82 + 82];
} T_BTIF_LE_CONN_IQ_REPORT_INFO;

typedef struct
{
    uint8_t  status;
    uint16_t conn_handle;

} T_BTIF_LE_SUPPL_REQUEST_FAILED_INFO;
#endif

typedef struct
{
    uint16_t    cause;
} T_BTIF_SW_RESET_RSP;

typedef union
{
    T_BTIF_REGISTER_RSP                 register_rsp;
    T_BTIF_RELEASE_RSP                  release_rsp;
    T_BTIF_ACT_INFO                     act_info;
    T_BTIF_DEV_CFG_SET_RSP              dev_cfg_rsp;
    T_BTIF_ACL_STATUS_INFO              acl_status_info;
    T_BTIF_ACL_PRIORITY_SET_RSP         acl_priority_set_rsp;
    T_BTIF_READ_RSSI_RSP                read_rssi_rsp;
    T_BTIF_VENDOR_CMD_RSP               vendor_cmd_rsp;
    T_BTIF_VENDOR_EVT_INFO              vendor_evt_info;
    T_BTIF_PAIRABLE_MODE_SET_RSP        pairable_mode_set_rsp;
    T_BTIF_USER_PASSKEY_REQ_IND         user_passkey_req_ind;
    T_BTIF_JUST_WORK_REQ_IND            just_work_req_ind;
    T_BTIF_USER_PASSKEY_REQ_REPLY_RSP   user_passkey_req_reply_rsp;
    T_BTIF_USER_PASSKEY_NOTIF_INFO      user_passkey_notif_info;
    T_BTIF_USER_PASSKEY_NOTIF_IND       user_passkey_notif_ind;
    T_BTIF_AUTHEN_RESULT_IND            authen_result_ind;
    T_BTIF_AUTHEN_KEY_REQ_IND           authen_key_req_ind;
    T_BTIF_USER_CFM_REQ_IND             user_cfm_req_ind;
    T_BTIF_KEYPRESS_NOTIF_RSP           keypress_notif_rsp;
    T_BTIF_KEYPRESS_NOTIF_INFO          keypress_notif_info;
    T_BTIF_HW_ERROR_INFO                hw_error_info;
    T_BTIF_SW_RESET_RSP                 sw_reset_rsp;

    T_BTIF_REMOTE_OOB_REQ_IND           remote_ood_req_ind;
    T_BTIF_GATT_SRV_REG_RSP             gatt_srv_reg_rsp;
    T_BTIF_GATT_ATTR_UPDATE_RSP         gatt_attr_update_rsp;
    T_BTIF_GATT_ATTR_UPDATE_STATUS_IND  gatt_attr_update_status_ind;
    T_BTIF_GATT_ATTR_READ_IND           gatt_attr_read_ind;
    T_BTIF_GATT_ATTR_WRITE_IND          gatt_attr_write_ind;
    T_BTIF_GATT_ATTR_EXEC_WRITE_IND     gatt_attr_exec_write_ind;
    T_BTIF_GATT_ATTR_CCCD_INFO          gatt_attr_cccd_info;
    T_BTIF_GATT_DISCOVERY_RSP           gatt_discovery_rsp;
    T_BTIF_GATT_DISCOVERY_IND           gatt_discovery_ind;
    T_BTIF_GATT_ATTR_READ_RSP           gatt_attr_read_rsp;
    T_BTIF_GATT_ATTR_READ_MULTI_RSP     gatt_attr_read_multi_rsp;
    T_BTIF_GATT_ATTR_WRITE_RSP          gatt_attr_write_rsp;
    T_BTIF_GATT_ATTR_PREP_WRITE_RSP     gatt_attr_prep_write_rsp;
    T_BTIF_GATT_ATTR_EXEC_WRITE_RSP     gatt_attr_exec_write_rsp;
    T_BTIF_GATT_ATTR_IND                gatt_attr_ind;
    T_BTIF_GATT_ATTR_NOTIF_INFO         gatt_attr_notif_info;
    T_BTIF_GATT_SECURITY_RSP            gatt_security_rsp;
    T_BTIF_GATT_SERVER_STORE_IND        gatt_server_store_ind;
    T_BTIF_GATT_MTU_SIZE_INFO           gatt_mtu_size_info;

    T_BTIF_LE_CONN_RSP                              le_conn_rsp;
    T_BTIF_LE_CONN_IND                              le_conn_ind;
    T_BTIF_LE_CONN_CMPL_INFO                        le_conn_cmpl_info;
#if F_BT_LE_READ_REMOTE_FEATS
    T_BTIF_LE_REMOTE_FEATS_INFO                     le_remote_feats_info;
#endif
    T_BTIF_LE_DISCONN_RSP                           le_disconn_rsp;
    T_BTIF_LE_DISCONN_IND                           le_disconn_ind;
    T_BTIF_LE_ADV_RSP                               le_adv_rsp;
    T_BTIF_LE_ADV_PARAM_SET_RSP                     le_adv_param_set_rsp;
    T_BTIF_LE_ADV_DATA_SET_RSP                      le_adv_data_set_rsp;
    T_BTIF_LE_SCAN_RSP                              le_scan_rsp;
    T_BTIF_LE_SCAN_PARAM_SET_RSP                    le_scan_param_set_rsp;
    T_BTIF_LE_SCAN_INFO                             le_scan_info;
    T_BTIF_LE_MODIFY_WHITE_LIST_RSP                 le_modify_white_list_rsp;
    T_BTIF_LE_CONN_UPDATE_RSP                       le_conn_update_rsp;
    T_BTIF_LE_CONN_UPDATE_IND                       le_conn_update_ind;
    T_BTIF_LE_CONN_PARAM_INFO                       le_conn_param_info;
#if F_BT_LE_4_2_SUPPORT
    T_BTIF_LE_CONN_PARAM_REQ_EVT_INFO               le_conn_param_req_evt_info;
    T_BTIF_LE_CONN_PARAM_REQ_REPLY_RSP              le_conn_param_req_reply_rsp;
    T_BTIF_LE_CONN_PARAM_REQ_NEG_REPLY_RSP          le_conn_param_req_neg_reply_rsp;
#endif
#if F_BT_LE_4_1_SUPPORT
    T_BTIF_LE_CREDIT_BASED_CONN_RSP                 le_credit_based_conn_rsp;
    T_BTIF_LE_CREDIT_BASED_CONN_IND                 le_credit_based_conn_ind;
    T_BTIF_LE_CREDIT_BASED_DISCONN_RSP              le_credit_based_disconn_rsp;
    T_BTIF_LE_CREDIT_BASED_DISCONN_IND              le_credit_based_disconn_ind;
    T_BTIF_LE_SEND_CREDIT_RSP                       le_send_credit_rsp;
    T_BTIF_LE_CREDIT_BASED_DATA_RSP                 le_credit_based_data_rsp;
    T_BTIF_LE_CREDIT_BASED_DATA_IND                 le_credit_based_data_ind;
    T_BTIF_LE_CREDIT_BASED_CONN_CMPL_INFO           le_credit_based_conn_cmpl_info;
    T_BTIF_LE_CREDIT_BASED_CONN_CREDIT_INFO         le_credit_based_conn_credit_info;
    T_BTIF_LE_CREDIT_BASED_SECURITY_REG_RSP         le_credit_based_security_reg_rsp;
    T_BTIF_LE_CREDIT_BASED_PSM_REG_RSP              le_credit_based_psm_reg_rsp;
#endif
    T_BTIF_LE_SET_CHANN_CLASSIF_RSP                 le_set_chann_classif_rsp;
    T_BTIF_LE_READ_CHANN_MAP_RSP                    le_read_chann_map_rsp;
    T_BTIF_LE_RECEIVER_TEST_RSP                     le_receiver_test_rsp;
    T_BTIF_LE_TRANSMITTER_TEST_RSP                  le_transmitter_test_rsp;
    T_BTIF_LE_TEST_END_RSP                          le_test_end_rsp;
    T_BTIF_LE_READ_ADV_TX_POWER_RSP                 le_read_adv_tx_power_rsp;
    T_BTIF_LE_SET_RAND_ADDR_RSP                     le_set_rand_addr_rsp;
#if F_BT_LE_4_2_SUPPORT
    T_BTIF_LE_READ_MAX_DATA_LEN_RSP                 le_read_max_data_len_rsp;
    T_BTIF_LE_READ_DEFAULT_DATA_LEN_RSP             le_read_default_data_len_rsp;
    T_BTIF_LE_WRITE_DEFAULT_DATA_LEN_RSP            le_write_default_data_len_rsp;
    T_BTIF_LE_SET_DATA_LEN_RSP                      le_set_data_len_rsp;
    T_BTIF_LE_DATA_LEN_CHANGE_INFO                  le_data_len_change_info;
#endif
#if F_BT_LE_PRIVACY_SUPPORT
    T_BTIF_LE_MODIFY_RESOLV_LIST_RSP                le_modify_resolv_list_rsp;
    T_BTIF_LE_READ_PEER_RESOLV_ADDR_RSP             le_read_peer_resolv_addr_rsp;
    T_BTIF_LE_READ_LOCAL_RESOLV_ADDR_RSP            le_read_local_resolv_addr_rsp;
    T_BTIF_LE_SET_RESOLUTION_ENABLE_RSP             le_set_resolution_enable_rsp;
    T_BTIF_LE_SET_RESOLV_PRIV_ADDR_TOUT_RSP         le_set_resolv_priv_addr_tout_rsp;
    T_BTIF_LE_CONFIG_LOCAL_IRK_RSP                  le_config_local_irk_rsp;
    T_BTIF_LE_SET_PRIVACY_MODE_RSP                  le_set_privacy_mode_rsp;
    T_BTIF_LE_DIRECT_ADV_INFO                       le_direct_adv_info;
#endif
    T_BTIF_LE_HIGH_DUTY_ADV_TIMEOUT_INFO            le_high_duty_adv_timeout_info;
#if F_BT_LE_5_0_SUPPORT
#if F_BT_LE_5_0_AE_ADV_SUPPORT
    T_BTIF_LE_SET_ADV_SET_RAND_ADDR_RSP             le_set_adv_set_rand_addr_rsp;
    T_BTIF_LE_EXT_ADV_PARAM_SET_RSP                 le_ext_adv_param_set_rsp;
    T_BTIF_LE_EXT_ADV_DATA_SET_RSP                  le_ext_adv_data_set_rsp;
    T_BTIF_LE_EXT_ADV_ENABLE_RSP                    le_ext_adv_enable_rsp;
#endif
#if F_BT_LE_5_0_AE_SCAN_SUPPORT
    T_BTIF_LE_EXT_SCAN_PARAM_SET_RSP                le_ext_scan_param_set_rsp;
    T_BTIF_LE_EXT_SCAN_RSP                          le_ext_scan_rsp;
#endif
    T_BTIF_LE_SET_DEFAULT_PHY_RSP                   le_set_default_phy_rsp;
    T_BTIF_LE_SET_PHY_RSP                           le_set_phy_rsp;
    T_BTIF_LE_ENHANCED_RECEIVER_TEST_RSP            le_enhanced_receiver_test_rsp;
    T_BTIF_LE_ENHANCED_TRANSMITTER_TEST_RSP         le_enhanced_transmitter_test_rsp;
    T_BTIF_LE_MODIFY_PERIODIC_ADV_LIST_RSP          le_modify_periodic_adv_list_rsp;
    T_BTIF_LE_READ_RF_PATH_COMPENSATION_RSP         le_read_rf_path_compensation_rsp;
    T_BTIF_LE_WRITE_RF_PATH_COMPENSATION_RSP        le_write_rf_path_compensation_rsp;
    T_BTIF_LE_MODIFY_ADV_SET_RSP                    le_modify_adv_set_rsp;
    T_BTIF_LE_SET_PERIODIC_ADV_PARAM_RSP            le_set_periodic_adv_param_rsp;
    T_BTIF_LE_SET_PERIODIC_ADV_DATA_RSP             le_set_periodic_adv_data_rsp;
    T_BTIF_LE_SET_PERIODIC_ADV_ENABLE_RSP           le_set_periodic_adv_enable_rsp;
    T_BTIF_LE_PERIODIC_ADV_CREATE_SYNC_RSP          le_periodic_adv_create_sync_rsp;
    T_BTIF_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL_RSP   le_periodic_adv_create_sync_cancel_rsp;
    T_BTIF_LE_PERIODIC_ADV_TERMINATE_SYNC_RSP       le_periodic_adv_terminate_sync_rsp;
    T_BTIF_LE_PHY_UPDATE_INFO                       le_phy_update_info;
    T_BTIF_LE_EXT_ADV_REPORT_INFO                   le_ext_adv_report_info;
    T_BTIF_LE_PERIODIC_ADV_SYNC_ESTABLISHED_INFO    le_periodic_adv_sync_established_info;
    T_BTIF_LE_PERIODIC_ADV_REPORT_INFO              le_periodic_adv_report_info;
    T_BTIF_LE_PERIODIC_ADV_SYNC_LOST_INFO           le_periodic_adv_sync_lost_info;
    T_BTIF_LE_ADV_SET_TERMINATED_INFO               le_adv_set_terminated_info;
    T_BTIF_LE_SCAN_REQ_RECEIVED_INFO                le_scan_req_received_info;
    T_BTIF_LE_ENABLE_EXT_ADV_MODE_RSP               le_enable_ext_adv_mode_rsp;
#endif
    T_BTIF_LE_LOCAL_DEV_INFO                        le_local_dev_info;
    T_BTIF_LE_SIGNED_STATUS_INFO                    le_signed_status_info;
#if F_BT_LE_6_0_AOA_AOD_SUPPORT
    T_BTIF_LE_SET_SUPPL_TEST_PARAMS_RSP             le_set_suppl_test_params_rsp;
    T_BTIF_LE_SET_CONNLESS_IQ_SAMPLING_ENABLE_RSP   le_set_connless_iq_sampling_enable_rsp;
    T_BTIF_LE_SET_CONN_SUPPL_PARAMS_RSP             le_set_conn_suppl_params_rsp;
    T_BTIF_LE_CONN_SUPPL_REQUEST_ENABLE_RSP         le_conn_suppl_request_enable_rsp;
    T_BTIF_LE_CONN_SUPPL_RESPONSE_ENABLE_RSP        le_conn_suppl_response_enable_rsp;
    T_BTIF_LE_READ_ANTENNA_INFO_RSP                 le_read_antenna_info_rsp;
    T_BTIF_LE_CONNLESS_IQ_REPORT_INFO               le_connless_iq_report_info;
    T_BTIF_LE_CONN_IQ_REPORT_INFO                   le_conn_iq_report_info;
    T_BTIF_LE_SUPPL_REQUEST_FAILED_INFO             le_suppl_request_failed_info;
#endif

    T_BTIF_RADIO_MODE_SET_RSP           radio_mode_set_rsp;
    T_BTIF_INQUIRY_RSP                  inquiry_rsp;
    T_BTIF_INQUIRY_RESULT_INFO          inquiry_result_info;
    T_BTIF_INQUIRY_CMPL_INFO            inquiry_cmpl_info;
    T_BTIF_INQUIRY_CANCEL_RSP           inquiry_cancel_rsp;
    T_BTIF_REMOTE_NAME_RSP              remote_name_rsp;
    T_BTIF_SDP_DISCOV_RSP               sdp_discov_rsp;
    T_BTIF_SDP_ATTR_IND                 sdp_attr_ind;
    T_BTIF_DID_ATTR_IND                 did_attr_ind;
    T_BTIF_SDP_DISCOV_CMPL_INFO         sdp_discov_cmpl_info;
    T_BTIF_STOP_SDP_DISCOV_RSP          stop_sdp_discov_rsp;
    T_BTIF_AUTHEN_RSP                   authen_rsp;
    T_BTIF_AUTHOR_REQ_IND               author_req_ind;
    T_BTIF_PIN_CODE_REQ_IND             pin_code_req_ind;
    T_BTIF_LEGACY_REMOTE_OOB_REQ_IND    legacy_remote_oob_req_ind;
    T_BTIF_LOCAL_OOB_RSP                local_oob_rsp;
    T_BTIF_ACL_CFG_RSP                  acl_cfg_rsp;
    T_BTIF_SDP_REG_RSP                  sdp_reg_rsp;
    T_BTIF_L2C_PROTO_REG_RSP            l2c_proto_reg_rsp;
    T_BTIF_L2C_CONN_RSP                 l2c_conn_rsp;
    T_BTIF_L2C_CONN_IND                 l2c_conn_ind;
    T_BTIF_L2C_CONN_CMPL_INFO           l2c_conn_cmpl_info;
    T_BTIF_L2C_DATA_IND                 l2c_data_ind;
    T_BTIF_L2C_DISCONN_RSP              l2c_disconn_rsp;
    T_BTIF_L2C_DISCONN_IND              l2c_disconn_ind;
    T_BTIF_L2C_SET_LOCAL_BUSY_RSP       l2c_set_local_busy_rsp;
    T_BTIF_L2C_SEC_REG_RSP              l2c_sec_reg_rsp;
    T_BTIF_RFC_AUTHEN_RSP               rfc_authen_rsp;
    T_BTIF_SCO_CONN_RSP                 sco_conn_rsp;
    T_BTIF_SCO_CONN_IND                 sco_conn_ind;
    T_BTIF_SCO_CONN_CMPL_INFO           sco_conn_cmpl_info;
    T_BTIF_SCO_DATA_IND                 sco_data_ind;
    T_BTIF_SCO_DISCONN_RSP              sco_disconn_rsp;
    T_BTIF_SCO_DISCONN_IND              sco_disconn_ind;
    T_BTIF_ACL_DISCONN_RSP              acl_disconn_rsp;
    T_BTIF_SETUP_QOS_RSP                setup_qos_rsp;

#if F_BT_CSB_SUPPORT
    T_BTIF_CSB_DATA_IND                 csb_data_ind;
#endif
} T_BTIF_UP_MSG_DATA;

typedef struct
{
    uint16_t            reserved;
    uint16_t            command;
    T_BTIF_UP_MSG_DATA  p;
} T_BTIF_UP_MSG;

typedef void (* P_BTIF_CALLBACK)(T_BTIF_UP_MSG *p_msg);


void *btif_buffer_get(uint16_t pool_id, uint16_t size, uint16_t offset);

bool btif_buffer_put(void *p_buf);

/**
 * btif.h
 *
 * \brief    Send a request to register a callback function to handle btif
 *           upsteam messages from Bluetooth stack to APP. If the request was
 *           successfully sent, a message whose type is \ref BTIF_MSG_REGISTER_RSP
 *           and data is \ref T_BTIF_REGISTER_RSP will be received in the callback
 *           function. If the function was successfully registered, another message
 *           whose type is \ref BTIF_MSG_ACT_INFO and data is \ref T_BTIF_ACT_INFO
 *           will also be received after that. <b>Sending messages to APP task
 *           in the callback function is highly recommended.</b> Btif message
 *           buffer should be released after the message is <b>truely</b> handled.
 *
 * \param[in]  p_callback     Callback function to handle btif message. The function
 *                            must has prototype as \ref P_BTIF_CALLBACK.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   #define BTIF_MSG_EVENT              0x01

   #define MAX_NUMBER_OF_RX_EVENT      0x10
   #define MAX_NUMBER_OF_MESSAGE       0x10

   void *evt_q_handle;
   void *msg_q_handle;

   // APP btif message callback function.
   void app_btif_msg_cb(T_BTIF_UP_MSG *p_msg)
   {
       unsigned char event = BTIF_MSG_EVENT;

       if (os_msg_send(msg_q_handle, &p_msg, 0) == false)
       {
           APP_PRINT_WARN0("app_btif_msg_cb: message queue is full");
           btif_buffer_put(p_msg);
       }
       else if (os_event_send(evt_q_handle, &event, 0) == false)
       {
           APP_PRINT_WARN0("app_btif_msg_cb: event queue is full");
       }
   }

   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_REGISTER_RSP:
           APP_PRINT_TRACE1("register response cause: 0x%04x", p_msg->p.register_rsp.cause);
           break;

       case BTIF_MSG_ACT_INFO:
           APP_PRINT_TRACE1("local address: %s", TRACE_BDADDR(p_msg->p.act_info.local_addr));
           break;

       default:
           break;
       }
   }

   void app_task(void *p_param)
   {
       char event;

       os_msg_queue_create(&evt_q_handle, MAX_NUMBER_OF_RX_EVENT, sizeof(unsigned char));
       os_msg_queue_create(&msg_q_handle, MAX_NUMBER_OF_MESSAGE, sizeof(T_BTIF_UP_MSG *));

       //Register callback function
       btif_register_req(app_btif_msg_cb);

       while (true)
       {
           if (os_msg_recv(evt_q_handle, &event, 0xFFFFFFFF) == true)
           {
               if (event == BTIF_MSG_EVENT)
               {
                   T_BTIF_UP_MSG *p_msg;

                   if (os_msg_recv(msg_q_handle, &p_msg, 0) == true)
                   {
                       app_handle_btif_msg(p_msg);
                       btif_buffer_put(p_msg);
                   }
               }
           }
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
void btif_param_reset(void);

bool btif_register_req(P_BTIF_CALLBACK p_callback);

bool btif_release_req(void);

bool btif_read_rssi_req(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type);

bool btif_vendor_cmd_req(uint16_t op, uint8_t paralen, uint8_t *para);

bool btif_sw_reset_req(uint8_t mode);

bool btif_pairable_mode_set_req(bool enable, uint16_t requirements,
                                T_BTIF_IOCAP io_caps, bool oob_present);

bool btif_user_passkey_req_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                               T_BTIF_CAUSE cause);

bool btif_user_passkey_req_reply_req(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                                     uint32_t passkey, T_BTIF_CAUSE cause);

bool btif_authen_result_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                            T_BTIF_KEY_TYPE key_type, T_BTIF_CAUSE cause);

bool btif_authen_key_req_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                             uint8_t key_len, uint8_t *p_key, T_BTIF_KEY_TYPE key_type,
                             T_BTIF_CAUSE cause);

bool btif_user_cfm_req_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                           T_BTIF_CAUSE cause);

bool btif_keypress_notif_req(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                             T_BTIF_KEYPRESS_NOTIF_TYPE event_type);

bool btif_just_work_req_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                            T_BTIF_CAUSE cause);

bool btif_user_passkey_notif_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                                 T_BTIF_CAUSE cause);

#if PATCH_CODE_EN
bool btif_user_defined(void *p_buf);
#endif

T_BTIF_IDENT_ADDR_TYPE btif_convert_addr_type(T_BTIF_REMOTE_ADDR_TYPE type);

bool btif_le_cfg_passkey_value(uint32_t value);

void btif_le_cfg_slave_init_mtu(bool enable);

void btif_le_cfg_local_key_distribute(uint8_t init_dist, uint8_t rsp_dist);

#if F_BT_LE_PRIVACY_SUPPORT
bool btif_le_cfg_local_identity_address(uint8_t *addr, T_BTIF_IDENT_ADDR_TYPE type);
#endif

bool btif_le_conn_req(bool use_extended_conn, uint8_t bd_addr[6],
                      T_BTIF_REMOTE_ADDR_TYPE remote_addr_type, T_BTIF_LOCAL_ADDR_TYPE local_addr_type,
                      uint8_t init_phys, uint16_t scan_timeout, T_BTIF_LE_CONN_REQ_PARAM *p_conn_req_param);

bool btif_le_conn_cfm(uint8_t *bd_addr, T_BTIF_CAUSE cause);

bool btif_le_disconn_req(uint16_t link_id, T_BTIF_CAUSE cause);

bool btif_le_disconn_cfm(uint16_t link_id);

bool btif_gatt_srv2_reg_req(uint16_t num_attr, void *p_srv, uint16_t start_handle);

bool btif_gatt_attr_update_req(void *p_buf, uint16_t link_id, void *p_srv_handle,
                               void *p_req_handle, uint16_t attr_index, uint16_t attr_len,
                               uint16_t offset, T_BTIF_GATT_PDU_TYPE type);

bool btif_gatt_attr_update_status_cfm(uint16_t link_id, void *p_srv_handle,
                                      void *p_req_handle, uint16_t attr_index);

bool btif_gatt_attr_read_cfm(void *p_buf, uint16_t link_id, void *p_srv_handle, uint16_t cause,
                             uint16_t attr_index, uint16_t attr_len, uint16_t offset);

bool btif_gatt_attr_write_req_cfm(uint16_t link_id, void *p_srv_handle,
                                  uint16_t cause, uint16_t attr_index);

bool btif_gatt_attr_prep_write_cfm(void *p_buf, uint16_t link_id, void *p_srv_handle,
                                   uint16_t cause, uint16_t attr_index, uint16_t attr_len, uint16_t offset);

bool btif_gatt_attr_exec_write_cfm(uint16_t link_id, uint16_t cause, uint16_t handle);

bool btif_gatt_attr_prep_write_req(void *p_buf, uint16_t link_id, uint16_t attr_handle,
                                   uint16_t attr_len, uint16_t write_offset, uint16_t offset);

bool btif_gatt_attr_exec_write_req(uint16_t link_id, uint8_t flags);

bool btif_gatt_discovery_req(uint16_t link_id, T_BTIF_GATT_DISCOVERY_TYPE type,
                             uint16_t start_handle, uint16_t end_handle, uint16_t uuid16, uint8_t *p_uuid128);

bool btif_gatt_discovery_cfm(uint16_t link_id, T_BTIF_GATT_DISCOVERY_TYPE type,
                             uint16_t start_handle, uint16_t end_handle);

bool btif_gatt_attr_read_req(uint16_t link_id, T_BTIF_GATT_READ_TYPE read_type,
                             uint16_t read_offset, uint16_t start_handle, uint16_t end_handle,
                             uint16_t uuid16, uint8_t *p_uuid128);

bool btif_gatt_attr_read_multi_req(uint16_t link_id, uint16_t num_handle, uint16_t *p_handle);

bool btif_gatt_attr_write_req(void *p_buf, uint16_t link_id, T_BTIF_GATT_WRITE_TYPE write_type,
                              uint16_t attr_handle, uint16_t attr_len, uint16_t offset);

bool btif_gatt_attr_cfm(uint16_t link_id);

bool btif_gatt_security_req(uint16_t link_id, uint16_t requirements, uint8_t min_key_size);

bool btif_gatt_server_store_cfm(T_BTIF_GATT_STORE_OPCODE op, uint8_t *bd_addr,
                                T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                                uint8_t data_len, uint8_t *data, T_BTIF_CAUSE cause);

#if F_BT_LE_SMP_OOB_SUPPORT
bool btif_remote_oob_req_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE addr_type,
                             uint8_t *p_data_c, T_BTIF_CAUSE cause);
#endif

bool btif_le_adv_req(T_BTIF_LE_ADV_MODE adv_mode);

bool btif_le_adv_param_set_req(T_BTIF_LE_ADV_TYPE adv_type,
                               T_BTIF_LE_ADV_FILTER_POLICY filter_policy,
                               uint16_t min_interval, uint16_t max_interval,
                               T_BTIF_LOCAL_ADDR_TYPE local_addr_type, uint8_t *bd_addr,
                               T_BTIF_REMOTE_ADDR_TYPE remote_addr_type, uint8_t chann_map);

bool btif_le_adv_data_set_req(T_BTIF_LE_ADV_DATA_TYPE data_type, uint8_t data_len, uint8_t *p_data);

bool btif_le_scan_req(T_BTIF_LE_SCAN_MODE mode, T_BTIF_LE_SCAN_FILTER_DUPLICATES filter_duplicates);

bool btif_le_scan_param_set_req(T_BTIF_LE_SCAN_TYPE type, uint16_t interval, uint16_t window,
                                T_BTIF_LE_SCAN_FILTER_POLICY filter_policy, T_BTIF_LOCAL_ADDR_TYPE local_addr_type);

bool btif_le_ext_scan_req(T_BTIF_LE_SCAN_MODE mode,
                          T_BTIF_LE_SCAN_FILTER_DUPLICATES filter_duplicates,
                          uint16_t duration, uint16_t period);

bool btif_le_ext_scan_param_set_req(T_BTIF_LOCAL_ADDR_TYPE local_addr_type,
                                    T_BTIF_LE_SCAN_FILTER_POLICY filter_policy,
                                    uint8_t scan_phys,
                                    T_BTIF_LE_EXT_SCAN_PARAM *p_extended_scan_param);

bool btif_le_modify_adv_set_req(T_BTIF_LE_ADV_SET_OP op, uint8_t adv_handle);

bool btif_le_modify_white_list_req(T_BTIF_LE_WHITE_LIST_OP op, uint8_t *bd_addr,
                                   T_BTIF_REMOTE_ADDR_TYPE remote_addr_type);

bool btif_le_conn_update_req(uint16_t link_id, uint16_t min_interval, uint16_t max_interval,
                             uint16_t conn_latency, uint16_t supv_tout, uint16_t min_ce_len,
                             uint16_t max_ce_len);

bool btif_le_conn_update_cfm(uint16_t link_id, T_BTIF_CAUSE cause);

bool btif_le_set_rand_addr_req(uint8_t *random_addr);

bool btif_le_enable_ext_adv_mode_req(void);

bool btif_le_set_adv_set_rand_addr_req(uint8_t *random_addr, uint8_t adv_handle);

bool btif_le_credit_based_conn_req(uint16_t link_id, uint16_t le_psm, uint16_t mtu,
                                   uint16_t initial_credits, uint16_t credits_threshold);

bool btif_le_credit_based_conn_cfm(uint16_t link_id, uint16_t channel, uint16_t mtu,
                                   uint16_t initial_credits, uint16_t credits_threshold,
                                   T_BTIF_L2C_LE_CONN_STATUS cause);

bool btif_le_credit_based_disconn_req(uint16_t link_id, uint16_t channel);

bool btif_le_credit_based_disconn_cfm(uint16_t link_id, uint16_t channel);

bool btif_le_send_credit_req(uint16_t link_id, uint16_t channel, uint16_t credits);

bool btif_le_credit_based_data_req(void *p_buf, uint16_t link_id, uint16_t channel,
                                   uint16_t length, uint16_t offset);

bool btif_le_credit_based_data_cfm(uint16_t link_id, uint16_t channel, T_BTIF_CAUSE cause);

bool btif_le_credit_based_security_reg_req(uint16_t le_psm, bool active,
                                           T_BTIF_LE_SECURITY_MODE mode, uint8_t key_size);

bool btif_le_credit_based_psm_reg_req(uint16_t le_psm, uint8_t action);

bool btif_le_set_data_len_req(uint16_t link_id, uint16_t tx_oct, uint16_t tx_time);

bool btif_le_conn_param_req_reply_req(uint16_t link_id, uint16_t min_interval,
                                      uint16_t max_interval, uint16_t latency, uint16_t supv_tout,
                                      uint16_t min_ce_len, uint16_t max_ce_len);

bool btif_le_conn_param_req_neg_reply_req(uint16_t link_id, uint8_t reason);

bool btif_le_config_local_irk_req(uint8_t *p_local_irk);

bool btif_le_gen_rand_addr(T_BTIF_ADDR_RAND rand_addr_type, uint8_t *p_addr);

bool btif_le_modify_resolv_list_req(T_BTIF_LE_RESOLVING_LIST_OP operation,
                                    T_BTIF_IDENT_ADDR_TYPE peer_ident_addr_type, uint8_t *peer_ident_addr,
                                    uint8_t *peer_irk, uint8_t *local_irk);

bool btif_le_read_peer_resolv_addr_req(T_BTIF_IDENT_ADDR_TYPE peer_ident_addr_type,
                                       uint8_t *peer_ident_addr);

bool btif_le_read_local_resolv_addr_req(T_BTIF_IDENT_ADDR_TYPE peer_ident_addr_type,
                                        uint8_t *peer_ident_addr);

bool btif_le_set_resolution_enable_req(uint8_t enable);

bool btif_le_set_resolv_priv_addr_tout_req(uint16_t timeout);

bool btif_le_set_privacy_mode_req(T_BTIF_IDENT_ADDR_TYPE peer_identity_address_type,
                                  uint8_t *peer_identity_address, T_BTIF_LE_PRIVACY_MODE privacy_mode);

bool btif_le_set_chann_classif_req(uint8_t *p_chann_map);

bool btif_le_read_chann_map_req(uint16_t link_id);

bool btif_le_receiver_test_req(uint8_t rx_chann);

bool btif_le_transmitter_test_req(uint8_t tx_chann, uint8_t data_len, uint8_t pkt_pl);

bool btif_le_test_end_req(void);

bool btif_le_read_adv_tx_power_req(void);

bool btif_le_set_default_phy_req(uint8_t all_phys, uint8_t tx_phys, uint8_t rx_phys);

bool btif_le_set_phy_req(uint16_t link_id, uint8_t all_phys, uint8_t tx_phys,
                         uint8_t rx_phys, T_BTIF_LE_PHY_OPTIONS phy_options);

bool btif_le_ext_adv_enable_req(T_BTIF_LE_EXT_ADV_MODE adv_mode, uint8_t num_of_sets,
                                T_BTIF_LE_EXT_ADV_SET_PARAM *p_adv_set_param);

bool btif_le_ext_adv_param_set_req(uint8_t adv_handle, uint16_t adv_event_prop,
                                   uint32_t primary_adv_interval_min, uint32_t primary_adv_interval_max,
                                   uint8_t primary_adv_channel_map, T_BTIF_LOCAL_ADDR_TYPE own_address_type,
                                   T_BTIF_REMOTE_ADDR_TYPE peer_address_type, uint8_t *p_peer_address,
                                   T_BTIF_LE_ADV_FILTER_POLICY filter_policy, uint8_t tx_power,
                                   T_BTIF_LE_PRIM_ADV_PHY_TYPE primary_adv_phy, uint8_t secondary_adv_max_skip,
                                   T_BTIF_LE_PHY_TYPE secondary_adv_phy, uint8_t adv_sid,
                                   T_BTIF_LE_SCAN_REQ_NOTIFY_TYPE scan_req_notification_enable);

bool btif_le_ext_adv_data_set_req(T_BTIF_LE_EXT_ADV_DATA_TYPE data_type,
                                  uint8_t adv_handle, T_BTIF_LE_ADV_FRAG_OP_TYPE op,
                                  T_BTIF_LE_ADV_FRAG_PREFERENCE_TYPE frag_preference,
                                  uint8_t data_len, uint8_t *p_data);

bool btif_le_enhanced_receiver_test_req(uint8_t rx_chann, uint8_t phy, uint8_t modulation_index);

bool btif_le_enhanced_transmitter_test_req(uint8_t tx_chann, uint8_t data_len,
                                           uint8_t pkt_pl, uint8_t phy);

bool btif_le_modify_periodic_adv_list_req(T_BTIF_LE_PERIODIC_ADV_LIST_OP op, uint8_t adv_addr_type,
                                          uint8_t *adv_addr, uint8_t adv_sid);

bool btif_le_read_rf_path_compensation_req(void);

bool btif_le_write_rf_path_compensation_req(int8_t rf_tx_path_compensation,
                                            int8_t rf_rx_path_compensation);

bool btif_le_set_periodic_adv_param_req(uint8_t adv_handle, uint16_t adv_interval_min,
                                        uint16_t adv_interval_max, uint16_t adv_prop);

bool btif_le_set_periodic_adv_data_req(uint8_t adv_handle, T_BTIF_LE_ADV_FRAG_OP_TYPE op,
                                       uint8_t data_len, uint8_t *p_data);

bool btif_le_set_periodic_adv_enable_req(T_BTIF_LE_PERIODIC_ADV_MODE mode, uint8_t adv_handle);

bool btif_le_periodic_adv_create_sync_req(uint8_t filter_policy, uint8_t adv_sid,
                                          uint8_t adv_addr_type, uint8_t adv_add[6], uint16_t skip,
                                          uint16_t sync_timeout, uint16_t unused);

bool btif_le_periodic_adv_create_sync_cancel_req(void);

bool btif_le_periodic_adv_terminate_sync_req(uint16_t sync_handle);

bool btif_remote_oob_req_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE addr_type,
                             uint8_t *p_data_c, T_BTIF_CAUSE cause);

#if F_BT_LE_6_0_AOA_AOD_SUPPORT
bool btif_le_set_suppl_test_params_req(bool is_receiver, uint8_t suppl_len, uint8_t suppl_slot_type,
                                       uint8_t num_antena_ids, uint8_t antenna_switching_pattern);
#if F_BT_LE_6_0_AOX_CONNLESS_SUPPORT
bool btif_le_set_connless_suppl_transmit_params_req(uint8_t adv_handle, uint8_t suppl_type,
                                                    uint8_t suppl_len,
                                                    uint8_t num_antena_ids, uint8_t *p_antenna_id_list);
bool btif_le_set_connless_suppl_transmit_enable_req(uint8_t adv_handle, uint8_t enable);
bool btif_le_set_connless_iq_sample_enable_req(uint16_t sync_handle, uint8_t enable,
                                               uint8_t slot_duration,
                                               uint8_t num_antena_ids, uint8_t *p_antenna_id_list);
#endif
bool btif_le_set_conn_suppl_params_req(bool is_receiver, uint16_t link_id, uint8_t slot_duration,
                                       uint8_t num_antena_ids, uint8_t *p_antenna_id_list);

bool btif_le_conn_suppl_request_enable_req(uint16_t link_id, uint8_t enable,
                                           uint16_t suppl_interval,
                                           uint8_t suppl_len, uint8_t suppl_type);
bool btif_le_conn_suppl_response_enable_req(uint16_t link_id, uint8_t enable);
bool btif_le_read_antenna_info_req(void);
#endif


bool btif_dev_cfg_name_req(uint32_t cod, uint8_t *p_name);

bool btif_dev_did_eir_cfg_req(uint16_t vendor_id, uint16_t vendor_id_src,
                              uint16_t product_id, uint16_t product_version);


bool btif_dev_page_scan_cfg_req(T_BTIF_BR_PAGE_SCAN_TYPE scan_type,
                                uint16_t interval, uint16_t window, uint16_t page_tout);

bool btif_dev_bt_mode_cfg_req(T_BTIF_BT_MODE mode);

bool btif_dev_inquiry_scan_cfg_req(T_BTIF_BR_INQUIRY_SCAN_TYPE scan_type,
                                   uint16_t interval, uint16_t window);

bool btif_dev_inquiry_mode_cfg_req(T_BTIF_BR_INQUIRY_MODE mode);

bool btif_dev_link_policy_cfg_req(T_BTIF_BR_LINK_POLICY policy, uint16_t supv_tout);


bool btif_radio_mode_set_req(T_BTIF_RADIO_MODE mode, bool limited, uint8_t limited_time);

bool btif_inquiry_req(bool limited_inquiry, uint8_t timeout);

bool btif_inquiry_cancel_req(void);

bool btif_remote_name_req(uint8_t *bd_addr);

bool btif_sdp_discov_req(uint8_t *bd_addr, T_BTIF_SDP_DISCOV_TYPE type,
                         T_BTIF_UUID_TYPE uuid_type, T_BTIF_UUID_DATA uuid_data);

bool btif_sdp_attr_cfm(T_BTIF_CAUSE cause);

bool btif_did_attr_cfm(T_BTIF_CAUSE cause);

bool btif_stop_sdp_discov_req(uint8_t *bd_addr);

bool btif_authen_req(uint8_t *bd_addr);

bool btif_author_req_cfm(uint8_t *bd_addr, T_BTIF_CAUSE cause);

bool btif_pin_code_req_cfm(uint8_t *bd_addr, uint8_t pin_code_len,
                           uint8_t *pin_code, T_BTIF_CAUSE cause);



bool btif_acl_link_policy_cfg_req(uint8_t *bd_addr, T_BTIF_BR_LINK_POLICY policy);

bool btif_acl_link_supv_tout_cfg_req(uint8_t *bd_addr, uint16_t supv_tout);

bool btif_acl_link_role_cfg_req(uint8_t *bd_addr, T_BTIF_BR_LINK_ROLE role);

bool btif_acl_pkt_type_cfg_req(uint8_t *bd_addr, uint16_t pkt_type);

bool btif_acl_sniff_mode_enter_req(uint8_t *bd_addr, uint16_t min_interval, uint16_t max_interval,
                                   uint16_t sniff_attempt, uint16_t sniff_tout, uint16_t max_latency,
                                   uint16_t min_remote_tout, uint16_t min_local_tout);

bool btif_acl_sniff_mode_exit_req(uint8_t *bd_addr);

bool btif_sdp_reg_req(void *p_buf);

bool btif_l2c_proto_reg_req(uint16_t psm, uint16_t proto_id, uint8_t action);

bool btif_l2c_conn_req(uint16_t psm, uint16_t uuid, uint8_t *bd_addr, uint16_t proto_id,
                       uint16_t mtu_size, uint8_t mode_bits, uint16_t flush_tout);

bool btif_l2c_conn_cfm(T_BTIF_L2C_CONN_CAUSE cause, uint16_t cid, uint16_t mtu_size,
                       uint8_t mode_bits, uint16_t flush_tout);

bool btif_l2c_data_req(void *p_buf, uint16_t offset, uint16_t cid,
                       uint16_t length, bool auto_flush);

bool btif_l2c_disconn_req(uint16_t cid);

bool btif_l2c_disconn_cfm(uint16_t cid);

bool btif_l2c_sec_reg_req(uint8_t active, uint16_t psm, uint16_t server_chann,
                          uint16_t uuid, uint8_t requirement);

bool btif_rfc_authen_req(uint8_t *bd_addr, uint16_t channel, uint16_t dlci,
                         uint16_t uuid, uint8_t outgoing);

bool btif_sco_conn_req(uint8_t *bd_addr, uint32_t tx_bandwidth, uint32_t rx_bandwidth,
                       uint16_t max_latency, uint16_t voice_setting, uint8_t  retrans_effort, uint16_t packet_type);

bool btif_sco_conn_cfm(uint8_t *bd_addr, uint32_t tx_bandwidth, uint32_t rx_bandwidth,
                       uint16_t max_latency, uint16_t voice_setting, uint8_t retrans_effort,
                       uint16_t packet_type, T_BTIF_CAUSE cause);

void *btif_sco_buffer_get(uint8_t len);

bool btif_sco_data_req(uint16_t handle, void *p_buf, uint8_t len);

bool btif_sco_data_cfm(uint8_t *p_buf);

bool btif_sco_disconn_req(uint16_t handle);

bool btif_sco_disconn_cfm(uint16_t handle);

bool btif_acl_disconn_req(uint8_t *bd_addr);


bool btif_set_l2c_local_busy_req(uint16_t cid, bool busy);

bool btif_set_l2c_rej_mode(uint8_t mode);

bool btif_set_l2c_fcs(uint8_t flag);

void *btif_get_l2c_buf(uint16_t size, uint16_t offset, bool ack);

uint8_t btif_get_l2c_data_quota(void);

#ifdef __cplusplus
}
#endif

#endif  /* _BTIF_H_ */
