set Install_Dir=D:\usr
Rem set Install_Dir=C:\usr


Rem set Xporer_Version=Xplorer-7.0.9
Rem set Win32_Dir=RG-2018.9-win32
Rem set Core_Type=Optek_HiFi3_prod


set Xplorer_Version=Xporer-9.0.18
set Win32_Dir=RI-2021.8-win32
set Core_Type=optek_hvf4


Rem set Prj_Name=optek_g4_soc_sdk


Rem @echo off
Rem @echo on
Rem for %%A in ("1 2 3 4 5") do echo %%A
Rem for %%A in ("..") do echo %%~nxA
Rem @pause


Rem for %%A in ("../..") do set Prj_Name=%%~nxA
for %%A in ("..") do set Prj_Name=%%~nxA

Rem @pause

set PATH=%Install_Dir%\xtensa\Xporer_Version;%PATH%
%Install_Dir%/xtensa/XtDevTools\install\tools\%Win32_Dir%\XtensaTools\Tools\misc\xtensaenv.bat %Install_Dir%/xtensa/XtDevTools\install\builds\%Win32_Dir%\%Core_Type% %Install_Dir%/xtensa/XtDevTools\install\tools\%Win32_Dir%\XtensaTools %Install_Dir%/xtensa/XtDevTools\XtensaRegistry\%Win32_Dir% %Core_Type%
