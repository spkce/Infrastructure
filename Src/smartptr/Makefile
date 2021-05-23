

SOURCES = $(wildcard *${FILE_TYPE})
object = $(patsubst %${FILE_TYPE}, %, ${SOURCES})

all: ECHO $(object)

% : %${FILE_TYPE}
	@$(CC) -c $(CFLAGS) -lpthread $^ -o $(ROOT_DIR)/$(DIR_OBJ)/$@ $(INC) $(INC_INTERNAL)

ECHO:
	@echo "smartptr compile ..."