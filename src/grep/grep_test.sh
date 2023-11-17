#!/bin/sh

GREP21=/tmp/grep21.txt
GREP=/tmp/grep.txt


while IFS= read -r line; do
    egrep $line > "$GREP" 2> /dev/null < /dev/null
    ./s21_grep $line > "$GREP21" 2> /dev/null < /dev/null
    if cmp -s "$GREP" "$GREP21"; then
        echo "PASSED: grep $line"
    else
        echo "FAILD: grep $line"
    fi
done <<END
123 test1.txt
-i shkola test1.txt
-v numbers test1.txt
-c 2 test1.txt
-l numbers test1.txt
-n char test1.txt
-h char test1.txt
-s char test1.txt
-o char test1.txt
-f pattern.txt test1.txt
-e numbers test1.txt
-echar test1.txt
-i char test1.txt test2.txt
-v numbers test1.txt test2.txt
-c nuMBers test1.txt test2.txt
-l number test1.txt test2.txt
-n number test1.txt test2.txt
-h number test1.txt test2.txt
-s number not_exists.txt test2.txt
-e number test1.txt test2.txt
-echar test1.txt test2.txt
-iv nUmber test1.txt test2.txt
-ic NUMb test1.txt test2.txt
-il nUm test1.txt test2.txt
-ivc nUm test1.txt test2.txt
-ice nUm test1.txt test2.txt
-vc number test1.txt test2.txt
-vl number test1.txt test2.txt
-vlo number test1.txt test2.txt
-cn number test1.txt test2.txt
-co number test1.txt test2.txt
-lo number test1.txt test2.txt
-nh number test1.txt test2.txt
number

-e number
number not_exists.txt
-e number not_exists.txt
-f not_exists.txt
-e 'shkola 42' test1.txt
-e
-f
-X
END