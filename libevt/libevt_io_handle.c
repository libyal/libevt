/*
 * Input/Output (IO) handle functions
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
#include "libevt_codepage.h"
#include "libevt_definitions.h"
#include "libevt_io_handle.h"
#include "libevt_libbfio.h"
#include "libevt_libcdata.h"
#include "libevt_libcerror.h"
#include "libevt_libcnotify.h"
#include "libevt_record_values.h"
#include "libevt_unused.h"

#include "evt_file_header.h"

const uint8_t evt_end_of_file_record_signature1[ 4 ] = { 0x11, 0x11, 0x11, 0x11 };
const uint8_t evt_end_of_file_record_signature2[ 4 ] = { 0x22, 0x22, 0x22, 0x22 };
const uint8_t evt_end_of_file_record_signature3[ 4 ] = { 0x33, 0x33, 0x33, 0x33 };
const uint8_t evt_end_of_file_record_signature4[ 4 ] = { 0x44, 0x44, 0x44, 0x44 };
const uint8_t evt_file_signature[ 4 ]                = { 'L', 'f', 'L', 'e' };

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevt_io_handle_initialize(
     libevt_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libevt_io_handle_initialize";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libevt_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libevt_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		goto on_error;
	}
	( *io_handle )->ascii_codepage = LIBEVT_CODEPAGE_WINDOWS_1252;

	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libevt_io_handle_free(
     libevt_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libevt_io_handle_free";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( 1 );
}

/* Clears the IO handle
 * Returns 1 if successful or -1 on error
 */
int libevt_io_handle_clear(
     libevt_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libevt_io_handle_clear";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     io_handle,
	     0,
	     sizeof( libevt_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		return( -1 );
	}
	io_handle->ascii_codepage = LIBEVT_CODEPAGE_WINDOWS_1252;

	return( 1 );
}

/* Reads the records into the records array
 * Returns 1 if successful or -1 on error
 */
int libevt_io_handle_read_records(
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t first_record_offset,
     uint32_t end_of_file_record_offset,
     libfdata_list_t *records_list,
     off64_t *last_record_offset,
     libcerror_error_t **error )
{
	libevt_record_values_t *record_values = NULL;
	static char *function                 = "libevt_io_handle_read_records";
	ssize_t read_count                    = 0;
	off64_t file_offset                   = 0;
	off64_t safe_last_record_offset       = 0;
	uint32_t record_iterator              = 0;
	uint8_t record_type                   = 0;
	int element_index                     = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( ( (off64_t) first_record_offset < (off64_t) sizeof( evt_file_header_t ) )
	 || ( (size64_t) first_record_offset >= io_handle->file_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid first record offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( last_record_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid last record offset.",
		 function );

		return( -1 );
	}
	file_offset = (off64_t) first_record_offset;

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
		 "%s: unable to seek record offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	do
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading record: %" PRIu32 " at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
			 function,
			 record_iterator,
			 file_offset,
			 file_offset );
		}
#endif
		if( libevt_record_values_initialize(
		     &record_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create record values.",
			 function );

			goto on_error;
		}
		safe_last_record_offset = file_offset;

		read_count = libevt_record_values_read_file_io_handle(
		              record_values,
		              file_io_handle,
		              io_handle,
		              &file_offset,
		              &( io_handle->has_wrapped ),
		              1,
		              error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read record: %" PRIu32 ".",
			 function,
			 record_iterator );

			goto on_error;
		}
		record_type = record_values->type;

		if( record_type == LIBEVT_RECORD_TYPE_EVENT )
		{
			if( libfdata_list_append_element(
			     records_list,
			     &element_index,
			     0,
			     safe_last_record_offset,
			     (size64_t) read_count,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append record to records list.",
				 function );

				goto on_error;
			}
		}
		if( libevt_record_values_free(
		     &record_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record values.",
			 function );

			goto on_error;
		}
		if( record_type == LIBEVT_RECORD_TYPE_END_OF_FILE )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: end-of-file record found at offset: 0x%08" PRIx64 ".\n",
				 function,
				 file_offset );
			}
