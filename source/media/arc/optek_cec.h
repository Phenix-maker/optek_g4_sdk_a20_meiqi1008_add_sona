#ifndef _OPTEK_CEC_H_
#define _OPTEK_CEC_H_


//#define OPTEK_CEC_LIB_ENABLE

#define CEC_HEADER_FLAG    0x00030C00


enum {
	CEC_READ_PHYSIC_ADDR_UNKNOWN,
	CEC_READ_PHYSIC_ADDR,
	CEC_READ_PHYSIC_ADDR_1,
	CEC_READ_PHYSIC_ADDR_2,
	CEC_READ_PHYSIC_ADDR_SUCCESS,
};


extern U8 cec_read_physcial_addr_status;


//----------------------------------------------------------
//==========================================================
//HdmiCECLogicalAddress
#define CEC_LOGICAL_ADDRESS_TV                    		0  		/*!< TV                    */
#define CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_1    		1  		/*!< Recording Device 1    */
#define CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_2    		2  		/*!< Recording Device 1    */
#define CEC_LOGICAL_ADDRESS_TUNER_1               		3  		/*!< Tuner 1               */
#define CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_1     		4  		/*!< Playback Device 1     */
#define CEC_LOGICAL_ADDRESS_AUDIO_SYSTEM          		5  		/*!< Audio System          */
#define CEC_LOGICAL_ADDRESS_TUNER_2               		6  		/*!< Tuner 2               */
#define CEC_LOGICAL_ADDRESS_TUNER_3               		7  		/*!< Tuner 3               */
#define CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_2     		8  		/*!< Playback Device 2     */
#define CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_3    		9  		/*!< Recording Device 3    */
#define CEC_LOGICAL_ADDRESS_TUNER_4               		10 		/*!< Tuner 4               */
#define CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_3     		11 		/*!< Playback Device 3     */
#define CEC_LOGICAL_ADDRESS_RESERVED1             		12 		/*!< Reserved              */
#define CEC_LOGICAL_ADDRESS_RESERVED2             		13 		/*!< Reserved              */
#define CEC_LOGICAL_ADDRESS_FREE_USE              		14 		/*!< Free Use              */
#define CEC_LOGICAL_ADDRESS_BROADCAST 					15 		/*!< Unregistred/Broadcast */
#define HDMI_CEC_UNREGISTRED_DEVICE				        0xf0
#define HDMI_CEC_AUDIO_DEVICE				            0x50



//HdmiCECPowerStatus
#define CEC_OPCODE_POLLING_MESSAGE                		0x02	//0x00
#define CEC_POWER_STATUS_ON                      		0x00	/*!< On                          */
#define CEC_POWER_STATUS_STANDBY                 		0x01	/*!< Standby                     */
#define CEC_POWER_STATUS_TRANSITION_STANDBY_TO_ON		0x02	/*!< In Transition Standby to On */
#define CEC_POWER_STATUS_TRANSITION_ON_TO_STANDBY		0x03	/*!< In Transition On to StandBy */


