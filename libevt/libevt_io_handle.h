/*
 * Input/Output (IO) handle functions
 *
 * Copyright (C) 2011-2018, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBEVT_IO_HANDLE_H )
#define _LIBEVT_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libevt_libbfio.h"
#include "libevt_libcerror.h"
#include "libevt_libfcache.h"
#include "libevt_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const char *evt_file_signature;

typedef struct libevt_io_handle libevt_io_handle_t;

struct libevt_io_handle
{
	/* The major version
	 */
	uint32_t major_version;

	/* The minor version
	 */
	uint32_t minor_version;

	/* The file flags
	 */
	uint32_t file_flags;

	/* The file size
	 */
	size64_t file_size;

	/* Value to indicate the record data has wrapped
	 */
	uint8_t has_wrapped;

	/* Flags
	 */
	uint8_t flags;

	/* The codepage of the extended ASCII strings
	 */
	int ascii_codepage;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libevt_io_handle_initialize(
     libevt_io_handle_t **io_handle,
     libcerror_error_t **error );

int libevt_io_handle_free(
     libevt_io_handle_t **io_handle,
     libcerror_error_t **error );

int libevt_io_handle_clear(
     libevt_io_handle_t *io_handle,
     libcerror_error_t **error );

int libevt_io_handle_read_file_header(
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t *first_record_offset,
     uint32_t *end_of_file_record_offset,
     libcerror_error_t **error );

int libevt_io_handle_read_records(
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t first_record_offset,
     uint32_t end_of_file_record_offset,
     libfdata_list_t *records_list,
     off64_t *last_record_offset,
     libcerror_error_t **error );

int libevt_io_handle_end_of_file_record_scan(
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t *first_record_offset,
     uint32_t *end_of_file_record_offset,
     libcerror_error_t **error );

int libevt_io_handle_event_record_scan(
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size64_t size,
     libfdata_list_t *recovered_records_list,
     libcerror_error_t **error );

int libevt_io_handle_recover_records(
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t first_record_offset,
     uint32_t end_of_file_record_offset,
     off64_t last_record_offset,
     libfdata_list_t *records_list,
     libfdata_list_t *recovered_records_list,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVT_IO_HANDLE_H ) */

