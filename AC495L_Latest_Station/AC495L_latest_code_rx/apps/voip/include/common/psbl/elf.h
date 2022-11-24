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
**|         Copyright (c) 1998-2003 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**| Permission is hereby granted to licensees of Texas Instruments           |**
**| Incorporated (TI) products to use this computer program for the sole     |**
**| purpose of implementing a licensee product based on TI products.         |**
**| No other rights to reproduce, use, or disseminate this computer          |**
**| program, whether in part or in whole, are granted.                       |**
**|                                                                          |**
**| TI makes no representation or warranties with respect to the             |**
**| performance of this computer program, and specifically disclaims         |**
**| any responsibility for any damages, special or consequential,            |**
**| connected with the use of this program.                                  |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/*******************************************************************************
 * FILE PURPOSE:    ELF header file
 *******************************************************************************
 * FILE NAME:       elf.h
 *
 * DESCRIPTION:     Definitions taken from the ELF standard.
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#ifndef	_ELF_H_
#define	_ELF_H_

#define MAX_PHDRS       5
#define MAX_SHDRS       10

/*
 * The platform-dependent typedefs.
 */
typedef	unsigned int	Elf32_Addr;
typedef	unsigned short	Elf32_Half;
typedef	unsigned int	Elf32_Off;
typedef	unsigned int	Elf32_Word;
typedef	signed int	    Elf32_Sword;

/* 
 * ELF Header.
 */
#define EI_NIDENT       16

typedef struct {
    unsigned char       e_ident[EI_NIDENT];
    Elf32_Half          e_type;
    Elf32_Half          e_machine;
    Elf32_Word          e_version;
    Elf32_Addr          e_entry;
    Elf32_Off           e_phoff;
    Elf32_Off           e_shoff;
    Elf32_Word          e_flags;
    Elf32_Half          e_ehsize;
    Elf32_Half          e_phentsize;
    Elf32_Half          e_phnum;
    Elf32_Half          e_shentsize;
    Elf32_Half          e_shnum;
    Elf32_Half          e_shstrndx;
} Elf32_Ehdr;

#define ET_NONE         0  /* No file type */
#define ET_REL          1  /* Relocatable file */
#define ET_EXEC         2  /* Executable file */
#define ET_DYN          3  /* Shared object file */
#define ET_CORE         4  /* Core file */
#define ET_LOPROC  0xff00  /* Processor-specific */
#define ET_HIPROC  0xffff  /* Processor-specific */

#define EM_NONE       0  /* No machine */
#define EM_M32        1  /* AT&T WE 32100 */
#define EM_SPARC      2  /* SPARC */
#define EM_386        3  /* Intel 80386 */
#define EM_68K        4  /* Motorola 68000 */
#define EM_88K        5  /* Motorola 88000 */
#define EM_860        7  /* Intel 80860 */
#define EM_MIPS       8  /* MIPS RS3000 */

#define EV_NONE       0  /* Invalid version */
#define EV_CURRENT    1  /* Current version */

/* e_ident[] Identification Indexes.
 */
#define EI_MAG0	     0      /* File identification */
#define EI_MAG1	     1      /* File identification */
#define EI_MAG2	     2      /* File identification */
#define EI_MAG3	     3      /* File identification */
#define EI_CLASS     4      /* File class */
#define EI_DATA	     5      /* Data encoding */
#define EI_VERSION   6      /* File version */
#define EI_PAD	     7      /* Start of padding bytes */

#define ELFMAG0      0x7f   /* e_ident[EI_MAG0] */
#define ELFMAG1      'E'    /* e_ident[EI_MAG1] */
#define ELFMAG2      'L'    /* e_ident[EI_MAG2] */
#define ELFMAG3      'F'    /* e_ident[EI_MAG3] */

#define ELFCLASSNONE  0     /* Invalid class */
#define ELFCLASS32    1     /* 32-bit objects */
#define ELFCLASS64    2     /* 64-bit objects */

#define ELFDATANONE   0     /* Invalid data encoding */
#define ELFDATA2LSB   1     /* little-endian */
#define ELFDATA2MSB   2     /* big-endian */

