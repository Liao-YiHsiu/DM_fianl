.PHONY: clean

LDFLAGS+=-lm # link to math library

TARGET=nn_train nn_graph nn_test
MODEL=model4.txt

all: $(TARGET)
# type make/make all to compile test_hmm

o3: CPPFLAGS+=-o3
o3: all

debug: CPPFLAGS+=-pg
debug: all

$(MODEL):
	./nn_graph ../graph.txt $(MODEL)

run: $(MODEL) all
	./nn_train $(MODEL) ../training.txt 1000 1 $(MODEL)

test: all
	./nn_test $(MODEL) ../test_data/test_data_q1.txt a1.txt
	./nn_test $(MODEL) ../test_data/test_data_q2.txt a2.txt
	./nn_test $(MODEL) ../test_data/test_data_q3.txt a3.txt
	python ../eval_k_precision.py a1.txt ../test_data/test_data_a1.txt | tee result.txt
	python ../eval_k_precision.py a2.txt ../test_data/test_data_a2.txt | tee -a result.txt
	python ../eval_k_precision.py a3.txt ../test_data/test_data_a3.txt | tee -a result.txt

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
