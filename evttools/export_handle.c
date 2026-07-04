/*
 * Export handle
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

#include <common.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include "evtinput.h"
#include "evttools_libcerror.h"
#include "evttools_libcnotify.h"
#include "evttools_libclocale.h"
#include "evttools_libevt.h"
#include "evttools_libfdatetime.h"
#include "evttools_unused.h"
#include "export_handle.h"
#include "log_handle.h"
#include "message_handle.h"
#include "message_string.h"
#include "resource_file.h"

#define EXPORT_HANDLE_NOTIFY_STREAM		stdout

const char *export_handle_get_event_log_key_name(
             int event_log_type )
{
	switch( event_log_type )
	{
		case EVTTOOLS_EVENT_LOG_TYPE_APPLICATION:
			return( "Application" );

		case EVTTOOLS_EVENT_LOG_TYPE_INTERNET_EXPLORER:
			return( "Internet Explorer" );

		case EVTTOOLS_EVENT_LOG_TYPE_SECURITY:
			return( "Security" );

		case EVTTOOLS_EVENT_LOG_TYPE_SYSTEM:
			return( "System" );

		case EVTTOOLS_EVENT_LOG_TYPE_WINDOWS_POWERSHELL:
			return( "Windows PowerShell" );

		default:
			break;
	}
	return( "(Unknown)" );
}

const char *export_handle_get_event_type(
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

/* Copies a string of a decimal value to a 64-bit value
 * Returns 1 if successful or -1 on error
 */
int evttools_system_string_copy_from_64_bit_in_decimal(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	static char *function              = "evttools_system_string_copy_from_64_bit_in_decimal";
	size_t string_index                = 0;
	system_character_t character_value = 0;
	uint8_t maximum_string_index       = 20;
	int8_t sign                        = 1;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value_64bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 64-bit.",
		 function );

		return( -1 );
	}
	*value_64bit = 0;

	if( string[ string_index ] == (system_character_t) '-' )
	{
		string_index++;
		maximum_string_index++;

		sign = -1;
	}
	else if( string[ string_index ] == (system_character_t) '+' )
	{
		string_index++;
		maximum_string_index++;
	}
	while( string_index < string_size )
	{
		if( string[ string_index ] == 0 )
		{
			break;
		}
		if( string_index > (size_t) maximum_string_index )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
			 "%s: string too large.",
			 function );

			return( -1 );
		}
		*value_64bit *= 10;

		if( ( string[ string_index ] >= (system_character_t) '0' )
		 && ( string[ string_index ] <= (system_character_t) '9' ) )
		{
			character_value = (system_character_t) ( string[ string_index ] - (system_character_t) '0' );
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character value: %" PRIc_SYSTEM " at index: %d.",
			 function,
			 string[ string_index ],
			 string_index );

			return( -1 );
		}
		*value_64bit += character_value;

		string_index++;
	}
	if( sign == -1 )
	{
		*value_64bit *= (uint64_t) -1;
	}
	return( 1 );
}

/* Creates an export handle
 * Make sure the value export_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int export_handle_initialize(
     export_handle_t **export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_initialize";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( *export_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid export handle value already set.",
		 function );

		return( -1 );
	}
	*export_handle = memory_allocate_structure(
	                  export_handle_t );

	if( *export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create export handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *export_handle,
	     0,
	     sizeof( export_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear export handle.",
		 function );

		goto on_error;
	}
	if( message_handle_initialize(
	     &( ( *export_handle )->message_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create message handle.",
		 function );

		goto on_error;
	}
	if( libevt_file_initialize(
	     &( ( *export_handle )->input_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input file.",
		 function );

		goto on_error;
	}
	( *export_handle )->export_mode    = EXPORT_MODE_ITEMS;
	( *export_handle )->event_log_type = EVTTOOLS_EVENT_LOG_TYPE_UNKNOWN;
	( *export_handle )->ascii_codepage = LIBEVT_CODEPAGE_WINDOWS_1252;
	( *export_handle )->notify_stream  = EXPORT_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *export_handle != NULL )
	{
		if( ( *export_handle )->message_handle != NULL )
		{
			message_handle_free(
			 &( ( *export_handle )->message_handle ),
			 NULL );
		}
		memory_free(
		 *export_handle );

		*export_handle = NULL;
	}
	return( -1 );
}

/* Frees an export handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_free(
     export_handle_t **export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_free";
	int result            = 1;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( *export_handle != NULL )
	{
		if( ( *export_handle )->input_is_open != 0 )
		{
			if( export_handle_close_input(
			     *export_handle,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close export handle.",
				 function );

				result = -1;
			}
		}
		if( message_handle_free(
		     &( ( *export_handle )->message_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free message handle.",
			 function );

			result = -1;
		}
		if( libevt_file_free(
		     &( ( *export_handle )->input_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
     libcerror_error_t **error )
{
	static char *function = "export_handle_signal_abort";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	export_handle->abort = 1;

	if( message_handle_signal_abort(
	     export_handle->message_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to signal message handle to abort.",
		 function );

		return( -1 );
	}
	if( export_handle->input_file != NULL )
	{
		if( libevt_file_signal_abort(
		     export_handle->input_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input file to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the export mode
 * Returns 1 if successful, 0 if unsupported values or -1 on error
 */
