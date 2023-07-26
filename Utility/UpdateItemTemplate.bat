@echo off
setlocal

set "SrcPath=..\Template\Item"
set "DstPath=%UserProfile%\Documents\Visual Studio 2022\Templates\ItemTemplates"

for /d %%i in ("%SrcPath%\*") do (
    pushd "%%i"
    tar -a -c -f "%DstPath%\%%~ni.zip" "*"
    popd
)

endlocal