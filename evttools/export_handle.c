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

const char *export_handle_get_event_log_type(
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
	( *export_handle )->event_log_type = EVTTOOLS_EVENT_LOG_TYPE_UNKNOWN;
	( *export_handle )->ascii_codepage = LIBEVT_CODEPAGE_WINDOWS_1252;
	( *export_handle )->notify_stream  = EXPORT_HANDLE_NOTIFY_STREAM;

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
		if( ( *export_handle )->system_registry_file != NULL )
		{
			if( libregf_file_free(
			     &( ( *export_handle )->system_registry_file ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free system registry file.",
				 function );

				result = -1;
			}
		}
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

	if( export_handle->system_registry_file != NULL )
	{
		if( libregf_file_signal_abort(
		     export_handle->system_registry_file,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal system registry file to abort.",
			 function );

			return( -1 );
		}
	}
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

/* Sets the ascii codepage
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_ascii_codepage(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     liberror_error_t **error )
{
	static char *function = "export_handle_set_ascii_codepage";
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
	result = evtinput_determine_ascii_codepage(
	          string,
	          &( export_handle->ascii_codepage ),
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine ASCII codepage.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the event log type
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_event_log_type(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     liberror_error_t **error )
{
	static char *function = "export_handle_set_event_log_type";
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
	result = evtinput_determine_event_log_type(
	          string,
	          &( export_handle->event_log_type ),
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
     const libcstring_system_character_t *filename,
     liberror_error_t **error )
{
	static char *function = "export_handle_set_event_log_type_from_filename";
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
	result = evtinput_determine_event_log_type_from_filename(
	          filename,
	          &( export_handle->event_log_type ),
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine event log type from filename.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Opens the input
 * Returns 1 if successful or -1 on error
 */
int export_handle_open_input(
     export_handle_t *export_handle,
     const libcstring_system_character_t *filename,
     liberror_error_t **error )
{
	const char *key_path    = NULL;
	libregf_key_t *base_key = NULL;
	libregf_key_t *key      = NULL;
	libregf_key_t *root_key = NULL;
	static char *function   = "export_handle_open_input";
	size_t key_path_length  = 0;
	int number_of_sub_keys  = 0;
	int result              = 0;

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
	if( export_handle->system_registry_filename != NULL )
	{
		if( libregf_file_initialize(
		     &( export_handle->system_registry_file ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize system registry file.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libregf_file_open_wide(
		     export_handle->system_registry_file,
		     export_handle->system_registry_filename,
		     LIBREGF_OPEN_READ,
		     error ) != 1 )
#else
		if( libregf_file_open(
		     export_handle->system_registry_file,
		     export_handle->system_registry_filename,
		     LIBREGF_OPEN_READ,
		     error ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open system registry file.",
			 function );

			goto on_error;
		}
		if( libregf_file_get_root_key(
		     export_handle->system_registry_file,
		     &root_key,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root key.",
			 function );

			goto on_error;
		}
		if( libregf_key_get_number_of_sub_keys(
		     root_key,
		     &number_of_sub_keys,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub keys.",
			 function );

			goto on_error;
		}
		if( number_of_sub_keys == 1 )
		{
			if( libregf_key_get_sub_key(
			     root_key,
			     0,
			     &base_key,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve base key.",
				 function );

				goto on_error;
			}
		}
		else if( number_of_sub_keys > 1 )
		{
			base_key = root_key;
		}
		key_path = "ControlSet001\\Services\\Eventlog";

		key_path_length = libcstring_narrow_string_length(
		                   key_path );

		result = libregf_key_get_sub_key_by_utf8_path(
		          base_key,
		          (uint8_t *) key_path,
		          key_path_length,
		          &key,
		          error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key: %s.",
			 function,
			 key_path );

			goto on_error;
		}
		else if( result != 0 )
		{
			key_path = export_handle_get_event_log_type(
			            export_handle->event_log_type );

			key_path_length = libcstring_system_string_length(
					   key_path );

			result = libregf_key_get_sub_key_by_utf8_name(
				  key,
				  (uint8_t *) key_path,
				  key_path_length,
				  &( export_handle->control_set1_key ),
				  error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub key: %s.",
				 function,
				 key_path );

				goto on_error;
			}
		}
		if( libregf_key_free(
		     &key,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			goto on_error;
		}
		key_path = "ControlSet002\\Services\\Eventlog";

		key_path_length = libcstring_system_string_length(
		                   key_path );

		result = libregf_key_get_sub_key_by_utf8_path(
		          base_key,
		          (uint8_t *) key_path,
		          key_path_length,
		          &key,
		          error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key: %s.",
			 function,
			 key_path );

			goto on_error;
		}
		else if( result != 0 )
		{
			key_path = export_handle_get_event_log_type(
			            export_handle->event_log_type );

			key_path_length = libcstring_system_string_length(
					   key_path );

			result = libregf_key_get_sub_key_by_utf8_name(
				  key,
				  (uint8_t *) key_path,
				  key_path_length,
				  &( export_handle->control_set2_key ),
				  error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub key: %s.",
				 function,
				 key_path );

				goto on_error;
			}
		}
		if( libregf_key_free(
		     &key,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			goto on_error;
		}
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

		goto on_error;
	}
	return( 1 );

on_error:
	if( key != NULL )
	{
		libregf_key_free(
		 &key,
		 NULL );
	}
	if( ( base_key != NULL )
	 && ( base_key != root_key ) )
	{
		libregf_key_free(
		 &base_key,
		 NULL );
	}
	if( root_key != NULL )
	{
		libregf_key_free(
		 &root_key,
		 NULL );
	}
	if( export_handle->control_set1_key != NULL )
	{
		libregf_key_free(
		 &( export_handle->control_set1_key ),
		 NULL );
	}
	if( export_handle->control_set2_key != NULL )
	{
		libregf_key_free(
		 &( export_handle->control_set2_key ),
		 NULL );
	}
	if( export_handle->system_registry_file != NULL )
	{
		libregf_file_free(
		 &( export_handle->system_registry_file ),
		 NULL );
	}
	return( -1 );
}

/* Closes the input
 * Returns the 0 if succesful or -1 on error
 */
int export_handle_close_input(
     export_handle_t *export_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_close_input";
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
	if( export_handle->control_set1_key != NULL )
	{
		if( libregf_key_free(
		     &( export_handle->control_set1_key ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free control set 1 key.",
			 function );

			result = -1;
		}
	}
	if( export_handle->control_set2_key != NULL )
	{
		if( libregf_key_free(
		     &( export_handle->control_set2_key ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free control set 2 key.",
			 function );

			result = -1;
		}
	}
	if( export_handle->system_registry_file != NULL )
	{
		if( libregf_file_close(
		     export_handle->system_registry_file,
		     error ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close system registry file.",
			 function );

			result = -1;
		}
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

		result = -1;
	}
	return( result );
}

/* Retrieves the message filename for a specific event source
 * The message filename is retrieved from the SYSTEM Windows Registry File if available
 * Returns 1 if successful, 0 if such event source or -1 error
 */
int export_handle_get_message_filename(
     export_handle_t *export_handle,
     const libcstring_system_character_t *event_source,
     size_t event_source_length,
     liberror_error_t **error )
{
	libcstring_system_character_t *value_string = NULL;
	size_t value_string_size                    = 0;

	libregf_key_t *key       = NULL;
	libregf_value_t *value   = NULL;
	const char *value_name   = NULL;
	static char *function    = "export_handle_get_message_filename";
	size_t value_name_length = 0;
	int result               = 0;

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
	if( export_handle->control_set1_key != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libregf_key_get_sub_key_by_utf16_name(
			  export_handle->control_set1_key,
			  (uint16_t *) event_source,
			  event_source_length,
			  &key,
			  error );
#else
		result = libregf_key_get_sub_key_by_utf8_name(
			  export_handle->control_set1_key,
			  (uint8_t *) event_source,
			  event_source_length,
			  &key,
			  error );
#endif
		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 event_source );

			goto on_error;
		}
	}
	if( result == 0 )
	{
		if( export_handle->control_set2_key != NULL )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libregf_key_get_sub_key_by_utf16_name(
				  export_handle->control_set2_key,
				  (uint16_t *) event_source,
				  event_source_length,
				  &key,
				  error );
#else
			result = libregf_key_get_sub_key_by_utf8_name(
				  export_handle->control_set2_key,
				  (uint8_t *) event_source,
				  event_source_length,
				  &key,
				  error );
#endif
			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
				 function,
				 event_source );

				goto on_error;
			}
		}
	}
	if( result != 0 )
	{
		value_name = "EventMessageFile";

		value_name_length = libcstring_narrow_string_length(
		                     value_name );

		result = libregf_key_get_value_by_utf8_name(
			  key,
			  (uint8_t *) value_name,
			  value_name_length,
			  &value,
			  error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %s.",
			 function,
			 value_name );

			goto on_error;
		}
		else if( result != 0 )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libregf_value_get_value_utf16_string_size(
			          value,
			          &value_string_size,
			          error );
