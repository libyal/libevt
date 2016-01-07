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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyevt_file.h"
#include "pyevt_libcerror.h"
#include "pyevt_libevt.h"
#include "pyevt_python.h"
#include "pyevt_record.h"
#include "pyevt_records.h"

PySequenceMethods pyevt_records_sequence_methods = {
	/* sq_length */
	(lenfunc) pyevt_records_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pyevt_records_getitem,
	/* sq_slice */
	0,
	/* sq_ass_item */
	0,
	/* sq_ass_slice */
	0,
	/* sq_contains */
	0,
	/* sq_inplace_concat */
	0,
	/* sq_inplace_repeat */
	0
};

PyTypeObject pyevt_records_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyevt._records",
	/* tp_basicsize */
	sizeof( pyevt_records_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyevt_records_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	&pyevt_records_sequence_methods,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
	/* tp_flags */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER,
	/* tp_doc */
	"internal pyevt records sequence and iterator object",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pyevt_records_iter,
	/* tp_iternext */
	(iternextfunc) pyevt_records_iternext,
	/* tp_methods */
	0,
	/* tp_members */
	0,
	/* tp_getset */
	0,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pyevt_records_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new records object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevt_records_new(
           pyevt_file_t *file_object,
           PyObject* (*get_record_by_index)(
                        pyevt_file_t *file_object,
                        int record_index ),
           int number_of_records )
{
	pyevt_records_t *pyevt_records = NULL;
	static char *function          = "pyevt_records_new";

	if( file_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file object.",
		 function );

		return( NULL );
	}
	if( get_record_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get record by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the records values are initialized
	 */
	pyevt_records = PyObject_New(
	                 struct pyevt_records,
	                 &pyevt_records_type_object );

	if( pyevt_records == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize records.",
		 function );

		goto on_error;
	}
	if( pyevt_records_init(
	     pyevt_records ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize records.",
		 function );

		goto on_error;
	}
	pyevt_records->file_object         = file_object;
	pyevt_records->get_record_by_index = get_record_by_index;
	pyevt_records->number_of_records   = number_of_records;

	Py_IncRef(
	 (PyObject *) pyevt_records->file_object );

	return( (PyObject *) pyevt_records );

on_error:
	if( pyevt_records != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyevt_records );
	}
	return( NULL );
}

/* Intializes a records object
 * Returns 0 if successful or -1 on error
 */
int pyevt_records_init(
     pyevt_records_t *pyevt_records )
{
	static char *function = "pyevt_records_init";

	if( pyevt_records == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid records.",
		 function );

		return( -1 );
	}
	/* Make sure the records values are initialized
	 */
	pyevt_records->file_object         = NULL;
	pyevt_records->get_record_by_index = NULL;
	pyevt_records->record_index        = 0;
	pyevt_records->number_of_records   = 0;

	return( 0 );
}

/* Frees a records object
 */
void pyevt_records_free(
      pyevt_records_t *pyevt_records )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyevt_records_free";

	if( pyevt_records == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid records.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyevt_records );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pyevt_records->file_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyevt_records->file_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyevt_records );
}

/* The records len() function
 */
Py_ssize_t pyevt_records_len(
            pyevt_records_t *pyevt_records )
{
	static char *function = "pyevt_records_len";

	if( pyevt_records == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid records.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) pyevt_records->number_of_records );
}

/* The records getitem() function
 */
PyObject *pyevt_records_getitem(
           pyevt_records_t *pyevt_records,
           Py_ssize_t item_index )
{
	PyObject *record_object = NULL;
	static char *function   = "pyevt_records_getitem";

	if( pyevt_records == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid records.",
		 function );

		return( NULL );
	}
	if( pyevt_records->get_record_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid records - missing get record by index function.",
		 function );

		return( NULL );
	}
	if( pyevt_records->number_of_records < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid records - invalid number of records.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) pyevt_records->number_of_records ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	record_object = pyevt_records->get_record_by_index(
	                 pyevt_records->file_object,
	                 (int) item_index );

	return( record_object );
}

/* The records iter() function
 */
PyObject *pyevt_records_iter(
           pyevt_records_t *pyevt_records )
{
	static char *function = "pyevt_records_iter";

	if( pyevt_records == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid records.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) pyevt_records );

	return( (PyObject *) pyevt_records );
}

/* The records iternext() function
 */
PyObject *pyevt_records_iternext(
           pyevt_records_t *pyevt_records )
{
	PyObject *record_object = NULL;
	static char *function   = "pyevt_records_iternext";

	if( pyevt_records == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid records.",
		 function );

		return( NULL );
	}
	if( pyevt_records->get_record_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid records - missing get record by index function.",
		 function );

		return( NULL );
	}
	if( pyevt_records->record_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid records - invalid record index.",
		 function );

		return( NULL );
	}
	if( pyevt_records->number_of_records < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid records - invalid number of records.",
		 function );

		return( NULL );
	}
	if( pyevt_records->record_index >= pyevt_records->number_of_records )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	record_object = pyevt_records->get_record_by_index(
	                 pyevt_records->file_object,
	                 pyevt_records->record_index );

	if( record_object != NULL )
	{
		pyevt_records->record_index++;
	}
	return( record_object );
}

