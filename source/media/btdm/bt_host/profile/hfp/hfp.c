#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "string.h"
#include "at_command.h"
#include "bt_def.h"
#include "bt_lib_mask.h"
#include "app_bt_tx.h"
#include "hfp_hf.h"
#include "hfp_ag.h"
#include "hfp.h"

#ifdef BTDM52_ENABLE

#define HFP_Printf DBG_Printf

/**
 ****************************************************************************************
 *
 * @file hfp.c
 *
 * @brief hfp profile commons implement.
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup hfp_profile_commons
 * @ingroup hfp_profile
 * 
 * @brief hfp profile commons implement.
 * 
 * @{ 
 ****************************************************************************************
 */

/*
 * translates command string into hfp_command_t CMD
 ****************************************************************************************
 */
typedef struct {
    const char * command;
    hfp_command_t command_id;
} hfp_command_entry_t;

static const hfp_command_entry_t hfp_ag_commmand_table[] = {
    { "AT+BAC=",   HFP_CMD_AVAILABLE_CODECS },
    { "AT+BCC",    HFP_CMD_TRIGGER_CODEC_CONNECTION_SETUP },
    { "AT+BCS=",   HFP_CMD_HF_CONFIRMED_CODEC },
    { "AT+BIA=",   HFP_CMD_ENABLE_INDIVIDUAL_AG_INDICATOR_STATUS_UPDATE, }, // +BIA:<enabled>,,<enabled>,,,<enabled>
    { "AT+BIEV=",  HFP_CMD_HF_INDICATOR_STATUS },
//    { "AT+BIND=",  HFP_CMD_LIST_GENERIC_STATUS_INDICATORS },
//    { "AT+BIND=?", HFP_CMD_RETRIEVE_GENERIC_STATUS_INDICATORS },
//    { "AT+BIND?",  HFP_CMD_RETRIEVE_GENERIC_STATUS_INDICATORS_STATE },
//    { "AT+BINP=",  HFP_CMD_HF_REQUEST_PHONE_NUMBER },
//    { "AT+BLDN",   HFP_CMD_REDIAL_LAST_NUMBER },
    { "AT+BRSF=",  HFP_CMD_SUPPORTED_FEATURES },
    { "AT+BTRH=",  HFP_CMD_RESPONSE_AND_HOLD_COMMAND },
    { "AT+BTRH?",  HFP_CMD_RESPONSE_AND_HOLD_QUERY },
    { "AT+BVRA=",  HFP_CMD_HF_ACTIVATE_VOICE_RECOGNITION },
    { "AT+CCWA=",  HFP_CMD_ENABLE_CALL_WAITING_NOTIFICATION},
    { "AT+CHLD=",  HFP_CMD_CALL_HOLD },
    { "AT+CHLD=?", HFP_CMD_SUPPORT_CALL_HOLD_AND_MULTIPARTY_SERVICES },
    { "AT+CHUP",   HFP_CMD_HANG_UP_CALL },
    { "AT+CIND=?", HFP_CMD_RETRIEVE_AG_INDICATORS },
    { "AT+CIND?",  HFP_CMD_RETRIEVE_AG_INDICATORS_STATUS },
//    { "AT+CLCC",   HFP_CMD_LIST_CURRENT_CALLS },
    { "AT+CLIP=",  HFP_CMD_ENABLE_CLIP},
    { "AT+CMEE=",  HFP_CMD_ENABLE_EXTENDED_AUDIO_GATEWAY_ERROR},
    { "AT+CMER=",  HFP_CMD_ENABLE_INDICATOR_STATUS_UPDATE },
//    { "AT+CNUM",   HFP_CMD_GET_SUBSCRIBER_NUMBER_INFORMATION },
    { "AT+COPS=",  HFP_CMD_QUERY_OPERATOR_SELECTION_NAME_FORMAT },
//    { "AT+COPS?",  HFP_CMD_QUERY_OPERATOR_SELECTION_NAME },
    { "AT+NREC=",  HFP_CMD_TURN_OFF_EC_AND_NR, },
    { "AT+VGM=",   HFP_CMD_SET_MICROPHONE_GAIN },
    { "AT+VGS=",   HFP_CMD_SET_SPEAKER_GAIN },
//    { "AT+VTS=",   HFP_CMD_TRANSMIT_DTMF_CODES },
    { "ATA",       HFP_CMD_CALL_ANSWERED },
    { "AT+XAPL=",   HFP_CMD_HF_BATTERY_REPORT_EN},    
};

