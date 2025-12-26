# ESP32 Wear Leveling Support

## Overview

The `fatfs` package includes full support for ESP32 wear leveling layer, which is required by the ESP32 Arduino Core's `FFat` library.

## Why Wear Leveling?

The ESP32 Arduino Core's `FFat.begin()` function uses `esp_vfs_fat_spiflash_mount_rw_wl()`, which expects FAT partitions to be wrapped with a wear leveling layer. This layer:

- Distributes write/erase cycles evenly across flash sectors
- Extends flash memory lifespan
- Provides transparent sector remapping
- Is required by ESP-IDF's FAT filesystem implementation

## Quick Start

### Creating ESP32-Compatible Images

```python
from fatfs import (
    RamDisk,
    Partition,
    create_esp32_wl_image,
    calculate_esp32_wl_overhead
)

# 1. Calculate overhead
partition_size = 1536 * 1024  # 1.5 MB partition
wl_info = calculate_esp32_wl_overhead(partition_size, sector_size=4096)

print(f"Partition: {wl_info['partition_size']} bytes")
print(f"FAT data: {wl_info['fat_size']} bytes")
print(f"WL overhead: {wl_info['wl_overhead_size']} bytes")

# 2. Create FAT filesystem (use fat_size, not partition_size!)
storage = bytearray(wl_info['fat_size'])
disk = RamDisk(storage, sector_size=4096, sector_count=wl_info['fat_sectors'])
partition = Partition(disk)

# 3. Format and mount
partition.mkfs()
partition.mount()

# 4. Add files
with partition.open("/config.json", "w") as f:
    f.write(b'{"version": "1.0"}')

partition.unmount()

# 5. Wrap with wear leveling
wl_image = create_esp32_wl_image(storage, partition_size, sector_size=4096)

# 6. Save for ESP32
with open("fatfs.bin", "wb") as f:
    f.write(wl_image)
```

### Extracting from ESP32 Images

```python
from fatfs import (
    RamDisk,
    Partition,
    is_esp32_wl_image,
    extract_fat_from_esp32_wl
)

# Read image downloaded from ESP32
with open("downloaded.bin", "rb") as f:
    wl_image = f.read()

# Check if it's a wear-leveling image
if is_esp32_wl_image(wl_image, sector_size=4096):
    print("Detected ESP32 wear leveling layer")
    
    # Extract FAT data
    fat_data = extract_fat_from_esp32_wl(wl_image, sector_size=4096)
    
    if fat_data:
        # Mount and read
        storage = bytearray(fat_data)
        disk = RamDisk(storage, sector_size=4096)
        partition = Partition(disk)
        partition.mount()
        
        # Read files
        with partition.open("/config.json", "r") as f:
            content = f.read()
            print(content)
        
        partition.unmount()
else:
    print("Not a wear-leveling image")
```

## API Reference

### `ESP32WearLeveling` Class

Main class for wear leveling operations.

```python
from fatfs import ESP32WearLeveling

wl = ESP32WearLeveling(sector_size=4096, update_rate=16)
```

**Parameters:**
- `sector_size` (int): Sector size in bytes (default: 4096)
- `update_rate` (int): Update rate for wear leveling (default: 16)

**Methods:**

#### `create_wl_state()`
Create a WL_State structure.

```python
state = wl.create_wl_state(
    pos=0,
    max_pos=100,
    move_count=0,
    access_count=0,
    max_count=1600,
    device_id=0
)
```

#### `wrap_fat_image()`
Wrap FAT data with wear leveling layer.

```python
wl_image = wl.wrap_fat_image(fat_data, partition_size)
```

#### `extract_fat_from_wl()`
Extract FAT data from wear-leveling image.

```python
fat_data = wl.extract_fat_from_wl(wl_image)
```

#### `verify_wl_state()`
Verify WL_State CRC32.

```python
is_valid = wl.verify_wl_state(state_data)
```

#### `calculate_overhead()`
Calculate wear leveling overhead.

```python
total, wl_overhead, fat_sectors = wl.calculate_overhead(partition_size)
```

### Convenience Functions

#### `create_esp32_wl_image()`
Create a wear-leveling wrapped FAT image.

```python
from fatfs import create_esp32_wl_image

wl_image = create_esp32_wl_image(
    fat_data,           # Raw FAT filesystem data
    partition_size,     # Total partition size
    sector_size=4096    # Sector size (default: 4096)
)
```

#### `extract_fat_from_esp32_wl()`
Extract FAT data from ESP32 wear-leveling image.

```python
from fatfs import extract_fat_from_esp32_wl

fat_data = extract_fat_from_esp32_wl(
    wl_data,            # Wear-leveling wrapped image
    sector_size=4096    # Sector size (default: 4096)
)
```

#### `is_esp32_wl_image()`
Check if data is an ESP32 wear-leveling image.

```python
from fatfs import is_esp32_wl_image

if is_esp32_wl_image(data, sector_size=4096):
    print("This is a wear-leveling image")
```

#### `calculate_esp32_wl_overhead()`
Calculate wear leveling overhead.

```python
from fatfs import calculate_esp32_wl_overhead

info = calculate_esp32_wl_overhead(
    partition_size,     # Total partition size
    sector_size=4096    # Sector size (default: 4096)
)

print(f"Total sectors: {info['total_sectors']}")
print(f"WL overhead: {info['wl_overhead_sectors']} sectors")
print(f"FAT sectors: {info['fat_sectors']}")
print(f"FAT size: {info['fat_size']} bytes")
```

## Wear Leveling Structure

### Image Layout

