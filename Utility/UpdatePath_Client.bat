@echo off
setlocal

set "hExtension=.h"
set "inlExtension=.inl"
set "cppExtension=.cpp"

set "SourceFolder=..\Client"
set "PrivateFolder=..\Client\Private"
set "PublicFolder=..\Client\Public"
set "targetProject=..\Client\Client.vcxproj"
set "targetFilters=..\Client\Client.vcxproj.filters"

if not exist "%PrivateFolder%" (
	mkdir "%PrivateFolder%"
)

if not exist "%PublicFolder%" (
	mkdir "%PublicFolder%"
)

for %%F in ("%SourceFolder%\*%hExtension%") do (
	move "%%F" "%PublicFolder%\"
	powershell -Command "(gc '%targetProject%') -replace '%%~nxF', 'Public\%%~nxF' | Set-Content -Encoding UTF8 '%targetProject%'"
	powershell -Command "(gc '%targetFilters%') -replace '%%~nxF', 'Public\%%~nxF' | Set-Content -Encoding UTF8 '%targetFilters%'"
)
echo Header Complete!

for %%F in ("%SourceFolder%\*%inlExtension%") do (
	move "%%F" "%PublicFolder%\"
	powershell -Command "(gc '%targetProject%') -replace '%%~nxF', 'Public\%%~nxF' | Set-Content -Encoding UTF8 '%targetProject%'"
	powershell -Command "(gc '%targetFilters%') -replace '%%~nxF', 'Public\%%~nxF' | Set-Content -Encoding UTF8 '%targetFilters%'"
)
echo Inline Complete!

for %%F in ("%SourceFolder%\*%cppExtension%") do (
	move "%%F" "%PrivateFolder%\"
	powershell -Command "(gc '%targetProject%') -replace '%%~nxF', 'Private\%%~nxF' | Set-Content -Encoding UTF8 '%targetProject%'"
	powershell -Command "(gc '%targetFilters%') -replace '%%~nxF', 'Private\%%~nxF' | Set-Content -Encoding UTF8 '%targetFilters%'"
)
echo Source Complete!

echo Done!

endlocal