#endif
			break;
		}
		if( ( file_offset > (off64_t) end_of_file_record_offset )
		 && ( file_offset < (off64_t) ( end_of_file_record_offset + read_count ) ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: file offset: 0x%08" PRIx64 " exceeds end-of-file offset: 0x%08" PRIx64 ".\n",
				 function,
				 file_offset,
				 end_of_file_record_offset );
			}
#endif
			break;
		}
		record_iterator++;
	}
	while( record_type != LIBEVT_RECORD_TYPE_END_OF_FILE );

	if( record_type == LIBEVT_RECORD_TYPE_END_OF_FILE )
	{
		if( safe_last_record_offset != (off64_t) end_of_file_record_offset )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: mismatch in end of file record offset ( %" PRIi64 " != %" PRIu32 " ).\n",
				 function,
				 safe_last_record_offset,
				 end_of_file_record_offset );
			}
#endif
			io_handle->flags |= LIBEVT_IO_HANDLE_FLAG_IS_CORRUPTED;
		}
		safe_last_record_offset += read_count;
	}
	if( ( io_handle->has_wrapped != 0 )
	 && ( ( io_handle->flags & LIBEVT_FILE_FLAG_HAS_WRAPPED ) == 0 ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: file has wrapped but file flags indicate otherwise.\n",
			 function );
		}
#endif
		io_handle->flags |= LIBEVT_IO_HANDLE_FLAG_IS_CORRUPTED;
	}
	*last_record_offset = safe_last_record_offset;

	return( 1 );

on_error:
	if( record_values != NULL )
	{
		libevt_record_values_free(
		 &record_values,
		 NULL );
	}
	*last_record_offset = safe_last_record_offset;

	return( -1 );
}

