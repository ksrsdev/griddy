CC = gcc
TARGET = test
CFLAGS = -std=c17 -O2 -march=native -pipe \
         -Wall -Wextra -Wpedantic -Werror \
         -Wshadow -Wstrict-prototypes -Wmissing-prototypes \
         -Wconversion -Wformat=2 -Wlogical-op \
         -Wnull-dereference -Wdouble-promotion \
         -fstack-protector-strong -D_FORTIFY_SOURCE=3 \
         $(shell pkg-config --cflags sdl3)
LDFLAGS = $(shell pkg-config --libs sdl3)

# 2. Project Files
# Folder Setup
SRCDIR = src
OBJDIR = obj

# Find all .c files in src/ and transform their names to .o in obj/
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# --- BUILD RULES ---

# Step 2: Link all .o files into the final program
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Step 1: Compile each .c file into a .o file
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the directory if it's missing
$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)
