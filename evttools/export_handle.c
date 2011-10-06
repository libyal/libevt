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

#include "export_handle.h"
#include "log_handle.h"
#include "evtinput.h"
#include "evttools_libevt.h"

#define EXPORT_HANDLE_NOTIFY_STREAM	stdout

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
	if( *export_handle == NULL )
	{
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
		( *export_handle )->dump_item_values         = 1;
		( *export_handle )->print_status_information = 1;
		( *export_handle )->notify_stream            = EXPORT_HANDLE_NOTIFY_STREAM;
	}
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
		if( ( *export_handle )->input_file != NULL )
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

/* Sets the target path
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_target_path(
     export_handle_t *export_handle,
     const libcstring_system_character_t *target_path,
     liberror_error_t **error )
{
	static char *function                                      = "export_handle_set_target_path";
	size_t target_path_length                                  = 0;

#if defined( WINAPI )
	libcstring_system_character_t *extended_length_target_path = NULL;
        size_t extended_length_target_path_size                    = 0;
	int result                                                 = 0;
#endif

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
	if( target_path == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid target path.",
		 function );

		return( -1 );
	}
	if( export_handle->target_path != NULL )
	{
		memory_free(
		 export_handle->target_path );

		export_handle->target_path      = NULL;
		export_handle->target_path_size = 0;
	}
	target_path_length = libcstring_system_string_length(
	                      target_path );

#if defined( WINAPI )
	result = libsystem_path_create_windows_extended(
	          target_path,
                  target_path_length,
                  &extended_length_target_path,
                  &extended_length_target_path_size,
                  error );

        if( result == -1 )
        {
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create extended-length target path.",
		 function );

		goto on_error;
        }
        else if( result != 0 )
        {
                target_path        = extended_length_target_path;
                target_path_length = extended_length_target_path_size - 1;
        }
#endif
	if( target_path_length > 0 )
	{
		export_handle->target_path = libcstring_system_string_allocate(
		                              target_path_length + 1 );

		if( export_handle->target_path == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create target path.",
			 function );

			goto on_error;
		}
		if( libcstring_system_string_copy(
		     export_handle->target_path,
		     target_path,
		     target_path_length ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy target path.",
			 function );

			goto on_error;
		}
		( export_handle->target_path )[ target_path_length ] = 0;

		export_handle->target_path_size = target_path_length + 1;
	}
#if defined( WINAPI )
	memory_free(
	 extended_length_target_path );
#endif
	return( 1 );

on_error:
	if( export_handle->target_path != NULL )
	{
		memory_free(
		 export_handle->target_path );

		export_handle->target_path      = NULL;
		export_handle->target_path_size = 0;
	}
#if defined( WINAPI )
	if( extended_length_target_path != NULL )
	{
		memory_free(
		 extended_length_target_path );
	}
#endif
	return( -1 );
}

/* Sets an export path consisting of a base path and a suffix
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_export_path(
     export_handle_t *export_handle,
     const libcstring_system_character_t *base_path,
     size_t base_path_length,
     const libcstring_system_character_t *suffix,
     size_t suffix_length,
     libcstring_system_character_t **export_path,
     size_t *export_path_size,
     liberror_error_t **error )
{
	static char *function = "export_handle_set_export_path";

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
	if( base_path == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base path.",
		 function );

		return( -1 );
	}
	if( base_path_length > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid base path length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( suffix == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid suffix.",
		 function );

		return( -1 );
	}
	if( suffix_length > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid suffix length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( export_path_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path size.",
		 function );

		return( -1 );
	}
	if( *export_path != NULL )
	{
		memory_free(
		 *export_path );

		*export_path      = NULL;
		*export_path_size = 0;
	}
	*export_path_size = base_path_length + suffix_length + 1;

	*export_path = libcstring_system_string_allocate(
	                *export_path_size );

	if( *export_path == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create export path.",
		 function );

		goto on_error;
	}
	if( libcstring_system_string_copy(
	     *export_path,
	     base_path,
	     base_path_length ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy base path to item export path.",
		 function );

		goto on_error;
	}
	if( libcstring_system_string_copy(
	     &( ( *export_path )[ base_path_length ] ),
	     suffix,
	     suffix_length ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy suffix to item export path.",
		 function );

		goto on_error;
	}
	( *export_path )[ *export_path_size - 1 ] = 0;

	return( 1 );

on_error:
	if( *export_path != NULL )
	{
		memory_free(
		 *export_path );

		*export_path      = NULL;
		*export_path_size = 0;
	}
	return( -1 );
}

/* Creates the items export path
 * Returns 1 if successful, 0 if already exists or -1 on error
 */
