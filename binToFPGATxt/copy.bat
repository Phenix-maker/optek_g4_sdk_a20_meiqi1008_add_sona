call DC_Optek_shortcut
Rem @pause

del .\%Prj_Name%
del .\%Prj_Name%.bin

Rem copy ..\..\bin\%Core_Type%\Debug\%Prj_Name% %Prj_Name%
Rem copy ..\bin\%Core_Type%\Debug\%Prj_Name% %Prj_Name%

Rem copy ..\bin\%Core_Type%\Debug\%Prj_Name% %Prj_Name%
copy ..\bin\%Core_Type%\Debug\%Prj_Name%.bin .\%Prj_Name%.bin

copy .\%Prj_Name%.bin firmware.bin

del .\%Prj_Name%.bin

Rem @pause
