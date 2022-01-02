CC=gcc
CFLAGS=-Wall -O2

TARGET=pd2cut

all: $(TARGET)
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $<
clean:
	rm -f *~ $(TARGET) core
