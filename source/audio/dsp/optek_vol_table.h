#ifndef OPTEK_VOL_TABLE_H
#define OPTEK_VOL_TABLE_H


extern const int optek_vol_up_rate [];
extern const int optek_vol_down_rate [];
extern const int optek_vol_vol_table [];
extern const int optek_vol_down_ultra_rate [];
extern const int optek_vol_down_very_ultra_rate [];


//#define OPTEK_VOL_UP_RATE_0P2DB  optek_vol_up_rate[20]
#define OPTEK_VOL_UP_RATE_0P2DB  optek_vol_up_rate[10]
#define OPTEK_VOL_DOWN_RATE_0P2DB  optek_vol_down_rate[10]
//#define OPTEK_VOL_DOWN_RATE_0P2DB  optek_vol_down_rate[20]

#define OPTEK_DRC_ATTACK_RATE_0P2DB  optek_vol_down_rate[20]
#define OPTEK_DRC_RELEASE_RATE_0P2DB  optek_vol_up_rate[20]

#define OPTEK_DRC_ATTACK_RATE_0P1DB  optek_vol_down_rate[10]
#define OPTEK_DRC_RELEASE_RATE_0P1DB  optek_vol_up_rate[10]


#define OPTEK_DRC_LEV_DOWN_RATE_0P04DB  optek_vol_down_rate[4]		//0.04 * 44 * 4 =0.7db (about 4 samples for 1kHz)
#define OPTEK_DRC_LEV_DOWN_RATE_0P01DB  optek_vol_down_rate[1]		//0.04 * 44 * 4 =0.7db (about 4 samples for 1kHz)

#define OPTEK_DRC_LEV_DOWN_RATE_0P004DB  optek_vol_down_ultra_rate[4]		//0.004 * 44 * 4 =0.07db (about 4 samples for 100Hz)
#define OPTEK_DRC_LEV_DOWN_RATE_0P001DB  optek_vol_down_ultra_rate[1]		//0.004 * 44 * 4 =0.07db (about 4 samples for 100Hz)

#define OPTEK_DRC_LEV_DOWN_RATE_0P001DB  optek_vol_down_ultra_rate[1]		//0.004 * 44 * 4 =0.07db (about 4 samples for 100Hz)

#define OPTEK_DRC_LEV_DOWN_RATE_0P0001DB  optek_vol_down_very_ultra_rate[1]		//0.004 * 44 * 4 =0.07db (about 4 samples for 100Hz)
#define OPTEK_DRC_LEV_DOWN_RATE_0P0002DB  optek_vol_down_very_ultra_rate[2]		//0.004 * 44 * 4 =0.07db (about 4 samples for 100Hz)



#endif		//OPTEK_VOL_TABLE_H















                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
