import argparse
import os
import shutil

def copy_directory(src, dst):
    """
    Copies a directory from source (src) to destination (dst),
    and always overwrites the copied files if they already exist.

    Args:
        src (str): Source directory path
        dst (str): Destination directory path

    Returns:
        None
    """
    try:
        # Create destination directory if it does not exist
        os.makedirs(dst, exist_ok=True)
        
        # Copy the directory recursively and always overwrite
        shutil.rmtree(dst)  # Remove destination directory if it already exists
        shutil.copytree(src, dst)
        print(f"Directory copied from '{src}' to '{dst}' successfully.")
    except shutil.Error as e:
        print(f"Error: Failed to copy directory: {e}")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    # Create argument parser
    parser = argparse.ArgumentParser(description="Copy a directory from source to destination, with overwriting")
    parser.add_argument("src", type=str, help="Source directory path")
    parser.add_argument("dst", type=str, help="Destination directory path")
    args = parser.parse_args()

    # Call copy_directory function with command line arguments
    copy_directory(args.src, args.dst)