static const hfp_command_entry_t hfp_hf_commmand_table[] = {
    { "+BCS:",  HFP_CMD_AG_SUGGESTED_CODEC },
//    { "+BIND:", HFP_CMD_SET_GENERIC_STATUS_INDICATOR_STATUS },
//    { "+BINP",  HFP_CMD_AG_SENT_PHONE_NUMBER },
    { "+BRSF:", HFP_CMD_SUPPORTED_FEATURES },
    { "+BSIR:", HFP_CMD_CHANGE_IN_BAND_RING_TONE_SETTING },
    { "+BTRH:", HFP_CMD_RESPONSE_AND_HOLD_STATUS },
//    { "+BVRA:", HFP_CMD_AG_ACTIVATE_VOICE_RECOGNITION },
    { "+CCWA:", HFP_CMD_AG_SENT_CALL_WAITING_NOTIFICATION_UPDATE, },
    { "+CHLD:", HFP_CMD_SUPPORT_CALL_HOLD_AND_MULTIPARTY_SERVICES },
    { "+CIEV:", HFP_CMD_TRANSFER_AG_INDICATOR_STATUS},
    { "+CIND:", HFP_CMD_RETRIEVE_AG_INDICATORS_GENERIC },
//    { "+CLCC:", HFP_CMD_LIST_CURRENT_CALLS },
    { "+CLIP:", HFP_CMD_AG_SENT_CLIP_INFORMATION },
//    { "+CME ERROR:", HFP_CMD_EXTENDED_AUDIO_GATEWAY_ERROR },
//    { "+CNUM:", HFP_CMD_GET_SUBSCRIBER_NUMBER_INFORMATION},
//    { "+COPS:", HFP_CMD_QUERY_OPERATOR_SELECTION_NAME },
    { "+VGM:",  HFP_CMD_SET_MICROPHONE_GAIN },
    { "+VGS:",  HFP_CMD_SET_SPEAKER_GAIN},
    { "+XAPL=",  HFP_CMD_AG_BATTERY_REPORT_EN},

    { "ERROR",  HFP_CMD_ERROR},
    { "NOP",    HFP_CMD_NONE}, // dummy command used by unit tests
    { "OK",     HFP_CMD_OK },
    { "RING",   HFP_CMD_RING },
};

/*
 * parse command string implement
 ****************************************************************************************
 */
static int get_bit(uint16_t bitmap, int position){
    return (bitmap >> position) & 1;
}

static uint32_t store_bit(uint32_t bitmap, int position, uint8_t value){
    if (value){
        bitmap |= 1 << position;
    } else {
        bitmap &= ~ (1 << position);
    }
    return bitmap;
}

static uint16_t hfp_parse_indicator_index(hfp_connection_t * hfp_connection){
    uint16_t index = atoi((char *)&hfp_connection->line_buffer[0]);

    if (index > HFP_MAX_NUM_INDICATORS){
        HFP_Printf("ignoring invalid indicator index bigger then HFP_MAX_NUM_INDICATORS");
        return HFP_MAX_NUM_INDICATORS - 1;
    } 

    // indicator index enumeration starts with 1, we substract 1 to store in array with starting index 0
    if (index > 0){
        index -= 1;
    } else {
        HFP_Printf("ignoring invalid indicator index 0");
        return 0;
    }
    return index;
}

