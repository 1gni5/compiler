ANTLR4_LIB=/usr/lib/x86_64-linux-gnu
ANTLR4_INCLUDE=/usr/include/antlr4-runtime
ANTLR_DIR=generated
ANTR4_CMD=/usr/bin/antlr4

ANTLR_FILES = $(wildcard $(ANTLR_DIR)/*.cpp)
ANTLR_OBJS = $(patsubst $(ANTLR_DIR)/%.cpp,$(ANTLR_DIR)/%.o,$(ANTLR_FILES))
ANTLR_OPTS=-visitor -no-listener -Dlanguage=Cpp

CC=g++
CCFLAGS=-std=c++17 -Wno-attributes -I$(ANTLR4_INCLUDE) -I$(ANTLR_DIR) -g
LDFLAGS=-L$(ANTLR4_LIB) -lantlr4-runtime
EXEC=cc

OBJS=main.o x86_64Visitor.o

default: all
all: $(OBJS) $(ANTLR_OBJS)
	$(CC) $(CCFLAGS) -o $(EXEC) $(OBJS) $(ANTLR_OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $< -o $@

antlr:
	mkdir -p $(ANTLR_DIR)
	$(ANTR4_CMD) $(ANTLR_OPTS) -o $(ANTLR_DIR) cc.g4

clean:
	rm -rf $(ANTLR_DIR) $(OBJS) $(EXEC)