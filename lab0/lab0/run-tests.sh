#!/bin/bash

program="lab0"
numbers="0 1 2 3 4 5"
testdir="tests"

g++ -Wall -Werror -std=c++17 -g *.cpp -o $program

for number in $numbers ; do
 
    echo "Running input" $number

    inputfile=$testdir/input$number.txt
    answerfile=$testdir/answer$number.txt
    outputfile=actual$number.txt

    ./$program $inputfile > $outputfile

    diff $answerfile $outputfile || echo "diff failed on test" $number

    rm $outputfile

done

rm $program

