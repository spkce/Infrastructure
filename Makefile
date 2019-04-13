

CC = g++
AR = ar
FILE_TYPE = .cpp
CFLAGS = -Wall

INC = -I./Include

DIR_SRC += \
./Src/Packet \
./Src/Timer

TEST_DIR += \
./Test \
./Test/Packet 

DIR_OBJ = obj

SRC += $(foreach d, ${DIR_SRC}, $(wildcard ${d}/*${FILE_TYPE}))
TEST_SRC = $(foreach d, ${TEST_DIR}, $(wildcard ${d}/*${FILE_TYPE}))
SRC_FLIE = $(notdir ${SRC})
OBJ = $(patsubst %${FILE_TYPE},${DIR_OBJ}/%.o,${SRC_FLIE})
LIB_PATH = ./lib
OUT = ./lib/libInfra.a
TEST = ./test.out
LIBS = -L$(LIB_PATH) -lInfra


all:ECHO $(OBJ) $(OUT) $(TEST)


$(OBJ):$(SRC)
	@sudo mkdir -p $(DIR_OBJ)
	@$(CC) -c $(CFLAGS) $^ -o $@ $(INC)

$(OUT):$(OBJ)
	@sudo mkdir -p $(LIB_PATH)
	@$(AR) rcs $(OUT) $(OBJ)

$(TEST):$(TEST_SRC)
	$(CC) $^ -o $@ $(INC) $(LIBS)

ECHO:
	@echo $(DIR_SRC)
	@echo $(SRC)
	@echo $(SRC_FLIE)
	@echo $(OBJ)

clean:
	@sudo rm -rf ${DIR_OBJ}
	@sudo rm -rf ${LIB_PATH}
	@sudo rm $(TEST)