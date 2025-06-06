#!/usr/bin/env python3

import os
import shutil
import argparse

# 🔑 Define keywords to search for in the file name
KEYWORDS = ["KFTG", "KCYS"]  # Edit this list as needed

def move_file_if_contains_keywords(file_path, base_output_dir):
    file_name = os.path.basename(file_path)

    for keyword in KEYWORDS:
        if keyword in file_name:
            try:
                # Construct the new directory path with the keyword appended
                keyword_output_dir = os.path.join(base_output_dir, keyword)
                os.makedirs(keyword_output_dir, exist_ok=True)

                destination = os.path.join(keyword_output_dir, file_name)
                shutil.move(file_path, destination)
                print(f"Moved '{file_name}' to '{keyword_output_dir}'")
                return  # Exit after the first match
            except Exception as e:
                print(f"Error moving file: {e}")
                return

    print(f"No keywords found in file name: {file_name}")

def main():
    parser = argparse.ArgumentParser(description="Move a file if its name contains specified keywords.")
    parser.add_argument("file_path", help="Path to the file to be checked and moved.")
    parser.add_argument("file_modify_time", help="An argument from InputWatcher that is not used.")

    args = parser.parse_args()
    output_dir = os.environ['INPUT_RADAR_DIR']

    move_file_if_contains_keywords(args.file_path, output_dir)

if __name__ == "__main__":
    main()
