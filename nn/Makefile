.PHONY: clean

LDFLAGS+=-lm # link to math library

TARGET=nn_train nn_graph nn_test

all: $(TARGET)
# type make/make all to compile test_hmm

o3: CPPFLAGS+=-o3
o3: all

debug: CPPFLAGS+=-pg
debug: all

model.txt:
	./nn_graph ../graph.txt model.txt

run: model.txt all
	./nn_train model.txt ../training.txt 1000 1e-3 model.txt

test: all
	./nn_test model.txt ../test_data/test_data_q1.txt a1.txt
	./nn_test model.txt ../test_data/test_data_q2.txt a2.txt
	./nn_test model.txt ../test_data/test_data_q3.txt a3.txt
	python ../eval.py a1.txt ../test_data/test_data_a1.txt | tee result.txt
	python ../eval.py a2.txt ../test_data/test_data_a2.txt | tee -a result.txt
	python ../eval.py a3.txt ../test_data/test_data_a3.txt | tee -a result.txt

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
