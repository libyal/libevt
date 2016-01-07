/*
 * Python object definition of the records sequence and iterator
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

#if !defined( _PYEVT_RECORDS_H )
#define _PYEVT_RECORDS_H

#include <common.h>
#include <types.h>

#include "pyevt_file.h"
#include "pyevt_libevt.h"
#include "pyevt_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyevt_records pyevt_records_t;

struct pyevt_records
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The file object
	 */
	pyevt_file_t *file_object;

	/* The get record by index callback function
	 */
	PyObject* (*get_record_by_index)(
	             pyevt_file_t *file_object,
	             int record_index );

	/* The (current) record index
	 */
	int record_index;

	/* The number of records
	 */
	int number_of_records;
};

extern PyTypeObject pyevt_records_type_object;

PyObject *pyevt_records_new(
           pyevt_file_t *file_object,
           PyObject* (*get_record_by_index)(
                        pyevt_file_t *file_object,
                        int record_index ),
           int number_of_records );

int pyevt_records_init(
     pyevt_records_t *pyevt_records );

void pyevt_records_free(
      pyevt_records_t *pyevt_records );

Py_ssize_t pyevt_records_len(
            pyevt_records_t *pyevt_records );

PyObject *pyevt_records_getitem(
           pyevt_records_t *pyevt_records,
           Py_ssize_t item_index );

PyObject *pyevt_records_iter(
           pyevt_records_t *pyevt_records );

PyObject *pyevt_records_iternext(
           pyevt_records_t *pyevt_records );

#if defined( __cplusplus )
}
#endif

#endif

