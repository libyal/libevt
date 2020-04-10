/*
 * File header functions
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

#include "libevt_file_header.h"
#include "libevt_libbfio.h"
#include "libevt_libcerror.h"
#include "libevt_libcnotify.h"

#include "evt_file_header.h"

/* Creates a file header
 * Make sure the value file_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevt_file_header_initialize(
     libevt_file_header_t **file_header,
     libcerror_error_t **error )
{
	static char *function = "libevt_file_header_initialize";

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header.",
		 function );

		return( -1 );
	}
	if( *file_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file header value already set.",
		 function );

		return( -1 );
	}
	*file_header = memory_allocate_structure(
	                libevt_file_header_t );

	if( *file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_header,
	     0,
	     sizeof( libevt_file_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file_header != NULL )
	{
		memory_free(
		 *file_header );

		*file_header = NULL;
	}
	return( -1 );
}

/* Frees a file header
 * Returns 1 if successful or -1 on error
 */
int libevt_file_header_free(
     libevt_file_header_t **file_header,
     libcerror_error_t **error )
{
	static char *function = "libevt_file_header_free";

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header.",
		 function );

		return( -1 );
	}
	if( *file_header != NULL )
	{
		memory_free(
		 *file_header );

		*file_header = NULL;
	}
	return( 1 );
}

/* Reads the file header data
 * Returns 1 if successful or -1 on error
 */
int libevt_file_header_read_data(
     libevt_file_header_t *file_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_file_header_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
#endif

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header.",
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
	if( data_size < sizeof( evt_file_header_t ) )
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
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( evt_file_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     ( (evt_file_header_t *) data )->signature,
	     "LfLe",
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_file_header_t *) data )->size,
	 file_header->size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_file_header_t *) data )->major_format_version,
	 file_header->major_format_version );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_file_header_t *) data )->minor_format_version,
	 file_header->minor_format_version );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_file_header_t *) data )->first_record_offset,
	 file_header->first_record_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_file_header_t *) data )->end_of_file_record_offset,
	 file_header->end_of_file_record_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_file_header_t *) data )->file_flags,
	 file_header->file_flags );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_file_header_t *) data )->copy_of_size,
	 file_header->copy_of_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: (header) size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 file_header->size );

		libcnotify_printf(
		 "%s: signature\t\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (evt_file_header_t *) data )->signature[ 0 ],
		 ( (evt_file_header_t *) data )->signature[ 1 ],
		 ( (evt_file_header_t *) data )->signature[ 2 ],
		 ( (evt_file_header_t *) data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: major format version\t\t\t: %" PRIu32 "\n",
		 function,
		 file_header->major_format_version );

		libcnotify_printf(
		 "%s: minor format version\t\t\t: %" PRIu32 "\n",
		 function,
		 file_header->minor_format_version );

		libcnotify_printf(
		 "%s: first (oldest) record offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 file_header->first_record_offset );

		libcnotify_printf(
		 "%s: end of file record offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 file_header->end_of_file_record_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_file_header_t *) data )->last_record_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: last (newest) record number\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_file_header_t *) data )->first_record_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: first (oldest) record number\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_file_header_t *) data )->maximum_file_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: maximum file size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: file flags\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 file_header->file_flags );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_file_header_t *) data )->retention,
		 value_32bit );
		libcnotify_printf(
		 "%s: retention\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: copy of (header) size\t\t\t: %" PRIu32 "\n",
		 function,
		 file_header->copy_of_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the file header from a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libevt_file_header_read_file_io_handle(
     libevt_file_header_t *file_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t data[ sizeof( evt_file_header_t ) ];

	static char *function = "libevt_file_header_read_file_io_handle";
	ssize_t read_count    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading file header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              data,
	              sizeof( evt_file_header_t ),
	              error );

	if( read_count != (ssize_t) sizeof( evt_file_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libevt_file_header_read_data(
	     file_header,
	     data,
	     sizeof( evt_file_header_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	return( 1 );
}

