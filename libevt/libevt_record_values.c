/*
 * Record values functions
 *
 * Copyright (c) 2011-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>
#include <libnotify.h>

#include "libevt_debug.h"
#include "libevt_io_handle.h"
#include "libevt_libbfio.h"
#include "libevt_libfdatetime.h"
#include "libevt_libfvalue.h"
#include "libevt_libfwnt.h"
#include "libevt_record_values.h"

#include "evt_file_header.h"
#include "evt_record.h"

const uint8_t evt_end_of_file_record_signature1[ 4 ] = { 0x11, 0x11, 0x11, 0x11 };
const uint8_t evt_end_of_file_record_signature2[ 4 ] = { 0x22, 0x22, 0x22, 0x22 };
const uint8_t evt_end_of_file_record_signature3[ 4 ] = { 0x33, 0x33, 0x33, 0x33 };
const uint8_t evt_end_of_file_record_signature4[ 4 ] = { 0x44, 0x44, 0x44, 0x44 };

/* Initialize the record_values
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_initialize(
     libevt_record_values_t **record_values,
     liberror_error_t **error )
{
	static char *function = "libevt_record_values_initialize";

	if( record_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( *record_values != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record values value already set.",
		 function );

		return( -1 );
	}
	*record_values = memory_allocate_structure(
	                  libevt_record_values_t );

	if( *record_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create record values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *record_values,
	     0,
	     sizeof( libevt_record_values_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear record values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *record_values != NULL )
	{
		memory_free(
		 *record_values );

		*record_values = NULL;
	}
	return( -1 );
}

/* Frees the record values including elements
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_free(
     libevt_record_values_t **record_values,
     liberror_error_t **error )
{
	static char *function = "libevt_record_values_free";
	int result            = 1;

	if( record_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( *record_values != NULL )
	{
		if( ( *record_values )->source_name != NULL )
		{
			if( libfvalue_value_free(
			     &( ( *record_values )->source_name ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free source name value.",
				 function );

				result = -1;
			}
		}
		if( ( *record_values )->computer_name != NULL )
		{
			if( libfvalue_value_free(
			     &( ( *record_values )->computer_name ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free computer name value.",
				 function );

				result = -1;
			}
		}
		if( ( *record_values )->strings != NULL )
		{
			if( libfvalue_value_free(
			     &( ( *record_values )->strings ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free strings.",
				 function );

				result = -1;
			}
		}
		if( ( *record_values )->data != NULL )
		{
			if( libfvalue_value_free(
			     &( ( *record_values )->data ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *record_values );

		*record_values = NULL;
	}
	return( result );
}

/* Clones the record values
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_clone(
     libevt_record_values_t **destination_record_values,
     libevt_record_values_t *source_record_values,
     liberror_error_t **error )
{
	static char *function = "libevt_record_values_clone";

	if( destination_record_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination record values.",
		 function );

		return( -1 );
	}
	if( *destination_record_values != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination record values value already set.",
		 function );

		return( -1 );
	}
	if( source_record_values == NULL )
	{
		*destination_record_values = NULL;

		return( 1 );
	}
	*destination_record_values = memory_allocate_structure(
	                              libevt_record_values_t );

	if( *destination_record_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination record values.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_record_values,
	     source_record_values,
	     sizeof( libevt_record_values_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination record values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_record_values != NULL )
	{
		memory_free(
		 *destination_record_values );

		*destination_record_values = NULL;
	}
	return( -1 );
}

/* Reads a record_values
 * Returns the number of bytes read if successful or -1 on error
 */
ssize_t libevt_record_values_read(
         libevt_record_values_t *record_values,
         libbfio_handle_t *file_io_handle,
         libevt_io_handle_t *io_handle,
         off64_t *file_offset,
         liberror_error_t **error )
{
	uint8_t record_size_data[ 4 ];

	uint8_t *record_data      = NULL;
	static char *function     = "libevt_record_values_read";
	size_t read_size          = 0;
	size_t record_data_offset = 0;
	ssize_t read_count        = 0;
	ssize_t total_read_count  = 0;
	uint32_t record_data_size = 0;

	if( record_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( file_offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file offset.",
		 function );

		return( -1 );
	}
	read_count = libbfio_handle_read(
	              file_io_handle,
	              record_size_data,
	              sizeof( uint32_t ),
	              error );

	if( read_count != (ssize_t) sizeof( uint32_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record size data.",
		 function );

		goto on_error;
	}
	*file_offset    += read_count;
	total_read_count = read_count;

	byte_stream_copy_to_uint32_little_endian(
	 record_size_data,
	 record_data_size );

	if( record_data_size < 4 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: record data size value out of bounds.",
		 function );

		goto on_error;
	}