int export_handle_set_export_mode(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_export_mode";
	size_t string_length  = 0;
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( string_length == 3 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "all" ),
		     3 ) == 0 )
		{
			export_handle->export_mode = EXPORT_MODE_ALL;

			result = 1;
		}
	}
	else if( string_length == 5 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "items" ),
		     5 ) == 0 )
		{
			export_handle->export_mode = EXPORT_MODE_ITEMS;

			result = 1;
		}
	}
	else if( string_length == 9 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "recovered" ),
		     9 ) == 0 )
		{
			export_handle->export_mode = EXPORT_MODE_RECOVERED;

			result = 1;
		}
	}
	return( result );
}

/* Sets the ascii codepage
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_ascii_codepage(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function  = "export_handle_set_ascii_codepage";
	size_t string_length   = 0;
	uint32_t feature_flags = 0;
	int result             = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	feature_flags = LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_KOI8
	              | LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS;

	string_length = system_string_length(
	                 string );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libclocale_codepage_copy_from_string_wide(
	          &( export_handle->ascii_codepage ),
	          string,
	          string_length,
	          feature_flags,
	          error );
#else
	result = libclocale_codepage_copy_from_string(
	          &( export_handle->ascii_codepage ),
	          string,
	          string_length,
	          feature_flags,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine ASCII codepage.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( message_handle_set_ascii_codepage(
		     export_handle->message_handle,
		     export_handle->ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set ASCII codepage in message handle.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Sets the preferred language identifier
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_preferred_language_identifier(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_preferred_language_identifier";
	size_t string_length  = 0;
	uint64_t value_64bit  = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( evttools_system_string_copy_from_64_bit_in_decimal(
	     string,
	     string_length + 1,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string to 64-bit decimal.",
		 function );

		return( -1 );
	}
	if( value_64bit > (uint64_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid preferred language identifier value out of bounds.",
		 function );

		return( -1 );
	}
	if( message_handle_set_preferred_language_identifier(
	     export_handle->message_handle,
	     (uint32_t) value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set preferred language identifier in message handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the event log type
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_event_log_type(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_event_log_type";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = evtinput_determine_event_log_type(
	          string,
	          &( export_handle->event_log_type ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine event log type.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the event log type from the filename
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_event_log_type_from_filename(
     export_handle_t *export_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_event_log_type_from_filename";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = evtinput_determine_event_log_type_from_filename(
	          filename,
	          &( export_handle->event_log_type ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine event log type from filename.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the name of the software registry file
 * Returns 1 if successful or -1 error
 */
