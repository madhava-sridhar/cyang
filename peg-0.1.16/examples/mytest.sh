#!/bin/bash
../leg  yang.leg -o yang.leg.c
if [[ $? -eq 0 ]]; then
echo Generation done, compiling
    gcc yang.c
if [[ $? -eq 0 ]]; then
    echo Run test
    cat test.yang | ./a.out
    if [[ $? -eq 0 ]]; then
        echo OK
    else
        echo Fail
    fi
fi
fi
