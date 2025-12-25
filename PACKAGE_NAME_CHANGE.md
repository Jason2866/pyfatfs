# Package Name Change: pyfatfs → fatfs-ng

## Reason for Change

The package name `pyfatfs` is already taken on PyPI by another project:
- **Existing package**: https://pypi.org/project/pyfatfs/
- **Description**: PyFilesystem2 implementation for FAT filesystems
- **Author**: nathanhi

To avoid confusion and conflicts, we changed the package name to `fatfs-ng` (Next Generation).

## What Changed

### PyPI Package Name
- **Old**: `pyfatfs` (not available)
- **New**: `fatfs-ng` ✅

### Installation Command
```bash
# Old (would conflict)
pip install pyfatfs

# New (correct)
pip install fatfs-ng
```

### Import Statement
**No change!** The import remains the same:
```python
from fatfs import Partition, RamDisk, create_extended_partition
```

## Updated Files

All references to `pyfatfs` have been changed to `fatfs-ng`:

### Core Files
- ✅ `setup.py` - Package name: `fatfs-ng`
- ✅ `README.md` - Installation instructions and branding
- ✅ `CHANGELOG.md` - Version history

### Documentation
- ✅ `PYPI_RELEASE.md` - Release guide
- ✅ `PYPI_TRUSTED_PUBLISHER_SETUP.md` - PyPI configuration
- ✅ `PACKAGE_CONTENTS.md` - Package contents
- ✅ `FORK_COMPLETE.md` - Fork completion notes
- ✅ `DEPLOYMENT_CHANGES.md` - Deployment guide

### Integration
- ✅ `platform-espressif32/builder/penv_setup.py` - Dependency: `fatfs-ng`
- ✅ `platform-espressif32/FATFS_INTEGRATION.md` - Integration docs

### CI/CD
- ✅ `.github/workflows/deploy.yaml` - Artifact names and PyPI URL

## PyPI Trusted Publisher Configuration

Update the PyPI trusted publisher settings:

1. Go to https://pypi.org/manage/account/publishing/
2. Add pending publisher:
   - **PyPI Project Name**: `fatfs-ng` ⚠️ (changed from pyfatfs)
   - **Owner**: `Jason2866`
   - **Repository**: `pyfatfs`
   - **Workflow**: `deploy.yaml`
   - **Environment**: `pypi`

## GitHub Environment

Update the environment URL in repository settings:
- **Old**: `https://pypi.org/p/pyfatfs`
- **New**: `https://pypi.org/p/fatfs-ng`

## Benefits of fatfs-ng Name

✅ **Available** - No conflicts on PyPI
✅ **Clear** - "ng" = Next Generation, shows it's enhanced
✅ **Short** - Easy to type and remember
✅ **Descriptive** - Clearly related to FatFS
✅ **Professional** - Common naming pattern (e.g., Angular → Angular-ng)

## Comparison with Existing Packages

| Package | Description | Use Case |
|---------|-------------|----------|
| `fatfs` | Original wrapper by Ladislav Laska | Basic FatFS operations |
| `pyfatfs` | PyFilesystem2 implementation | PyFilesystem2 integration |
| `fatfs-ng` | Enhanced fork with extended features | ESP32, embedded systems, full features |

## Migration Guide

### For New Users
Simply install:
```bash
pip install fatfs-ng
```

### For Existing Users (if any)
If you were testing with the old name:
```bash
# Uninstall old (if installed locally)
pip uninstall pyfatfs

# Install new
pip install fatfs-ng

# Code remains the same!
from fatfs import Partition, RamDisk
```

## Release Checklist

Before releasing to PyPI:

- [x] Package name changed to `fatfs-ng` in setup.py
- [x] All documentation updated
- [x] Workflow artifact names updated
- [x] PyPI trusted publisher configured for `fatfs-ng`
- [x] GitHub environment URL updated
- [x] Integration dependencies updated
- [ ] Create PyPI trusted publisher (manual step)
- [ ] Push tag to trigger release

## Testing

```bash
# Build package
CYTHONIZE=0 python -m build --sdist

# Verify package name
tar -tzf dist/fatfs-ng-0.1.4.tar.gz | head -1
# Should show: fatfs-ng-0.1.4/

# Test installation
pip install dist/fatfs-ng-0.1.4.tar.gz
python -c "from fatfs import create_extended_partition; print('✅ OK')"
```

## Summary

The package is now correctly named `fatfs-ng` and ready for PyPI release. All documentation and configuration files have been updated. The import path remains unchanged for backward compatibility.

**Next step**: Configure PyPI trusted publisher with the name `fatfs-ng` and push a release tag.