/* 
 * Program Header.
 */
typedef struct {
    Elf32_Word        p_type;
    Elf32_Off         p_offset;
    Elf32_Addr        p_vaddr;
    Elf32_Addr        p_paddr;
    Elf32_Word        p_filesz;
    Elf32_Word        p_memsz;
    Elf32_Word        p_flags;
    Elf32_Word        p_align;
} Elf32_Phdr;

#define PT_NULL              0
#define PT_LOAD              1
#define PT_DYNAMIC           2
#define PT_INTERP            3
#define PT_NOTE              4
#define PT_SHLIB             5
#define PT_PHDR              6
#define PT_LOPROC   0x70000000
#define PT_HIPROC   0x7fffffff

/* 
 * Special Section Indexes.
 */
#define SHN_UNDEF            0
#define SHN_LORESERVE   0xff00
#define SHN_LOPROC      0xff00
#define SHN_HIPROC      0xff1f
#define SHN_ABS         0xfff1
#define SHN_COMMON      0xfff2
#define SHN_HIRESERVE   0xffff

/* 
 * Section Header.
 */
typedef struct {
    Elf32_Word        sh_name;
    Elf32_Word        sh_type;
    Elf32_Word        sh_flags;
    Elf32_Addr        sh_addr;
    Elf32_Off         sh_offset;
    Elf32_Word        sh_size;
    Elf32_Word        sh_link;
    Elf32_Word        sh_info;
    Elf32_Word        sh_addralign;
    Elf32_Word        sh_entsize;
} Elf32_Shdr;

/* Section Types (sh_type)
 */
#define SHT_NULL               0
#define SHT_PROGBITS           1
#define SHT_SYMTAB             2
#define SHT_STRTAB	           3
#define SHT_RELA	           4
#define SHT_HASH	           5
#define SHT_DYNAMIC            6
#define SHT_NOTE	           7
#define SHT_NOBITS	           8
#define SHT_REL		           9
#define SHT_SHLIB             10
#define SHT_DYNSYM            11
#define SHT_LOPROC    0x70000000
#define SHT_HIPROC    0x7fffffff
#define SHT_LOUSER    0x80000000
#define SHT_HIUSER    0xffffffff

/* Section Attribute Flags (sh_flags)
 */
#define SHF_WRITE             0x1
#define SHF_ALLOC             0x2
#define SHF_EXECINSTR         0x4
#define SHF_MASKPROC   0xf0000000

#define	STN_UNDEF		0

/*
 * Supporting ELF Macros.
 */
/* ELF File Header elements. */
#define ELFF_ENTRY         (elf_info.ehdr.e_entry)
#define ELFF_IDENT(idx)    (elf_info.ehdr.e_ident[idx])
#define ELFF_TYPE          (elf_info.ehdr.e_type)
#define ELFF_MACHINE       (elf_info.ehdr.e_machine)
#define ELFF_VERSION       (elf_info.ehdr.e_version)
#define ELFF_PHOFF         (elf_info.ehdr.e_phoff)
#define ELFF_SHOFF         (elf_info.ehdr.e_shoff)
#define ELFF_FLAGS         (elf_info.ehdr.e_flags)
#define ELFF_EHSIZE        (elf_info.ehdr.e_ehsize)
#define ELFF_PHENTSIZE     (elf_info.ehdr.e_phentsize)
#define ELFF_PHNUM         (elf_info.ehdr.e_phnum)
#define ELFF_SHENTSIZE     (elf_info.ehdr.e_shentsize)
#define ELFF_SHNUM         (elf_info.ehdr.e_shnum)
#define ELFF_SHSTRNDX      (elf_info.ehdr.e_shstrndx)

typedef struct ELF_INFO {
	Elf32_Ehdr      ehdr;
	Elf32_Phdr      phdr[MAX_PHDRS];
	Elf32_Shdr      shdr[MAX_SHDRS];
} ELF_INFO;

#endif /* _ELF_H_ */
