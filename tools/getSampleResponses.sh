#!/bin/bash

# URL to request
URL="http://test.brightsign.io:3000"

# Default values
LOOP_COUNT=${1:-50}
OUTPUT_DIR=${2:-"."}

# Ensure output directory exists
mkdir -p "$OUTPUT_DIR"

# Loop through and make requests
for ((i=1; i<=LOOP_COUNT; i++))
do
    OUTPUT_FILE="$OUTPUT_DIR/fakeRestData-$i"
    echo "Fetching data... iteration $i"
    curl -s "$URL" -o "$OUTPUT_FILE"
    echo "Saved response to $OUTPUT_FILE"
done

echo "All requests completed."
