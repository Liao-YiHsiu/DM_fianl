TARGET=baseline nn mf svm

.PHONY: $(TARGET) 

all: $(TARGET)

$(TARGET):
	cd $@; make
