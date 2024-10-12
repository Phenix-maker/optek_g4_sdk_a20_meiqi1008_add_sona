#ifndef _HFP_H_
#define _HFP_H_

/**
 ****************************************************************************************
 *
 * @file hfp.h
 *
 * @brief hfp profile commons headerfile.
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @defgroup hfp_profile_commons
 * @ingroup hfp_profile
 * 
 * @brief hfp profile commons implement.
 * 
 * @{ 
 ****************************************************************************************
 */



/*
 * rfcomm FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void hci_conn_del_rfcomm_by_HFP(void);

//interface to rfcomm hfp
struct rfcomm_dlc;
int hfp_data_send(struct rfcomm_dlc *d, void *data, u16 len);

struct rfcomm_dlc *rfcomm_get_top_dlc (void);
void *rfcomm_get_dlc_user_data(struct rfcomm_dlc *d);


void BtHCI_hfp_init(void);

void rfcomm_hfp_call_setup_cb (struct rfcomm_dlc *d);
void rfcomm_hfp_call (struct rfcomm_dlc *d);


/*
 * hf part(will improve later)
 ****************************************************************************************
 */
#define HFP_CLIP_BUF_SIZE 24

#define HFP_STATE				0
#define HFP_CIEV_CALL			1
#define HFP_CIEV_CALLSETUP		2
#define HFP_CIEV_CALLHELD		3
#define HFP_FEAT_L				4
#define HFP_FEAT_H				5

#define HFP_ROLE_SWITCH			6

//hfp state
#define HFP_CALL				7
#define HFP_CALLSETUP			8
#define HFP_CALLHELD			9

#define HFP_BATT_LEV			10

#define HFP_CODEC			    11

#define HFP_CLIP_BUF			12

enum {
	HFP_NOT_INIT,
		HFP_BRSF,
		HFP_BAC,
		HFP_CIND_1,
		HFP_CIND_2,
		HFP_CMER,
		HFP_CHLD,
		HFP_NREC,
		HFP_VGS,
		HFP_CLIP,
		HFP_APPLE_XAPL,						//Apple bluetooth accessory
		HFP_INITed,
		HFP_CLOSED
};


/*
 * Supported features bitmap
 ****************************************************************************************
 */

/* HF Supported Features: 
0: EC and/or NR function
1: Three-way calling
2: CLI presentation capability
3: Voice recognition activation
4: Remote volume control
5: Enhanced call status
6: Enhanced call control
7: Codec negotiation
8: HF Indicators
9: eSCO S4 (and T2) Settings Supported
10-31: Reserved for future definition
*/
#define HFP_HFSF_EC_NR_FUNCTION                          0
#define HFP_HFSF_THREE_WAY_CALLING                       1
#define HFP_HFSF_CLI_PRESENTATION_CAPABILITY             2
#define HFP_HFSF_VOICE_RECOGNITION_FUNCTION              3 
#define HFP_HFSF_REMOTE_VOLUME_CONTROL                   4
#define HFP_HFSF_ENHANCED_CALL_STATUS                    5  
#define HFP_HFSF_ENHANCED_CALL_CONTROL                   6 
#define HFP_HFSF_CODEC_NEGOTIATION                       7  
#define HFP_HFSF_HF_INDICATORS                           8
#define HFP_HFSF_ESCO_S4                                 9
#define HFP_HFSF_ENHANCED_VOICE_RECOGNITION_STATUS      10
#define HFP_HFSF_VOICE_RECOGNITION_TEXT                 11

/* AG Supported Features:
0: Three-way calling
1: EC and/or NR function
2: Voice recognition function
3: In-band ring tone capability
4: Attach a number to a voice tag
5: Ability to reject a call
6: Enhanced call status
7: Enhanced call control
8: Extended Error Result Codes
9: Codec negotiation
10: HF Indicators
11: eSCO S4 (and T2) Settings Supported
12-31: Reserved for future definition
*/
#define HFP_AGSF_THREE_WAY_CALLING                       0
#define HFP_AGSF_EC_NR_FUNCTION                          1
#define HFP_AGSF_VOICE_RECOGNITION_FUNCTION              2
#define HFP_AGSF_IN_BAND_RING_TONE                       3
#define HFP_AGSF_ATTACH_A_NUMBER_TO_A_VOICE_TAG          4
#define HFP_AGSF_ABILITY_TO_REJECT_A_CALL                5
#define HFP_AGSF_ENHANCED_CALL_STATUS                    6
#define HFP_AGSF_ENHANCED_CALL_CONTROL                   7
#define HFP_AGSF_EXTENDED_ERROR_RESULT_CODES             8
#define HFP_AGSF_CODEC_NEGOTIATION                       9
#define HFP_AGSF_HF_INDICATORS                          10
#define HFP_AGSF_ESCO_S4                                11
#define HFP_AGSF_ENHANCED_VOICE_RECOGNITION_STATUS      12
#define HFP_AGSF_VOICE_RECOGNITION_TEXT                 13