static void hfp_next_indicators_index(hfp_connection_t * hfp_connection){
    if (hfp_connection->parser_item_index < HFP_MAX_NUM_INDICATORS - 1){
        hfp_connection->parser_item_index++;
    } else {
        HFP_Printf("Ignoring additional indicator");
    }
}

static void hfp_next_remote_call_services_index(hfp_connection_t * hfp_connection){
    if (hfp_connection->remote_call_services_index < HFP_MAX_NUM_CALL_SERVICES - 1){
        hfp_connection->remote_call_services_index++;
    } else {
        HFP_Printf("Ignoring additional remote_call_services");
    }
}

static void parse_sequence(hfp_connection_t * hfp_connection){
    int value;
    int i;
    switch (hfp_connection->command){
        case HFP_CMD_SET_MICROPHONE_GAIN:
            value = atoi((char *)&hfp_connection->line_buffer[0]);
            hfp_connection->microphone_gain = value;
            HFP_Printf("hfp parse mic gain %d\n", value);
            break;
        case HFP_CMD_SET_SPEAKER_GAIN:
            value = atoi((char *)&hfp_connection->line_buffer[0]);
            hfp_connection->speaker_gain = value;
            HFP_Printf("hfp parse speaker gain %d\n", value);
            break;
        case HFP_CMD_HF_ACTIVATE_VOICE_RECOGNITION:
            value = atoi((char *)&hfp_connection->line_buffer[0]);
            hfp_connection->ag_activate_voice_recognition = value;
            HFP_Printf("hfp parse HFP_CMD_HF_ACTIVATE_VOICE_RECOGNITION %d\n", value);
            break;
        case HFP_CMD_TURN_OFF_EC_AND_NR:
            value = atoi((char *)&hfp_connection->line_buffer[0]);
            hfp_connection->ag_echo_and_noise_reduction = value;
            HFP_Printf("hfp parse HFP_CMD_TURN_OFF_EC_AND_NR %d\n", value);
            break;
        case HFP_CMD_CHANGE_IN_BAND_RING_TONE_SETTING:
            value = atoi((char *)&hfp_connection->line_buffer[0]);
            hfp_connection->remote_supported_features = store_bit(hfp_connection->remote_supported_features, HFP_AGSF_IN_BAND_RING_TONE, value);
            HFP_Printf("hfp parse HFP_CHANGE_IN_BAND_RING_TONE_SETTING %d\n", value);
            break;
        case HFP_CMD_HF_CONFIRMED_CODEC:
            hfp_connection->codec_confirmed = atoi((char*)hfp_connection->line_buffer);
            HFP_Printf("hfp parse HFP_CMD_HF_CONFIRMED_CODEC %d\n", hfp_connection->codec_confirmed);
            break;
        case HFP_CMD_AG_SUGGESTED_CODEC:
            hfp_connection->suggested_codec = atoi((char*)hfp_connection->line_buffer);
            HFP_Printf("hfp parse HFP_CMD_AG_SUGGESTED_CODEC %d\n", hfp_connection->suggested_codec);
            break;
        case HFP_CMD_SUPPORTED_FEATURES:
            hfp_connection->remote_supported_features = atoi((char*)hfp_connection->line_buffer);
            HFP_Printf("Parsed supported feature 0x%x\n", (int) hfp_connection->remote_supported_features);
            break;
        case HFP_CMD_AVAILABLE_CODECS:
            HFP_Printf("Parsed codec %s\n", hfp_connection->line_buffer);
            if (atoi((char*)hfp_connection->line_buffer) == HFP_CODEC_MSBC)
            {
                hfp_connection->supported_wbs = TRUE;
                HFP_Printf("remote supported wide band speech\n");
            }
            break;
        case HFP_CMD_RETRIEVE_AG_INDICATORS:
            strncpy((char *)hfp_connection->ag_indicators[hfp_connection->parser_item_index].name,  (char *)hfp_connection->line_buffer, HFP_MAX_INDICATOR_DESC_SIZE);
            hfp_connection->ag_indicators[hfp_connection->parser_item_index].name[HFP_MAX_INDICATOR_DESC_SIZE-1] = 0;
            hfp_connection->ag_indicators[hfp_connection->parser_item_index].index = hfp_connection->parser_item_index+1;
            HFP_Printf("Indicator %d: %s (", hfp_connection->ag_indicators_nr+1, hfp_connection->line_buffer);
            break;
        case HFP_CMD_RETRIEVE_AG_INDICATORS_STATUS:
            HFP_Printf("Parsed Indicator %d with status: %s\n", hfp_connection->parser_item_index+1, hfp_connection->line_buffer);
            hfp_connection->ag_indicators[hfp_connection->parser_item_index].status = atoi((char *) hfp_connection->line_buffer);
            hfp_next_indicators_index(hfp_connection);
            break;
        case HFP_CMD_ENABLE_INDICATOR_STATUS_UPDATE:
            hfp_next_indicators_index(hfp_connection);
            if (hfp_connection->parser_item_index != 4) break;
            HFP_Printf("Parsed Enable indicators: %s\n", hfp_connection->line_buffer);
            value = atoi((char *)&hfp_connection->line_buffer[0]);
            hfp_connection->enable_status_update_for_ag_indicators = (uint8_t) value;
            break;
        case HFP_CMD_SUPPORT_CALL_HOLD_AND_MULTIPARTY_SERVICES:
            HFP_Printf("Parsed Support call hold: %s\n", hfp_connection->line_buffer);
            if (hfp_connection->line_size > 2 ) break;
            memcpy((char *)hfp_connection->remote_call_services[hfp_connection->remote_call_services_index].name, (char *)hfp_connection->line_buffer, HFP_CALL_SERVICE_SIZE-1);
            hfp_connection->remote_call_services[hfp_connection->remote_call_services_index].name[HFP_CALL_SERVICE_SIZE - 1] = 0;
            hfp_next_remote_call_services_index(hfp_connection);
            break;
        case HFP_CMD_HF_INDICATOR_STATUS:
            hfp_connection->parser_indicator_index = hfp_parse_indicator_index(hfp_connection);
            HFP_Printf("Parsed HF indicator index %u", hfp_connection->parser_indicator_index);
            break;
        case HFP_CMD_ENABLE_INDIVIDUAL_AG_INDICATOR_STATUS_UPDATE:
            // AG parses new gen. ind. state
            if (hfp_connection->ignore_value){
                hfp_connection->ignore_value = 0;
                HFP_Printf("Parsed Enable AG indicator pos %u('%s') - unchanged (stays %u)\n", hfp_connection->parser_item_index,
                    hfp_connection->ag_indicators[hfp_connection->parser_item_index].name, hfp_connection->ag_indicators[hfp_connection->parser_item_index].enabled);
            }
            else if (hfp_connection->ag_indicators[hfp_connection->parser_item_index].mandatory){
                HFP_Printf("Parsed Enable AG indicator pos %u('%s') - ignore (mandatory)\n", 
                    hfp_connection->parser_item_index, hfp_connection->ag_indicators[hfp_connection->parser_item_index].name);
            } else {
                value = atoi((char *)&hfp_connection->line_buffer[0]);
                hfp_connection->ag_indicators[hfp_connection->parser_item_index].enabled = value;
                HFP_Printf("Parsed Enable AG indicator pos %u('%s'): %u\n", hfp_connection->parser_item_index,
                    hfp_connection->ag_indicators[hfp_connection->parser_item_index].name, value);
            }
            hfp_next_indicators_index(hfp_connection);
            break;
        case HFP_CMD_TRANSFER_AG_INDICATOR_STATUS:
            // indicators are indexed starting with 1
            hfp_connection->parser_item_index = hfp_parse_indicator_index(hfp_connection);
            HFP_Printf("Parsed status of the AG indicator %d, status ", hfp_connection->parser_item_index);
            break;
        case HFP_CMD_ERROR:
            break;
        case HFP_CMD_AG_SENT_CALL_WAITING_NOTIFICATION_UPDATE:
        case HFP_CMD_AG_SENT_CLIP_INFORMATION:
            strncpy(hfp_connection->bnip_number, (char *)hfp_connection->line_buffer, sizeof(hfp_connection->bnip_number));
            hfp_connection->bnip_number[sizeof(hfp_connection->bnip_number)-1] = 0;
            break;
        case HFP_CMD_CALL_HOLD:
            hfp_connection->ag_call_hold_action = hfp_connection->line_buffer[0] - '0';
            if (hfp_connection->line_buffer[1] != '\0'){
                hfp_connection->call_index = atoi((char *)&hfp_connection->line_buffer[1]);
            }
            break;
        case HFP_CMD_RESPONSE_AND_HOLD_COMMAND:
            hfp_connection->ag_response_and_hold_action = atoi((char *)&hfp_connection->line_buffer[0]);
            break;
        case HFP_CMD_ENABLE_CLIP:
            hfp_connection->clip_enabled = hfp_connection->line_buffer[0] != '0';
            break;
        case HFP_CMD_ENABLE_CALL_WAITING_NOTIFICATION:
            hfp_connection->call_waiting_notification_enabled = hfp_connection->line_buffer[0] != '0';
            break;
        default:
            break;
    }  
}

