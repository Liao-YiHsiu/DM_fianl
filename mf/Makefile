.PHONY: clean

LDFLAGS+=-lm # link to math library
LIBMF=~/libmf-1.1/libmf

RAMDISK=/tmp/ramdisk
TARGET=MF
RESULT=a1.txt a2.txt a3.txt

all: $(TARGET)

o3: CPPFLAGS+=-o3
o3: all

debug: CPPFLAGS+=-pg
debug: all

# type make/make all to compile test_hmm
run: all
	cp ../graph.txt $(RAMDISK)
	cp ../training.txt $(RAMDISK)
	./MF $(LIBMF) $(RAMDISK)/graph.txt $(RAMDISK)/training.txt ../test_data/test_data_q1.txt a1.txt $(RAMDISK)
	./MF $(LIBMF) $(RAMDISK)/graph.txt $(RAMDISK)/training.txt ../test_data/test_data_q2.txt a2.txt $(RAMDISK)
	./MF $(LIBMF) $(RAMDISK)/graph.txt $(RAMDISK)/training.txt ../test_data/test_data_q3.txt a3.txt $(RAMDISK)

test: all
	python ../eval_k_precision.py a1.txt ../test_data/test_data_a1.txt | tee result.txt
	python ../eval_k_precision.py a2.txt ../test_data/test_data_a2.txt | tee -a result.txt
	python ../eval_k_precision.py a3.txt ../test_data/test_data_a3.txt | tee -a result.txt


clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
