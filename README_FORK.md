# pyfatfs - Enhanced FatFS Python Wrapper

**Fork of [fatfs-python](https://github.com/krakonos/fatfs-python) by Ladislav Laska**

This is an enhanced fork with extended directory traversal features, improved API, and better code quality.

## What's Different from Original?

### Extended Features
- ✅ **Full Directory Traversal**: `walk()`, `listdir()`, `stat()`
- ✅ **Path Operations**: `exists()`, `isfile()`, `isdir()`
- ✅ **File Operations**: `remove()`, `rmdir()`, `rename()`
- ✅ **Convenience Methods**: `makedirs()`, `read_file()`, `write_file()`
- ✅ **Bulk Operations**: `copy_tree_from()`, `copy_tree_to()`

### Improvements
- ✅ **Fixed SyntaxWarnings** in Python 3.13+
- ✅ **Abstract Base Class** for Disk with proper `@abstractmethod`
- ✅ **Better Error Messages** with `NotImplementedError`
- ✅ **Type Hints** throughout the codebase
- ✅ **Comprehensive Documentation** with examples
- ✅ **Bounds Checking** in read/write operations

## Installation

```bash
pip install pyfatfs
```

## Quick Start

### Basic Usage (Compatible with Original)

```python
from fatfs import RamDisk, Partition

# Create and format filesystem
storage = bytearray(1024 * 1024)  # 1MB
disk = RamDisk(storage, sector_size=512)
partition = Partition(disk)
partition.mkfs()
partition.mount()

# Create directory and write file
partition.mkdir("/test")
with partition.open("/test/file.txt", "w") as f:
    f.write(b"Hello FatFS!")

partition.unmount()
```

### Extended Features (New in Fork)

```python
from fatfs import RamDisk, create_extended_partition

# Create filesystem with extended features
storage = bytearray(1024 * 1024)
disk = RamDisk(storage, sector_size=512)
partition = create_extended_partition(disk)
partition.mkfs()
partition.mount()

# Use extended features
partition.makedirs("/test/deep/dir", exist_ok=True)
partition.write_file("/test/file.txt", b"Hello World")

# Walk directory tree (like os.walk)
for root, dirs, files in partition.walk("/"):
    print(f"Directory: {root}")
    for file in files:
        print(f"  File: {file}")

# Copy entire tree from host to FatFS
from pathlib import Path
partition.copy_tree_from(Path("./data"), "/uploaded")

# Copy entire tree from FatFS to host
partition.copy_tree_to("/uploaded", Path("./extracted"))

partition.unmount()
```

## Use Case: ESP32 Platform Integration

This fork was created specifically for integration with [platform-espressif32](https://github.com/tasmota/platform-espressif32) to enable full filesystem extraction from ESP32 devices.

```python
# In platform-espressif32 download_fatfs() function
from fatfs import RamDisk, create_extended_partition

# Download filesystem image from device
# ... (download code) ...

# Extract with full directory traversal
disk = RamDisk(fs_data, sector_size=512, sector_count=sector_count)
partition = create_extended_partition(disk)
partition.mount()

# Full extraction with all files and directories!
partition.copy_tree_to("/", Path("./unpacked_fs"))

partition.unmount()
```

## API Reference

### Extended Partition Methods

#### Directory Operations
- `listdir(path="/")` - List directory contents
- `walk(top="/")` - Walk directory tree (generator, like os.walk)
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

## Documentation

- [EXTENDED_FEATURES.md](EXTENDED_FEATURES.md) - Complete guide for extended features
- [CHANGELOG.md](CHANGELOG.md) - Version history and changes

## Credits

### Original Author
- **Ladislav Laska** - Original [fatfs-python](https://github.com/krakonos/fatfs-python) library

### Fork Maintainer
- **Johann Obermeier** - Extended features and improvements

### Underlying Library
- **ChaN** - [FatFS](http://elm-chan.org/fsw/ff/00index_e.html) C library

## License

MIT License (same as original fatfs-python)

## Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Add tests for new features
4. Submit a pull request

## Changelog

### v0.1.8 (2024-12-26) - Fork Release
- Added extended directory traversal features
- Improved diskio.py with Abstract Base Class
- Fixed SyntaxWarnings in Python 3.13+
- Added comprehensive documentation
- Better error handling and type hints

### v0.1.2 (Original)
- Initial alpha release

## Links

- **PyPI**: https://pypi.org/project/pyfatfs/
- **GitHub**: https://github.com/Jason2866/pyfatfs
- **Original Project**: https://github.com/krakonos/fatfs-python
- **FatFS Library**: http://elm-chan.org/fsw/ff/00index_e.html