//HdmiCECFeatureOpcode
#define CEC_OPCODE_FEATURE_ABORT                 		0x00	/*!< Abort						*/
#define CEC_OPCODE_IMAGE_VIEW_ON                 		0x04	/*!< Image view on				*/
#define CEC_OPCODE_TUNER_STEP_INCREMENT          		0x05	/*!< Tuner step increment		*/
#define CEC_OPCODE_TUNER_STEP_DECREMENT          		0x06	/*!< Tuner step decrement		*/
#define CEC_OPCODE_TUNER_DEVICE_STATUS           		0x07	/*!< Tuner device status		*/
#define CEC_OPCODE_GIVE_TUNER_DEVICE_STATUS      		0x08	/*!< Give tuner device status	*/
#define CEC_OPCODE_RECORD_ON                     		0x09	/*!< Record on					*/
#define CEC_OPCODE_RECORD_STATUS                 		0x0A	/*!< Record status				*/
#define CEC_OPCODE_RECORD_OFF                    		0x0B	/*!< Record off					*/
#define CEC_OPCODE_TEXT_VIEW_ON                  		0x0D	/*!< Text view on				*/
#define CEC_OPCODE_RECORD_TV_SCREEN              		0x0F	/*!< Record tv screen			*/
#define CEC_OPCODE_GIVE_DECK_STATUS              		0x1A	/*!< Give deck status			*/
#define CEC_OPCODE_DECK_STATUS                   		0x1B	/*!< Deck status				*/
#define CEC_OPCODE_SET_MENU_LANGUAGE             		0x32	/*!< Set menu language			*/
#define CEC_OPCODE_CLEAR_ANALOGUE_TIMER          		0x33	/*!< Clear analogue timer		*/
#define CEC_OPCODE_SET_ANALOGUE_TIMER            		0x34	/*!< Set analogue timer			*/
#define CEC_OPCODE_TIMER_STATUS                  		0x35	/*!< Timer status				*/
#define CEC_OPCODE_STANDBY                       		0x36	/*!< Standby					*/
#define CEC_OPCODE_PLAY                          		0x41	/*!< Play						*/
#define CEC_OPCODE_DECK_CONTROL                 		0x42	/*!< Deck control				*/
#define CEC_OPCODE_TIMER_CLEARED_STATUS          		0x43	/*!< Timer cleared status		*/
#define CEC_OPCODE_USER_CONTROL_PRESSED          		0x44	/*!< User control pressed		*/
#define CEC_OPCODE_USER_CONTROL_RELEASED         		0x45	/*!< User control released		*/
#define CEC_OPCODE_GIVE_OSD_NAME                 		0x46	/*!< Give osd name				*/
#define CEC_OPCODE_SET_OSD_NAME                  		0x47	/*!< Set  osd name				*/
#define CEC_OPCODE_SET_OSD_STRING                		0x64	/*!< Set  osd string			*/
#define CEC_OPCODE_SET_TIMER_PROGRAM_TITLE       		0x67	/*!< Set timer program title	*/
#define CEC_OPCODE_SYSTEM_AUDIO_MODE_REQUEST     		0x70	/*!< System audio mode request	*/
#define CEC_OPCODE_GIVE_AUDIO_STATUS             		0x71	/*!< Give audio status			*/
#define CEC_OPCODE_SET_SYSTEM_AUDIO_MODE         		0x72	/*!< Set system audio mode		*/
#define CEC_OPCODE_REPORT_AUDIO_STATUS           		0x7A	/*!< Report audio status		*/
#define CEC_OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS 		0x7D	/*!< Give system audio mode status*/
#define CEC_OPCODE_REPORT_SYSTEM_AUDIO_MODE_STATUS      0x7E	/*!< System audio mode status 	*/
#define CEC_OPCODE_SYSTEM_AUDIO_MODE_STATUS      		0x7E	/*!< System audio mode status 	*/
#define CEC_OPCODE_ROUTING_CHANGE                		0x80	/*!< Routing change				*/
#define CEC_OPCODE_ROUTING_INFORMATION           		0x81	/*!< Routing information		*/
#define CEC_OPCODE_ACTIVE_SOURCE                 		0x82	/*!< Active source				*/
#define CEC_OPCODE_GIVE_PHYSICAL_ADDRESS         		0x83	/*!< Give physical address		*/
#define CEC_OPCODE_REPORT_PHYSICAL_ADDRESS       		0x84	/*!< Report physical address	*/
#define CEC_OPCODE_REQUEST_ACTIVE_SOURCE         		0x85	/*!< Request active source 		*/
#define CEC_OPCODE_SET_STREAM_PATH               		0x86	/*!< Set stream path			*/
#define CEC_OPCODE_DEVICE_VENDOR_ID                     0x87	/*!< Report Device vendor ID	*/
#define CEC_OPCODE_REPORT_DEVICE_VENDOR_ID           	0x87	/*!< Report Device vendor ID	*/
#define CEC_OPCODE_VENDOR_COMMAND                		0x89	/*!< Vendor command				*/
#define CEC_OPCODE_VENDOR_REMOTE_BUTTON_DOWN     		0x8A	/*!< Vendor remote button down	*/
#define CEC_OPCODE_VENDOR_REMOTE_BUTTON_UP       		0x8B	/*!< Vendor remote button up	*/
#define CEC_OPCODE_GIVE_DEVICE_VENDOR_ID         		0x8C	/*!< Give device vendor ID		*/
#define CEC_OPCODE_MENU_REQUEST                  		0x8D	/*!< Menu request				*/
#define CEC_OPCODE_MENU_STATUS                   		0x8E	/*!< Menu status				*/
#define CEC_OPCODE_GIVE_DEVICE_POWER_STATUS      		0x8F	/*!< Give device power status	*/
#define CEC_OPCODE_REPORT_POWER_STATUS           		0x90	/*!< Report power status		*/
#define CEC_OPCODE_GET_MENU_LANGUAGE             		0x91	/*!< Get menu language			*/
#define CEC_OPCODE_SET_ANALOGUE_SERVICE          		0x92	/*!< Set analogue service		*/
#define CEC_OPCODE_SET_DIGITAL_SERVICE           		0x93	/*!< Set digital  service		*/
#define CEC_OPCODE_SET_DIGITAL_TIMER             		0x97	/*!< Set digital timer			*/
#define CEC_OPCODE_CLEAR_DIGITAL_TIMER           		0x99	/*!< Clear digital timer		*/
#define CEC_OPCODE_SET_AUDIO_RATE                		0x9A	/*!< Set audio rate				*/
#define CEC_OPCODE_INACTIVE_SOURCE               		0x9D	/*!< Inactive source			*/
#define CEC_OPCODE_CEC_VERSION                   		0x9E	/*!< CEC Version				*/
#define CEC_OPCODE_GET_CEC_VERSION               		0x9F	/*!< Get CEC Version			*/
#define CEC_OPCODE_VENDOR_COMMAND_WITH_ID        		0xA0	/*!< Vendor command with ID		*/
#define CEC_OPCODE_CLEAR_EXTERNAL_TIMER          		0xA1	/*!< Clear external timer 		*/
#define CEC_OPCODE_SET_EXTERNAL_TIMER            		0xA2	/*!< Set   external timer 		*/
#define CEC_OPCODE_REQUEST_SHORT_AUDIO_DESC            	0xA4	/*! 		*/
#define CEC_OPCODE_REPORT_SHORT_AUDIO_DESC            	0xA3	/*! 		*/
#define CEC_OPCODE_INITIATE_ARC							0xC0	/*!< Initiate ARC 				*/
#define CEC_OPCODE_REPORT_ARC_INITIATED					0xC1	/*!< Report ARC Initiated 		*/
#define CEC_OPCODE_REPORT_ARC_TERMINATED				0xC2	/*!< Report ARC Terminated 		*/
#define CEC_OPCODE_REQUEST_ARC_INITIATION				0xC3	/*!< Request ARC Initiation 	*/
#define CEC_OPCODE_REQUEST_ARC_TERMINATION				0xC4	/*!< Request ARC Termination 	*/
#define CEC_OPCODE_TERMINATE_ARC						0xC5	/*!< Terminate ARC			 	*/
#define CEC_OPCODE_CDC_MESSAGE							0xF8	/*!< CDC Message			 	*/
#define CEC_OPCODE_ABORT_MESSAGE                 		0xFF	/*!< This message is reserved for testing*/

