/*
 * OSS-Fuzz target for libevt file type
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

#include <stddef.h>
#include <stdint.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libevt.h"

#if !defined( LIBEVT_HAVE_BFIO )

/* Determines if a file contains an EVT file signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEVT_EXTERN \
int libevt_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libevt_error_t **error );

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_open_file_io_handle(
     libevt_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libevt_error_t **error );

#endif /* !defined( LIBEVT_HAVE_BFIO ) */

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	libbfio_handle_t *file_io_handle = NULL;
	libevt_file_t *file              = NULL;
	libevt_record_t *record          = NULL;
	uint32_t flags                   = 0;
	uint32_t major_format_version    = 0;
	uint32_t minor_format_version    = 0;
	int ascii_codepage               = 0;
	int number_of_records            = 0;
	int number_of_recovered_records  = 0;
	int result                       = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	libevt_set_codepage(
	 LIBEVT_CODEPAGE_WINDOWS_1252,
	 NULL );

	if( libevt_file_initialize(
	     &file,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	libevt_get_codepage(
	 &ascii_codepage,
	 NULL );

	libevt_check_file_signature_file_io_handle(
	 file_io_handle,
	 NULL );

	if( libevt_file_set_ascii_codepage(
	     file,
	     LIBEVT_CODEPAGE_WINDOWS_1252,
	     NULL ) != 1 )
	{
		goto on_error_libevt;
	}
	if( libevt_file_get_ascii_codepage(
	     file,
	     &ascii_codepage,
	     NULL ) != 1 )
	{
		goto on_error_libevt;
	}
	if( libevt_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     LIBEVT_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libevt;
	}
	libevt_file_is_corrupted(
	 file,
	 NULL );

	libevt_file_get_format_version(
	 file,
	 &major_format_version,
	 &minor_format_version,
	 NULL );

	libevt_file_get_flags(
	 file,
	 &flags,
	 NULL );

	result = libevt_file_get_number_of_records(
	          file,
	          &number_of_records,
	          NULL );
       
	if( ( result == 1 )
	 && ( number_of_records > 0 ) )
	{
		if( libevt_file_get_record_by_index(
		     file,
		     0,
		     &record,
		     NULL ) == 1 )
		{
			libevt_record_free(
			 &record,
			 NULL );
		}
	}
	result = libevt_file_get_number_of_recovered_records(
	          file,
	          &number_of_recovered_records,
	          NULL );
       
	if( ( result == 1 )
	 && ( number_of_recovered_records > 0 ) )
	{
		if( libevt_file_get_recovered_record_by_index(
		     file,
		     0,
		     &record,
		     NULL ) == 1 )
		{
			libevt_record_free(
			 &record,
			 NULL );
		}
	}
	libevt_file_close(
	 file,
	 NULL );

on_error_libevt:
	libevt_file_free(
	 &file,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );

	return( 0 );
}

} /* extern "C" */
