/*
 * OSS-Fuzz target for libevt record type
 *
 * Copyright (C) 2011-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libevt.h"

#define MAXIMUM_STRING_SIZE	8 * 1024 * 1024

#if !defined( LIBEVT_HAVE_BFIO )

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_open_file_io_handle(
     libevt_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libevt_error_t **error );

#endif /* !defined( LIBEVT_HAVE_BFIO ) */

static void fuzzer_fetch_record_properties(
     libevt_record_t *record )
{
	uint16_t *utf16_string    = NULL;
	uint8_t *data             = NULL;
	uint8_t *utf8_string      = NULL;
	off64_t offset            = 0;
	uint16_t event_category   = 0;
	uint16_t event_type       = 0;
	uint32_t event_identifier = 0;
	uint32_t identifier       = 0;
	uint32_t posix_time       = 0;
	size_t data_size          = 0;
	size_t utf16_string_size  = 0;
	size_t utf8_string_size   = 0;
	int number_of_strings     = 0;
	int result                = 0;
	int string_index          = 0;

	if( record == NULL )
	{
		return;
	}
	libevt_record_get_offset(
	 record,
	 &offset,
	 NULL );

	libevt_record_get_identifier(
	 record,
	 &identifier,
	 NULL );

	libevt_record_get_creation_time(
	 record,
	 &posix_time,
	 NULL );

	libevt_record_get_written_time(
	 record,
	 &posix_time,
	 NULL );

	libevt_record_get_event_identifier(
	 record,
	 &event_identifier,
	 NULL );

	libevt_record_get_event_type(
	 record,
	 &event_type,
	 NULL );

	libevt_record_get_event_category(
	 record,
	 &event_category,
	 NULL );

	result = libevt_record_get_utf8_source_name_size(
	          record,
	          &utf8_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf8_string_size > 0 )
	 && ( utf8_string_size < MAXIMUM_STRING_SIZE ) )
	{
		utf8_string = (uint8_t *) malloc( utf8_string_size );

		if( utf8_string != NULL )
		{
			libevt_record_get_utf8_source_name(
			 record,
			 utf8_string,
			 utf8_string_size,
			 NULL );
			free( utf8_string );

			utf8_string = NULL;
		}
	}
	result = libevt_record_get_utf16_source_name_size(
	          record,
	          &utf16_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf16_string_size > 0 )
	 && ( utf16_string_size < MAXIMUM_STRING_SIZE ) )
	{
		utf16_string = (uint16_t *) malloc( utf16_string_size * sizeof( uint16_t ) );

		if( utf16_string != NULL )
		{
			libevt_record_get_utf16_source_name(
			 record,
			 utf16_string,
			 utf16_string_size,
			 NULL );
			free( utf16_string );

			utf16_string = NULL;
		}
	}
	result = libevt_record_get_utf8_computer_name_size(
	          record,
	          &utf8_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf8_string_size > 0 )
	 && ( utf8_string_size < MAXIMUM_STRING_SIZE ) )
	{
		utf8_string = (uint8_t *) malloc( utf8_string_size );

		if( utf8_string != NULL )
		{
			libevt_record_get_utf8_computer_name(
			 record,
			 utf8_string,
			 utf8_string_size,
			 NULL );
			free( utf8_string );

			utf8_string = NULL;
		}
	}
	result = libevt_record_get_utf16_computer_name_size(
	          record,
	          &utf16_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf16_string_size > 0 )
	 && ( utf16_string_size < MAXIMUM_STRING_SIZE ) )
	{
		utf16_string = (uint16_t *) malloc( utf16_string_size * sizeof( uint16_t ) );

		if( utf16_string != NULL )
		{
			libevt_record_get_utf16_computer_name(
			 record,
			 utf16_string,
			 utf16_string_size,
			 NULL );
			free( utf16_string );

			utf16_string = NULL;
		}
	}
	result = libevt_record_get_utf8_user_security_identifier_size(
	          record,
	          &utf8_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf8_string_size > 0 )
	 && ( utf8_string_size < MAXIMUM_STRING_SIZE ) )
	{
		utf8_string = (uint8_t *) malloc( utf8_string_size );

		if( utf8_string != NULL )
		{
			libevt_record_get_utf8_user_security_identifier(
			 record,
			 utf8_string,
			 utf8_string_size,
			 NULL );
			free( utf8_string );

			utf8_string = NULL;
		}
	}
	result = libevt_record_get_utf16_user_security_identifier_size(
	          record,
	          &utf16_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf16_string_size > 0 )
	 && ( utf16_string_size < MAXIMUM_STRING_SIZE ) )
	{
		utf16_string = (uint16_t *) malloc( utf16_string_size * sizeof( uint16_t ) );

		if( utf16_string != NULL )
		{
			libevt_record_get_utf16_user_security_identifier(
			 record,
			 utf16_string,
			 utf16_string_size,
			 NULL );
			free( utf16_string );

			utf16_string = NULL;
		}
	}
	result = libevt_record_get_number_of_strings(
	          record,
	          &number_of_strings,
	          NULL );

	if( ( result == 1 )
	 && ( number_of_strings > 0 ) )
	{
		for( string_index = 0;
		     string_index < number_of_strings;
		     string_index++ )
		{
			result = libevt_record_get_utf8_string_size(
			          record,
			          string_index,
			          &utf8_string_size,
			          NULL );

			if( ( result == 1 )
			 && ( utf8_string_size > 0 )
			 && ( utf8_string_size < MAXIMUM_STRING_SIZE ) )
			{
				utf8_string = (uint8_t *) malloc( utf8_string_size );

				if( utf8_string != NULL )
				{
					libevt_record_get_utf8_string(
					 record,
					 string_index,
					 utf8_string,
					 utf8_string_size,
					 NULL );
					free( utf8_string );

					utf8_string = NULL;
				}
			}
			result = libevt_record_get_utf16_string_size(
			          record,
			          string_index,
			          &utf16_string_size,
			          NULL );

			if( ( result == 1 )
			 && ( utf16_string_size > 0 )
			 && ( utf16_string_size < MAXIMUM_STRING_SIZE ) )
			{
				utf16_string = (uint16_t *) malloc( utf16_string_size * sizeof( uint16_t ) );

				if( utf16_string != NULL )
				{
					libevt_record_get_utf16_string(
					 record,
					 string_index,
					 utf16_string,
					 utf16_string_size,
					 NULL );
					free( utf16_string );

					utf16_string = NULL;
				}
			}
		}
	}
	result = libevt_record_get_data_size(
	          record,
	          &data_size,
	          NULL );

	if( ( result == 1 )
	 && ( data_size > 0 )
	 && ( data_size < MAXIMUM_STRING_SIZE ) )
	{
		data = (uint8_t *) malloc( data_size );

		if( data != NULL )
		{
			libevt_record_get_data(
			 record,
			 data,
			 data_size,
			 NULL );
			free( data );

			data = NULL;
		}
	}
}

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	libbfio_handle_t *file_io_handle = NULL;
	libevt_file_t *file              = NULL;
	libevt_record_t *record          = NULL;
	int number_of_records            = 0;
	int record_index                 = 0;
	int result                       = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libevt_file_initialize(
	     &file,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libevt_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     LIBEVT_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libevt_file;
	}
	result = libevt_file_get_number_of_records(
	          file,
	          &number_of_records,
	          NULL );
       
	if( ( result == 1 )
	 && ( number_of_records > 0 ) )
	{
		for( record_index = 0;
		     record_index < number_of_records;
		     record_index++ )
		{
			if( libevt_file_get_record_by_index(
			     file,
			     record_index,
			     &record,
			     NULL ) != 1 )
			{
				continue;
			}
			fuzzer_fetch_record_properties(
			 record );

			libevt_record_free(
			 &record,
			 NULL );
		}
		if( libevt_file_get_record_by_index(
		     file,
		     number_of_records,
		     &record,
		     NULL ) == 1 )
		{
			libevt_record_free(
			 &record,
			 NULL );
		}
	}
	libevt_file_close(
	 file,
	 NULL );

on_error_libevt_file:
	libevt_file_free(
	 &file,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );

	return( 0 );
}

} /* extern "C" */