//added by Optek
#define CEC_OPCODE_POLLING                 				0xFE	/*!Just one byte to checking logic ID */


//HdmiCECVersion
#define CEC_VERSION_1_1  								0x00	/*!< CEC Version 1.1  */
#define CEC_VERSION_1_2  								0x01	/*!< CEC Version 1.2  */
#define CEC_VERSION_1_2a 								0x02	/*!< CEC Version 1.2a */
#define CEC_VERSION_1_3  								0x03	/*!< CEC Version 1.3  */
#define CEC_VERSION_1_3a 								0x04  	/*!< CEC Version 1.3a */
#define CEC_VERSION_1_4 								0x05  	/*!< CEC Version 1.4 */


//Hdmi CEC Menu Request Type
#define CEC_MENU_ACTIVATE								0
#define CEC_MENU_DEACTIVATE								1
#define CEC_MENU_QUERY									2

//Hdmi CEC Abort Reason
#define CEC_ABORT_UNRECONGNIZED_OPT						0
#define CEC_ABORT_N0T_IN_CORRECT_MOD					1
#define CEC_ABORT_CANNOT_PROVIDE_SOURCE					2
#define CEC_ABORT_INVALID_OPRAND						3
#define CEC_ABORT_REFUSED								4
#define CEC_UNABLE_DETERMIN								5	


//Hdmi CEC PQL2 Application command
#define CEC_PQL2_ENABLE									0x00
#define CEC_PQL2_OK										0x01
#define CEC_PQL2_TS_DISABLE_PROCESSING					0x04
#define CEC_PQL2_ST_PROCESS_DISABLED					0x07
#define CEC_PQL2_VIDEO_INFO_REQUEST						0x06
#define CEC_PQL2_VIDEO_ATTRIBUTES						0x0d


