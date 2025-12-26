# Extended Features for fatfs-python

This document describes the extended directory traversal features added to fatfs-python.

## Overview

The extended features add full directory traversal support, including:
- `walk()` - os.walk()-like directory traversal
- `listdir()` - List directory contents
- `stat()` - Get file/directory information
- `remove()` / `rmdir()` - Delete files and directories
- `rename()` - Rename/move files
- `exists()` / `isfile()` / `isdir()` - Path checking
- `read_file()` / `write_file()` - Convenient file I/O
- `copy_tree_from()` / `copy_tree_to()` - Bulk copy operations

## Integration Steps

### 1. Add Extended Wrapper Functions

Add the contents of `wrapper_extended.pyx` to the existing `fatfs/wrapper.pyx` file.

Key additions:
- `DIR_Handle` class - Directory handle wrapper
- `FILINFO_Handle` class - File info wrapper with helper methods
- `pyf_opendir()`, `pyf_closedir()`, `pyf_readdir()` - Directory operations
- `pyf_stat()`, `pyf_unlink()`, `pyf_rename()` - File operations
- `pyf_read()`, `pyf_lseek()`, `pyf_sync()`, `pyf_truncate()` - File I/O

### 2. Update __init__.py

Add the extended partition class to the module exports:

```python
from fatfs.wrapper import *
from fatfs.diskio import RamDisk
from fatfs.partition_extended import PartitionExtended, create_extended_partition

__all__ = ["wrapper", "diskio", "PartitionExtended", "create_extended_partition"]
```

### 3. Rebuild the Package

```bash
cd fatfs-python

# Clean old builds
rm -rf build/ dist/ *.egg-info

# Set environment variable to trigger Cython compilation
export CYTHONIZE=1

# Build
python3 -m build

# Install locally for testing
pip install dist/fatfs-0.1.8-*.whl
```

## Usage Examples

### Basic Directory Traversal

```python
from fatfs import RamDisk, create_extended_partition

# Create and format filesystem
storage = bytearray(1024 * 1024)  # 1MB
disk = RamDisk(storage, sector_size=512)
partition = create_extended_partition(disk)
partition.mkfs()
partition.mount()

# Create directory structure
partition.makedirs("/test/subdir", exist_ok=True)
partition.write_file("/test/file1.txt", b"Hello")
partition.write_file("/test/subdir/file2.txt", b"World")

# List directory
files = partition.listdir("/test")
print(files)  # ['file1.txt', 'subdir']

# Walk directory tree
for root, dirs, files in partition.walk("/"):
    print(f"Directory: {root}")
    for file in files:
        print(f"  File: {file}")
    for dir in dirs:
        print(f"  Dir: {dir}")
```

### File Operations

```python
# Check if path exists
if partition.exists("/test/file1.txt"):
    print("File exists")

# Get file info
info = partition.stat("/test/file1.txt")
print(f"Size: {info['size']} bytes")
print(f"Is directory: {info['is_dir']}")

# Read file
data = partition.read_file("/test/file1.txt")
print(data.decode())  # "Hello"

# Rename file
partition.rename("/test/file1.txt", "/test/renamed.txt")

# Delete file
partition.remove("/test/renamed.txt")
```

### Bulk Operations

```python
from pathlib import Path

# Copy entire directory tree from host to FatFS
source_dir = Path("./data")
partition.copy_tree_from(source_dir, "/uploaded")

# Copy entire directory tree from FatFS to host
dest_dir = Path("./extracted")
partition.copy_tree_to("/uploaded", dest_dir)
```

### Integration with platform-espressif32

The extended features can be used in the download_fatfs function:

```python
def download_fatfs(target, source, env):
    """Download FAT filesystem from device and extract to directory."""
    # ... download image ...
    
    # Create extended partition
    disk = RamDisk(fs_data, sector_size=sector_size, sector_count=sector_count)
    partition = create_extended_partition(disk)
    partition.mount()
    
    # Extract using walk()
    unpack_path = Path(get_project_dir()) / unpack_dir
    partition.copy_tree_to("/", unpack_path)
    
    partition.unmount()
```

## API Reference

### PartitionExtended Class

#### Directory Operations

- `listdir(path="/")` - List directory contents
- `walk(top="/")` - Walk directory tree (generator)
- `makedirs(path, exist_ok=False)` - Create directory and parents
- `rmdir(path)` - Remove empty directory

#### File Operations

- `stat(path)` - Get file/directory information
- `exists(path)` - Check if path exists
- `isfile(path)` - Check if path is a file
- `isdir(path)` - Check if path is a directory
- `remove(path)` - Delete file
- `rename(old_path, new_path)` - Rename/move file

#### File I/O

- `read_file(path)` - Read entire file
- `write_file(path, data)` - Write entire file
- `open(path, mode)` - Open file (from base Partition)

#### Bulk Operations

- `copy_tree_from(source_dir, dest_path="/")` - Copy from host to FatFS
- `copy_tree_to(source_path, dest_dir)` - Copy from FatFS to host

### Helper Functions

- `create_extended_partition(disk)` - Create PartitionExtended instance

## Testing

```python
import pytest
from fatfs import RamDisk, create_extended_partition

def test_directory_traversal():
    # Setup
    storage = bytearray(1024 * 1024)
    disk = RamDisk(storage, sector_size=512)
    partition = create_extended_partition(disk)
    partition.mkfs()
    partition.mount()
    
    # Test makedirs
    partition.makedirs("/test/deep/dir", exist_ok=True)
    assert partition.isdir("/test/deep/dir")
    
    # Test write/read
    partition.write_file("/test/file.txt", b"test data")
    data = partition.read_file("/test/file.txt")
    assert data == b"test data"
    
    # Test walk
    dirs_found = []
    files_found = []
    for root, dirs, files in partition.walk("/"):
        dirs_found.extend(dirs)
        files_found.extend(files)
    
    assert "test" in dirs_found
    assert "file.txt" in files_found
    
    # Cleanup
    partition.unmount()

if __name__ == "__main__":
    test_directory_traversal()
    print("✅ All tests passed")
```

## Performance Considerations

- `walk()` recursively traverses directories - can be slow for large trees
- `listdir()` reads entire directory at once - efficient for small directories
- `copy_tree_*()` operations copy files one by one - consider progress callbacks for large operations

## Limitations

- No symbolic link support (FatFS limitation)
- No file permissions beyond read-only attribute (FatFS limitation)
- No timestamps in current implementation (can be added)
- Directory operations are not atomic

## Future Enhancements

1. **Timestamps**: Add support for file modification times
2. **Progress Callbacks**: Add progress reporting for bulk operations
3. **Async Operations**: Add async/await support for I/O operations
4. **Caching**: Add directory listing cache for performance
5. **Context Managers**: Add context manager support for directories

## Changelog

### v0.1.3
- Added extended directory traversal features
- Added `walk()`, `listdir()`, `stat()` functions
- Added bulk copy operations
- Added convenience file I/O methods

## Contributing

To contribute extended features:

1. Add new functions to `wrapper_extended.pyx`
2. Add Python wrappers to `partition_extended.py`
3. Add tests to verify functionality
4. Update this documentation
5. Submit pull request

## License

Same as fatfs-python (MIT License)
