/*
 * Codepage functions
 *
 * Copyright (c) 2011-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _EVTTOOLS_CODEPAGE_H )
#define _EVTTOOLS_CODEPAGE_H

#include <common.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#include "evttools_libevt.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* The codepage feature flag definitions
 */
enum EVTTOOLS_CODEPAGE_FEATURE_FLAGS
{
	EVTTOOLS_CODEPAGE_FEATURE_FLAG_HAVE_ISO_8859_CODEPAGES	= 0x00000001UL,
	EVTTOOLS_CODEPAGE_FEATURE_FLAG_HAVE_KOI8_CODEPAGES	= 0x00000002UL,
	EVTTOOLS_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS_CODEPAGES	= 0x00000004UL,
};

/* The codepage set definitions
 */
enum EVTTOOLS_CODEPAGE_SETS
{
	EVTTOOLS_CODEPAGE_SET_UNDEFINED				= 0,
	EVTTOOLS_CODEPAGE_SET_GENERIC				= 1,
	EVTTOOLS_CODEPAGE_SET_ISO_8859				= (int) 'i',
	EVTTOOLS_CODEPAGE_SET_KOI8				= (int) 'k',
	EVTTOOLS_CODEPAGE_SET_WINDOWS				= (int) 'w'
};

/* The codepage definitions
 */
enum EVTTOOLS_CODEPAGES
{
	EVTTOOLS_CODEPAGE_UNDEFINED				= 0,

	EVTTOOLS_CODEPAGE_ASCII					= LIBEVT_CODEPAGE_ASCII,

	EVTTOOLS_CODEPAGE_ISO_8859_1				= LIBEVT_CODEPAGE_ISO_8859_1,
	EVTTOOLS_CODEPAGE_ISO_8859_2				= LIBEVT_CODEPAGE_ISO_8859_2,
	EVTTOOLS_CODEPAGE_ISO_8859_3				= LIBEVT_CODEPAGE_ISO_8859_3,
	EVTTOOLS_CODEPAGE_ISO_8859_4				= LIBEVT_CODEPAGE_ISO_8859_4,
	EVTTOOLS_CODEPAGE_ISO_8859_5				= LIBEVT_CODEPAGE_ISO_8859_5,
	EVTTOOLS_CODEPAGE_ISO_8859_6				= LIBEVT_CODEPAGE_ISO_8859_6,
	EVTTOOLS_CODEPAGE_ISO_8859_7				= LIBEVT_CODEPAGE_ISO_8859_7,
	EVTTOOLS_CODEPAGE_ISO_8859_8				= LIBEVT_CODEPAGE_ISO_8859_8,
	EVTTOOLS_CODEPAGE_ISO_8859_9				= LIBEVT_CODEPAGE_ISO_8859_9,
	EVTTOOLS_CODEPAGE_ISO_8859_10				= LIBEVT_CODEPAGE_ISO_8859_10,
	EVTTOOLS_CODEPAGE_ISO_8859_11				= LIBEVT_CODEPAGE_ISO_8859_11,
	EVTTOOLS_CODEPAGE_ISO_8859_13				= LIBEVT_CODEPAGE_ISO_8859_13,
	EVTTOOLS_CODEPAGE_ISO_8859_14				= LIBEVT_CODEPAGE_ISO_8859_14,
	EVTTOOLS_CODEPAGE_ISO_8859_15				= LIBEVT_CODEPAGE_ISO_8859_15,
	EVTTOOLS_CODEPAGE_ISO_8859_16				= LIBEVT_CODEPAGE_ISO_8859_16,

	EVTTOOLS_CODEPAGE_KOI8_R				= LIBEVT_CODEPAGE_KOI8_R,
	EVTTOOLS_CODEPAGE_KOI8_U				= LIBEVT_CODEPAGE_KOI8_U,

	EVTTOOLS_CODEPAGE_WINDOWS_874				= LIBEVT_CODEPAGE_WINDOWS_874,
	EVTTOOLS_CODEPAGE_WINDOWS_932				= LIBEVT_CODEPAGE_WINDOWS_932,
	EVTTOOLS_CODEPAGE_WINDOWS_936				= LIBEVT_CODEPAGE_WINDOWS_936,
	EVTTOOLS_CODEPAGE_WINDOWS_949				= LIBEVT_CODEPAGE_WINDOWS_949,
	EVTTOOLS_CODEPAGE_WINDOWS_950				= LIBEVT_CODEPAGE_WINDOWS_950,
	EVTTOOLS_CODEPAGE_WINDOWS_1250				= LIBEVT_CODEPAGE_WINDOWS_1250,
	EVTTOOLS_CODEPAGE_WINDOWS_1251				= LIBEVT_CODEPAGE_WINDOWS_1251,
	EVTTOOLS_CODEPAGE_WINDOWS_1252				= LIBEVT_CODEPAGE_WINDOWS_1252,
	EVTTOOLS_CODEPAGE_WINDOWS_1253				= LIBEVT_CODEPAGE_WINDOWS_1253,
	EVTTOOLS_CODEPAGE_WINDOWS_1254				= LIBEVT_CODEPAGE_WINDOWS_1254,
	EVTTOOLS_CODEPAGE_WINDOWS_1255				= LIBEVT_CODEPAGE_WINDOWS_1255,
	EVTTOOLS_CODEPAGE_WINDOWS_1256				= LIBEVT_CODEPAGE_WINDOWS_1256,
	EVTTOOLS_CODEPAGE_WINDOWS_1257				= LIBEVT_CODEPAGE_WINDOWS_1257,
	EVTTOOLS_CODEPAGE_WINDOWS_1258				= LIBEVT_CODEPAGE_WINDOWS_1258
};

int evttools_codepage_from_string(
     int *codepage,
     const libcstring_system_character_t *string,
     size_t string_length,
     uint32_t feature_flags,
     liberror_error_t **error );

const libcstring_system_character_t *evttools_codepage_to_string(
                                      int codepage );

/* TODO add list function */

#if defined( __cplusplus )
}
#endif

#endif

