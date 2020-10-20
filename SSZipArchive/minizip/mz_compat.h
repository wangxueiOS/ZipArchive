/* mz_compat.h -- Backwards compatible interface for older versions
   Version 2.8.6, April 8, 2019
   part of the MiniZip project

   Copyright (C) 2010-2019 Nathan Moinvaziri
	 https://github.com/nmoinvaz/minizip
   Copyright (C) 1998-2010 Gilles Vollant
	 https://www.winimage.com/zLibDll/minizip.html

   This program is distributed under the terms of the same license as zlib.
   See the accompanying LICENSE file for the full text of the license.
*/

#ifndef MZ_COMPAT_H
#define MZ_COMPAT_H

#include "mz.h"
#include "../SSZipCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************/

#if defined(HAVE_ZLIB) && defined(MAX_MEM_LEVEL)
#ifndef DEF_MEM_LEVEL
#  if MAX_MEM_LEVEL >= 8
#    define DEF_MEM_LEVEL 8
#  else
#    define DEF_MEM_LEVEL  MAX_MEM_LEVEL
#  endif
#endif
#endif
#ifndef MAX_WBITS
#define MAX_WBITS     15
#endif
#ifndef DEF_MEM_LEVEL
#define DEF_MEM_LEVEL 8
#endif

#ifndef ZEXPORT
#  define ZEXPORT MZ_EXPORT
#endif

/***************************************************************************/

#if defined(STRICTZIP) || defined(STRICTZIPUNZIP)
/* like the STRICT of WIN32, we define a pointer that cannot be converted
	from (void*) without cast */
typedef struct TagzipFile__ { int unused; } zip_file__;
typedef zip_file__ *zipFile;
#else
typedef void *zipFile;
#endif

/***************************************************************************/

typedef void *zlib_filefunc_def;
typedef void *zlib_filefunc64_def;
typedef const char *zipcharpc;

typedef struct tm tm_unz;
typedef struct tm tm_zip;

typedef uint64_t ZPOS64_T;

/***************************************************************************/

// ZipArchive 2.x uses dos_date
#define MZ_COMPAT_VERSION 120

#if MZ_COMPAT_VERSION <= 110
#define mz_dos_date dosDate
#else
#define mz_dos_date dos_date
#endif

typedef struct
{
	uint32_t    mz_dos_date;
	struct tm   tmz_date;
	uint16_t    internal_fa;        /* internal file attributes        2 bytes */
	uint32_t    external_fa;        /* external file attributes        4 bytes */
} zip_fileinfo;

/***************************************************************************/

#define ZIP_OK                          (0)
#define ZIP_EOF                         (0)
#define ZIP_ERRNO                       (-1)
#define ZIP_PARAMERROR                  (-102)
#define ZIP_BADZIPFILE                  (-103)
#define ZIP_INTERNALERROR               (-104)

#define Z_BZIP2ED                       (12)

#define APPEND_STATUS_CREATE            (0)
#define APPEND_STATUS_CREATEAFTER       (1)
#define APPEND_STATUS_ADDINZIP          (2)

/***************************************************************************/
/* Writing a zip file  */

ZEXPORT zipFile tzipOpen(const char *path, int append);
ZEXPORT zipFile tzipOpen64(const void *path, int append);
ZEXPORT zipFile tzipOpen2(const char *path, int append, const char **globalcomment,
	zlib_filefunc_def *pzlib_filefunc_def);
ZEXPORT zipFile tzipOpen2_64(const void *path, int append, const char **globalcomment,
	zlib_filefunc64_def *pzlib_filefunc_def);
		zipFile tzipOpen_MZ(void *stream, int append, const char **globalcomment);

ZEXPORT int     tzipOpenNewFileInZip5(zipFile file, const char *filename, const zip_fileinfo *zipfi,
	const void *extrafield_local, uint16_t size_extrafield_local, const void *extrafield_global,
	uint16_t size_extrafield_global, const char *comment, uint16_t compression_method, int level,
	int raw, int windowBits, int memLevel, int strategy, const char *password,
	signed char aes, uint16_t version_madeby, uint16_t flag_base, int zip64);

ZEXPORT int     tzipWriteInFileInZip(zipFile file, const void *buf, uint32_t len);

ZEXPORT int     tzipCloseFileInZipRaw(zipFile file, uint32_t uncompressed_size, uint32_t crc32);
ZEXPORT int     tzipCloseFileInZipRaw64(zipFile file, int64_t uncompressed_size, uint32_t crc32);
ZEXPORT int     tzipCloseFileInZip(zipFile file);
ZEXPORT int     tzipCloseFileInZip64(zipFile file);

ZEXPORT int     tzipClose(zipFile file, const char *global_comment);
ZEXPORT int     tzipClose_64(zipFile file, const char *global_comment);
ZEXPORT int     tzipClose2_64(zipFile file, const char *global_comment, uint16_t version_madeby);
		int     tzipClose_MZ(zipFile file, const char *global_comment);
		int     tzipClose2_MZ(zipFile file, const char *global_comment, uint16_t version_madeby);
ZEXPORT void*   tzipGetStream(zipFile file);

/***************************************************************************/

#if defined(STRICTUNZIP) || defined(STRICTZIPUNZIP)
/* like the STRICT of WIN32, we define a pointer that cannot be converted
	from (void*) without cast */
typedef struct TagunzFile__ { int unused; } unz_file__;
typedef unz_file__ *unzFile;
#else
typedef void *unzFile;
#endif

/***************************************************************************/