//HdmiCECUserRemoteControlCommand
#define CEC_REMOTE_BUTTON_SELECT                       	0
#define CEC_REMOTE_BUTTON_UP                           	1
#define CEC_REMOTE_BUTTON_DOWN                         	2
#define CEC_REMOTE_BUTTON_LEFT                         	3
#define CEC_REMOTE_BUTTON_RIGHT                        	4
#define CEC_REMOTE_BUTTON_RIGHT_UP                     	5
#define CEC_REMOTE_BUTTON_RIGHT_DOWN                   	6
#define CEC_REMOTE_BUTTON_LEFT_UP                      	7
#define CEC_REMOTE_BUTTON_LEFT_DOWN                    	8
#define CEC_REMOTE_BUTTON_ROOT_MENU                    	9
#define CEC_REMOTE_BUTTON_SETUP_MENU                   	10
#define CEC_REMOTE_BUTTON_CONTENTS_MENU                	11
#define CEC_REMOTE_BUTTON_FAVORITE_MENU                	12
#define CEC_REMOTE_BUTTON_EXIT                         	13
#define CEC_REMOTE_BUTTON_NUMBER_0                     	32
#define CEC_REMOTE_BUTTON_NUMBER_1                     	33
#define CEC_REMOTE_BUTTON_NUMBER_2                     	34
#define CEC_REMOTE_BUTTON_NUMBER_3                     	35
#define CEC_REMOTE_BUTTON_NUMBER_4                     	36
#define CEC_REMOTE_BUTTON_NUMBER_5                     	37
#define CEC_REMOTE_BUTTON_NUMBER_6                     	38
#define CEC_REMOTE_BUTTON_NUMBER_7                     	39
#define CEC_REMOTE_BUTTON_NUMBER_8                     	40
#define CEC_REMOTE_BUTTON_NUMBER_9                     	41
#define CEC_REMOTE_BUTTON_DOT                          	42
#define CEC_REMOTE_BUTTON_ENTER                        	43
#define CEC_REMOTE_BUTTON_CLEAR                        	44
#define CEC_REMOTE_BUTTON_NEXT_FAVORITE                	47
#define CEC_REMOTE_BUTTON_CHANNEL_UP                   	48
#define CEC_REMOTE_BUTTON_CHANNEL_DOWN                 	49
#define CEC_REMOTE_BUTTON_PREVIOUS_CHANNEL             	50
#define CEC_REMOTE_BUTTON_SOUND_SELECT                 	51
#define CEC_REMOTE_BUTTON_INPUT_SELECT                 	52
#define CEC_REMOTE_BUTTON_DISPLAY_INFORMATION          	53
#define CEC_REMOTE_BUTTON_HELP                         	54
#define CEC_REMOTE_BUTTON_PAGE_UP                      	55
#define CEC_REMOTE_BUTTON_PAGE_DOWN                    	56
#define CEC_REMOTE_BUTTON_POWER                        	64
#define CEC_REMOTE_BUTTON_VOLUME_UP                    	65
#define CEC_REMOTE_BUTTON_VOLUME_DOWN                  	66
#define CEC_REMOTE_BUTTON_MUTE                         	67
#define CEC_REMOTE_BUTTON_PLAY                         	68
#define CEC_REMOTE_BUTTON_STOP                         	69
#define CEC_REMOTE_BUTTON_PAUSE                        	70
#define CEC_REMOTE_BUTTON_RECORD                       	71
#define CEC_REMOTE_BUTTON_REWIND                       	72
#define CEC_REMOTE_BUTTON_FAST_FORWARD                 	73
#define CEC_REMOTE_BUTTON_EJECT                        	74
#define CEC_REMOTE_BUTTON_FORWARD                      	75
#define CEC_REMOTE_BUTTON_BACKWARD                     	76
#define CEC_REMOTE_BUTTON_STOP_RECORD                  	77
#define CEC_REMOTE_BUTTON_PAUSE_RECORD                 	78
#define CEC_REMOTE_BUTTON_ANGLE                        	80
#define CEC_REMOTE_BUTTON_SUB_PICTURE                  	81
#define CEC_REMOTE_BUTTON_VIDEO_ON_DEMAND              	82
#define CEC_REMOTE_BUTTON_ELECTRONIC_PROGRAM_GUIDE     	83
#define CEC_REMOTE_BUTTON_TIMER_PROGRAMMING            	84
#define CEC_REMOTE_BUTTON_INITIAL_CONFIGURATION        	85
#define CEC_REMOTE_BUTTON_PLAY_FUNCTION                	96
#define CEC_REMOTE_BUTTON_PAUSE_PLAY_FUNCTION          	97
#define CEC_REMOTE_BUTTON_RECORD_FUNCTION              	98
#define CEC_REMOTE_BUTTON_PAUSE_RECORD_FUNCTION        	99
#define CEC_REMOTE_BUTTON_STOP_FUNCTION                	100
#define CEC_REMOTE_BUTTON_MUTE_FUNCTION                	101
#define CEC_REMOTE_BUTTON_RESTORE_VOLUME_FUNCTION      	102
#define CEC_REMOTE_BUTTON_TUNE_FUNCTION                	103
#define CEC_REMOTE_BUTTON_SELECT_MEDIA_FUNCTION        	104
#define CEC_REMOTE_BUTTON_SELECT_AV_INPUT_FUNCTION     	105
#define CEC_REMOTE_BUTTON_SELECT_AUDIO_INPUT_FUNCTION  	106
#define CEC_REMOTE_BUTTON_POWER_TOGGLE_FUNCTION        	107
#define CEC_REMOTE_BUTTON_POWER_OFF_FUNCTION           	108
#define CEC_REMOTE_BUTTON_POWER_ON_FUNCTION            	109
#define CEC_REMOTE_BUTTON_F1_BLUE                      	113
#define CEC_REMOTE_BUTTON_F2_RED                       	114
#define CEC_REMOTE_BUTTON_F3_GREEN                     	115
#define CEC_REMOTE_BUTTON_F4_YELLOW                    	116
#define CEC_REMOTE_BUTTON_F5                           	117
#define CEC_REMOTE_BUTTON_DATA                         	118


