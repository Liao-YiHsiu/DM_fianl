.PHONY: clean

LDFLAGS+=-lm # link to math library

TARGET=nn_train nn_test

all: $(TARGET)
# type make/make all to compile test_hmm

o3: CPPFLAGS+=-o3
o3: all

debug: CPPFLAGS+=-g
debug: all

run: all

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
