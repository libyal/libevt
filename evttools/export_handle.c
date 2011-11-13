/* 
 * Export handle
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
#include <memory.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#include <libsystem.h>

#include "evtinput.h"
#include "evttools_libevt.h"
#include "evttools_libfdatetime.h"
#include "export_handle.h"
#include "log_handle.h"

#define EXPORT_HANDLE_NOTIFY_STREAM	stdout

char *export_handle_get_event_type(
       uint16_t event_type )
{
	switch( event_type )
	{
		case LIBEVT_EVENT_TYPE_ERROR:
			return( "Error event" );

		case LIBEVT_EVENT_TYPE_WARNING:
			return( "Warning event" );

		case LIBEVT_EVENT_TYPE_INFORMATION:
			return( "Information event" );

		case LIBEVT_EVENT_TYPE_AUDIT_SUCCESS:
			return( "Success Audit event" );

		case LIBEVT_EVENT_TYPE_AUDIT_FAILURE:
			return( "Failure Audit event" );

		default:
			break;
	}
	return( "(Unknown)" );
}

/* Initializes the export handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_initialize(
     export_handle_t **export_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_initialize";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( *export_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid export handle value already set.",
		 function );

		return( -1 );
	}
	*export_handle = memory_allocate_structure(
	                  export_handle_t );

	if( *export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create export handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *export_handle,
	     0,
	     sizeof( export_handle_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear export handle.",
		 function );

		goto on_error;
	}
	if( libevt_file_initialize(
	     &( ( *export_handle )->input_file ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input file.",
		 function );

		goto on_error;
	}
	( *export_handle )->notify_stream = EXPORT_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *export_handle != NULL )
	{
		memory_free(
		 *export_handle );

		*export_handle = NULL;
	}
	return( -1 );
}

/* Frees the export handle and its elements
 * Returns 1 if successful or -1 on error
 */
