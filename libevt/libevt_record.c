/*
 * Record functions
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
#include <memory.h>
#include <types.h>

#include "libevt_definitions.h"
#include "libevt_io_handle.h"
#include "libevt_libbfio.h"
#include "libevt_libcerror.h"
#include "libevt_libcthreads.h"
#include "libevt_record.h"
#include "libevt_record_values.h"

/* Creates record
 * Make sure the value record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevt_record_initialize(
     libevt_record_t **record,
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libevt_record_values_t *record_values,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_initialize";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( *record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record value already set.",
		 function );

		return( -1 );
	}
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
	internal_record = memory_allocate_structure(
	                   libevt_internal_record_t );

	if( internal_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create internal record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_record,
	     0,
	     sizeof( libevt_internal_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear internal record.",
		 function );

		memory_free(
		 internal_record );

		return( -1 );
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_record->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_record->file_io_handle = file_io_handle;
	internal_record->io_handle      = io_handle;
	internal_record->record_values  = record_values;

	*record = (libevt_record_t *) internal_record;

	return( 1 );

on_error:
	if( internal_record != NULL )
	{
		memory_free(
		 internal_record );
	}
	return( -1 );
}

/* Frees a record
 * Returns 1 if successful or -1 on error
 */
int libevt_record_free(
     libevt_record_t **record,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_free";
	int result                                = 1;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( *record != NULL )
	{
		internal_record = (libevt_internal_record_t *) *record;
		*record         = NULL;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_record->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		/* The file_io_handle, io_handle and record_values references are freed elsewhere
		 */
		memory_free(
		 internal_record );
	}
	return( result );
}

/* Retrieves the offset
 * Returns 1 if successful or -1 on error
 */