#if SIZEOF_SIZE_T <= 4
	if( (size_t) record_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid record data size value exceeds maximum.",
		 function );

		goto on_error;
	}
#endif
	/* Allocating record data as 4 bytes and then using realloc here
	 * corrupts the memory
	 */
	record_data = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * record_data_size );

	if( record_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create record data.",
		 function );

		goto on_error;
	}
	byte_stream_copy_from_uint32_little_endian(
	 record_data,
	 record_data_size );

	record_data_offset = 4;

	read_size = record_data_size - record_data_offset;

	if( ( (size64_t) *file_offset + read_size ) > io_handle->file_size )
	{
		read_size = io_handle->file_size - *file_offset;
	}
	read_count = libbfio_handle_read(
	              file_io_handle,
	              &( record_data[ record_data_offset ] ),
	              read_size,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record data.",
		 function );

		goto on_error;
	}
	*file_offset       += read_count;
	record_data_offset += read_count;
	total_read_count   += read_count;

	if( record_data_offset < (size_t) record_data_size )
	{
		if( libbfio_handle_seek_offset(
		     file_io_handle,
		     (off64_t) sizeof( evt_file_header_t ),
		     SEEK_SET,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek file header offset: %" PRIzd ".",
			 function,
			 sizeof( evt_file_header_t ) );

			goto on_error;
		}
		*file_offset = (off64_t) sizeof( evt_file_header_t );

		read_size = (size_t) record_data_size - record_data_offset;

		read_count = libbfio_handle_read(
		              file_io_handle,
		              &( record_data[ record_data_offset ] ),
		              read_size,
		              error );

		if( read_count != (ssize_t) read_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read record data.",
			 function );

			goto on_error;
		}
		*file_offset     += read_count;
		total_read_count += read_count;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: record data:\n",
		 function );
		libnotify_print_data(
		 record_data,
		 (size_t) record_data_size,
		 0 );
	}
#endif
	if( memory_compare(
	     &( record_data[ 4 ] ),
	     evt_file_signature,
	     4 ) == 0 )
	{
		record_values->type = LIBEVT_RECORD_TYPE_EVENT;
	}
	else if( memory_compare(
	          &( record_data[ 4 ] ),
	          evt_end_of_file_record_signature1,
	          4 ) == 0 )
	{
		record_values->type = LIBEVT_RECORD_TYPE_END_OF_FILE;
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record values signature.",
		 function );

		goto on_error;
	}
	if( record_values->type == LIBEVT_RECORD_TYPE_EVENT )
	{
		if( libevt_record_values_read_event(
		     record_values,
		     record_data,
		     (size_t) record_data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read event record values.",
			 function );

			goto on_error;
		}
	}
	else if( record_values->type == LIBEVT_RECORD_TYPE_END_OF_FILE )
	{
		if( libevt_record_values_read_end_of_file(
		     record_values,
		     record_data,
		     (size_t) record_data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read end of file record values.",
			 function );

			goto on_error;
		}
	}
	memory_free(
	 record_data );

	return( total_read_count );

on_error:
	if( record_data != NULL )
	{
		memory_free(
		 record_data );
	}
	return( -1 );
}

