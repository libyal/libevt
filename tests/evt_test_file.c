/*
 * Library file type test program
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "evt_test_functions.h"
#include "evt_test_getopt.h"
#include "evt_test_libbfio.h"
#include "evt_test_libcerror.h"
#include "evt_test_libevt.h"
#include "evt_test_macros.h"
#include "evt_test_memory.h"
#include "evt_test_unused.h"

#include "../libevt/libevt_file.h"

#if !defined( LIBEVT_HAVE_BFIO )

LIBEVT_EXTERN \
int libevt_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_file_open_file_io_handle(
     libevt_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libevt_error_t **error );

#endif /* !defined( LIBEVT_HAVE_BFIO ) */

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make evt_test_file generate verbose output
#define EVT_TEST_FILE_VERBOSE
 */

/* Creates and opens a source file
 * Returns 1 if successful or -1 on error
 */
int evt_test_file_open_source(
     libevt_file_t **file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "evt_test_file_open_source";
	int result            = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libevt_file_initialize(
	     file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file.",
		 function );

		goto on_error;
	}
	result = libevt_file_open_file_io_handle(
	          *file,
	          file_io_handle,
	          LIBEVT_OPEN_READ,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file != NULL )
	{
		libevt_file_free(
		 file,
		 NULL );
	}
	return( -1 );
}

/* Closes and frees a source file
 * Returns 1 if successful or -1 on error
 */