int export_handle_create_items_export_path(
     export_handle_t *export_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_create_items_export_path";
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
	if( export_handle_set_export_path(
	     export_handle,
	     export_handle->target_path,
	     export_handle->target_path_size - 1,
	     _LIBCSTRING_SYSTEM_STRING( ".export" ),
	     7,
	     &( export_handle->items_export_path ),
	     &( export_handle->items_export_path_size ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set items export path.",
		 function );

		return( -1 );
	}
	result = libsystem_file_exists(
		  export_handle->items_export_path,
		  error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if %" PRIs_LIBCSTRING_SYSTEM " exists.",
		 function,
		 export_handle->items_export_path );

		return( -1 );
	}
	else if( result == 1 )
	{
		return( 0 );
	}
	return( 1 );
}

/* Makes (creates) a directory
 * Returns 1 if successful or -1 on error
 */
int export_handle_make_directory(
     export_handle_t *export_handle,
     const libcstring_system_character_t *directory_name,
     log_handle_t *log_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_make_directory";

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
	if( directory_name == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory name.",
		 function );

		return( -1 );
	}
	if( libsystem_directory_make(
	     directory_name ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to make directory: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 directory_name );

		return( -1 );
	}
	log_handle_printf(
	 log_handle,
	 "Created directory: %" PRIs_LIBCSTRING_SYSTEM ".\n",
	 directory_name );

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

/* Opens the export handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_open(
     export_handle_t *export_handle,
     const libcstring_system_character_t *filename,
     liberror_error_t **error )
{
	static char *function = "export_handle_open";

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

/* Closes the export handle
 * Returns the 0 if succesful or -1 on error
 */
int export_handle_close(
     export_handle_t *export_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_close";

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

/* Creates the default item directory path
 * Returns 1 if successful or -1 on error
 */
int export_handle_create_default_item_directory(
     export_handle_t *export_handle,
     int item_index,
     const libcstring_system_character_t *item_prefix,
     size_t item_prefix_length,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     libcstring_system_character_t **item_directory_path,
     size_t *item_directory_path_size,
     log_handle_t *log_handle,
     liberror_error_t **error )
{
	libcstring_system_character_t item_directory_name[ 64 ];

	static char *function             = "export_handle_create_default_item_directory";
	size_t item_directory_name_length = 0;
	int print_count                   = 0;
	int result                        = 0;

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
	if( item_index < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid item index value less than zero.",
		 function );

		return( -1 );
	}
	if( item_prefix == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item prefix.",
		 function );

		return( -1 );
	}
	if( item_prefix_length > (size_t) 63 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid item prefix length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( item_directory_path == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item directory path.",
		 function );

		return( -1 );
	}
	/* Create the item directory
	 */
	print_count = libcstring_system_string_sprintf(
	               item_directory_name,
	               64,
	               _LIBCSTRING_SYSTEM_STRING( "%s%05d" ),
	               item_prefix,
	               item_index + 1 );

	if( ( print_count < 0 )
	 || ( (size_t) print_count > ( item_prefix_length + 5 ) ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set item directory name.",
		 function );

		goto on_error;
	}
	item_directory_name[ item_prefix_length + 5 ] = 0;
	item_directory_name_length                    = item_prefix_length + 5;

	if( libsystem_path_create(
	     item_directory_name,
	     item_directory_name_length,
	     export_path,
	     export_path_length,
	     item_directory_path,
	     item_directory_path_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable create item directory path.",
		 function );

		goto on_error;
	}
	result = libsystem_file_exists(
	          *item_directory_path,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if %" PRIs_LIBCSTRING_SYSTEM " exists.",
		 function,
		 *item_directory_path );

		goto on_error;
	}
	else if( result != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_GENERIC,
		 "%s: %" PRIs_LIBCSTRING_SYSTEM " already exists.",
		 function,
		 *item_directory_path );

		goto on_error;
	}
	if( export_handle_make_directory(
	     export_handle,
	     *item_directory_path,
	     log_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to create directory: %" PRIs_LIBCSTRING_SYSTEM "",
		 function,
		 *item_directory_path );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *item_directory_path != NULL )
	{
		memory_free(
		 *item_directory_path );

		*item_directory_path      = NULL;
		*item_directory_path_size = 0;
	}
	return( -1 );
}

