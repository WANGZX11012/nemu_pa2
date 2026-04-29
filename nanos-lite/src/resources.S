.section .data
.global ramdisk_start, ramdisk_end
ramdisk_start:
.incbin "build/ramdisk.img"
ramdisk_end:

.section .rodata
.globl logo
logo:
.incbin "resources/logo.txt"
/* The above embeds the raw ASCII bytes of resources/logo.txt into the
 * read-only data section as the symbol `logo`.
 * At runtime `printf("%s", logo)` prints this embedded data.
 */
.byte 0
