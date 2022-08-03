CC=cc
CFLAGS=-I. # -g3 -gdwarf-2 -DDEBUG
LIBS=

BUILD_DIR=build
SRC_DIR=src

_DEPS = prim.h builtin.h parse.h print.h
DEPS = $(patsubst %,$(SRC_DIR)/%,$(_DEPS))

_OBJ = karasu.o prim.o builtin.o parse.o print.o
OBJ = $(patsubst %,$(BUILD_DIR)/%,$(_OBJ))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

karasu: $(OBJ)
	${CC} -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -r build
