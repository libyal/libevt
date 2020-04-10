/*
 * Library file_header type test program
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "evt_test_functions.h"
#include "evt_test_libbfio.h"
#include "evt_test_libcerror.h"
#include "evt_test_libevt.h"
#include "evt_test_macros.h"
#include "evt_test_memory.h"
#include "evt_test_unused.h"

#include "../libevt/libevt_file_header.h"

uint8_t evt_test_file_header_data1[ 48 ] = {
	0x30, 0x00, 0x00, 0x00, 0x4c, 0x66, 0x4c, 0x65, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x30, 0x00, 0x00, 0x00, 0x38, 0xe5, 0x0a, 0x00, 0x1b, 0x0f, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x0b, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT )

/* Tests the libevt_file_header_initialize function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_header_initialize(
     void )
{
	libcerror_error_t *error          = NULL;
	libevt_file_header_t *file_header = NULL;
	int result                        = 0;

#if defined( HAVE_EVT_TEST_MEMORY )
	int number_of_malloc_fail_tests   = 1;
	int number_of_memset_fail_tests   = 1;
	int test_number                   = 0;
#endif

	/* Test regular cases
	 */
	result = libevt_file_header_initialize(
	          &file_header,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "file_header",
	 file_header );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_file_header_free(
	          &file_header,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "file_header",
	 file_header );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_file_header_initialize(
	          NULL,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	file_header = (libevt_file_header_t *) 0x12345678UL;

	result = libevt_file_header_initialize(
	          &file_header,
	          &error );

	file_header = NULL;

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_EVT_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libevt_file_header_initialize with malloc failing
		 */
		evt_test_malloc_attempts_before_fail = test_number;

		result = libevt_file_header_initialize(
		          &file_header,
		          &error );

		if( evt_test_malloc_attempts_before_fail != -1 )
		{
			evt_test_malloc_attempts_before_fail = -1;

			if( file_header != NULL )
			{
				libevt_file_header_free(
				 &file_header,
				 NULL );
			}
		}
		else
		{
			EVT_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			EVT_TEST_ASSERT_IS_NULL(
			 "file_header",
			 file_header );

			EVT_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libevt_file_header_initialize with memset failing
		 */
		evt_test_memset_attempts_before_fail = test_number;

		result = libevt_file_header_initialize(
		          &file_header,
		          &error );

		if( evt_test_memset_attempts_before_fail != -1 )
		{
			evt_test_memset_attempts_before_fail = -1;

			if( file_header != NULL )
			{
				libevt_file_header_free(
				 &file_header,
				 NULL );
			}
		}
		else
		{
			EVT_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			EVT_TEST_ASSERT_IS_NULL(
			 "file_header",
			 file_header );

			EVT_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_EVT_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file_header != NULL )
	{
		libevt_file_header_free(
		 &file_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevt_file_header_free function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_header_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libevt_file_header_free(
	          NULL,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libevt_file_header_read_data function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_header_read_data(
     void )
{
	libcerror_error_t *error          = NULL;
	libevt_file_header_t *file_header = NULL;
	int result                        = 0;

	/* Initialize test
	 */
	result = libevt_file_header_initialize(
	          &file_header,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "file_header",
	 file_header );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevt_file_header_read_data(
	          file_header,
	          evt_test_file_header_data1,
	          48,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_file_header_read_data(
	          NULL,
	          evt_test_file_header_data1,
	          48,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libevt_file_header_read_data(
	          file_header,
	          NULL,
	          48,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libevt_file_header_read_data(
	          file_header,
	          evt_test_file_header_data1,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libevt_file_header_read_data(
	          file_header,
	          evt_test_file_header_data1,
	          0,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where signature is invalid
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_file_header_data1[ 4 ] ),
	 0xffffffffUL );

	result = libevt_file_header_read_data(
	          file_header,
	          evt_test_file_header_data1,
	          0,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_file_header_data1[ 4 ] ),
	 0x654c664cUL );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libevt_file_header_free(
	          &file_header,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "file_header",
	 file_header );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file_header != NULL )
	{
		libevt_file_header_free(
		 &file_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevt_file_header_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_header_read_file_io_handle(
     void )
{
	libbfio_handle_t *file_io_handle  = NULL;
	libcerror_error_t *error          = NULL;
	libevt_file_header_t *file_header = NULL;
	int result                        = 0;

	/* Initialize test
	 */
	result = libevt_file_header_initialize(
	          &file_header,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "file_header",
	 file_header );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = evt_test_open_file_io_handle(
	          &file_io_handle,
	          evt_test_file_header_data1,
	          48,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevt_file_header_read_file_io_handle(
	          file_header,
	          file_io_handle,
	          0,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_file_header_read_file_io_handle(
	          NULL,
	          file_io_handle,
	          0,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libevt_file_header_read_file_io_handle(
	          file_header,
	          NULL,
	          0,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libevt_file_header_read_file_io_handle(
	          file_header,
	          file_io_handle,
	          -1,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up file IO handle
	 */
	result = evt_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test data too small
	 */
	result = evt_test_open_file_io_handle(
	          &file_io_handle,
	          evt_test_file_header_data1,
	          8,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_file_header_read_file_io_handle(
	          file_header,
	          file_io_handle,
	          0,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = evt_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test invalid signature
	 */
	result = evt_test_open_file_io_handle(
	          &file_io_handle,
	          evt_test_file_header_data1,
	          48,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_file_header_data1[ 4 ] ),
	 0xffffffffUL );

	result = libevt_file_header_read_file_io_handle(
	          file_header,
	          file_io_handle,
	          0,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_file_header_data1[ 4 ] ),
	 0x654c664cUL );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = evt_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libevt_file_header_free(
	          &file_header,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "file_header",
	 file_header );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( file_header != NULL )
	{
		libevt_file_header_free(
		 &file_header,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc EVT_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] EVT_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc EVT_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] EVT_TEST_ATTRIBUTE_UNUSED )
#endif
{
	EVT_TEST_UNREFERENCED_PARAMETER( argc )
	EVT_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT )

	EVT_TEST_RUN(
	 "libevt_file_header_initialize",
	 evt_test_file_header_initialize );

	EVT_TEST_RUN(
	 "libevt_file_header_free",
	 evt_test_file_header_free );

	EVT_TEST_RUN(
	 "libevt_file_header_read_data",
	 evt_test_file_header_read_data );

	EVT_TEST_RUN(
	 "libevt_file_header_read_file_io_handle",
	 evt_test_file_header_read_file_io_handle );

#endif /* defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

