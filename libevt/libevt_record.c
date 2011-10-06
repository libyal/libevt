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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libevt_io_handle.h"
#include "libevt_record.h"

#include "evt_record.h"

const uint8_t evt_end_of_file_record_signature1[ 4 ] = { 0x11, 0x11, 0x11, 0x11 };
const uint8_t evt_end_of_file_record_signature2[ 4 ] = { 0x22, 0x22, 0x22, 0x22 };
const uint8_t evt_end_of_file_record_signature3[ 4 ] = { 0x33, 0x33, 0x33, 0x33 };
const uint8_t evt_end_of_file_record_signature4[ 4 ] = { 0x44, 0x44, 0x44, 0x44 };

/* Initialize the record
 * Returns 1 if successful or -1 on error
 */
int libevt_record_initialize(
     libevt_record_t **record,
     liberror_error_t **error )
{
	static char *function = "libevt_record_initialize";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( *record == NULL )
	{
		*record = memory_allocate_structure(
		           libevt_record_t );

		if( *record == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create record.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     *record,
		     0,
		     sizeof( libevt_record_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear record.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *record != NULL )
	{
		memory_free(
		 *record );

		*record = NULL;
	}
	return( -1 );
}

/* Frees the record including elements
 * Returns 1 if successful or -1 on error
 */
int libevt_record_free(
     libevt_record_t *record,
     liberror_error_t **error )
{
	static char *function = "libevt_record_free";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( 1 );
	}
	memory_free(
	 record );

	return( 1 );
}

/* Clones the record
 * Returns 1 if successful or -1 on error
 */
int libevt_record_clone(
     libevt_record_t **destination_record,
     libevt_record_t *source_record,
     liberror_error_t **error )
{
	static char *function = "libevt_record_clone";

	if( destination_record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination record.",
		 function );

		return( -1 );
	}
	if( *destination_record != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination record value already set.",
		 function );

		return( -1 );
	}
	if( source_record == NULL )
	{
		*destination_record = NULL;

		return( 1 );
	}
	*destination_record = memory_allocate_structure(
			       libevt_record_t );

	if( *destination_record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination record.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_record,
	     source_record,
	     sizeof( libevt_record_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination record.",
		 function );

		memory_free(
		 *destination_record );

		*destination_record = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *destination_record != NULL )
	{
		memory_free(
		 *destination_record );

		*destination_record = NULL;
	}
	return( -1 );
}

/* Reads a record
 * Returns the number of bytes read if successful or -1 on error
 */
ssize_t libevt_record_read(
         libevt_record_t *record,
         libevt_io_handle_t *io_handle,
         libbfio_handle_t *file_io_handle,
         liberror_error_t **error )
{
	uint8_t *record_data     = NULL;
	static char *function    = "libevt_record_read";
	void *reallocation       = NULL;
	ssize_t read_count       = 0;
	ssize_t total_read_count = 0;
	uint32_t record_size     = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
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
	record_data = (uint8_t *) memory_allocate(
	                           sizeof( uint32_t ) );

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
	read_count = libbfio_handle_read(
	              file_io_handle,
	              record_data,
	              sizeof( uint32_t ),
	              error );

	if( read_count != (ssize_t) sizeof( uint32_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record size.",
		 function );

		goto on_error;
	}
	total_read_count = read_count;

	byte_stream_copy_to_uint32_little_endian(
	 record_data,
	 record_size );

	if( (size_t) record_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid record size value exceeds maximum.",
		 function );

		goto on_error;
	}
	reallocation = memory_reallocate(
			record_data,
			(size_t) record_size );

	if( reallocation == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to resize record data.",
		 function );

		goto on_error;
	}
	record_data = (uint8_t *) reallocation;

	read_count = libbfio_handle_read(
	              file_io_handle,
	              &( record_data[ 4 ] ),
	              record_size - 4,
	              error );

	if( read_count != (ssize_t) ( record_size - 4 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record size.",
		 function );

		goto on_error;
	}
	total_read_count += read_count;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: record data:\n",
		 function );
		libnotify_print_data(
		 record_data,
		 (size_t) record_size );
	}
