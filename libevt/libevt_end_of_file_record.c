/*
 * End-of-file record functions
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

#include "libevt_end_of_file_record.h"
#include "libevt_libcerror.h"
#include "libevt_libcnotify.h"

#include "evt_end_of_file_record.h"

/* Creates a end-of-file record
 * Make sure the value end_of_file_record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevt_end_of_file_record_initialize(
     libevt_end_of_file_record_t **end_of_file_record,
     libcerror_error_t **error )
{
	static char *function = "libevt_end_of_file_record_initialize";

	if( end_of_file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid end-of-file record.",
		 function );

		return( -1 );
	}
	if( *end_of_file_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid end-of-file record value already set.",
		 function );

		return( -1 );
	}
	*end_of_file_record = memory_allocate_structure(
	                       libevt_end_of_file_record_t );

	if( *end_of_file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create end-of-file record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *end_of_file_record,
	     0,
	     sizeof( libevt_end_of_file_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear end-of-file record.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *end_of_file_record != NULL )
	{
		memory_free(
		 *end_of_file_record );

		*end_of_file_record = NULL;
	}
	return( -1 );
}

/* Frees a end-of-file record
 * Returns 1 if successful or -1 on error
 */
int libevt_end_of_file_record_free(
     libevt_end_of_file_record_t **end_of_file_record,
     libcerror_error_t **error )
{
	static char *function = "libevt_end_of_file_record_free";

	if( end_of_file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid end-of-file record.",
		 function );

		return( -1 );
	}
	if( *end_of_file_record != NULL )
	{
		memory_free(
		 *end_of_file_record );

		*end_of_file_record = NULL;
	}
	return( 1 );
}

/* Reads the end-of-file record data
 * Returns 1 if successful or -1 on error
 */
int libevt_end_of_file_record_read_data(
     libevt_end_of_file_record_t *end_of_file_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_end_of_file_record_read_data";
	uint32_t copy_of_size = 0;
	uint32_t size         = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
#endif

	if( end_of_file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid end-of-file record.",
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
	if( data_size < sizeof( evt_end_of_file_record_t ) )
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
		 "%s: end-of-file record data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( evt_end_of_file_record_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     ( (evt_end_of_file_record_t *) data )->signature1,
	     "\x11\x11\x11\x11",
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature1.",
		 function );

		return( -1 );
	}
	if( memory_compare(
	     ( (evt_end_of_file_record_t *) data )->signature2,
	     "\x22\x22\x22\x22",
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature2.",
		 function );

		return( -1 );
	}
	if( memory_compare(
	     ( (evt_end_of_file_record_t *) data )->signature3,
	     "\x33\x33\x33\x33",
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature3.",
		 function );

		return( -1 );
	}
	if( memory_compare(
	     ( (evt_end_of_file_record_t *) data )->signature4,
	     "\x44\x44\x44\x44",
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature4.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_end_of_file_record_t *) data )->size,
	 size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_end_of_file_record_t *) data )->copy_of_size,
	 copy_of_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: (record) size\t\t\t: %" PRIu32 "\n",
		 function,
		 size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_end_of_file_record_t *) data )->signature1,
		 value_32bit );
		libcnotify_printf(
		 "%s: signature1\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_end_of_file_record_t *) data )->signature2,
		 value_32bit );
		libcnotify_printf(
		 "%s: signature2\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_end_of_file_record_t *) data )->signature3,
		 value_32bit );
		libcnotify_printf(
		 "%s: signature3\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_end_of_file_record_t *) data )->signature4,
		 value_32bit );
		libcnotify_printf(
		 "%s: signature4\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_end_of_file_record_t *) data )->first_record_offset,
		 value_32bit );
		libcnotify_printf(
		 "%s: first (oldest) record offset\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_end_of_file_record_t *) data )->end_of_file_record_offset,
		 value_32bit );
		libcnotify_printf(
		 "%s: end of file record offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_end_of_file_record_t *) data )->last_record_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: last (newest) record number\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_end_of_file_record_t *) data )->first_record_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: first (oldest) record number\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: copy of (record) size\t\t: %" PRIu32 "\n",
		 function,
		 copy_of_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( size != copy_of_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for size and copy of size ( %" PRIu32 " != %" PRIu32 " ).\n",
		 function,
		 size,
		 copy_of_size );

		return( -1 );
	}
	if( (size_t) size != sizeof( evt_end_of_file_record_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: record size: %" PRIu32 " does not match known value.\n",
		 function,
		 size );

		return( -1 );
	}
	return( 1 );
}

