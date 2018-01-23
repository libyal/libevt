/*
 * Record functions
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

#if !defined( _LIBEVT_RECORD_H )
#define _LIBEVT_RECORD_H

#include <common.h>
#include <types.h>

#include "libevt_extern.h"
#include "libevt_io_handle.h"
#include "libevt_libbfio.h"
#include "libevt_libcerror.h"
#include "libevt_record_values.h"
#include "libevt_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevt_internal_record libevt_internal_record_t;

struct libevt_internal_record
{
	/* The IO handle
	 */
	libevt_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The (event) record values
	 */
	libevt_record_values_t *record_values;
};

int libevt_record_initialize(
     libevt_record_t **record,
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libevt_record_values_t *record_values,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_free(
     libevt_record_t **record,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_offset(
     libevt_record_t *record,
     off64_t *offset,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_identifier(
     libevt_record_t *record,
     uint32_t *identifier,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_creation_time(
     libevt_record_t *record,
     uint32_t *posix_time,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_written_time(
     libevt_record_t *record,
     uint32_t *posix_time,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_event_identifier(
     libevt_record_t *record,
     uint32_t *event_identifier,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_event_type(
     libevt_record_t *record,
     uint16_t *event_type,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_event_category(
     libevt_record_t *record,
     uint16_t *event_category,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf8_source_name_size(
     libevt_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf8_source_name(
     libevt_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf16_source_name_size(
     libevt_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf16_source_name(
     libevt_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf8_computer_name_size(
     libevt_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf8_computer_name(
     libevt_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf16_computer_name_size(
     libevt_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf16_computer_name(
     libevt_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf8_user_security_identifier_size(
     libevt_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf8_user_security_identifier(
     libevt_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf16_user_security_identifier_size(
     libevt_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf16_user_security_identifier(
     libevt_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_number_of_strings(
     libevt_record_t *record,
     int *number_of_strings,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf8_string_size(
     libevt_record_t *record,
     int string_index,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf8_string(
     libevt_record_t *record,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf16_string_size(
     libevt_record_t *record,
     int string_index,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_utf16_string(
     libevt_record_t *record,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_data_size(
     libevt_record_t *record,
     size_t *data_size,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_record_get_data(
     libevt_record_t *record,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVT_RECORD_H ) */

