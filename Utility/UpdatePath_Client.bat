@echo off
setlocal

set "cppExtension=.cpp"
set "hExtension=.h"

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

for %%F in ("%SourceFolder%\*%cppExtension%") do (
	move "%%F" "%PrivateFolder%\"
	powershell -Command "(gc '%targetProject%') -replace '%%~nxF', 'Private\%%~nxF' | Set-Content -Encoding UTF8 '%targetProject%'"
	powershell -Command "(gc '%targetFilters%') -replace '%%~nxF', 'Private\%%~nxF' | Set-Content -Encoding UTF8 '%targetFilters%'"
)

for %%F in ("%SourceFolder%\*%hExtension%") do (
	move "%%F" "%PublicFolder%\"
	powershell -Command "(gc '%targetProject%') -replace '%%~nxF', 'Public\%%~nxF' | Set-Content -Encoding UTF8 '%targetProject%'"
	powershell -Command "(gc '%targetFilters%') -replace '%%~nxF', 'Public\%%~nxF' | Set-Content -Encoding UTF8 '%targetFilters%'"
)

endlocal
