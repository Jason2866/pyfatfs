# Fork Information

## About This Fork

This is `pyfatfs`, an enhanced fork of the original `fatfs-python` library by Ladislav Laska.

### Fork Details

- **Original Project**: [fatfs-python](https://github.com/krakonos/fatfs-python) by Ladislav Laska
- **Fork Name**: `pyfatfs`
- **Fork Maintainer**: Johann Obermeier
- **Fork Repository**: https://github.com/Jason2866/pyfatfs
- **PyPI Package**: https://pypi.org/project/pyfatfs/

### Why Fork?

The original `fatfs-python` library (v0.1.2) had several limitations:
1. Limited directory traversal support (no `walk()` function)
2. Basic file listing only
3. Incomplete download extraction capabilities
4. SyntaxWarnings in Python 3.13+
5. No Abstract Base Class for Disk interface

This fork addresses all these issues and adds extensive new features.

### Relationship to Original

This fork:
- ✅ **Maintains compatibility** with the original API
- ✅ **Adds new features** without breaking existing code
- ✅ **Fixes bugs** from the original
- ✅ **Improves code quality** with type hints and better documentation
- ✅ **Credits the original author** in all documentation

### Installation

```bash
# Install the fork
pip install pyfatfs

# The original package name still works in imports
from fatfs import RamDisk, Partition
```

### Migration from Original

If you're using the original `fatfs-python`:

```bash
# Uninstall original
pip uninstall fatfs

# Install fork
pip install pyfatfs
```

Your existing code will continue to work! The fork is a drop-in replacement.

### Contributing Back

If you have improvements that should go back to the original project:
1. Submit them to the original repository first
2. Then we can incorporate them here

### License

This fork maintains the same MIT License as the original project.

### Credits

#### Original Author
**Ladislav Laska** (krakonos@krakonos.org)
- Created the original fatfs-python library
- Provided the foundation for this fork

#### Fork Maintainer
**Johann Obermeier**
- Extended directory traversal features
- Improved code quality and documentation
- Integration with platform-espressif32

#### Underlying Library
**ChaN**
- FatFS C library author
- http://elm-chan.org/fsw/ff/00index_e.html

### Version History

| Version | Type | Maintainer | Notes |
|---------|------|------------|-------|
| 0.1.2 | Original | Ladislav Laska | Initial alpha release |
| 0.1.3 | Original | Ladislav Laska | Fixed assert warnings |
| 0.1.4 | Fork | Johann Obermeier | Extended features, improved API |

### Contact

- **Issues**: https://github.com/Jason2866/pyfatfs/issues
- **Original Issues**: https://github.com/krakonos/fatfs-python/issues

### Acknowledgments

Special thanks to:
- Ladislav Laska for creating the original library
- ChaN for the FatFS C library
- The platform-espressif32 project for the use case that drove these improvements