static hfp_command_t parse_command(const char * line_buffer, int isHandsFree){

    // table lookup based on role
    uint16_t num_entries;
    hfp_command_entry_t * table;
    if (isHandsFree == 0){
        table = hfp_ag_commmand_table;
        num_entries = sizeof(hfp_ag_commmand_table) / sizeof(hfp_command_entry_t);
    } else {
        table = hfp_hf_commmand_table;
        num_entries = sizeof(hfp_hf_commmand_table) / sizeof(hfp_command_entry_t);
    }
    // binary search
    uint16_t left = 0;
    uint16_t right = num_entries - 1;
    while (left <= right){
        uint16_t middle = left + (right - left) / 2;
        hfp_command_entry_t *entry = &table[middle];
        int match = strcmp(line_buffer, entry->command);
        if (match < 0){
            // search term is lower than middle element
            if (right == 0) break;
            right = middle - 1;
        } else if (match == 0){
            return entry->command_id;
        } else {
            // search term is higher than middle element
            left = middle + 1;
        }
    }

    // note: if parser in CMD_HEADER state would treats digits and maybe '+' as separator, match on "ATD" would work.
    // note: phone number is currently expected in line_buffer[3..]
    // prefix match on 'ATD', AG only
    if ((isHandsFree == 0) && (strncmp(line_buffer, HFP_CALL_PHONE_NUMBER, strlen(HFP_CALL_PHONE_NUMBER)) == 0)){
        return HFP_CMD_CALL_PHONE_NUMBER;
    }

    // Valid looking, but unknown commands/responses
    if ((isHandsFree == 0) && (strncmp(line_buffer, "AT+", 3) == 0)){
        return HFP_CMD_UNKNOWN;
    }

    if ((isHandsFree != 0) && (strncmp(line_buffer, "+", 1) == 0)){
        return HFP_CMD_UNKNOWN;
    }

    return HFP_CMD_NONE;
}

