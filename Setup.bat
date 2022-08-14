@echo off

if not exist .\dependencies\ (
    echo dependencies not found 
    goto end
)
if not exist \Plugins\UnrealLibretro\Binaries\Win64\ThirdParty\ (
    echo libs copying...
    copy .\dependencies\libs\*  .\
)

REM create folders dependencies in root project 
md musics
md themes
md 7zip\Rom\


if exist .\Config\config.xml (
     echo "config.xml exists" 
     goto configsys
)
echo "config.xml not exists copying..." 
echo f|xcopy /s/y   .\dependencies\Config\config.xml .\Config\config.xml


:configsys

if exist .\Config\configsys.xml (
    echo "configsys.xml exists" 
    goto end
)
echo "configsys.xml not exists copying..."
echo f|xcopy /s/y .\dependencies\Config\configsys.xml .\Config\configsys.xml


:end
pause
