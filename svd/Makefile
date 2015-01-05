.PHONY: clean

#LDFLAGS+=-larmadillo
#CPPFLAGS+=-larmadillo

TARGET=main

run:
	matlab -r "run('../graph.txt', '../training.txt', '../test_data/test_data_q1.txt', 'out1.txt');exit(0);" -nodesktop -nosplash 
	matlab -r "run('../graph.txt', '../training.txt', '../test_data/test_data_q2.txt', 'out2.txt');exit(0);" -nodesktop -nosplash
	matlab -r "run('../graph.txt', '../training.txt', '../test_data/test_data_q3.txt', 'out3.txt');exit(0);" -nodesktop -nosplash

test:
	python ../eval.py out1.txt ../test_data/test_data_a1.txt
	python ../eval.py out2.txt ../test_data/test_data_a2.txt
	python ../eval.py out3.txt ../test_data/test_data_a3.txt
all: main.cpp
	g++ main.cpp -larmadillo -o main

o3: CPPFLAGS+=-o3
o3: all

debug: CPPFLAGS+=-g
debug: all

# type make/make all to compile test_hmm


clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