int export_handle_set_software_registry_filename(
     export_handle_t *export_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_software_registry_filename";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( message_handle_set_software_registry_filename(
	     export_handle->message_handle,
	     filename,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set SOFTWARE registry filename in message handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the name of the system registry file
 * Returns 1 if successful or -1 error
 */
int export_handle_set_system_registry_filename(
     export_handle_t *export_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_system_registry_filename";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( message_handle_set_system_registry_filename(
	     export_handle->message_handle,
	     filename,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set SYSTEM registry filename in message handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the name of the directory containing the software and system registry file
 * Returns 1 if successful or -1 error
 */
int export_handle_set_registry_directory_name(
     export_handle_t *export_handle,
     const system_character_t *name,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_registry_directory_name";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( message_handle_set_registry_directory_name(
	     export_handle->message_handle,
	     name,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set registry directory name in message handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the path of the resource files
 * Returns 1 if successful or -1 error
 */
int export_handle_set_resource_files_path(
     export_handle_t *export_handle,
     const system_character_t *path,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_resource_files_path";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( message_handle_set_resource_files_path(
	     export_handle->message_handle,
	     path,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set resource files path in message handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the input
 * Returns 1 if successful or -1 on error
 */
int export_handle_open_input(
     export_handle_t *export_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "export_handle_open_input";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_is_open != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid export handle input is already open.",
		 function );

		return( -1 );
	}
	if( message_handle_open_input(
	     export_handle->message_handle,
	     export_handle_get_event_log_key_name(
	      export_handle->event_log_type ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open input of message handle.",
		 function );

		return( -1 );
	}
	if( libevt_file_set_ascii_codepage(
	     export_handle->input_file,
	     export_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set ASCII codepage in input file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open input file.",
		 function );

		return( -1 );
	}
	export_handle->input_is_open = 1;

	return( 1 );
}

/* Closes the input
 * Returns the 0 if successful or -1 on error
 */
int export_handle_close_input(
     export_handle_t *export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_close_input";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_is_open != 0 )
	{
		if( message_handle_close_input(
		     export_handle->message_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close input of message handle.",
			 function );

			result = -1;
		}
		if( libevt_file_close(
		     export_handle->input_file,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close input file.",
			 function );

			result = -1;
		}
		export_handle->input_is_open = 0;
	}
	return( result );
}

/* Prints a POSIX value
 * Returns 1 if successful or -1 on error
 */
int export_handle_posix_time_value_fprint(
     export_handle_t *export_handle,
     const char *value_name,
     uint32_t value_32bit,
     libcerror_error_t **error )
{
	system_character_t date_time_string[ 32 ];

	libfdatetime_posix_time_t *posix_time = NULL;
	static char *function                 = "export_handle_posix_time_fprint";
	int result                            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( value_32bit == 0 )
	{
		fprintf(
		 export_handle->notify_stream,
		 "%s: Not set (0)\n",
		 value_name );
	}
	else
	{
		if( libfdatetime_posix_time_initialize(
		     &posix_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create POSIX time.",
			 function );

			goto on_error;
		}
		if( libfdatetime_posix_time_copy_from_32bit(
		     posix_time,
		     value_32bit,
		     LIBFDATETIME_POSIX_TIME_VALUE_TYPE_SECONDS_32BIT_SIGNED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy POSIX time from 32-bit.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_posix_time_copy_to_utf16_string(
			  posix_time,
			  (uint16_t *) date_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  error );
#else
		result = libfdatetime_posix_time_copy_to_utf8_string(
			  posix_time,
			  (uint8_t *) date_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy POSIX time to string.",
			 function );

			goto on_error;
		}
		fprintf(
		 export_handle->notify_stream,
		 "%s: %" PRIs_SYSTEM " UTC\n",
		 value_name,
		 date_time_string );

		if( libfdatetime_posix_time_free(
		     &posix_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free POSIX time.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( posix_time != NULL )
	{
		libfdatetime_posix_time_free(
		 &posix_time,
		 NULL );
	}
	return( -1 );
}

/* Exports the record event category
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_record_event_category(
     export_handle_t *export_handle,
     libevt_record_t *record,
     const system_character_t *event_source,
     size_t event_source_length,
     log_handle_t *log_handle EVTTOOLS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	system_character_t *message_filename = NULL;
	message_string_t *message_string     = NULL;
	static char *function                = "export_handle_export_record_event_category";
	size_t message_filename_size         = 0;
	size_t message_string_index          = 0;
	uint16_t event_category              = 0;
	int result                           = 0;

	EVTTOOLS_UNREFERENCED_PARAMETER( log_handle )

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( libevt_record_get_event_category(
	     record,
	     &event_category,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event category.",
		 function );

		goto on_error;
	}
	if( event_source != NULL )
	{
		result = message_handle_get_value_by_event_source(
		          export_handle->message_handle,
		          event_source,
		          event_source_length,
		          _SYSTEM_STRING( "CategoryMessageFile" ),
		          19,
		          &message_filename,
		          &message_filename_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve category message filename.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			fprintf(
			 export_handle->notify_stream,
			 "Category file(s)\t\t: %" PRIs_SYSTEM "\n",
			 message_filename );

			result = message_handle_get_message_string(
				  export_handle->message_handle,
				  message_filename,
				  message_filename_size - 1,
				  (uint32_t) event_category,
				  &message_string,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve message string: 0x%08" PRIx32 " from: %" PRIs_SYSTEM ".",
				 function,
				 (uint32_t) event_category,
				 message_filename );

				goto on_error;
			}
			memory_free(
			 message_filename );

			message_filename      = NULL;
			message_filename_size = 0;
		}
	}
	if( message_string != NULL )
	{
		for( message_string_index = 0;
		     message_string_index < message_string->string_size;
		     message_string_index++ )
		{
			if( ( ( message_string->string )[ message_string_index ] == (system_character_t) '\n' )
			 || ( ( message_string->string )[ message_string_index ] == (system_character_t) '\r' ) )
			{
				( message_string->string )[ message_string_index ] = 0;
			}
		}
		fprintf(
		 export_handle->notify_stream,
		 "Event category\t\t\t: %" PRIs_SYSTEM " (%" PRIu16 ")\n",
		 message_string->string,
		 event_category );
	}
	else
	{
		fprintf(
		 export_handle->notify_stream,
		 "Event category\t\t\t: %" PRIu16 "\n",
		 event_category );
	}
	return( 1 );

on_error:
	if( message_filename != NULL )
	{
		memory_free(
		 message_filename );
	}
	return( -1 );
}

/* Exports the record event message
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_record_event_message(
     export_handle_t *export_handle,
     libevt_record_t *record,
     const system_character_t *event_source,
     size_t event_source_length,
     uint32_t event_identifier,
     log_handle_t *log_handle EVTTOOLS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	system_character_t *message_filename   = NULL;
	system_character_t *parameter_filename = NULL;
	message_string_t *message_string       = NULL;
	system_character_t *value_string       = NULL;
	static char *function                  = "export_handle_export_record_event_message";
	size_t message_filename_size           = 0;
	size_t parameter_filename_size         = 0;
	size_t value_string_size               = 0;
	int number_of_strings                  = 0;
	int result                             = 0;
	int value_string_index                 = 0;

	EVTTOOLS_UNREFERENCED_PARAMETER( log_handle )

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( event_source != NULL )
	{
		result = message_handle_get_value_by_event_source(
		          export_handle->message_handle,
		          event_source,
		          event_source_length,
		          _SYSTEM_STRING( "EventMessageFile" ),
		          16,
		          &message_filename,
		          &message_filename_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve event message filename.",
			 function );

			goto on_error;
		}
	}
	if( message_filename != NULL )
	{
		fprintf(
		 export_handle->notify_stream,
		 "Message file(s)\t\t\t: %" PRIs_SYSTEM "\n",
		 message_filename );

		result = message_handle_get_message_string(
			  export_handle->message_handle,
			  message_filename,
			  message_filename_size - 1,
			  event_identifier,
			  &message_string,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message string: 0x%08" PRIx32 " from: %" PRIs_SYSTEM ".",
			 function,
			 event_identifier,
			 message_filename );

			goto on_error;
		}
		memory_free(
		 message_filename );

		message_filename      = NULL;
		message_filename_size = 0;
	}
	if( event_source != NULL )
	{
		result = message_handle_get_value_by_event_source(
		          export_handle->message_handle,
		          event_source,
		          event_source_length,
		          _SYSTEM_STRING( "ParameterMessageFile" ),
		          20,
		          &parameter_filename,
		          &parameter_filename_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve parameter message filename.",
			 function );

			goto on_error;
		}
	}
	if( parameter_filename != NULL )
	{
		fprintf(
		 export_handle->notify_stream,
		 "Parameter file(s)\t\t: %" PRIs_SYSTEM "\n",
		 parameter_filename );
	}
	if( libevt_record_get_number_of_strings(
	     record,
	     &number_of_strings,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of strings in record.",
		 function );

		goto on_error;
	}
	fprintf(
	 export_handle->notify_stream,
	 "Number of strings\t\t: %d\n",
	 number_of_strings );

	for( value_string_index = 0;
	     value_string_index < number_of_strings;
	     value_string_index++ )
	{
		fprintf(
		 export_handle->notify_stream,
		 "String: %d\t\t\t: ",
		 value_string_index + 1 );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libevt_record_get_utf16_string_size(
			  record,
			  value_string_index,
			  &value_string_size,
			  error );
#else
		result = libevt_record_get_utf8_string_size(
			  record,
			  value_string_index,
			  &value_string_size,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string: %d size.",
			 function,
			 value_string_index );

			goto on_error;
		}
		if( value_string_size > 0 )
		{
			value_string = system_string_allocate(
					value_string_size );

			if( value_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create value string.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libevt_record_get_utf16_string(
				  record,
				  value_string_index,
				  (uint16_t *) value_string,
				  value_string_size,
				  error );
#else
			result = libevt_record_get_utf8_string(
				  record,
				  value_string_index,
				  (uint8_t *) value_string,
				  value_string_size,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve string: %d.",
				 function,
				 value_string_index );

				goto on_error;
			}
			fprintf(
			 export_handle->notify_stream,
			 "%" PRIs_SYSTEM "",
			 value_string );

			memory_free(
			 value_string );

			value_string = NULL;
		}
		fprintf(
		 export_handle->notify_stream,
		 "\n" );
	}
	if( message_string != NULL )
	{
		if( export_handle_print_message(
		     export_handle,
		     parameter_filename,
		     parameter_filename_size - 1,
		     message_string,
		     record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print message.",
			 function );

			goto on_error;
		}
	}
	if( parameter_filename != NULL )
	{
		memory_free(
		 parameter_filename );
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( parameter_filename != NULL )
	{
		memory_free(
		 parameter_filename );
	}
	if( message_filename != NULL )
	{
		memory_free(
		 message_filename );
	}
	return( -1 );
}

/* Exports the record
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_record(
     export_handle_t *export_handle,
     libevt_record_t *record,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t *source_name       = NULL;
	system_character_t *value_string      = NULL;
	static char *function                 = "export_handle_export_record";
	size_t source_name_size               = 0;
	size_t value_string_size              = 0;
	uint32_t event_identifier             = 0;
	uint32_t value_32bit                  = 0;
	uint16_t event_type                   = 0;
	int result                            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( libevt_record_get_identifier(
	     record,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	fprintf(
	 export_handle->notify_stream,
	 "Event number\t\t\t: %" PRIu32 "\n",
	 value_32bit );

	if( libevt_record_get_creation_time(
	     record,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		goto on_error;
	}
	if( export_handle_posix_time_value_fprint(
	     export_handle,
	     "Creation time\t\t\t",
	     value_32bit,
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
	if( libevt_record_get_written_time(
	     record,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve written time.",
		 function );

		goto on_error;
	}
	if( export_handle_posix_time_value_fprint(
	     export_handle,
	     "Written time\t\t\t",
	     value_32bit,
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
	if( libevt_record_get_event_type(
	     record,
	     &event_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event type.",
		 function );

		goto on_error;
	}
	fprintf(
	 export_handle->notify_stream,
	 "Event type\t\t\t: %s (%" PRIu16 ")\n",
	 export_handle_get_event_type(
	  event_type ),
	 event_type );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libevt_record_get_utf16_user_security_identifier_size(
	          record,
	          &value_string_size,
	          error );
#else
	result = libevt_record_get_utf8_user_security_identifier_size(
	          record,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve user security identifier size.",
		 function );

		goto on_error;
	}
	if( ( result != 0 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libevt_record_get_utf16_user_security_identifier(
		          record,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libevt_record_get_utf8_user_security_identifier(
		          record,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve user security identifier.",
			 function );

			goto on_error;
		}
		fprintf(
		 export_handle->notify_stream,
		 "User security identifier\t: %" PRIs_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libevt_record_get_utf16_computer_name_size(
	          record,
	          &value_string_size,
	          error );
#else
	result = libevt_record_get_utf8_computer_name_size(
	          record,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve computer name size.",
		 function );

		goto on_error;
	}
	if( ( result != 0 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libevt_record_get_utf16_computer_name(
		          record,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libevt_record_get_utf8_computer_name(
		          record,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve computer name.",
			 function );

			goto on_error;
		}
		fprintf(
		 export_handle->notify_stream,
		 "Computer name\t\t\t: %" PRIs_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libevt_record_get_utf16_source_name_size(
	          record,
	          &source_name_size,
	          error );
#else
	result = libevt_record_get_utf8_source_name_size(
	          record,
	          &source_name_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve source name size.",
		 function );

		goto on_error;
	}
	if( ( result != 0 )
	 && ( source_name_size > 0 ) )
	{
		source_name = system_string_allocate(
		               source_name_size );

		if( source_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create event source.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libevt_record_get_utf16_source_name(
		          record,
		          (uint16_t *) source_name,
		          source_name_size,
		          error );
#else
		result = libevt_record_get_utf8_source_name(
		          record,
		          (uint8_t *) source_name,
		          source_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve source name.",
			 function );

			goto on_error;
		}
		fprintf(
		 export_handle->notify_stream,
		 "Source name\t\t\t: %" PRIs_SYSTEM "\n",
		 source_name );
	}
	if( export_handle_export_record_event_category(
	     export_handle,
	     record,
	     source_name,
	     source_name_size - 1,
	     log_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export event category.",
		 function );

		goto on_error;
	}
	if( libevt_record_get_event_identifier(
	     record,
	     &event_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event identifier.",
		 function );

		goto on_error;
	}
	fprintf(
	 export_handle->notify_stream,
	 "Event identifier\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
	 event_identifier,
	 event_identifier );

	if( export_handle_export_record_event_message(
	     export_handle,
	     record,
	     source_name,
	     source_name_size - 1,
	     event_identifier,
	     log_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export event message.",
		 function );

		goto on_error;
	}
	fprintf(
	 export_handle->notify_stream,
	 "\n" );

	if( source_name != NULL )
	{
		memory_free(
		 source_name );

		source_name = NULL;
	}
	return( 1 );

on_error:
	if( source_name != NULL )
	{
		memory_free(
		 source_name );
	}
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints the message
 * Returns 1 if successful or -1 on error
 */
int export_handle_print_message(
     export_handle_t *export_handle,
     const system_character_t *parameter_filename,
     size_t parameter_filename_length,
     message_string_t *message_string,
     libevt_record_t *record,
     libcerror_error_t **error )
{
	system_character_t *string         = NULL;
	system_character_t *value_string   = NULL;
	static char *function              = "export_handle_print_message";
	size_t conversion_specifier_length = 0;
	size_t string_length               = 0;
	size_t string_index                = 0;
	size_t value_string_size           = 0;
	system_character_t last_character  = 0;
	int number_of_strings              = 0;
	int result                         = 0;
	int value_string_index             = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( message_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message string.",
		 function );

		return( -1 );
	}
	if( libevt_record_get_number_of_strings(
	     record,
	     &number_of_strings,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of strings in record.",
		 function );

		goto on_error;
	}
	/* Make a copy of the message string in case it is cached out by parameter messagee string lookups
	 */
	string = system_string_allocate(
	          message_string->string_size );

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.",
		 function );

		goto on_error;
	}
	string_length = message_string->string_size - 1;
	string_index  = 0;

	if( system_string_copy(
	     string,
	     message_string->string,
	     string_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy message string.",
		 function );

		goto on_error;
	}
	string[ string_length ] = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	fprintf(
	 export_handle->notify_stream,
	 "Message format string\t\t: %" PRIs_SYSTEM "\n",
	 string );
#endif
	fprintf(
	 export_handle->notify_stream,
	 "Message string\t\t\t: " );

	while( string_index < string_length )
	{
		if( export_handle_print_message_string(
		     export_handle,
		     string,
		     string_length,
		     &string_index,
		     &last_character,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print message string.",
			 function );

			goto on_error;
		}
		if( ( string[ string_index ] == (system_character_t) '%' )
		 && ( ( string_index + 1 ) < string_length ) )
		{
			if( ( string[ string_index + 1 ] < (system_character_t) '1' )
			 || ( string[ string_index + 1 ] > (system_character_t) '9' ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported conversion specifier: %" PRIs_SYSTEM ".",
				 function,
				 &( string[ string_index ] ) );

				goto on_error;
			}
			value_string_index = (int) string[ string_index + 1 ] - (int) '0';

			conversion_specifier_length = 2;

			if( ( ( string_index + 3 ) < string_length )
			 && ( string[ string_index + 2 ] >= (system_character_t) '0' )
			 && ( string[ string_index + 2 ] <= (system_character_t) '9' ) )
			{
				value_string_index *= 10;
				value_string_index += (int) string[ string_index + 2 ] - (int) '0';

				conversion_specifier_length += 1;
			}
			value_string_index -= 1;

			if( ( ( conversion_specifier_length + 3 ) < ( string_length - string_index ) )
			 && ( string[ string_index + conversion_specifier_length ] == (system_character_t) '!' ) )
			{
				if( ( string[ string_index + conversion_specifier_length + 1 ] != (system_character_t) 's' )
				 || ( string[ string_index + conversion_specifier_length + 2 ] != (system_character_t) '!' ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported conversion specifier: %" PRIs_SYSTEM ".",
					 function,
					 &( string[ string_index ] ) );

					goto on_error;
				}
				conversion_specifier_length += 3;
			}
/* TODO remove index check after user data support */
			if( value_string_index < number_of_strings )
			{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
				result = libevt_record_get_utf16_string_size(
					  record,
					  value_string_index,
					  &value_string_size,
					  error );
#else
				result = libevt_record_get_utf8_string_size(
					  record,
					  value_string_index,
					  &value_string_size,
					  error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve string: %d size.",
					 function,
					 value_string_index );

					goto on_error;
				}
				if( value_string_size > 0 )
				{
					value_string = system_string_allocate(
							value_string_size );

					if( value_string == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						 "%s: unable to create value string.",
						 function );

						goto on_error;
					}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result = libevt_record_get_utf16_string(
						  record,
						  value_string_index,
						  (uint16_t *) value_string,
						  value_string_size,
						  error );
#else
					result = libevt_record_get_utf8_string(
						  record,
						  value_string_index,
						  (uint8_t *) value_string,
						  value_string_size,
						  error );
#endif
					if( result != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve string: %d.",
						 function,
						 value_string_index );

						goto on_error;
					}
					if( export_handle_print_value_string(
					     export_handle,
					     parameter_filename,
					     parameter_filename_length,
					     value_string,
					     value_string_size - 1,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
						 "%s: unable to print string: %d.",
						 function,
						 value_string_index );

						goto on_error;
					}
					memory_free(
					 value_string );

					value_string = NULL;
				}
				string_index += conversion_specifier_length;
			}
			else
			{
				do
				{
					fprintf(
					 export_handle->notify_stream,
					 "%" PRIc_SYSTEM "",
					 string[ string_index++ ] );

					conversion_specifier_length--;
				}
				while( conversion_specifier_length > 0 );

				last_character = string[ string_index ];
			}
		}
	}
	fprintf(
	 export_handle->notify_stream,
	 "\n" );

	memory_free(
	 string );

	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( string != NULL )
	{
		memory_free(
		 string );
	}
	return( -1 );
}

/* Prints a message string upto the next placeholder (%#)
 * Returns 1 if successful or -1 on error
 */
int export_handle_print_message_string(
     export_handle_t *export_handle,
     system_character_t *string,
     size_t string_length,
     size_t *string_index,
     system_character_t *last_character,
     libcerror_error_t **error )
{
	static char *function                  = "export_handle_print_message_string";
	size_t safe_string_index               = 0;
	system_character_t safe_last_character = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string index.",
		 function );

		return( -1 );
	}
	safe_string_index = *string_index;

	if( last_character == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid last character.",
		 function );

		return( -1 );
	}
	safe_last_character = *last_character;

	while( safe_string_index < string_length )
	{
		if( string[ safe_string_index ] == 0 )
		{
			break;
		}
		if( ( string[ safe_string_index ] == (system_character_t) '%' )
		 && ( ( safe_string_index + 1 ) < string_length ) )
		{
			/* Ignore %0 = end of string, %r = cariage return */
			if( ( string[ safe_string_index + 1 ] == (system_character_t) '0' )
			 || ( string[ safe_string_index + 1 ] == (system_character_t) 'r' ) )
			{
				safe_string_index += 2;

				continue;
			}
			/* Replace:
			 *  %<space> = <space>
			 *  %! = !
			 *  %% = %
			 *  %. = .
			 */
			if( ( string[ safe_string_index + 1 ] == (system_character_t) ' ' )
			 || ( string[ safe_string_index + 1 ] == (system_character_t) '!' )
			 || ( string[ safe_string_index + 1 ] == (system_character_t) '%' )
			 || ( string[ safe_string_index + 1 ] == (system_character_t) '.' ) )
			{
				safe_last_character = string[ safe_string_index + 1 ];

				fprintf(
				 export_handle->notify_stream,
				 "%c",
				 safe_last_character );

				safe_string_index += 2;

				continue;
			}
			/* Replace %b = <space> */
			if( string[ safe_string_index + 1 ] == (system_character_t) 'b' )
			{
				safe_last_character = (system_character_t) ' ';

				fprintf(
				 export_handle->notify_stream,
				 "%c",
				 safe_last_character );

				safe_string_index += 2;

				continue;
			}
			/* Replace %n = <new line> */
			if( string[ safe_string_index + 1 ] == (system_character_t) 'n' )
			{
				if( safe_last_character != (system_character_t) '\n' )
				{
					safe_last_character = (system_character_t) '\n';

					fprintf(
					 export_handle->notify_stream,
					 "%c",
					 safe_last_character );
				}
				safe_string_index += 2;

				continue;
			}
			/* Replace %t = <tab> */
			if( string[ safe_string_index + 1 ] == (system_character_t) 't' )
			{
				safe_last_character = (system_character_t) '\t';

				fprintf(
				 export_handle->notify_stream,
				 "%c",
				 safe_last_character );

				safe_string_index += 2;

				continue;
			}
			break;
		}
		if( string[ safe_string_index ] == (system_character_t) '\r' )
		{
			/* Ignore \r characters */
		}
		else if( ( string[ safe_string_index ] == (system_character_t) '\n' )
		      && ( safe_last_character == (system_character_t) '\n' ) )
		{
			/* Ignore multiple \n characters */
		}
		else
		{
			fprintf(
			 export_handle->notify_stream,
			 "%" PRIc_SYSTEM "",
			 string[ safe_string_index ] );

			safe_last_character = string[ safe_string_index ];
		}
		safe_string_index += 1;
	}
	*string_index   = safe_string_index;
	*last_character = safe_last_character;

	return( 1 );
}

