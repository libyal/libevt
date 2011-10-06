/*
 * Extracts items from a Nickfile (EVT)
 *
 * Copyright (c) 2011, Joachim Metz <jbmetz@users.sourceforge.net>
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
#include <memory.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <libsystem.h>

#include "export_handle.h"
#include "log_handle.h"
#include "evtinput.h"
#include "evtoutput.h"
#include "evttools_libevt.h"

export_handle_t *evtexport_export_handle = NULL;
int evtexport_abort                      = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use evtexport to export items stored in a Nickfile (EVT).\n\n" );

	fprintf( stream, "Usage: evtexport [ -c codepage ] [ -l logfile ] [ -t target ] [ -hvV ]\n"
	                 "                 source\n\n" );

	fprintf( stream, "\tsource: the source file\n\n" );

	fprintf( stream, "\t-c:     codepage of ASCII strings, options: ascii, windows-874,\n"
	                 "\t        windows-1250, windows-1251, windows-1252 (default),\n"
	                 "\t        windows-1253, windows-1254, windows-1255, windows-1256,\n"
	                 "\t        windows-1257 or windows-1258\n" );
	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-l:     logs information about the exported items\n" );
	fprintf( stream, "\t-q:     quiet shows minimal status information\n" );
	fprintf( stream, "\t-t:     specify the target directory to export to\n"
	                 "\t        (default is the source filename followed by .export)\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Signal handler for evtexport
 */
void evtexport_signal_handler(
      libsystem_signal_t signal )
{
	liberror_error_t *error = NULL;
	static char *function   = "evtexport_signal_handler";

	evtexport_abort = 1;

	if( evtexport_export_handle != NULL )
	{
		if( export_handle_signal_abort(
		     evtexport_export_handle,
		     &error ) != 1 )
		{
			libsystem_notify_printf(
			 "%s: unable to signal export handle to abort.\n",
			 function );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
	if( libsystem_file_io_close(
	     0 ) != 0 )
	{
		libsystem_notify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	export_handle_t *export_handle                       = NULL;
	liberror_error_t *error                              = NULL;
	log_handle_t *log_handle                             = NULL;
	libcstring_system_character_t *log_filename          = NULL;
	libcstring_system_character_t *option_ascii_codepage = NULL;
	libcstring_system_character_t *option_target_path    = NULL;
	libcstring_system_character_t *path_separator        = NULL;
	libcstring_system_character_t *source                = NULL;
	char *program                                        = "evtexport";
	size_t source_length                                 = 0;
	libcstring_system_integer_t option                   = 0;
	uint8_t print_status_information                     = 1;
	int result                                           = 0;
	int verbose                                          = 0;

	libsystem_notify_set_stream(
	 stderr,
	 NULL );
	libsystem_notify_set_verbose(
	 1 );

	if( libsystem_initialize(
	     "evttools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	evtoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = libsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "c:hl:qt:vV" ) ) ) != (libcstring_system_integer_t) -1 )
	{
		switch( option )
		{
			case (libcstring_system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libcstring_system_integer_t) 'c':
				option_ascii_codepage = optarg;

				break;

			case (libcstring_system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (libcstring_system_integer_t) 'q':
				print_status_information = 0;

				break;

			case (libcstring_system_integer_t) 't':
				option_target_path = optarg;

				break;

			case (libcstring_system_integer_t) 'v':
				verbose = 1;

				break;

			case (libcstring_system_integer_t) 'V':
				evtoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

	if( option_target_path == NULL )
	{
		source_length = libcstring_system_string_length(
		                 source );

		path_separator = libcstring_system_string_search_character_reverse(
		                  source,
		                  (libcstring_system_character_t) LIBSYSTEM_PATH_SEPARATOR,
		                  source_length );

		if( path_separator == NULL )
		{
			path_separator = source;
		}
		else
		{
			path_separator++;
		}
		option_target_path = path_separator;
	}
	libsystem_notify_set_verbose(
	 verbose );
	libevt_notify_set_stream(
	 stderr,
	 NULL );
	libevt_notify_set_verbose(
	 verbose );

	if( log_handle_initialize(
	     &log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize log handle.\n" );

		goto on_error;
	}
	if( export_handle_initialize(
	     &evtexport_export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize export handle.\n" );

		goto on_error;
	}
	evtexport_export_handle->print_status_information = print_status_information;

	if( option_ascii_codepage != NULL )
	{
		result = export_handle_set_ascii_codepage(
		          evtexport_export_handle,
		          option_ascii_codepage,
		          &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set ASCII codepage in export handle.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported ASCII codepage defaulting to: windows-1252.\n" );
		}
	}
	if( export_handle_set_target_path(
	     evtexport_export_handle,
	     option_target_path,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set target path.\n" );

		goto on_error;
	}
	result = export_handle_create_items_export_path(
	          evtexport_export_handle,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to create items export path.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		fprintf(
		 stderr,
		 "%" PRIs_LIBCSTRING_SYSTEM " already exists.\n",
		 evtexport_export_handle->items_export_path );

		goto on_error;
	}
	if( log_handle_open(
	     log_handle,
	     log_filename,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open log file: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 log_filename );

		goto on_error;
	}
	fprintf(
	 stdout,
	 "Opening file.\n" );

	if( export_handle_open(
	     evtexport_export_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	fprintf(
	 stdout,
	 "Exporting aliases.\n" );

	result = export_handle_export_file(
	          export_handle,
	          log_handle,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to export file.\n" );

		goto on_error;
	}
	if( export_handle_close(
	     evtexport_export_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close export handle.\n" );

		goto on_error;
	}
	if( export_handle_free(
	     &evtexport_export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free export handle.\n" );

		goto on_error;
	}
	if( log_handle_close(
	     log_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close log handle.\n" );

		goto on_error;
	}
	if( log_handle_free(
	     &log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free log handle.\n" );

		goto on_error;
	}
	if( result == 0 )
	{
		fprintf(
		 stdout,
		 "No aliases to export.\n" );
	}
	else
	{
		fprintf(
		 stdout,
		 "Export completed.\n" );
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
	if( evtexport_export_handle != NULL )
	{
		export_handle_free(
		 &evtexport_export_handle,
		 NULL );
	}
	if( log_handle != NULL )
	{
		log_handle_free(
		 &log_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