/* Reads the event record values
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_read_event(
     libevt_record_values_t *record_values,
     uint8_t *record_data,
     size_t record_data_size,
     liberror_error_t **error )
{
	static char *function                 = "libevt_record_values_read_event";
	size_t record_data_offset             = 0;
	size_t value_data_size                = 0;
	uint32_t data_offset                  = 0;
	uint32_t data_size                    = 0;
	uint32_t members_data_size            = 0;
	uint32_t size                         = 0;
	uint32_t size_copy                    = 0;
	uint32_t strings_offset               = 0;
	uint32_t strings_size                 = 0;
	uint32_t user_sid_offset              = 0;
	uint32_t user_sid_size                = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcstring_system_character_t posix_time_string[ 32 ];
	libcstring_system_character_t sid_string[ 128 ];

	libfdatetime_posix_time_t *posix_time = NULL;
	libfwnt_security_identifier_t *sid    = NULL;
	size_t sid_string_size                = 0;
	uint32_t value_32bit                  = 0;
	uint16_t value_16bit                  = 0;
	int result                            = 0;
#endif

	if( record_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record data.",
		 function );

		return( -1 );
	}
	if( record_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid record data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( record_data_size < ( sizeof( evt_record_event_header_t ) + 4 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: record data size value out of bounds.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_record_event_header_t *) record_data )->size,
	 size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_record_event_header_t *) record_data )->record_number,
	 record_values->number );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_record_event_header_t *) record_data )->creation_time,
	 record_values->creation_time );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_record_event_header_t *) record_data )->written_time,
	 record_values->written_time );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_record_event_header_t *) record_data )->event_identifier,
	 record_values->event_identifier );

	byte_stream_copy_to_uint16_little_endian(
	 ( (evt_record_event_header_t *) record_data )->event_type,
	 record_values->event_type );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_record_event_header_t *) record_data )->strings_offset,
	 strings_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_record_event_header_t *) record_data )->user_sid_size,
	 user_sid_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_record_event_header_t *) record_data )->user_sid_offset,
	 user_sid_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_record_event_header_t *) record_data )->data_size,
	 data_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_record_event_header_t *) record_data )->data_offset,
	 data_offset );

	byte_stream_copy_to_uint32_little_endian(
	 &( record_data[ record_data_size - 4 ] ),
	 size_copy );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 size );

		libnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (evt_record_event_header_t *) record_data )->signature[ 0 ],
		 ( (evt_record_event_header_t *) record_data )->signature[ 1 ],
		 ( (evt_record_event_header_t *) record_data )->signature[ 2 ],
		 ( (evt_record_event_header_t *) record_data )->signature[ 3 ] );

		libnotify_printf(
		 "%s: record number\t\t\t\t: %" PRIu32 "\n",
		 function,
		 record_values->number );

		if( libfdatetime_posix_time_initialize(
		     &posix_time,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create POSIX time.",
			 function );

			goto on_error;
		}
		if( libfdatetime_posix_time_copy_from_byte_stream(
		     posix_time,
		     ( (evt_record_event_header_t *) record_data )->creation_time,
		     4,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy POSIX time from byte stream.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_posix_time_copy_to_utf16_string(
			  posix_time,
			  (uint16_t *) posix_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  LIBFDATETIME_DATE_TIME_FORMAT_CTIME,
			  error );
#else
		result = libfdatetime_posix_time_copy_to_utf8_string(
			  posix_time,
			  (uint8_t *) posix_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  LIBFDATETIME_DATE_TIME_FORMAT_CTIME,
			  error );
#endif
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy POSIX time to string.",
			 function );

			goto on_error;
		}
		libnotify_printf(
		 "%s: creation time\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
		 function,
		 posix_time_string );

		if( libfdatetime_posix_time_copy_from_byte_stream(
		     posix_time,
		     ( (evt_record_event_header_t *) record_data )->written_time,
		     4,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy POSIX time from byte stream.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_posix_time_copy_to_utf16_string(
			  posix_time,
			  (uint16_t *) posix_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  LIBFDATETIME_DATE_TIME_FORMAT_CTIME,
			  error );
#else
		result = libfdatetime_posix_time_copy_to_utf8_string(
			  posix_time,
			  (uint8_t *) posix_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  LIBFDATETIME_DATE_TIME_FORMAT_CTIME,
			  error );
#endif
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy POSIX time to string.",
			 function );

			goto on_error;
		}
		libnotify_printf(
		 "%s: written time\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
		 function,
		 posix_time_string );

		if( libfdatetime_posix_time_free(
		     &posix_time,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free POSIX time.",
			 function );

			goto on_error;
		}
		libnotify_printf(
		 "%s: event identifier\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 record_values->event_identifier );

		libnotify_printf(
		 "%s: event identifier: code\t\t\t: %" PRIu32 "\n",
		 function,
		 record_values->event_identifier & 0x0000ffffUL );

		libnotify_printf(
		 "%s: event identifier: facility\t\t: %" PRIu32 "\n",
		 function,
		 ( record_values->event_identifier & 0x0fff0000UL ) >> 16 );

		libnotify_printf(
		 "%s: event identifier: reserved\t\t: %" PRIu32 "\n",
		 function,
		 ( record_values->event_identifier & 0x10000000UL ) >> 28 );

		libnotify_printf(
		 "%s: event identifier: customer flags\t: %" PRIu32 "\n",
		 function,
		 ( record_values->event_identifier & 0x20000000UL ) >> 29 );

		libnotify_printf(
		 "%s: event identifier: severity\t\t: %" PRIu32 " (",
		 function,
		 ( record_values->event_identifier & 0xc0000000UL ) >> 30 );
		libevt_debug_print_event_identifier_severity(
		 record_values->event_identifier );
		libnotify_printf(
		 ")\n" );

		libnotify_printf(
		 "%s: event type\t\t\t\t: %" PRIu16 " (",
		 function,
		 record_values->event_type );
		libevt_debug_print_event_type(
		 record_values->event_type );
		libnotify_printf(
		 ")\n" );

		byte_stream_copy_to_uint16_little_endian(
		 ( (evt_record_event_header_t *) record_data )->number_of_strings,
		 value_16bit );
		libnotify_printf(
		 "%s: number of strings\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (evt_record_event_header_t *) record_data )->event_category,
		 value_16bit );
		libnotify_printf(
		 "%s: event category\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (evt_record_event_header_t *) record_data )->event_flags,
		 value_16bit );
		libnotify_printf(
		 "%s: event flags\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_event_header_t *) record_data )->closing_record_number,
		 value_32bit );
		libnotify_printf(
		 "%s: closing record values number\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "%s: strings offset\t\t\t\t: %" PRIu32 "\n",
		 function,
		 strings_offset );

		libnotify_printf(
		 "%s: user security identifier (SID) size\t: %" PRIu32 "\n",
		 function,
		 user_sid_size );

		libnotify_printf(
		 "%s: user security identifier (SID) offset\t: %" PRIu32 "\n",
		 function,
		 user_sid_offset );

		libnotify_printf(
		 "%s: data size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 data_size );

		libnotify_printf(
		 "%s: data offset\t\t\t\t: %" PRIu32 "\n",
		 function,
		 data_offset );
	}
#endif
	record_data_offset = sizeof( evt_record_event_header_t );

	if( ( user_sid_offset == 0 )
	 && ( user_sid_size != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: user SID offset or size value out of bounds.",
		 function );

		goto on_error;
	}
	if( user_sid_offset != 0 )
	{
		if( ( (size_t) user_sid_offset < record_data_offset )
		 || ( (size_t) user_sid_offset >= ( record_data_size - 4 ) ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: user SID offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( user_sid_size != 0 )
		{
			if( (size_t) ( user_sid_offset + user_sid_size ) >= ( record_data_size - 4 ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: user SID size value out of bounds.",
				 function );

				goto on_error;
			}
		}
	}
/* TODO
	if( strings_offset != 0 )
*/
	{
		if( ( (size_t) strings_offset < user_sid_offset )
		 || ( (size_t) strings_offset >= ( record_data_size - 4 ) ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: strings offset value out of bounds.",
			 function );

			goto on_error;
		}
	}
	if( ( (size_t) data_offset < strings_offset )
	 || ( (size_t) data_offset >= ( record_data_size - 4 ) ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: data offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( user_sid_offset != 0 )
	{
		members_data_size = user_sid_offset - record_data_offset;
	}
	else if( strings_offset != 0 )
	{
		members_data_size = strings_offset - record_data_offset;
	}
	strings_size = data_offset - strings_offset;

	if( data_size != 0 )
	{
		if( (size_t) ( data_offset + data_size ) >= ( record_data_size - 4 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: data size value out of bounds.",
			 function );

			goto on_error;
		}
	}
	if( members_data_size != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: members data:\n",
			 function );
			libnotify_print_data(
			 &( record_data[ record_data_offset ] ),
			 members_data_size,
			 0 );
		}
#endif
		if( libfvalue_value_initialize(
		     &( record_values->source_name ),
		     LIBFVALUE_VALUE_TYPE_STRING_UTF16,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create source name value.",
			 function );

			goto on_error;
		}
		if( libfvalue_value_set_data_string(
		     record_values->source_name,
		     &( record_data[ record_data_offset ] ),
		     members_data_size,
		     LIBFVALUE_ENDIAN_LITTLE,
		     LIBFVALUE_VALUE_DATA_FLAG_MANAGED,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set data of source name value.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: source name\t\t\t\t: ",
			 function );

			if( libfvalue_debug_print_value(
			     record_values->source_name,
			     0,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print source name value.",
				 function );

				goto on_error;
			}
			libnotify_printf(
			 "\n" );
		}
#endif
		if( libfvalue_value_get_data_size(
		     record_values->source_name,
		     &value_data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to retrieve data size of source name value.",
			 function );

			goto on_error;
		}
		record_data_offset += value_data_size;
		members_data_size  -= (uint32_t) value_data_size;

		if( libfvalue_value_initialize(
		     &( record_values->computer_name ),
		     LIBFVALUE_VALUE_TYPE_STRING_UTF16,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create computer name value.",
			 function );

			goto on_error;
		}
		if( libfvalue_value_set_data_string(
		     record_values->computer_name,
		     &( record_data[ record_data_offset ] ),
		     members_data_size,
		     LIBFVALUE_ENDIAN_LITTLE,
		     LIBFVALUE_VALUE_DATA_FLAG_MANAGED,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set data of computer name value.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: computer name\t\t\t\t: ",
			 function );

			if( libfvalue_debug_print_value(
			     record_values->computer_name,
			     0,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print computer name value.",
				 function );

				goto on_error;
			}
			libnotify_printf(
			 "\n" );
		}
#endif
		if( libfvalue_value_get_data_size(
		     record_values->computer_name,
		     &value_data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to retrieve data size of computer name value.",
			 function );

			goto on_error;
		}
		record_data_offset += value_data_size;
		members_data_size  -= (uint32_t) value_data_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			if( members_data_size > 0 )
			{
				libnotify_printf(
				 "%s: members trailing data:\n",
				 function );
				libnotify_print_data(
				 &( record_data[ record_data_offset ] ),
				 members_data_size,
				 0 );
			}
		}
