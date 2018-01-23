/*
 * Common input functions for the evttools
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "evtinput.h"
#include "evttools_libcerror.h"
#include "evttools_libcpath.h"
#include "evttools_libevt.h"
#include "evttools_wide_string.h"

/* Determines the event log type from a string
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int evtinput_determine_event_log_type(
     const system_character_t *string,
     int *event_log_type,
     libcerror_error_t **error )
{
	static char *function = "evtinput_determine_event_log_type";
	size_t string_length  = 0;
	int result            = 0;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( event_log_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event log type.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( string_length == 6 )
	{
		if( system_string_compare_no_case(
		     string,
		     _SYSTEM_STRING( "system" ),
		     6 ) == 0 )
		{
			*event_log_type = EVTTOOLS_EVENT_LOG_TYPE_SYSTEM;
			result          = 1;
		}
	}
	else if( string_length == 8 )
	{
		if( system_string_compare_no_case(
		     string,
		     _SYSTEM_STRING( "security" ),
		     8 ) == 0 )
		{
			*event_log_type = EVTTOOLS_EVENT_LOG_TYPE_SECURITY;
			result          = 1;
		}
	}
	else if( string_length == 11 )
	{
		if( system_string_compare_no_case(
		     string,
		     _SYSTEM_STRING( "application" ),
		     11 ) == 0 )
		{
			*event_log_type = EVTTOOLS_EVENT_LOG_TYPE_APPLICATION;
			result          = 1;
		}
	}
	return( result );
}

/* Determines the event log type from the filename
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int evtinput_determine_event_log_type_from_filename(
     const system_character_t *filename,
     int *event_log_type,
     libcerror_error_t **error )
{
	const system_character_t *path_separator = NULL;
	static char *function                    = "evtinput_determine_event_log_type_from_filename";
	size_t filename_length                   = 0;
	int result                               = 0;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( event_log_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event log type.",
		 function );

		return( -1 );
	}
	filename_length = system_string_length(
	                   filename );

	path_separator = system_string_search_character_reverse(
			  filename,
			  (system_character_t) LIBCPATH_SEPARATOR,
			  filename_length );

	if( path_separator == NULL )
	{
		path_separator = filename;
	}
	else
	{
		path_separator++;

		filename_length = system_string_length(
		                   path_separator );
	}
	if( filename_length == 12 )
	{
		if( system_string_compare_no_case(
		     path_separator,
		     _SYSTEM_STRING( "AppEvent.Evt" ),
		     12 ) == 0 )
		{
			*event_log_type = EVTTOOLS_EVENT_LOG_TYPE_APPLICATION;
			result          = 1;
		}
		else if( system_string_compare_no_case(
		          path_separator,
		          _SYSTEM_STRING( "Internet.Evt" ),
		          12 ) == 0 )
		{
			*event_log_type = EVTTOOLS_EVENT_LOG_TYPE_INTERNET_EXPLORER;
			result          = 1;
		}
		else if( system_string_compare_no_case(
		          path_separator,
		          _SYSTEM_STRING( "SecEvent.Evt" ),
		          12 ) == 0 )
		{
			*event_log_type = EVTTOOLS_EVENT_LOG_TYPE_SECURITY;
			result          = 1;
		}
		else if( system_string_compare_no_case(
		          path_separator,
		          _SYSTEM_STRING( "SysEvent.Evt" ),
		          12 ) == 0 )
		{
			*event_log_type = EVTTOOLS_EVENT_LOG_TYPE_SYSTEM;
			result          = 1;
		}
	}
	return( result );
}

