CFLAGS:=-Wall
LDGLAGS:=-Wl,--copy-dt-needed-entries -liotp-as-device -lpaho-mqtt3as -largp -lubus
SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:.c=.o)
TARGET_EXEC:=daemon

.PHONY: all clean

all: $(TARGET_EXEC)

$(TARGET_EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDGLAGS) 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm -f $(TARGET_EXEC) $(OBJS)
 