#endif
	}
	if( user_sid_size != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			if( libfwnt_security_identifier_initialize(
			     &sid,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create SID.",
				 function );

				goto on_error;
			}
			if( libfwnt_security_identifier_copy_from_byte_stream(
			     sid,
			     &( record_data[ user_sid_offset ] ),
			     (size_t) user_sid_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy byte stream to SID.",
				 function );

				goto on_error;
			}
			result = libfwnt_security_identifier_get_string_size(
				  sid,
				  &sid_string_size,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve SID string size.",
				 function );

				goto on_error;
			}
			/* It is assumed that the SID string cannot be larger than 127 characters
			 * otherwise using dynamic allocation is more appropriate
			 */
			if( sid_string_size > 128 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: SID string size value exceeds maximum.",
				 function );

				goto on_error;
			}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfwnt_security_identifier_copy_to_utf16_string(
				  sid,
				  (uint16_t *) sid_string,
				  128,
				  error );
#else
			result = libfwnt_security_identifier_copy_to_utf8_string(
				  sid,
				  (uint8_t *) sid_string,
				  128,
				  error );
#endif
			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy SID to string.",
				 function );

				goto on_error;
			}
			if( libfwnt_security_identifier_free(
			     &sid,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free SID.",
				 function );

				goto on_error;
			}
			libnotify_printf(
			 "%s: user security identifier (SID)\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
			 function,
			 sid_string );
		}