static void hfp_parser_store_byte(hfp_connection_t * hfp_connection, uint8_t byte){
    if ((hfp_connection->line_size + 1 ) >= HFP_MAX_INDICATOR_DESC_SIZE) return;
    hfp_connection->line_buffer[hfp_connection->line_size++] = byte;
    hfp_connection->line_buffer[hfp_connection->line_size] = 0;
}
static int hfp_parser_is_buffer_empty(hfp_connection_t * hfp_connection){
    return hfp_connection->line_size == 0;
}

static int hfp_parser_is_end_of_line(uint8_t byte){
    return (byte == '\n') || (byte == '\r');
}

static void hfp_parser_reset_line_buffer(hfp_connection_t *hfp_connection) {
    hfp_connection->line_size = 0;
}

static void hfp_parser_store_if_token(hfp_connection_t * hfp_connection, uint8_t byte){
    switch (byte){
        case ',':
		case '-':
        case ';':
        case '(':
        case ')':
        case '\n':
        case '\r':
            break;
        default:
            hfp_parser_store_byte(hfp_connection, byte);
            break;
    }
}

static bool hfp_parser_is_separator( uint8_t byte){
    switch (byte){
        case ',':
		case '-':
        case ';':
        case '\n':
        case '\r':
            return true;
        default:
            return false;
    }
}

