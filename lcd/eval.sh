#!/bin/bash
echo -e "\e[1;33m  a1.txt  \e[0m"
python ../eval_k_precision.py a1.txt ../test_data/test_data_a1.txt
echo -e "\e[1;33m  a2.txt  \e[0m"
python ../eval_k_precision.py a2.txt ../test_data/test_data_a2.txt
echo -e "\e[1;33m  a3.txt  \e[0m"
python ../eval_k_precision.py a3.txt ../test_data/test_data_a3.txt
