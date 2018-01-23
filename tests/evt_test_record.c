/*
 * Library record type test program
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

#include "../libevt/libevt_record.h"

/* Tests the libevt_record_free function
 * Returns 1 if successful or 0 if not
 */
int evt_test_record_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libevt_record_free(
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

#if defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT )

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

	/* TODO: add tests for libevt_record_initialize */

#endif /* defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT ) */

	EVT_TEST_RUN(
	 "libevt_record_free",
	 evt_test_record_free );

#if defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT )

	/* TODO: add tests for libevt_record_get_offset */

	/* TODO: add tests for libevt_record_get_identifier */

	/* TODO: add tests for libevt_record_get_creation_time */

	/* TODO: add tests for libevt_record_get_written_time */

	/* TODO: add tests for libevt_record_get_event_identifier */

	/* TODO: add tests for libevt_record_get_event_type */

	/* TODO: add tests for libevt_record_get_event_category */

	/* TODO: add tests for libevt_record_get_utf8_source_name_size */

	/* TODO: add tests for libevt_record_get_utf8_source_name */

	/* TODO: add tests for libevt_record_get_utf16_source_name_size */

	/* TODO: add tests for libevt_record_get_utf16_source_name */

	/* TODO: add tests for libevt_record_get_utf8_computer_name_size */

	/* TODO: add tests for libevt_record_get_utf8_computer_name */

	/* TODO: add tests for libevt_record_get_utf16_computer_name_size */

	/* TODO: add tests for libevt_record_get_utf16_computer_name */

	/* TODO: add tests for libevt_record_get_utf8_user_security_identifier_size */

	/* TODO: add tests for libevt_record_get_utf8_user_security_identifier */

	/* TODO: add tests for libevt_record_get_utf16_user_security_identifier_size */

	/* TODO: add tests for libevt_record_get_utf16_user_security_identifier */

	/* TODO: add tests for libevt_record_get_number_of_strings */

	/* TODO: add tests for libevt_record_get_utf8_string_size */

	/* TODO: add tests for libevt_record_get_utf8_string */

	/* TODO: add tests for libevt_record_get_utf16_string_size */

	/* TODO: add tests for libevt_record_get_utf16_string */

	/* TODO: add tests for libevt_record_get_data_size */

	/* TODO: add tests for libevt_record_get_data */

#endif /* defined( __GNUC__ ) && !defined( LIBEVT_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

