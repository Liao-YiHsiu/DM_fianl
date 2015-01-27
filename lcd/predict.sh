#!/bin/bash
cat ../test_data/test_data_q1.txt ../test_data/test_data_q2.txt ../test_data/test_data_q3.txt >cat_query
./naive_edge ../training.txt ../graph.txt cat_query result
split -l 10 result a
mv aaa a1.txt
mv aab a2.txt
mv aac a3.txt
rm result
rm cat_query


cat ../valid_data/valid_data_q1.txt ../valid_data/valid_data_q2.txt ../valid_data/valid_data_q3.txt >cat_query
./naive_edge ../training.txt ../graph.txt cat_query result
split -l 20 result a
mv aaa vad_a1.txt
mv aab vad_a2.txt
mv aac vad_a3.txt
rm result
rm cat_query
