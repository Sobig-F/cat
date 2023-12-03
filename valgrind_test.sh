#!/bin/bash

COUNTER=0
SUCCESS=0
DIFF=""
COUNT_ERRORS=0

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
"-ce l test_files/test1.txt test_files/test2.txt"
"-e o -e test test_files/test1.txt test_files/test2.txt"
"-e hello -f test_files/test3.txt test_files/test1.txt test_files/test2.txt"
"-s test do_not_exist.txt"
)
run_test() {
    echo "--------------------"
    let "COUNTER++"
    param=$(echo "$@" | sed "s/FLAGS/$var/")

    valgrind --log-file="valgrind.log" --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all "${s21_command[@]}" $param  >> s21_cat.log
    
    echo $COUNTER " ${s21_command[@]} $param" >> errors.log
    grep -oe "[0-9] errors" valgrind.log >> errors.log
    echo >> errors.log

    "${sys_command[@]}" $param >> cat.log

    COUNT_ERRORS="$(grep -ce "0 errors" errors.log)"

    if [ "$COUNT_ERRORS" == "0" ]
    then
        echo $(diff -u s21_cat.log cat.log | grep -e '-')
        # echo "$COUNTER ${s21_command[@]} $param" >> s21_grep_fail.log
        # echo "$COUNTER ${sys_command[@]} $param" >> grep_fail.log
        echo "$COUNTER - Fail $param"
        exit
    else
        let "SUCCESS++"
        # echo "$COUNTER - Success $param"
        rm -f cat.log s21_cat.log
    fi
}

rm -f valgrind.log errors.log s21_cat.log s21_cat

gcc -g s21_cat.c -o s21_cat -lcheck -lcheck -lpthread -lm -D_GNU_SOURCE -lrt -lsubunit

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
# for i in "${manual[@]}"
# do
#     var="-"
#     run_test "$i"
# done
echo "ALL - ${COUNTER}"
echo "SUCCESS - ${SUCCESS}"

make rebuild