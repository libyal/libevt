/*
 * File functions
 *
 * Copyright (c) 2011, Joachim Metz <jbmetz@users.sourceforge.net>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBEVT_INTERNAL_FILE_H )
#define _LIBEVT_INTERNAL_FILE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libevt_array_type.h"
#include "libevt_extern.h"
#include "libevt_io_handle.h"
#include "libevt_libbfio.h"
#include "libevt_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevt_internal_file libevt_internal_file_t;

struct libevt_internal_file
{
	/* The records array
	 */
	libevt_array_t *records_array;

	/* The IO handle
	 */
	libevt_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;
};

LIBEVT_EXTERN \
int libevt_file_initialize(
     libevt_file_t **file,
     liberror_error_t **error );

LIBEVT_EXTERN \
int libevt_file_free(
     libevt_file_t **file,
     liberror_error_t **error );

LIBEVT_EXTERN \
int libevt_file_signal_abort(
     libevt_file_t *file,
     liberror_error_t **error );

LIBEVT_EXTERN \
int libevt_file_open(
     libevt_file_t *file,
     const char *filename,
     int access_flags,
     liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEVT_EXTERN \
int libevt_file_open_wide(
     libevt_file_t *file,
     const wchar_t *filename,
     int access_flags,
     liberror_error_t **error );
#endif

LIBEVT_EXTERN \
int libevt_file_open_file_io_handle(
     libevt_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     liberror_error_t **error );

LIBEVT_EXTERN \
int libevt_file_close(
     libevt_file_t *file,
     liberror_error_t **error );

int libevt_file_open_read(
     libevt_internal_file_t *internal_file,
     liberror_error_t **error );

LIBEVT_EXTERN \
int libevt_file_get_ascii_codepage(
     libevt_file_t *file,
     int *ascii_codepage,
     liberror_error_t **error );

LIBEVT_EXTERN \
int libevt_file_set_ascii_codepage(
     libevt_file_t *file,
     int ascii_codepage,
     liberror_error_t **error );

LIBEVT_EXTERN \
int libevt_file_get_version(
     libevt_file_t *file,
     uint32_t *major_version,
     uint32_t *minor_version,
     liberror_error_t **error );

LIBEVT_EXTERN \
int libevt_file_get_number_of_records(
     libevt_file_t *file,
     int *number_of_records,
     liberror_error_t **error );

LIBEVT_EXTERN \
int libevt_file_get_record(
     libevt_file_t *file,
     int record_index,
     libevt_record_t **record,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

