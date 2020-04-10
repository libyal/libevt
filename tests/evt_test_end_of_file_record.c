/*
 * Library end_of_file_record type test program
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

#include "evt_test_libcerror.h"
#include "evt_test_libevt.h"
#include "evt_test_macros.h"
#include "evt_test_memory.h"
#include "evt_test_unused.h"

#include "../libevt/libevt_end_of_file_record.h"

uint8_t evt_test_end_of_file_record_data1[ 40 ] = {
	0x28, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x33, 0x33, 0x33, 0x33,
	0x44, 0x44, 0x44, 0x44, 0x30, 0x00, 0x00, 0x00, 0xe0, 0xea, 0x02, 0x00, 0x83, 0x03, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT )

/* Tests the libevt_end_of_file_record_initialize function
 * Returns 1 if successful or 0 if not
 */
int evt_test_end_of_file_record_initialize(
     void )
{
	libcerror_error_t *error                        = NULL;
	libevt_end_of_file_record_t *end_of_file_record = NULL;
	int result                                      = 0;

#if defined( HAVE_EVT_TEST_MEMORY )
	int number_of_malloc_fail_tests                 = 1;
	int number_of_memset_fail_tests                 = 1;
	int test_number                                 = 0;
#endif

	/* Test regular cases
	 */
	result = libevt_end_of_file_record_initialize(
	          &end_of_file_record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "end_of_file_record",
	 end_of_file_record );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_end_of_file_record_free(
	          &end_of_file_record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "end_of_file_record",
	 end_of_file_record );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_end_of_file_record_initialize(
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

	end_of_file_record = (libevt_end_of_file_record_t *) 0x12345678UL;

	result = libevt_end_of_file_record_initialize(
	          &end_of_file_record,
	          &error );

	end_of_file_record = NULL;

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
		/* Test libevt_end_of_file_record_initialize with malloc failing
		 */
		evt_test_malloc_attempts_before_fail = test_number;

		result = libevt_end_of_file_record_initialize(
		          &end_of_file_record,
		          &error );

		if( evt_test_malloc_attempts_before_fail != -1 )
		{
			evt_test_malloc_attempts_before_fail = -1;

			if( end_of_file_record != NULL )
			{
				libevt_end_of_file_record_free(
				 &end_of_file_record,
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
			 "end_of_file_record",
			 end_of_file_record );

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
		/* Test libevt_end_of_file_record_initialize with memset failing
		 */
		evt_test_memset_attempts_before_fail = test_number;

		result = libevt_end_of_file_record_initialize(
		          &end_of_file_record,
		          &error );

		if( evt_test_memset_attempts_before_fail != -1 )
		{
			evt_test_memset_attempts_before_fail = -1;

			if( end_of_file_record != NULL )
			{
				libevt_end_of_file_record_free(
				 &end_of_file_record,
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
			 "end_of_file_record",
			 end_of_file_record );

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
	if( end_of_file_record != NULL )
	{
		libevt_end_of_file_record_free(
		 &end_of_file_record,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevt_end_of_file_record_free function
 * Returns 1 if successful or 0 if not
 */
int evt_test_end_of_file_record_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libevt_end_of_file_record_free(
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

/* Tests the libevt_end_of_file_record_read_data function
 * Returns 1 if successful or 0 if not
 */
int evt_test_end_of_file_record_read_data(
     void )
{
	libcerror_error_t *error                        = NULL;
	libevt_end_of_file_record_t *end_of_file_record = NULL;
	int result                                      = 0;

	/* Initialize test
	 */
	result = libevt_end_of_file_record_initialize(
	          &end_of_file_record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "end_of_file_record",
	 end_of_file_record );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevt_end_of_file_record_read_data(
	          end_of_file_record,
	          evt_test_end_of_file_record_data1,
	          40,
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
	result = libevt_end_of_file_record_read_data(
	          NULL,
	          evt_test_end_of_file_record_data1,
	          40,
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

	result = libevt_end_of_file_record_read_data(
	          end_of_file_record,
	          NULL,
	          40,
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

	result = libevt_end_of_file_record_read_data(
	          end_of_file_record,
	          evt_test_end_of_file_record_data1,
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

	result = libevt_end_of_file_record_read_data(
	          end_of_file_record,
	          evt_test_end_of_file_record_data1,
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

	/* Test error case where signature1 is invalid
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_end_of_file_record_data1[ 4 ] ),
	 0xffffffffUL );

	result = libevt_end_of_file_record_read_data(
	          end_of_file_record,
	          evt_test_end_of_file_record_data1,
	          40,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_end_of_file_record_data1[ 4 ] ),
	 0x11111111UL );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where signature2 is invalid
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_end_of_file_record_data1[ 8 ] ),
	 0xffffffffUL );

	result = libevt_end_of_file_record_read_data(
	          end_of_file_record,
	          evt_test_end_of_file_record_data1,
	          40,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_end_of_file_record_data1[ 8 ] ),
	 0x22222222UL );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where signature3 is invalid
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_end_of_file_record_data1[ 12 ] ),
	 0xffffffffUL );

	result = libevt_end_of_file_record_read_data(
	          end_of_file_record,
	          evt_test_end_of_file_record_data1,
	          40,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_end_of_file_record_data1[ 12 ] ),
	 0x33333333UL );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where signature4 is invalid
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_end_of_file_record_data1[ 16 ] ),
	 0xffffffffUL );

	result = libevt_end_of_file_record_read_data(
	          end_of_file_record,
	          evt_test_end_of_file_record_data1,
	          40,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_end_of_file_record_data1[ 16 ] ),
	 0x44444444UL );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where size and copy of size mismatch
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_end_of_file_record_data1[ 36 ] ),
	 0xffffffffUL );

	result = libevt_end_of_file_record_read_data(
	          end_of_file_record,
	          evt_test_end_of_file_record_data1,
	          40,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_end_of_file_record_data1[ 36 ] ),
	 0x00000028UL );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where size and copy of size contain an unsupport value
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_end_of_file_record_data1[ 0 ] ),
	 0x00000030UL );

	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_end_of_file_record_data1[ 36 ] ),
	 0x00000030UL );

	result = libevt_end_of_file_record_read_data(
	          end_of_file_record,
	          evt_test_end_of_file_record_data1,
	          40,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_end_of_file_record_data1[ 0 ] ),
	 0x00000028UL );

	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_end_of_file_record_data1[ 36 ] ),
	 0x00000028UL );

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
	result = libevt_end_of_file_record_free(
	          &end_of_file_record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "end_of_file_record",
	 end_of_file_record );

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
	if( end_of_file_record != NULL )
	{
		libevt_end_of_file_record_free(
		 &end_of_file_record,
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
	 "libevt_end_of_file_record_initialize",
	 evt_test_end_of_file_record_initialize );

	EVT_TEST_RUN(
	 "libevt_end_of_file_record_free",
	 evt_test_end_of_file_record_free );

	EVT_TEST_RUN(
	 "libevt_end_of_file_record_read_data",
	 evt_test_end_of_file_record_read_data );

#endif /* defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

