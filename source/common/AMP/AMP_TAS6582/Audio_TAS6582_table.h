typedef unsigned char cfg_u8;
typedef union {
    struct {
        cfg_u8 offset;
        cfg_u8 value;
    };
    struct {
        cfg_u8 command;
        cfg_u8 param;
    };
} cfg_reg;

#define CFG_META_SWITCH (255)
#define CFG_META_DELAY  (254)
#define CFG_META_BURST  (253)

/* Example C code */
/*
    // Externally implemented function that can write n-bytes to the device
    // PCM51xx and TAS5766 targets require the high bit (0x80) of the I2C register to be set on multiple writes.
    // Refer to the device data sheet for more information.
    extern int i2c_write(unsigned char *data, int n);
    // Externally implemented function that delays execution by n milliseconds
    extern int delay(int n);
    // Example implementation.  Call like:
    //     transmit_registers(registers, sizeof(registers)/sizeof(registers[0]));
    void transmit_registers(cfg_reg *r, int n)
    {
        int i = 0;
        while (i < n) {
            switch (r[i].command) {
            case CFG_META_SWITCH:
                // Used in legacy applications.  Ignored here.
                break;
            case CFG_META_DELAY:
                delay(r[i].param);
                break;
            case CFG_META_BURST:
                i2c_write((unsigned char *)&r[i+1], r[i].param);
                i +=  (r[i].param / 2) + 1;
                break;
            default:
                i2c_write((unsigned char *)&r[i], 2);
                break;
            }
            i++;
        }
    }
 */

