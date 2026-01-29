CC = gcc
TARGET = RojasFilters
LDFLAGS = -Wl,-Map=$(TARGET).map 
CFLAGS = -Wall -Werror -g -O0
DUMP = objdump

SOURCES = RojasFilters.c BmpProcessor.c PixelProcessor.c

INCLUDES = -I.

OBJS = $(SOURCES:.c=.o)
DEPS = $(SOURCES:.c=.d)

%.o : %.c
	$(CC) -c $< $(CFLAGS) $(INCLUDES) -o $@

%.d : %.c
	$(CC) -MM $< $(CFLAGS) $(INCLUDES) -o $@

%.i : %.c
	$(CC) -E $< $(CFLAGS) $(INCLUDES) -o $@

%.asm : %.o
	$(DUMP) -S --disassemble $< > $@

.PHONY: compile-all
compile-all: build

.PHONY: build
build: $(TARGET).exe $(TARGET).out

$(TARGET).exe: $(OBJS) $(DEPS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@ 
	size $(TARGET).exe

$(TARGET).out: $(OBJS) $(DEPS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@ 
	size $(TARGET).out

.PHONY: clean
clean:
	rm -f *.o *.d *.i *.exe *.map *.out