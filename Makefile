
ROOT_DIR = $(shell pwd)
DIR_OBJ = ./obj
DIR_LIB = ./lib
INC = -I$(ROOT_DIR)/Include

OBJS_PATH ?= ./obj
LIBS ?= -L/mnt/d/source/Infrastructure/lib -lpthread -lrt
INC ?= -I /mnt/d/source/Infrastructure/Include
OUT_PATH ?= .
OUT ?= test.out
CC ?= gcc
CXX ?= g++
AR ?= ar
CFLAGS ?= -Wall --no-print-directory #-DINFRA_LOG

ROOT_DIR = $(shell pwd)
DIR_OBJ = ./obj
DIR_LIB = ./lib
INC = -I$(ROOT_DIR)/Include

#make -C xx ：路径不能使用./xx格式
#DIR_SRC += \
#Src


#export CC FILE_TYPE CFLAGS MAKE_CFLAGS INC INC_INTERNAL ROOT_DIR DIR_OBJ

all: CHECKDIR target

test: all ECHO_TEST

CHECKDIR:
	@mkdir -p $(DIR_OBJ)
	@mkdir -p $(DIR_LIB)

#生成静态库libInfra.a
target:
	@make "CFLAGS="$(CFLAGS) -C Src

#编译测试代码
test:
	@make "CFLAGS="$(CFLAGS) -C Test

clean:
	@rm -rf ${DIR_OBJ}
	@rm -rf ${DIR_LIB}
#ifeq必须顶格
ifeq ($(TEST), $(wildcard $(TEST)))
	@rm $(TEST)
endif