/*
 * macros
 ****************************************************************************************
 */
#define HFP_MAX_NUM_INDICATORS                  10
#define HFP_MAX_NUM_CALL_SERVICES               4
#define HFP_CALL_SERVICE_SIZE                    3
#define HFP_MAX_NUM_CODECS                      3

#define HFP_MAX_INDICATOR_DESC_SIZE  10
#define HFP_MAX_VR_TEXT_SIZE        32

/*
 * commands string
 ****************************************************************************************
 */    
#define HFP_SUPPORTED_FEATURES "+BRSF"
#define HFP_AVAILABLE_CODECS "+BAC"
#define HFP_INDICATOR "+CIND"
#define HFP_ENABLE_STATUS_UPDATE_FOR_AG_INDICATORS "+CMER"
#define HFP_ENABLE_CLIP "+CLIP"
#define HFP_ENABLE_CALL_WAITING_NOTIFICATION "+CCWA"
#define HFP_UPDATE_ENABLE_STATUS_FOR_INDIVIDUAL_AG_INDICATORS "+BIA" // +BIA:<enabled>,,<enabled>,,,<enabled>
#define HFP_SUPPORT_CALL_HOLD_AND_MULTIPARTY_SERVICES "+CHLD"
#define HFP_GENERIC_STATUS_INDICATOR "+BIND"
#define HFP_TRANSFER_AG_INDICATOR_STATUS "+CIEV" // +CIEV: <index>,<value>
#define HFP_TRANSFER_HF_INDICATOR_STATUS "+BIEV" // +BIEC: <index>,<value>
#define HFP_QUERY_OPERATOR_SELECTION "+COPS"     // +COPS: <mode>,0,<opearator>
#define HFP_ENABLE_EXTENDED_AUDIO_GATEWAY_ERROR "+CMEE"
#define HFP_EXTENDED_AUDIO_GATEWAY_ERROR "+CME ERROR"
#define HFP_TRIGGER_CODEC_CONNECTION_SETUP "+BCC"
#define HFP_CONFIRM_COMMON_CODEC "+BCS"
#define HFP_ANSWER_CALL "ATA"
#define HFP_HANG_UP_CALL "+CHUP"
#define HFP_CHANGE_IN_BAND_RING_TONE_SETTING "+BSIR"
#define HFP_CALL_PHONE_NUMBER "ATD"
#define HFP_REDIAL_LAST_NUMBER "+BLDN"
#define HFP_TURN_OFF_EC_AND_NR "+NREC" // EC (Echo CAnceling), NR (Noise Reduction)
#define HFP_ACTIVATE_VOICE_RECOGNITION "+BVRA" // Voice Recognition
#define HFP_SET_MICROPHONE_GAIN  "+VGM"
#define HFP_SET_SPEAKER_GAIN     "+VGS"
#define HFP_PHONE_NUMBER_FOR_VOICE_TAG "+BINP"
#define HFP_TRANSMIT_DTMF_CODES  "+VTS"
#define HFP_SUBSCRIBER_NUMBER_INFORMATION "+CNUM"
#define HFP_LIST_CURRENT_CALLS "+CLCC"
#define HFP_RESPONSE_AND_HOLD "+BTRH"

//custom
#define HFP_IPONE_BATTERY_REPORT_EN "+XAPL"

#define HFP_OK "OK"
#define HFP_ERROR "ERROR"
#define HFP_RING "RING"

/*
 * hfp codecs
 ****************************************************************************************
 */  
#define HFP_CODEC_CVSD 0x01
#define HFP_CODEC_MSBC 0x02

/*
 * hfp roles
 ****************************************************************************************
 */ 
typedef enum {
    HFP_ROLE_INVALID = 0,
    HFP_ROLE_AG,
    HFP_ROLE_HF,
} hfp_role_t;

/*
 * hfp command id
 ****************************************************************************************
 */ 
