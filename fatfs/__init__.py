from fatfs.wrapper import *
from fatfs.diskio import RamDisk

# Import extended features if available
try:
    from fatfs.partition_extended import PartitionExtended, create_extended_partition
    __all__ = ["wrapper", "diskio", "PartitionExtended", "create_extended_partition"]
except ImportError:
    __all__ = ["wrapper", "diskio"]
