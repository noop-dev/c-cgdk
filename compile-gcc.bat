set name=my_strategy

if not exist %name%.c (
    echo Unable to find %name%.c > compilation.log
    exit 1
)

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

gcc.exe -std=gnu99 -static -fno-optimize-sibling-calls -fno-strict-aliasing -DONLINE_JUDGE -DWIN32 -lm -s -x c -Wl,--stack=268435456 -O2 -Wall -o %name%.exe %FILES% -lws2_32 -lwsock32 2>compilation.log
