/*
 * Python object wrapper of libevt_file_t
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

#if !defined( _PYEVT_FILE_H )
#define _PYEVT_FILE_H

#include <common.h>
#include <types.h>

#include "pyevt_libbfio.h"
#include "pyevt_libevt.h"
#include "pyevt_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyevt_file pyevt_file_t;

struct pyevt_file
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libevt file
	 */
	libevt_file_t *file;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

extern PyMethodDef pyevt_file_object_methods[];
extern PyTypeObject pyevt_file_type_object;

PyObject *pyevt_file_new(
           void );

PyObject *pyevt_file_new_open(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyevt_file_new_open_file_object(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

int pyevt_file_init(
     pyevt_file_t *pyevt_file );

void pyevt_file_free(
      pyevt_file_t *pyevt_file );

PyObject *pyevt_file_signal_abort(
           pyevt_file_t *pyevt_file,
           PyObject *arguments );

PyObject *pyevt_file_open(
           pyevt_file_t *pyevt_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyevt_file_open_file_object(
           pyevt_file_t *pyevt_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyevt_file_close(
           pyevt_file_t *pyevt_file,
           PyObject *arguments );

PyObject *pyevt_file_is_corrupted(
           pyevt_file_t *pyevt_file,
           PyObject *arguments );

PyObject *pyevt_file_get_ascii_codepage(
           pyevt_file_t *pyevt_file,
           PyObject *arguments );

int pyevt_file_set_ascii_codepage_from_string(
     pyevt_file_t *pyevt_file,
     const char *codepage_string );

PyObject *pyevt_file_set_ascii_codepage(
           pyevt_file_t *pyevt_file,
           PyObject *arguments,
           PyObject *keywords );

int pyevt_file_set_ascii_codepage_setter(
     pyevt_file_t *pyevt_file,
     PyObject *string_object,
     void *closure );

PyObject *pyevt_file_get_format_version(
           pyevt_file_t *pyevt_file,
           PyObject *arguments );

PyObject *pyevt_file_get_number_of_records(
           pyevt_file_t *pyevt_file,
           PyObject *arguments );

PyObject *pyevt_file_get_record_by_index(
           PyObject *pyevt_file,
           int record_index );

PyObject *pyevt_file_get_record(
           pyevt_file_t *pyevt_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyevt_file_get_records(
           pyevt_file_t *pyevt_file,
           PyObject *arguments );

PyObject *pyevt_file_get_number_of_recovered_records(
           pyevt_file_t *pyevt_file,
           PyObject *arguments );

PyObject *pyevt_file_get_recovered_record_by_index(
           PyObject *pyevt_file,
           int record_index );

PyObject *pyevt_file_get_recovered_record(
           pyevt_file_t *pyevt_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyevt_file_get_recovered_records(
           pyevt_file_t *pyevt_file,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYEVT_FILE_H ) */

