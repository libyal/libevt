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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include "libevt_debug.h"
#include "libevt_end_of_file_record.h"
#include "libevt_event_record.h"
#include "libevt_io_handle.h"
#include "libevt_libbfio.h"
#include "libevt_libcerror.h"
#include "libevt_libcnotify.h"
#include "libevt_libfdatetime.h"
#include "libevt_libfwnt.h"
#include "libevt_record_values.h"
#include "libevt_unused.h"

#include "evt_file_header.h"
#include "evt_record.h"

/* Creates record values
 * Make sure the value record_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_initialize(
     libevt_record_values_t **record_values,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_initialize";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( *record_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record values value already set.",
		 function );

		return( -1 );
	}
	*record_values = memory_allocate_structure(
	                  libevt_record_values_t );

	if( *record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create record values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *record_values,
	     0,
	     sizeof( libevt_record_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
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

/* Frees record values
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_free(
     libevt_record_values_t **record_values,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_free";
	int result            = 1;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( *record_values != NULL )
	{
		if( ( *record_values )->event_record != NULL )
		{
			if( libevt_event_record_free(
			     &( ( *record_values )->event_record ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free event record.",
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

/* Reads a record_values
 * Returns the number of bytes read if successful or -1 on error
 */
ssize_t libevt_record_values_read(
         libevt_record_values_t *record_values,
         libbfio_handle_t *file_io_handle,
         libevt_io_handle_t *io_handle,
         off64_t *file_offset,
         uint8_t strict_mode,
         libcerror_error_t **error )
{
	uint8_t record_size_data[ 4 ];

	libevt_end_of_file_record_t *end_of_file_record = NULL;
	uint8_t *record_data                            = NULL;
	static char *function                           = "libevt_record_values_read";
	size_t read_size                                = 0;
	size_t record_data_offset                       = 0;
	ssize_t read_count                              = 0;
	ssize_t total_read_count                        = 0;
	uint32_t record_data_size                       = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( file_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file offset.",
		 function );

		return( -1 );
	}
	record_values->offset = *file_offset;

	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              record_size_data,
	              sizeof( uint32_t ),
	              error );

	if( read_count != (ssize_t) sizeof( uint32_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record size data.",
		 function );

		goto on_error;
	}
	*file_offset    += read_count;
	total_read_count = read_count;

	byte_stream_copy_to_uint32_little_endian(
	 record_size_data,
	 record_data_size );

	/* Note that 128 MiB is an arbitrary selected upper limit here
	 * At least 8 bytes is needed to check the record signature
	 */
	if( ( record_data_size < 8 )
	 || ( record_data_size > (uint32_t) ( 128 * 1024 * 1024 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: record data size value out of bounds.",
		 function );

		goto on_error;
	}
	/* Allocating record data as 4 bytes and then using realloc here
	 * corrupts the memory
	 */
	record_data = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * record_data_size );

	if( record_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
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
		read_size = (size_t) ( io_handle->file_size - *file_offset );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              &( record_data[ record_data_offset ] ),
	              read_size,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek file header offset: %" PRIzd ".",
			 function,
			 sizeof( evt_file_header_t ) );

			goto on_error;
		}
		*file_offset = (off64_t) sizeof( evt_file_header_t );

		read_size = (size_t) record_data_size - record_data_offset;

		read_count = libbfio_handle_read_buffer(
		              file_io_handle,
		              &( record_data[ record_data_offset ] ),
		              read_size,
		              error );

		if( read_count != (ssize_t) read_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read record data.",
			 function );

			goto on_error;
		}
		*file_offset     += read_count;
		total_read_count += read_count;
	}
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
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: record data:\n",
			 function );
			libcnotify_print_data(
			 record_data,
			 (size_t) record_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
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
		     strict_mode,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read event record values.",
			 function );

			goto on_error;
		}
	}
	else if( record_values->type == LIBEVT_RECORD_TYPE_END_OF_FILE )
	{
		if( libevt_end_of_file_record_initialize(
		     &end_of_file_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create end-of-file record.",
			 function );

			goto on_error;
		}
		if( libevt_end_of_file_record_read_data(
		     end_of_file_record,
		     record_data,
		     (size_t) record_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read end-of-file record.",
			 function );

			goto on_error;
		}
		if( libevt_end_of_file_record_free(
		     &end_of_file_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free end-of-file record.",
			 function );

			goto on_error;
		}
	}
	memory_free(
	 record_data );

	return( total_read_count );