/* Creates a text item file
 * Returns 1 if successful, 0 if the file already exists or -1 on error
 */
int export_handle_create_text_item_file(
     export_handle_t *export_handle,
     const libcstring_system_character_t *item_filename,
     size_t item_filename_length,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     FILE **item_file_stream,
     liberror_error_t **error )
{
	libcstring_system_character_t *item_filename_path = NULL;
	static char *function                             = "export_handle_create_text_item_file";
	size_t item_filename_path_size                    = 0;
	int result                                        = 0;

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
	if( item_file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item file stream.",
		 function );

		return( -1 );
	}
	if( libsystem_path_create(
	     item_filename,
	     item_filename_length,
	     export_path,
	     export_path_length,
	     &item_filename_path,
	     &item_filename_path_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item filename path.",
		 function );

		goto on_error;
	}
	result = libsystem_file_exists(
	          item_filename_path,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if %" PRIs_LIBCSTRING_SYSTEM " exists.",
		 function,
		 item_filename_path );

		goto on_error;
	}
	else if( result != 0 )
	{
		memory_free(
		 item_filename_path );

		return( 0 );
	}
	*item_file_stream = libsystem_file_stream_open(
	                     item_filename_path,
	                     _LIBCSTRING_SYSTEM_STRING( "w" ) );

	if( *item_file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 item_filename_path );

		goto on_error;
	}
	memory_free(
	 item_filename_path );

	item_filename_path = NULL;

	return( 1 );

on_error:
	if( item_filename_path != NULL )
	{
		memory_free(
		 item_filename_path );
	}
	return( -1 );
}

