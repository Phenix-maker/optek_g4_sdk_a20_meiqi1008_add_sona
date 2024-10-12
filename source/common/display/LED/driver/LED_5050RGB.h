#ifndef __LED_5050RGB_H__
#define __LED_5050RGB_H__


typedef struct {
	U32 spi_red_data:       8;
    U32 spi_green_data:     8;
    U32 spi_blue_data:      8;
} LED_RGB_SPI_DATA;

void LED_5050RGB_init(void);
void LED_5050RGB_open(void);
void LED_5050RGB_close(void);
void LED_5050RGB_fini(void);
void LED_5050RGB_disp_source(U8 audio_source);
void LED_5050RGB_disp_bt_status (U8 bt_status, U32 flash_time);
void LED_5050RGB_disp_music_play(U32 music_level);
void LED_5050RGB_oneLed_set(U8 Red, U8 Yellow, U8 Green);

#define LED_5050RGB_FUNC    LED_5050RGB_init,\
                            LED_5050RGB_open,\
                            LED_5050RGB_close,\
                            LED_5050RGB_fini,\
                            LED_5050RGB_disp_source,\
                            LED_5050RGB_disp_bt_status,\
                            LED_5050RGB_disp_music_play

#endif /* __LED_5050RGB_H__ */
