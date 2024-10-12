
Rem bin_added_1_chkSum_behind upgrade_JVC_HR55.bin UPGRADE_START_POSITION_FOR_CHECKSUM:0x0000 upgrade_JVC_HR55x_chkSum.bin

Rem bin_added_1_chkSum_behind upgrade_JVC_HR55.bin UPGRADE_START_POSITION_FOR_CHECKSUM:0x20000 upgrade_JVC_HR55x_chkSum.bin

Rem bin_added_1_volatile_chkSum_behind upgrade_sFlash.bin UPGRADE_START_POSITION_FOR_CHECKSUM:0x60000 upgrade_Bluesea_SDK.bin

Rem //for the bin file checksum
Rem bin_added_1_volatile_chkSum_behind upgrade_sFlash.bin UPGRADE_START_POSITION_FOR_CHECKSUM:0x0000 usb_upgrade_BlueSea_SDK.bin

Rem //for app project bin of the bin file checksum
Rem bin_added_1_volatile_chkSum_behind upgrade_sFlash.bin UPGRADE_START_POSITION_FOR_CHECKSUM:0x70000 upgrade_sFlash_SDK.bin
bin_add_chksum_aligned_4 upgrade_sFlash.bin UPGRADE_START_POSITION_FOR_CHECKSUM:0x70000 upgrade_sFlash_SDK.bin


del upgrade_sFlash.bin


Rem @pause


Rem convertFPGA16.exe soctest.bin soctest16b.txt
Rem convertFPGA8.exe soctest.bin soctest8b.txt
Rem call upgrade_3bin.bat
