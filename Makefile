CC := g++
BIN := bin
SRCF := src
SRCS := $(wildcard src/*.cpp)
HDRS := $(wildcard src/*.h)
OBJ := ${SRCS:.cpp=.o}
OUT := $(BIN)/nlp_exercise
DATA := data

all: $(OUT)
	@echo "--------------"
	@echo "done"

$(OUT): $(OBJ) $(BIN)
	mv $(SRCF)/*.o $(BIN)
	$(CC) -o $@ $(BIN)/*.o


$(BIN):
	if ! -e $(BIN); then \
		mkdir $(BIN); \
	fi

run:
	./$(OUT)
	

clean:
	if test -e $(BIN); then \
		rm -rf $(BIN); \
	fi