#endif
		record_data_offset += user_sid_size;
	}
	if( strings_size != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: strings data:\n",
			 function );
			libnotify_print_data(
			 &( record_data[ strings_offset ] ),
			 strings_size,
			 0 );
		}
#endif
		if( libfvalue_value_initialize(
		     &( record_values->strings ),
		     LIBFVALUE_VALUE_TYPE_STRING_UTF16,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create strings value.",
			 function );

			goto on_error;
		}
		if( libfvalue_value_set_data_strings_array(
		     record_values->strings,
		     &( record_data[ record_data_offset ] ),
		     strings_size,
		     LIBFVALUE_ENDIAN_LITTLE,
		     LIBFVALUE_VALUE_DATA_FLAG_MANAGED,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set data of strings value.",
			 function );

			goto on_error;
		}
		record_data_offset += strings_size;
	}
	if( data_size != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: data:\n",
			 function );
			libnotify_print_data(
			 &( record_data[ data_offset ] ),
			 (size_t) data_size,
			 0 );
		}
#endif
		if( libfvalue_value_initialize(
		     &( record_values->data ),
		     LIBFVALUE_VALUE_TYPE_BINARY_DATA,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data value.",
			 function );

			goto on_error;
		}
		if( libfvalue_value_set_data(
		     record_values->data,
		     &( record_data[ record_data_offset ] ),
		     (size_t) data_size,
		     LIBFVALUE_ENDIAN_LITTLE,
		     LIBFVALUE_VALUE_DATA_FLAG_MANAGED,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set data of data value.",
			 function );

			goto on_error;
		}
		record_data_offset += data_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( record_data_offset < ( record_data_size - 4 ) )
		{
			libnotify_printf(
			 "%s: padding:\n",
			 function );
			libnotify_print_data(
			 &( record_data[ record_data_offset ] ),
			 (size_t) record_data_size - record_data_offset - 4,
			 0 );
		}
		libnotify_printf(
		 "%s: size copy\t\t\t\t: %" PRIu32 "\n",
		 function,
		 size_copy );

		libnotify_printf(
		 "\n" );
	}
