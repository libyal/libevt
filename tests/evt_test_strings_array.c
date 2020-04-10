/*
 * Library strings_array type test program
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

#include "../libevt/libevt_strings_array.h"

uint8_t evt_test_strings_array_data1[ 28 ] = {
	0x52, 0x00, 0x53, 0x00, 0x56, 0x00, 0x50, 0x00, 0x00, 0x00, 0x51, 0x00, 0x6f, 0x00, 0x53, 0x00,
	0x20, 0x00, 0x52, 0x00, 0x53, 0x00, 0x56, 0x00, 0x50, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT )

/* Tests the libevt_strings_array_initialize function
 * Returns 1 if successful or 0 if not
 */
int evt_test_strings_array_initialize(
     void )
{
	libcerror_error_t *error              = NULL;
	libevt_strings_array_t *strings_array = NULL;
	int result                            = 0;

#if defined( HAVE_EVT_TEST_MEMORY )
	int number_of_malloc_fail_tests       = 1;
	int number_of_memset_fail_tests       = 1;
	int test_number                       = 0;
#endif

	/* Test regular cases
	 */
	result = libevt_strings_array_initialize(
	          &strings_array,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "strings_array",
	 strings_array );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_strings_array_free(
	          &strings_array,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "strings_array",
	 strings_array );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_strings_array_initialize(
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

	strings_array = (libevt_strings_array_t *) 0x12345678UL;

	result = libevt_strings_array_initialize(
	          &strings_array,
	          &error );

	strings_array = NULL;

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
		/* Test libevt_strings_array_initialize with malloc failing
		 */
		evt_test_malloc_attempts_before_fail = test_number;

		result = libevt_strings_array_initialize(
		          &strings_array,
		          &error );

		if( evt_test_malloc_attempts_before_fail != -1 )
		{
			evt_test_malloc_attempts_before_fail = -1;

			if( strings_array != NULL )
			{
				libevt_strings_array_free(
				 &strings_array,
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
			 "strings_array",
			 strings_array );

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
		/* Test libevt_strings_array_initialize with memset failing
		 */
		evt_test_memset_attempts_before_fail = test_number;

		result = libevt_strings_array_initialize(
		          &strings_array,
		          &error );

		if( evt_test_memset_attempts_before_fail != -1 )
		{
			evt_test_memset_attempts_before_fail = -1;

			if( strings_array != NULL )
			{
				libevt_strings_array_free(
				 &strings_array,
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
			 "strings_array",
			 strings_array );

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
	if( strings_array != NULL )
	{
		libevt_strings_array_free(
		 &strings_array,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevt_strings_array_free function
 * Returns 1 if successful or 0 if not
 */
int evt_test_strings_array_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libevt_strings_array_free(
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

/* Tests the libevt_strings_array_read_data function
 * Returns 1 if successful or 0 if not
 */
int evt_test_strings_array_read_data(
     void )
{
	libcerror_error_t *error              = NULL;
	libevt_strings_array_t *strings_array = NULL;
	int result                            = 0;

	/* Initialize test
	 */
	result = libevt_strings_array_initialize(
	          &strings_array,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "strings_array",
	 strings_array );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevt_strings_array_read_data(
	          strings_array,
	          evt_test_strings_array_data1,
	          28,
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
	result = libevt_strings_array_read_data(
	          strings_array,
	          evt_test_strings_array_data1,
	          28,
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
	result = libevt_strings_array_free(
	          &strings_array,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "strings_array",
	 strings_array );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libevt_strings_array_initialize(
	          &strings_array,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "strings_array",
	 strings_array );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_strings_array_read_data(
	          NULL,
	          evt_test_strings_array_data1,
	          28,
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

	result = libevt_strings_array_read_data(
	          strings_array,
	          NULL,
	          28,
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

	result = libevt_strings_array_read_data(
	          strings_array,
	          evt_test_strings_array_data1,
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

	result = libevt_strings_array_read_data(
	          strings_array,
	          evt_test_strings_array_data1,
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

	result = libevt_strings_array_read_data(
	          strings_array,
	          evt_test_strings_array_data1,
	          27,
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
	result = libevt_strings_array_free(
	          &strings_array,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "strings_array",
	 strings_array );

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
	if( strings_array != NULL )
	{
		libevt_strings_array_free(
		 &strings_array,
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
	 "libevt_strings_array_initialize",
	 evt_test_strings_array_initialize );

	EVT_TEST_RUN(
	 "libevt_strings_array_free",
	 evt_test_strings_array_free );

	EVT_TEST_RUN(
	 "libevt_strings_array_read_data",
	 evt_test_strings_array_read_data );

	/* TODO add tests for libevt_strings_array_get_number_of_strings */

	/* TODO add tests for libevt_strings_array_get_utf8_string_size */

	/* TODO add tests for libevt_strings_array_get_utf8_string */

	/* TODO add tests for libevt_strings_array_get_utf16_string_size */

	/* TODO add tests for libevt_strings_array_get_utf16_string */

#endif /* defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

