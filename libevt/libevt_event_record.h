/*
 * Event record functions
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

#if !defined( _LIBEVT_EVENT_RECORD_H )
#define _LIBEVT_EVENT_RECORD_H

#include <common.h>
#include <types.h>

#include "libevt_libcerror.h"
#include "libevt_libfwnt.h"
#include "libevt_strings_array.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevt_event_record libevt_event_record_t;

struct libevt_event_record
{
	/* Record size
	 */
	uint32_t record_size;

	/* Record number
	 */
	uint32_t record_number;

	/* Creation time
	 */
	uint32_t creation_time;

	/* Last written time
	 */
	uint32_t last_written_time;

	/* Event identifier
	 */
	uint32_t event_identifier;

	/* Event type
	 */
	uint16_t event_type;

	/* Event category
	 */
	uint16_t event_category;

	/* Source name
	 */
	uint8_t *source_name;

	/* Source name size
	 */
	size_t source_name_size;

	/* Computer name
	 */
	uint8_t *computer_name;

	/* Computer name size
	 */
	size_t computer_name_size;

	/* User security identifier
	 */
	libfwnt_security_identifier_t *user_security_identifier;

	/* Strings
	 */
	libevt_strings_array_t *strings;

	/* Data
	 */
	uint8_t *data;

	/* Data size
	 */
	size_t data_size;

	/* Copy of record size
	 */
	uint32_t copy_of_record_size;
};

int libevt_event_record_initialize(
     libevt_event_record_t **event_record,
     libcerror_error_t **error );

int libevt_event_record_free(
     libevt_event_record_t **event_record,
     libcerror_error_t **error );

int libevt_event_record_read_data(
     libevt_event_record_t *event_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libevt_event_record_get_record_number(
     libevt_event_record_t *event_record,
     uint32_t *record_number,
     libcerror_error_t **error );

int libevt_event_record_get_creation_time(
     libevt_event_record_t *event_record,
     uint32_t *posix_time,
     libcerror_error_t **error );

int libevt_event_record_get_last_written_time(
     libevt_event_record_t *event_record,
     uint32_t *posix_time,
     libcerror_error_t **error );

int libevt_event_record_get_event_identifier(
     libevt_event_record_t *event_record,
     uint32_t *event_identifier,
     libcerror_error_t **error );

int libevt_event_record_get_event_type(
     libevt_event_record_t *event_record,
     uint16_t *event_type,
     libcerror_error_t **error );

int libevt_event_record_get_event_category(
     libevt_event_record_t *event_record,
     uint16_t *event_category,
     libcerror_error_t **error );

int libevt_event_record_get_utf8_source_name_size(
     libevt_event_record_t *event_record,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_utf8_source_name(
     libevt_event_record_t *event_record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_utf16_source_name_size(
     libevt_event_record_t *event_record,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_utf16_source_name(
     libevt_event_record_t *event_record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_utf8_computer_name_size(
     libevt_event_record_t *event_record,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_utf8_computer_name(
     libevt_event_record_t *event_record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_utf16_computer_name_size(
     libevt_event_record_t *event_record,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_utf16_computer_name(
     libevt_event_record_t *event_record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_utf8_user_security_identifier_size(
     libevt_event_record_t *event_record,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_utf8_user_security_identifier(
     libevt_event_record_t *event_record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_utf16_user_security_identifier_size(
     libevt_event_record_t *event_record,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_utf16_user_security_identifier(
     libevt_event_record_t *event_record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_number_of_strings(
     libevt_event_record_t *event_record,
     int *number_of_strings,
     libcerror_error_t **error );

int libevt_event_record_get_utf8_string_size(
     libevt_event_record_t *event_record,
     int string_index,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_utf8_string(
     libevt_event_record_t *event_record,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_utf16_string_size(
     libevt_event_record_t *event_record,
     int string_index,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_utf16_string(
     libevt_event_record_t *event_record,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libevt_event_record_get_data_size(
     libevt_event_record_t *event_record,
     size_t *data_size,
     libcerror_error_t **error );

int libevt_event_record_get_data(
     libevt_event_record_t *event_record,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVT_EVENT_RECORD_H ) */

