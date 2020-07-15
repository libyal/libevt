/*
 * Record values functions
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

#if !defined( _LIBEVT_RECORD_VALUES_H )
#define _LIBEVT_RECORD_VALUES_H

#include <common.h>
#include <types.h>

#include "libevt_event_record.h"
#include "libevt_io_handle.h"
#include "libevt_libbfio.h"
#include "libevt_libcerror.h"
#include "libevt_libfcache.h"
#include "libevt_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBEVT_RECORD_TYPES
{
	LIBEVT_RECORD_TYPE_EVENT,
	LIBEVT_RECORD_TYPE_END_OF_FILE
};

typedef struct libevt_record_values libevt_record_values_t;

struct libevt_record_values
{
	/* The offset
	 */
	off64_t offset;

	/* The (record) type
	 */
	uint8_t type;

	/* The (record) number
	 */
	uint32_t number;

	/* The event record
	 */
	libevt_event_record_t *event_record;
};

int libevt_record_values_initialize(
     libevt_record_values_t **record_values,
     libcerror_error_t **error );

int libevt_record_values_free(
     libevt_record_values_t **record_values,
     libcerror_error_t **error );

ssize_t libevt_record_values_read_file_io_handle(
         libevt_record_values_t *record_values,
         libbfio_handle_t *file_io_handle,
         libevt_io_handle_t *io_handle,
         off64_t *file_offset,
         uint8_t *has_wrapped,
         uint8_t strict_mode,
         libcerror_error_t **error );

int libevt_record_values_read_event(
     libevt_record_values_t *record_values,
     uint8_t *record_values_data,
     size_t record_values_data_size,
     uint8_t strict_mode,
     libcerror_error_t **error );

int libevt_record_values_get_offset(
     libevt_record_values_t *record_values,
     off64_t *offset,
     libcerror_error_t **error );

int libevt_record_values_get_type(
     libevt_record_values_t *record_values,
     uint8_t *type,
     libcerror_error_t **error );

int libevt_record_values_get_number(
     libevt_record_values_t *record_values,
     uint32_t *number,
     libcerror_error_t **error );

int libevt_record_values_get_creation_time(
     libevt_record_values_t *record_values,
     uint32_t *posix_time,
     libcerror_error_t **error );

int libevt_record_values_get_written_time(
     libevt_record_values_t *record_values,
     uint32_t *posix_time,
     libcerror_error_t **error );

int libevt_record_values_get_event_identifier(
     libevt_record_values_t *record_values,
     uint32_t *event_identifier,
     libcerror_error_t **error );

int libevt_record_values_get_event_type(
     libevt_record_values_t *record_values,
     uint16_t *event_type,
     libcerror_error_t **error );

int libevt_record_values_get_event_category(
     libevt_record_values_t *record_values,
     uint16_t *event_category,
     libcerror_error_t **error );

int libevt_record_values_get_utf8_source_name_size(
     libevt_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_utf8_source_name(
     libevt_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_utf16_source_name_size(
     libevt_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_utf16_source_name(
     libevt_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_utf8_computer_name_size(
     libevt_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_utf8_computer_name(
     libevt_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_utf16_computer_name_size(
     libevt_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_utf16_computer_name(
     libevt_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_utf8_user_security_identifier_size(
     libevt_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_utf8_user_security_identifier(
     libevt_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_utf16_user_security_identifier_size(
     libevt_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_utf16_user_security_identifier(
     libevt_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_number_of_strings(
     libevt_record_values_t *record_values,
     int *number_of_strings,
     libcerror_error_t **error );

int libevt_record_values_get_utf8_string_size(
     libevt_record_values_t *record_values,
     int string_index,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_utf8_string(
     libevt_record_values_t *record_values,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_utf16_string_size(
     libevt_record_values_t *record_values,
     int string_index,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_utf16_string(
     libevt_record_values_t *record_values,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libevt_record_values_get_data_size(
     libevt_record_values_t *record_values,
     size_t *data_size,
     libcerror_error_t **error );

int libevt_record_values_get_data(
     libevt_record_values_t *record_values,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libevt_record_values_read_element_data(
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int element_file_index,
     off64_t element_offset,
     size64_t element_size,
     uint32_t element_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVT_RECORD_VALUES_H ) */

