CLEANUP = rm -f
MKDIR = mkdir -p
TARGET_EXTENSION=.out

C_COMPILER=gcc
CFLAGS += -g -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

TARGET_BASE=project
TARGET = $(TARGET_BASE)$(TARGET_EXTENSION)
SRC_FILES := $(wildcard src/*.c)
SRC_TEST_FILES=$(filter-out src/main.c, $(SRC_FILES))
INC_DIRS=-Isrc
LDFLAGS= -lm

all: clean production

production: $(SRC_FILES)
	$(C_COMPILER) -o $(TARGET) $(SRC_FILES) $(CFLAGS) $(LDFLAGS) $(INC_DIRS)

unittest: $(SRC_TEST_FILES)
	$(C_COMPILER) -o $(TARGET) $(SRC_TEST_FILES) $(CFLAGS) $(INC_DIRS)
	- ./$(TARGET)
	
clean:
	$(CLEANUP) $(TARGET)

ci: CFLAGS += -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
ci: production