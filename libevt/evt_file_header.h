/*
 * The file header definition of a Windows Event Log (EVT)
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

#if !defined( _EVT_FILE_HEADER_H )
#define _EVT_FILE_HEADER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct evt_file_header evt_file_header_t;

struct evt_file_header
{
	/* The (header) size
	 * Consists of 4 bytes
	 */
	uint8_t size[ 4 ];

	/* Signature
	 * Consists of 4 bytes
	 * "LfLe"
	 */
	uint8_t signature[ 4 ];

	/* The major version
	 * Consists of 4 bytes
	 */
	uint8_t major_version[ 4 ];

	/* The minor version
	 * Consists of 4 bytes
	 */
	uint8_t minor_version[ 4 ];

	/* The first (oldest) record offset
	 * Consists of 4 bytes
	 */
	uint8_t first_record_offset[ 4 ];

	/* The end of file record offset
	 * Consists of 4 bytes
	 */
	uint8_t end_of_file_record_offset[ 4 ];

	/* The last (newest) record number
	 * Consists of 4 bytes
	 */
	uint8_t last_record_number[ 4 ];

	/* The first (oldest) record number
	 * Consists of 4 bytes
	 */
	uint8_t first_record_number[ 4 ];

	/* The maximum file size
	 * Consists of 4 bytes
	 */
	uint8_t maximum_file_size[ 4 ];

	/* The file flags
	 * Consists of 4 bytes
	 */
	uint8_t file_flags[ 4 ];

	/* The retention
	 * Consists of 4 bytes
	 */
	uint8_t retention[ 4 ];

	/* Copy of the (header) size
	 * Consists of 4 bytes
	 */
	uint8_t size_copy[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EVT_FILE_HEADER_H ) */