#endif
	if( size != size_copy )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for size and size copy.",
		 function );

		goto on_error;
	}
	if( record_data_size != (size_t) size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for record_values data size and size.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( posix_time != NULL )
	{
		libfdatetime_posix_time_free(
		 &posix_time,
		 NULL );
	}
	if( sid != NULL )
	{
		libfwnt_security_identifier_free(
		 &sid,
		 NULL );
	}
#endif
	if( record_values->data != NULL )
	{
		libfvalue_value_free(
		 &( record_values->data ),
		 NULL );
	}
	if( record_values->strings != NULL )
	{
		libfvalue_value_free(
		 &( record_values->strings ),
		 NULL );
	}
	if( record_values->computer_name != NULL )
	{
		libfvalue_value_free(
		 &( record_values->computer_name ),
		 NULL );
	}
	if( record_values->source_name != NULL )
	{
		libfvalue_value_free(
		 &( record_values->source_name ),
		 NULL );
	}
	return( -1 );
}

/* Reads the end of file record values
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_read_end_of_file(
     libevt_record_values_t *record_values,
     uint8_t *record_data,
     size_t record_data_size,
     liberror_error_t **error )
{
	static char *function = "libevt_record_values_read_event";
	uint32_t size         = 0;
	uint32_t size_copy    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
#endif

	if( record_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record data.",
		 function );

		return( -1 );
	}
	if( record_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid record data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( record_data_size < sizeof( evt_record_end_of_file_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: record data size value out of bounds.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_record_end_of_file_t *) record_data )->size,
	 size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_record_end_of_file_t *) record_data )->size_copy,
	 size_copy );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->signature1,
		 value_32bit );
		libnotify_printf(
		 "%s: signature1\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->signature2,
		 value_32bit );
		libnotify_printf(
		 "%s: signature2\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->signature3,
		 value_32bit );
		libnotify_printf(
		 "%s: signature3\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->signature4,
		 value_32bit );
		libnotify_printf(
		 "%s: signature4\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->first_record_offset,
		 value_32bit );
		libnotify_printf(
		 "%s: first record offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->end_of_file_record_offset,
		 value_32bit );
		libnotify_printf(
		 "%s: end of file record offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->last_record_number,
		 value_32bit );
		libnotify_printf(
		 "%s: last record number\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->first_record_number,
		 value_32bit );
		libnotify_printf(
		 "%s: first record number\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "%s: size copy\t\t\t\t: %" PRIu32 "\n",
		 function,
		 size_copy );

		libnotify_printf(
		 "\n" );
	}
#endif
	if( size != size_copy )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for size and size copy.",
		 function );

		return( -1 );
	}
	if( record_data_size != (size_t) size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for record data size and size.",
		 function );

		return( -1 );
	}
/* TODO correct values in IO handle if necessary */

	return( 1 );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_type(
     libevt_record_values_t *record_values,
     uint8_t *type,
     liberror_error_t **error )
{
	static char *function = "libevt_record_values_get_type";

	if( record_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( type == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	*type = record_values->type;

	return( 1 );
}

