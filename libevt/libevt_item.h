/*
 * Item functions
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

#if !defined( _LIBEVT_INTERNAL_ITEM_H )
#define _LIBEVT_INTERNAL_ITEM_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libevt_extern.h"
#include "libevt_io_handle.h"
#include "libevt_libbfio.h"
#include "libevt_record.h"
#include "libevt_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBEVT_ITEM_FLAGS_DEFAULT			LIBEVT_ITEM_FLAG_NON_MANAGED_FILE_IO_HANDLE

enum LIBEVT_ITEM_FLAGS
{
	LIBEVT_ITEM_FLAG_NON_MANAGED_FILE_IO_HANDLE	= 0x00,
	LIBEVT_ITEM_FLAG_MANAGED_FILE_IO_HANDLE		= 0x01,
};

typedef struct libevt_internal_item libevt_internal_item_t;

struct libevt_internal_item
{
	/* The IO handle
	 */
	libevt_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The event record
	 */
	libevt_record_t *event_record;

	/* The flags
	 */
	uint8_t flags;
};

int libevt_item_initialize(
     libevt_item_t **item,
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libevt_record_t *event_record,
     uint8_t flags,
     liberror_error_t **error );

LIBEVT_EXTERN \
int libevt_item_free(
     libevt_item_t **item,
     liberror_error_t **error );

LIBEVT_EXTERN \
int libevt_item_get_number_of_entries(
     libevt_item_t *item,
     uint32_t *number_of_entries,
     liberror_error_t **error );

LIBEVT_EXTERN \
int libevt_item_get_entry_type(
     libevt_item_t *item,
     int entry_index,
     uint32_t *entry_type,
     uint32_t *value_type,
     liberror_error_t **error );

LIBEVT_EXTERN \
int libevt_item_get_entry_value(
     libevt_item_t *item,
     uint32_t entry_type,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     uint8_t flags,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

