/*
 * Library to access the Windows Event Log (EVT) format
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

#if !defined( _LIBEVT_H )
#define _LIBEVT_H

#include <libevt/codepage.h>
#include <libevt/definitions.h>
#include <libevt/error.h>
#include <libevt/extern.h>
#include <libevt/features.h>
#include <libevt/types.h>

#include <stdio.h>

#if defined( LIBEVT_HAVE_BFIO )
#include <libbfio.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBEVT_EXTERN \
const char *libevt_get_version(
             void );

/* Returns the access flags for reading
 */
LIBEVT_EXTERN \
int libevt_get_access_flags_read(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_get_codepage(
     int *codepage,
     libevt_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_set_codepage(
     int codepage,
     libevt_error_t **error );

/* Determines if a file contains an EVT file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEVT_EXTERN \
int libevt_check_file_signature(
     const char *filename,
     libevt_error_t **error );

#if defined( LIBEVT_HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file contains an EVT file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEVT_EXTERN \
int libevt_check_file_signature_wide(
     const wchar_t *filename,
     libevt_error_t **error );

#endif /* defined( LIBEVT_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBEVT_HAVE_BFIO )

/* Determines if a file contains an EVT file signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEVT_EXTERN \
int libevt_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libevt_error_t **error );

#endif /* defined( LIBEVT_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBEVT_EXTERN \
void libevt_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_notify_set_stream(
     FILE *stream,
     libevt_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_notify_stream_open(
     const char *filename,
     libevt_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_notify_stream_close(
     libevt_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBEVT_EXTERN \
void libevt_error_free(
      libevt_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_error_fprint(
     libevt_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_error_sprint(
     libevt_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_error_backtrace_fprint(
     libevt_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_error_backtrace_sprint(
     libevt_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * File functions
 * ------------------------------------------------------------------------- */

/* Creates a file
 * Make sure the value file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_initialize(
     libevt_file_t **file,
     libevt_error_t **error );

/* Frees a file
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_free(
     libevt_file_t **file,
     libevt_error_t **error );

/* Signals the libevt file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_signal_abort(
     libevt_file_t *file,
     libevt_error_t **error );

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_open(
     libevt_file_t *file,
     const char *filename,
     int access_flags,
     libevt_error_t **error );

#if defined( LIBEVT_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_open_wide(
     libevt_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libevt_error_t **error );

#endif /* defined( LIBEVT_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBEVT_HAVE_BFIO )

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_open_file_io_handle(
     libevt_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libevt_error_t **error );

#endif /* defined( LIBEVT_HAVE_BFIO ) */

/* Closes a file
 * Returns 0 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_close(
     libevt_file_t *file,
     libevt_error_t **error );

/* Determine if the file corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_is_corrupted(
     libevt_file_t *file,
     libevt_error_t **error );

/* Retrieves the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_get_ascii_codepage(
     libevt_file_t *file,
     int *ascii_codepage,
     libevt_error_t **error );

/* Sets the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_set_ascii_codepage(
     libevt_file_t *file,
     int ascii_codepage,
     libevt_error_t **error );

/* Retrieves the format version
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_get_format_version(
     libevt_file_t *file,
     uint32_t *major_version,
     uint32_t *minor_version,
     libevt_error_t **error );

/* Retrieves the flags
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_get_flags(
     libevt_file_t *file,
     uint32_t *flags,
     libevt_error_t **error );

/* Retrieves the number of records
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_get_number_of_records(
     libevt_file_t *file,
     int *number_of_records,
     libevt_error_t **error );

/* Retrieves a specific record
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_get_record(
     libevt_file_t *file,
     int record_index,
     libevt_record_t **record,
     libevt_error_t **error );

/* Retrieves the number of recovered records
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_get_number_of_recovered_records(
     libevt_file_t *file,
     int *number_of_records,
     libevt_error_t **error );

/* Retrieves a specific recovered record
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_file_get_recovered_record(
     libevt_file_t *file,
     int record_index,
     libevt_record_t **record,
     libevt_error_t **error );

/* -------------------------------------------------------------------------
 * File functions - deprecated
 * ------------------------------------------------------------------------- */

/* Retrieves the version
 *
 * This function deprecated use libevt_file_get_format_version instead
 *
 * Returns 1 if successful or -1 on error
 */
LIBEVT_DEPRECATED \
LIBEVT_EXTERN \
int libevt_file_get_version(
     libevt_file_t *file,
     uint32_t *major_version,
     uint32_t *minor_version,
     libevt_error_t **error );

/* -------------------------------------------------------------------------
 * Record functions
 * ------------------------------------------------------------------------- */

/* Frees a record
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_free(
     libevt_record_t **record,
     libevt_error_t **error );

/* Retrieves the offset
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_offset(
     libevt_record_t *record,
     off64_t *offset,
     libevt_error_t **error );

/* Retrieves the identifier (record number)
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_identifier(
     libevt_record_t *record,
     uint32_t *identifier,
     libevt_error_t **error );

/* Retrieves the creation time
 * The timestamp is a 32-bit POSIX date and time value
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_creation_time(
     libevt_record_t *record,
     uint32_t *posix_time,
     libevt_error_t **error );

/* Retrieves the written time
 * The timestamp is a 32-bit POSIX date and time value
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_written_time(
     libevt_record_t *record,
     uint32_t *posix_time,
     libevt_error_t **error );

/* Retrieves the event identifier
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_event_identifier(
     libevt_record_t *record,
     uint32_t *event_identifier,
     libevt_error_t **error );

/* Retrieves the event type
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_event_type(
     libevt_record_t *record,
     uint16_t *event_type,
     libevt_error_t **error );

/* Retrieves the event category
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_event_category(
     libevt_record_t *record,
     uint16_t *event_category,
     libevt_error_t **error );

/* Retrieves the size of the UTF-8 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf8_source_name_size(
     libevt_record_t *record,
     size_t *utf8_string_size,
     libevt_error_t **error );

/* Retrieves the UTF-8 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf8_source_name(
     libevt_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libevt_error_t **error );

/* Retrieves the size of the UTF-16 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf16_source_name_size(
     libevt_record_t *record,
     size_t *utf16_string_size,
     libevt_error_t **error );

/* Retrieves the UTF-16 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf16_source_name(
     libevt_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libevt_error_t **error );

/* Retrieves the size of the UTF-8 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf8_computer_name_size(
     libevt_record_t *record,
     size_t *utf8_string_size,
     libevt_error_t **error );

/* Retrieves the UTF-8 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf8_computer_name(
     libevt_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libevt_error_t **error );

/* Retrieves the size of the UTF-16 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf16_computer_name_size(
     libevt_record_t *record,
     size_t *utf16_string_size,
     libevt_error_t **error );

/* Retrieves the UTF-16 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf16_computer_name(
     libevt_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libevt_error_t **error );

/* Retrieves the size of the UTF-8 encoded user security identifier (SID)
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf8_user_security_identifier_size(
     libevt_record_t *record,
     size_t *utf8_string_size,
     libevt_error_t **error );

/* Retrieves the UTF-8 encoded user security identifier (SID)
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf8_user_security_identifier(
     libevt_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libevt_error_t **error );

/* Retrieves the size of the UTF-16 encoded user security identifier (SID)
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf16_user_security_identifier_size(
     libevt_record_t *record,
     size_t *utf16_string_size,
     libevt_error_t **error );

/* Retrieves the UTF-16 encoded user security identifier (SID)
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf16_user_security_identifier(
     libevt_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libevt_error_t **error );

/* Retrieves the number of strings
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_number_of_strings(
     libevt_record_t *record,
     int *number_of_strings,
     libevt_error_t **error );

/* Retrieves the size of a specific UTF-8 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf8_string_size(
     libevt_record_t *record,
     int string_index,
     size_t *utf8_string_size,
     libevt_error_t **error );

/* Retrieves a specific UTF-8 encoded string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf8_string(
     libevt_record_t *record,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libevt_error_t **error );

/* Retrieves the size of a specific UTF-16 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf16_string_size(
     libevt_record_t *record,
     int string_index,
     size_t *utf16_string_size,
     libevt_error_t **error );

/* Retrieves a specific UTF-16 encoded string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_utf16_string(
     libevt_record_t *record,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libevt_error_t **error );

/* Retrieves the size of the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_data_size(
     libevt_record_t *record,
     size_t *data_size,
     libevt_error_t **error );

/* Retrieves the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVT_EXTERN \
int libevt_record_get_data(
     libevt_record_t *record,
     uint8_t *data,
     size_t data_size,
     libevt_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVT_H ) */

