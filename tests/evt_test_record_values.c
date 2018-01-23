/*
 * Library record_values type test program
 *
 * Copyright (C) 2011-2018, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
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

#include "../libevt/libevt_record_values.h"

#if defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT )

/* Tests the libevt_record_values_initialize function
 * Returns 1 if successful or 0 if not
 */
int evt_test_record_values_initialize(
     void )
{
	libcerror_error_t *error              = NULL;
	libevt_record_values_t *record_values = NULL;
	int result                            = 0;

#if defined( HAVE_EVT_TEST_MEMORY )
	int number_of_malloc_fail_tests       = 1;
	int number_of_memset_fail_tests       = 1;
	int test_number                       = 0;
#endif

	/* Test regular cases
	 */
	result = libevt_record_values_initialize(
	          &record_values,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_record_values_free(
	          &record_values,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_record_values_initialize(
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

	record_values = (libevt_record_values_t *) 0x12345678UL;

	result = libevt_record_values_initialize(
	          &record_values,
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

	record_values = NULL;

#if defined( HAVE_EVT_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libevt_record_values_initialize with malloc failing
		 */
		evt_test_malloc_attempts_before_fail = test_number;

		result = libevt_record_values_initialize(
		          &record_values,
		          &error );

		if( evt_test_malloc_attempts_before_fail != -1 )
		{
			evt_test_malloc_attempts_before_fail = -1;

			if( record_values != NULL )
			{
				libevt_record_values_free(
				 &record_values,
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
			 "record_values",
			 record_values );

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
		/* Test libevt_record_values_initialize with memset failing
		 */
		evt_test_memset_attempts_before_fail = test_number;

		result = libevt_record_values_initialize(
		          &record_values,
		          &error );

		if( evt_test_memset_attempts_before_fail != -1 )
		{
			evt_test_memset_attempts_before_fail = -1;

			if( record_values != NULL )
			{
				libevt_record_values_free(
				 &record_values,
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
			 "record_values",
			 record_values );

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
	if( record_values != NULL )
	{
		libevt_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevt_record_values_free function
 * Returns 1 if successful or 0 if not
 */
int evt_test_record_values_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libevt_record_values_free(
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

/* Tests the libevt_record_values_clone function
 * Returns 1 if successful or 0 if not
 */
int evt_test_record_values_clone(
     void )
{
	libcerror_error_t *error                          = NULL;
	libevt_record_values_t *destination_record_values = NULL;
	libevt_record_values_t *source_record_values      = NULL;
	int result                                        = 0;

	/* Initialize test
	 */
	result = libevt_record_values_initialize(
	          &source_record_values,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "source_record_values",
	 source_record_values );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevt_record_values_clone(
	          &destination_record_values,
	          source_record_values,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "destination_record_values",
	 destination_record_values );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_record_values_free(
	          &destination_record_values,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "destination_record_values",
	 destination_record_values );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_record_values_clone(
	          &destination_record_values,
	          NULL,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "destination_record_values",
	 destination_record_values );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_record_values_clone(
	          NULL,
	          source_record_values,
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
	result = libevt_record_values_free(
	          &source_record_values,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "source_record_values",
	 source_record_values );

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
	if( destination_record_values != NULL )
	{
		libevt_record_values_free(
		 &destination_record_values,
		 NULL );
	}
	if( source_record_values != NULL )
	{
		libevt_record_values_free(
		 &source_record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevt_record_values_read function
 * Returns 1 if successful or 0 if not
 */
int evt_test_record_values_read(
     void )
{
	libcerror_error_t *error              = NULL;
	libevt_record_values_t *record_values = NULL;
	int result                            = 0;

	/* Initialize test
	 */
	result = libevt_record_values_initialize(
	          &record_values,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_record_values_read(
	          NULL,
	          NULL,
	          NULL,
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

	result = libevt_record_values_read(
	          record_values,
	          NULL,
	          NULL,
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

	/* Clean up
	 */
	result = libevt_record_values_free(
	          &record_values,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevt_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevt_record_values_get_type function
 * Returns 1 if successful or 0 if not
 */
int evt_test_record_values_get_type(
     void )
{
	libcerror_error_t *error              = NULL;
	libevt_record_values_t *record_values = NULL;
	uint8_t type                          = 0;
	int result                            = 0;
	int type_is_set                       = 0;

	/* Initialize test
	 */
	result = libevt_record_values_initialize(
	          &record_values,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevt_record_values_get_type(
	          record_values,
	          &type,
	          &error );

	EVT_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	type_is_set = result;

	/* Test error cases
	 */
	result = libevt_record_values_get_type(
	          NULL,
	          &type,
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

	if( type_is_set != 0 )
	{
		result = libevt_record_values_get_type(
		          record_values,
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
	}
	/* Clean up
	 */
	result = libevt_record_values_free(
	          &record_values,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevt_record_values_free(
		 &record_values,
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
	 "libevt_record_values_initialize",
	 evt_test_record_values_initialize );

	EVT_TEST_RUN(
	 "libevt_record_values_free",
	 evt_test_record_values_free );

	EVT_TEST_RUN(
	 "libevt_record_values_clone",
	 evt_test_record_values_clone );

	EVT_TEST_RUN(
	 "libevt_record_values_read",
	 evt_test_record_values_read );

	/* TODO: add tests for libevt_record_values_read_event */

	/* TODO: add tests for libevt_record_values_read_end_of_file */

	EVT_TEST_RUN(
	 "libevt_record_values_get_type",
	 evt_test_record_values_get_type );

	/* TODO: add tests for libevt_record_values_read_element_data */

#endif /* defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

