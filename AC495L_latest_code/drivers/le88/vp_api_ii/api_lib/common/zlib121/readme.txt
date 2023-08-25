Legerity VoicePath API-II
Optional VpBootLoad() code for handling compressed boot images

This directory (the directory in which this readme file is located) contains
optional files which are only required if support for VpBootLoad()
decompression is compiled into the API-II using "#undef VE_NO_COMPRESS" in
ve_api.h.

The decompression code is based on zlib, a free, general-purpose,
portable data-compression library, Copyright (C) 1995-2003 Jean-loup Gailly
and Mark Adler.  See http://www.gzip.org/zlib for more information.

When choosing compiler optimizations, the C files in this directory should
probably be optimized for speed, to minimize the execution time of
VeBootLoad().

The compile-time options specified in vp_api_cfg.h determine which of the files
in this directory are required.  If compression is compiled in using
"#undef VE_NO_COMPRESS" in vp_api_cfg.h, then the following files are required:
	zstructs.h
	zlib.h
	zutil.h
	zconf.h
	inflate.h
	inflate.c

If additional "fast" compression code is compiled in using "#define
VE_COMPRESS_FAST" in vp_api_cfg.h, then the following file is required in
addition to the above files:
	inffast.c

There is no need to remove the unused files (if any) from this directory.
Removing unused files from this directory will not affect code size.  Code
size is determined by the compile-time options in vp_api_cfg.h.