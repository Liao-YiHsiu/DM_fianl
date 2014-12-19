.PHONY: clean

CPPFLAGS+=-g
LDFLAGS+=-lm # link to math library
LIBMF=~/libmf-1.1/libmf

TARGET=baseline nn_train nn_test svm_data preMF MF

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
	./MF $(LIBMF) ../graph.txt ../training.txt ../test_data/test_data_q1.txt mf_a1.txt
	./MF $(LIBMF) ../graph.txt ../training.txt ../test_data/test_data_q2.txt mf_a2.txt
	./MF $(LIBMF) ../graph.txt ../training.txt ../test_data/test_data_q3.txt mf_a3.txt
	python eval.py mf_a1.txt ../test_data/test_data_a1.txt | tee result.txt
	python eval.py mf_a2.txt ../test_data/test_data_a2.txt | tee -a result.txt
	python eval.py mf_a3.txt ../test_data/test_data_a3.txt | tee -a result.txt


clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
