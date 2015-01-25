.PHONY: clean

LDFLAGS+=-lm # link to math library
LIBMF=~/libmf-1.1/libmf

RAMDISK=tmp
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
	./MF $(LIBMF) $(RAMDISK)/graph.txt $(RAMDISK)/training.txt ../test_data/test_data_q1.txt results/a1.txt $(RAMDISK)
	./MF $(LIBMF) $(RAMDISK)/graph.txt $(RAMDISK)/training.txt ../test_data/test_data_q2.txt results/a2.txt $(RAMDISK)
	./MF $(LIBMF) $(RAMDISK)/graph.txt $(RAMDISK)/training.txt ../test_data/test_data_q3.txt results/a3.txt $(RAMDISK)

test: all
	./test.sh


clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
