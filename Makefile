#compiler
CC = gcc
#binary name
TARGET = griddy
# Folder Setup
SRCDIR = src
INCLUDE_DIRS := include
OBJDIR = build
#compiler flags
CFLAGS = -std=c17 -O2 -march=native -pipe \
         -Wall -Wextra -Wpedantic -Werror \
         -Wshadow -Wstrict-prototypes -Wmissing-prototypes \
         -Wconversion -Wformat=2 -Wlogical-op \
         -Wnull-dereference -Wdouble-promotion \
         -fstack-protector-strong -D_FORTIFY_SOURCE=3 \
		 -MMD -MP \
		 -I$(INCLUDE_DIRS) \
         $(shell pkg-config --cflags sdl3 sdl3-ttf sdl3-image sdl3-shadercross)
#linker flags
LDFLAGS = $(shell pkg-config --libs sdl3 sdl3-ttf sdl3-image sdl3-shadercross) -lm

# Find all .c files in src/ and transform their names to .o in obj/
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
#make all of the new .o files .d files
DEPS = $(OBJECTS:.o=.d)

# --- BUILD RULES ---

# Step 1: Compile each .c file into a .o file
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Step 2: Link all .o files into the final program
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Create the directory if it's missing
$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)

#Read the .d files if they exist
-include $(DEPS)

run: $(TARGET)
	./$(TARGET)
