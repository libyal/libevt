/* 
 * Message file
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
#include <memory.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#include "evttools_libexe.h"
#include "evttools_libwrc.h"
#include "message_file.h"

/* Initializes the message file
 * Returns 1 if successful or -1 on error
 */
int message_file_initialize(
     message_file_t **message_file,
     liberror_error_t **error )
{
	static char *function = "message_file_initialize";

	if( message_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message file.",
		 function );

		return( -1 );
	}
	if( *message_file != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid message file value already set.",
		 function );

		return( -1 );
	}
	*message_file = memory_allocate_structure(
	                message_file_t );

	if( *message_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create message file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *message_file,
	     0,
	     sizeof( message_file_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear message file.",
		 function );

		memory_free(
		 *message_file );

		*message_file = NULL;

		return( -1 );
	}
	if( libexe_file_initialize(
	     &( ( *message_file )->exe_file ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize EXE file.",
		 function );

		goto on_error;
	}
	if( libwrc_stream_initialize(
	     &( ( *message_file )->resource_stream ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize resource stream.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *message_file != NULL )
	{
		if( ( *message_file )->resource_stream != NULL )
		{
			libwrc_stream_free(
			 &( ( *message_file )->resource_stream ),
			 NULL );
		}
		if( ( *message_file )->exe_file != NULL )
		{
			libexe_file_free(
			 &( ( *message_file )->exe_file ),
			 NULL );
		}
		memory_free(
		 *message_file );

		*message_file = NULL;
	}
	return( -1 );
}

/* Frees the message file and its elements
 * Returns 1 if successful or -1 on error
 */
int message_file_free(
     message_file_t **message_file,
     liberror_error_t **error )
{
	static char *function = "message_file_free";
	int result            = 1;

	if( message_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message file.",
		 function );

		return( -1 );
	}
	if( *message_file != NULL )
	{
		if( ( *message_file )->message_table_resource != NULL )
		{
			if( libwrc_resource_free(
			     &( ( *message_file )->message_table_resource ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free message table resource.",
				 function );

				result = -1;
			}
		}
		if( libwrc_stream_free(
		     &( ( *message_file )->resource_stream ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free resource stream.",
			 function );

			result = -1;
		}
		if( ( *message_file )->resource_section != NULL )
		{
			if( libexe_section_free(
			     &( ( *message_file )->resource_section ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free resource section.",
				 function );

				result = -1;
			}
		}
		if( libexe_file_free(
		     &( ( *message_file )->exe_file ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free EXE file.",
			 function );

			result = -1;
		}
		memory_free(
		 *message_file );

		*message_file = NULL;
	}
	return( result );
}

/* Opens the message file
 * Returns 1 if successful, 0 if no message table resource could be found or -1 on error
 */
int message_file_open(
     message_file_t *message_file,
     const libcstring_system_character_t *filename,
     liberror_error_t **error )
{
	static char *function    = "message_file_open";
	uint32_t virtual_address = 0;
	int result               = 0;

	if( message_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message file.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libexe_file_open_wide(
	     message_file->exe_file,
	     filename,
	     LIBEXE_OPEN_READ,
	     error ) != 1 )
#else
	if( libexe_file_open(
	     message_file->exe_file,
	     filename,
	     LIBEXE_OPEN_READ,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open EXE file.",
		 function );

		goto on_error;
	}
	result = libexe_file_get_section_by_name(
	          message_file->exe_file,
	          ".rsrc",
	          5,
	          &( message_file->resource_section ),
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve resource section.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libexe_section_get_virtual_address(
		     message_file->resource_section,
		     &virtual_address,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve resource section virtual adress.",
			 function );

			goto on_error;
		}
		if( libexe_section_get_data_file_io_handle(
		     message_file->resource_section,
		     &( message_file->resource_section_file_io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve resource section file IO handle.",
			 function );

			goto on_error;
		}
		if( libwrc_stream_set_virtual_address(
		     message_file->resource_stream,
		     virtual_address,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set resource stream virtual adress.",
			 function );

			goto on_error;
		}
		if( libwrc_stream_open_file_io_handle(
		     message_file->resource_stream,
		     message_file->resource_section_file_io_handle,
		     LIBWRC_OPEN_READ,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open resource stream.",
			 function );

			goto on_error;
		}
		result = libwrc_stream_get_resource_by_type(
			  message_file->resource_stream,
			  LIBWRC_RESOURCE_TYPE_MESSAGE_TABLE,
			  &( message_file->message_table_resource ),
			  error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message table resource.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( message_file->resource_section_file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &( message_file->resource_section_file_io_handle ),
		 NULL );
	}
	if( message_file->resource_section != NULL )
	{
		libexe_section_free(
		 &( message_file->resource_section ),
		 NULL );
	}
	libexe_file_close(
	 message_file->exe_file,
	 NULL );

	return( -1 );
}

/* Closes the message file
 * Returns the 0 if succesful or -1 on error
 */
int message_file_close(
     message_file_t *message_file,
     liberror_error_t **error )
{
	static char *function = "message_file_close";

	if( message_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message file.",
		 function );

		return( -1 );
	}
	if( libwrc_stream_close(
	     message_file->resource_stream,
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close resource stream.",
		 function );

		return( -1 );
	}
	if( libexe_file_close(
	     message_file->exe_file,
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close EXE file.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Retrieves a specific message string
 * Returns 1 if successful, 0 if no such message string or -1 error
 */
int message_file_get_string(
     message_file_t *message_file,
     uint32_t message_identifier,
     libcstring_system_character_t **message_string,
     size_t *message_string_size,
     liberror_error_t **error )
{
	static char *function = "message_file_get_string";
	int message_index     = 0;
	int result            = 0;

	if( message_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message file.",
		 function );

		return( -1 );
	}
	if( message_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message string.",
		 function );

		return( -1 );
	}
	if( *message_string != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid message string value already set.",
		 function );

		return( -1 );
	}
	if( message_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message string size.",
		 function );

		return( -1 );
	}
/* TODO cache message strings */
	result = libwrc_message_table_get_index_by_identifier(
	          message_file->message_table_resource,
	          message_identifier,
	          &message_index,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve message index for message identifier: 0x%08" PRIx32 ".",
		 function,
		 message_identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libwrc_message_table_get_utf16_message_size(
		          message_file->message_table_resource,
			  message_index,
			  message_string_size,
			  error );
#else
		result = libwrc_message_table_get_utf8_message_size(
		          message_file->message_table_resource,
			  message_index,
			  message_string_size,
			  error );
#endif
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message: %d size.",
			 function,
			 message_index );

			goto on_error;
		}
		*message_string = libcstring_system_string_allocate(
				   *message_string_size );

		if( message_string == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create message string.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libwrc_message_table_get_utf16_message(
		          message_file->message_table_resource,
			  message_index,
			  (uint16_t *) *message_string,
			  *message_string_size,
			  error );
#else
		result = libwrc_message_table_get_utf8_message(
		          message_file->message_table_resource,
			  message_index,
			  (uint8_t *) *message_string,
			  *message_string_size,
			  error );
#endif
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message string.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( *message_string != NULL )
	{
		memory_free(
		 *message_string );

		*message_string = NULL;
	}
	*message_string_size = 0;

	return( -1 );
}

