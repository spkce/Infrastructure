
object = link.o

SOURCES = $(wildcard *${FILE_TYPE})

all: ECHO $(object)

$(object) : $(SOURCES)
	@$(CC) -c $(CFLAGS) $^ -o $(ROOT_DIR)/$(DIR_OBJ)/$@ $(INC) $(INC_INTERNAL)

ECHO:
	@echo "link compile ..."