/* Scans for the end-of-file record and adjusts the offsets accordingly
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libevt_io_handle_end_of_file_record_scan(
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t *first_record_offset,
     uint32_t *end_of_file_record_offset,
     libcerror_error_t **error )
{
	uint8_t *scan_block         = NULL;
	static char *function       = "libevt_io_handle_end_of_file_record_scan";
	off64_t file_offset         = 0;
	off64_t initial_file_offset = 0;
	size_t read_size            = 0;
	size_t scan_block_offset    = 0;
	size_t scan_block_size      = 8192;
	ssize_t read_count          = 0;
	uint8_t scan_state          = LIBEVT_RECOVER_SCAN_STATE_START;
	uint8_t scan_has_wrapped    = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( first_record_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first record offset.",
		 function );

		return( -1 );
	}
	if( end_of_file_record_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid end of file record offset.",
		 function );

		return( -1 );
	}
	scan_block = (uint8_t *) memory_allocate(
				  sizeof( uint8_t ) * scan_block_size );

	if( scan_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create scan block.",
		 function );

		goto on_error;
	}
	/* If the file has wrapped start looking for the end-of-file record after the end-of-file record offset
	 */
	file_offset = (off64_t) *end_of_file_record_offset;

	if( ( file_offset < (off64_t) sizeof( evt_file_header_t ) )
	 || ( (size64_t) file_offset >= io_handle->file_size ) )
	{
		file_offset = (off64_t) sizeof( evt_file_header_t );
	}
	initial_file_offset = file_offset;

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
		 "%s: unable to seek scan block offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	do
	{
		if( ( (size64_t) file_offset + scan_block_size ) > io_handle->file_size )
		{
			read_size = (size_t) ( io_handle->file_size - file_offset );
		}
		else
		{
			read_size = scan_block_size;
		}
		read_count = libbfio_handle_read_buffer(
			      file_io_handle,
			      scan_block,
			      read_size,
			      error );

		if( read_count != (ssize_t) read_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read scan block at offset: %" PRIi64 ".",
			 function,
			 file_offset );

			goto on_error;
		}
		file_offset += read_count;

		if( read_size >= 4 )
		{
			for( scan_block_offset = 0;
			     scan_block_offset < ( read_size - 4 );
			     scan_block_offset += 4 )
			{
				if( scan_state == LIBEVT_RECOVER_SCAN_STATE_START )
				{
					if( memory_compare(
					     &( scan_block[ scan_block_offset ] ),
					     evt_end_of_file_record_signature1,
					     4 ) == 0 )
					{
						scan_state = LIBEVT_RECOVER_SCAN_STATE_FOUND_EOF_SIGNATURE1;
					}
				}
				else if( scan_state == LIBEVT_RECOVER_SCAN_STATE_FOUND_EOF_SIGNATURE1 )
				{
					if( memory_compare(
					     &( scan_block[ scan_block_offset ] ),
					     evt_end_of_file_record_signature2,
					     4 ) == 0 )
					{
						scan_state = LIBEVT_RECOVER_SCAN_STATE_FOUND_EOF_SIGNATURE2;
					}
					else
					{
						scan_state = LIBEVT_RECOVER_SCAN_STATE_START;
					}
				}
				else if( scan_state == LIBEVT_RECOVER_SCAN_STATE_FOUND_EOF_SIGNATURE2 )
				{
					if( memory_compare(
					     &( scan_block[ scan_block_offset ] ),
					     evt_end_of_file_record_signature3,
					     4 ) == 0 )
					{
						scan_state = LIBEVT_RECOVER_SCAN_STATE_FOUND_EOF_SIGNATURE3;
					}
					else
					{
						scan_state = LIBEVT_RECOVER_SCAN_STATE_START;
					}
				}
				else if( scan_state == LIBEVT_RECOVER_SCAN_STATE_FOUND_EOF_SIGNATURE3 )
				{
					if( memory_compare(
					     &( scan_block[ scan_block_offset ] ),
					     evt_end_of_file_record_signature4,
					     4 ) == 0 )
					{
						*end_of_file_record_offset = (uint32_t) ( file_offset - read_count + scan_block_offset - 16 );

						scan_state = LIBEVT_RECOVER_SCAN_STATE_FOUND_EOF_SIGNATURE4;
					}
					else
					{
						scan_state = LIBEVT_RECOVER_SCAN_STATE_START;
					}
				}
				else if( scan_state == LIBEVT_RECOVER_SCAN_STATE_FOUND_EOF_SIGNATURE4 )
				{
					if( memory_compare(
					     &( scan_block[ scan_block_offset ] ),
					     evt_file_signature,
					     4 ) == 0 )
					{
						*first_record_offset = (uint32_t) ( file_offset - read_count + scan_block_offset - 4 );

						scan_state = LIBEVT_RECOVER_SCAN_STATE_FOUND_RECORD_SIGNATURE;

						break;
					}
				}
			}
		}
		if( scan_state == LIBEVT_RECOVER_SCAN_STATE_FOUND_RECORD_SIGNATURE )
		{
			break;
		}
		if( ( scan_has_wrapped == 0 )
		 && ( (size64_t) file_offset >= io_handle->file_size ) )
		{
			if( libbfio_handle_seek_offset(
			     file_io_handle,
			     (off64_t) sizeof( evt_file_header_t ),
			     SEEK_SET,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_SEEK_FAILED,
				 "%s: unable to seek scan block offset: %" PRIzd ".",
				 function,
				 sizeof( evt_file_header_t ) );

				goto on_error;
			}
			file_offset = (off64_t) sizeof( evt_file_header_t );

			scan_has_wrapped = 1;
		}
	}
	while( ( scan_has_wrapped == 0 )
	    || ( file_offset < initial_file_offset ) );

	memory_free(
	 scan_block );

	scan_block = NULL;

	if( scan_state == LIBEVT_RECOVER_SCAN_STATE_FOUND_RECORD_SIGNATURE )
	{
		return( 1 );
	}
	return( 0 );

on_error:
	if( scan_block != NULL )
	{
		memory_free(
		 scan_block );
	}
	return( -1 );
}

