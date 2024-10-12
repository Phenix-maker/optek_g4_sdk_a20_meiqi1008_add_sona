Rem del upgrade_combination_temp1.bin
Rem del upgrade_combination_temp2.bin

Rem optek_combination_copy src=bt_upgrade_base.bin dest=upgrade_combination_temp1.bin srcStartAddr=0x0000 destStartAddr=0x000 size=0x6F000
    optek_combination_copy src=firmware.bin dest=upgrade_combination_temp1.bin srcStartAddr=0x0000 destStartAddr=0x0000 size=0x400000

copy upgrade_combination_temp1.bin upgrade_combination_temp2.bin

Rem optek_combination_copy src=firmware.bin dest=upgrade_combination_temp2.bin srcStartAddr=0x6F000 destStartAddr=0x6F000 size=0x400000

Rem reserved_448k_for_usb_upgrade
Rem optek_combination_copy src=bt_upgrade_base.bin dest=upgrade_combination_temp2.bin srcStartAddr=0x0800 destStartAddr=0x0800 size=0x6E800
Rem optek_combination_copy src=bt_upgrade_base.bin dest=upgrade_combination_temp2.bin srcStartAddr=0x0800 destStartAddr=0x0800 size=0x68000

Rem reserved_64k*3_for_usb_upgrade
    optek_combination_copy src=bt_upgrade_base.bin dest=upgrade_combination_temp2.bin srcStartAddr=0x0800 destStartAddr=0x0800 size=0x48000

Rem reserved_128k_for_usb_upgrade
Rem optek_combination_copy src=bt_upgrade_base.bin dest=upgrade_combination_temp2.bin srcStartAddr=0x0800 destStartAddr=0x0800 size=0x1E000

copy upgrade_combination_temp2.bin upgrade_sFlash.bin 

del upgrade_combination_temp1.bin
del upgrade_combination_temp2.bin


Rem @pause