CC=gcc

SRC_DIR := ./src
EXEC_CLIENT := ./bin/client
EXEC_SERVER := ./bin/server
INC_DIR := ./headers
OBJ_DIR := ./obj

CFLAGS=-c -Wall
INC_FLAGS := -I$(INC_DIR)
LDFLAGS=

SOURCES_CLIENT= $(shell find $(SRC_DIR) -name client*.c)
OBJECTS_CLIENT= $(SOURCES_CLIENT:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

SOURCES_SERVER= $(shell find $(SRC_DIR) -name server*.c)
OBJECTS_SERVER= $(SOURCES_SERVER:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: client server
client: $(EXEC_CLIENT)
server: $(EXEC_SERVER)

$(EXEC_CLIENT): $(OBJECTS_CLIENT)
	$(CC) $(LDFLAGS) $(OBJECTS_CLIENT) -o $@

$(EXEC_SERVER): $(OBJECTS_SERVER)
	$(CC) $(LDFLAGS) $(OBJECTS_SERVER) -o $@

$(OBJECTS_CLIENT): $(OBJ_DIR)/client%.o : $(SRC_DIR)/client%.c
	@$(CC) $(INC_FLAGS) $(CFLAGS) $< -o $@

$(OBJECTS_SERVER): $(OBJ_DIR)/server%.o : $(SRC_DIR)/server%.c
	@$(CC) $(INC_FLAGS) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXEC_CLIENT)  $(OBJECTS_CLIENT) $(EXEC_SERVER) $(OBJECTS_SERVER)
	find . -iname *~ -exec rm -rf {} \;
