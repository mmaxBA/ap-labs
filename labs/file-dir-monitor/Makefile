CC       = gcc
OBJFILES = logger.o monitor.o
TARGET   = monitor

all: $(TARGET)

$(TARGET) : $(OBJFILES)
	$(CC) -o $(TARGET) $(OBJFILES)

clean:
	rm -f $(OBJFILES) $(TARGET) *~
include ../../common.mk
