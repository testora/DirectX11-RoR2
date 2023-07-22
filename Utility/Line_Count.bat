@echo off
setlocal

set "folderPath=."
set "extensions=*.cpp *.h *.hlsl"
set "totalCount=0"

for /r "%folderPath%" %%F in (%extensions%) do (
    set "filePath=%%F"
    set "lines=0"

    for /f %%L in ('type "%%F" ^| find /v /c ""') do (
        set "lines=%%L"
    )

    set /a totalCount+=lines
)

echo Total Lines: %totalCount%

pause
endlocal