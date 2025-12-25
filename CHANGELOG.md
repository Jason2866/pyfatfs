# Changelog

All notable changes to this project will be documented in this file.

## [0.1.4] - 2024-12-25

### Added
- **Extended Directory Traversal**: Full directory traversal support
  - `walk()` - os.walk()-like directory tree traversal
  - `listdir()` - List directory contents
  - `stat()` - Get file/directory information
  - `exists()`, `isfile()`, `isdir()` - Path checking utilities
  - `remove()`, `rmdir()` - Delete files and directories
  - `rename()` - Rename/move files and directories
  - `makedirs()` - Create directory trees
  - `read_file()`, `write_file()` - Convenient file I/O
  - `copy_tree_from()`, `copy_tree_to()` - Bulk copy operations

- **New Classes**:
  - `DIR_Handle` - Directory handle wrapper
  - `FILINFO_Handle` - File info wrapper with helper methods
  - `PartitionExtended` - Extended partition class with all new features

- **New Python Wrapper Functions**:
  - `pyf_opendir()`, `pyf_closedir()`, `pyf_readdir()` - Directory operations
  - `pyf_stat()`, `pyf_unlink()`, `pyf_rename()` - File operations
  - `pyf_read()`, `pyf_lseek()`, `pyf_sync()`, `pyf_truncate()` - File I/O

- **Documentation**:
  - `EXTENDED_FEATURES.md` - Complete guide for extended features
  - Usage examples and API reference
  - Integration guide for platform-espressif32

### Improved
- **diskio.py**: Converted to proper Abstract Base Class (ABC)
  - `Disk` is now an ABC with `@abstractmethod` decorators
  - Better error messages with `NotImplementedError` instead of `assert`
  - Added comprehensive docstrings
  - Added type hints
  - Added bounds checking in `RamDisk.read()` and `RamDisk.write()`
  - Added `__repr__()` for better debugging
  - Improved parameter validation with descriptive error messages

### Changed
- Development Status: Alpha → Beta (with extended features)
- `RamDisk.__init__()`: `sector_count` parameter now optional (auto-calculated)
- Better error messages throughout

### Benefits
- Complete directory traversal for download operations
- Compatible with os.walk() patterns
- Enables full filesystem extraction from devices
- Simplifies bulk file operations
- More pythonic and maintainable code

## [0.1.3] - 2024-12-25

### Fixed
- Fixed incorrect `assert()` syntax that caused SyntaxWarnings in Python 3.13+
  - Changed `assert(0, "message")` to `assert False, "message"` in Disk class methods
  - Changed `assert(condition, "message")` to `assert condition, "message"` in RamDisk class
  - Fixes 7 SyntaxWarning instances in diskio.py

### Changed
- Updated Development Status classifier from "Pre-Alpha" to "Alpha"
- Added Python 3.8-3.13 version classifiers

### Technical Details
The incorrect syntax `assert(0, "message")` creates a tuple `(0, "message")` which is always truthy,
making the assertion always pass. The correct syntax `assert False, "message"` properly raises
AssertionError when the condition is false.

## [0.1.2] - Previous Release

Initial alpha release with basic FatFS functionality.
