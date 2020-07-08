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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libevt_libcerror.h"
#include "libevt_libcnotify.h"
#include "libevt_libuna.h"
#include "libevt_strings_array.h"

/* Creates a strings array
 * Make sure the value strings_array is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevt_strings_array_initialize(
     libevt_strings_array_t **strings_array,
     libcerror_error_t **error )
{
	static char *function = "libevt_strings_array_initialize";

	if( strings_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid strings array.",
		 function );

		return( -1 );
	}
	if( *strings_array != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid strings array value already set.",
		 function );

		return( -1 );
	}
	*strings_array = memory_allocate_structure(
	                  libevt_strings_array_t );

	if( *strings_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create strings array.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *strings_array,
	     0,
	     sizeof( libevt_strings_array_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear strings array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *strings_array != NULL )
	{
		memory_free(
		 *strings_array );

		*strings_array = NULL;
	}
	return( -1 );
}

/* Frees a strings array
 * Returns 1 if successful or -1 on error
 */
int libevt_strings_array_free(
     libevt_strings_array_t **strings_array,
     libcerror_error_t **error )
{
	static char *function = "libevt_strings_array_free";

	if( strings_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid strings array.",
		 function );

		return( -1 );
	}
	if( *strings_array != NULL )
	{
		if( ( *strings_array )->string_sizes != NULL )
		{
			memory_free(
			 ( *strings_array )->string_sizes );
		}
		if( ( *strings_array )->strings != NULL )
		{
			memory_free(
			 ( *strings_array )->strings );
		}
		if( ( *strings_array )->strings_data != NULL )
		{
			memory_free(
			 ( *strings_array )->strings_data );
		}
		memory_free(
		 *strings_array );

		*strings_array = NULL;
	}
	return( 1 );
}

/* Reads the strings array data
 * Returns 1 if successful or -1 on error
 */
int libevt_strings_array_read_data(
     libevt_strings_array_t *strings_array,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_strings_array_read_data";
	size_t data_offset    = 0;
	size_t string_offset  = 0;
	int number_of_strings = 0;
	int string_index      = 0;

	if( strings_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid strings array.",
		 function );

		return( -1 );
	}
	if( strings_array->strings_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid strings array - strings data value already set.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < 2 )
	 || ( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( data_size % 2 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid data size - value must be a multitude of 2.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: strings array data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	for( data_offset = 0;
	     data_offset < data_size;
	     data_offset += 2 )
	{
		if( ( data[ data_offset ] == 0 )
		 && ( data[ data_offset + 1 ] == 0 ) )
		{
			number_of_strings++;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of strings\t\t\t: %d\n",
		 function,
		 number_of_strings );

		libcnotify_printf(
		 "\n" );
	}
#endif
	strings_array->strings_data = (uint8_t *) memory_allocate(
	                                           sizeof( uint8_t ) * data_size );

	if( strings_array->strings_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create strings data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     strings_array->strings_data,
	     data,
	     sizeof( uint8_t ) * data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy strings data.",
		 function );

		goto on_error;
	}
	strings_array->strings_data_size = data_size;

	strings_array->strings = (uint8_t **) memory_allocate(
	                                       sizeof( uint8_t* ) * number_of_strings );

	if( strings_array->strings == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create strings.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     strings_array->strings,
	     0,
	     sizeof( uint8_t* ) * number_of_strings ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear strings.",
		 function );

		goto on_error;
	}
	strings_array->string_sizes = (size_t *) memory_allocate(
	                                          sizeof( size_t ) * number_of_strings );

	if( strings_array->string_sizes == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string sizes.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     strings_array->string_sizes,
	     0,
	     sizeof( uint8_t* ) * number_of_strings ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear string sizes.",
		 function );

		goto on_error;
	}
	for( data_offset = 0;
	     data_offset < strings_array->strings_data_size;
	     data_offset += 2 )
	{
		if( ( strings_array->strings_data[ data_offset ] == 0 )
		 && ( strings_array->strings_data[ data_offset + 1 ] == 0 ) )
		{
			strings_array->strings[ string_index ]      = &( strings_array->strings_data[ string_offset ] );
			strings_array->string_sizes[ string_index ] = ( data_offset + 2 ) - string_offset;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: string: %d data:\n",
				 function,
				 string_index );
				libcnotify_print_data(
				 strings_array->strings[ string_index ],
				 strings_array->string_sizes[ string_index ],
				 0 );
			}