#define UNZ_OK                          (0)
#define UNZ_END_OF_LIST_OF_FILE         (-100)
#define UNZ_ERRNO                       (-1)
#define UNZ_EOF                         (0)
#define UNZ_PARAMERROR                  (-102)
#define UNZ_BADZIPFILE                  (-103)
#define UNZ_INTERNALERROR               (-104)
#define UNZ_CRCERROR                    (-105)
#define UNZ_BADPASSWORD                 (-106)

/***************************************************************************/

typedef int (*unzFileNameComparer)(unzFile file, const char *filename1, const char *filename2);
typedef int (*unzIteratorFunction)(unzFile file);
typedef int (*unzIteratorFunction2)(unzFile file, unz_file_info64 *pfile_info, char *filename,
	uint16_t filename_size, void *extrafield, uint16_t extrafield_size, char *comment,
	uint16_t comment_size);

/***************************************************************************/
/* Reading a zip file */

ZEXPORT unzFile tunzOpen(const char *path);
ZEXPORT unzFile tunzOpen64(const void *path);
ZEXPORT unzFile tunzOpen2(const char *path, zlib_filefunc_def *pzlib_filefunc_def);
ZEXPORT unzFile tunzOpen2_64(const void *path, zlib_filefunc64_def *pzlib_filefunc_def);
		unzFile tunzOpen_MZ(void *stream);

ZEXPORT int     tunzClose(unzFile file);
		int     tunzClose_MZ(unzFile file);

ZEXPORT int     tunzGetGlobalInfo(unzFile file, unz_global_info* pglobal_info32);
ZEXPORT int     tunzGetGlobalInfo64(unzFile file, unz_global_info64 *pglobal_info);
ZEXPORT int     tunzGetGlobalComment(unzFile file, char *comment, uint16_t comment_size);

ZEXPORT int     tunzOpenCurrentFile(unzFile file);
ZEXPORT int     tunzOpenCurrentFilePassword(unzFile file, const char *password);
ZEXPORT int     tunzOpenCurrentFile2(unzFile file, int *method, int *level, int raw);
ZEXPORT int     tunzOpenCurrentFile3(unzFile file, int *method, int *level, int raw, const char *password);
ZEXPORT int     tunzReadCurrentFile(unzFile file, void *buf, uint32_t len);
ZEXPORT int     tunzCloseCurrentFile(unzFile file);


ZEXPORT int     tunzGetCurrentFileInfo(unzFile file, unz_file_info *pfile_info, char *filename,
	uint16_t filename_size, void *extrafield, uint16_t extrafield_size, char *comment,
	uint16_t comment_size);
ZEXPORT int     tunzGetCurrentFileInfo64(unzFile file, unz_file_info64 * pfile_info, char *filename,
	uint16_t filename_size, void *extrafield, uint16_t extrafield_size, char *comment,
	uint16_t comment_size);

ZEXPORT int     tunzGoToFirstFile(unzFile file);
ZEXPORT int     tunzGoToNextFile(unzFile file);
ZEXPORT int     tunzLocateFile(unzFile file, const char *filename, unzFileNameComparer filename_compare_func);

ZEXPORT int     tunzGetLocalExtrafield(unzFile file, void *buf, unsigned int len);

/***************************************************************************/
/* Raw access to zip file */

typedef struct unz_file_pos_s
{
	uint32_t pos_in_zip_directory;  /* offset in zip file directory */
	uint32_t num_of_file;           /* # of file */
} unz_file_pos;

ZEXPORT int     unzGetFilePos(unzFile file, unz_file_pos *file_pos);
ZEXPORT int     unzGoToFilePos(unzFile file, unz_file_pos *file_pos);

typedef struct unz64_file_pos_s
{
	int64_t  pos_in_zip_directory;   /* offset in zip file directory  */
	uint64_t num_of_file;            /* # of file */
} unz64_file_pos;

ZEXPORT int     unzGetFilePos64(unzFile file, unz64_file_pos *file_pos);
ZEXPORT int     unzGoToFilePos64(unzFile file, const unz64_file_pos *file_pos);

ZEXPORT int64_t unzGetOffset64(unzFile file);
ZEXPORT int32_t unzGetOffset(unzFile file);
ZEXPORT int     unzSetOffset64(unzFile file, int64_t pos);
ZEXPORT int     unzSetOffset(unzFile file, uint32_t pos);
ZEXPORT int64_t unztell(unzFile file);
ZEXPORT int32_t unzTell(unzFile file);
ZEXPORT int64_t unzTell64(unzFile file);
ZEXPORT int     unzSeek(unzFile file, int32_t offset, int origin);
ZEXPORT int     unzSeek64(unzFile file, int64_t offset, int origin);
ZEXPORT int     unzEndOfFile(unzFile file);
ZEXPORT void*   unzGetStream(unzFile file);

/***************************************************************************/

ZEXPORT void fill_fopen_filefunc(zlib_filefunc_def *pzlib_filefunc_def);
ZEXPORT void fill_fopen64_filefunc(zlib_filefunc64_def *pzlib_filefunc_def);
ZEXPORT void fill_win32_filefunc(zlib_filefunc_def *pzlib_filefunc_def);
ZEXPORT void fill_win32_filefunc64(zlib_filefunc64_def *pzlib_filefunc_def);
ZEXPORT void fill_win32_filefunc64A(zlib_filefunc64_def *pzlib_filefunc_def);
ZEXPORT void fill_win32_filefunc64W(zlib_filefunc64_def *pzlib_filefunc_def);
ZEXPORT void fill_memory_filefunc(zlib_filefunc_def *pzlib_filefunc_def);

/***************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
