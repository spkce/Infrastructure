

CC = g++
AR = ar
FILE_TYPE = .cpp
CFLAGS = -Wall -DINFRA_LOG 
MAKE_CFLAGS = --no-print-directory

ROOT_DIR = $(shell pwd)
DIR_OBJ = ./obj
DIR_LIB = ./lib
INC = -I$(ROOT_DIR)/Include

INC_INTERNAL = -I$(ROOT_DIR)/Src/Log

#make -C xx ：路径不能使用./xx格式
DIR_SRC += \
Src

TEST_DIR += \
./Test \
./Test/Packet \
./Test/Log \
./Test/Thread \
./Test/Link \
./Test/Callback \
./Test/Timer \
./Test/MsgQueue \

OUT = $(DIR_LIB)/libInfra.a
TEST = test.out
LIBS = -L$(DIR_LIB) -lInfra -lpthread -lrt

OBJ = $(wildcard ${DIR_OBJ}/*.o)
TEST_SRC = $(foreach d, ${TEST_DIR}, $(wildcard ${d}/*${FILE_TYPE}))

export CC FILE_TYPE CFLAGS MAKE_CFLAGS INC INC_INTERNAL ROOT_DIR DIR_OBJ

all: CHECKDIR $(OUT) End

test: all ECHO_TEST $(TEST)

CHECKDIR:
	@mkdir -p $(DIR_OBJ)
	@mkdir -p $(DIR_LIB)

#递归编译各个模块
$(DIR_SRC) : ECHO
	@make $(MAKE_CFLAGS) -C $@

#生成静态库
$(OUT) : $(DIR_SRC)
	@$(AR) rcs $@ $(OBJ)

#编译测试代码
$(TEST) : $(TEST_SRC) 
	@$(CC) $^ -g -o $@ $(INC) $(LIBS) 

ECHO:
	@echo $(INC)
	@echo "Infrastructure compiles ..."

ECHO_TEST:
	@echo $(TEST_SRC) $(LIBS) $(INC)
	@echo "Infrastructure test compiles ..."

End:
	@echo "compiles end "
	@echo "CFLAGS = " $(CFLAGS)

clean:
	@rm -rf ${DIR_OBJ}
	@rm -rf ${DIR_LIB}
#ifeq必须顶格
ifeq ($(TEST), $(wildcard $(TEST)))
	@rm $(TEST)
endif