.PHONY: clean

LDLIBS+=-lm -lafcuda# link to math library

TARGET=main test generate

all: $(TARGET)
# type make/make all to compile test_hmm

o3: CPPFLAGS+=-o3
o3: all

debug: CPPFLAGS+=-g
debug: all

run: all
	./generate ../graph.txt model_init.txt
	./main model.txt ../training.txt 1000 1e-3 model_init.txt
	./test model.txt ../test_data/test_data_q1.txt a1.txt
	./test model.txt ../test_data/test_data_q2.txt a2.txt
	./test model.txt ../test_data/test_data_q3.txt a3.txt
	python ../eval_k_precision.py a1.txt ../test_data/test_data_a1.txt | tee result.txt
	python ../eval_k_precision.py a2.txt ../test_data/test_data_a2.txt | tee -a result.txt
	python ../eval_k_precision.py a3.txt ../test_data/test_data_a3.txt | tee -a result.txt

eval: all
	python ../eval_k_precision.py a1.txt ../test_data/test_data_a1.txt | tee result.txt
	python ../eval_k_precision.py a2.txt ../test_data/test_data_a2.txt | tee -a result.txt
	python ../eval_k_precision.py a3.txt ../test_data/test_data_a3.txt | tee -a result.txt

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
