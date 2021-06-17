
SOURCES = $(wildcard *${FILE_TYPE})
object = $(patsubst %${FILE_TYPE}, %.o, ${SOURCES})

all: ECHO $(object)

$(object) : %.o : %${FILE_TYPE}
	@echo "CC $(notdir $@)"
	@$(CC)  $(CFLAGS) $(INC) $(INC_INTERNAL) -lpthread -c $< -o $(ROOT_DIR)/$(DIR_OBJ)/$@

ECHO:
	@echo "log compile ..."