static bool hfp_parse_byte(hfp_connection_t * hfp_connection, uint8_t byte, int isHandsFree){

    // handle doubles quotes
    if (byte == '"'){
        hfp_connection->parser_quoted = !hfp_connection->parser_quoted;
        return true;
    }
    if (hfp_connection->parser_quoted) {
        hfp_parser_store_byte(hfp_connection, byte);
        return true;
    }

    // ignore spaces outside command or double quotes (required e.g. for '+CME ERROR:.. command") 
    if ((byte == ' ') && (hfp_connection->parser_state != HFP_PARSER_CMD_HEADER)) return true;

    bool processed = true;

    switch (hfp_connection->parser_state) {
        case HFP_PARSER_CMD_HEADER:
            switch (byte) {
                case '\n':
                case '\r':
                case ';':
                    // ignore separator
                    break;
                case ':':
                case '?':
                    // store separator
                    hfp_parser_store_byte(hfp_connection, byte);
                    break;
                case '=':
                    // equal sign: remember and wait for next char to decided between '=?' and '=\?'
                    hfp_connection->found_equal_sign = true;
                    hfp_parser_store_byte(hfp_connection, byte);
                    return true;
                default:
                    // store if not lookahead
                    if (!hfp_connection->found_equal_sign) {
                        hfp_parser_store_byte(hfp_connection, byte);
                        return true;
                    }
                    // mark as lookahead
                    processed = false;
                    break;
            }

            // ignore empty tokens
            if (hfp_parser_is_buffer_empty(hfp_connection)) return true;

            // parse
            hfp_connection->command = parse_command((char *)hfp_connection->line_buffer, isHandsFree);

            // pick +CIND version based on connection state: descriptions during SLC vs. states later
            if (hfp_connection->command == HFP_CMD_RETRIEVE_AG_INDICATORS_GENERIC){
                switch(hfp_connection->state){
                    case HFP_W4_RETRIEVE_INDICATORS_STATUS:
                        hfp_connection->command = HFP_CMD_RETRIEVE_AG_INDICATORS_STATUS;
                        break;
                    case HFP_W4_RETRIEVE_INDICATORS:
                        hfp_connection->command = HFP_CMD_RETRIEVE_AG_INDICATORS;
                        break;
                    default:
                        hfp_connection->command = HFP_CMD_UNKNOWN;
                        break;
                }
            }

            HFP_Printf("command: '%s'\r\n", (char *)hfp_connection->line_buffer);

            // next state
            hfp_connection->found_equal_sign = false;
            hfp_parser_reset_line_buffer(hfp_connection);
            hfp_connection->parser_state = HFP_PARSER_CMD_SEQUENCE;

            return processed;

        case HFP_PARSER_CMD_SEQUENCE:
            // handle empty fields
            if ((byte == ',' ) && (hfp_connection->line_size == 0)){
                hfp_connection->line_buffer[0] = 0;
                hfp_connection->ignore_value = 1;
                parse_sequence(hfp_connection);
                return true;
            }

            hfp_parser_store_if_token(hfp_connection, byte);
            if (!hfp_parser_is_separator(byte)) return true;

            // ignore empty tokens
            if (hfp_parser_is_buffer_empty(hfp_connection) && (hfp_connection->ignore_value == 0)) return true;

            parse_sequence(hfp_connection);

            hfp_parser_reset_line_buffer(hfp_connection);

            switch (hfp_connection->command){
                case HFP_CMD_AG_SENT_CALL_WAITING_NOTIFICATION_UPDATE:
                case HFP_CMD_AG_SENT_CLIP_INFORMATION:
                case HFP_CMD_TRANSFER_AG_INDICATOR_STATUS:
                case HFP_CMD_RETRIEVE_AG_INDICATORS:
                case HFP_CMD_HF_INDICATOR_STATUS:
                    hfp_connection->parser_state = HFP_PARSER_SECOND_ITEM;
                    break;
                default:
                    break;
            }
            return true;

        case HFP_PARSER_SECOND_ITEM:

            hfp_parser_store_if_token(hfp_connection, byte);
            if (!hfp_parser_is_separator(byte)) return true;

            switch (hfp_connection->command){
                case HFP_CMD_TRANSFER_AG_INDICATOR_STATUS:
                    hfp_connection->ag_indicators[hfp_connection->parser_item_index].status = (uint8_t)atoi((char*)hfp_connection->line_buffer);
                    HFP_Printf("%d \n", hfp_connection->ag_indicators[hfp_connection->parser_item_index].status);
                    hfp_connection->ag_indicators[hfp_connection->parser_item_index].status_changed = 1;
                    break;
                case HFP_CMD_RETRIEVE_AG_INDICATORS:
                    hfp_connection->ag_indicators[hfp_connection->parser_item_index].min_range = atoi((char *)hfp_connection->line_buffer);
                    HFP_Printf("%s, ", hfp_connection->line_buffer);
                    break;
                case HFP_CMD_AG_SENT_CALL_WAITING_NOTIFICATION_UPDATE:
                case HFP_CMD_AG_SENT_CLIP_INFORMATION:
                    hfp_connection->bnip_type = (uint8_t)atoi((char*)hfp_connection->line_buffer);
                    break;
                case HFP_CMD_HF_INDICATOR_STATUS:
                    hfp_connection->parser_indicator_value = atoi((char *)&hfp_connection->line_buffer[0]);
                    break;
                default:
                    break;
            }

            hfp_parser_reset_line_buffer(hfp_connection);

            hfp_connection->parser_state = HFP_PARSER_THIRD_ITEM;

            return true;

        case HFP_PARSER_THIRD_ITEM:

            hfp_parser_store_if_token(hfp_connection, byte);
            if (!hfp_parser_is_separator(byte)) return true;

            switch (hfp_connection->command){
                case HFP_CMD_RETRIEVE_AG_INDICATORS:
                    hfp_connection->ag_indicators[hfp_connection->parser_item_index].max_range = atoi((char *)hfp_connection->line_buffer);
                    hfp_next_indicators_index(hfp_connection);
                    hfp_connection->ag_indicators_nr = hfp_connection->parser_item_index;
                    HFP_Printf("%s)\n", hfp_connection->line_buffer);
                    break;
                default:
                    break;
            }

            hfp_parser_reset_line_buffer(hfp_connection);

            if (hfp_connection->command == HFP_CMD_RETRIEVE_AG_INDICATORS){
                hfp_connection->parser_state = HFP_PARSER_CMD_SEQUENCE;
            } else {
                hfp_connection->parser_state = HFP_PARSER_CMD_HEADER;
            }
            return true;

        default:
            DBG_Assert(false);
            return true;
    }
}

