# Deployment Configuration Changes

## Summary

Updated GitHub Actions workflow to use PyPI Trusted Publisher for secure, token-free deployment.

## Changes Made

### 1. Workflow Trigger
**Before:**
```yaml
on:
  push:
```

**After:**
```yaml
on:
  push:
    tags:
      - 'v*'
  workflow_dispatch:
```

**Reason:** Only trigger on version tags (v*) or manual dispatch, not on every push.

### 2. SDist Build Fix
**Before:**
```yaml
- name: Generate *.c from *.pyx
  run: python -m cython -3 **/*.pyx
```

**After:**
```yaml
- name: Generate *.c from *.pyx
  run: |
    python -m cython -3 fatfs/wrapper.pyx
```

**Reason:** 
- `**/*.pyx` glob pattern doesn't work in shell
- Explicitly specify the wrapper.pyx file
- Fixes SDist build failure

### 3. Submodules Support
**Added:**
```yaml
- uses: actions/checkout@v4
  with:
    submodules: true
```

**Reason:** Ensure FatFS source code in `foreign/` is checked out.

### 4. Artifact Names
**Before:**
```yaml
name: fatfs-dist-${{ matrix.os }}
```

**After:**
```yaml
name: pyfatfs-dist-${{ matrix.os }}
```

**Reason:** Match the new package name `pyfatfs`.

### 5. Trusted Publisher Configuration
**Before:**
```yaml
- uses: pypa/gh-action-pypi-publish@v1.5.0
  with:
    user: __token__
    password: ${{ secrets.pypi_token }}
```

**After:**
```yaml
- name: Publish to PyPI
  uses: pypa/gh-action-pypi-publish@release/v1
  # No credentials needed - uses trusted publishing
```

**Added:**
```yaml
environment:
  name: pypi
  url: https://pypi.org/p/pyfatfs
permissions:
  id-token: write  # Required for trusted publishing
```

**Reason:**
- More secure - no API tokens to manage
- Uses OpenID Connect (OIDC) for authentication
- Automatic credential rotation
- Better audit trail

### 6. Python Version Support
**Added:**
```yaml
CIBW_BUILD: "cp38-* cp39-* cp310-* cp311-* cp312-* cp313-*"
CIBW_SKIP: "*-win32 pp* *-musllinux*"
```

**Reason:**
- Explicitly build for Python 3.8-3.13
- Skip 32-bit Windows, PyPy, and musllinux
- Clearer build matrix

### 7. Cython Version
**Before:**
```yaml
pip install "cython~=3.0.10"
```

**After:**
```yaml
pip install "cython>=3.0.0"
```

**Reason:** Allow any Cython 3.x version for better compatibility.

## Required PyPI Configuration

### Trusted Publisher Setup

1. Go to https://pypi.org/manage/account/publishing/
2. Add pending publisher with:
   - **PyPI Project Name**: `pyfatfs`
   - **Owner**: `Jason2866`
   - **Repository**: `pyfatfs`
   - **Workflow**: `deploy.yaml`
   - **Environment**: `pypi`

### GitHub Environment

1. Go to repository Settings → Environments
2. Create environment: `pypi`
3. (Optional) Add protection rules

## Testing

### Test Locally
```bash
# Build sdist
CYTHONIZE=0 python -m build --sdist

# Build wheel
CYTHONIZE=1 python -m build --wheel

# Check packages
twine check dist/*
```

### Test with TestPyPI
1. Configure TestPyPI trusted publisher
2. Uncomment `repository-url` in workflow
3. Push test tag

## Deployment Process

### 1. Prepare Release
```bash
# Update version in setup.py
vim setup.py

# Update changelog
vim CHANGELOG.md

# Commit changes
git add setup.py CHANGELOG.md
git commit -m "Prepare release v0.1.4"
git push
```

### 2. Create Tag
```bash
git tag -a v0.1.4 -m "Release v0.1.4 - Extended features and FatFS R0.16 compatibility"
git push origin v0.1.4
```

### 3. Monitor Workflow
- Go to Actions tab
- Watch "Build and upload to PyPI" workflow
- Check for any errors

### 4. Verify Release
```bash
# Wait a few minutes for PyPI to process
pip install --upgrade pyfatfs

# Test import
python -c "from fatfs import create_extended_partition; print('✅ Success')"
```

## Benefits

✅ **Security**: No API tokens in repository
✅ **Simplicity**: No secrets management
✅ **Reliability**: Automatic credential rotation
✅ **Traceability**: Clear audit trail on PyPI
✅ **Automation**: Fully automated release process

## Rollback Plan

If trusted publishing fails, you can temporarily use API tokens:

1. Create PyPI API token
2. Add to GitHub Secrets as `PYPI_TOKEN`
3. Update workflow:
   ```yaml
   - uses: pypa/gh-action-pypi-publish@release/v1
     with:
       user: __token__
       password: ${{ secrets.PYPI_TOKEN }}
   ```

## Documentation

- `PYPI_TRUSTED_PUBLISHER_SETUP.md` - Detailed setup guide
- `PYPI_RELEASE.md` - Release process guide
- `.github/workflows/deploy.yaml` - Workflow configuration
