#!/bin/bash

# Define file name and size
FILENAME="huge_file.bin"
FILESIZE_MB=2000

# Convert MB to bytes
FILESIZE_BYTES=$((FILESIZE_MB * 1024 * 1024))

# Generate the file
dd if=/dev/urandom of=$FILENAME bs=1M count=$FILESIZE_MB iflag=fullblock

# Confirm creation
echo "File '$FILENAME' of size $FILESIZE_MB MB created successfully."
