Rem set binPrefixion=BlueSea_BT_SDK
Rem set binGenerationDir=binGenerationTools

Rem set binPrefixion=usb_sd_upgrade_BlueSea_SDK
set binPrefixion=Optek_USB_SDK
set binGenerationDir=binGenerationTools_for_USB_Upgrade

cd %binGenerationDir%

call uart_boot_loader_for_programming_sFlash.bat

copy uart_boot_loader_programming_sFlash.bin ..\%binPrefixion%_uart_programm.bin 
copy upgrade_sFlash_SDK.bin ..\%binPrefixion%_upgrade.bin 

del uart_boot_loader_programming_sFlash.bin

del upgrade_sFlash_SDK.bin

pause