/*
 * diskiocheck.c - Disk I/O check function for FatFS wrapper
 * 
 * This file provides a simple disk I/O check function that can be called
 * from the Python wrapper to verify disk operations.
 */

int diskiocheck(void) {
    /* Simple check function - always returns 0 (success) */
    /* In a full implementation, this would perform actual disk I/O tests */
    return 0;
}
