# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = 

# Directories
SRCDIR = .
INCDIR = .
OBJDIR = obj

# Target executable name
TARGET = clox

# Source files (automatically find all .c files)
SOURCES = $(wildcard $(SRCDIR)/*.c)

# Object files (convert .c to .o in obj directory)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Header files (for dependency tracking)
HEADERS = $(wildcard $(INCDIR)/*.h)

# Default target
all: $(TARGET)

# Create target executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Create obj directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean build files
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Clean and rebuild
rebuild: clean all

# Install (optional - adjust path as needed)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall (optional)
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the program (default)"
	@echo "  clean    - Remove build files"
	@echo "  rebuild  - Clean and build"
	@echo "  install  - Install to /usr/local/bin"
	@echo "  help     - Show this help"

# Declare phony targets
.PHONY: all clean rebuild install uninstall help