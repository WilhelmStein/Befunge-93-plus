
CC = g++
CCFLAGS = -Wall -Wextra -std=c++17 -O3

LIBS = 

PATH_SRC = ./src
PATH_INC = ./include
PATH_BIN = ./bin
PATH_TEST = ./main

.PHONY: all
all:
	mkdir -p $(PATH_BIN)
	@echo
	@echo "*** Compiling object files ***"
	@echo "***"
	make $(OBJS)
	make $(PATH_BIN)/befunge93.exe
	@echo "***"

.PHONY: clean
clean:
	@echo
	@echo "*** Purging binaries ***"
	@echo "***"
	rm -rvf $(PATH_BIN)
	@echo "***"


INTERPRETER_DEP = $(addprefix $(PATH_INC)/, interpreter.hpp opcodes.hpp) $(PATH_SRC)/interpreter.cpp


$(PATH_BIN)/interpreter.o: $(INTERPRETER_DEP)
	$(CC) -I $(PATH_INC) $(DEFINED) $(CCFLAGS) $(PATH_SRC)/interpreter.cpp -c -o $(PATH_BIN)/interpreter.o


OBJS = $(addprefix $(PATH_BIN)/,  interpreter.o)

$(PATH_BIN)/%.exe: $(PATH_TEST)/%.cpp $(OBJS)
	$(CC) -I $(PATH_INC) $(DEFINED) $(CCFLAGS) $< $(OBJS) $(LIBS) -o $@
