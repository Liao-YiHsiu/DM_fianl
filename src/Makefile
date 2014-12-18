.PHONY: clean

CPPFLAGS+=-g
LDFLAGS+=-lm     # link to math library
LIBMF=~/libmf-1.1/libmf

TARGET=baseline nn_train nn_test svm_data preMF

all: $(TARGET)
# type make/make all to compile test_hmm
run: all
	./baseline ../graph.txt ../test_data/test_data_q1.txt a1.txt
	./baseline ../graph.txt ../test_data/test_data_q2.txt a2.txt
	./baseline ../graph.txt ../test_data/test_data_q3.txt a3.txt
	python eval.py a1.txt ../test_data/test_data_a1.txt
	python eval.py a2.txt ../test_data/test_data_a2.txt
	python eval.py a3.txt ../test_data/test_data_a3.txt

mf: all
	./preMF tr ../training.txt mf.train
	./preMF te ../graph.txt ../test_data/test_data_q1.txt mf.test1
	./preMF te ../graph.txt ../test_data/test_data_q2.txt mf.test2
	./preMF te ../graph.txt ../test_data/test_data_q3.txt mf.test3
	$(LIBMF) convert mf.train mf.train.bin
	$(LIBMF) convert mf.test1  mf.test1.bin 
	$(LIBMF) convert mf.test2  mf.test2.bin 
	$(LIBMF) convert mf.test3  mf.test3.bin 
	$(LIBMF) train --tr-rmse --obj -k 20 -t 1000 -s 4 -p 0.05 -q 0.05 -g 0.003 -ub -1 -ib -1 --no-use-avg --rand-shuffle mf.train.bin mf_model
	$(LIBMF) predict mf.test1.bin mf_model mf.test1.out 
	$(LIBMF) predict mf.test2.bin mf_model mf.test2.out 
	$(LIBMF) predict mf.test3.bin mf_model mf.test3.out 
	./preMF ev ../test_data/test_data_q1.txt mf.test1.out 100 mf_a1.txt
	./preMF ev ../test_data/test_data_q2.txt mf.test1.out 100 mf_a2.txt
	./preMF ev ../test_data/test_data_q3.txt mf.test1.out 100 mf_a3.txt
	python eval.py mf_a1.txt ../test_data/test_data_a1.txt
	python eval.py mf_a2.txt ../test_data/test_data_a2.txt
	python eval.py mf_a3.txt ../test_data/test_data_a3.txt


clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
