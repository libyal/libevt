/*
 * End-of-file record of the Windows Event Log (EVT) format
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

#if !defined( _EVT_END_OF_FILE_RECORD_H )
#define _EVT_END_OF_FILE_RECORD_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct evt_end_of_file_record evt_end_of_file_record_t;

struct evt_end_of_file_record
{
	/* (Record) size
	 */
	uint8_t size[ 4 ];

	/* Signature1
	 */
	uint8_t signature1[ 4 ];

	/* Signature2
	 */
	uint8_t signature2[ 4 ];

	/* Signature3
	 */
	uint8_t signature3[ 4 ];

	/* Signature4
	 */
	uint8_t signature4[ 4 ];

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

	/* Copy of (record) size
	 */
	uint8_t copy_of_size[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EVT_END_OF_FILE_RECORD_H ) */