//HdmiCECDeviceState
#define CEC_DEVICE_STATE_ON                   			0	 /*!< All devices  */
#define CEC_DEVICE_STATE_STANDBY              			1	 /*!< All devices  */
#define CEC_DEVICE_STATE_TEXT_DISPLAY         			2	 /*!< TV devices   */
#define CEC_DEVICE_STATE_IMAGE_DISPLAY        			3	 /*!< TV devices   */
#define CEC_DEVICE_STATE_MENU_DISPLAY         			4	 /*!< TV devices   */
#define CEC_DEVICE_STATE_DEVICE_MENU_INACTIVE 			1	 /*!< TV devices   */
#define CEC_DEVICE_STATE_DEVICE_MENU_ACTIVE   			0	 /*!< TV devices   */
#define CEC_DEVICE_STATE_NOT_RECORDING        			7	 /*!< Recording devices */
#define CEC_DEVICE_STATE_RECORDING            			8	 /*!< Recording devices */
#define CEC_DEVICE_STATE_DECK_INACTIVE        			9	 /*!< Playback devices  */
#define CEC_DEVICE_STATE_DECK_ACTIVE          			10	 /*!< Playback devices  */



#define TV_MENU_LANGUAGE_BLANK							0
#define TV_MENU_LANGUAGE_ENG							1		//ENGLISH
#define TV_MENU_LANGUAGE_CHI							2		//CHINESE
#define TV_MENU_LANGUAGE_FRE							3		//FRENCH
#define TV_MENU_LANGUAGE_GER							4		//GERMAN		
#define TV_MENU_LANGUAGE_DUT							5		//DUTCH			//HeLan
#define TV_MENU_LANGUAGE_ITA							6		//ITALIAN		//YiDaLi
#define TV_MENU_LANGUAGE_SPA							7		//SPANISH		//XiBanYa
#define TV_MENU_LANGUAGE_SWE							8		//SWEDISH		//RuiDan
#define TV_MENU_LANGUAGE_POR							9		//PORTUGUESE	//PuTaoYa
#define TV_MENU_LANGUAGE_DEN							10		//DENISH		//DanMai
#define TV_MENU_LANGUAGE_NOR							11		//NORWEGIAN		//NuoWei
#define TV_MENU_LANGUAGE_FIN							12		//FINNISH		//FenLan
#define TV_MENU_LANGUAGE_POL							13		//POLISH		//PoLan
#define TV_MENU_LANGUAGE_TUR							14		//TURKISH		//TuErQi
#define TV_MENU_LANGUAGE_CZE							15		//CZECH			//JieKe
#define TV_MENU_LANGUAGE_HUN							16		//HUNGARIAN		//XiongYaLi
#define TV_MENU_LANGUAGE_JAP							17		//JAPANESS		//
#define TV_MENU_LANGUAGE_KOR							18		//KOREAN		//
#define TV_MENU_LANGUAGE_RUS							19		//RUSSIAN		//
#define TV_MENU_LANGUAGE_TRA							20		//TRAD.CHINESE	//




extern const U8 cec_logical_addr;
extern const U8 cec_logical_bc_addr;


void optek_cec_init (void);
void optek_cec_open (void);
void optek_cec_close (void);
void optek_cec_fini (void);

int cec_i2c_data_analyse (U32 header) ;

int cee_set_initiate_start(void);
int cee_set_initiate_stop(void);
void cee_set_initiat_wait_callback();

const char *optek_cec_version_get (void);
const char *optek_cec_data_get(void);
const char *optek_cec_time_get(void);


#endif //_OPTEK_CEC_H_
