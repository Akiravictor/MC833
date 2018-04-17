CC=gcc

SRC_DIR := ./src
EXEC_CLIENT := ./bin/client.exec
EXEC_SERVER := ./bin/server.exec
INC_DIR := ./headers
OBJ_DIR := ./obj

CFLAGS=-c  -pedantic -Wall -m64
INC_FLAGS := -I$(INC_DIR)
LDFLAGS:=

SOURCES_CLIENT= $(shell find $(SRC_DIR) -name client*.c)
OBJECTS_CLIENT= $(SOURCES_CLIENT:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

SOURCES_SERVER= $(shell find $(SRC_DIR) -name server*.c)
OBJECTS_SERVER= $(SOURCES_SERVER:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

SOURCES_COMMON= $(shell find $(SRC_DIR) -name common*.c)
OBJECTS_COMMON= $(SOURCES_COMMON:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: client server
client: makedir $(EXEC_CLIENT) 
server: makedir $(EXEC_SERVER) 
run_server: server
	$(EXEC_SERVER)
run_client: client
	$(EXEC_CLIENT)
run: client server
	$(EXEC_SERVER)
	$(EXEC_CLIENT)

makedir:
	mkdir -p obj bin

$(EXEC_CLIENT): $(OBJECTS_CLIENT) $(OBJECTS_COMMON)
	$(CC) $(LDFLAGS) $(OBJECTS_CLIENT) $(OBJECTS_COMMON) -o $@

$(EXEC_SERVER): $(OBJECTS_SERVER) $(OBJECTS_COMMON)
	$(CC) $(LDFLAGS) $(OBJECTS_SERVER) $(OBJECTS_COMMON) -o $@

$(OBJECTS_CLIENT): $(OBJ_DIR)/client%.o : $(SRC_DIR)/client%.c
	@$(CC) $(INC_FLAGS) $(CFLAGS) $< -o $@

$(OBJECTS_SERVER): $(OBJ_DIR)/server%.o : $(SRC_DIR)/server%.c
	@$(CC) $(INC_FLAGS) $(CFLAGS) $< -o $@

$(OBJECTS_COMMON): $(OBJ_DIR)/common%.o : $(SRC_DIR)/common%.c
	@$(CC) $(INC_FLAGS) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXEC_CLIENT)  $(OBJECTS_CLIENT) $(EXEC_SERVER) $(OBJECTS_SERVER) $(OBJECTS_COMMON)
	find . -iname *~ -exec rm -rf {} \;
