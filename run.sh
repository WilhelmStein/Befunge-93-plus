#!/bin/bash
make clean && cereal --rebuild

for filename in ./tests/*.bf; do
    test_output=${filename/.bf/.out}
    diff -y --text <(./bin/befunge93.exe $filename) <(./Befunge-93/bin/bef $filename) > ${test_output}
done