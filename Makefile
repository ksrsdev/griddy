# ──────────────────────────────
# raylib macOS Makefile (2025) – clean root, src/ only
# ──────────────────────────────

TARGET   = griddy
SRCDIR   = src
SOURCES  := $(wildcard $(SRCDIR)/*.c)
OBJECTS  := $(SOURCES:.c=.o)

# Compiler & flags
CC = gcc
INCLUDE_DIRS := include
CFLAGS  = -Wall -Wextra -std=c99 -I$(BREW_PREFIX)/include $(addprefix -I,$(INCLUDE_DIRS))
#CFLAGS = -std=c17 -pedantic-errors -Wall -Wextra -Wshadow -Wstrict-prototypes -Wmissing-prototypes -Wformat=2 -Wconversion -Wswitch-enum -Wlogical-op -Wnull-dereference -Wdouble-promotion -D_FORTIFY_SOURCE=3 -fstack-protector-strong -O2 -march=native -pipe -I$(BREW_PREFIX)/include $(addprefix -I,$(INCLUDE_DIRS))

# Auto-detect platform and set correct flags
ifeq ($(shell uname),Darwin)    # macOS
    ifeq ($(shell uname -m),arm64)
        BREW_PREFIX = /opt/homebrew
    else
        BREW_PREFIX = /usr/local
    endif
    LDFLAGS = -lraylib \
              -framework OpenGL -framework Cocoa -framework IOKit \
              -framework CoreVideo -framework CoreFoundation \
              -L$(BREW_PREFIX)/lib
else                            # Linux
    BREW_PREFIX = /usr/local    # change if you use a different prefix on Linux
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 \
              -L$(BREW_PREFIX)/lib
endif

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
