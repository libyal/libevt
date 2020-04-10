/*
 * File header of the Windows Event Log (EVT) format
 *
 * Copyright (C) 2011-2020, Joachim Metz <joachim.metz@gmail.com>
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
	/* (Header) size
	 */
	uint8_t size[ 4 ];

	/* Signature
	 */
	uint8_t signature[ 4 ];

	/* Major format version
	 */
	uint8_t major_format_version[ 4 ];

	/* Minor format version
	 */
	uint8_t minor_format_version[ 4 ];

	/* First (oldest) record offset
	 */
	uint8_t first_record_offset[ 4 ];

	/* End of file record offset
	 */
	uint8_t end_of_file_record_offset[ 4 ];

	/* Last (newest) record number
	 */
	uint8_t last_record_number[ 4 ];

	/* First (oldest) record number
	 */
	uint8_t first_record_number[ 4 ];

	/* Maximum file size
	 */
	uint8_t maximum_file_size[ 4 ];

	/* File flags
	 */
	uint8_t file_flags[ 4 ];

	/* Retention
	 */
	uint8_t retention[ 4 ];

	/* Copy of (header) size
	 */
	uint8_t copy_of_size[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EVT_FILE_HEADER_H ) */

