
ROOT_DIR = $(shell pwd)
DIR_OBJ ?= $(ROOT_DIR)/obj
DIR_LIB ?= $(ROOT_DIR)/lib
TARGET = $(DIR_LIB)/libInfra.a
TEST = $(ROOT_DIR)/test.out

INC ?= -I$(ROOT_DIR)/Include
LIBS ?= -lpthread -lrt
CC ?= gcc
CXX ?= g++
AR ?= ar
CFLAGS ?= -Wall -DINFRA_LOG -g
MAKE_CFLAGS ?= --no-print-directory

export CC CXX AR CFLAGS MAKE_CFLAGS

.PHONY: all checkdir target test clean 

all: checkdir target

test: all

checkdir:
	@mkdir -p $(DIR_OBJ)
	@mkdir -p $(DIR_LIB)

#生成静态库libInfra.a
target:
	@make $(MAKE_CFLAGS) OBJS_PATH="$(DIR_OBJ)" TARGET="$(TARGET)" LIBS="$(LIBS)" -C Src

#编译测试代码 # LIBS="-L$(DIR_LIB) $(LIBS) -lInfra"
test:
	@make $(MFLAGS) OBJS_PATH="$(DIR_OBJ)/test" TARGET"=$(TEST)" -C Test

clean:
	@rm -rf $(DIR_OBJ)
	@rm -rf $(DIR_LIB)
#ifeq必须顶格
ifeq ($(TEST), $(wildcard $(TEST)))
	@rm $(TEST)
endif