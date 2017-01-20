/*
 * Debug functions
 *
 * Copyright (C) 2011-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "libevt_debug.h"
#include "libevt_definitions.h"
#include "libevt_libbfio.h"
#include "libevt_libcerror.h"
#include "libevt_libcnotify.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the file flags
 */
void libevt_debug_print_file_flags(
      uint32_t file_flags )
{
	if( ( file_flags & LIBEVT_FILE_FLAG_IS_DIRTY ) != 0 )
	{
		libcnotify_printf(
		 "Is dirty (ELF_LOGFILE_HEADER_DIRTY)\n" );
	}
	if( ( file_flags & LIBEVT_FILE_FLAG_HAS_WRAPPED ) != 0 )
	{
		libcnotify_printf(
		 "Has wrapped (ELF_LOGFILE_HEADER_WRAP)\n" );
	}
	if( ( file_flags & LIBEVT_FILE_FLAG_IS_FULL ) != 0 )
	{
		libcnotify_printf(
		 "Is full (ELF_LOGFILE_LOGFULL_WRITTEN)\n" );
	}
	if( ( file_flags & LIBEVT_FILE_FLAG_ARCHIVE ) != 0 )
	{
		libcnotify_printf(
		 "Should be archived (ELF_LOGFILE_ARCHIVE_SET)\n" );
	}
}

/* Prints the event identifier severity
 */
void libevt_debug_print_event_identifier_severity(
      uint32_t event_identifier_severity )
{
	event_identifier_severity >>= 30;

	switch( event_identifier_severity )
	{
		case 0:
			libcnotify_printf(
			 "Success" );
			break;

		case 1:
			libcnotify_printf(
			 "Informational" );
			break;

		case 2:
			libcnotify_printf(
			 "Warning" );
			break;

		case 3:
			libcnotify_printf(
			 "Error" );
			break;
	}
}

/* Prints the event type
 */
void libevt_debug_print_event_type(
      uint16_t event_type )
{
	switch( event_type )
	{
		case LIBEVT_EVENT_TYPE_ERROR:
			libcnotify_printf(
			 "Error event" );
			break;

		case LIBEVT_EVENT_TYPE_WARNING:
			libcnotify_printf(
			 "Warning event" );
			break;

		case LIBEVT_EVENT_TYPE_INFORMATION:
			libcnotify_printf(
			 "Information event" );
			break;

		case LIBEVT_EVENT_TYPE_AUDIT_SUCCESS:
			libcnotify_printf(
			 "Success Audit event" );
			break;

		case LIBEVT_EVENT_TYPE_AUDIT_FAILURE:
			libcnotify_printf(
			 "Failure Audit event" );
			break;

		default:
			libcnotify_printf(
			 "(Unknown)" );
	}
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libevt_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libevt_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 ( offset_iterator + 1 ) );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIi64 " ( 0x%08" PRIx64 " ) - %08" PRIi64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + size,
		 offset + size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif

