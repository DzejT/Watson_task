CFLAGS:=-Wall
LDGLAGS:=-Wl,--copy-dt-needed-entries -liotp-as-device -lpaho-mqtt3as -largp -lubus
# LDGLAGS:=-L/home/dzej/Downloads/iot-c-master/build -liotp-as-device -L/home/dzej/Downloads/iot-c-master/paho.mqtt.c/build/output -lpaho-mqtt3as
SRCS:=$(wildcard *.c)
#SRCS:=daemon.c watson.c utils.c
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
 
