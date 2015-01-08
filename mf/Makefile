.PHONY: clean

LDFLAGS+=-lm # link to math library
LIBMF=~/libmf-1.1/libmf

TARGET=MF

all: $(TARGET)

o3: CPPFLAGS+=-o3
o3: all

debug: CPPFLAGS+=-g
debug: all

# type make/make all to compile test_hmm
run: all
	./MF $(LIBMF) ../graph.txt ../training.txt ../test_data/test_data_q1.txt mf_a1.txt
	./MF $(LIBMF) ../graph.txt ../training.txt ../test_data/test_data_q2.txt mf_a2.txt
	./MF $(LIBMF) ../graph.txt ../training.txt ../test_data/test_data_q3.txt mf_a3.txt

test: all
	python ../eval_k_precision.py mf_a1.txt ../test_data/test_data_a1.txt | tee result.txt
	python ../eval_k_precision.py mf_a2.txt ../test_data/test_data_a2.txt | tee -a result.txt
	python ../eval_k_precision.py mf_a3.txt ../test_data/test_data_a3.txt | tee -a result.txt


clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