#endif
	if( memory_compare(
	     &( record_data[ 4 ] ),
	     evt_file_signature,
	     4 ) == 0 )
	{
		record->type = LIBEVT_RECORD_TYPE_EVENT;
	}
	else if( memory_compare(
	          &( record_data[ 4 ] ),
	          evt_end_of_file_record_signature1,
	          4 ) == 0 )
	{
		record->type = LIBEVT_RECORD_TYPE_END_OF_FILE;
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record signature.",
		 function );

		goto on_error;
	}
	if( record->type == LIBEVT_RECORD_TYPE_EVENT )
	{
		if( libevt_record_read_event(
		     record,
		     io_handle,
		     record_data,
		     (size_t) record_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read event record.",
			 function );

			goto on_error;
		}
	}
	else if( record->type == LIBEVT_RECORD_TYPE_END_OF_FILE )
	{
		if( libevt_record_read_end_of_file(
		     record,
		     io_handle,
		     record_data,
		     (size_t) record_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read end of file record.",
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

/* Reads the event record
 * Returns 1 if successful or -1 on error
 */
int libevt_record_read_event(
     libevt_record_t *record,
     libevt_io_handle_t *io_handle,
     uint8_t *record_data,
     size_t record_data_size,
     liberror_error_t **error )
{
	static char *function = "libevt_record_read_event";
	uint32_t size         = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
	uint16_t value_16bit  = 0;
#endif

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
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
	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_record_event_header_t *) record_data )->size,
	 size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: size\t\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 size );

		libnotify_printf(
		 "%s: signature\t\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (evt_record_event_header_t *) record_data )->signature[ 0 ],
		 ( (evt_record_event_header_t *) record_data )->signature[ 1 ],
		 ( (evt_record_event_header_t *) record_data )->signature[ 2 ],
		 ( (evt_record_event_header_t *) record_data )->signature[ 3 ] );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_event_header_t *) record_data )->record_number,
		 value_32bit );
		libnotify_printf(
		 "%s: record number\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_event_header_t *) record_data )->creation_time,
		 value_32bit );
		libnotify_printf(
		 "%s: creation time\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_event_header_t *) record_data )->written_time,
		 value_32bit );
		libnotify_printf(
		 "%s: written time\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_event_header_t *) record_data )->event_identifier,
		 value_32bit );
		libnotify_printf(
		 "%s: event identifier\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (evt_record_event_header_t *) record_data )->event_type,
		 value_16bit );
		libnotify_printf(
		 "%s: event type\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (evt_record_event_header_t *) record_data )->number_of_strings,
		 value_16bit );
		libnotify_printf(
		 "%s: number of strings\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_event_header_t *) record_data )->closing_record_number,
		 value_32bit );
		libnotify_printf(
		 "%s: closing record number\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_event_header_t *) record_data )->strings_offset,
		 value_32bit );
		libnotify_printf(
		 "%s: strings offset\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_event_header_t *) record_data )->user_sid_size,
		 value_32bit );
		libnotify_printf(
		 "%s: used security identifier (SID) size\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_event_header_t *) record_data )->user_sid_offset,
		 value_32bit );
		libnotify_printf(
		 "%s: used security identifier (SID) offset\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads the end of file record
 * Returns 1 if successful or -1 on error
 */
int libevt_record_read_end_of_file(
     libevt_record_t *record,
     libevt_io_handle_t *io_handle,
     uint8_t *record_data,
     size_t record_data_size,
     liberror_error_t **error )
{
	static char *function = "libevt_record_read_event";
	uint32_t size         = 0;
	uint32_t size_copy    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
#endif

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
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
		 "%s: size\t\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->signature1,
		 value_32bit );
		libnotify_printf(
		 "%s: signature1\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->signature2,
		 value_32bit );
		libnotify_printf(
		 "%s: signature2\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->signature3,
		 value_32bit );
		libnotify_printf(
		 "%s: signature3\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->signature4,
		 value_32bit );
		libnotify_printf(
		 "%s: signature4\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->first_record_offset,
		 value_32bit );
		libnotify_printf(
		 "%s: first record offset\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->end_of_file_record_offset,
		 value_32bit );
		libnotify_printf(
		 "%s: end of file record offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->last_record_number,
		 value_32bit );
		libnotify_printf(
		 "%s: last record number\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->first_record_number,
		 value_32bit );
		libnotify_printf(
		 "%s: first record number\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "%s: size copy\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 size_copy );

		libnotify_printf(
		 "\n" );
	}
#endif
/* TODO check signatures and size and size_copy */
/* TODO correct values in IO handle if necessary */

	return( 1 );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libevt_record_get_type(
     libevt_record_t *record,
     uint8_t *type,
     liberror_error_t **error )
{
	static char *function = "libevt_record_get_type";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
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
	*type = record->type;

	return( 1 );
}

