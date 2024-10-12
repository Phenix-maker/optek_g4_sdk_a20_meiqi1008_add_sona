#ifndef __AUDIO_USB_INPUT_H__
#define __AUDIO_USB_INPUT_H__


extern FIFO MicInFifo;
extern FIFO UsbDevUpFifo;

extern U32 *usbdev_up_fifo_buf;
extern U32 *usbdev_mic_in_fifo_buf;

extern U8 usbslaveRx_Dma_Opened;

void audio_usb_speaker_input_rx_init(void);
void audio_usb_speaker_input_rx_start(U32 *p_data, U32 size);
void audio_usb_speaker_input_receive_stop_req(void);
void audio_usb_speaker_input_put_next_frame_dataPtr(U32 *p_data, U32 size);
U32 audio_usb_speaker_input_ready_to_receive_dataPtr(void);

void audio_usb_speaker_input_rx_enable(void);
void audio_usb_speaker_input_rx_disable(void);

U8 audio_usb_speaker_input_rx_status_get(void);

#endif
