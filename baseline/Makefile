.PHONY: clean

LDFLAGS+=-lm # link to math library

TARGET=baseline

all: $(TARGET)

o3: CPPFLAGS+=-o3
o3: all

debug: CPPFLAGS+=-g
debug: all

# type make/make all to compile test_hmm
run: all
	./baseline ../graph.txt ../test_data/test_data_q1.txt a1.txt
	./baseline ../graph.txt ../test_data/test_data_q2.txt a2.txt
	./baseline ../graph.txt ../test_data/test_data_q3.txt a3.txt
	python ../eval.py a1.txt ../test_data/test_data_a1.txt
	python ../eval.py a2.txt ../test_data/test_data_a2.txt
	python ../eval.py a3.txt ../test_data/test_data_a3.txt

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
