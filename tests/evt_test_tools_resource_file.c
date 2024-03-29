/*
 * Tools resource_file type test program
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

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "evt_test_libcerror.h"
#include "evt_test_macros.h"
#include "evt_test_memory.h"
#include "evt_test_unused.h"

#include "../evttools/resource_file.h"

/* Tests the resource_file_initialize function
 * Returns 1 if successful or 0 if not
 */
int evt_test_tools_resource_file_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	resource_file_t *resource_file  = NULL;
	int result                      = 0;

#if defined( HAVE_EVT_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = resource_file_initialize(
	          &resource_file,
	          0x00000409UL,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "resource_file",
	 resource_file );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = resource_file_free(
	          &resource_file,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "resource_file",
	 resource_file );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = resource_file_initialize(
	          NULL,
	          0x00000409UL,
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

	resource_file = (resource_file_t *) 0x12345678UL;

	result = resource_file_initialize(
	          &resource_file,
	          0x00000409UL,
	          &error );

	resource_file = NULL;

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
		/* Test resource_file_initialize with malloc failing
		 */
		evt_test_malloc_attempts_before_fail = test_number;

		result = resource_file_initialize(
		          &resource_file,
		          0x00000409UL,
		          &error );

		if( evt_test_malloc_attempts_before_fail != -1 )
		{
			evt_test_malloc_attempts_before_fail = -1;

			if( resource_file != NULL )
			{
				resource_file_free(
				 &resource_file,
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
			 "resource_file",
			 resource_file );

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
		/* Test resource_file_initialize with memset failing
		 */
		evt_test_memset_attempts_before_fail = test_number;

		result = resource_file_initialize(
		          &resource_file,
		          0x00000409UL,
		          &error );

		if( evt_test_memset_attempts_before_fail != -1 )
		{
			evt_test_memset_attempts_before_fail = -1;

			if( resource_file != NULL )
			{
				resource_file_free(
				 &resource_file,
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
			 "resource_file",
			 resource_file );

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
	if( resource_file != NULL )
	{
		resource_file_free(
		 &resource_file,
		 NULL );
	}
	return( 0 );
}

/* Tests the resource_file_free function
 * Returns 1 if successful or 0 if not
 */
int evt_test_tools_resource_file_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = resource_file_free(
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

	EVT_TEST_RUN(
	 "resource_file_initialize",
	 evt_test_tools_resource_file_initialize );

	EVT_TEST_RUN(
	 "resource_file_free",
	 evt_test_tools_resource_file_free );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

