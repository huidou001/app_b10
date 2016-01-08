#ifndef __DATA_TYPE_H__
#define __DATA_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <math.h>

#define TRUE		1
#define FALSE	0

#define FS_CREATE_ALWAYS        0x00020000L
#define FS_READ_ONLY            0x00000100L
#define FS_READ_WRITE           0x00000000L

typedef unsigned char           kal_uint8;
typedef signed char             kal_int8;
typedef char                    	kal_char;
typedef unsigned short          kal_wchar;

typedef unsigned short int      kal_uint16;
typedef signed short int        kal_int16;

typedef unsigned int            kal_uint32;
typedef signed int              kal_int32;

typedef unsigned long long	 kal_uint64;
typedef signed long long	 kal_int64;

typedef char				S8;
typedef char 			   *PS8;
typedef unsigned char	   U8;
typedef unsigned char    u8;
typedef unsigned char	   *PU8;

typedef unsigned short	   pBOOL;

typedef short int		   S16;
typedef short int		   *PS16;
typedef unsigned short int  U16;
typedef unsigned short int  u16;
typedef unsigned short int  *PU16;

typedef int				   S32;
typedef int				   *PS32;
typedef unsigned int 	   U32;
typedef unsigned int       u32;
typedef unsigned int 	   *PU32;

typedef float			   float32;

typedef unsigned char  BOOL;
typedef unsigned char bool;
typedef char CHAR;
typedef int FS_HANDLE;
typedef unsigned int   UINT;

typedef void (*FuncPtr) (void);
typedef void* (*jt_get_sym_entry_t)(const char* sym);

#define IN
#define OUT

extern jt_get_sym_entry_t jt_get_sym_entry;

//定义系统定时器ID号
enum
{
     GIS_TIMER_ID_0=207,
     GIS_TIMER_ID_1,
     GIS_TIMER_ID_2,
     GIS_TIMER_ID_3,
     GIS_TIMER_ID_4,
     GIS_TIMER_ID_5,
     GIS_TIMER_ID_6,
     GIS_TIMER_ID_7,
     GIS_TIMER_ID_8,
     GIS_TIMER_ID_9,
     GIS_TIMER_ID_10,
     GIS_TIMER_ID_11,
     GIS_TIMER_ID_12,
     GIS_TIMER_ID_13,
     GIS_TIMER_ID_14,
     GIS_TIMER_ID_15,
     GIS_TIMER_ID_16,
     GIS_TIMER_ID_17,
     GIS_TIMER_ID_18,
     GIS_TIMER_ID_19
};

enum
{
	AVL_LSTOR_FOR,
	AVL_LSTOR_THI,
	AVL_LSTOR_SEC,
	AVL_LSTOR_FIR
};

enum
{
	AVL_WSTOR_SEC,
	AVL_WSTOR_FIR
};

//union data  type long
typedef union
{
	u32 	l;
	u8	b[4];
}Avl_ULong;

//union data  type Word
typedef union
{
	u16  w;
	u8	b[2];
}Avl_UWord;




