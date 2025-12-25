# PyPI Release Guide for pyfatfs v0.1.4

## Changes in v0.1.4

### New Features
- **Extended Directory Traversal**: Complete directory operations (walk, listdir, stat)
- **Path Operations**: exists(), isfile(), isdir()
- **File Operations**: remove(), rmdir(), rename(), makedirs()
- **Convenience Methods**: read_file(), write_file()
- **Bulk Operations**: copy_tree_from(), copy_tree_to()

### Improvements
- Proper Abstract Base Class for Disk with @abstractmethod decorators
- Better error messages with NotImplementedError
- Comprehensive docstrings and type hints
- Updated development status to "Beta"

## Pre-Release Checklist

### 1. Verify Changes
```bash
cd fatfs-python

# Check syntax
python3 -m py_compile fatfs/diskio.py

# Test import without warnings
python3 -c "import warnings; warnings.simplefilter('error', SyntaxWarning); from fatfs import Partition, RamDisk, create_extended_partition; print('✅ No warnings')"

# Run tests (if available)
python3 -m pytest tests/ || echo "No tests found"
```

### 2. Update Version
- [x] `setup.py`: version="0.1.4", name="pyfatfs"
- [x] `CHANGELOG.md`: Added v0.1.4 section
- [x] `README.md`: Updated to pyfatfs branding

### 3. Build Package
```bash
# Install build tools
pip install build twine

# Clean old builds
rm -rf dist/ build/ *.egg-info

# Build source and wheel distributions
python3 -m build

# Verify build
ls -lh dist/
# Should see:
# - pyfatfs-0.1.4.tar.gz
# - pyfatfs-0.1.4-*.whl
```

### 4. Test Package Locally
```bash
# Create test environment
python3 -m venv test_env
source test_env/bin/activate  # or test_env\Scripts\activate on Windows

# Install from local build
pip install dist/pyfatfs-0.1.4-*.whl

# Test import
python3 -c "from fatfs import Partition, RamDisk, create_extended_partition; print('✅ Import successful')"

# Deactivate
deactivate
```

### 5. Upload to TestPyPI (Optional)
```bash
# Upload to TestPyPI first
python3 -m twine upload --repository testpypi dist/*

# Test installation from TestPyPI
pip install --index-url https://test.pypi.org/simple/ pyfatfs==0.1.4
```

### 6. Upload to PyPI
```bash
# Upload to PyPI
python3 -m twine upload dist/*

# Enter credentials when prompted
# Username: __token__
# Password: pypi-... (your API token)
```

### 7. Verify Release
```bash
# Wait a few minutes, then test
pip install --upgrade pyfatfs

# Verify version
python3 -c "import fatfs; print(fatfs.__version__ if hasattr(fatfs, '__version__') else 'Version not set')"

# Test functionality
python3 -c "from fatfs import Partition, RamDisk, create_extended_partition; print('✅ Release verified')"
```

### 8. Create Git Tag
```bash
git tag -a v0.1.4 -m "Release v0.1.4 - Extended features and improvements"
git push origin v0.1.4
```

## PyPI Credentials

You'll need:
1. PyPI account at https://pypi.org
2. API token from https://pypi.org/manage/account/token/
3. Configure in `~/.pypirc`:

```ini
[pypi]
username = __token__
password = pypi-...your-token-here...

[testpypi]
username = __token__
password = pypi-...your-test-token-here...
```

## Post-Release

### Update platform-espressif32
Update the dependency version in `platform-espressif32/builder/penv_setup.py`:

```python
python_deps = {
    ...
    "pyfatfs": ">=0.1.4",  # Changed from "fatfs": ">=0.1.2"
    ...
}
```

### Announce Release
- GitHub Release: https://github.com/Jason2866/pyfatfs/releases
- Update README.md with new version
- Notify users of the extended features

## Troubleshooting

### Build Errors
```bash
# Install Cython if missing
pip install cython

# Ensure C compiler is available
# Linux: sudo apt-get install build-essential
# macOS: xcode-select --install
# Windows: Install Visual Studio Build Tools
```

### Upload Errors
```bash
# If "File already exists" error
# You cannot re-upload the same version
# Increment version number and rebuild

# If authentication fails
# Check ~/.pypirc or use --username and --password flags
```

## Quick Release Commands

```bash
# Full release process
cd fatfs-python
rm -rf dist/ build/ *.egg-info
python3 -m build
python3 -m twine check dist/*
python3 -m twine upload dist/*
git tag -a v0.1.4 -m "Release v0.1.4"
git push origin v0.1.4
```

## Notes

- This is a major feature release with extended functionality
- Compatible with all Python versions 3.8-3.13
- Import path remains `from fatfs import ...` for backward compatibility
- Package name changed from `fatfs` to `pyfatfs` on PyPI
- Fork of original fatfs-python by Ladislav Laska
- Maintained by Johann Obermeier
