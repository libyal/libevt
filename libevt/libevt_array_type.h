/* 
 * Array type functions
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

#if !defined( _LIBEVT_ARRAY_TYPE_H )
#define _LIBEVT_ARRAY_TYPE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* The array comparison definitions
 */
enum LIBEVT_ARRAY_COMPARE_DEFINITIONS
{
	/* The first value is less than the second value
	 */
        LIBEVT_ARRAY_COMPARE_LESS,

	/* The first and second values are equal
	 */
        LIBEVT_ARRAY_COMPARE_EQUAL,

	/* The first value is greater than the second value
	 */
        LIBEVT_ARRAY_COMPARE_GREATER
};

/* The array insert flag definitions
 */
enum LIBEVT_ARRAY_INSERT_FLAGS
{
	/* Allow duplicate entries
	 */
	LIBEVT_ARRAY_INSERT_FLAG_NON_UNIQUE_ENTRIES	= 0x00,

	/* Only allow unique entries, no duplicates
	 */
	LIBEVT_ARRAY_INSERT_FLAG_UNIQUE_ENTRIES		= 0x01,
};

typedef struct libevt_array libevt_array_t;

struct libevt_array
{
	/* The number of allocated entries
	 */
	int number_of_allocated_entries;

	/* The number of entries
	 */
	int number_of_entries;

	/* The entries
	 */
	intptr_t **entries;
};

int libevt_array_initialize(
     libevt_array_t **array,
     int number_of_entries,
     liberror_error_t **error );

int libevt_array_free(
     libevt_array_t **array,
     int (*entry_free_function)(
            intptr_t **entry,
            liberror_error_t **error ),
     liberror_error_t **error );

int libevt_array_empty(
     libevt_array_t *array,
     int (*entry_free_function)(
            intptr_t **entry,
            liberror_error_t **error ),
     liberror_error_t **error );

int libevt_array_clear(
     libevt_array_t *array,
     int (*entry_free_function)(
            intptr_t **entry,
            liberror_error_t **error ),
     liberror_error_t **error );

int libevt_array_clone(
     libevt_array_t **destination_array,
     libevt_array_t *source_array,
     int (*entry_free_function)(
            intptr_t **entry,
            liberror_error_t **error ),
     int (*entry_clone_function)(
            intptr_t **destination,
            intptr_t *source,
            liberror_error_t **error ),
     liberror_error_t **error );

int libevt_array_resize(
     libevt_array_t *array,
     int number_of_entries,
     int (*entry_free_function)(
            intptr_t **entry,
            liberror_error_t **error ),
     liberror_error_t **error );

int libevt_array_get_number_of_entries(
     libevt_array_t *array,
     int *number_of_entries,
     liberror_error_t **error );

int libevt_array_get_entry_by_index(
     libevt_array_t *array,
     int entry_index,
     intptr_t **entry,
     liberror_error_t **error );

int libevt_array_set_entry_by_index(
     libevt_array_t *array,
     int entry_index,
     intptr_t *entry,
     liberror_error_t **error );

int libevt_array_append_entry(
     libevt_array_t *array,
     int *entry_index,
     intptr_t *entry,
     liberror_error_t **error );

int libevt_array_insert_entry(
     libevt_array_t *array,
     int *entry_index,
     intptr_t *entry,
     int (*entry_compare_function)(
            intptr_t *first_entry,
            intptr_t *second_entry,
            liberror_error_t **error ),
     uint8_t insert_flags,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

