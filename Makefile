CC = gcc
CFLAGS = -Wall -Icore/include
LIBS = -lssl -lcrypto -lmysqlclient

# Corrected source file paths
SRC = core/src/main.c core/src/wipe.c core/src/algorithms.c core/src/hash_log.c database/db.c
TARGET = core/build/wipe_engine

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p core/build
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)