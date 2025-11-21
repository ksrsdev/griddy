# ──────────────────────────────
# raylib macOS Makefile (2025) – clean root, src/ only
# ──────────────────────────────

TARGET   = griddy
SRCDIR   = src
SOURCES  := $(wildcard $(SRCDIR)/*.c)
OBJECTS  := $(SOURCES:.c=.o)

# Compiler & flags
CC      = clang
CFLAGS  = -Wall -Wextra -std=c99 -I$(BREW_PREFIX)/include
LDFLAGS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -L$(BREW_PREFIX)/lib

# Auto-detect Apple Silicon vs Intel Homebrew path
ifeq ($(shell uname -m),arm64)
    BREW_PREFIX = /opt/homebrew
else
    BREW_PREFIX = /usr/local
endif

# Default target
all: $(TARGET)

# Link the final executable (and delete .o files right after)
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@rm -f $(OBJECTS)   # ← this keeps your root directory clean

# Compile .c → .o (objects go temporarily in src/, then get deleted)
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Convenience targets
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(SRCDIR)/*.o

.PHONY: all run clean
