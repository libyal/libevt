/*
 * Extracts items from a Windows Event Log (EVT) file.
 *
 * Copyright (C) 2011-2026, Joachim Metz <joachim.metz@gmail.com>
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
#include <system_string.h>
#include <types.h>

#if defined( HAVE_FCNTL_H ) || defined( WINAPI )
#include <fcntl.h>
#endif

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#include "evtinput.h"
#include "evttools_getopt.h"
#include "evttools_libcerror.h"
#include "evttools_libclocale.h"
#include "evttools_libcnotify.h"
#include "evttools_libevt.h"
#include "evttools_signal.h"
#include "evttools_output.h"
#include "evttools_unused.h"
#include "export_handle.h"
#include "log_handle.h"

export_handle_t *evtexport_export_handle = NULL;
int evtexport_abort                      = 0;

/* Signal handler for evtexport
 */
void evtexport_signal_handler(
      evttools_signal_t signal EVTTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "evtexport_signal_handler";

	EVTTOOLS_UNREFERENCED_PARAMETER( signal )

	evtexport_abort = 1;

	if( evtexport_export_handle != NULL )
	{
		if( export_handle_signal_abort(
		     evtexport_export_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal export handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	const char *description = \
		"Use evtexport to export items stored in a Windows Event Log (EVT) file.";

	evttools_option_t options[ ] = {
		{ 'c', "codepage", "codepage of ASCII strings, options: ascii, windows-874, windows-932, windows-936, windows-949, windows-950, windows-1250, windows-1251, windows-1252 (default), windows-1253, windows-1254, windows-1255, windows-1256, windows-1257 or windows-1258" },
		{ 'h', NULL, "shows this help" },
		{ 'l', "log_file", "logs information about the exported items" },
		{ 'm', "mode", "export mode, option: all, items (default), recovered. 'all' exports the (allocated) items and recovered items, 'items' exports the (allocated) items and 'recovered' exports the recovered items" },
		{ 'p', "resource_files_path", "search PATH for the resource files (default is the current working directory)" },
		{ 'r', "registy_files_path", "name of the directory containing the SOFTWARE and SYSTEM (Windows) Registry file" },
		{ 's', "system_file", "filename of the SYSTEM (Windows) Registry file. This option overrides the path provided by -r" },
		{ 'S', "software_file", "filename of the SOFTWARE (Windows) Registry file. This option overrides the path provided by -r" },
		{ 't', "event_log_type", "event log type, options: application, security, system. if not specified the event log type is determined based on the filename." },
		{ 'v', NULL, "verbose output to stderr" },
		{ 'V', NULL, "print version" },
		{ 0, "source", "the source file" },
	};
	system_character_t options_string[ 32 ];

	libcerror_error_t *error                              = NULL;
	log_handle_t *log_handle                              = NULL;
	system_character_t *option_ascii_codepage             = NULL;
	system_character_t *option_event_log_type             = NULL;
	system_character_t *option_export_mode                = NULL;
	system_character_t *option_log_filename               = NULL;
	system_character_t *option_resource_files_path        = NULL;
	system_character_t *option_preferred_language         = NULL;
	system_character_t *option_registry_directory_name    = NULL;
	system_character_t *option_software_registry_filename = NULL;
	system_character_t *option_system_registry_filename   = NULL;
	system_character_t *source                            = NULL;
	char *program                                         = "evtexport";
	system_integer_t option                               = 0;
	int number_of_options                                 = (int) ( sizeof( options ) / sizeof( evttools_option_t ) );
	int result                                            = 0;
	int verbose                                           = 0;

#if defined( __MINGW32__ ) && defined( HAVE_MINGW_BINMODE )
	_setmode( _fileno( stdout ), _O_BINARY );
	_setmode( _fileno( stderr ), _O_BINARY );
#endif

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
	     "evttools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
	if( evttools_output_initialize(
	     _IONBF,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	evttools_output_version_fprint(
	 stdout,
	 program );

	if( evttools_getopt_get_options_string(
	     options,
	     number_of_options,
	     options_string,
	     32 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to determine options string.\n" );

		goto on_error;
	}
	while( ( option = evttools_getopt(
	                   argc,
	                   argv,
	                   options_string ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				evttools_getopt_usage_fprint(
				 stdout,
				 program,
				 description,
				 options,
				 number_of_options );

				return( EXIT_FAILURE );

			case (system_integer_t) 'c':
				option_ascii_codepage = optarg;

				break;

			case (system_integer_t) 'h':
				evttools_getopt_usage_fprint(
				 stdout,
				 program,
				 description,
				 options,
				 number_of_options );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'l':
				option_log_filename = optarg;

				break;

			case (system_integer_t) 'm':
				option_export_mode = optarg;

				break;

			case (system_integer_t) 'p':
				option_resource_files_path = optarg;

				break;

			case (system_integer_t) 'r':
				option_registry_directory_name = optarg;

				break;

			case (system_integer_t) 's':
				option_system_registry_filename = optarg;

				break;

			case (system_integer_t) 'S':
				option_software_registry_filename = optarg;

				break;

			case (system_integer_t) 't':
				option_event_log_type = optarg;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				evttools_output_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file.\n" );

		evttools_getopt_usage_fprint(
		 stdout,
		 program,
		 description,
		 options,
		 number_of_options );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

	libcnotify_verbose_set(
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
	if( option_event_log_type != NULL )
	{
		result = export_handle_set_event_log_type(
		          evtexport_export_handle,
		          option_event_log_type,
		          &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set event log type in export handle.\n" );

			goto on_error;
		}
	}
	if( option_export_mode != NULL )
	{
		result = export_handle_set_export_mode(
			  evtexport_export_handle,
			  option_export_mode,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set export mode.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported export mode defaulting to: items.\n" );
		}
	}
	if( ( option_event_log_type == NULL )
	 || ( result == 0 ) )
	{
		result = export_handle_set_event_log_type_from_filename(
			  evtexport_export_handle,
			  source,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set event log type from filename in export handle.\n" );

			goto on_error;
		}
	}
	if( option_resource_files_path != NULL )
	{
		if( export_handle_set_resource_files_path(
		     evtexport_export_handle,
		     option_resource_files_path,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set resource files path in export handle.\n" );

			goto on_error;
		}
	}
	if( option_software_registry_filename != NULL )
	{
		if( export_handle_set_software_registry_filename(
		     evtexport_export_handle,
		     option_software_registry_filename,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set software registry filename in export handle.\n" );

			goto on_error;
		}
	}
	if( option_system_registry_filename != NULL )
	{
		if( export_handle_set_system_registry_filename(
		     evtexport_export_handle,
		     option_system_registry_filename,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set system registry filename in export handle.\n" );

			goto on_error;
		}
	}
	if( option_registry_directory_name != NULL )
	{
		if( export_handle_set_registry_directory_name(
		     evtexport_export_handle,
		     option_registry_directory_name,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set registry directory name in export handle.\n" );

			goto on_error;
		}
	}
	if( option_preferred_language != NULL )
	{
/* TODO set preferred language identifier from input */
		if( export_handle_set_preferred_language_identifier(
		     evtexport_export_handle,
		     0x0409,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set preferred language identifier in export handle.\n" );

			goto on_error;
		}
	}
	if( log_handle_open(
	     log_handle,
	     option_log_filename,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open log file: %" PRIs_SYSTEM ".\n",
		 option_log_filename );

		goto on_error;
	}
	if( export_handle_open_input(
	     evtexport_export_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	result = export_handle_export_file(
	          evtexport_export_handle,
	          log_handle,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to export file.\n" );

		goto on_error;
	}
	if( export_handle_close_input(
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
		 "No records to export.\n" );
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
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

