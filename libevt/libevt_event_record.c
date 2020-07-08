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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libevt_debug.h"
#include "libevt_event_record.h"
#include "libevt_libcerror.h"
#include "libevt_libcnotify.h"
#include "libevt_libfdatetime.h"
#include "libevt_libfwnt.h"
#include "libevt_libuna.h"
#include "libevt_strings_array.h"

#include "evt_event_record.h"

/* Creates a event record
 * Make sure the value event_record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_initialize(
     libevt_event_record_t **event_record,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_initialize";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( *event_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid event record value already set.",
		 function );

		return( -1 );
	}
	*event_record = memory_allocate_structure(
	                 libevt_event_record_t );

	if( *event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create event record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *event_record,
	     0,
	     sizeof( libevt_event_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear event record.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *event_record != NULL )
	{
		memory_free(
		 *event_record );

		*event_record = NULL;
	}
	return( -1 );
}

/* Frees a event record
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_free(
     libevt_event_record_t **event_record,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_free";
	int result            = 1;

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( *event_record != NULL )
	{
		if( ( *event_record )->source_name != NULL )
		{
			memory_free(
			 ( *event_record )->source_name );
		}
		if( ( *event_record )->computer_name != NULL )
		{
			memory_free(
			 ( *event_record )->computer_name );
		}
		if( ( *event_record )->user_security_identifier != NULL )
		{
			if( libfwnt_security_identifier_free(
			     &( ( *event_record )->user_security_identifier ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free user security identifier.",
				 function );

				result = -1;
			}
		}
		if( ( *event_record )->strings != NULL )
		{
			if( libevt_strings_array_free(
			     &( ( *event_record )->strings ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free strings.",
				 function );

				result = -1;
			}
		}
		if( ( *event_record )->data != NULL )
		{
			memory_free(
			 ( *event_record )->data );
		}
		memory_free(
		 *event_record );

		*event_record = NULL;
	}
	return( result );
}

/* Reads the event record data
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_read_data(
     libevt_event_record_t *event_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	const uint8_t *computer_name             = NULL;
	const uint8_t *event_data                = NULL;
	const uint8_t *event_strings             = NULL;
	const uint8_t *source_name               = NULL;
	const uint8_t *user_security_identifier  = NULL;
	static char *function                    = "libevt_event_record_read_data";
	size_t computer_name_size                = 0;
	size_t data_offset                       = 0;
	size_t event_strings_size                = 0;
	size_t maximum_data_size                 = 0;
	size_t maximum_strings_data_size         = 0;
	size_t source_name_size                  = 0;
	uint32_t event_data_offset               = 0;
	uint32_t event_data_size                 = 0;
	uint32_t event_strings_offset            = 0;
	uint32_t user_security_identifier_offset = 0;
	uint32_t user_security_identifier_size   = 0;
	uint16_t number_of_event_strings         = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                     = 0;
	uint16_t value_16bit                     = 0;
#endif

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( event_record->source_name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid event record - source name value already set.",
		 function );

		return( -1 );
	}
	if( event_record->computer_name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid event record - computer name value already set.",
		 function );

		return( -1 );
	}
	if( event_record->user_security_identifier != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid event record - user security identifier value already set.",
		 function );

		return( -1 );
	}
	if( event_record->strings != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid event record - strings value already set.",
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
	if( data_size < sizeof( evt_event_record_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data size value too small.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	maximum_data_size = data_size - 4;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: event record data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( evt_event_record_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     ( (evt_event_record_t *) data )->signature,
	     "LfLe",
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_event_record_t *) data )->size,
	 event_record->record_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_event_record_t *) data )->record_number,
	 event_record->record_number );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_event_record_t *) data )->creation_time,
	 event_record->creation_time );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_event_record_t *) data )->last_written_time,
	 event_record->last_written_time );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_event_record_t *) data )->event_identifier,
	 event_record->event_identifier );

	byte_stream_copy_to_uint16_little_endian(
	 ( (evt_event_record_t *) data )->event_type,
	 event_record->event_type );

	byte_stream_copy_to_uint16_little_endian(
	 ( (evt_event_record_t *) data )->number_of_strings,
	 number_of_event_strings );

	byte_stream_copy_to_uint16_little_endian(
	 ( (evt_event_record_t *) data )->event_category,
	 event_record->event_category );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_event_record_t *) data )->event_strings_offset,
	 event_strings_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_event_record_t *) data )->user_security_identifier_size,
	 user_security_identifier_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_event_record_t *) data )->user_security_identifier_offset,
	 user_security_identifier_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_event_record_t *) data )->event_data_size,
	 event_data_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_event_record_t *) data )->event_data_offset,
	 event_data_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 event_record->record_size );

		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (evt_event_record_t *) data )->signature[ 0 ],
		 ( (evt_event_record_t *) data )->signature[ 1 ],
		 ( (evt_event_record_t *) data )->signature[ 2 ],
		 ( (evt_event_record_t *) data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: record number\t\t\t\t: %" PRIu32 "\n",
		 function,
		 event_record->record_number );

		if( libevt_debug_print_posix_time_value(
		     function,
		     "creation time\t\t\t\t",
		     ( (evt_event_record_t *) data )->creation_time,
		     4,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_POSIX_TIME_VALUE_TYPE_SECONDS_32BIT_SIGNED,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			goto on_error;
		}
		if( libevt_debug_print_posix_time_value(
		     function,
		     "last written time\t\t\t",
		     ( (evt_event_record_t *) data )->last_written_time,
		     4,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_POSIX_TIME_VALUE_TYPE_SECONDS_32BIT_SIGNED,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: event identifier\t\t\t\t: %" PRIu32 "\n",
		 function,
		 event_record->event_identifier );

		libcnotify_printf(
		 "%s: event type\t\t\t\t: %" PRIu16 "\n",
		 function,
		 event_record->event_type );

		libcnotify_printf(
		 "%s: number of event strings\t\t\t: %" PRIu16 "\n",
		 function,
		 number_of_event_strings );

		libcnotify_printf(
		 "%s: event category\t\t\t\t: %" PRIu16 "\n",
		 function,
		 event_record->event_category );

		byte_stream_copy_to_uint16_little_endian(
		 ( (evt_event_record_t *) data )->unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_event_record_t *) data )->unknown2,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: event strings offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 event_strings_offset );

		libcnotify_printf(
		 "%s: user security identifier size\t\t: %" PRIu32 "\n",
		 function,
		 user_security_identifier_size );

		libcnotify_printf(
		 "%s: user security identifier offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 user_security_identifier_offset );

		libcnotify_printf(
		 "%s: event data size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 event_data_size );

		libcnotify_printf(
		 "%s: event data offset\t\t\t: %" PRIu32 "\n",
		 function,
		 event_data_offset );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = sizeof( evt_event_record_t );

	source_name      = &( data[ data_offset ] );
	source_name_size = 0;

	while( ( data_offset + 1 ) < maximum_data_size )
	{
		source_name_size += 2;

		if( ( data[ data_offset ] == 0 )
		 && ( data[ data_offset + 1 ] == 0 ) )
		{
			break;
		}
		data_offset += 2;
	}
	data_offset += 2;

	if( source_name_size > 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libevt_debug_print_utf16_string_value(
			     function,
			     "source name\t\t\t\t",
			     source_name,
			     source_name_size,
			     LIBUNA_ENDIAN_LITTLE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print source name UTF-16 string value.",
				 function );

				goto on_error;
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( source_name_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid source name size value exceeds maximum allocation size.",
			 function );

			goto on_error;
		}
		event_record->source_name = (uint8_t *) memory_allocate(
		                                         sizeof( uint8_t ) * source_name_size );

		if( event_record->source_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create source name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     event_record->source_name,
		     source_name,
		     sizeof( uint8_t ) * source_name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source name.",
			 function );

			goto on_error;
		}
		event_record->source_name_size = source_name_size;
	}
	computer_name      = &( data[ data_offset ] );
	computer_name_size = 0;

	while( ( data_offset + 1 ) < maximum_data_size )
	{
		computer_name_size += 2;

		if( ( data[ data_offset ] == 0 )
		 && ( data[ data_offset + 1 ] == 0 ) )
		{
			break;
		}
		data_offset += 2;
	}
	data_offset += 2;

	if( computer_name_size > 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libevt_debug_print_utf16_string_value(
			     function,
			     "computer name\t\t\t\t",
			     computer_name,
			     computer_name_size,
			     LIBUNA_ENDIAN_LITTLE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print computer name UTF-16 string value.",
				 function );

				goto on_error;
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( computer_name_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid computer name size value exceeds maximum allocation size.",
			 function );

			goto on_error;
		}
		event_record->computer_name = (uint8_t *) memory_allocate(
		                                           sizeof( uint8_t ) * computer_name_size );

		if( event_record->computer_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create computer name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     event_record->computer_name,
		     computer_name,
		     sizeof( uint8_t ) * computer_name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy computer name.",
			 function );

			goto on_error;
		}
		event_record->computer_name_size = computer_name_size;
	}
/* TODO use dtFabric condition rules to generate code below */

	if( user_security_identifier_size != 0 )
	{
		if( ( (size_t) user_security_identifier_offset < data_offset )
		 || ( (size_t) user_security_identifier_offset >= maximum_data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: user security identifier offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( (size_t) user_security_identifier_size > maximum_data_size )
		 || ( (size_t) user_security_identifier_offset >= ( maximum_data_size - user_security_identifier_size ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: user identifier size value out of bounds.",
			 function );

			goto on_error;
		}
		user_security_identifier = &( data[ user_security_identifier_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libevt_debug_print_security_identifier_value(
			     function,
			     "user security identifier\t\t\t",
			     user_security_identifier,
			     user_security_identifier_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print user security identifier value.",
				 function );

				goto on_error;
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( libfwnt_security_identifier_initialize(
		     &( event_record->user_security_identifier ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create user security identifier.",
			 function );

			goto on_error;
		}
		if( libfwnt_security_identifier_copy_from_byte_stream(
		     event_record->user_security_identifier,
		     user_security_identifier,
		     user_security_identifier_size,
		     LIBFWNT_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy byte stream to user security identifier.",
			 function );

			goto on_error;
		}
		data_offset += user_security_identifier_size;
	}
	if( number_of_event_strings != 0 )
	{
		maximum_strings_data_size = event_data_offset;

		if( ( maximum_strings_data_size == 0 )
		 || ( (size_t) maximum_strings_data_size >= maximum_data_size ) )
		{
			maximum_strings_data_size = maximum_data_size;
		}
		if( ( event_strings_offset < data_offset )
		 || ( (size_t) event_strings_offset >= maximum_strings_data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: event strings offset value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( event_strings_offset > data_offset )
			{
				libcnotify_printf(
				 "%s: unknown3 data\n",
				 function );
				libcnotify_print_data(
				 &( data[ data_offset ] ),
				 event_strings_offset - data_offset,
				 0 );
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		event_strings      = &( data[ event_strings_offset ] );
		event_strings_size = maximum_strings_data_size - event_strings_offset;

		if( libevt_strings_array_initialize(
		     &( event_record->strings ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create strings array.",
			 function );

			goto on_error;
		}
		if( libevt_strings_array_read_data(
		     event_record->strings,
		     event_strings,
		     event_strings_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read strings array.",
			 function );

			goto on_error;
		}
		data_offset += event_strings_size;
	}
	if( event_data_size != 0 )
	{
		if( ( event_data_offset < data_offset )
		 || ( (size_t) event_data_offset >= maximum_data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: event data offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( (size_t) event_data_size > maximum_data_size )
		 || ( (size_t) event_data_offset > ( maximum_data_size - event_data_size ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: event data size value out of bounds.",
			 function );

			goto on_error;
		}
		event_data = &( data[ data_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: event data\n",
			 function );
			libcnotify_print_data(
			 event_data,
			 event_data_size,
			 0 );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( event_data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid event data size value exceeds maximum allocation size.",
			 function );

			goto on_error;
		}
		event_record->data = (uint8_t *) memory_allocate(
		                                  sizeof( uint8_t ) * event_data_size );

		if( event_record->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     event_record->data,
		     event_data,
		     sizeof( uint8_t ) * event_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy data.",
			 function );

			goto on_error;
		}
		event_record->data_size = event_data_size;

		data_offset += event_data_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( data_offset < maximum_data_size )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: alignment padding:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 maximum_data_size - data_offset,
			 0 );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	byte_stream_copy_to_uint32_little_endian(
	 &( data[ data_size - 4 ] ),
	 event_record->copy_of_record_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: copy of size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 event_record->copy_of_record_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );

on_error:
	if( event_record->data != NULL )
	{
		memory_free(
		 event_record->data );

		event_record->data = NULL;
	}
	event_record->data_size = 0;

	if( event_record->strings != NULL )
	{
		libevt_strings_array_free(
		 &( event_record->strings ),
		 NULL );
	}
	if( event_record->user_security_identifier != NULL )
	{
		libfwnt_security_identifier_free(
		 &( event_record->user_security_identifier ),
		 NULL );
	}
	if( event_record->computer_name != NULL )
	{
		memory_free(
		 event_record->computer_name );

		event_record->computer_name = NULL;
	}
	event_record->computer_name_size = 0;

	if( event_record->source_name != NULL )
	{
		memory_free(
		 event_record->source_name );

		event_record->source_name = NULL;
	}
	event_record->source_name_size = 0;

	return( -1 );
}

/* Retrieves the record number
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_record_number(
     libevt_event_record_t *event_record,
     uint32_t *record_number,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_record_number";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( record_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record number.",
		 function );

		return( -1 );
	}
	*record_number = event_record->record_number;

	return( 1 );
}

/* Retrieves the creation time
 * The timestamp is a 32-bit POSIX date and time value
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_creation_time(
     libevt_event_record_t *event_record,
     uint32_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_creation_time";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( posix_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid POSIX time.",
		 function );

		return( -1 );
	}
	*posix_time = event_record->creation_time;

	return( 1 );
}

/* Retrieves the last written time
 * The timestamp is a 32-bit POSIX date and time value
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_last_written_time(
     libevt_event_record_t *event_record,
     uint32_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_last_written_time";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( posix_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid POSIX time.",
		 function );

		return( -1 );
	}
	*posix_time = event_record->last_written_time;

	return( 1 );
}

/* Retrieves the event identifier
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_event_identifier(
     libevt_event_record_t *event_record,
     uint32_t *event_identifier,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_event_identifier";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( event_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event identifier.",
		 function );

		return( -1 );
	}
	*event_identifier = event_record->event_identifier;

	return( 1 );
}

/* Retrieves the event type
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_event_type(
     libevt_event_record_t *event_record,
     uint16_t *event_type,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_event_type";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( event_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event type.",
		 function );

		return( -1 );
	}
	*event_type = event_record->event_type;

	return( 1 );
}

/* Retrieves the event category
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_event_category(
     libevt_event_record_t *event_record,
     uint16_t *event_category,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_event_category";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( event_category == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event category.",
		 function );

		return( -1 );
	}
	*event_category = event_record->event_category;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_utf8_source_name_size(
     libevt_event_record_t *event_record,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_utf8_source_name_size";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( event_record->source_name == NULL )
	{
		if( utf8_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-8 string size.",
			 function );

			return( -1 );
		}
		*utf8_string_size = 0;
	}
	else
	{
		if( libuna_utf8_string_size_from_utf16_stream(
		     event_record->source_name,
		     event_record->source_name_size,
		     LIBUNA_ENDIAN_LITTLE,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine size of UTF-8 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_utf8_source_name(
     libevt_event_record_t *event_record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_utf8_source_name";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     (libuna_utf8_character_t *) utf8_string,
	     utf8_string_size,
	     event_record->source_name,
	     event_record->source_name_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_utf16_source_name_size(
     libevt_event_record_t *event_record,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_utf16_source_name_size";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( event_record->source_name == NULL )
	{
		if( utf16_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string size.",
			 function );

			return( -1 );
		}
		*utf16_string_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_utf16_stream(
		     event_record->source_name,
		     event_record->source_name_size,
		     LIBUNA_ENDIAN_LITTLE,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine size of UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_utf16_source_name(
     libevt_event_record_t *event_record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_utf16_source_name";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     (libuna_utf16_character_t *) utf16_string,
	     utf16_string_size,
	     event_record->source_name,
	     event_record->source_name_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_utf8_computer_name_size(
     libevt_event_record_t *event_record,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_utf8_computer_name_size";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( event_record->computer_name == NULL )
	{
		if( utf8_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-8 string size.",
			 function );

			return( -1 );
		}
		*utf8_string_size = 0;
	}
	else
	{
		if( libuna_utf8_string_size_from_utf16_stream(
		     event_record->computer_name,
		     event_record->computer_name_size,
		     LIBUNA_ENDIAN_LITTLE,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine size of UTF-8 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_utf8_computer_name(
     libevt_event_record_t *event_record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_utf8_computer_name";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     (libuna_utf8_character_t *) utf8_string,
	     utf8_string_size,
	     event_record->computer_name,
	     event_record->computer_name_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_utf16_computer_name_size(
     libevt_event_record_t *event_record,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_utf16_computer_name_size";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( event_record->computer_name == NULL )
	{
		if( utf16_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string size.",
			 function );

			return( -1 );
		}
		*utf16_string_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_utf16_stream(
		     event_record->computer_name,
		     event_record->computer_name_size,
		     LIBUNA_ENDIAN_LITTLE,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine size of UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_utf16_computer_name(
     libevt_event_record_t *event_record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_utf16_computer_name";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     (libuna_utf16_character_t *) utf16_string,
	     utf16_string_size,
	     event_record->computer_name,
	     event_record->computer_name_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded user security identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_event_record_get_utf8_user_security_identifier_size(
     libevt_event_record_t *event_record,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_utf8_user_security_identifier_size";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( event_record->user_security_identifier == NULL )
	{
		return( 0 );
	}
	if( libfwnt_security_identifier_get_string_size(
	     event_record->user_security_identifier,
	     utf8_string_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded user security identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_event_record_get_utf8_user_security_identifier(
     libevt_event_record_t *event_record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_utf8_user_security_identifier";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( event_record->user_security_identifier == NULL )
	{
		return( 0 );
	}
	if( libfwnt_security_identifier_copy_to_utf8_string(
	     event_record->user_security_identifier,
	     utf8_string,
	     utf8_string_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded user security identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_event_record_get_utf16_user_security_identifier_size(
     libevt_event_record_t *event_record,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_utf16_user_security_identifier_size";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( event_record->user_security_identifier == NULL )
	{
		return( 0 );
	}
	if( libfwnt_security_identifier_get_string_size(
	     event_record->user_security_identifier,
	     utf16_string_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded user security identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_event_record_get_utf16_user_security_identifier(
     libevt_event_record_t *event_record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_utf16_user_security_identifier";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event record.",
		 function );

		return( -1 );
	}
	if( event_record->user_security_identifier == NULL )
	{
		return( 0 );
	}
	if( libfwnt_security_identifier_copy_to_utf16_string(
	     event_record->user_security_identifier,
	     utf16_string,
	     utf16_string_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of strings
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_number_of_strings(
     libevt_event_record_t *event_record,
     int *number_of_strings,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_number_of_strings";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( event_record->strings == NULL )
	{
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
		*number_of_strings = 0;
	}
	else
	{
		if( libevt_strings_array_get_number_of_strings(
		     event_record->strings,
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
	}
	return( 1 );
}

/* Retrieves the size of a specific UTF-8 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_event_record_get_utf8_string_size(
     libevt_event_record_t *event_record,
     int string_index,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_utf8_string_size";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_strings_array_get_utf8_string_size(
	     event_record->strings,
	     string_index,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of strings value entry: %d.",
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
int libevt_event_record_get_utf8_string(
     libevt_event_record_t *event_record,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_values_record_get_utf8_string";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_strings_array_get_utf8_string(
	     event_record->strings,
	     string_index,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy strings value entry: %d to UTF-8 string.",
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
int libevt_event_record_get_utf16_string_size(
     libevt_event_record_t *event_record,
     int string_index,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_utf16_string_size";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_strings_array_get_utf16_string_size(
	     event_record->strings,
	     string_index,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size of strings value entry: %d.",
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
int libevt_event_record_get_utf16_string(
     libevt_event_record_t *event_record,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_values_record_get_utf16_string";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( libevt_strings_array_get_utf16_string(
	     event_record->strings,
	     string_index,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy strings value entry: %d to UTF-16 string.",
		 function,
		 string_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_event_record_get_data_size(
     libevt_event_record_t *event_record,
     size_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_data_size";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	if( event_record->data == NULL )
	{
		return( 0 );
	}
	*data_size = event_record->data_size;

	return( 1 );
}

/* Retrieves the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_event_record_get_data(
     libevt_event_record_t *event_record,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_event_record_get_data";

	if( event_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
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
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( event_record->data == NULL )
	{
		return( 0 );
	}
	if( data_size < event_record->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data size value too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     data,
	     event_record->data,
	     event_record->data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

