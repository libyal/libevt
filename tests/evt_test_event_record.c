/*
 * Library event_record type test program
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "evt_test_libcerror.h"
#include "evt_test_libevt.h"
#include "evt_test_macros.h"
#include "evt_test_memory.h"
#include "evt_test_unused.h"

#include "../libevt/libevt_event_record.h"

uint8_t evt_test_event_record_data1[ 144 ] = {
	0x90, 0x00, 0x00, 0x00, 0x4c, 0x66, 0x4c, 0x65, 0x01, 0x00, 0x00, 0x00, 0x79, 0x42, 0xdb, 0x4c,
	0x79, 0x42, 0xdb, 0x4c, 0xe8, 0x03, 0x00, 0x40, 0x04, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00,
	0x04, 0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x4c, 0x00, 0x6f, 0x00, 0x61, 0x00, 0x64, 0x00,
	0x50, 0x00, 0x65, 0x00, 0x72, 0x00, 0x66, 0x00, 0x00, 0x00, 0x57, 0x00, 0x4b, 0x00, 0x53, 0x00,
	0x2d, 0x00, 0x57, 0x00, 0x49, 0x00, 0x4e, 0x00, 0x58, 0x00, 0x50, 0x00, 0x33, 0x00, 0x32, 0x00,
	0x42, 0x00, 0x49, 0x00, 0x54, 0x00, 0x00, 0x00, 0x52, 0x00, 0x53, 0x00, 0x56, 0x00, 0x50, 0x00,
	0x00, 0x00, 0x51, 0x00, 0x6f, 0x00, 0x53, 0x00, 0x20, 0x00, 0x52, 0x00, 0x53, 0x00, 0x56, 0x00,
	0x50, 0x00, 0x00, 0x00, 0x0c, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT )

/* Tests the libevt_event_record_initialize function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_initialize(
     void )
{
	libcerror_error_t *error            = NULL;
	libevt_event_record_t *event_record = NULL;
	int result                          = 0;

#if defined( HAVE_EVT_TEST_MEMORY )
	int number_of_malloc_fail_tests     = 1;
	int number_of_memset_fail_tests     = 1;
	int test_number                     = 0;
#endif

	/* Test regular cases
	 */
	result = libevt_event_record_initialize(
	          &event_record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "event_record",
	 event_record );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_event_record_free(
	          &event_record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "event_record",
	 event_record );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_initialize(
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

	event_record = (libevt_event_record_t *) 0x12345678UL;

	result = libevt_event_record_initialize(
	          &event_record,
	          &error );

	event_record = NULL;

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
		/* Test libevt_event_record_initialize with malloc failing
		 */
		evt_test_malloc_attempts_before_fail = test_number;

		result = libevt_event_record_initialize(
		          &event_record,
		          &error );

		if( evt_test_malloc_attempts_before_fail != -1 )
		{
			evt_test_malloc_attempts_before_fail = -1;

			if( event_record != NULL )
			{
				libevt_event_record_free(
				 &event_record,
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
			 "event_record",
			 event_record );

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
		/* Test libevt_event_record_initialize with memset failing
		 */
		evt_test_memset_attempts_before_fail = test_number;

		result = libevt_event_record_initialize(
		          &event_record,
		          &error );

		if( evt_test_memset_attempts_before_fail != -1 )
		{
			evt_test_memset_attempts_before_fail = -1;

			if( event_record != NULL )
			{
				libevt_event_record_free(
				 &event_record,
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
			 "event_record",
			 event_record );

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
	if( event_record != NULL )
	{
		libevt_event_record_free(
		 &event_record,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevt_event_record_free function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libevt_event_record_free(
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

/* Tests the libevt_event_record_read_data function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_read_data(
     void )
{
	libcerror_error_t *error            = NULL;
	libevt_event_record_t *event_record = NULL;
	int result                          = 0;

#if defined( HAVE_EVT_TEST_MEMORY )
#if defined( OPTIMIZATION_DISABLED )
	int number_of_memcpy_fail_tests     = 2;
#endif
	int number_of_malloc_fail_tests     = 2;
	int test_number                     = 0;
#endif

	/* Initialize test
	 */
	result = libevt_event_record_initialize(
	          &event_record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "event_record",
	 event_record );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevt_event_record_read_data(
	          event_record,
	          evt_test_event_record_data1,
	          144,
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
	result = libevt_event_record_read_data(
	          event_record,
	          evt_test_event_record_data1,
	          144,
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

	/* Clean up
	 */
	result = libevt_event_record_free(
	          &event_record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "event_record",
	 event_record );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libevt_event_record_initialize(
	          &event_record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "event_record",
	 event_record );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_read_data(
	          NULL,
	          evt_test_event_record_data1,
	          144,
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

	result = libevt_event_record_read_data(
	          event_record,
	          NULL,
	          144,
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

	result = libevt_event_record_read_data(
	          event_record,
	          evt_test_event_record_data1,
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

	result = libevt_event_record_read_data(
	          event_record,
	          evt_test_event_record_data1,
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

#if defined( HAVE_EVT_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libevt_event_record_initialize with malloc failing
		 */
		evt_test_malloc_attempts_before_fail = test_number;

		result = libevt_event_record_read_data(
		          event_record,
		          evt_test_event_record_data1,
		          144,
		          &error );

		if( evt_test_malloc_attempts_before_fail != -1 )
		{
			evt_test_malloc_attempts_before_fail = -1;
		}
		else
		{
			EVT_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			EVT_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#if defined( OPTIMIZATION_DISABLED )
	for( test_number = 0;
	     test_number < number_of_memcpy_fail_tests;
	     test_number++ )
	{
		/* Test libevt_event_record_initialize with memcpy failing
		 */
		evt_test_memcpy_attempts_before_fail = test_number;

		result = libevt_event_record_read_data(
		          event_record,
		          evt_test_event_record_data1,
		          144,
		          &error );

		if( evt_test_memcpy_attempts_before_fail != -1 )
		{
			evt_test_memcpy_attempts_before_fail = -1;
		}
		else
		{
			EVT_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			EVT_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_EVT_TEST_MEMORY ) */

	/* Test error case where signature is invalid
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_event_record_data1[ 4 ] ),
	 0xffffffffUL );

	result = libevt_event_record_read_data(
	          event_record,
	          evt_test_event_record_data1,
	          144,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( evt_test_event_record_data1[ 4 ] ),
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

	/* TODO test error case where user security identifier offset and size are out of bounds */

	/* TODO test error case where strings offset and size are out of bounds */

	/* TODO test error case where data offset and size are out of bounds */

	/* Clean up
	 */
	result = libevt_event_record_free(
	          &event_record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "event_record",
	 event_record );

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
	if( event_record != NULL )
	{
		libevt_event_record_free(
		 &event_record,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevt_event_record_get_record_number function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_record_number(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error = NULL;
	uint32_t record_number   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_record_number(
	          event_record,
	          &record_number,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_EQUAL_UINT32(
	 "record_number",
	 record_number,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_record_number(
	          NULL,
	          &record_number,
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

	result = libevt_event_record_get_record_number(
	          event_record,
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

/* Tests the libevt_event_record_get_creation_time function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_creation_time(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error = NULL;
	uint32_t posix_time      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_creation_time(
	          event_record,
	          &posix_time,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_EQUAL_UINT32(
	 "posix_time",
	 posix_time,
	 1289437817 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_creation_time(
	          NULL,
	          &posix_time,
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

	result = libevt_event_record_get_creation_time(
	          event_record,
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

/* Tests the libevt_event_record_get_last_written_time function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_last_written_time(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error = NULL;
	uint32_t posix_time      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_last_written_time(
	          event_record,
	          &posix_time,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_EQUAL_UINT32(
	 "posix_time",
	 posix_time,
	 1289437817 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_last_written_time(
	          NULL,
	          &posix_time,
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

	result = libevt_event_record_get_last_written_time(
	          event_record,
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

/* Tests the libevt_event_record_get_event_identifier function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_event_identifier(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error  = NULL;
	uint32_t event_identifier = 0;
	int result                = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_event_identifier(
	          event_record,
	          &event_identifier,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_EQUAL_UINT32(
	 "event_identifier",
	 event_identifier,
	 1073742824 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_event_identifier(
	          NULL,
	          &event_identifier,
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

	result = libevt_event_record_get_event_identifier(
	          event_record,
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

/* Tests the libevt_event_record_get_event_type function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_event_type(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error = NULL;
	uint16_t event_type      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_event_type(
	          event_record,
	          &event_type,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_EQUAL_UINT16(
	 "event_type",
	 event_type,
	 4 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_event_type(
	          NULL,
	          &event_type,
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

	result = libevt_event_record_get_event_type(
	          event_record,
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

/* Tests the libevt_event_record_get_event_category function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_event_category(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error = NULL;
	uint16_t event_category  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_event_category(
	          event_record,
	          &event_category,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_EQUAL_UINT16(
	 "event_category",
	 event_category,
	 0 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_event_category(
	          NULL,
	          &event_category,
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

	result = libevt_event_record_get_event_category(
	          event_record,
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

/* Tests the libevt_event_record_get_utf8_source_name_size function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf8_source_name_size(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf8_source_name_size(
	          event_record,
	          &utf8_string_size,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_string_size",
	 utf8_string_size,
	 (size_t) 9 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf8_source_name_size(
	          NULL,
	          &utf8_string_size,
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

	result = libevt_event_record_get_utf8_source_name_size(
	          event_record,
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

/* Tests the libevt_event_record_get_utf8_source_name function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf8_source_name(
     libevt_event_record_t *event_record )
{
	uint8_t utf8_string[ 32 ];

	uint8_t expected_utf8_string[ 9 ] = {
		'L', 'o', 'a', 'd', 'P', 'e', 'r', 'f', 0 };

	libcerror_error_t *error          = NULL;
	int result                        = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf8_source_name(
	          event_record,
	          utf8_string,
	          32,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          utf8_string,
	          expected_utf8_string,
	          9 );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf8_source_name(
	          NULL,
	          utf8_string,
	          32,
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

	result = libevt_event_record_get_utf8_source_name(
	          event_record,
	          NULL,
	          32,
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

	result = libevt_event_record_get_utf8_source_name(
	          event_record,
	          utf8_string,
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

	result = libevt_event_record_get_utf8_source_name(
	          event_record,
	          utf8_string,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libevt_event_record_get_utf16_source_name_size function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf16_source_name_size(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf16_source_name_size(
	          event_record,
	          &utf16_string_size,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_string_size",
	 utf16_string_size,
	 (size_t) 9 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf16_source_name_size(
	          NULL,
	          &utf16_string_size,
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

	result = libevt_event_record_get_utf16_source_name_size(
	          event_record,
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

/* Tests the libevt_event_record_get_utf16_source_name function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf16_source_name(
     libevt_event_record_t *event_record )
{
	uint16_t utf16_string[ 32 ];

	uint16_t expected_utf16_string[ 9 ] = {
		'L', 'o', 'a', 'd', 'P', 'e', 'r', 'f', 0 };

	libcerror_error_t *error           = NULL;
	int result                         = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf16_source_name(
	          event_record,
	          utf16_string,
	          32,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          utf16_string,
	          expected_utf16_string,
	          9 );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf16_source_name(
	          NULL,
	          utf16_string,
	          32,
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

	result = libevt_event_record_get_utf16_source_name(
	          event_record,
	          NULL,
	          32,
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

	result = libevt_event_record_get_utf16_source_name(
	          event_record,
	          utf16_string,
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

	result = libevt_event_record_get_utf16_source_name(
	          event_record,
	          utf16_string,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libevt_event_record_get_utf8_computer_name_size function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf8_computer_name_size(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf8_computer_name_size(
	          event_record,
	          &utf8_string_size,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_string_size",
	 utf8_string_size,
	 (size_t) 15 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf8_computer_name_size(
	          NULL,
	          &utf8_string_size,
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

	result = libevt_event_record_get_utf8_computer_name_size(
	          event_record,
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

/* Tests the libevt_event_record_get_utf8_computer_name function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf8_computer_name(
     libevt_event_record_t *event_record )
{
	uint8_t utf8_string[ 32 ];

	uint8_t expected_utf8_string[ 15 ] = {
		'W', 'K', 'S', '-', 'W', 'I', 'N', 'X', 'P', '3', '2', 'B', 'I', 'T', 0 };

	libcerror_error_t *error           = NULL;
	int result                         = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf8_computer_name(
	          event_record,
	          utf8_string,
	          32,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          utf8_string,
	          expected_utf8_string,
	          15 );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf8_computer_name(
	          NULL,
	          utf8_string,
	          32,
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

	result = libevt_event_record_get_utf8_computer_name(
	          event_record,
	          NULL,
	          32,
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

	result = libevt_event_record_get_utf8_computer_name(
	          event_record,
	          utf8_string,
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

	result = libevt_event_record_get_utf8_computer_name(
	          event_record,
	          utf8_string,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libevt_event_record_get_utf16_computer_name_size function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf16_computer_name_size(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf16_computer_name_size(
	          event_record,
	          &utf16_string_size,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_string_size",
	 utf16_string_size,
	 (size_t) 15 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf16_computer_name_size(
	          NULL,
	          &utf16_string_size,
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

	result = libevt_event_record_get_utf16_computer_name_size(
	          event_record,
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

/* Tests the libevt_event_record_get_utf16_computer_name function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf16_computer_name(
     libevt_event_record_t *event_record )
{
	uint16_t utf16_string[ 32 ];

	uint16_t expected_utf16_string[ 15 ] = {
		'W', 'K', 'S', '-', 'W', 'I', 'N', 'X', 'P', '3', '2', 'B', 'I', 'T', 0 };

	libcerror_error_t *error            = NULL;
	int result                          = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf16_computer_name(
	          event_record,
	          utf16_string,
	          32,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          utf16_string,
	          expected_utf16_string,
	          15 );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf16_computer_name(
	          NULL,
	          utf16_string,
	          32,
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

	result = libevt_event_record_get_utf16_computer_name(
	          event_record,
	          NULL,
	          32,
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

	result = libevt_event_record_get_utf16_computer_name(
	          event_record,
	          utf16_string,
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

	result = libevt_event_record_get_utf16_computer_name(
	          event_record,
	          utf16_string,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libevt_event_record_get_utf8_user_security_identifier_size function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf8_user_security_identifier_size(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf8_user_security_identifier_size(
	          event_record,
	          &utf8_string_size,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf8_user_security_identifier_size(
	          NULL,
	          &utf8_string_size,
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

/* Tests the libevt_event_record_get_utf8_user_security_identifier function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf8_user_security_identifier(
     libevt_event_record_t *event_record )
{
	uint8_t utf8_string[ 32 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf8_user_security_identifier(
	          event_record,
	          utf8_string,
	          32,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf8_user_security_identifier(
	          NULL,
	          utf8_string,
	          32,
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

/* Tests the libevt_event_record_get_utf16_user_security_identifier_size function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf16_user_security_identifier_size(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf16_user_security_identifier_size(
	          event_record,
	          &utf16_string_size,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf16_user_security_identifier_size(
	          NULL,
	          &utf16_string_size,
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

/* Tests the libevt_event_record_get_utf16_user_security_identifier function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf16_user_security_identifier(
     libevt_event_record_t *event_record )
{
	uint16_t utf16_string[ 32 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf16_user_security_identifier(
	          event_record,
	          utf16_string,
	          32,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf16_user_security_identifier(
	          NULL,
	          utf16_string,
	          32,
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

/* Tests the libevt_event_record_get_number_of_strings function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_number_of_strings(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error = NULL;
	int number_of_strings    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_number_of_strings(
	          event_record,
	          &number_of_strings,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_EQUAL_UINT16(
	 "number_of_strings",
	 number_of_strings,
	 2 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_number_of_strings(
	          NULL,
	          &number_of_strings,
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

	result = libevt_event_record_get_number_of_strings(
	          event_record,
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

/* Tests the libevt_event_record_get_utf8_string_size function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf8_string_size(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf8_string_size(
	          event_record,
	          0,
	          &utf8_string_size,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_string_size",
	 utf8_string_size,
	 (size_t) 5 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf8_string_size(
	          NULL,
	          0,
	          &utf8_string_size,
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

	result = libevt_event_record_get_utf8_string_size(
	          event_record,
	          -1,
	          &utf8_string_size,
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

	result = libevt_event_record_get_utf8_string_size(
	          event_record,
	          0,
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

/* Tests the libevt_event_record_get_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf8_string(
     libevt_event_record_t *event_record )
{
	uint8_t utf8_string[ 32 ];

	uint8_t expected_utf8_string[ 5 ] = {
		'R', 'S', 'V', 'P', 0 };

	libcerror_error_t *error          = NULL;
	int result                        = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf8_string(
	          event_record,
	          0,
	          utf8_string,
	          32,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          utf8_string,
	          expected_utf8_string,
	          5 );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf8_string(
	          NULL,
	          0,
	          utf8_string,
	          32,
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

	result = libevt_event_record_get_utf8_string(
	          event_record,
	          -1,
	          utf8_string,
	          32,
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

	result = libevt_event_record_get_utf8_string(
	          event_record,
	          0,
	          NULL,
	          32,
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

	result = libevt_event_record_get_utf8_string(
	          event_record,
	          0,
	          utf8_string,
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

	result = libevt_event_record_get_utf8_string(
	          event_record,
	          0,
	          utf8_string,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libevt_event_record_get_utf16_string_size function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf16_string_size(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf16_string_size(
	          event_record,
	          0,
	          &utf16_string_size,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_string_size",
	 utf16_string_size,
	 (size_t) 5 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf16_string_size(
	          NULL,
	          0,
	          &utf16_string_size,
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

	result = libevt_event_record_get_utf16_string_size(
	          event_record,
	          -1,
	          &utf16_string_size,
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

	result = libevt_event_record_get_utf16_string_size(
	          event_record,
	          0,
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

/* Tests the libevt_event_record_get_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_utf16_string(
     libevt_event_record_t *event_record )
{
	uint16_t utf16_string[ 32 ];

	uint16_t expected_utf16_string[ 5 ] = {
		'R', 'S', 'V', 'P', 0 };

	libcerror_error_t *error            = NULL;
	int result                          = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_utf16_string(
	          event_record,
	          0,
	          utf16_string,
	          32,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          utf16_string,
	          expected_utf16_string,
	          5 );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libevt_event_record_get_utf16_string(
	          NULL,
	          0,
	          utf16_string,
	          32,
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

	result = libevt_event_record_get_utf16_string(
	          event_record,
	          -1,
	          utf16_string,
	          32,
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

	result = libevt_event_record_get_utf16_string(
	          event_record,
	          0,
	          NULL,
	          32,
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

	result = libevt_event_record_get_utf16_string(
	          event_record,
	          0,
	          utf16_string,
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

	result = libevt_event_record_get_utf16_string(
	          event_record,
	          0,
	          utf16_string,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libevt_event_record_get_data_size function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_data_size(
     libevt_event_record_t *event_record )
{
	libcerror_error_t *error = NULL;
	size_t data_size         = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_data_size(
	          event_record,
	          &data_size,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_EQUAL_SIZE(
	 "data_size",
	 data_size,
	 (size_t) 4 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_event_record_get_data_size(
	          NULL,
	          &data_size,
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

	result = libevt_event_record_get_data_size(
	          event_record,
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

/* Tests the libevt_event_record_get_data function
 * Returns 1 if successful or 0 if not
 */
int evt_test_event_record_get_data(
     libevt_event_record_t *event_record )
{
	uint8_t data[ 32 ];

	uint8_t expected_data[ 4 ] = {
		0x0c, 0x14, 0x00, 0x00 };

	libcerror_error_t *error   = NULL;
	int result                 = 0;

	/* Test regular cases
	 */
	result = libevt_event_record_get_data(
	          event_record,
	          data,
	          32,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          data,
	          expected_data,
	          4 );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libevt_event_record_get_data(
	          NULL,
	          data,
	          32,
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

	result = libevt_event_record_get_data(
	          event_record,
	          NULL,
	          32,
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

	result = libevt_event_record_get_data(
	          event_record,
	          data,
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

	result = libevt_event_record_get_data(
	          event_record,
	          data,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
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
#if defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT )

	libcerror_error_t *error            = NULL;
	libevt_event_record_t *event_record = NULL;
	int result                          = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT ) */

	EVT_TEST_UNREFERENCED_PARAMETER( argc )
	EVT_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT )

	EVT_TEST_RUN(
	 "libevt_event_record_initialize",
	 evt_test_event_record_initialize );

	EVT_TEST_RUN(
	 "libevt_event_record_free",
	 evt_test_event_record_free );

	EVT_TEST_RUN(
	 "libevt_event_record_read_data",
	 evt_test_event_record_read_data );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = libevt_event_record_initialize(
	          &event_record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "event_record",
	 event_record );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_event_record_read_data(
	          event_record,
	          evt_test_event_record_data1,
	          144,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_record_number",
	 evt_test_event_record_get_record_number,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_creation_time",
	 evt_test_event_record_get_creation_time,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_last_written_time",
	 evt_test_event_record_get_last_written_time,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_event_identifier",
	 evt_test_event_record_get_event_identifier,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_event_type",
	 evt_test_event_record_get_event_type,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_event_category",
	 evt_test_event_record_get_event_category,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf8_source_name_size",
	 evt_test_event_record_get_utf8_source_name_size,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf8_source_name",
	 evt_test_event_record_get_utf8_source_name,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf16_source_name_size",
	 evt_test_event_record_get_utf16_source_name_size,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf16_source_name",
	 evt_test_event_record_get_utf16_source_name,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf8_computer_name_size",
	 evt_test_event_record_get_utf8_computer_name_size,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf8_computer_name",
	 evt_test_event_record_get_utf8_computer_name,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf16_computer_name_size",
	 evt_test_event_record_get_utf16_computer_name_size,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf16_computer_name",
	 evt_test_event_record_get_utf16_computer_name,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf8_user_security_identifier_size",
	 evt_test_event_record_get_utf8_user_security_identifier_size,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf8_user_security_identifier",
	 evt_test_event_record_get_utf8_user_security_identifier,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf16_user_security_identifier_size",
	 evt_test_event_record_get_utf16_user_security_identifier_size,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf16_user_security_identifier",
	 evt_test_event_record_get_utf16_user_security_identifier,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_number_of_strings",
	 evt_test_event_record_get_number_of_strings,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf8_string_size",
	 evt_test_event_record_get_utf8_string_size,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf8_string",
	 evt_test_event_record_get_utf8_string,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf16_string_size",
	 evt_test_event_record_get_utf16_string_size,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_utf16_string",
	 evt_test_event_record_get_utf16_string,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_data_size",
	 evt_test_event_record_get_data_size,
	 event_record );

	EVT_TEST_RUN_WITH_ARGS(
	 "libevt_event_record_get_data",
	 evt_test_event_record_get_data,
	 event_record );

	/* Clean up
	 */
	result = libevt_event_record_free(
	          &event_record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "event_record",
	 event_record );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
#if defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT )

	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( event_record != NULL )
	{
		libevt_event_record_free(
		 &event_record,
		 NULL );
	}
#endif /* defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}

