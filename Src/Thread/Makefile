
object = Thread.o

SOURCES = $(wildcard *${FILE_TYPE})

all: ECHO $(object)

$(object) : $(SOURCES)
	@$(CC) -c $(CFLAGS) -lpthred $^ -o $(ROOT_DIR)/$(DIR_OBJ)/$@ $(INC)

ECHO:
	@echo "Thread compile ..."