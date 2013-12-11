@echo off

set name=my_strategy

if not exist %name%.c (
    echo Unable to find %name%.c > compilation.log
    exit 1
)

set VS_VERSION=

if "%VS_VERSION%"=="" (
   if not "%VS130COMNTOOLS%"=="" (
       call "%VS130COMNTOOLS%\vsvars32.bat"
       call "%VS130COMNTOOLS%\..\..\VC\bin\vcvars32.bat"
       set VS_VERSION="130"
   )
)

if "%VS_VERSION%"=="" (
   if not "%VS120COMNTOOLS%"=="" (
       call "%VS120COMNTOOLS%\vsvars32.bat"
       call "%VS120COMNTOOLS%\..\..\VC\bin\vcvars32.bat"
       set VS_VERSION="120"
       echo Compiling with Visual Studio 2013
   )
)

if "%VS_VERSION%"=="" (
   if not "%VS110COMNTOOLS%"=="" (
       call "%VS110COMNTOOLS%\vsvars32.bat"
       call "%VS110COMNTOOLS%\..\..\VC\bin\vcvars32.bat"
       set VS_VERSION="110"
       echo Compiling with Visual Studio 2012
   )
)

if "%VS_VERSION%"=="" (
   if not "%VS100COMNTOOLS%"=="" (
       call "%VS100COMNTOOLS%\vsvars32.bat"
       call "%VS100COMNTOOLS%\..\..\VC\bin\vcvars32.bat"
       set VS_VERSION="100"
       echo Compiling with Visual Studio 2010
   )
)

if "%VS_VERSION%"=="" (
   if not "%VS90COMNTOOLS%"=="" (
       call "%VS90COMNTOOLS%\vsvars32.bat"
       call "%VS90COMNTOOLS%\..\..\VC\bin\vcvars32.bat"
       set VS_VERSION="90"
       echo Compiling with Visual Studio 2008
   )
)

if "%VS_VERSION%"=="" (
   if not "%VS80COMNTOOLS%"=="" (
       call "%VS80COMNTOOLS%\vsvars32.bat"
       call "%VS80COMNTOOLS%\..\..\VC\bin\vcvars32.bat"
       set VS_VERSION="80"
       echo Compiling with Visual Studio 2005
   )
)

if "%VS_VERSION%"=="" (
   if not "%VS70COMNTOOLS%"=="" (
       call "%VS70COMNTOOLS%\vsvars32.bat"
       call "%VS70COMNTOOLS%\..\..\VC\bin\vcvars32.bat"
       set VS_VERSION="70"
       echo Compiling with Visual Studio 2003
   )
)

if "%VS_VERSION%"=="" (
    echo No Visual Studio installation has been found
    exit 1
)

echo ***

del /F /Q %name%.exe

SET FILES=

for %%i in (*.c) do (
    call concatenate %%i
)

for %%i in (model\*.c) do (
    call concatenate %%i
)

for %%i in (platform\*.c) do (
    call concatenate %%i
)

cl /Fe%name%.exe /W4 /F268435456 /O2 /DONLINE_JUDGE /DWIN32 ws2_32.lib %FILES% 1>compilation.log
