#!/bin/bash
make clean && make all

if [ $# -eq 0 ]
then

    echo "Do you want to use the (B)asic Tests, the (G)arbage Collector Tests, or the (U)ser Input Tests?: "
    read TEST_FOLDER
    echo $TEST_FOLDER

    if [[ "$TEST_FOLDER" == "b" || "$TEST_FOLDER" == "B" ]]; then
        TEST_FOLDER="basic_tests"
    elif [[ "$TEST_FOLDER" == "g" || "$TEST_FOLDER" == "G" ]]; then
        TEST_FOLDER="garbage_collector_tests"
    elif [[ "$TEST_FOLDER" == "u" || "$TEST_FOLDER" == "U" ]]; then
        TEST_FOLDER="user_input_tests"
    else
        echo "Invalid Command."
        exit
    fi


    for filename in ./tests/$TEST_FOLDER/*.bf; do
        test_output=${filename/.bf/.out}
        echo "Testing file: "${filename}
        diff --text <(./Befunge-93/bin/bef $filename) <(./bin/befunge93.exe $filename) # > ${test_output}
    done
else
    diff -y -s --text <(./Befunge-93/bin/bef $1) <(./bin/befunge93.exe $1) # > ${test_output}
fi