#!/bin/sh

CAT21=/tmp/cat21.txt
CAT=/tmp/cat.txt

while IFS= read -r line; do
    cat $line > "$CAT" 2> /dev/null < /dev/null
    ./s21_cat $line > "$CAT21" 2> /dev/null < /dev/null
    if cmp -s "$CAT" "$CAT21"; then
        echo "PASSED: cat $line"
    else
        echo "FAILD: cat $line"
    fi
done <<END
test.txt
-b test.txt
-e test.txt
-n test.txt
-v test2.txt
-s test.txt
-t test.txt
-e test.txt test1.txt
-s test.txt test1.txt
-t test.txt test2.txt
-be test.txt
-bv test.txt
-bv test.txt
-nn test.txt
-benst test.txt
--number test1.txt
--number-nonblank test.txt
--squeeze-blank test.txt
-T test.txt
-E test.txt
not_exists.txt
test.txt not_exists.txt
END