U8 hfp_parse_rx_data(hfp_connection_t * hfp_connection, char *buf,uint8_t len,uint8_t *consumed_len)
{
    int i;
    for (i=0;i<len;i++)
    {
        while(!hfp_parse_byte(hfp_connection, buf[i], hfp_connection->local_role == HFP_ROLE_HF));

        // reset parser state on end-of-line
        if (hfp_parser_is_end_of_line(buf[i]))
        {
            hfp_connection->parser_item_index = 0;
            hfp_connection->parser_state = HFP_PARSER_CMD_HEADER;
            *consumed_len = i+1;
            return TRUE;
        }   
    }

    *consumed_len = i;  
    return FALSE;        
}

/*
 * PUBLIC FUNCTION DEFINITIONS
 ****************************************************************************************
 */
void hfp_state_init(struct rfcomm_dlc *pDLC)
{
	u8 *pData;
    
    pData = (hfp_connection_t *)rfcomm_get_dlc_user_data(pDLC);
    DBG_Assert (pData != NULL);

    #ifdef BT_TX_ROLE_ENABLE
    //do nothing
    #else
	pData[HFP_STATE] = HFP_NOT_INIT;
	pData[HFP_CIEV_CALL] = 0;
	pData[HFP_CIEV_CALLSETUP] = 0;
	pData[HFP_CIEV_CALLHELD] = 0;
	pData[HFP_FEAT_L] = 0;
	pData[HFP_FEAT_H] = 0;

	pData[HFP_ROLE_SWITCH] = 0;
	pData[HFP_CALL] = 0;
	pData[HFP_CALLSETUP] = 0;
	pData[HFP_CALLHELD] = 0;
    #endif
}

