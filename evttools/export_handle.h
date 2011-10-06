/* 
 * Export handle
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

#if !defined( _EXPORT_HANDLE_H )
#define _EXPORT_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#include "log_handle.h"
#include "evttools_libevt.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct export_handle export_handle_t;

struct export_handle
{
	/* Value to indicate item values should be dumped
	 */
	uint8_t dump_item_values;

	/* The libevt input file
	 */
	libevt_file_t *input_file;

	/* The ascii codepage
	 */
	int ascii_codepage;

	/* The target path
	 */
	libcstring_system_character_t *target_path;

	/* The target path size
	 */
	size_t target_path_size;

	/* The items export path
	 */
	libcstring_system_character_t *items_export_path;

	/* The items export path size
	 */
	size_t items_export_path_size;

	/* Value to indicate if status information
	 * should be printed to the notify stream
	 */
	uint8_t print_status_information;

	/* The nofication output stream
	 */
	FILE *notify_stream;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int export_handle_initialize(
     export_handle_t **export_handle,
     liberror_error_t **error );

int export_handle_free(
     export_handle_t **export_handle,
     liberror_error_t **error );

int export_handle_signal_abort(
     export_handle_t *export_handle,
     liberror_error_t **error );

int export_handle_set_export_mode(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     liberror_error_t **error );

int export_handle_set_ascii_codepage(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     liberror_error_t **error );

int export_handle_set_target_path(
     export_handle_t *export_handle,
     const libcstring_system_character_t *target_path,
     liberror_error_t **error );

int export_handle_set_export_path(
     export_handle_t *export_handle,
     const libcstring_system_character_t *base_path,
     size_t base_path_length,
     const libcstring_system_character_t *suffix,
     size_t suffix_length,
     libcstring_system_character_t **export_path,
     size_t *export_path_size,
     liberror_error_t **error );

int export_handle_create_items_export_path(
     export_handle_t *export_handle,
     liberror_error_t **error );

int export_handle_make_directory(
     export_handle_t *export_handle,
     const libcstring_system_character_t *directory_name,
     log_handle_t *log_handle,
     liberror_error_t **error );

int export_handle_print_data(
     export_handle_t *export_handle,
     FILE *stream,
     const uint8_t *data,
     size_t data_size,
     liberror_error_t **error );

int export_handle_open(
     export_handle_t *export_handle,
     const libcstring_system_character_t *filename,
     liberror_error_t **error );

int export_handle_close(
     export_handle_t *export_handle,
     liberror_error_t **error );

/* Item generic export functions
 */
int export_handle_create_default_item_directory(
     export_handle_t *export_handle,
     int item_index,
     const libcstring_system_character_t *item_prefix,
     size_t item_prefix_length,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     libcstring_system_character_t **item_directory_path,
     size_t *item_directory_path_size,
     log_handle_t *log_handle,
     liberror_error_t **error );

int export_handle_create_text_item_file(
     export_handle_t *export_handle,
     const libcstring_system_character_t *item_filename,
     size_t item_filename_length,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     FILE **item_file_stream,
     liberror_error_t **error );

int export_handle_export_item_values(
     export_handle_t *export_handle,
     libevt_item_t *item,
     const libcstring_system_character_t *item_values_filename,
     size_t item_values_filename_length,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     liberror_error_t **error );

/* Item specific export functions
 */
int export_handle_export_alias(
     export_handle_t *export_handle,
     libevt_item_t *alias,
     int alias_index,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     liberror_error_t **error );

/* File export functions
 */
int export_handle_export_items(
     export_handle_t *export_handle,
     libevt_file_t *file,
     log_handle_t *log_handle,
     liberror_error_t **error );

int export_handle_export_file(
     export_handle_t *export_handle,
     log_handle_t *log_handle,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

