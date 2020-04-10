/*
 * Event record of the Windows Event Log (EVT) format
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

#if !defined( _EVT_EVENT_RECORD_H )
#define _EVT_EVENT_RECORD_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct evt_event_record evt_event_record_t;

struct evt_event_record
{
	/* Size
	 */
	uint8_t size[ 4 ];

	/* Signature
	 */
	uint8_t signature[ 4 ];

	/* Record number
	 */
	uint8_t record_number[ 4 ];

	/* Creation time
	 */
	uint8_t creation_time[ 4 ];

	/* Last written time
	 */
	uint8_t last_written_time[ 4 ];

	/* Event identifier
	 */
	uint8_t event_identifier[ 4 ];

	/* Event type
	 */
	uint8_t event_type[ 2 ];

	/* Number of strings
	 */
	uint8_t number_of_strings[ 2 ];

	/* Event category
	 */
	uint8_t event_category[ 2 ];

	/* Unknown1
	 */
	uint8_t unknown1[ 2 ];

	/* Unknown2
	 */
	uint8_t unknown2[ 4 ];

	/* Event strings offset
	 */
	uint8_t event_strings_offset[ 4 ];

	/* User security identifier size
	 */
	uint8_t user_security_identifier_size[ 4 ];

	/* User security identifier offset
	 */
	uint8_t user_security_identifier_offset[ 4 ];

	/* Event data size
	 */
	uint8_t event_data_size[ 4 ];

	/* Event data offset
	 */
	uint8_t event_data_offset[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EVT_EVENT_RECORD_H ) */

