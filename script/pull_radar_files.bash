#!/bin/bash

# === CONFIGURATION ===
REMOTE_USER="radarop"
REMOTE_HOST="nws4.rap.ucar.edu"
REMOTE_DIR="/usr/iris_data/product_raw"
PREFIX_LIST=("$R1_NAME" "$R2_NAME")
LOCAL_BASE_DIR="$INPUT_RADAR_DIR"

# === TEST CONFIGURATION ===
#REMOTE_USER="ldm"
#REMOTE_HOST="nws4.rap.ucar.edu"
#REMOTE_DIR="/d1/ldm/data/nexrad"

# === MAIN LOGIC ===
for PREFIX in "${PREFIX_LIST[@]}"; do
    echo "Checking files for prefix: $PREFIX"

    # Remote find command to get recently modified files
    ssh "${REMOTE_USER}@${REMOTE_HOST}" "find '${REMOTE_DIR}' -maxdepth 2 -type f -name '${PREFIX}*' -mmin -1 -printf '%p|%s\n'" |
    while IFS='|' read -r REMOTE_FILE REMOTE_SIZE; do
        FILE_NAME=$(basename "$REMOTE_FILE")
        LOCAL_DIR="${LOCAL_BASE_DIR}/${PREFIX}"
        LOCAL_FILE="${LOCAL_DIR}/${FILE_NAME}"

        # Create local directory if it doesn't exist
        mkdir -p "$LOCAL_DIR"

        # Check if the file already exists and has the same size
        if [[ -f "$LOCAL_FILE" ]]; then
            LOCAL_SIZE=$(stat -c%s "$LOCAL_FILE")
            if [[ "$LOCAL_SIZE" -eq "$REMOTE_SIZE" ]]; then
                echo "File already exists and size matches: $LOCAL_FILE — Skipping"
                continue
            else
                echo "File exists but size mismatch: $LOCAL_FILE — Re-downloading"
            fi
        else
            echo "Downloading new file: $FILE_NAME"
        fi

        # SCP the file from remote to local
        scp "${REMOTE_USER}@${REMOTE_HOST}:${REMOTE_FILE}" "$LOCAL_FILE"
        sleep 2
    done
done