u8 hfp_is_inited (struct rfcomm_dlc *pDLC)
{
	u8 hfp_state;
	u8 *pData;
    
    pData = (hfp_connection_t *)rfcomm_get_dlc_user_data(pDLC);
    DBG_Assert (pData != NULL);


    #ifdef BT_TX_ROLE_ENABLE
    hfp_connection_t *hfp_connection = (hfp_connection_t *)pData;

    if (hfp_connection->state != HFP_W4_EXCHANGE_SUPPORTED_FEATURES)
	    return TRUE;
    else
        return FALSE;
    #else
	hfp_state = pData[HFP_STATE];

	if (hfp_state == HFP_NOT_INIT)
		return FALSE;
	else
		return TRUE;
    #endif
}

void hfp_init (void *pDLC)
{
	#ifdef BT_TX_ROLE_ENABLE
    hfp_ag_init(pDLC);
    #else
    hfp_hf_init(pDLC);
	#endif
}

u8 hci_sync_conn_cvsd_voice_setting_cb (void)
{
	struct rfcomm_dlc *pDLC;
    u8 *pData;

	pDLC = rfcomm_get_top_dlc();
	if (pDLC == NULL)
		return -1;
    
    pData = (hfp_connection_t *)rfcomm_get_dlc_user_data(pDLC);

    #ifdef BT_TX_ROLE_ENABLE
    hfp_connection_t *hfp_connection = (hfp_connection_t *)pData;
	if (hfp_connection->sco_codec == HFP_CODEC_MSBC)
		return 0;
	else
		return 1;    
    #else
	if (pData[HFP_CODEC] == HFP_CODEC_MSBC)
		return 0;
	else
		return 1;
    #endif    
}

u8 hfp_get_codec (void)
{
	struct rfcomm_dlc *pDLC;
    u8 *pData;

	pDLC = rfcomm_get_top_dlc();
	if (pDLC == NULL)
		return -1;
    
    pData = (hfp_connection_t *)rfcomm_get_dlc_user_data(pDLC);

    #ifdef BT_TX_ROLE_ENABLE
    hfp_connection_t *hfp_connection = (hfp_connection_t *)pData;
	return hfp_connection->sco_codec; 
    #else
	return pData[HFP_CODEC];
    #endif
}

void hfp_recv_data(struct rfcomm_dlc *pDLC, u8 *pBuf, u8 len)
{
	#ifdef BT_TX_ROLE_ENABLE
	hfp_AG_recv_process(pDLC,pBuf,len);
	#else
	hfp_HF_recv_process(pDLC,pBuf,len);
	#endif
}

#endif
///@} hfp_profile_commons
