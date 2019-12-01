#!/bin/bash
make clean && make all

if [ $# -eq 0 ]
then
    for filename in ./tests/*.bf; do
        test_output=${filename/.bf/.out}
        echo "Testing file: "${filename}
        diff --text <(./Befunge-93/bin/bef $filename) <(./bin/befunge93.exe $filename) # > ${test_output}
    done
else
    diff -y -s --text <(./Befunge-93/bin/bef $1) <(./bin/befunge93.exe $1) # > ${test_output}
fi