int evt_test_file_close_source(
     libevt_file_t **file,
     libcerror_error_t **error )
{
	static char *function = "evt_test_file_close_source";
	int result            = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( libevt_file_close(
	     *file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file.",
		 function );

		result = -1;
	}
	if( libevt_file_free(
	     file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libevt_file_initialize function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libevt_file_t *file             = NULL;
	int result                      = 0;

#if defined( HAVE_EVT_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libevt_file_initialize(
	          &file,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_file_free(
	          &file,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevt_file_initialize(
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

	file = (libevt_file_t *) 0x12345678UL;

	result = libevt_file_initialize(
	          &file,
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

	file = NULL;

#if defined( HAVE_EVT_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libevt_file_initialize with malloc failing
		 */
		evt_test_malloc_attempts_before_fail = test_number;

		result = libevt_file_initialize(
		          &file,
		          &error );

		if( evt_test_malloc_attempts_before_fail != -1 )
		{
			evt_test_malloc_attempts_before_fail = -1;

			if( file != NULL )
			{
				libevt_file_free(
				 &file,
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
			 "file",
			 file );

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
		/* Test libevt_file_initialize with memset failing
		 */
		evt_test_memset_attempts_before_fail = test_number;

		result = libevt_file_initialize(
		          &file,
		          &error );

		if( evt_test_memset_attempts_before_fail != -1 )
		{
			evt_test_memset_attempts_before_fail = -1;

			if( file != NULL )
			{
				libevt_file_free(
				 &file,
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
			 "file",
			 file );

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
	if( file != NULL )
	{
		libevt_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevt_file_free function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libevt_file_free(
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

/* Tests the libevt_file_open function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_open(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error = NULL;
	libevt_file_t *file      = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = evt_test_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_file_initialize(
	          &file,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libevt_file_open(
	          file,
	          narrow_source,
	          LIBEVT_OPEN_READ,
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
	result = libevt_file_open(
	          NULL,
	          narrow_source,
	          LIBEVT_OPEN_READ,
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

	result = libevt_file_open(
	          file,
	          NULL,
	          LIBEVT_OPEN_READ,
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

	result = libevt_file_open(
	          file,
	          narrow_source,
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

	/* Test open when already opened
	 */
	result = libevt_file_open(
	          file,
	          narrow_source,
	          LIBEVT_OPEN_READ,
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
	result = libevt_file_free(
	          &file,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

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
	if( file != NULL )
	{
		libevt_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libevt_file_open_wide function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_open_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error = NULL;
	libevt_file_t *file      = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = evt_test_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_file_initialize(
	          &file,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libevt_file_open_wide(
	          file,
	          wide_source,
	          LIBEVT_OPEN_READ,
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
	result = libevt_file_open_wide(
	          NULL,
	          wide_source,
	          LIBEVT_OPEN_READ,
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

	result = libevt_file_open_wide(
	          file,
	          NULL,
	          LIBEVT_OPEN_READ,
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

	result = libevt_file_open_wide(
	          file,
	          wide_source,
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

	/* Test open when already opened
	 */
	result = libevt_file_open_wide(
	          file,
	          wide_source,
	          LIBEVT_OPEN_READ,
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
	result = libevt_file_free(
	          &file,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

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
	if( file != NULL )
	{
		libevt_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libevt_file_open_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_open_file_io_handle(
     const system_character_t *source )
{
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libevt_file_t *file              = NULL;
	size_t string_length             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libbfio_file_initialize(
	          &file_io_handle,
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

	string_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbfio_file_set_name_wide(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#else
	result = libbfio_file_set_name(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#endif
	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EVT_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libevt_file_initialize(
	          &file,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libevt_file_open_file_io_handle(
	          file,
	          file_io_handle,
	          LIBEVT_OPEN_READ,
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
	result = libevt_file_open_file_io_handle(
	          NULL,
	          file_io_handle,
	          LIBEVT_OPEN_READ,
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

	result = libevt_file_open_file_io_handle(
	          file,
	          NULL,
	          LIBEVT_OPEN_READ,
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

	result = libevt_file_open_file_io_handle(
	          file,
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

	/* Test open when already opened
	 */
	result = libevt_file_open_file_io_handle(
	          file,
	          file_io_handle,
	          LIBEVT_OPEN_READ,
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
	result = libevt_file_free(
	          &file,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_handle_free(
	          &file_io_handle,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
         "file_io_handle",
         file_io_handle );

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
	if( file != NULL )
	{
		libevt_file_free(
		 &file,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevt_file_close function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libevt_file_close(
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

/* Tests the libevt_file_open and libevt_file_close functions
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_open_close(
     const system_character_t *source )
{
	libcerror_error_t *error = NULL;
	libevt_file_t *file      = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = libevt_file_initialize(
	          &file,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libevt_file_open_wide(
	          file,
	          source,
	          LIBEVT_OPEN_READ,
	          &error );
#else
	result = libevt_file_open(
	          file,
	          source,
	          LIBEVT_OPEN_READ,
	          &error );
#endif

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_file_close(
	          file,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libevt_file_open_wide(
	          file,
	          source,
	          LIBEVT_OPEN_READ,
	          &error );
#else
	result = libevt_file_open(
	          file,
	          source,
	          LIBEVT_OPEN_READ,
	          &error );
#endif

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_file_close(
	          file,
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
	result = libevt_file_free(
	          &file,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

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
	if( file != NULL )
	{
		libevt_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevt_file_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_signal_abort(
     libevt_file_t *file )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_file_signal_abort(
	          file,
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
	result = libevt_file_signal_abort(
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

/* Tests the libevt_file_get_ascii_codepage function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_get_ascii_codepage(
     libevt_file_t *file )
{
	libcerror_error_t *error  = NULL;
	int ascii_codepage        = 0;
	int ascii_codepage_is_set = 0;
	int result                = 0;

	/* Test regular cases
	 */
	result = libevt_file_get_ascii_codepage(
	          file,
	          &ascii_codepage,
	          &error );

	EVT_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	ascii_codepage_is_set = result;

	/* Test error cases
	 */
	result = libevt_file_get_ascii_codepage(
	          NULL,
	          &ascii_codepage,
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

	if( ascii_codepage_is_set != 0 )
	{
		result = libevt_file_get_ascii_codepage(
		          file,
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
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libevt_file_set_ascii_codepage function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_set_ascii_codepage(
     libevt_file_t *file )
{
	int supported_codepages[ 15 ] = {
		LIBEVT_CODEPAGE_ASCII,
		LIBEVT_CODEPAGE_WINDOWS_874,
		LIBEVT_CODEPAGE_WINDOWS_932,
		LIBEVT_CODEPAGE_WINDOWS_936,
		LIBEVT_CODEPAGE_WINDOWS_949,
		LIBEVT_CODEPAGE_WINDOWS_950,
		LIBEVT_CODEPAGE_WINDOWS_1250,
		LIBEVT_CODEPAGE_WINDOWS_1251,
		LIBEVT_CODEPAGE_WINDOWS_1252,
		LIBEVT_CODEPAGE_WINDOWS_1253,
		LIBEVT_CODEPAGE_WINDOWS_1254,
		LIBEVT_CODEPAGE_WINDOWS_1255,
		LIBEVT_CODEPAGE_WINDOWS_1256,
		LIBEVT_CODEPAGE_WINDOWS_1257,
		LIBEVT_CODEPAGE_WINDOWS_1258 };

	int unsupported_codepages[ 17 ] = {
		LIBEVT_CODEPAGE_ISO_8859_1,
		LIBEVT_CODEPAGE_ISO_8859_2,
		LIBEVT_CODEPAGE_ISO_8859_3,
		LIBEVT_CODEPAGE_ISO_8859_4,
		LIBEVT_CODEPAGE_ISO_8859_5,
		LIBEVT_CODEPAGE_ISO_8859_6,
		LIBEVT_CODEPAGE_ISO_8859_7,
		LIBEVT_CODEPAGE_ISO_8859_8,
		LIBEVT_CODEPAGE_ISO_8859_9,
		LIBEVT_CODEPAGE_ISO_8859_10,
		LIBEVT_CODEPAGE_ISO_8859_11,
		LIBEVT_CODEPAGE_ISO_8859_13,
		LIBEVT_CODEPAGE_ISO_8859_14,
		LIBEVT_CODEPAGE_ISO_8859_15,
		LIBEVT_CODEPAGE_ISO_8859_16,
		LIBEVT_CODEPAGE_KOI8_R,
		LIBEVT_CODEPAGE_KOI8_U };

	libcerror_error_t *error = NULL;
	int codepage             = 0;
	int index                = 0;
	int result               = 0;

	/* Test set ASCII codepage
	 */
	for( index = 0;
	     index < 15;
	     index++ )
	{
		codepage = supported_codepages[ index ];

		result = libevt_file_set_ascii_codepage(
		          file,
		          codepage,
		          &error );

		EVT_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EVT_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libevt_file_set_ascii_codepage(
	          NULL,
	          LIBEVT_CODEPAGE_ASCII,
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

	for( index = 0;
	     index < 17;
	     index++ )
	{
		codepage = unsupported_codepages[ index ];

		result = libevt_file_set_ascii_codepage(
		          file,
		          codepage,
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
	result = libevt_file_set_ascii_codepage(
	          file,
	          LIBEVT_CODEPAGE_WINDOWS_1252,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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
	return( 0 );
}

/* Tests the libevt_file_get_flags function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_get_flags(
     libevt_file_t *file )
{
	libcerror_error_t *error = NULL;
	uint32_t flags           = 0;
	int flags_is_set         = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libevt_file_get_flags(
	          file,
	          &flags,
	          &error );

	EVT_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	flags_is_set = result;

	/* Test error cases
	 */
	result = libevt_file_get_flags(
	          NULL,
	          &flags,
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

	if( flags_is_set != 0 )
	{
		result = libevt_file_get_flags(
		          file,
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
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libevt_file_get_number_of_records function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_get_number_of_records(
     libevt_file_t *file )
{
	libcerror_error_t *error     = NULL;
	int number_of_records        = 0;
	int number_of_records_is_set = 0;
	int result                   = 0;

	/* Test regular cases
	 */
	result = libevt_file_get_number_of_records(
	          file,
	          &number_of_records,
	          &error );

	EVT_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_records_is_set = result;

	/* Test error cases
	 */
	result = libevt_file_get_number_of_records(
	          NULL,
	          &number_of_records,
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

	if( number_of_records_is_set != 0 )
	{
		result = libevt_file_get_number_of_records(
		          file,
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
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libevt_file_get_record function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_get_record(
     libevt_file_t *file )
{
	libcerror_error_t *error = NULL;
	libevt_record_t *record  = 0;
	int number_of_records    = 0;
	int result               = 0;

	/* Initialize test
	 */
	result = libevt_file_get_number_of_records(
	          file,
	          &number_of_records,
	          &error );

	EVT_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( number_of_records == 0 )
	{
		return( 1 );
	}
	/* Test regular cases
	 */
	result = libevt_file_get_record(
	          file,
	          0,
	          &record,
	          &error );

	EVT_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "record",
	 record );

	result = libevt_record_free(
	          &record,
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
	result = libevt_file_get_record(
	          NULL,
	          0,
	          &record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "record",
	 record );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libevt_file_get_record(
	          file,
	          -1,
	          &record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "record",
	 record );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libevt_file_get_record(
	          file,
	          0,
	          NULL,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "record",
	 record );

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

/* Tests the libevt_file_get_number_of_recovered_records function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_get_number_of_recovered_records(
     libevt_file_t *file )
{
	libcerror_error_t *error               = NULL;
	int number_of_recovered_records        = 0;
	int number_of_recovered_records_is_set = 0;
	int result                             = 0;

	/* Test regular cases
	 */
	result = libevt_file_get_number_of_recovered_records(
	          file,
	          &number_of_recovered_records,
	          &error );

	EVT_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_recovered_records_is_set = result;

	/* Test error cases
	 */
	result = libevt_file_get_number_of_recovered_records(
	          NULL,
	          &number_of_recovered_records,
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

	if( number_of_recovered_records_is_set != 0 )
	{
		result = libevt_file_get_number_of_recovered_records(
		          file,
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
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libevt_file_get_recovered_record function
 * Returns 1 if successful or 0 if not
 */
int evt_test_file_get_recovered_record(
     libevt_file_t *file )
{
	libcerror_error_t *error          = NULL;
	libevt_record_t *recovered_record = 0;
	int number_of_recovered_records   = 0;
	int result                        = 0;

	/* Initialize test
	 */
	result = libevt_file_get_number_of_recovered_records(
	          file,
	          &number_of_recovered_records,
	          &error );

	EVT_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( number_of_recovered_records == 0 )
	{
		return( 1 );
	}
	/* Test regular cases
	 */
	result = libevt_file_get_recovered_record(
	          file,
	          0,
	          &recovered_record,
	          &error );

	EVT_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "recovered_record",
	 recovered_record );

	result = libevt_record_free(
	          &recovered_record,
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
	result = libevt_file_get_recovered_record(
	          NULL,
	          0,
	          &recovered_record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "recovered_record",
	 recovered_record );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libevt_file_get_recovered_record(
	          file,
	          -1,
	          &recovered_record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "recovered_record",
	 recovered_record );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libevt_file_get_recovered_record(
	          file,
	          0,
	          NULL,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "recovered_record",
	 recovered_record );

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
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libevt_file_t *file              = NULL;
	system_character_t *source       = NULL;
	system_integer_t option          = 0;
	size_t string_length             = 0;
	int result                       = 0;

	while( ( option = evt_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( EVT_TEST_FILE_VERBOSE )
	libevt_notify_set_verbose(
	 1 );
	libevt_notify_set_stream(
	 stderr,
	 NULL );
#endif

	EVT_TEST_RUN(
	 "libevt_file_initialize",
	 evt_test_file_initialize );

	EVT_TEST_RUN(
	 "libevt_file_free",
	 evt_test_file_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		result = libbfio_file_initialize(
		          &file_io_handle,
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

		string_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_set_name_wide(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#else
		result = libbfio_file_set_name(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#endif
		EVT_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        EVT_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		result = libevt_check_file_signature_file_io_handle(
		          file_io_handle,
		          &error );

		EVT_TEST_ASSERT_NOT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EVT_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( result != 0 )
	{
		EVT_TEST_RUN_WITH_ARGS(
		 "libevt_file_open",
		 evt_test_file_open,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		EVT_TEST_RUN_WITH_ARGS(
		 "libevt_file_open_wide",
		 evt_test_file_open_wide,
		 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		EVT_TEST_RUN_WITH_ARGS(
		 "libevt_file_open_file_io_handle",
		 evt_test_file_open_file_io_handle,
		 source );

		EVT_TEST_RUN(
		 "libevt_file_close",
		 evt_test_file_close );

		EVT_TEST_RUN_WITH_ARGS(
		 "libevt_file_open_close",
		 evt_test_file_open_close,
		 source );

		/* Initialize file for tests
		 */
		result = evt_test_file_open_source(
		          &file,
		          file_io_handle,
		          &error );

		EVT_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EVT_TEST_ASSERT_IS_NOT_NULL(
		 "file",
		 file );

		EVT_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		EVT_TEST_RUN_WITH_ARGS(
		 "libevt_file_signal_abort",
		 evt_test_file_signal_abort,
		 file );

#if defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT )

		/* TODO: add tests for libevt_file_open_read */

#endif /* defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT ) */

		/* TODO: add tests for libevt_file_is_corrupted */

		EVT_TEST_RUN_WITH_ARGS(
		 "libevt_file_get_ascii_codepage",
		 evt_test_file_get_ascii_codepage,
		 file );

		EVT_TEST_RUN_WITH_ARGS(
		 "libevt_file_set_ascii_codepage",
		 evt_test_file_set_ascii_codepage,
		 file );

		/* TODO: add tests for libevt_file_get_format_version */

		/* TODO: add tests for libevt_file_get_version */

		EVT_TEST_RUN_WITH_ARGS(
		 "libevt_file_get_flags",
		 evt_test_file_get_flags,
		 file );

		EVT_TEST_RUN_WITH_ARGS(
		 "libevt_file_get_number_of_records",
		 evt_test_file_get_number_of_records,
		 file );

		EVT_TEST_RUN_WITH_ARGS(
		 "libevt_file_get_record",
		 evt_test_file_get_record,
		 file );

		EVT_TEST_RUN_WITH_ARGS(
		 "libevt_file_get_number_of_recovered_records",
		 evt_test_file_get_number_of_recovered_records,
		 file );

		EVT_TEST_RUN_WITH_ARGS(
		 "libevt_file_get_recovered_record",
		 evt_test_file_get_recovered_record,
		 file );

		/* Clean up
		 */
		result = evt_test_file_close_source(
		          &file,
		          &error );

		EVT_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		EVT_TEST_ASSERT_IS_NULL(
		 "file",
		 file );

		EVT_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libbfio_handle_free(
		          &file_io_handle,
		          &error );

		EVT_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EVT_TEST_ASSERT_IS_NULL(
	         "file_io_handle",
	         file_io_handle );

	        EVT_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file != NULL )
	{
		libevt_file_free(
		 &file,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

