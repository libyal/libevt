/*
 * End-of-file record functions
 *
 * Copyright (C) 2011-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBEVT_END_OF_FILE_RECORD_H )
#define _LIBEVT_END_OF_FILE_RECORD_H

#include <common.h>
#include <types.h>

#include "libevt_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevt_end_of_file_record libevt_end_of_file_record_t;

struct libevt_end_of_file_record
{
	/* Dummy
	 */
	int dummy;
};

int libevt_end_of_file_record_initialize(
     libevt_end_of_file_record_t **end_of_file_record,
     libcerror_error_t **error );

int libevt_end_of_file_record_free(
     libevt_end_of_file_record_t **end_of_file_record,
     libcerror_error_t **error );

int libevt_end_of_file_record_read_data(
     libevt_end_of_file_record_t *end_of_file_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVT_END_OF_FILE_RECORD_H ) */

