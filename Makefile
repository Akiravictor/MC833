CC=gcc

SRC_DIR := ./src
EXEC_CLIENT := ./bin/client.exec
EXEC_SERVER := ./bin/server.exec
EXEC_AUTO := ./bin/auto.exec
INC_DIR := ./headers
OBJ_DIR := ./obj

CFLAGS=-c  -pedantic -Wall -m64
INC_FLAGS := -I$(INC_DIR)
LDFLAGS:=

SOURCES_CLIENT= $(shell find $(SRC_DIR) -name client*.c)
OBJECTS_CLIENT= $(SOURCES_CLIENT:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

SOURCES_SERVER= $(shell find $(SRC_DIR) -name server*.c)
OBJECTS_SERVER= $(SOURCES_SERVER:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

SOURCES_AUTO= $(shell find $(SRC_DIR) -name auto*.c)
OBJECTS_AUTO= $(SOURCES_AUTO:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

SOURCES_COMMON= $(shell find $(SRC_DIR) -name common*.c)
OBJECTS_COMMON= $(SOURCES_COMMON:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: client server auto
client: makedir $(EXEC_CLIENT) 
server: makedir $(EXEC_SERVER)
auto: makedir $(EXEC_AUTO)
run_server: server
	$(EXEC_SERVER)
run_client: client
	$(EXEC_CLIENT)
run_auto: auto
	$(EXEC_AUTO)

makedir:
	mkdir -p obj bin

$(EXEC_AUTO): $(OBJECTS_AUTO) $(OBJECTS_COMMON)
	$(CC) $(LDFLAGS) $(OBJECTS_AUTO) $(OBJECTS_COMMON) -o $@

$(EXEC_CLIENT): $(OBJECTS_CLIENT) $(OBJECTS_COMMON)
	$(CC) $(LDFLAGS) $(OBJECTS_CLIENT) $(OBJECTS_COMMON) -o $@

$(EXEC_SERVER): $(OBJECTS_SERVER) $(OBJECTS_COMMON)
	$(CC) $(LDFLAGS) $(OBJECTS_SERVER) $(OBJECTS_COMMON) -o $@

$(OBJECTS_AUTO): $(OBJ_DIR)/auto%.o : $(SRC_DIR)/auto%.c
	@$(CC) $(INC_FLAGS) $(CFLAGS) $< -o $@

$(OBJECTS_CLIENT): $(OBJ_DIR)/client%.o : $(SRC_DIR)/client%.c
	@$(CC) $(INC_FLAGS) $(CFLAGS) $< -o $@

$(OBJECTS_SERVER): $(OBJ_DIR)/server%.o : $(SRC_DIR)/server%.c
	@$(CC) $(INC_FLAGS) $(CFLAGS) $< -o $@

$(OBJECTS_COMMON): $(OBJ_DIR)/common%.o : $(SRC_DIR)/common%.c
	@$(CC) $(INC_FLAGS) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXEC_CLIENT) $(OBJECTS_CLIENT) $(EXEC_SERVER) $(OBJECTS_SERVER) $(OBJECTS_COMMON) $(EXEC_AUTO) $(OBJECTS_AUTO)
	find . -iname *~ -exec rm -rf {} \;

kill:
	killall server.exec
