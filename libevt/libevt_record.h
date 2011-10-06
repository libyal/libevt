/*
 * Record functions
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

#if !defined( _LIBEVT_RECORD_H )
#define _LIBEVT_RECORD_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libevt_io_handle.h"
#include "libevt_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBEVT_RECORD_TYPES
{
	LIBEVT_RECORD_TYPE_EVENT,
	LIBEVT_RECORD_TYPE_END_OF_FILE
};

typedef struct libevt_record libevt_record_t;

struct libevt_record
{
	/* The type
	 */
	uint8_t type;
};

int libevt_record_initialize(
     libevt_record_t **record,
     liberror_error_t **error );

int libevt_record_free(
     libevt_record_t *record,
     liberror_error_t **error );

int libevt_record_clone(
     libevt_record_t **destination_record,
     libevt_record_t *source_record,
     liberror_error_t **error );

ssize_t libevt_record_read(
         libevt_record_t *record,
         libevt_io_handle_t *io_handle,
         libbfio_handle_t *file_io_handle,
         liberror_error_t **error );

int libevt_record_read_event(
     libevt_record_t *record,
     libevt_io_handle_t *io_handle,
     uint8_t *record_data,
     size_t record_data_size,
     liberror_error_t **error );

int libevt_record_read_end_of_file(
     libevt_record_t *record,
     libevt_io_handle_t *io_handle,
     uint8_t *record_data,
     size_t record_data_size,
     liberror_error_t **error );

int libevt_record_get_type(
     libevt_record_t *record,
     uint8_t *type,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