#ifdef __cplusplus
}
#endif

  typedef enum
    {
        /* System record, keep the system version */
        NVRAM_EF_SYS_LID,
        /* Branch record, keep the branch version */
        NVRAM_EF_BRANCH_VERNO_LID,
        /* Flavor record, keep the flavor version */
        NVRAM_EF_FLAVOR_VERNO_LID,
        /* CustPack record, keep the custpack version */
        NVRAM_EF_CUSTPACK_VERNO_LID,
        /* SecuPack record, keep the secupack version */
        NVRAM_EF_SECUPACK_VERNO_LID,
        /* Security setting in NVRAM */
        NVRAM_EF_NVRAM_CONFIG_LID,

        NVRAM_EF_START,
        NVRAM_EF_SYS_EXCEPTION_LID = NVRAM_EF_START,
        NVRAM_EF_SYS_STATISTICS_LID,
//stupid-prevant
        NVRAM_EF_IMPT_COUNTER_LID,       //9
        NVRAM_EF_CAT_TIMESTAMP_LID,


    /********************************************
     *
     *  Calibration Data
     *
     **********************************************/

        NVRAM_EF_L1_START = 15,
        NVRAM_EF_L1_AGCPATHLOSS_LID = NVRAM_EF_L1_START,
        NVRAM_EF_L1_RAMPTABLE_GSM850_LID,
        NVRAM_EF_L1_RAMPTABLE_GSM900_LID,
        NVRAM_EF_L1_RAMPTABLE_DCS1800_LID,
        NVRAM_EF_L1_RAMPTABLE_PCS1900_LID,
        NVRAM_EF_L1_EPSK_START,
        NVRAM_EF_L1_EPSK_RAMPTABLE_GSM850_LID = NVRAM_EF_L1_EPSK_START,
        NVRAM_EF_L1_EPSK_RAMPTABLE_GSM900_LID,
        NVRAM_EF_L1_EPSK_RAMPTABLE_DCS1800_LID,
        NVRAM_EF_L1_EPSK_RAMPTABLE_PCS1900_LID,
        NVRAM_EF_L1_EPSK_INTERSLOT_RAMP_GSM850_LID,
        NVRAM_EF_L1_EPSK_INTERSLOT_RAMP_GSM900_LID,
        NVRAM_EF_L1_EPSK_INTERSLOT_RAMP_DCS1800_LID,
        NVRAM_EF_L1_EPSK_INTERSLOT_RAMP_PCS1900_LID,
        NVRAM_EF_L1_EPSK_END = NVRAM_EF_L1_EPSK_INTERSLOT_RAMP_PCS1900_LID,
        NVRAM_EF_L1_AFCDATA_LID,
        NVRAM_EF_L1_TXIQ_LID,
        NVRAM_EF_L1_RFSPECIALCOEF_LID,
        NVRAM_EF_L1_INTERSLOT_RAMP_GSM850_LID,
        NVRAM_EF_L1_INTERSLOT_RAMP_GSM900_LID,
        NVRAM_EF_L1_INTERSLOT_RAMP_DCS1800_LID,
        NVRAM_EF_L1_INTERSLOT_RAMP_PCS1900_LID,
        NVRAM_EF_L1_CRYSTAL_AFCDATA_LID,
        NVRAM_EF_L1_CRYSTAL_CAPDATA_LID,
        /*Chuwei: for TX power rollback*/
        NVRAM_EF_L1_GMSK_TX_POWER_ROLLBACK_TABLE_LID,
        NVRAM_EF_L1_EPSK_TX_POWER_ROLLBACK_TABLE_LID,
        /*for TX power control*/
        NVRAM_EF_L1_GMSK_TXPC_LID,
        NVRAM_EF_L1_EPSK_TXPC_LID,
        /*for LNA Middle/Low mode*/
        NVRAM_EF_L1_LNAPATHLOSS_LID,
        /*for Temperature ADC*/
        NVRAM_EF_L1_TEMPERATURE_ADC_LID,
        /*for DCXO LPM Cload freq. offset*/
        NVRAM_EF_L1_CLOAD_FREQ_OFFSET_LID,
        NVRAM_EF_L1_END =  NVRAM_EF_L1_CLOAD_FREQ_OFFSET_LID,
        NVRAM_EF_UL1_START = 45,
        NVRAM_EF_UL1_TEMP_DAC_LID = NVRAM_EF_UL1_START,
        NVRAM_EF_UL1_PATHLOSS_BAND1_LID,
        NVRAM_EF_UL1_PATHLOSS_BAND2_LID,
        NVRAM_EF_UL1_PATHLOSS_BAND3_LID,
        NVRAM_EF_UL1_PATHLOSS_BAND4_LID,
        NVRAM_EF_UL1_PATHLOSS_BAND5_LID,
        NVRAM_EF_UL1_PATHLOSS_BAND6_LID,
        NVRAM_EF_UL1_PATHLOSS_BAND7_LID,
        NVRAM_EF_UL1_PATHLOSS_BAND8_LID,
        NVRAM_EF_UL1_PATHLOSS_BAND9_LID,
        NVRAM_EF_UL1_PATHLOSS_BAND10_LID,
        NVRAM_EF_UL1_TXDAC_BAND1_LID,
        NVRAM_EF_UL1_TXDAC_BAND2_LID,
        NVRAM_EF_UL1_TXDAC_BAND3_LID,
        NVRAM_EF_UL1_TXDAC_BAND4_LID,
        NVRAM_EF_UL1_TXDAC_BAND5_LID,
        NVRAM_EF_UL1_TXDAC_BAND6_LID,
        NVRAM_EF_UL1_TXDAC_BAND7_LID,
        NVRAM_EF_UL1_TXDAC_BAND8_LID,
        NVRAM_EF_UL1_TXDAC_BAND9_LID,
        NVRAM_EF_UL1_TXDAC_BAND10_LID,
        NVRAM_EF_UL1_END = NVRAM_EF_UL1_TXDAC_BAND10_LID,
        NVRAM_EF_WNDRV_START = 67,
        NVRAM_EF_WNDRV_MAC_ADDRESS_LID = NVRAM_EF_WNDRV_START,
        NVRAM_EF_WNDRV_TX_POWER_2400M_LID,
        NVRAM_EF_WNDRV_TX_POWER_5000M_LID,
        NVRAM_EF_WNDRV_DAC_DC_OFFSET_LID,
        NVRAM_EF_WNDRV_TX_ALC_POWER_LID,
        NVRAM_EF_WNDRV_EXT_SETTING_TRIMVAL_THERMOVAL_LID,
        NVRAM_EF_WNDRV_ALC_SLOPE_LID,
        NVRAM_EF_WNDRV_END = NVRAM_EF_WNDRV_ALC_SLOPE_LID,

        // BT
        NVRAM_EF_BTRADIO_RFMD3500_LID = 74,   /* __BT_SUPPORT__,BTMTK */
        NVRAM_EF_BTRADIO_MT6601_LID,     /* __BT_SUPPORT__,BTMTK_MT6601 */
        NVRAM_EF_BTRADIO_MT6611_LID,     /* __BT_SUPPORT__,BTMTK_MT6611 */
        //NVRAM_EF_BTRADIO_MT6612_LID,     /* __BT_SUPPORT__,BTMTK_MT6612 */
        //NVRAM_EF_BTRADIO_MT6616_LID,     /* __BT_SUPPORT__,BTMTK_MT6616 */
        //NVRAM_EF_BTRADIO_MT6236_LID,     /* __BT_SUPPORT__,BTMTK_MT6236 */
        //NVRAM_EF_BTRADIO_MT6256_LID,     /* __BT_SUPPORT__,BTMTK_MT6256 */
        //NVRAM_EF_BTRADIO_MT6276_LID,     /* __BT_SUPPORT__,BTMTK_MT6276 */
        NVRAM_EF_BTRADIO_MTK_BT_CHIP_LID = 79,     /* __BT_SUPPORT__,BTMTK_MT6622,BTMTK_MT6626 */

        // TD :wrap with compile option: __AST_TL1_TDD__
        NVRAM_EF_AST_TL1_START = 85,
        NVRAM_EF_AST_TL1_TEMP_DAC_LID = NVRAM_EF_AST_TL1_START,
        NVRAM_EF_AST_TL1_AFC_DATA_LID,
        NVRAM_EF_AST_TL1_PATHLOSS_BAND33_35_37_39_LID,
        NVRAM_EF_AST_TL1_PATHLOSS_BAND34_LID,
        NVRAM_EF_AST_TL1_PATHLOSS_BAND36_LID,
        NVRAM_EF_AST_TL1_PATHLOSS_BAND38_LID,
        NVRAM_EF_AST_TL1_PATHLOSS_BAND40_LID,
        NVRAM_EF_AST_TL1_TXDAC_BAND33_35_37_39_LID,
        NVRAM_EF_AST_TL1_TXDAC_BAND34_LID,
        NVRAM_EF_AST_TL1_TXDAC_BAND36_LID,
        NVRAM_EF_AST_TL1_TXDAC_BAND38_LID,
        NVRAM_EF_AST_TL1_TXDAC_BAND40_LID,
        NVRAM_EF_AST_TL1_ABB_CAL_LID,
        NVRAM_EF_AST_TL1_END = NVRAM_EF_AST_TL1_ABB_CAL_LID,

        NVRAM_EF_BARCODE_NUM_LID,
        NVRAM_EF_CAL_FLAG_LID,
        NVRAM_EF_CAL_DATA_CHECK_LID,
        NVRAM_EF_RF_CAL_ENV_LID,
        NVRAM_EF_RF_CAL_LOSS_SETTING_LID,
        NVRAM_EF_RF_TEST_POWER_RESULT_LID,

        /* PA 8-level control (for MT6276, MT6573) */
        NVRAM_EF_UL1_TXPAOCTLEV_START,
        NVRAM_EF_UL1_TXPAOCTLEV_BAND1_LID = NVRAM_EF_UL1_TXPAOCTLEV_START,
        NVRAM_EF_UL1_TXPAOCTLEV_BAND2_LID,
        NVRAM_EF_UL1_TXPAOCTLEV_BAND3_LID,
        NVRAM_EF_UL1_TXPAOCTLEV_BAND4_LID,
        NVRAM_EF_UL1_TXPAOCTLEV_BAND5_LID,
        NVRAM_EF_UL1_TXPAOCTLEV_BAND6_LID,
        NVRAM_EF_UL1_TXPAOCTLEV_BAND7_LID,
        NVRAM_EF_UL1_TXPAOCTLEV_BAND8_LID,
        NVRAM_EF_UL1_TXPAOCTLEV_BAND9_LID,
        NVRAM_EF_UL1_TXPAOCTLEV_BAND10_LID,
        NVRAM_EF_UL1_TXPAOCTLEV_END = NVRAM_EF_UL1_TXPAOCTLEV_BAND10_LID,

        /********************************************************/
        /* MT6573 3G RF Customization for Modem Bin Update Tool */
        /********************************************************/
        /*for modem bin update tool*/
        NVRAM_EF_L1_2G_RF_PARAMETER_LID,
        /* For WNDRV Tx Power Calibration Free Flow */
        NVRAM_EF_WNDRV_TPCFF_LID,

        /* AST 3001 DCXO support */
        NVRAM_EF_AST_TL1_CAP_DATA_LID,

     	NVRAM_EF_CALIBRATION_END = 120,

    /********************************************
     *
     *  Device Setting
     *
     **********************************************/
        NVRAM_EF_BAND_INFO_LID,
        NVRAM_EF_TST_FILTER_LID,
        NVRAM_EF_PORT_SETTING_LID,
        NVRAM_EF_PS_L2COPRO_FILTER_SETTINGS_LID,  //L2Copro filter settings
        NVRAM_EF_CLASSMARK_RACAP_LID,
        NVRAM_EF_SIM_ASSERT_LID,
        NVRAM_EF_RTC_DATA_LID,
        NVRAM_EF_NET_PAR_LID,
        NVRAM_EF_UMTS_PLMN_LID,
        NVRAM_EF_UMTS_IMSI_LID,
        NVRAM_EF_UMTS_START_HFN_LID,
        NVRAM_EF_UMTS_USIME_RRC_DYNAMIC_CAP_LID,
        NVRAM_EF_L1_3G_CAL_DATA_LID,
        NVRAM_EF_FLC_STATISTICS_LID,
        NVRAM_EF_ECOMPASS_DATA_LID,
        NVRAM_EF_BAND_BLOCK_LID,
        NVRAM_EF_UMTS_FREQUENCY_REPOSITORY_LID,


        NVRAM_EF_CUST_HW_LEVEL_TBL_LID,
        NVRAM_EF_UEM_MANUFACTURE_DATA_LID,
        NVRAM_EF_UEM_RMI_DATA_LID,


        /* GPS */
        NVRAM_EF_GPS_SETTING_DATA_LID,
        NVRAM_EF_MNL_SETTING_DATA_LID,

        /* __WIFI_BT_SINGLE_ANTENNA_SUPPORT__ */
        NVRAM_EF_BWCS_SETTING_DATA_LID,

    /********************************************
     *
     *  L4 Item
     *
     **********************************************/

        NVRAM_EF_L4_START,
        NVRAM_EF_TCM_STATISTICS_LID,
        NVRAM_EF_TCM_PDP_PROFILE_LID,
        NVRAM_EF_CFU_FLAG_LID,
        NVRAM_EF_MM_EQPLMN_LOCIGPRS_LID,
        NVRAM_EF_CTM_DEFAULT_SETTINGS_LID,
        NVRAM_EF_ALS_LINE_ID_LID,
        NVRAM_EF_MSCAP_LID,
        NVRAM_EF_ATCMD_ON_OFF_CHECK_LID,
        NVRAM_EF_SMSAL_SMS_LID,
        NVRAM_EF_SMSAL_MAILBOX_ADDR_LID,
        NVRAM_EF_SMSAL_COMMON_PARAM_LID,
        NVRAM_EF_SMSAL_SMSP_LID,
        NVRAM_EF_SMSAL_MWIS_LID,
        NVRAM_EF_CB_DEFAULT_CH_LID,
        NVRAM_EF_CB_CH_INFO_LID,
        NVRAM_EF_IMEI_IMEISV_LID,
        NVRAM_EF_SML_LID,                   /* SIM-ME Lock */
        NVRAM_EF_SIM_LOCK_LID, /* __SMART_PHONE_MODEM__ */
        NVRAM_EF_MS_SECURITY_LID,
    #ifndef __PHB_STORAGE_BY_MMI__
        NVRAM_EF_PHB_LID,
    #endif
        NVRAM_EF_PHB_LN_ENTRY_LID,      //__PHB_NO_CALL_LOG__
        NVRAM_EF_PHB_LN_TYPE_SEQ_LID,   //__PHB_NO_CALL_LOG__
        NVRAM_EF_PS_CONFORMANCE_TESTMODE_LID,   /* 2010.11.4   Add for EM menu for TestMode */
        NVRAM_EF_ETWS_SETTING_LID,
        NVRAM_EF_MOBILE_BROADBAND_PROVISION_CONTEXT_LID, //__MOBILE_BROADBAND_PROVISION_CONTEXT__
        NVRAM_EF_CSM_ESSP_LID,
        NVRAM_EF_L4_END,

    /********************************************
     *
     *  Other Items
     *
     **********************************************/


        NVRAM_EF_SYS_CACHE_OCTET_LID,
		/*DRM V2.0*/
        NVRAM_EF_DRM_SETTING_LID,
        NVRAM_EF_DRM_STIME_LID,
        NVRAM_EF_DRM_CERPATH_LID,

        NVRAM_EF_RAC_PREFERENCE_LID,

        NVRAM_EF_MM_IMSI_LOCI_GLOCI_LID,  //for man-in-middle-attach prevention


        NVRAM_EF_UMTS_URR_CONFIGURATION_LID,
        NVRAM_EF_TST_CONFIG_LID,
        /* Don't remove this line: insert LID definition above */
        NVRAM_EF_NVRAM_UNIT_TEST_LID,
        NVRAM_EF_ABM_PS_QOS_PROFILE_LID,
        NVRAM_EF_LAST_LID_CORE
    }nvram_lid_core_enum;

#define NVRAM_EF_L1_AFCDATA_SIZE         sizeof(l1cal_afcData_T)
#define NVRAM_EF_L1_AFCDATA_TOTAL        1

#endif
