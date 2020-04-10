/*
 * Strings array functions
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

#if !defined( _LIBEVT_STRINGS_ARRAY_H )
#define _LIBEVT_STRINGS_ARRAY_H

#include <common.h>
#include <types.h>

#include "libevt_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevt_strings_array libevt_strings_array_t;

struct libevt_strings_array
{
	/* The strings data
	 */
	uint8_t *strings_data;

	/* The strings data size
	 */
	size_t strings_data_size;

	/* The number of strings
	 */
	int number_of_strings;

	/* The strings
	 */
	uint8_t **strings;

	/* The string sizes
	 */
	size_t *string_sizes;
};

int libevt_strings_array_initialize(
     libevt_strings_array_t **strings_array,
     libcerror_error_t **error );

int libevt_strings_array_free(
     libevt_strings_array_t **strings_array,
     libcerror_error_t **error );

int libevt_strings_array_read_data(
     libevt_strings_array_t *strings_array,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libevt_strings_array_get_number_of_strings(
     libevt_strings_array_t *strings_array,
     int *number_of_strings,
     libcerror_error_t **error );

int libevt_strings_array_get_utf8_string_size(
     libevt_strings_array_t *strings_array,
     int string_index,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevt_strings_array_get_utf8_string(
     libevt_strings_array_t *strings_array,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libevt_strings_array_get_utf16_string_size(
     libevt_strings_array_t *strings_array,
     int string_index,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevt_strings_array_get_utf16_string(
     libevt_strings_array_t *strings_array,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVT_STRINGS_ARRAY_H ) */

