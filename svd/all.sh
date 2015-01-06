#!/bin/bash
arr=(1 2 5 10 15 20 25 30 40 50 100 150 200)

for i in ${arr[@]}; do
   matlab -r "run('../graph.txt', '../training.txt', '../test_data/test_data_q1.txt', 'out1.txt', $i);exit(0);" -nodesktop -nosplash 
   matlab -r "run('../graph.txt', '../training.txt', '../test_data/test_data_q2.txt', 'out2.txt', $i);exit(0);" -nodesktop -nosplash
   matlab -r "run('../graph.txt', '../training.txt', '../test_data/test_data_q3.txt', 'out3.txt', $i);exit(0);" -nodesktop -nosplash
   
   python ../eval.py out1.txt ../test_data/test_data_a1.txt | tee result.txt
   python ../eval.py out2.txt ../test_data/test_data_a2.txt | tee -a result.txt
   python ../eval.py out3.txt ../test_data/test_data_a3.txt | tee -a result.txt
   
   git add out* result.txt
   git commit -m "N = $i"$'\n'"`cat result`"
done
