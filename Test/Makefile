

OBJS_PATH ?= ../obj/test
LIBS ?= -L ../lib -lpthread -lrt -lInfra
INC ?= -I ../Include
CC ?= gcc
CXX ?= g++
AR ?= ar

TARGET ?= test.out

SRC_DIR := $(shell ls -l | grep ^d | awk '{if($$9 != "obj") print $$9}')
SRC_DIR += ./
CPP_SRCS := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cpp))
CPP_OBJS :=  $(patsubst %.cpp, %.o, $(CPP_SRCS))
CPPOBJS := $(addprefix $(OBJS_PATH)/, $(CPP_OBJS))

C_SRCS := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
C_OBJS := $(patsubst %.c, %.o, $(C_SRCS))
COBJS := $(addprefix $(OBJS_PATH)/, $(C_OBJS))

export CC CXX AR CFLAGS MAKE_CFLAGS

.PHONY: all ECHO

all: ECHO $(TARGET)

$(COBJS) : $(OBJS_PATH)/%.o: %.c
	@echo "CC $(notdir $@)"
	@mkdir -p $(dir $(@))
	@$(CC) -o $@ -c $< $(CFLAGS) $(INC)
	
$(CPPOBJS) : $(OBJS_PATH)/%.o: %.cpp
	@echo "CXX $(notdir $@)"
	@mkdir -p $(dir $(@))
	@$(CXX) -o $@ -c $< $(CFLAGS) $(INC)

$(TARGET) : $(COBJS) $(CPPOBJS)
	@$(CXX)  $(COBJS) $(CPPOBJS) -lrt -lInfra -g -o $@ $(INC) $(LIBS)

ECHO:
	@echo "*************************************"
	@echo "TARGET="$(TARGET)
	@echo "OBJS_PATH="$(OBJS_PATH)
	@echo "LIBS="$(LIBS)
	@echo "CC="$(CC)
	@echo "CXX="$(CXX)
	@echo "AR="$(AR)
	@echo "INC="$(INC)
	@echo "SRC_DIR="$(SRC_DIR)
	@echo "CPP_SRCS="$(CPP_SRCS)
	@echo "CPP_OBJS="$(CPP_OBJS)
	@echo "CPPOBJS="$(CPPOBJS)
	@echo "C_SRCS="$(C_SRCS)
	@echo "C_OBJS="$(C_OBJS)
	@echo "COBJS="$(COBJS)
	@echo "*************************************"