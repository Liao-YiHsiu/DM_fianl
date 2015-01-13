#!/bin/bash

rm result.txt
ls results/ | sort -h >list
file=list
while read line
do
    if [[ $line = *"a1.txt"* ]]
    then
        echo -e "\033[1;31m  $line \033[0m" | tee -a result.txt
        python ../eval_k_precision.py results/$line ../test_data/test_data_a1.txt | tee -a result.txt
    fi
    if [[ $line = *"a2.txt"* ]]
    then
        echo -e "\033[1;31m  $line \033[0m" | tee -a result.txt
        python ../eval_k_precision.py results/$line ../test_data/test_data_a2.txt | tee -a result.txt
    fi
    if [[ $line = *"a3.txt"* ]]
    then
        echo -e "\033[1;31m  $line \033[0m" | tee -a result.txt
        python ../eval_k_precision.py results/$line ../test_data/test_data_a3.txt | tee -a result.txt
    fi
done <$file

rm -f list