/* Scans for the event record and adds them to the recovered records array
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libevt_io_handle_event_record_scan(
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size64_t size,
     libfdata_list_t *recovered_records_list,
     libcerror_error_t **error )
{
	libevt_record_values_t *record_values = NULL;
	uint8_t *scan_block                   = NULL;
	static char *function                 = "libevt_io_handle_event_record_scan";
	off64_t record_offset                 = 0;
	size_t read_size                      = 0;
	size_t scan_block_offset              = 0;
	size_t scan_block_size                = 8192;
	ssize_t read_count                    = 0;
	int element_index                     = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( file_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid file offset value less than zero.",
		 function );

		return( -1 );
	}
	if( size > (off64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	scan_block = (uint8_t *) memory_allocate(
				  sizeof( uint8_t ) * scan_block_size );

	if( scan_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create scan block.",
		 function );

		goto on_error;
	}
	while( size >= 4 )
	{
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
			 "%s: unable to seek scan block offset: %" PRIi64 ".",
			 function,
			 file_offset );

			goto on_error;
		}
		if( scan_block_size > size )
		{
			read_size = (size_t) size;
		}
		else
		{
			read_size = scan_block_size;
		}
		read_count = libbfio_handle_read_buffer(
			      file_io_handle,
			      scan_block,
			      read_size,
			      error );

		if( read_count != (ssize_t) read_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read scan block at offset: %" PRIi64 ".",
			 function,
			 file_offset );

			goto on_error;
		}
		scan_block_offset = 0;

		while( scan_block_offset <= ( read_size - 4 ) )
		{
			if( memory_compare(
			     &( scan_block[ scan_block_offset ] ),
			     evt_file_signature,
			     4 ) != 0 )
			{
				scan_block_offset += 4;

				continue;
			}
			record_offset = file_offset + scan_block_offset - 4;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: reading recovered record at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
				 function,
				 record_offset,
				 record_offset );
			}
#endif
			if( libbfio_handle_seek_offset(
			     file_io_handle,
			     record_offset,
			     SEEK_SET,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_SEEK_FAILED,
				 "%s: unable to seek record offset: %" PRIi64 ".",
				 function,
				 record_offset );

				goto on_error;
			}
			if( record_values == NULL )
			{
				if( libevt_record_values_initialize(
				     &record_values,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create record values.",
					 function );

					goto on_error;
				}
			}
			read_count = libevt_record_values_read_file_io_handle(
				      record_values,
				      file_io_handle,
				      io_handle,
				      &record_offset,
			              &( io_handle->has_wrapped ),
				      0,
				      error );

			if( read_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read record at offset: %" PRIi64 ".",
				 function,
				 record_offset );

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcnotify_print_error_backtrace(
						 *error );
					}
				}
#endif
				libcerror_error_free(
				 error );

				scan_block_offset += 4;

				continue;
			}
			if( record_values->type == LIBEVT_RECORD_TYPE_EVENT )
			{
				if( libfdata_list_append_element(
				     recovered_records_list,
				     &element_index,
				     0,
				     file_offset + scan_block_offset - 4,
				     (size64_t) read_count,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append recovered record to records list.",
					 function );

					goto on_error;
				}
			}
			if( libevt_record_values_free(
			     &record_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free record values.",
				 function );

				goto on_error;
			}
			scan_block_offset += read_count - 4;
		}
		file_offset += scan_block_offset;
		size        -= scan_block_offset;
	}
	if( record_values != NULL )
	{
		if( libevt_record_values_free(
		     &record_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record values.",
			 function );

			goto on_error;
		}
	}
	memory_free(
	 scan_block );

	scan_block = NULL;

	return( 1 );

on_error:
	if( record_values != NULL )
	{
		libevt_record_values_free(
		 &record_values,
		 NULL );
	}
	if( scan_block != NULL )
	{
		memory_free(
		 scan_block );
	}
	return( -1 );
}

/* Tries to recover records
 * Returns 1 if successful or -1 on error
 */
int libevt_io_handle_recover_records(
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t first_record_offset,
     uint32_t end_of_file_record_offset,
     off64_t last_record_offset,
     libfdata_list_t *records_list,
     libfdata_list_t *recovered_records_list,
     libcerror_error_t **error )
{
	static char *function  = "libevt_io_handle_recover_records";
	int result             = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( last_record_offset == (off64_t) first_record_offset )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: no records found at specified offsets scanning for end-of-file record.\n",
			 function );
		}
