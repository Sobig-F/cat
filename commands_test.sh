#!/bin/bash

COUNTER=0
SUCCESS=0
DIFF=""

s21_command="./s21_cat"
sys_command="cat"
flags=(
    "b"
    "e"
    "n"
    "s"
    "t"
)
tests=(
"FLAGS bytes.txt"
)
manual=(
"-b test do_not_exist.txt"
"-b bytes.txt test.txt"
)
run_test() {
    let "COUNTER++"
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    "${s21_command[@]}" $param >> s21_cat.log
    "${sys_command[@]}" $param >> cat.log
    DIFF="$(diff -s s21_cat.log cat.log)"
    echo $COUNTER " ${s21_command[@]} $param" >> all_comm.log
    if [ "$DIFF" != "Files s21_cat.log and cat.log are identical" ]
    then
        echo $(diff -u s21_cat.log cat.log | grep -e '-')
        echo "$COUNTER - Fail $param"
        exit
    else
        let "SUCCESS++"
        rm -f cat.log s21_cat.log
    fi
}

rm -f s21_cat.log cat.log valgrind.log

for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        run_test "$i"
    done
done
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            for var4 in "${flags[@]}"
            do
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3$var4"
                    run_test "$i"
                done
            done
        done
    done
done
for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done
echo "ALL - ${COUNTER}"
echo "SUCCESS - ${SUCCESS}"