/* Exports the item values
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_item_values(
     export_handle_t *export_handle,
     libevt_item_t *item,
     const libcstring_system_character_t *item_values_filename,
     size_t item_values_filename_length,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     liberror_error_t **error )
{
	FILE *item_values_file_stream = NULL;
	uint8_t *value_data           = NULL;
	static char *function         = "export_handle_export_item_values";
	size_t value_data_size        = 0;
	uint32_t entry_iterator       = 0;
	uint32_t entry_type           = 0;
	uint32_t number_of_entries    = 0;
	uint32_t value_type           = LIBEVT_VALUE_TYPE_UNSPECIFIED;
	int result                    = 0;

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
	if( item == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	if( item_values_filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item value filename.",
		 function );

		return( -1 );
	}
	result = export_handle_create_text_item_file(
	          export_handle,
                  item_values_filename,
                  item_values_filename_length,
	          export_path,
	          export_path_length,
	          &item_values_file_stream,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item values file.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping item values file: %" PRIs_LIBCSTRING_SYSTEM " it already exists.\n",
		 item_values_filename );

		return( 1 );
	}
	if( libevt_item_get_number_of_entries(
	     item,
	     &number_of_entries,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		goto on_error;
	}
	fprintf(
	 item_values_file_stream,
	 "Number of entries:\t%" PRIu32 "\n",
	 number_of_entries );
	fprintf(
	 item_values_file_stream,
	 "\n" );

	for( entry_iterator = 0;
	     entry_iterator < number_of_entries;
	     entry_iterator++ )
	{
		fprintf(
		 item_values_file_stream,
		 "Entry:\t\t\t%" PRIu32 "\n",
		 entry_iterator );

		if( libevt_item_get_entry_type(
		     item,
		     entry_iterator,
		     &entry_type,
		     &value_type,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry type of entry: %" PRIu32 ".",
			 function,
			 entry_iterator );

			goto on_error;
		}
		fprintf(
		 item_values_file_stream,
		 "Entry type:\t\t0x%04" PRIx32 "\n",
		 entry_type );
		fprintf(
		 item_values_file_stream,
		 "Value type:\t\t0x%04" PRIx32 "\n",
		 value_type );

		result = libevt_item_get_entry_value(
			  item,
			  entry_type,
			  &value_type,
			  &value_data,
			  &value_data_size,
			  0,
			  error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry value of entry: %" PRIu32 ".",
			 function,
			 entry_iterator );

			goto on_error;
		}
		fprintf(
		 item_values_file_stream,
		 "Value:\n" );
		export_handle_print_data(
		 export_handle,
		 item_values_file_stream,
		 value_data,
		 value_data_size,
		 NULL );
	}
	if( libsystem_file_stream_close(
	     item_values_file_stream ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item values file.",
		 function );

		item_values_file_stream = NULL;

		goto on_error;
	}
	item_values_file_stream = NULL;

	return( 1 );
on_error:
	if( item_values_file_stream != NULL )
	{
		libsystem_file_stream_close(
		 item_values_file_stream );
	}
	return( -1 );
}

/* Exports the alias
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_alias(
     export_handle_t *export_handle,
     libevt_item_t *alias,
     int alias_index,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     liberror_error_t **error )
{
	libcstring_system_character_t *alias_path = NULL;
	static char *function                     = "export_handle_export_alias";
	size_t alias_path_size                    = 0;

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
	if( alias == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alias.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	log_handle_printf(
	 log_handle,
	 "Processing alias: %05d in path: %" PRIs_LIBCSTRING_SYSTEM "%c\n",
	 alias_index,
	 export_path,
	 LIBSYSTEM_PATH_SEPARATOR );

	if( export_handle_create_default_item_directory(
	     export_handle,
	     alias_index,
	     _LIBCSTRING_SYSTEM_STRING( "Alias" ),
	     5,
	     export_path,
	     export_path_length,
	     &alias_path,
	     &alias_path_size,
	     log_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable create alias directory.",
		 function );

		goto on_error;
	}
	if( alias_path == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alias directory path.",
		 function );

		goto on_error;
	}
	if( export_handle->dump_item_values != 0 )
	{
		if( export_handle_export_item_values(
		     export_handle,
		     alias,
		     _LIBCSTRING_SYSTEM_STRING( "ItemValues.txt" ),
		     14,
		     alias_path,
		     alias_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libsystem_notify_verbose != 0 )
			{
				libsystem_notify_printf(
				 "%s: unable to export item values.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libsystem_notify_print_error_backtrace(
				 *error );
			}
			liberror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export alias item values.\n" );
		}
	}
/* TODO Create the alias file */

	memory_free(
	 alias_path );

	return( 1 );

on_error:
	if( alias_path != NULL )
	{
		memory_free(
		 alias_path );
	}
	return( 1 );
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
	fprintf(
	 export_handle->notify_stream,
	 "Exporting items.\n" );

	if( export_handle_make_directory(
	     export_handle,
	     export_handle->items_export_path,
	     log_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to make directory: %" PRIs_LIBCSTRING_SYSTEM "",
		 function,
		 export_handle->items_export_path );

		return( -1 );
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
		if( export_handle->print_status_information != 0 )
		{
			fprintf(
			 export_handle->notify_stream,
			 "Exporting alias item %d out of %d.\n",
			 item_index + 1,
			 number_of_items );
		}
		if( export_handle_export_alias(
		     export_handle,
		     item,
		     item_index,
		     export_handle->items_export_path,
		     export_handle->items_export_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			fprintf(
			 export_handle->notify_stream,
			 "Unable to export item %d out of %d.\n",
			 item_index + 1,
			 number_of_items );

			if( libsystem_notify_verbose != 0 )
			{
				libsystem_notify_printf(
				 "%s: unable to export item: %d.\n",
				 function,
				 item_index + 1 );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libsystem_notify_print_error_backtrace(
				 *error );
			}
			liberror_error_free(
			 error );
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