#endif
		io_handle->flags |= LIBEVT_IO_HANDLE_FLAG_IS_CORRUPTED;

		result = libevt_io_handle_end_of_file_record_scan(
		          io_handle,
		          file_io_handle,
		          &first_record_offset,
		          &end_of_file_record_offset,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to scan for end of file record.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: end-of-file record found at offset: 0x%08" PRIx64 ".\n",
				 function,
				 end_of_file_record_offset );
			}
#endif
			result = libevt_io_handle_read_records(
			          io_handle,
			          file_io_handle,
			          first_record_offset,
			          end_of_file_record_offset,
			          records_list,
			          &last_record_offset,
			          error );

			if( result != 1 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcerror_error_free(
						 error );
					}
				}
#endif
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read records.",
				 function );

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcnotify_print_error_backtrace(
						 *error );
					}
				}
#endif
				libcerror_error_free(
				 error );
			}
		}
		else
		{
			first_record_offset = (uint32_t) sizeof( evt_file_header_t );
			last_record_offset  = (off64_t) sizeof( evt_file_header_t );
		}
	}
	if( io_handle->has_wrapped == 0 )
	{
		if( first_record_offset > (uint32_t) sizeof( evt_file_header_t ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: scanning unused space before records at offset: 0x%08" PRIzd " - 0x%08" PRIx32 "\n",
				 function,
				 sizeof( evt_file_header_t ),
				 first_record_offset );
			}
#endif
			if( libevt_io_handle_event_record_scan(
			     io_handle,
			     file_io_handle,
			     (off64_t) sizeof( evt_file_header_t ),
			     (size64_t) ( first_record_offset - sizeof( evt_file_header_t ) ),
			     recovered_records_list,
			     error ) != 1 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcerror_error_free(
						 error );
					}
				}
#endif
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to scan for event records.",
				 function );

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcnotify_print_error_backtrace(
						 *error );
					}
				}
#endif
				libcerror_error_free(
				 error );
			}
		}
		if( last_record_offset < (off64_t) io_handle->file_size )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( last_record_offset > (off64_t) sizeof( evt_file_header_t ) )
				{
					libcnotify_printf(
					 "%s: scanning unused space after records at offset: 0x%08" PRIx64 " - 0x%08" PRIx64 "\n",
					 function,
					 last_record_offset,
					 io_handle->file_size );
				}
				else
				{
					libcnotify_printf(
					 "%s: scanning unused space after header at offset: 0x%08" PRIx64 " - 0x%08" PRIx64 "\n",
					 function,
					 last_record_offset,
					 io_handle->file_size );
				}
			}
#endif
			if( libevt_io_handle_event_record_scan(
			     io_handle,
			     file_io_handle,
			     last_record_offset,
			     io_handle->file_size - last_record_offset,
			     recovered_records_list,
			     error ) != 1 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcerror_error_free(
						 error );
					}
				}
#endif
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to scan for event records.",
				 function );

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcnotify_print_error_backtrace(
						 *error );
					}
				}
#endif
				libcerror_error_free(
				 error );
			}
		}
	}
	else
	{
		if( last_record_offset < (off64_t) first_record_offset )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: scanning unused space between records at offset: 0x%08" PRIx64 " - 0x%08" PRIx32 "\n",
				 function,
				 last_record_offset,
				 first_record_offset );
			}
#endif
			if( libevt_io_handle_event_record_scan(
			     io_handle,
			     file_io_handle,
			     last_record_offset,
			     (size64_t) first_record_offset - last_record_offset,
			     recovered_records_list,
			     error ) != 1 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcerror_error_free(
						 error );
					}
				}
#endif
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to scan for event records.",
				 function );

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcnotify_print_error_backtrace(
						 *error );
					}
				}
#endif
				libcerror_error_free(
				 error );
			}
		}
	}
	return( 1 );
}