/* Prints a value string
 * Returns 1 if successful or -1 on error
 */
int export_handle_print_value_string(
     export_handle_t *export_handle,
     const system_character_t *parameter_filename,
     size_t parameter_filename_length,
     system_character_t *string,
     size_t string_length,
     libcerror_error_t **error )
{
	message_string_t *parameter_string          = NULL;
	static char *function                       = "export_handle_print_value_string";
	system_character_t parameter_last_character = 0;
	size_t parameter_string_index               = 0;
	uint64_t value_64bit                        = 0;
	int result                                  = 0;

	if( ( string_length > 2 )
	 && ( string[ 0 ] == '%' )
	 && ( string[ 1 ] == '%' ) )
	{
		if( evttools_system_string_copy_from_64_bit_in_decimal(
		     &( string[ 2 ] ),
		     string_length - 2,
		     &value_64bit,
		     NULL ) == 1 )
		{
			if( value_64bit > (uint64_t) UINT32_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid parameter message identifier value out of bounds.",
				 function );

				return( -1 );
			}
			result = 1;
		}
	}
	if( result != 0 )
	{
		if( parameter_filename == NULL )
		{
			result = 0;
		}
		else
		{
			result = message_handle_get_message_string(
				  export_handle->message_handle,
				  parameter_filename,
				  parameter_filename_length,
				  (uint32_t) value_64bit,
				  &parameter_string,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve parameter string: 0x%08" PRIx64 " from: %" PRIs_SYSTEM ".",
				 function,
				 value_64bit,
				 parameter_filename );

				return( -1 );
			}
		}
	}
	if( result == 0 )
	{
		fprintf(
		 export_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 string );
	}
	else
	{
		if( parameter_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing parameter string.",
			 function );

			return( -1 );
		}
		if( export_handle_print_message_string(
		     export_handle,
		     parameter_string->string,
		     parameter_string->string_size - 1,
		     &parameter_string_index,
		     &parameter_last_character,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print parameter string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Exports the records
 * Returns the 1 if successful, 0 if no records are available or -1 on error
 */
int export_handle_export_records(
     export_handle_t *export_handle,
     libevt_file_t *file,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libevt_record_t *record = NULL;
	static char *function   = "export_handle_export_records";
	int number_of_records   = 0;
	int record_index        = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( libevt_file_get_number_of_records(
	     file,
	     &number_of_records,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of records.",
		 function );

		return( -1 );
	}
	if( number_of_records == 0 )
	{
		return( 0 );
	}
	for( record_index = 0;
	     record_index < number_of_records;
	     record_index++ )
	{
		if( export_handle->abort != 0 )
		{
			return( -1 );
		}
		if( libevt_file_get_record_by_index(
		     file,
		     record_index,
		     &record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record: %d.",
			 function,
			 record_index );

			return( -1 );
		}
		if( export_handle_export_record(
		     export_handle,
		     record,
		     log_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export record: %d.",
			 function,
			 record_index );

#if defined( HAVE_DEBUG_OUTPUT )
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
#endif
			libcerror_error_free(
			 error );
		}
		if( libevt_record_free(
		     &record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record: %d.",
			 function,
			 record_index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Exports the recovered records
 * Returns the 1 if successful, 0 if no records are available or -1 on error
 */
int export_handle_export_recovered_records(
     export_handle_t *export_handle,
     libevt_file_t *file,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libevt_record_t *record = NULL;
	static char *function   = "export_handle_export_recovered_records";
	int number_of_records   = 0;
	int record_index        = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( libevt_file_get_number_of_recovered_records(
	     file,
	     &number_of_records,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of recovered records.",
		 function );

		return( -1 );
	}
	if( number_of_records == 0 )
	{
		return( 0 );
	}
	for( record_index = 0;
	     record_index < number_of_records;
	     record_index++ )
	{
		if( export_handle->abort != 0 )
		{
			return( -1 );
		}
		if( libevt_file_get_recovered_record_by_index(
		     file,
		     record_index,
		     &record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve recovered record: %d.",
			 function,
			 record_index );

			return( -1 );
		}
		if( export_handle_export_record(
		     export_handle,
		     record,
		     log_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export recovered record: %d.",
			 function,
			 record_index );

#if defined( HAVE_DEBUG_OUTPUT )
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
#endif
			libcerror_error_free(
			 error );
		}
		if( libevt_record_free(
		     &record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record: %d.",
			 function,
			 record_index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Exports the records from the file
 * Returns the 1 if successful, 0 if no records are available or -1 on error
 */
int export_handle_export_file(
     export_handle_t *export_handle,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	static char *function        = "export_handle_export_file";
	int result_recovered_records = 0;
	int result_records           = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->export_mode != EXPORT_MODE_RECOVERED )
	{
		result_records = export_handle_export_records(
				  export_handle,
				  export_handle->input_file,
				  log_handle,
				  error );

		if( result_records == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export records.",
			 function );

			return( -1 );
		}
	}
	if( export_handle->export_mode != EXPORT_MODE_ITEMS )
	{
		result_recovered_records = export_handle_export_recovered_records(
					    export_handle,
					    export_handle->input_file,
					    log_handle,
					    error );

		if( result_recovered_records == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export recovered records.",
			 function );

			return( -1 );
		}
	}
	if( ( result_records != 0 )
	 || ( result_recovered_records != 0 ) )
	{
		return( 1 );
	}
	return( 0 );
}

