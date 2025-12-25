# PyPI Package Contents

## Overview

The fatfs-ng package contains only the essential files needed for installation and compilation.

## Included Files

### Documentation (3 files)
```
README.md           - Main documentation
LICENSE.md          - MIT License
CHANGELOG.md        - Version history
```

### Python Source Code (3 files)
```
fatfs/__init__.py              - Package initialization
fatfs/diskio.py                - Disk I/O interface
fatfs/partition_extended.py    - Extended partition features
```

### Cython Source Files (4 files)
```
fatfs/wrapper.pyx          - Main Cython wrapper
fatfs/wrapper_extended.pyx - Extended features wrapper (not compiled separately)
fatfs/diskio.pxd           - Cython declarations for diskio
fatfs/ff.pxd               - Cython declarations for FatFS API
```

### C Source Files (2 files)
```
fatfs/diskiocheck.c  - Disk I/O check utility
fatfs/wrapper.c      - Pre-compiled Cython wrapper (for sdist)
```

### FatFS Library (7 files)
```
foreign/fatfs/source/ff.c          - FatFS core implementation
foreign/fatfs/source/ff.h          - FatFS API header
foreign/fatfs/source/ffconf.h      - FatFS configuration
foreign/fatfs/source/ffsystem.c    - System functions
foreign/fatfs/source/ffunicode.c   - Unicode support
foreign/fatfs/source/diskio.c      - Disk I/O template
foreign/fatfs/source/diskio.h      - Disk I/O header
```

### Build Configuration (3 files)
```
setup.py      - Package setup script
setup.cfg     - Setup configuration
MANIFEST.in   - Package manifest
```

## Total Size

- **Source files**: ~24 files
- **Estimated size**: ~500 KB (uncompressed)
- **Compressed tarball**: ~150 KB

## Excluded Files

The following files are **NOT** included in the PyPI package:

### Development Files
- `.git/` - Git repository
- `.github/` - GitHub Actions workflows
- `build/` - Build artifacts
- `test_venv/` - Virtual environment
- `tests/` - Test files
- `__pycache__/` - Python cache
- `*.pyc`, `*.pyo` - Compiled Python files
- `*.so`, `*.dll`, `*.dylib` - Compiled extensions

### Documentation (Development Only)
- `FORK_INFO.md` - Fork information
- `FORK_COMPLETE.md` - Fork completion notes
- `README_FORK.md` - Fork-specific readme
- `PYPI_RELEASE.md` - Release guide
- `PYPI_TRUSTED_PUBLISHER_SETUP.md` - Deployment setup
- `DEPLOYMENT_CHANGES.md` - Deployment changes
- `FATFS_R016_COMPATIBILITY.md` - Compatibility report
- `EXTENDED_FEATURES.md` - Extended features guide

### Build Tools
- `Makefile` - Build automation
- `tox.ini` - Testing configuration
- `run_check.py` - Check script
- `.gitignore`, `.gitmodules` - Git configuration

## Verification

To verify package contents:

```bash
# Build source distribution
CYTHONIZE=0 python -m build --sdist

# List contents
tar -tzf dist/pyfatfs-0.1.4.tar.gz

# Extract and inspect
tar -xzf dist/pyfatfs-0.1.4.tar.gz
cd pyfatfs-0.1.4
ls -la
```

## Installation Requirements

When users install from PyPI:

### From Source Distribution (sdist)
```bash
pip install fatfs-ng
```

**Requires:**
- C compiler (gcc, clang, MSVC)
- Python development headers
- Cython (automatically installed)

**Process:**
1. Downloads source tarball
2. Installs Cython
3. Compiles C extensions
4. Installs package

### From Wheel (binary)
```bash
pip install fatfs-ng
```

**Requires:**
- Nothing! Pre-compiled binary

**Process:**
1. Downloads wheel for platform
2. Installs package directly

## Wheel Contents

Wheels contain compiled extensions instead of source:

```
fatfs-ng/
├── __init__.py
├── diskio.py
├── partition_extended.py
├── wrapper.cpython-311-x86_64-linux-gnu.so  # Compiled extension
└── ...
```

## MANIFEST.in Configuration

The `MANIFEST.in` file controls what's included:

```
# Include essential documentation
include README.md LICENSE.md CHANGELOG.md

# Include FatFS source code
recursive-include foreign/fatfs/source *.c *.h

# Include Cython source files
include fatfs/*.pyx fatfs/*.pxd fatfs/diskiocheck.c

# Include Python source files
include fatfs/*.py

# Exclude everything else
global-exclude *.pyc *.pyo *.so __pycache__
prune build dist tests .git .github
exclude FORK_INFO.md PYPI_RELEASE.md ...
```

## Size Comparison

| Package Type | Size | Contents |
|--------------|------|----------|
| Source (sdist) | ~150 KB | Source code + FatFS |
| Wheel (Linux) | ~200 KB | Compiled extension |
| Wheel (macOS) | ~180 KB | Compiled extension |
| Wheel (Windows) | ~220 KB | Compiled extension |

## Benefits

✅ **Minimal size** - Only essential files
✅ **Fast downloads** - Small package size
✅ **Clean installation** - No unnecessary files
✅ **Reproducible builds** - All source included
✅ **Cross-platform** - Works on all platforms

## Verification Checklist

Before release, verify:

- [ ] No `.pyc` files in package
- [ ] No `__pycache__` directories
- [ ] No `.so`/`.dll` files in sdist
- [ ] No development documentation
- [ ] No test files
- [ ] No `.git` directory
- [ ] All FatFS source files present
- [ ] All Python source files present
- [ ] All Cython source files present
- [ ] README, LICENSE, CHANGELOG present

## Testing Package Contents

```bash
# Build and check
CYTHONIZE=0 python -m build --sdist
tar -tzf dist/fatfs-ng-*.tar.gz | wc -l  # Should be ~30 files

# Install from sdist
pip install dist/fatfs-ng-*.tar.gz

# Test import
python -c "from fatfs import create_extended_partition; print('✅ OK')"
```

## Conclusion

The PyPI package is **optimized and minimal**, containing only the files necessary for:
1. Installation
2. Compilation
3. Runtime operation
4. Basic documentation

All development files, tests, and extra documentation are excluded to keep the package size small and installation fast.