const cfg_reg registers[] = {
    { 0x00, 0x00 },
    { 0x7f, 0x00 },
    { 0x01, 0x11 },
    { CFG_META_DELAY, 5 },
// Set to Sleep
    { 0x03, 0x22 },
    { 0x04, 0x22 },
//Tuning Coeffs
// Book 0x78
    { 0x7f, 0x78 },
    { 0x00, 0x02 },
    { 0xfc, 0x00 }, //  Class-H Advanced Controls
    { 0xfd, 0x00 },
    { 0xfe, 0x00 },
    { 0xff, 0x00 },
    { 0x00, 0x03 },
    { 0x04, 0x00 }, //  Class-H Advanced Controls
    { 0x05, 0x00 },
    { 0x06, 0x00 },
    { 0x07, 0x00 },
    { 0x08, 0x01 }, //  Class-H Advanced Controls
    { 0x09, 0x01 },
    { 0x0a, 0x01 },
    { 0x0b, 0x01 },
    { 0x0c, 0x01 }, //  Class-H Advanced Controls
    { 0x0d, 0x01 },
    { 0x0e, 0x01 },
    { 0x0f, 0x01 },
    { 0x10, 0x03 }, //  Class-H Advanced Controls
    { 0x11, 0x03 },
    { 0x12, 0x03 },
    { 0x13, 0x03 },
    { 0x14, 0x03 }, //  Class-H Advanced Controls
    { 0x15, 0x03 },
    { 0x16, 0x03 },
    { 0x17, 0x03 },
    { 0x18, 0x07 }, //  Class-H Advanced Controls
    { 0x19, 0x07 },
    { 0x1a, 0x07 },
    { 0x1b, 0x07 },
    { 0x1c, 0x07 }, //  Class-H Advanced Controls
    { 0x1d, 0x07 },
    { 0x1e, 0x07 },
    { 0x1f, 0x07 },
    { 0x20, 0x0f }, //  Class-H Advanced Controls
    { 0x21, 0x0f },
    { 0x22, 0x0f },
    { 0x23, 0x0f },
    { 0x24, 0x0f }, //  Class-H Advanced Controls
    { 0x25, 0x0f },
    { 0x26, 0x0f },
    { 0x27, 0x0f },
    { 0x28, 0x1f }, //  Class-H Advanced Controls
    { 0x29, 0x1f },
    { 0x2a, 0x1f },
    { 0x2b, 0x1f },
    { 0x2c, 0x1f }, //  Class-H Advanced Controls
    { 0x2d, 0x1f },
    { 0x2e, 0x1f },
    { 0x2f, 0x1f },
    { 0x30, 0x3f }, //  Class-H Advanced Controls
    { 0x31, 0x3f },
    { 0x32, 0x3f },
    { 0x33, 0x3f },
    { 0x34, 0x3f }, //  Class-H Advanced Controls
    { 0x35, 0x3f },
    { 0x36, 0x3f },
    { 0x37, 0x3f },
    { 0x38, 0x7f }, //  Class-H Advanced Controls
    { 0x39, 0x7f },
    { 0x3a, 0x7f },
    { 0x3b, 0x7f },
    { 0x3c, 0x7f }, //  Class-H Advanced Controls
    { 0x3d, 0x7f },
    { 0x3e, 0x7f },
    { 0x3f, 0x7f },
    { 0x40, 0xff }, //  Class-H Advanced Controls
    { 0x41, 0xff },
    { 0x42, 0xff },
    { 0x43, 0xff },
// Book 0x8c
    { 0x00, 0x00 },
    { 0x7f, 0x8c },
    { 0x00, 0x01 },
    { 0x3c, 0x00 }, //  Max Detect Sample = 100 samples
    { 0x3d, 0x00 },
    { 0x3e, 0x00 },
    { 0x3f, 0x63 },
    { 0x40, 0x00 }, //  Peak Hold Time (samples) = 960 samples
    { 0x41, 0x00 },
    { 0x42, 0x03 },
    { 0x43, 0xc0 },
    { 0x44, 0x7e }, //  Peak Offset = 0.99
    { 0x45, 0xb8 },
    { 0x46, 0x51 },
    { 0x47, 0xec },
    { 0x48, 0x7f }, //  Peak Decay = 0.9999 db/ms
    { 0x49, 0xfc },
    { 0x4a, 0x3a },
    { 0x4b, 0x61 },
    { 0x4c, 0x05 }, //  Smooth = 0.5 ms
    { 0x4d, 0x39 },
    { 0x4e, 0x47 },
    { 0x4f, 0xa6 },
    { 0x50, 0x04 }, //  Class-H Advanced Controls
    { 0x51, 0xb6 },
    { 0x52, 0x45 },
    { 0x53, 0xa2 },
    { 0x54, 0x04 }, //  Class-H Advanced Controls
    { 0x55, 0xb6 },
    { 0x56, 0x45 },
    { 0x57, 0xa2 },
    { 0x58, 0x04 }, //  Class-H Advanced Controls
    { 0x59, 0x68 },
    { 0x5a, 0x72 },
    { 0x5b, 0xb0 },
    { 0x5c, 0x04 }, //  Class-H Advanced Controls
    { 0x5d, 0x68 },
    { 0x5e, 0x72 },
    { 0x5f, 0xb0 },
    { 0x60, 0x04 }, //  Class-H Advanced Controls
    { 0x61, 0x1a },
    { 0x62, 0x9f },
    { 0x63, 0xbe },
    { 0x64, 0x04 }, //  Class-H Advanced Controls
    { 0x65, 0x1a },
    { 0x66, 0x9f },
    { 0x67, 0xbe },
    { 0x68, 0x03 }, //  Class-H Advanced Controls
    { 0x69, 0xcc },
    { 0x6a, 0xcc },
    { 0x6b, 0xcd },
    { 0x6c, 0x03 }, //  Class-H Advanced Controls
    { 0x6d, 0xcc },
    { 0x6e, 0xcc },
    { 0x6f, 0xcd },
    { 0x70, 0x03 }, //  Class-H Advanced Controls
    { 0x71, 0x7e },
    { 0x72, 0xf9 },
    { 0x73, 0xdb },
    { 0x74, 0x03 }, //  Class-H Advanced Controls
    { 0x75, 0x7e },
    { 0x76, 0xf9 },
    { 0x77, 0xdb },
    { 0x78, 0x03 }, //  Class-H Advanced Controls
    { 0x79, 0x31 },
    { 0x7a, 0x26 },
    { 0x7b, 0xe9 },
    { 0x7c, 0x03 }, //  Class-H Advanced Controls
    { 0x7d, 0x31 },
    { 0x7e, 0x26 },
    { 0x7f, 0xe9 },
    { 0x80, 0x02 }, //  Class-H Advanced Controls
    { 0x81, 0xe3 },
    { 0x82, 0x53 },
    { 0x83, 0xf8 },
    { 0x84, 0x02 }, //  Class-H Advanced Controls
    { 0x85, 0xe3 },
    { 0x86, 0x53 },
    { 0x87, 0xf8 },
    { 0x88, 0x02 }, //  Class-H Advanced Controls
    { 0x89, 0x95 },
    { 0x8a, 0x81 },
    { 0x8b, 0x06 },
    { 0x8c, 0x02 }, //  Class-H Advanced Controls
    { 0x8d, 0x95 },
    { 0x8e, 0x81 },
    { 0x8f, 0x06 },
    { 0x00, 0x02 },
    { 0x5c, 0x04 }, //  AGL Softening Alpha: 0.55 ms
    { 0x5d, 0xc1 },
    { 0x5e, 0xff },
    { 0x5f, 0x93 },
    { 0x60, 0x01 }, //  AGL Attack Rate: 0.2
    { 0x61, 0x12 },
    { 0x62, 0x6e },
    { 0x63, 0x98 },
    { 0x68, 0x7b }, //  AGL Softening Omega: 0.55 ms
    { 0x69, 0x3e },
    { 0x6a, 0x00 },
    { 0x6b, 0x6d },
    { 0x6c, 0x00 }, //  AGL Release Rate: 0.001
    { 0x6d, 0x00 },
    { 0x6e, 0xae },
    { 0x6f, 0xc3 },
    { 0x74, 0x08 }, //  AGL Threshold: 0 dB
    { 0x75, 0x00 },
    { 0x76, 0x00 },
    { 0x77, 0x00 },
    { 0x80, 0x00 }, //  Voltage Scale: 0 dB
    { 0x81, 0x08 },
    { 0x82, 0x80 },
    { 0x83, 0x00 },
    { 0x88, 0x1e }, //  OL Threshold = 40 ohm
    { 0x89, 0xc8 },
    { 0x8a, 0x00 },
    { 0x8b, 0x00 },
    { 0x8c, 0x00 }, //  SL Threshold = 1 ohm
    { 0x8d, 0xc5 },
    { 0x8e, 0x00 },
    { 0x8f, 0x00 },
    { 0x00, 0x03 },
    { 0x14, 0x00 }, //  CH-L Delay Time (samples) = 240 samples
    { 0x15, 0x00 },
    { 0x16, 0x00 },
    { 0x17, 0xf0 },
    { 0x18, 0x00 }, //  CH-R Delay Time (samples) = 240 samples
    { 0x19, 0x00 },
    { 0x1a, 0x00 },
    { 0x1b, 0xf0 },
    { 0x24, 0x00 }, //  Tone Generator Amplitude = -36 dB
    { 0x25, 0x20 },
    { 0x26, 0x75 },
    { 0x27, 0x68 },
    { 0x28, 0x02 }, //  PVDD Time Constant: 1 ms
    { 0x29, 0xa3 },
    { 0x2a, 0x9a },
    { 0x2b, 0xcc },
    { 0x34, 0x00 }, //  V smooth time(ms) = 1000 ms
    { 0x35, 0x00 },
    { 0x36, 0xae },
    { 0x37, 0xc3 },
    { 0x38, 0x00 }, //  I smooth time(ms) = 1000 ms
    { 0x39, 0x00 },
    { 0x3a, 0xae },
    { 0x3b, 0xc3 },
    { 0x48, 0x00 }, //  OLSL enable/disable = false
    { 0x49, 0x00 },
    { 0x4a, 0x00 },
    { 0x4b, 0x01 },
    { 0x54, 0x00 }, //  V/I smooth time(ms) = 1000 ms
    { 0x55, 0x00 },
    { 0x56, 0xae },
    { 0x57, 0xc3 },
    { 0xa4, 0x7f }, //  Tone Generator Frequency = 5.859375 Hz
    { 0xa5, 0xff },
    { 0xa6, 0xfd },
    { 0xa7, 0x88 },
    { 0xa8, 0xff }, //  Tone Generator Frequency = 5.859375 Hz
    { 0xa9, 0xe6 },
    { 0xaa, 0xde },
    { 0xab, 0x05 },
    { 0xac, 0x00 }, //  Tone Generator Frequency = 5.859375 Hz
    { 0xad, 0x00 },
    { 0xae, 0x7f },
    { 0xaf, 0xf7 },
    { 0x00, 0x04 },
    { 0x1c, 0x00 }, //  Band Pass Filter
    { 0x1d, 0x00 },
    { 0x1e, 0x66 },
    { 0x1f, 0xec },
    { 0x20, 0x00 },
    { 0x21, 0x00 },
    { 0x22, 0x00 },
    { 0x23, 0x00 },
    { 0x24, 0xff },
    { 0x25, 0xff },
    { 0x26, 0x99 },
    { 0x27, 0x14 },
    { 0x28, 0x0f },
    { 0x29, 0xff },
    { 0x2a, 0x31 },
    { 0x2b, 0xdd },
    { 0x2c, 0xf8 },
    { 0x2d, 0x00 },
    { 0x2e, 0xcd },
    { 0x2f, 0xd9 },
    { 0x58, 0x00 }, //  S2PS2P enable/disable = false
    { 0x59, 0x00 },
    { 0x5a, 0x00 },
    { 0x5b, 0x00 },
    { 0x5c, 0x00 }, //  I smooth Time(ms) = 500 ms
    { 0x5d, 0x01 },
    { 0x5e, 0x5d },
    { 0x5f, 0x85 },
    { 0x60, 0x00 }, //  P/N switch duration(ms) = 500 ms
    { 0x61, 0x00 },
    { 0x62, 0x5d },
    { 0x63, 0xc0 },
    { 0x64, 0x06 }, //  S2P Threshold = 1.5
    { 0x65, 0x46 },
    { 0x66, 0x46 },
    { 0x67, 0x46 },
    { 0x74, 0x00 }, //  BypassEQ = true
    { 0x75, 0x00 },
    { 0x76, 0x00 },
    { 0x77, 0x00 },
    { 0x9c, 0x00 }, //  Hold Time (Samples) = 250 Samples
    { 0x9d, 0x00 },
    { 0x9e, 0x00 },
    { 0x9f, 0xfa },
    { 0xa0, 0x40 }, //  Release Rate (dB/Sample) = 0.1 dB/Sample
    { 0xa1, 0xbd },
    { 0xa2, 0xb7 },
    { 0xa3, 0xc0 },
    { 0xa8, 0x7c }, //  Attack Rate (dB/Sample) = 0.25 dB/Sample
    { 0xa9, 0x5e },
    { 0xaa, 0x4e },
    { 0xab, 0x02 },
    { 0xb0, 0x00 }, //  Hold Time (Samples) = 250 Samples
    { 0xb1, 0x00 },
    { 0xb2, 0x00 },
    { 0xb3, 0xfa },
    { 0xb4, 0x40 }, //  Release Rate (dB/Sample) = 0.1 dB/Sample
    { 0xb5, 0xbd },
    { 0xb6, 0xb7 },
    { 0xb7, 0xc0 },
    { 0xbc, 0x7c }, //  Attack Rate (dB/Sample) = 0.25 dB/Sample
    { 0xbd, 0x5e },
    { 0xbe, 0x4e },
    { 0xbf, 0x02 },
    { 0xe4, 0x3c }, //  Clip Detect Threshold (dB) = 0 dB
    { 0xe5, 0x90 },
    { 0xe6, 0x38 },
    { 0xe7, 0x70 },
    { 0xe8, 0x00 }, //  Pseudo-Analog Clipdetect = OFF
    { 0xe9, 0x00 },
    { 0xea, 0x00 },
    { 0xeb, 0x00 },
    { 0x00, 0x05 },
    { 0x90, 0x06 }, //  S2G Threshold = 1.5
    { 0x91, 0x46 },
    { 0x92, 0x46 },
    { 0x93, 0x46 },
    { 0x08, 0x08 }, //  Band Pass Filter
    { 0x09, 0x00 },
    { 0x0a, 0x00 },
    { 0x0b, 0x00 },
    { 0x0c, 0x00 },
    { 0x0d, 0x00 },
    { 0x0e, 0x00 },
    { 0x0f, 0x00 },
    { 0x10, 0x00 },
    { 0x11, 0x00 },
    { 0x12, 0x00 },
    { 0x13, 0x00 },
    { 0x14, 0x00 },
    { 0x15, 0x00 },
    { 0x16, 0x00 },
    { 0x17, 0x00 },
    { 0x18, 0x00 },
    { 0x19, 0x00 },
    { 0x1a, 0x00 },
    { 0x1b, 0x00 },
    { 0x1c, 0x08 }, //  Band Pass Filter
    { 0x1d, 0x00 },
    { 0x1e, 0x00 },
    { 0x1f, 0x00 },
    { 0x20, 0x00 },
    { 0x21, 0x00 },
    { 0x22, 0x00 },
    { 0x23, 0x00 },
    { 0x24, 0x00 },
    { 0x25, 0x00 },
    { 0x26, 0x00 },
    { 0x27, 0x00 },
    { 0x28, 0x00 },
    { 0x29, 0x00 },
    { 0x2a, 0x00 },
    { 0x2b, 0x00 },
    { 0x2c, 0x00 },
    { 0x2d, 0x00 },
    { 0x2e, 0x00 },
    { 0x2f, 0x00 },
// Ungang DSP
    { 0x00, 0x00 },
    { 0x7f, 0x00 },
    { 0x39, 0x10 },
// Book 0x8c
    { 0x7f, 0x8c },
    { 0x00, 0x03 },
    { 0x0c, 0x00 }, //  Volume CH1 = 0 dB
    { 0x0d, 0x80 },
    { 0x0e, 0x00 },
    { 0x0f, 0x00 },
    { 0x10, 0x07 }, //  Volume CH2 = 24 dB
    { 0x11, 0xec },
    { 0x12, 0xa9 },
    { 0x13, 0xcd },
// Book 0x8d
    { 0x00, 0x00 },
    { 0x7f, 0x8d },
    { 0x00, 0x03 },
    { 0x0c, 0x07 }, //  Volume CH3 = 24 dB
    { 0x0d, 0xec },
    { 0x0e, 0xa9 },
    { 0x0f, 0xcd },
    { 0x10, 0x00 }, //  Volume CH4 = 0 dB
    { 0x11, 0x80 },
    { 0x12, 0x00 },
    { 0x13, 0x00 },
//Gang DSP
    { 0x00, 0x00 },
    { 0x7f, 0x00 },
    { 0x39, 0x00 },
// Book 0, Page 0
    { 0x02, 0x02 },
    { 0x05, 0x0f },
// Clock Polarity
    { 0x20, 0x00 },
    { 0x21, 0x00 },
// Input Word Length
    { 0x23, 0x0a },
// Output Word Length
    { 0x25, 0x1a },
// Output Word Length in Aux Ch1-4
    { 0x26, 0x22 },
// MSB offset of input audio
    { 0x27, 0x00 },
// LSB offset of Ch1-4
    { 0x28, 0x00 },
// LSB offset of LL Ch1-4
    { 0x29, 0x60 },
// MSB offset of output audio
    { 0x2c, 0xcf },
// V-predict LSB offset
    { 0x2d, 0xff },
// ISense LSB offset
    { 0x2e, 0x00 },
// Aux LSB offset
    { 0x2f, 0xff },
// Output data type in SDOUT
    { 0x31, 0x00 },
// LL path enable/disable
    { 0x32, 0x00 },
    { 0x39, 0x00 },
    { 0x3a, 0x02 },
    { 0x3b, 0x04 },
    { 0x40, 0x30 },
    { 0x41, 0x30 },
    { 0x42, 0x30 },
    { 0x43, 0x30 },
    { 0x44, 0x33 },
    { 0x45, 0x30 },
    { 0x46, 0x00 },
    { 0x47, 0x00 },
    { 0x48, 0x00 },
    { 0x49, 0x00 },
    { 0x4a, 0x00 },
    { 0x4b, 0x00 },
    { 0x4c, 0x00 },
    { 0x4d, 0x00 },
    { 0x4e, 0x08 },
    { 0x52, 0x00 },
    { 0x54, 0x00 },
    { 0x55, 0x00 },
    { 0x5b, 0x08 },
    { 0x5c, 0x40 },
    { 0x60, 0x00 },
    { 0x61, 0x02 },
    { 0x62, 0x00 },
    { 0x68, 0x00 },
    { 0x69, 0x00 },
    { 0x6a, 0x00 },
    { 0x6b, 0x00 },
    { 0x6c, 0x00 },
    { 0x6d, 0x19 },
    { 0x7c, 0x00 },
    { 0x8f, 0x00 },
    { 0x90, 0xa2 },
    { 0x91, 0x00 },
    { 0x92, 0x06 },
    { 0x93, 0x0f },
    { 0x94, 0x00 },
    { 0x95, 0x00 },
    { 0x96, 0x00 },
    { 0x97, 0x10 },
    { 0x9b, 0x00 },
    { 0x9c, 0x00 },
    { 0x9d, 0x00 },
    { 0x9e, 0x00 },
    { 0x9f, 0x10 },
    { 0xa0, 0x22 },
    { 0xa1, 0x20 },
    { 0xa4, 0x00 },
    { 0xaa, 0x04 },
    { 0xb0, 0x00 },
    { 0xb2, 0x00 },
    { 0xb3, 0x11 },
    { 0xb4, 0x11 },
    { 0xb5, 0x10 },
    { 0xb6, 0x08 },
    { 0xb7, 0x00 },
    { 0xb8, 0xc8 },
    { 0xe0, 0x00 },
    { 0xe1, 0x00 },
    { 0xe2, 0x11 },
    { 0xe3, 0x11 },
    { 0xe4, 0x00 },
// Channel state
    { 0x03, 0x34 },
    { 0x04, 0x43 },
// Clear Fault
    { 0x01, 0x08 },

};
