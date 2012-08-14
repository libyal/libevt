/*
 * Python object definition of the libevt file
 *
 * Copyright (c) 2011-2012, Joachim Metz <joachim.metz@gmail.com>
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
};

extern PyMethodDef pyevt_file_object_methods[];
extern PyTypeObject pyevt_file_type_object;

PyObject *pyevt_file_new(
           PyObject *self );

PyObject *pyevt_file_new_open(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

int pyevt_file_init(
     pyevt_file_t *pyevt_file );

void pyevt_file_free(
      pyevt_file_t *pyevt_file );

PyObject *pyevt_file_signal_abort(
           pyevt_file_t *pyevt_file );

PyObject *pyevt_file_open(
           pyevt_file_t *pyevt_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyevt_file_open_file_object(
           pyevt_file_t *pyevt_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyevt_file_close(
           pyevt_file_t *pyevt_file );

PyObject *pyevt_file_get_ascii_codepage(
           pyevt_file_t *pyevt_file );

PyObject *pyevt_file_set_ascii_codepage(
           pyevt_file_t *pyevt_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyevt_file_get_number_of_records(
           pyevt_file_t *pyevt_file );

PyObject *pyevt_key_get_record(
           pyevt_file_t *pyevt_file,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif

