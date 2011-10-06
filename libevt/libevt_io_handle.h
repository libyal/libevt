/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBEVT_IO_HANDLE_H )
#define _LIBEVT_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libevt_array_type.h"
#include "libevt_libbfio.h"
#include "libevt_libfvalue.h"

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

	/* The codepage of the extended ASCII strings
	 */
	int ascii_codepage;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libevt_io_handle_initialize(
     libevt_io_handle_t **io_handle,
     liberror_error_t **error );

int libevt_io_handle_free(
     libevt_io_handle_t **io_handle,
     liberror_error_t **error );

int libevt_io_handle_read_file_header(
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     liberror_error_t **error );

int libevt_io_handle_read_items(
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libevt_array_t *items_array,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

