PROJECT = click-bait
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lkernel32 -luser32
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = $(BIN_DIR)\obj

SRCS = $(subst \,/,$(shell dir /s /b $(SRC_DIR)\*.c))
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))
TARGET = $(BIN_DIR)/$(PROJECT).exe

all: $(OBJ_DIR) $(TARGET)

$(OBJ_DIR):
	@if not exist $(BIN_DIR) @mkdir $(BIN_DIR)
	@if not exist $(OBJ_DIR) @mkdir $(OBJ_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o:
	$(CC) $(CFLAGS) -c $(filter %/$(notdir $*).c, $(SRCS)) -o $@
	
run: $(OBJ_DIR) $(TARGET)
	./$(TARGET)

clean:
	@if exist $(BIN_DIR) rd /s /q $(BIN_DIR)