```
┌─────────────────────────────────────────────────────────────┐
│ Sector 0: WL State Copy 1 (4096 bytes)                      │
├─────────────────────────────────────────────────────────────┤
│ Sector 1: WL State Copy 2 (4096 bytes)                      │
├─────────────────────────────────────────────────────────────┤
│ Sectors 2-N: FAT Filesystem Data                            │
│              (Boot sector, FATs, Root dir, Data area)       │
├─────────────────────────────────────────────────────────────┤
│ Sector N+1: Temp Sector (4096 bytes, for WL operations)     │
├─────────────────────────────────────────────────────────────┤
│ Sector N+2: WL State Copy 3 (4096 bytes)                    │
├─────────────────────────────────────────────────────────────┤
│ Sector N+3: WL State Copy 4 (4096 bytes)                    │
└─────────────────────────────────────────────────────────────┘
```

### WL_State Structure (48 bytes)

```c
struct WL_State {
    uint32_t pos;           // 0x00: Current position
    uint32_t max_pos;       // 0x04: Maximum position (FAT sectors)
    uint32_t move_count;    // 0x08: Move counter
    uint32_t access_count;  // 0x0C: Access counter
    uint32_t max_count;     // 0x10: Maximum count (update_rate × fat_sectors)
    uint32_t block_size;    // 0x14: Block size (4096)
    uint32_t version;       // 0x18: WL version (2)
    uint32_t device_id;     // 0x1C: Device ID
    uint8_t  reserved[12];  // 0x20: Reserved (0xFF)
    uint32_t crc32;         // 0x2C: CRC32 checksum
};
```

### Overhead Calculation

For a 1.5 MB partition (1,507,328 bytes):

```
Total sectors:     368 (1,507,328 / 4096)
WL overhead:       5 sectors (20,480 bytes)
  - 2 state sectors at start
  - 1 temp sector
  - 2 state sectors at end
FAT data:          363 sectors (1,486,848 bytes)
```

## Integration with PlatformIO

### platformio.ini

```ini
[env:esp32dev]
platform = espressif32
framework = arduino
board = esp32dev
board_build.filesystem = fatfs
board_build.partitions = partitions.csv
```

### partitions.csv

```csv
# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,     0x9000,  0x5000,
otadata,  data, ota,     0xe000,  0x2000,
app0,     app,  ota_0,   0x10000, 0x140000,
app1,     app,  ota_1,   0x150000,0x140000,
ffat,     data, fat,     0x290000,0x170000,
```

### Build Script

```python
# In your build script
from fatfs import (
    RamDisk,
    Partition,
    create_esp32_wl_image,
    calculate_esp32_wl_overhead
)
from pathlib import Path

def build_fatfs_image(source_dir, output_file, partition_size):
    # Calculate overhead
    wl_info = calculate_esp32_wl_overhead(partition_size, sector_size=4096)
    
    # Create FAT filesystem
    storage = bytearray(wl_info['fat_size'])
    disk = RamDisk(storage, sector_size=4096, sector_count=wl_info['fat_sectors'])
    partition = Partition(disk)
    partition.mkfs()
    partition.mount()
    
    # Copy files from source directory
    for file_path in Path(source_dir).rglob("*"):
        if file_path.is_file():
            rel_path = file_path.relative_to(source_dir)
            fs_path = "/" + str(rel_path).replace("\\", "/")
            
            # Create parent directories
            parent = "/" + str(rel_path.parent).replace("\\", "/")
            if parent != "/.":
                partition.makedirs(parent, exist_ok=True)
            
            # Copy file
            with partition.open(fs_path, "w") as f:
                f.write(file_path.read_bytes())
    
    partition.unmount()
    
    # Wrap with wear leveling
    wl_image = create_esp32_wl_image(storage, partition_size, sector_size=4096)
    
    # Write to output
    Path(output_file).write_bytes(wl_image)
    
    print(f"Created {output_file}")
    print(f"  Partition: {partition_size} bytes")
    print(f"  FAT data: {wl_info['fat_size']} bytes")
    print(f"  WL overhead: {wl_info['wl_overhead_size']} bytes")
```

## Arduino Code

```cpp
#include <FFat.h>

void setup() {
    Serial.begin(115200);
    
    // Mount FAT filesystem (with wear leveling)
    if (!FFat.begin(false)) {
        Serial.println("FFat Mount Failed");
        return;
    }
    
    Serial.println("FFat mounted successfully");
    
    // Read file
    File file = FFat.open("/config.json", "r");
    if (file) {
        String content = file.readString();
        Serial.println(content);
        file.close();
    }
    
    // Write file
    file = FFat.open("/output.txt", "w");
    if (file) {
        file.println("Hello from ESP32!");
        file.close();
    }
}

void loop() {
    // Your code here
}
```

## Troubleshooting

### "FFat Mount Failed"

**Cause:** Image doesn't have wear leveling layer or is corrupted.

**Solution:**
1. Verify image was created with `create_esp32_wl_image()`
2. Check partition size matches partition table
3. Verify sector size is 4096

### "Invalid CRC"

**Cause:** WL_State structure is corrupted.

**Solution:**
1. Rebuild the image
2. Check for transmission errors during upload
3. Verify flash is not damaged

### "Partition too small"

**Cause:** FAT data + WL overhead exceeds partition size.

**Solution:**
1. Increase partition size in `partitions.csv`
2. Reduce data in `data/` directory
3. Use `calculate_esp32_wl_overhead()` to check sizes

## References

- [ESP-IDF Wear Levelling](https://github.com/espressif/esp-idf/tree/master/components/wear_levelling)
- [ESP-IDF FAT Filesystem](https://github.com/espressif/esp-idf/tree/master/components/fatfs)
- [Arduino-ESP32 FFat Library](https://github.com/espressif/arduino-esp32/tree/master/libraries/FFat)
- [ChaN's FatFS](http://elm-chan.org/fsw/ff/00index_e.html)

## License

MIT License (same as fatfs-python)
