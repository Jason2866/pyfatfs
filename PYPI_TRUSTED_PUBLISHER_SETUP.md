# PyPI Trusted Publisher Setup for fatfs-ng

## Overview

This project uses PyPI's Trusted Publisher feature for secure, token-free publishing from GitHub Actions.

## Setup Instructions

### 1. Configure PyPI Trusted Publisher

1. Go to https://pypi.org/manage/account/publishing/
2. Click "Add a new pending publisher"
3. Fill in the following details:
   - **PyPI Project Name**: `fatfs-ng`
   - **Owner**: `Jason2866`
   - **Repository name**: `pyfatfs`
   - **Workflow name**: `deploy.yaml`
   - **Environment name**: `pypi`
4. Click "Add"

### 2. GitHub Repository Settings

No secrets needed! The workflow uses OpenID Connect (OIDC) for authentication.

**Required GitHub Environment:**
1. Go to repository Settings → Environments
2. Create environment named `pypi`
3. (Optional) Add protection rules:
   - Required reviewers
   - Wait timer
   - Deployment branches: Only protected branches or tags

### 3. Workflow Configuration

The workflow is already configured in `.github/workflows/deploy.yaml`:

```yaml
environment:
  name: pypi
  url: https://pypi.org/p/pyfatfs
permissions:
  id-token: write  # Required for trusted publishing
```

## How It Works

1. **Tag Creation**: Push a tag starting with `v` (e.g., `v0.1.4`)
   ```bash
   git tag -a v0.1.4 -m "Release v0.1.4"
   git push origin v0.1.4
   ```

2. **Automatic Build**: GitHub Actions builds:
   - Source distribution (sdist)
   - Wheels for Linux, Windows, macOS (x86_64 and arm64)
   - Python versions: 3.8, 3.9, 3.10, 3.11, 3.12, 3.13

3. **Trusted Publishing**: 
   - GitHub generates a short-lived OIDC token
   - PyPI verifies the token matches the trusted publisher configuration
   - Artifacts are uploaded without manual authentication

## Manual Trigger

You can also trigger the workflow manually:

1. Go to Actions → Build and upload to PyPI
2. Click "Run workflow"
3. Select branch/tag
4. Click "Run workflow"

## Testing with TestPyPI

To test the release process:

1. Configure TestPyPI trusted publisher at https://test.pypi.org/manage/account/publishing/
2. Uncomment these lines in `deploy.yaml`:
   ```yaml
   with:
     repository-url: https://test.pypi.org/legacy/
   ```
3. Push a test tag

## Advantages of Trusted Publishing

✅ **No API tokens** - No secrets to manage or rotate
✅ **More secure** - Short-lived credentials, no token leakage risk
✅ **Simpler setup** - No manual token creation or storage
✅ **Audit trail** - PyPI logs show which workflow published each release
✅ **Automatic** - Works seamlessly with GitHub Actions

## Troubleshooting

### Error: "Trusted publishing exchange failure"

**Cause**: PyPI trusted publisher not configured correctly

**Solution**: 
1. Verify the publisher configuration at https://pypi.org/manage/account/publishing/
2. Ensure all fields match exactly:
   - Owner: `Jason2866`
   - Repository: `pyfatfs`
   - Workflow: `deploy.yaml`
   - Environment: `pypi`

### Error: "Permission denied"

**Cause**: Missing `id-token: write` permission

**Solution**: Already configured in workflow, but verify:
```yaml
permissions:
  id-token: write
```

### Error: "Environment not found"

**Cause**: GitHub environment `pypi` not created

**Solution**: Create environment in repository Settings → Environments

### Build Failures

**SDist fails**: 
- Check that `fatfs/wrapper.pyx` exists
- Verify Cython is installed: `pip install "cython>=3.0.0"`

**Wheel build fails**:
- Check cibuildwheel logs for specific platform errors
- Verify C compiler is available on build platform

## Release Checklist

Before creating a release:

- [ ] Update version in `setup.py`
- [ ] Update `CHANGELOG.md`
- [ ] Update `README.md` if needed
- [ ] Commit all changes
- [ ] Create and push tag:
  ```bash
  git tag -a v0.1.4 -m "Release v0.1.4 - Description"
  git push origin v0.1.4
  ```
- [ ] Monitor GitHub Actions workflow
- [ ] Verify release on PyPI: https://pypi.org/project/pyfatfs/
- [ ] Test installation: `pip install pyfatfs`

## References

- [PyPI Trusted Publishers Documentation](https://docs.pypi.org/trusted-publishers/)
- [GitHub Actions OIDC](https://docs.github.com/en/actions/deployment/security-hardening-your-deployments/about-security-hardening-with-openid-connect)
- [pypa/gh-action-pypi-publish](https://github.com/pypa/gh-action-pypi-publish)
