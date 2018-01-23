/*
 * Common input functions for the evttools
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

#if !defined( _EVTINPUT_H )
#define _EVTINPUT_H

#include <common.h>
#include <types.h>

#include "evttools_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum EVTTOOLS_EVENT_LOG_TYPES
{
	EVTTOOLS_EVENT_LOG_TYPE_UNKNOWN,
	EVTTOOLS_EVENT_LOG_TYPE_APPLICATION,
	EVTTOOLS_EVENT_LOG_TYPE_INTERNET_EXPLORER,
	EVTTOOLS_EVENT_LOG_TYPE_SECURITY,
	EVTTOOLS_EVENT_LOG_TYPE_SYSTEM,
	EVTTOOLS_EVENT_LOG_TYPE_WINDOWS_POWERSHELL,
};

int evtinput_determine_event_log_type(
     const system_character_t *string,
     int *event_log_type,
     libcerror_error_t **error );

int evtinput_determine_event_log_type_from_filename(
     const system_character_t *filename,
     int *event_log_type,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EVTINPUT_H ) */

