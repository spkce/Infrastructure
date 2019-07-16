

CC = g++
AR = ar
FILE_TYPE = .cpp
CFLAGS = -Wall
MAKE_CFLAGS = --no-print-directory

ROOT_DIR = $(shell pwd)
DIR_OBJ = ./obj
DIR_LIB = ./lib
INC = -I$(ROOT_DIR)/Include

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

OUT = $(DIR_LIB)/libInfra.a
TEST = test.out
LIBS = -L$(DIR_LIB) -lInfra

OBJ = $(wildcard ${DIR_OBJ}/*.o)
TEST_SRC = $(foreach d, ${TEST_DIR}, $(wildcard ${d}/*${FILE_TYPE}))

export CC FILE_TYPE CFLAGS MAKE_CFLAGS INC ROOT_DIR DIR_OBJ

all: CHECKDIR $(DIR_SRC) $(OUT) End

test: ECHO_TEST $(TEST)

CHECKDIR:
	@sudo mkdir -p $(DIR_OBJ)
	@sudo mkdir -p $(DIR_LIB)

#递归编译各个模块
$(DIR_SRC) : ECHO
	@make $(MAKE_CFLAGS) -C $@

#生成静态库
$(OUT) : $(OBJ)
	@$(AR) rcs $@ $^

#编译测试代码
$(TEST) : $(TEST_SRC) 
	@$(CC) $^ -g -o $@ $(INC) $(LIBS) -lpthread

ECHO:
	@echo $(INC)
	@echo "Infrastructure compiles ..."

ECHO_TEST:
	@echo "Infrastructure test compiles ..."

End:
	@echo "compiles end "
	@echo "CFLAGS = " $(CFLAGS)

clean:
	@sudo rm -rf ${DIR_OBJ}
	@sudo rm -rf ${DIR_LIB}
	@sudo rm $(TEST)