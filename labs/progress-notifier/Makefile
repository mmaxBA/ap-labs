CC       = gcc
OBJFILES = logger.o base64.o
TARGET   = base64

all: $(TARGET)

$(TARGET) : $(OBJFILES)
	$(CC) -o $(TARGET) $(OBJFILES)

clean:
	rm -f $(OBJFILES) $(TARGET) *~

include ../../common.mk
