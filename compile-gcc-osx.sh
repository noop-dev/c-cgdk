name="my_strategy"

if [ ! -f $name.c ]
then
    echo Unable to find $name.c > compilation.log
    exit 1
fi

rm -f $name

files=""

for i in *.c
do
    files="$files $i"
done

for i in model/*.c
do
    files="$files $i"
done

for i in platform/*.c
do
    files="$files $i"
done

gcc -std=gnu99 -fno-optimize-sibling-calls -fno-strict-aliasing -DONLINE_JUDGE -lm -x c -O2 -Wall -o $name $files 2>compilation.log
