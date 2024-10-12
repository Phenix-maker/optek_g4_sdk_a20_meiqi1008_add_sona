Rem Step 1 bat
call DC_Optek_shortcut


Rem copy ..\..\bin\%Core_Type%\Debug\BlueSea*
Rem copy ..\..\bin\%Core_Type%\Debug\%Prj_Name% %Prj_Name%


Rem xt-objcopy -O binary ..\..\bin\%Core_Type%\Debug\BlueSea* firmware.bin

Rem xt-objcopy --xtensa-core=%Core_Type% -O binary soc_btdm52_test_for_os_codec firmware.bin
Rem xt-objcopy --xtensa-core=%Core_Type% -O binary %Prj_Name% %Prj_Name%.bin

Rem xt-objcopy --xtensa-core=%Core_Type% -O binary ..\..\bin\%Core_Type%\Debug\%Prj_Name% firmware.bin
xt-objcopy --xtensa-core=%Core_Type% --gap-fill=0xFF -O binary ..\..\bin\%Core_Type%\Debug\%Prj_Name% firmware.bin

Rem copy %Prj_Name%.bin firmware.bin


Rem del %Prj_Name%
Rem del %Prj_Name%.bin


Rem Step 2 bat
Rem call upgrade_3bin.bat
call optek_combination_copy.bat


Rem Step 3 bat
call bin_added_1_volatile_chkSum_behind.bat


Rem Step 4 bat
uart_boot_loader_bin_combination upgrade_sFlash_SDK.bin BOOT_LOADER_FLAG:1 SFLASH_BIT_SIZE:64M uart_boot_loader_programming_sFlash.bin


del firmware.bin

Rem @pause

