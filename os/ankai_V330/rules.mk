
CROSS_PREFIX := arm-anykav500-eabi-
CROSS_PATH      ?= /opt/arm-anykav500-eabi
CC           = $(CROSS_PREFIX)gcc
CPP          = $(CROSS_PREFIX)g++
SIZE         = $(CROSS_PREFIX)size
STRIP        = $(CROSS_PREFIX)strip
AS           = $(CC)
AR           = $(CROSS_PREFIX)ar
LD           = $(CROSS_PREFIX)ld
RM           = rm -rf
MKDIR        = mkdir

CFLAGS		:= -Wall -O2 -std=c99 -mlittle-endian -fno-builtin  -fno-short-enums -nostdlib -mlong-calls $(DEFINE) $(INCLUDE)
ASFLAGS += -mlittle-endian -x assembler-with-cpp -O2 

# Rules

# --------------------------- s -> o
$(BUILDPATH)/%.o:%.s
	@echo ---------------------[build $<]----------------------------------
	$(CC) -c $(ASFLAGS) $(CFLAGS) -o $@ $<

# ----------------------------- c -> o
$(BUILDPATH)/%.o:%.c
	@echo ---------------------[build $<]----------------------------------
	$(CC) -c $(CFLAGS) -o $@ $<