on_error:
	if( end_of_file_record != NULL )
	{
		libevt_end_of_file_record_free(
		 &end_of_file_record,
		 NULL );
	}
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
     uint8_t strict_mode,
     libcerror_error_t **error )
{
	libevt_event_record_t *event_record = NULL;
	static char *function               = "libevt_record_values_read_event";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_initialize(
	     &event_record,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create event record.",
		 function );

		goto on_error;
	}
	if( libevt_event_record_read_data(
	     event_record,
	     record_data,
	     record_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read event record.",
		 function );

		goto on_error;
	}
	if( strict_mode != 0 )
	{
		if( event_record->record_size != event_record->copy_of_record_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: value mismatch for record size and copy of record size.",
			 function );

			goto on_error;
		}
		if( record_data_size != (size_t) event_record->record_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: value mismatch for record data size and record size.",
			 function );

			goto on_error;
		}
	}
	record_values->event_record = event_record;

	return( 1 );

on_error:
	if( event_record != NULL )
	{
		libevt_event_record_free(
		 &event_record,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the offset
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_offset(
     libevt_record_values_t *record_values,
     off64_t *offset,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_offset";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
	*offset = record_values->offset;

	return( 1 );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_type(
     libevt_record_values_t *record_values,
     uint8_t *type,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_type";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	*type = record_values->type;

	return( 1 );
}

/* Retrieves the number
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_number(
     libevt_record_values_t *record_values,
     uint32_t *number,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_number";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_record_number(
	     record_values->event_record,
	     number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record number.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the creation time
 * The timestamp is a 32-bit POSIX date and time value
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_creation_time(
     libevt_record_values_t *record_values,
     uint32_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_creation_time";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_creation_time(
	     record_values->event_record,
	     posix_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the written time
 * The timestamp is a 32-bit POSIX date and time value
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_written_time(
     libevt_record_values_t *record_values,
     uint32_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_written_time";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_last_written_time(
	     record_values->event_record,
	     posix_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve last written time.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the event identifier
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_event_identifier(
     libevt_record_values_t *record_values,
     uint32_t *event_identifier,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_event_identifier";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_event_identifier(
	     record_values->event_record,
	     event_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the event type
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_event_type(
     libevt_record_values_t *record_values,
     uint16_t *event_type,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_event_type";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_event_type(
	     record_values->event_record,
	     event_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event type.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the event category
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_event_category(
     libevt_record_values_t *record_values,
     uint16_t *event_category,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_event_category";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_event_category(
	     record_values->event_record,
	     event_category,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event category.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_utf8_source_name_size(
     libevt_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_utf8_source_name_size";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_utf8_source_name_size(
	     record_values->event_record,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of source name as UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_utf8_source_name(
     libevt_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_utf8_source_name";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_utf8_source_name(
	     record_values->event_record,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve source name as UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_utf16_source_name_size(
     libevt_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_utf16_source_name_size";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_utf16_source_name_size(
	     record_values->event_record,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of source name as UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_utf16_source_name(
     libevt_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_utf16_source_name";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_utf16_source_name(
	     record_values->event_record,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve source name as UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_utf8_computer_name_size(
     libevt_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_utf8_computer_name_size";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_utf8_computer_name_size(
	     record_values->event_record,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of computer name as UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_utf8_computer_name(
     libevt_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_utf8_computer_name";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_utf8_computer_name(
	     record_values->event_record,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve computer name as UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_utf16_computer_name_size(
     libevt_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_utf16_computer_name_size";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_utf16_computer_name_size(
	     record_values->event_record,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of computer name as UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_utf16_computer_name(
     libevt_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_utf16_computer_name";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_utf16_computer_name(
	     record_values->event_record,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve computer name as UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded user security identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_values_get_utf8_user_security_identifier_size(
     libevt_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_utf8_user_security_identifier_size";
	int result            = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	result = libevt_event_record_get_utf8_user_security_identifier_size(
	          record_values->event_record,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of user security identifier as UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-8 encoded user security identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_values_get_utf8_user_security_identifier(
     libevt_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_utf8_user_security_identifier";
	int result            = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	result = libevt_event_record_get_utf8_user_security_identifier(
	          record_values->event_record,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve user security identifier as UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-16 encoded user security identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_values_get_utf16_user_security_identifier_size(
     libevt_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_utf16_user_security_identifier_size";
	int result            = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	result = libevt_event_record_get_utf16_user_security_identifier_size(
	          record_values->event_record,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of user security identifier as UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded user security identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_values_get_utf16_user_security_identifier(
     libevt_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_utf16_user_security_identifier";
	int result            = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	result = libevt_event_record_get_utf16_user_security_identifier(
	          record_values->event_record,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve user security identifier as UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the number of strings
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_number_of_strings(
     libevt_record_values_t *record_values,
     int *number_of_strings,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_number_of_strings";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_number_of_strings(
	     record_values->event_record,
	     number_of_strings,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of strings.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of a specific UTF-8 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_get_utf8_string_size(
     libevt_record_values_t *record_values,
     int string_index,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_utf8_string_size";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_utf8_string_size(
	     record_values->event_record,
	     string_index,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 string: %d.",
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
int libevt_record_values_get_utf8_string(
     libevt_record_values_t *record_values,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_values_record_get_utf8_string";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_utf8_string(
	     record_values->event_record,
	     string_index,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string: %d.",
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
int libevt_record_values_get_utf16_string_size(
     libevt_record_values_t *record_values,
     int string_index,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_utf16_string_size";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_utf16_string_size(
	     record_values->event_record,
	     string_index,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 string: %d.",
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
int libevt_record_values_get_utf16_string(
     libevt_record_values_t *record_values,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_values_record_get_utf16_string";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_utf16_string(
	     record_values->event_record,
	     string_index,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_values_get_data_size(
     libevt_record_values_t *record_values,
     size_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_data_size";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_data_size(
	     record_values->event_record,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_values_get_data(
     libevt_record_values_t *record_values,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_get_data";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_event_record_get_data(
	     record_values->event_record,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads record values
 * Callback for the (recovered) records list
 * Returns 1 if successful or -1 on error
 */
int libevt_record_values_read_element_data(
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int element_file_index LIBEVT_ATTRIBUTE_UNUSED,
     off64_t element_offset,
     size64_t element_size LIBEVT_ATTRIBUTE_UNUSED,
     uint32_t element_flags LIBEVT_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBEVT_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libevt_record_values_t *record_values = NULL;
	static char *function                 = "libevt_record_values_read_element_data";
	off64_t file_offset                   = 0;
	ssize_t read_count                    = 0;

	LIBEVT_UNREFERENCED_PARAMETER( element_size )
	LIBEVT_UNREFERENCED_PARAMETER( element_file_index )
	LIBEVT_UNREFERENCED_PARAMETER( element_flags )
	LIBEVT_UNREFERENCED_PARAMETER( read_flags )

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading record  at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 element_offset,
		 element_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     element_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek record offset: %" PRIi64 ".",
		 function,
		 element_offset );

		goto on_error;
	}
	if( libevt_record_values_initialize(
	     &record_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record values.",
		 function );

		goto on_error;
	}
	/* File offset must be before being passed to libevt_record_values_read
	 */
	file_offset = element_offset;

	read_count = libevt_record_values_read(
		      record_values,
		      file_io_handle,
		      io_handle,
		      &file_offset,
		      0,
		      error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record at offset: %" PRIi64 ".",
		 function,
		 element_offset );

		goto on_error;
	}
	if( libfdata_list_element_set_element_value(
	     element,
	     (intptr_t *) file_io_handle,
	     cache,
	     (intptr_t *) record_values,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libevt_record_values_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set record values as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( record_values != NULL )
	{
		libevt_record_values_free(
		 &record_values,
		 NULL );
	}
	return( -1 );
}