int export_handle_free(
     export_handle_t **export_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_free";
	int result            = 1;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( *export_handle != NULL )
	{
		if( libevt_file_free(
		     &( ( *export_handle )->input_file ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input file.",
			 function );

			result = -1;
		}
		memory_free(
		 *export_handle );

		*export_handle = NULL;
	}
	return( result );
}

/* Signals the export handle to abort
 * Returns 1 if successful or -1 on error
 */
int export_handle_signal_abort(
     export_handle_t *export_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_signal_abort";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	export_handle->abort = 1;

	if( export_handle->input_file != NULL )
	{
		if( libevt_file_signal_abort(
		     export_handle->input_file,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input file to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Prints the data on the stream
 * Returns the number of printed characters if successful or -1 on error
 */
int export_handle_print_data(
     export_handle_t *export_handle,
     FILE *stream,
     const uint8_t *data,
     size_t data_size,
     liberror_error_t **error )
{
	static char *function = "export_handle_print_data";
	size_t byte_iterator  = 0;
	size_t data_iterator  = 0;
	int print_count       = 0;
	int total_print_count = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		return( 0 );
	}
	while( data_iterator < data_size )
	{
		while( byte_iterator < data_size )
		{
			if( byte_iterator % 16 == 0 )
			{
				print_count = fprintf(
					       stream,
					       "%.8" PRIzx ": ",
					       byte_iterator );

				if( print_count <= -1 )
				{
					return( -1 );
				}
				total_print_count += print_count;
			}
			print_count = fprintf(
				       stream,
				       "%.2" PRIx8 " ",
				       data[ byte_iterator++ ] );

			if( print_count <= -1 )
			{
				return( -1 );
			}
			total_print_count += print_count;

			if( byte_iterator % 16 == 0 )
			{
				break;
			}
			else if( byte_iterator % 8 == 0 )
			{
				print_count = fprintf(
					       stream,
					       " " );

				if( print_count <= -1 )
				{
					return( -1 );
				}
				total_print_count += print_count;
			}
		}
		while( byte_iterator % 16 != 0 )
		{
			byte_iterator++;

			print_count = fprintf(
				       stream,
				       "   " );

			if( print_count <= -1 )
			{
				return( -1 );
			}
			total_print_count += print_count;

			if( ( byte_iterator % 8 == 0 )
			 && ( byte_iterator % 16 != 0 ) )
			{
				print_count = fprintf(
					       stream,
					       " " );

				if( print_count <= -1 )
				{
					return( -1 );
				}
				total_print_count += print_count;
			}
		}
		print_count = fprintf(
			       stream,
			       "  " );

		if( print_count <= -1 )
		{
			return( -1 );
		}
		total_print_count += print_count;

		byte_iterator = data_iterator;

		while( byte_iterator < data_size )
		{
			if( ( data[ byte_iterator ] >= 0x20 )
			 && ( data[ byte_iterator ] <= 0x7e ) )
			{
				print_count = fprintf(
					       stream,
					       "%c",
					       (char) data[ byte_iterator ] );
			}
			else
			{
				print_count = fprintf(
					       stream,
					       "." );
			}
			if( print_count <= -1 )
			{
				return( -1 );
			}
			total_print_count += print_count;

			byte_iterator++;

			if( byte_iterator % 16 == 0 )
			{
				break;
			}
			else if( byte_iterator % 8 == 0 )
			{
				print_count = fprintf(
					       stream,
					       " " );

				if( print_count <= -1 )
				{
					return( -1 );
				}
				total_print_count += print_count;
			}
		}
		print_count = fprintf(
			       stream,
			       "\n" );

		if( print_count <= -1 )
		{
			return( -1 );
		}
		total_print_count += print_count;

		data_iterator = byte_iterator;
	}
	print_count = fprintf(
		       stream,
		       "\n" );

	if( print_count <= -1 )
	{
		return( -1 );
	}
	total_print_count += print_count;

	return( total_print_count );
}

/* Opens the input
 * Returns 1 if successful or -1 on error
 */
int export_handle_open_input(
     export_handle_t *export_handle,
     const libcstring_system_character_t *filename,
     liberror_error_t **error )
{
	static char *function = "export_handle_open_input";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libevt_file_open_wide(
	     export_handle->input_file,
	     filename,
	     LIBEVT_OPEN_READ,
	     error ) != 1 )
#else
	if( libevt_file_open(
	     export_handle->input_file,
	     filename,
	     LIBEVT_OPEN_READ,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open input file.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes the input
 * Returns the 0 if succesful or -1 on error
 */
int export_handle_close_input(
     export_handle_t *export_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_close_input";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( libevt_file_close(
	     export_handle->input_file,
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input file.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Exports the record
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_record(
     export_handle_t *export_handle,
     libevt_item_t *record,
     log_handle_t *log_handle,
     liberror_error_t **error )
{
	libcstring_system_character_t posix_time_string[ 32 ];

	libcstring_system_character_t *value_string = NULL;
	libfdatetime_posix_time_t *posix_time       = NULL;
	static char *function                       = "export_handle_export_record";
	size_t value_string_size                    = 0;
	uint32_t value_32bit                        = 0;
	uint16_t event_type                         = 0;
	int result                                  = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
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
	if( libevt_item_get_identifier(
	     record,
	     &value_32bit,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	fprintf(
	 export_handle->notify_stream,
	 "%" PRIu32 "\t",
	 value_32bit );

	if( libevt_item_get_creation_time(
	     record,
	     &value_32bit,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_posix_time_copy_from_32bit(
	     posix_time,
	     value_32bit,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy POSIX time from 32-bit.",
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
	fprintf(
	 export_handle->notify_stream,
	 "%" PRIs_LIBCSTRING_SYSTEM " UTC\t",
	 posix_time_string );

	if( libevt_item_get_written_time(
	     record,
	     &value_32bit,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve written time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_posix_time_copy_from_32bit(
	     posix_time,
	     value_32bit,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy POSIX time from 32-bit.",
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
	fprintf(
	 export_handle->notify_stream,
	 "%" PRIs_LIBCSTRING_SYSTEM " UTC\t",
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
	if( libevt_item_get_event_identifier(
	     record,
	     &value_32bit,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event identifier.",
		 function );

		goto on_error;
	}
	fprintf(
	 export_handle->notify_stream,
	 "0x%08" PRIx32 "\t",
	 value_32bit );

	if( libevt_item_get_event_type(
	     record,
	     &event_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event type.",
		 function );

		goto on_error;
	}
	fprintf(
	 export_handle->notify_stream,
	 "%s\t",
	 export_handle_get_event_type(
	  event_type ) );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libevt_item_get_utf16_source_name_size(
	          record,
	          &value_string_size,
	          error );
#else
	result = libevt_item_get_utf8_source_name_size(
	          record,
	          &value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve source name size.",
		 function );

		goto on_error;
	}
	if( value_string_size > 0 )
	{
		value_string = libcstring_system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value string.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libevt_item_get_utf16_source_name(
		          record,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libevt_item_get_utf8_source_name(
		          record,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve source name.",
			 function );

			goto on_error;
		}
		fprintf(
		 export_handle->notify_stream,
		 "%" PRIs_LIBCSTRING_SYSTEM "\t",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libevt_item_get_utf16_computer_name_size(
	          record,
	          &value_string_size,
	          error );
#else
	result = libevt_item_get_utf8_computer_name_size(
	          record,
	          &value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve computer name size.",
		 function );

		goto on_error;
	}
	if( value_string_size > 0 )
	{
		value_string = libcstring_system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value string.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libevt_item_get_utf16_computer_name(
		          record,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libevt_item_get_utf8_computer_name(
		          record,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve computer name.",
			 function );

			goto on_error;
		}
		fprintf(
		 export_handle->notify_stream,
		 "%" PRIs_LIBCSTRING_SYSTEM "\t",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
/* TODO print more values */
	fprintf(
	 export_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( posix_time != NULL )
	{
		libfdatetime_posix_time_free(
		 &posix_time,
		 NULL );
	}
	return( -1 );
}

/* Exports the items
 * Returns the 1 if succesful, 0 if no items are available or -1 on error
 */
int export_handle_export_items(
     export_handle_t *export_handle,
     libevt_file_t *file,
     log_handle_t *log_handle,
     liberror_error_t **error )
{
	libevt_item_t *item   = NULL;
	static char *function = "export_handle_export_items";
	int number_of_items   = 0;
	int item_index        = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( libevt_file_get_number_of_items(
	     file,
	     &number_of_items,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of items.",
		 function );

		return( -1 );
	}
	if( number_of_items == 0 )
	{
		return( 0 );
	}
	for( item_index = 0;
	     item_index < number_of_items;
	     item_index++ )
	{
		if( export_handle->abort != 0 )
		{
			return( -1 );
		}
		if( libevt_file_get_item(
		     file,
		     item_index,
		     &item,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve item: %d.",
			 function,
			 item_index );

			return( -1 );
		}
		if( export_handle_export_record(
		     export_handle,
		     item,
		     log_handle,
		     error ) != 1 )
		{
/* TODO add error tollerance and log error */
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export item: %d.",
			 function,
			 item_index );

			return( -1 );
		}
		if( libevt_item_free(
		     &item,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free item: %d.",
			 function,
			 item_index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Exports the items from the file
 * Returns the 1 if succesful, 0 if no items are available or -1 on error
 */
int export_handle_export_file(
     export_handle_t *export_handle,
     log_handle_t *log_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_export_file";
	int result            = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = export_handle_export_items(
	          export_handle,
	          export_handle->input_file,
	          log_handle,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export items.",
		 function );

		return( -1 );
	}
	return( result );
}

