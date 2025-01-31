#!/bin/bash

INSTALL_DIR="$HOME/.local/bin"

mkdir -p "$INSTALL_DIR"

echo "Compiling the program..."
gcc -g -o gdpoise src/gdpoise.c

if [[ $? -ne 0 ]]; then
  echo "Compilation failed!"
  exit 1
fi

echo "Installing the program..."
mv gdpoise "$INSTALL_DIR/"

if ! echo "$PATH" | grep -q "$INSTALL_DIR"; then
  echo "Adding $INSTALL_DIR to PATH..."
  echo 'export PATH="$HOME/.local/bin:$PATH"' >> "$HOME/.zshrc"
  source "$HOME/.zshrc"
fi

echo "Installation complete!"
echo "You can now run the program with: gdpoise"