int libevt_record_get_offset(
     libevt_record_t *record,
     off64_t *offset,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_offset";
	int result                                = 1;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libevt_record_values_get_offset(
	     internal_record->record_values,
	     offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve offset.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the identifier (record number)
 * Returns 1 if successful or -1 on error
 */
int libevt_record_get_identifier(
     libevt_record_t *record,
     uint32_t *identifier,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_identifier";
	int result                                = 1;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libevt_record_values_get_number(
	     internal_record->record_values,
	     identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the creation time
 * The timestamp is a 32-bit POSIX date and time value
 * Returns 1 if successful or -1 on error
 */
int libevt_record_get_creation_time(
     libevt_record_t *record,
     uint32_t *posix_time,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_creation_time";
	int result                                = 1;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libevt_record_values_get_creation_time(
	     internal_record->record_values,
	     posix_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to creation time.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the written time
 * The timestamp is a 32-bit POSIX date and time value
 * Returns 1 if successful or -1 on error
 */
int libevt_record_get_written_time(
     libevt_record_t *record,
     uint32_t *posix_time,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_written_time";
	int result                                = 1;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libevt_record_values_get_written_time(
	     internal_record->record_values,
	     posix_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to written time.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the event identifier
 * Returns 1 if successful or -1 on error
 */
int libevt_record_get_event_identifier(
     libevt_record_t *record,
     uint32_t *event_identifier,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_event_identifier";
	int result                                = 1;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libevt_record_values_get_event_identifier(
	     internal_record->record_values,
	     event_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event identifier.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the event type
 * Returns 1 if successful or -1 on error
 */
int libevt_record_get_event_type(
     libevt_record_t *record,
     uint16_t *event_type,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_event_type";
	int result                                = 1;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libevt_record_values_get_event_type(
	     internal_record->record_values,
	     event_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event type.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the event category
 * Returns 1 if successful or -1 on error
 */
int libevt_record_get_event_category(
     libevt_record_t *record,
     uint16_t *event_category,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_event_category";
	int result                                = 1;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libevt_record_values_get_event_category(
	     internal_record->record_values,
	     event_category,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event category.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_get_utf8_source_name_size(
     libevt_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf8_source_name_size";
	int result                                = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libevt_record_values_get_utf8_source_name_size(
	          internal_record->record_values,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 source name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_get_utf8_source_name(
     libevt_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf8_source_name";
	int result                                = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libevt_record_values_get_utf8_source_name(
	          internal_record->record_values,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 source name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_get_utf16_source_name_size(
     libevt_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf16_source_name_size";
	int result                                = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libevt_record_values_get_utf16_source_name_size(
	          internal_record->record_values,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 source name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_get_utf16_source_name(
     libevt_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf16_source_name";
	int result                                = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libevt_record_values_get_utf16_source_name(
	          internal_record->record_values,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 source name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_get_utf8_computer_name_size(
     libevt_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf8_computer_name_size";
	int result                                = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libevt_record_values_get_utf8_computer_name_size(
	          internal_record->record_values,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 computer name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_get_utf8_computer_name(
     libevt_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf8_computer_name";
	int result                                = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libevt_record_values_get_utf8_computer_name(
	          internal_record->record_values,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 computer name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_get_utf16_computer_name_size(
     libevt_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf16_computer_name_size";
	int result                                = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libevt_record_values_get_utf16_computer_name_size(
	          internal_record->record_values,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 computer name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_get_utf16_computer_name(
     libevt_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf16_computer_name";
	int result                                = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libevt_record_values_get_utf16_computer_name(
	          internal_record->record_values,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 computer name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded user security identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_get_utf8_user_security_identifier_size(
     libevt_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf8_user_security_identifier_size";
	int result                                = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libevt_record_values_get_utf8_user_security_identifier_size(
	          internal_record->record_values,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the of the user security identifier as UTF-8 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded user security identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_get_utf8_user_security_identifier(
     libevt_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf8_user_security_identifier";
	int result                                = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libevt_record_values_get_utf8_user_security_identifier(
	          internal_record->record_values,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy user security identifier to UTF-8 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded user security identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_get_utf16_user_security_identifier_size(
     libevt_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf16_user_security_identifier_size";
	int result                                = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libevt_record_values_get_utf16_user_security_identifier_size(
	          internal_record->record_values,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the of the user security identifier as UTF-16 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded user security identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_get_utf16_user_security_identifier(
     libevt_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf16_user_security_identifier";
	int result                                = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libevt_record_values_get_utf16_user_security_identifier(
	          internal_record->record_values,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy user security identifier to UTF-16 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of strings
 * Returns 1 if successful or -1 on error
 */
int libevt_record_get_number_of_strings(
     libevt_record_t *record,
     int *number_of_strings,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_number_of_strings";
	int result                                = 1;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libevt_record_values_get_number_of_strings(
	     internal_record->record_values,
	     number_of_strings,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of strings.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of a specific UTF-8 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_record_get_utf8_string_size(
     libevt_record_t *record,
     int string_index,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf8_string_size";
	int result                                = 1;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libevt_record_values_get_utf8_string_size(
	     internal_record->record_values,
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

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves a specific UTF-8 encoded string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_record_get_utf8_string(
     libevt_record_t *record,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf8_string";
	int result                                = 1;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libevt_record_values_get_utf8_string(
	     internal_record->record_values,
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

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of a specific UTF-16 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_record_get_utf16_string_size(
     libevt_record_t *record,
     int string_index,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf16_string_size";
	int result                                = 1;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libevt_record_values_get_utf16_string_size(
	     internal_record->record_values,
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

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves a specific UTF-16 encoded string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevt_record_get_utf16_string(
     libevt_record_t *record,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_utf16_string";
	int result                                = 1;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libevt_record_values_get_utf16_string(
	     internal_record->record_values,
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

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_get_data_size(
     libevt_record_t *record,
     size_t *data_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_data_size";
	int result                                = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libevt_record_values_get_data_size(
	          internal_record->record_values,
	          data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevt_record_get_data(
     libevt_record_t *record,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libevt_internal_record_t *internal_record = NULL;
	static char *function                     = "libevt_record_get_data";
	int result                                = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevt_internal_record_t *) record;

#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libevt_record_values_get_data(
	          internal_record->record_values,
	          data,
	          data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy data.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEVT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_record->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

