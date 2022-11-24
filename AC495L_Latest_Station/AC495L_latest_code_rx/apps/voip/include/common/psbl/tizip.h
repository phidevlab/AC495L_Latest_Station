#ifndef _TIZIP_H_
#define _TIZIP_H_

#define ZIP_MAGIC  0x705A6954ul
#define ELF_MAGIC  0x464c457ful

#define COMMENT_STRING_LEN 180

#pragma pack(1)
typedef struct {
	unsigned long magic;          /* magic number                                 */
	unsigned long image_length;   /* length of uncompressed image                 */
	unsigned long zip_length;     /* length of compressed image                   */
	unsigned long load_start;     /* default 0x100 for rom.bin built from rom.hex */
	unsigned long run_address;    /* default 0x4004 for application               */
	unsigned long image_crc;      /* crc for ziped data                           */
	unsigned long unzip_crc;      /* crc for ziped data                           */
   char          comment[COMMENT_STRING_LEN];
   unsigned long hw_flags;
   unsigned char rsv[44];       /* alignment for a page of 256 byte             */
} IMAGE_PREFIX;
#pragma pack()

#define b_htonl(data)         ((((unsigned int)(data) & 0x000000FF)<<24) | \
                              ( ((unsigned int)(data) & 0x0000FF00)<<8 ) | \
                              ( ((unsigned int)(data) & 0x00FF0000)>>8 ) | \
                              ( ((unsigned int)(data) & 0xFF000000)>>24))

/*#define MAX_MODULE_SIZE 0x00180000 */

#endif /* _TIZIP_H_ */

