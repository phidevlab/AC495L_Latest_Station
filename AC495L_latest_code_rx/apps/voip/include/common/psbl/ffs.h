/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                            ****                                          |**
**|                            ****                                          |**
**|                            ******o***                                    |**
**|                      ********_///_****                                   |**
**|                      ***** /_//_/ ****                                   |**
**|                       ** ** (__/ ****                                    |**
**|                           *********                                      |**
**|                            ****                                          |**
**|                            ***                                           |**
**|                                                                          |**
**|         Copyright (c) 1998-2008 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/*******************************************************************************
 * FILE PURPOSE:    FFS related definintions
 *******************************************************************************
 * FILE NAME:       ffs.h
 *
 * DESCRIPTION:     FFS related definintions
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#ifndef ___FFS_H___
#define ___FFS_H___

#define FFS_VERSION 0x00

#define FFS_F_DELETE           0x8000
#define FFS_F_BLOCK_ALIGN      0x4000
#define FFS_F_AUTO_EXECUTE     0x2000
#define FFS_F_EXT_FIFLE_SYSTEM 0x1000
#define FFS_F_VOLUME_ID        0x0800
#define FFS_F_LINK             0x0400
#define FFS_F_DIRECTORY        0x0200
#define FFS_F_USER_READ        0x0100
#define FFS_F_USER_WRITE       0x0080
#define FFS_F_USER_EXECUTE     0x0040
#define FFS_F_GROUP_READ       0x0020
#define FFS_F_GROUP_WRITE      0x0010
#define FFS_F_GROUP_EXECUTE    0x0008
#define FFS_F_WORLD_READ       0x0004
#define FFS_F_WORLD_WRITE      0x0002
#define FFS_F_WORLD_EXECUTE    0x0001

#define FFS_ERRS_NO_ERROR              0
#define FFS_ERRS_NO_SUCH_FFS           1
#define FFS_ERRS_NOT_A_VALID_FFS       2
#define FFS_ERRS_SLOT_NOT_ENABLED      3
#define FFS_ERRS_VERSION_NOT_SUPPORTED 4
#define FFS_ERRS_FFS_ALREADY_MOUNTED   5

typedef char cbit16[2];
typedef char cbit8;
typedef char cbit24[3];

typedef struct ffs_sEntry
                  {
                  cbit16 inode;
                  cbit16 flags;
                  cbit16 mdate;
                  cbit16 mtime;
                  cbit8  uid;
                  cbit8  gid;
                  cbit8  fnlen;
                  cbit24 flen;
                  cbit8  aeo;
                  cbit8  data[1];
                  }ffs_tEntry;

typedef struct fs_sBlock
                 {
                 struct fs_sBlock  *Next;
                 int               Length;
                 char              Data[1];
                 }fs_tBlock;

typedef struct fs_sFileHandle
                 {
                 int        FlashFile;
                 int        Mode;
                 int        State;
                 int        SeekValue;
                 ffs_tEntry *FlashFileSystemEntry;
                 fs_tBlock  *FSRamBlock;
                 }fs_tFileHandle;

#define SLOT_BASE     0xbc000000
#define NUM_OF_SLOTS 8

#define FLASH_BASE          0xb0000000
#define MIN_BOOT_BLOCK_SIZE     (128*1024)

typedef struct ffs_s
                 {
                 void *ffs_ptr;
                 int  slot;
                 }ffs_tMountPoint;

/* Initialize calls */

typedef struct _fs_sFILE
          {
          int   _AvailableBytes;        /* Available Bytes in buffer        */
          struct _fs_sFILE *SubFileSystem; /* Pointer to sub file system    */
          char  *_NextBytePtr;          /* next byte from/to here in buffer */
          char  *_BufferBase;           /* the malloc pointer               */
          char  _SubSystemType;         /* File Subsystem Type              */
          char  _FileState;             /* the state of the file            */
          }FFS_FILE;

void   ffs_InitFlashFIleSystem(void);
int ffs_init(void);

/* File minipulation calls */

FFS_FILE   *ffs_fopen(const char *filename, const char *type);
unsigned int ffs_fread(void *ptr, unsigned int size, unsigned int nitems, FFS_FILE *stream);
unsigned int ffs_fwrite(const void *ptr, unsigned int size, unsigned int nitems, FFS_FILE *stream);
int    ffs_fclose(FFS_FILE *stream);
int    ffs_fseek(FFS_FILE *stream, int offset, int ptrname);
int    ffs_remove(const char *filename);
/* We are not using these functions */
unsigned int ffs_ftell(FFS_FILE *stream);
int ffs_feof(FFS_FILE *);
void    ffs_ls(int show_deleted);


/* Mounting and unmounting Calls */
int    ffs_AddFileSystem(int slot,void *ffs);
int    ffs_RemoveSlotMount(int slot);

/* FFS Information */
#ifdef FFS
ffs_tEntry *GetFfsBase(int ffs_index);
#endif
ffs_tEntry *ffs_getFfsBase(int ffs_index);
unsigned int ffs_getFfsSize( int ffs_index );

/* Internal ffs.c calls also used by ffs_utils.c */

int    ffs_AddNewSlotMount(int slot);
unsigned short _ffs_GetBit16Value(char *cbit16);
void   _ffs_PutBit16Value(char *cbit16, unsigned short val);
void   _ffs_UpdateBit16Value(char *cbit16, unsigned short val);
int  _ffs_GetBit24Value(char *cbit24);
void   _ffs_PutBit24Value(char *cbit16, unsigned int val);
int  _ffs_GetBit32Value(char *cbit32);
void   _ffs_DecodeAndPrintMdate(int mdate);
void   _ffs_DecodeAndPrintMtime(int mtime);
void   _ffs_SplitIntoFirstDirAndRest(char *src,char **d1,char **l1);
void   _ffs_xferstream(FFS_FILE *stream,ffs_tEntry *oldf ,ffs_tEntry *newf );

void _ffs_UpdateBit16Value_nor(char *cbit16, unsigned short val);
void _ffs_UpdateBit16Value_nand(char *cbit16, unsigned short val);

#ifdef FFS
FFS_FILE   *ffs_fopen_nand(const char *filename, const char *type);
unsigned int ffs_fread_nand(void *ptr, unsigned int size, unsigned int nitems, FFS_FILE *stream);
unsigned int ffs_fwrite_nand(const void *ptr, unsigned int size, unsigned int nitems, FFS_FILE *stream);
int    ffs_fclose_nand(FFS_FILE *stream);
int    ffs_fseek_nand(FFS_FILE *stream, int offset, int ptrname);
unsigned int ffs_ftell_nand(FFS_FILE *stream);
int ffs_remove_nand(ffs_tEntry *ffs, unsigned short val);
void    ffs_ls_nand(void* ffs, int show_deleted);
/* Mounting and unmounting Calls */
int    ffs_AddFileSystem_nand(int slot,void *ffs);

ffs_tEntry *_ffs_DumpFlashFileSystemEntry_nand(ffs_tEntry *ffse,int show_deleted);
ffs_tEntry *_ffs_FindFFSEWithNameInDir_nand(ffs_tEntry *ffs,int inode,char *name, FFS_FILE *flashPtr);

/*NOR Flash functions */
FFS_FILE   *ffs_fopen_nor(const char *filename, const char *type);
unsigned int ffs_fread_nor(void *ptr, unsigned int size, unsigned int nitems, FFS_FILE *stream);
unsigned int ffs_fwrite_nor(const void *ptr, unsigned int size, unsigned int nitems, FFS_FILE *stream);
int    ffs_fclose_nor(FFS_FILE *stream);
int    ffs_fseek_nor(FFS_FILE *stream, int offset, int ptrname);
unsigned int ffs_ftell_nor(FFS_FILE *stream);
int    ffs_remove_nor(const char *filename);
void    ffs_ls_nor(void* ffs, int show_deleted);
/* Mounting and unmounting Calls */
int    ffs_AddFileSystem_nor(int slot,void *ffs);

ffs_tEntry *_ffs_DumpFlashFileSystemEntry_nor(ffs_tEntry *ffse,int show_deleted);
ffs_tEntry *_ffs_FindFFSEWithNameInDir_nor(ffs_tEntry *ffs,int inode,char *name);
#endif


#endif  /*___FFS_H___*/
