CC=gcc
TARGET=mp1
SRCS=getinfo.c hop.c log.c reveal.c systemcommand.c execute.c getcommand.c main.c proclore.c seek.c

all:$(TARGET)

$(TARGET):$(SRCS)
	$(CC) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)