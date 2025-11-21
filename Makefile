# ──────────────────────────────
# raylib macOS Makefile (2025)
# Works on Intel + Apple Silicon
# ──────────────────────────────

TARGET   = griddy
SRC      = main.c
CFLAGS   = -Wall -Wextra -std=c99
LDFLAGS  = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# Auto-detect Apple Silicon vs Intel
ifeq ($(shell uname -m),arm64)
    BREW_PREFIX = /opt/homebrew
else
    BREW_PREFIX = /usr/local
endif

CFLAGS  += -I$(BREW_PREFIX)/include
LDFLAGS += -L$(BREW_PREFIX)/lib

# files to delete on clean
CLEAN_FILES = $(TARGET) *.o *~ .*.swp .*.swo .swp .swo

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
