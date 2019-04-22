
DIR_SRC = $(shell ls -l | grep ^d | awk '{if($$9 != "obj") print $$9}')
SOURCES = $(wildcard ${SUBDIRS}/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

export CC FILE_TYPE CFLAGS MAKE_CFLAGS INC ROOT_DIR DIR_OBJ

all: $(DIR_SRC) 

$(DIR_SRC) : ECHO
	@make $(MAKE_CFLAGS) -C $@

ECHO:
	@echo "Src compile ..."