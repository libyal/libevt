/*
 * Python object definition of the strings sequence and iterator
 *
 * Copyright (C) 2011-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYEVT_STRINGS_H )
#define _PYEVT_STRINGS_H

#include <common.h>
#include <types.h>

#include "pyevt_libevt.h"
#include "pyevt_python.h"
#include "pyevt_record.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyevt_strings pyevt_strings_t;

struct pyevt_strings
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The record object
	 */
	pyevt_record_t *record_object;

	/* The get string by index callback function
	 */
	PyObject* (*get_string_by_index)(
	             pyevt_record_t *record_object,
	             int string_index );

	/* The (current) string index
	 */
	int string_index;

	/* The number of strings
	 */
	int number_of_strings;
};

extern PyTypeObject pyevt_strings_type_object;

PyObject *pyevt_strings_new(
           pyevt_record_t *record_object,
           PyObject* (*get_string_by_index)(
                        pyevt_record_t *record_object,
                        int string_index ),
           int number_of_strings );

int pyevt_strings_init(
     pyevt_strings_t *pyevt_strings );

void pyevt_strings_free(
      pyevt_strings_t *pyevt_strings );

Py_ssize_t pyevt_strings_len(
            pyevt_strings_t *pyevt_strings );

PyObject *pyevt_strings_getitem(
           pyevt_strings_t *pyevt_strings,
           Py_ssize_t item_index );

PyObject *pyevt_strings_iter(
           pyevt_strings_t *pyevt_strings );

PyObject *pyevt_strings_iternext(
           pyevt_strings_t *pyevt_strings );

#if defined( __cplusplus )
}
#endif

#endif