typedef enum {
    HFP_CMD_NONE = 0,
    HFP_CMD_ERROR,
    HFP_CMD_UNKNOWN,
    HFP_CMD_OK,
    HFP_CMD_RING,
    HFP_CMD_SUPPORTED_FEATURES,                             // 5
    HFP_CMD_AVAILABLE_CODECS,
    HFP_CMD_RETRIEVE_AG_INDICATORS_GENERIC,
    HFP_CMD_RETRIEVE_AG_INDICATORS,
    HFP_CMD_RETRIEVE_AG_INDICATORS_STATUS, 
    HFP_CMD_ENABLE_INDICATOR_STATUS_UPDATE,                 // 10
    HFP_CMD_ENABLE_INDIVIDUAL_AG_INDICATOR_STATUS_UPDATE,
    HFP_CMD_SUPPORT_CALL_HOLD_AND_MULTIPARTY_SERVICES,
    HFP_CMD_ENABLE_CLIP,
    HFP_CMD_AG_SENT_CLIP_INFORMATION,
    HFP_CMD_ENABLE_CALL_WAITING_NOTIFICATION,               // 15
    HFP_CMD_AG_SENT_CALL_WAITING_NOTIFICATION_UPDATE,
    HFP_CMD_LIST_GENERIC_STATUS_INDICATORS,
    HFP_CMD_RETRIEVE_GENERIC_STATUS_INDICATORS,
    HFP_CMD_RETRIEVE_GENERIC_STATUS_INDICATORS_STATE,
    HFP_CMD_SET_GENERIC_STATUS_INDICATOR_STATUS,            // 20
    HFP_CMD_TRANSFER_AG_INDICATOR_STATUS,
    HFP_CMD_QUERY_OPERATOR_SELECTION_NAME,
    HFP_CMD_QUERY_OPERATOR_SELECTION_NAME_FORMAT,
    HFP_CMD_ENABLE_EXTENDED_AUDIO_GATEWAY_ERROR,
    HFP_CMD_EXTENDED_AUDIO_GATEWAY_ERROR,                   // 25
    HFP_CMD_TRIGGER_CODEC_CONNECTION_SETUP,
    HFP_CMD_AG_SEND_COMMON_CODEC,
    HFP_CMD_AG_SUGGESTED_CODEC,
    HFP_CMD_HF_CONFIRMED_CODEC,
    HFP_CMD_CALL_ANSWERED,
    HFP_CMD_CALL_HOLD,
    HFP_CMD_HANG_UP_CALL,
    HFP_CMD_CHANGE_IN_BAND_RING_TONE_SETTING,
    HFP_CMD_CALL_PHONE_NUMBER,
    HFP_CMD_REDIAL_LAST_NUMBER,
    HFP_CMD_TURN_OFF_EC_AND_NR,
    HFP_CMD_AG_ACTIVATE_VOICE_RECOGNITION,
    HFP_CMD_HF_ACTIVATE_VOICE_RECOGNITION,
    HFP_CMD_AG_ACTIVATE_ENHANCED_VOICE_RECOGNITION,
    HFP_CMD_HF_REQUEST_PHONE_NUMBER,
    HFP_CMD_AG_SENT_PHONE_NUMBER,
    HFP_CMD_TRANSMIT_DTMF_CODES,
    HFP_CMD_SET_MICROPHONE_GAIN,
    HFP_CMD_SET_SPEAKER_GAIN,
    HFP_CMD_GET_SUBSCRIBER_NUMBER_INFORMATION,
    HFP_CMD_LIST_CURRENT_CALLS,
    HFP_CMD_RESPONSE_AND_HOLD_QUERY,
    HFP_CMD_RESPONSE_AND_HOLD_COMMAND,
    HFP_CMD_RESPONSE_AND_HOLD_STATUS,
    HFP_CMD_HF_INDICATOR_STATUS,
    HFP_CMD_AG_BATTERY_REPORT_EN, 
    HFP_CMD_HF_BATTERY_REPORT_EN,   
} hfp_command_t;

/*
 * command string parser states
 ****************************************************************************************
 */
typedef enum {
    HFP_PARSER_CMD_HEADER = 0,
    HFP_PARSER_CMD_SEQUENCE,
    HFP_PARSER_SECOND_ITEM,
    HFP_PARSER_THIRD_ITEM
} hfp_parser_state_t;

/*
 * hfp connection states
 ****************************************************************************************
 */
typedef enum {
    HFP_IDLE = 0, //0
    
    HFP_EXCHANGE_SUPPORTED_FEATURES,   // 5
    HFP_W4_EXCHANGE_SUPPORTED_FEATURES, 
    
    HFP_NOTIFY_ON_CODECS,
    HFP_W4_NOTIFY_ON_CODECS,
    
    HFP_RETRIEVE_INDICATORS, 
    HFP_W4_RETRIEVE_INDICATORS,        // 10
    
    HFP_RETRIEVE_INDICATORS_STATUS, 
    HFP_W4_RETRIEVE_INDICATORS_STATUS,
    
    HFP_ENABLE_INDICATORS_STATUS_UPDATE,
    HFP_W4_ENABLE_INDICATORS_STATUS_UPDATE,
    
    HFP_RETRIEVE_CAN_HOLD_CALL,        // 15
    HFP_W4_RETRIEVE_CAN_HOLD_CALL, 
    
    HFP_LIST_GENERIC_STATUS_INDICATORS,
    HFP_W4_LIST_GENERIC_STATUS_INDICATORS,
    
    HFP_RETRIEVE_GENERIC_STATUS_INDICATORS,
    HFP_W4_RETRIEVE_GENERIC_STATUS_INDICATORS,  //20
    
    HFP_RETRIEVE_INITITAL_STATE_GENERIC_STATUS_INDICATORS, 
    HFP_W4_RETRIEVE_INITITAL_STATE_GENERIC_STATUS_INDICATORS,
    
    HFP_SERVICE_LEVEL_CONNECTION_ESTABLISHED
} hfp_state_t;


/*
 * hfp ag indicator struct
 ****************************************************************************************
 */
typedef struct{
    char name[HFP_MAX_INDICATOR_DESC_SIZE];
    uint8_t index;
    uint8_t min_range;
    uint8_t max_range;
    uint8_t status;
    uint8_t mandatory;
    uint8_t enabled;
    uint8_t status_changed;
} hfp_ag_indicator_t;

typedef struct{
    char name[HFP_CALL_SERVICE_SIZE];
} hfp_call_service_t;

/*
 * hfp connection struct
 ****************************************************************************************
 */    
typedef struct {        

    // local role: HF or AG
    u8 local_role;//type:hfp_role_t

    u8 state;//type:hfp_state_t

    // used during service level connection establishment
    u8 command;//type:hfp_command_t
    u8 parser_state;//type:hfp_parser_state_t
    uint8_t      parser_item_index;
    uint8_t      parser_indicator_index;
    uint8_t      parser_indicator_value;
    uint8_t     parser_quoted;
    uint8_t  line_buffer[HFP_MAX_VR_TEXT_SIZE];
    uint8_t      line_size;
    
    uint32_t remote_supported_features;

    hfp_ag_indicator_t ag_indicators[HFP_MAX_NUM_INDICATORS];
    uint8_t ag_indicators_nr;
    uint8_t  enable_status_update_for_ag_indicators;
    
    uint8_t  supported_wbs;
    
    uint8_t remote_call_services_index;
    hfp_call_service_t remote_call_services[HFP_MAX_NUM_CALL_SERVICES];

    // HF -> AG configuration
    uint8_t clip_enabled;
    uint8_t call_waiting_notification_enabled;

    uint8_t found_equal_sign;
    uint8_t ignore_value;

    // establish codecs connection
    uint8_t suggested_codec;
    uint8_t codec_confirmed;
    uint8_t sco_codec;//sco_for_msbc_failed;
    uint8_t Reserveds2;//trigger_codec_exchange;

    uint8_t microphone_gain;
    uint8_t send_microphone_gain;

    uint8_t speaker_gain;
    uint8_t send_speaker_gain;
    
    // AG only
    uint8_t ag_echo_and_noise_reduction;
    uint8_t ag_activate_voice_recognition;
    uint8_t ag_call_hold_action;
    uint8_t ag_response_and_hold_action;

    // HF only
    uint8_t call_index;
    // also used for CLCC, CCWA, CLIP if set
    uint8_t bnip_type;       // 0 == not set
    char    bnip_number[25]; // 
    uint8_t Reserveds[20];
} hfp_connection_t;//This structure is limited in size to 300 bytes

/*
 * PUBLIC FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief parse hfp receive data
 * The return of this function means that there are at least one of two cases, 
 * case one is a command has been parsed and case two is data has been consumed
 *
 * @param[in out]   hfp_connection      environment of current hfp connection
 * @param[in]       buf                 hfp receive data
 * @param[in]       len                 hfp receive data len
 * @param[out]      consumed_len        consumed length
 *
 * @return end-of-line:  true:yes  false:no
 ****************************************************************************************
 */
U8 hfp_parse_rx_data(hfp_connection_t * hfp_connection, char *buf,uint8_t len,uint8_t *consumed_len);

#endif //_HFP_H_

///@} hfp_profile_commons