#endif
			string_offset = data_offset + 2;

			string_index++;
		}
	}
	strings_array->number_of_strings = number_of_strings;

	return( 1 );

on_error:
	if( strings_array->string_sizes != NULL )
	{
		memory_free(
		 strings_array->string_sizes );

		strings_array->string_sizes = NULL;
	}
	if( strings_array->strings != NULL )
	{
		memory_free(
		 strings_array->strings );

		strings_array->strings = NULL;
	}
	if( strings_array->strings_data != NULL )
	{
		memory_free(
		 strings_array->strings_data );

		strings_array->strings_data = NULL;
	}
	strings_array->strings_data_size = 0;

	return( -1 );
}

/* Retrieves the number of strings
 * Returns 1 if successful or -1 on error
 */
int libevt_strings_array_get_number_of_strings(
     libevt_strings_array_t *strings_array,
     int *number_of_strings,
     libcerror_error_t **error )
{
	static char *function = "libevt_strings_array_get_number_of_strings";

	if( strings_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid strings array.",
		 function );

		return( -1 );
	}
	if( number_of_strings == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of strings.",
		 function );

		return( -1 );
	}
	*number_of_strings = strings_array->number_of_strings;

	return( 1 );
}

/* Retrieves the size of a specific UTF-8 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_strings_array_get_utf8_string_size(
     libevt_strings_array_t *strings_array,
     int string_index,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_strings_array_get_utf8_string_size";

	if( strings_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( strings_array->strings == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid string index value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( string_index < 0 )
	 || ( string_index >= strings_array->number_of_strings ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid strings index value out of bounds.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     strings_array->strings[ string_index ],
	     strings_array->string_sizes[ string_index ],
	     LIBUNA_ENDIAN_LITTLE,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of UTF-8 string: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific UTF-8 encoded string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_strings_array_get_utf8_string(
     libevt_strings_array_t *strings_array,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_values_record_get_utf8_string";

	if( strings_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( strings_array->strings == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid string index value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( string_index < 0 )
	 || ( string_index >= strings_array->number_of_strings ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid strings index value out of bounds.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     (libuna_utf8_character_t *) utf8_string,
	     utf8_string_size,
	     strings_array->strings[ string_index ],
	     strings_array->string_sizes[ string_index ],
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-8 string: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of a specific UTF-16 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_strings_array_get_utf16_string_size(
     libevt_strings_array_t *strings_array,
     int string_index,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_strings_array_get_utf16_string_size";

	if( strings_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( strings_array->strings == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid string index value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( string_index < 0 )
	 || ( string_index >= strings_array->number_of_strings ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid strings index value out of bounds.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_size_from_utf16_stream(
	     strings_array->strings[ string_index ],
	     strings_array->string_sizes[ string_index ],
	     LIBUNA_ENDIAN_LITTLE,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of UTF-16 string: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific UTF-16 encoded string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_strings_array_get_utf16_string(
     libevt_strings_array_t *strings_array,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_values_record_get_utf16_string";

	if( strings_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( strings_array->strings == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid string index value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( string_index < 0 )
	 || ( string_index >= strings_array->number_of_strings ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid strings index value out of bounds.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     (libuna_utf16_character_t *) utf16_string,
	     utf16_string_size,
	     strings_array->strings[ string_index ],
	     strings_array->string_sizes[ string_index ],
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-16 string: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	return( 1 );
}

