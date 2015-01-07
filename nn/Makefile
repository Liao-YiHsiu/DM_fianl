.PHONY: clean

LDFLAGS+=-lm # link to math library

TARGET=nn_train nn_graph

all: $(TARGET)
# type make/make all to compile test_hmm

o3: CPPFLAGS+=-o3
o3: all

debug: CPPFLAGS+=-g
debug: all

model.txt:
	./nn_graph ../graph.txt model.txt

run: model.txt all
	./nn_train model.txt ../training.txt 100 1e-5 model.txt

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
