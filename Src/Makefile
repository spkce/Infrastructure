OBJS_PATH ?= ./obj
LIBS ?= -lpthread -lrt
INC ?= -I /mnt/d/source/Infrastructure/Include
TARGET ?= ./libInfra.a
CC ?= gcc
CXX ?= g++
AR ?= ar
CFLAGS ?=
MAKE_CFLAGS ?= --no-print-directory

INC_INTERNAL := -I/mnt/d/source/Infrastructure/Src/Log
INC_INTERNAL +=  $(INC)

SRC_DIR := $(shell ls -l | grep ^d | awk '{if($$9 != "obj") print $$9}')
SRC_DIR += ./
CPP_SRCS := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cpp))
CPP_OBJS :=  $(patsubst %.cpp, %.o, $(CPP_SRCS))
CPPOBJS := $(addprefix $(OBJS_PATH)/, $(CPP_OBJS))

C_SRCS := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
C_OBJS := $(patsubst %.c, %.o, $(C_SRCS))
COBJS := $(addprefix $(OBJS_PATH)/, $(C_OBJS))

export CC CXX AR CFLAGS MFLAGS

.PHONY: all ECHO

all: ECHO $(TARGET)

$(COBJS) : $(OBJS_PATH)/%.o: %.c
	@echo "CC $(notdir $@)"
	@mkdir -p $(dir $(@))
	@$(CC) -o $@ -c $< $(CFLAGS) $(INC_INTERNAL)
	
$(CPPOBJS) : $(OBJS_PATH)/%.o: %.cpp
	@echo "CXX $(notdir $@)"
	@mkdir -p $(dir $(@))
	@$(CXX) -o $@ -c $< $(CFLAGS) $(INC_INTERNAL)

$(TARGET) : $(COBJS) $(CPPOBJS)
	@$(AR) rcs $@ $^

ECHO:
	@echo "*************************************"
	@echo "TARGET="$(TARGET)
	@echo "CC="$(CC)
	@echo "CXX="$(CXX)
	@echo "AR="$(AR)
	@echo "OBJS_PATH="$(OBJS_PATH)
	@echo "LIBS="$(LIBS)
	@echo "INC="$(INC_INTERNAL)
	@echo "SRC_DIR="$(SRC_DIR)
	@echo "CPP_SRCS="$(CPP_SRCS)
	@echo "CPP_OBJS="$(CPP_OBJS)
	@echo "CPPOBJS="$(CPPOBJS)
	@echo "C_SRCS="$(C_SRCS)
	@echo "C_OBJS="$(C_OBJS)
	@echo "COBJS="$(COBJS)
	@echo "CFLAGS="$(CFLAGS)
	@echo "MAKE_CFLAGS="$(MAKE_CFLAGS)
	@echo "*************************************"