#else
			result = libregf_value_get_value_utf8_string_size(
			          value,
			          &value_string_size,
			          error );
#endif
			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value string size.",
				 function );

				goto on_error;
			}
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
			result = libregf_value_get_value_utf16_string(
				  value,
				  (uint16_t *) value_string,
				  value_string_size,
				  error );
#else
			result = libregf_value_get_value_utf8_string(
				  value,
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
				 "%s: unable to retrieve value string.",
				 function );

				goto on_error;
			}
			fprintf(
			 export_handle->notify_stream,
			 "Message Filename\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
			 value_string );

			memory_free(
			 value_string );

			value_string = NULL;

			if( libregf_value_free(
			     &value,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value.",
				 function );

				goto on_error;
			}
		}
		if( libregf_key_free(
		     &key,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( value != NULL )
	{
		libregf_value_free(
		 &value,
		 NULL );
	}
	if( key != NULL )
	{
		libregf_key_free(
		 &key,
		 NULL );
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
     liberror_error_t **error )
{
	libcstring_system_character_t posix_time_string[ 32 ];

	libcstring_system_character_t *event_source = NULL;
	libcstring_system_character_t *value_string = NULL;
	libfdatetime_posix_time_t *posix_time       = NULL;
	static char *function                       = "export_handle_export_record";
	size_t event_source_size                    = 0;
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
	if( libevt_record_get_identifier(
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
	 "Event number\t\t: %" PRIu32 "\n",
	 value_32bit );

	if( libevt_record_get_creation_time(
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
	 "Creation time\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
	 posix_time_string );

	if( libevt_record_get_written_time(
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
	 "Written time\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
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
	posix_time = NULL;

	if( libevt_record_get_event_identifier(
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
	 "Event identifier\t: 0x%08" PRIx32 "\n",
	 value_32bit );

	if( libevt_record_get_event_type(
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
	 "Event type\t\t: %s\n",
	 export_handle_get_event_type(
	  event_type ) );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libevt_record_get_utf16_source_name_size(
	          record,
	          &event_source_size,
	          error );
#else
	result = libevt_record_get_utf8_source_name_size(
	          record,
	          &event_source_size,
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
	if( event_source_size > 0 )
	{
		event_source = libcstring_system_string_allocate(
		                event_source_size );

		if( event_source == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create event source.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libevt_record_get_utf16_source_name(
		          record,
		          (uint16_t *) event_source,
		          event_source_size,
		          error );
#else
		result = libevt_record_get_utf8_source_name(
		          record,
		          (uint8_t *) event_source,
		          event_source_size,
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
		 "Source name\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 event_source );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
		 "Computer name\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	if( event_source != NULL )
	{
		result = export_handle_get_message_filename(
		          export_handle,
		          event_source,
		          event_source_size - 1,
		          error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message filename.",
			 function );

			goto on_error;
		}
		memory_free(
		 event_source );

		event_source = NULL;
	}
	fprintf(
	 export_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( event_source != NULL )
	{
		memory_free(
		 event_source );
	}
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

/* Exports the records
 * Returns the 1 if succesful, 0 if no records are available or -1 on error
 */
int export_handle_export_records(
     export_handle_t *export_handle,
     libevt_file_t *file,
     log_handle_t *log_handle,
     liberror_error_t **error )
{
	libevt_record_t *record = NULL;
	static char *function   = "export_handle_export_records";
	int number_of_records   = 0;
	int record_index        = 0;

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
	if( libevt_file_get_number_of_records(
	     file,
	     &number_of_records,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		if( libevt_file_get_record(
		     file,
		     record_index,
		     &record,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
/* TODO add error tollerance and log error */
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export record: %d.",
			 function,
			 record_index );

			return( -1 );
		}
		if( libevt_record_free(
		     &record,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record: %d.",
			 function,
			 record_index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Exports the records from the file
 * Returns the 1 if succesful, 0 if no records are available or -1 on error
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
	result = export_handle_export_records(
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
		 "%s: unable to export records.",
		 function );

		return( -1 );
	}
	return( result );
}

