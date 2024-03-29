/*
 * The item definition of a Windows Event Log (EVT)
 *
 * Copyright (C) 2011-2024, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _EVT_RECORD_H )
#define _EVT_RECORD_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct evt_record_event_header evt_record_event_header_t;

struct evt_record_event_header
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

	/* The record number
	 * Consists of 4 bytes
	 */
	uint8_t record_number[ 4 ];

	/* The creation date and time
	 * Consists of 4 bytes
	 * Contain s 32-bit Unix epoch timestamp
	 */
	uint8_t creation_time[ 4 ];

	/* The (last) written date and time
	 * Consists of 4 bytes
	 * Contain s 32-bit Unix epoch timestamp
	 */
	uint8_t written_time[ 4 ];

	/* The event identifier
	 * Consists of 4 bytes
	 */
	uint8_t event_identifier[ 4 ];

	/* The event type
	 * Consists of 2 bytes
	 */
	uint8_t event_type[ 2 ];

	/* The number of strings
	 * Consists of 2 bytes
	 */
	uint8_t number_of_strings[ 2 ];

	/* The event category
	 * Consists of 2 bytes
	 */
	uint8_t event_category[ 2 ];

	/* The event flags
	 * Consists of 2 bytes
	 */
	uint8_t event_flags[ 2 ];

	/* The closing record number
	 * Consists of 4 bytes
	 */
	uint8_t closing_record_number[ 4 ];

	/* The strings offset
	 * Consists of 4 bytes
	 */
	uint8_t strings_offset[ 4 ];

	/* The user security identifier (SID) size
	 * Consists of 4 bytes
	 */
	uint8_t user_sid_size[ 4 ];

	/* The user security identifier (SID) offset
	 * Consists of 4 bytes
	 */
	uint8_t user_sid_offset[ 4 ];

	/* The data size
	 * Consists of 4 bytes
	 */
	uint8_t data_size[ 4 ];

	/* The data offset
	 * Consists of 4 bytes
	 */
	uint8_t data_offset[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EVT_RECORD_H ) */

