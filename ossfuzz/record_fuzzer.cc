/*
 * OSS-Fuzz target for libevt record type
 *
 * Copyright (C) 2011-2024, Joachim Metz <joachim.metz@gmail.com>
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
	int number_of_records            = 0;

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
	if( libevt_file_initialize(
	     &file,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libevt_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     LIBEVT_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libevt_file;
	}
	if( libevt_file_get_number_of_records(
	     file,
	     &number_of_records,
	     NULL ) != 1 )
	{
		goto on_error_libevt_file;
	}
	if( number_of_records > 0 )
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
	libevt_file_close(
	 file,
	 NULL );

on_error_libevt_file:
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

