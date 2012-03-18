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

#include <common.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#include "evttools_codepage.h"

/* Determines the codepage from a string
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int evttools_codepage_from_string(
     int *codepage,
     const libcstring_system_character_t *string,
     size_t string_length,
     uint32_t feature_flags,
     liberror_error_t **error )
{
	static char *function    = "evttools_codepage_from_string";
	size_t string_index      = 0;
	uint32_t supported_flags = 0;
	int codepage_set         = EVTTOOLS_CODEPAGE_SET_UNDEFINED;

	if( codepage == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid codepage.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_length > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	supported_flags = EVTTOOLS_CODEPAGE_FEATURE_FLAG_HAVE_ISO_8859_CODEPAGES
	                | EVTTOOLS_CODEPAGE_FEATURE_FLAG_HAVE_KOI8_CODEPAGES
	                | EVTTOOLS_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS_CODEPAGES;

	if( ( feature_flags & ~( supported_flags ) ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported feature flags value: 0x%08" PRIx32 ".",
		 function,
		 feature_flags );

		return( -1 );
	}
	*codepage = EVTTOOLS_CODEPAGE_UNDEFINED;

	if( string_length == 5 )
	{
		if( libcstring_system_string_compare_no_case(
		     string,
		     _LIBCSTRING_SYSTEM_STRING( "ascii" ),
		     5 ) == 0 )
		{
			*codepage = EVTTOOLS_CODEPAGE_ASCII;

			string_index = 5;
		}
	}
	if( ( feature_flags & EVTTOOLS_CODEPAGE_FEATURE_FLAG_HAVE_ISO_8859_CODEPAGES ) != 0 )
	{
		/* Supported string formats:
		 * iso8859-1, iso8859_1, iso-8859-1, iso-8859_1, iso_8859-1, iso_8859_1
		 */
		if( string_length >= 8 )
		{
			if( libcstring_system_string_compare_no_case(
			     string,
			     _LIBCSTRING_SYSTEM_STRING( "iso" ),
			     3 ) == 0 )
			{
				string_index = 3;

				if( ( string[ string_index ] == (libcstring_system_character_t) '-' )
				 || ( string[ string_index ] == (libcstring_system_character_t) '_' ) )
				{
					string_index++;
				}
				if( ( string[ string_index     ] == (libcstring_system_character_t) '8' )
				 && ( string[ string_index + 1 ] == (libcstring_system_character_t) '8' )
				 && ( string[ string_index + 2 ] == (libcstring_system_character_t) '5' )
				 && ( string[ string_index + 3 ] == (libcstring_system_character_t) '9' ) )
				{
					string_index += 4;

					if( string_index < string_length )
					{
						if( ( string[ string_index ] == (libcstring_system_character_t) '-' )
						 || ( string[ string_index ] == (libcstring_system_character_t) '_' ) )
						{
							string_index++;

							codepage_set = EVTTOOLS_CODEPAGE_SET_ISO_8859;
						}
					}
				}
			}
		}
	}
	if( ( feature_flags & EVTTOOLS_CODEPAGE_FEATURE_FLAG_HAVE_KOI8_CODEPAGES ) != 0 )
	{
		/* Supported string formats:
		 * koi8, koi8-r, koi8_r
		 */
		if( string_length >= 4 )
		{
			if( libcstring_system_string_compare_no_case(
			     string,
			     _LIBCSTRING_SYSTEM_STRING( "koi8" ),
			     4 ) == 0 )
			{
				string_index = 4;

				codepage_set = EVTTOOLS_CODEPAGE_SET_KOI8;

				if( string_index < string_length )
				{
					if( ( string[ string_index ] == (libcstring_system_character_t) '-' )
					 || ( string[ string_index ] == (libcstring_system_character_t) '_' ) )
					{
						string_index++;
					}
				}
			}
		}
	}
	if( ( feature_flags & EVTTOOLS_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS_CODEPAGES ) != 0 )
	{
		/* Supported string formats:
		 * cp1252, ms1252, windows1252, windows-1252, windows_1252
		 */
		if( string_length >= 7 )
		{
			if( libcstring_system_string_compare_no_case(
			     string,
			     _LIBCSTRING_SYSTEM_STRING( "windows" ),
			     7 ) == 0 )
			{
				string_index = 7;

				codepage_set = EVTTOOLS_CODEPAGE_SET_WINDOWS;

				if( string_index < string_length )
				{
					if( ( string[ string_index ] == (libcstring_system_character_t) '-' )
					 || ( string[ string_index ] == (libcstring_system_character_t) '_' ) )
					{
						string_index++;
					}
				}
			}
		}
		else if( string_length >= 2 )
		{
			if( libcstring_system_string_compare_no_case(
			     string,
			     _LIBCSTRING_SYSTEM_STRING( "cp" ),
			     2 ) == 0 )
			{
				string_index = 2;

				codepage_set = EVTTOOLS_CODEPAGE_SET_GENERIC;
			}
			else if( libcstring_system_string_compare_no_case(
			          string,
			          _LIBCSTRING_SYSTEM_STRING( "ms" ),
			          2 ) == 0 )
			{
				string_index = 2;

				codepage_set = EVTTOOLS_CODEPAGE_SET_WINDOWS;
			}
		}
	}
	if( codepage_set == EVTTOOLS_CODEPAGE_SET_ISO_8859 )
	{
		if( ( string_index + 1 ) == string_length )
		{
			switch( string[ string_index ] )
			{
				case (libcstring_system_character_t) '1':
					*codepage = EVTTOOLS_CODEPAGE_ISO_8859_1;
					break;

				case (libcstring_system_character_t) '2':
					*codepage = EVTTOOLS_CODEPAGE_ISO_8859_2;
					break;

				case (libcstring_system_character_t) '3':
					*codepage = EVTTOOLS_CODEPAGE_ISO_8859_3;
					break;

				case (libcstring_system_character_t) '4':
					*codepage = EVTTOOLS_CODEPAGE_ISO_8859_4;
					break;

				case (libcstring_system_character_t) '5':
					*codepage = EVTTOOLS_CODEPAGE_ISO_8859_5;
					break;

				case (libcstring_system_character_t) '6':
					*codepage = EVTTOOLS_CODEPAGE_ISO_8859_6;
					break;

				case (libcstring_system_character_t) '7':
					*codepage = EVTTOOLS_CODEPAGE_ISO_8859_7;
					break;

				case (libcstring_system_character_t) '8':
					*codepage = EVTTOOLS_CODEPAGE_ISO_8859_8;
					break;

				case (libcstring_system_character_t) '9':
					*codepage = EVTTOOLS_CODEPAGE_ISO_8859_9;
					break;

				default:
					break;
			}
		}
		else if( ( string_index + 2 ) == string_length )
		{
			if( string[ string_index ] == (libcstring_system_character_t) '1' )
			{
				string_index++;

				switch( string[ string_index ] )
				{
					case (libcstring_system_character_t) '0':
						*codepage = EVTTOOLS_CODEPAGE_ISO_8859_10;
						break;

					case (libcstring_system_character_t) '1':
						*codepage = EVTTOOLS_CODEPAGE_ISO_8859_11;
						break;

					case (libcstring_system_character_t) '3':
						*codepage = EVTTOOLS_CODEPAGE_ISO_8859_13;
						break;

					case (libcstring_system_character_t) '4':
						*codepage = EVTTOOLS_CODEPAGE_ISO_8859_14;
						break;

					case (libcstring_system_character_t) '5':
						*codepage = EVTTOOLS_CODEPAGE_ISO_8859_15;
						break;

					case (libcstring_system_character_t) '6':
						*codepage = EVTTOOLS_CODEPAGE_ISO_8859_16;
						break;

					default:
						break;
				}
			}
		}
	}
	if( codepage_set == EVTTOOLS_CODEPAGE_SET_KOI8 )
	{
		if( ( string_index + 1 ) == string_length )
		{
			switch( string[ string_index ] )
			{
				case (libcstring_system_character_t) 'r':
				case (libcstring_system_character_t) 'R':
					*codepage = EVTTOOLS_CODEPAGE_KOI8_R;
					break;

				case (libcstring_system_character_t) 'u':
				case (libcstring_system_character_t) 'U':
					*codepage = EVTTOOLS_CODEPAGE_KOI8_U;
					break;

				default:
					break;
			}
		}
	}
	if( ( codepage_set == EVTTOOLS_CODEPAGE_SET_GENERIC )
	 || ( codepage_set == EVTTOOLS_CODEPAGE_SET_WINDOWS ) )
	{
		if( ( string_index + 3 ) == string_length )
		{
			switch( string[ string_index ] )
			{
				case (libcstring_system_character_t) '8':
					if( ( string[ string_index + 1 ] == (libcstring_system_character_t) '7' )
					 && ( string[ string_index + 2 ] == (libcstring_system_character_t) '4' ) )
					{
						*codepage = EVTTOOLS_CODEPAGE_WINDOWS_874;
					}
					break;

				case (libcstring_system_character_t) '9':
					if( ( string[ string_index + 1 ] == (libcstring_system_character_t) '3' )
					 && ( string[ string_index + 2 ] == (libcstring_system_character_t) '2' ) )
					{
						*codepage = EVTTOOLS_CODEPAGE_WINDOWS_932;
					}
					else if( ( string[ string_index + 1 ] == (libcstring_system_character_t) '3' )
					      && ( string[ string_index + 2 ] == (libcstring_system_character_t) '6' ) )
					{
						*codepage = EVTTOOLS_CODEPAGE_WINDOWS_936;
					}
					else if( ( string[ string_index + 1 ] == (libcstring_system_character_t) '4' )
					      && ( string[ string_index + 2 ] == (libcstring_system_character_t) '9' ) )
					{
						*codepage = EVTTOOLS_CODEPAGE_WINDOWS_949;
					}
					else if( ( string[ string_index + 1 ] == (libcstring_system_character_t) '5' )
					      && ( string[ string_index + 2 ] == (libcstring_system_character_t) '0' ) )
					{
						*codepage = EVTTOOLS_CODEPAGE_WINDOWS_949;
					}
					break;

				default:
					break;
			}
		}
		else if( ( string_index + 4 ) == string_length )
		{
			if( ( string[ string_index     ] == (libcstring_system_character_t) '1' )
			 && ( string[ string_index + 1 ] == (libcstring_system_character_t) '2' )
			 && ( string[ string_index + 2 ] == (libcstring_system_character_t) '5' ) )
			{
				switch( string[ string_index + 3 ] )
				{
					case (libcstring_system_character_t) '0':
						*codepage = EVTTOOLS_CODEPAGE_WINDOWS_1250;
						break;

					case (libcstring_system_character_t) '1':
						*codepage = EVTTOOLS_CODEPAGE_WINDOWS_1251;
						break;

					case (libcstring_system_character_t) '2':
						*codepage = EVTTOOLS_CODEPAGE_WINDOWS_1252;
						break;

					case (libcstring_system_character_t) '3':
						*codepage = EVTTOOLS_CODEPAGE_WINDOWS_1253;
						break;

					case (libcstring_system_character_t) '4':
						*codepage = EVTTOOLS_CODEPAGE_WINDOWS_1254;
						break;

					case (libcstring_system_character_t) '5':
						*codepage = EVTTOOLS_CODEPAGE_WINDOWS_1255;
						break;

					case (libcstring_system_character_t) '6':
						*codepage = EVTTOOLS_CODEPAGE_WINDOWS_1256;
						break;

					case (libcstring_system_character_t) '7':
						*codepage = EVTTOOLS_CODEPAGE_WINDOWS_1257;
						break;

					case (libcstring_system_character_t) '8':
						*codepage = EVTTOOLS_CODEPAGE_WINDOWS_1258;
						break;

					default:
						break;
				}
			}
		}
	}
	if( *codepage != EVTTOOLS_CODEPAGE_UNDEFINED )
	{
		return( 1 );
	}
	return( 0 );
}

/* Returns a string representation of the codepage
 * Returns 1 if successful or NULL if codepage is not supported
 */
const libcstring_system_character_t *evttools_codepage_to_string(
                                      int codepage )
{
	switch( codepage )
	{
		case EVTTOOLS_CODEPAGE_ASCII:
			return( _LIBCSTRING_SYSTEM_STRING( "ascii" ) );

		case EVTTOOLS_CODEPAGE_ISO_8859_1:
			return( _LIBCSTRING_SYSTEM_STRING( "iso-8859-1" ) );

		case EVTTOOLS_CODEPAGE_ISO_8859_2:
			return( _LIBCSTRING_SYSTEM_STRING( "iso-8859-2" ) );

		case EVTTOOLS_CODEPAGE_ISO_8859_3:
			return( _LIBCSTRING_SYSTEM_STRING( "iso-8859-3" ) );

		case EVTTOOLS_CODEPAGE_ISO_8859_4:
			return( _LIBCSTRING_SYSTEM_STRING( "iso-8859-4" ) );

		case EVTTOOLS_CODEPAGE_ISO_8859_5:
			return( _LIBCSTRING_SYSTEM_STRING( "iso-8859-5" ) );

		case EVTTOOLS_CODEPAGE_ISO_8859_6:
			return( _LIBCSTRING_SYSTEM_STRING( "iso-8859-6" ) );

		case EVTTOOLS_CODEPAGE_ISO_8859_7:
			return( _LIBCSTRING_SYSTEM_STRING( "iso-8859-7" ) );

		case EVTTOOLS_CODEPAGE_ISO_8859_8:
			return( _LIBCSTRING_SYSTEM_STRING( "iso-8859-8" ) );

		case EVTTOOLS_CODEPAGE_ISO_8859_9:
			return( _LIBCSTRING_SYSTEM_STRING( "iso-8859-9" ) );

		case EVTTOOLS_CODEPAGE_ISO_8859_10:
			return( _LIBCSTRING_SYSTEM_STRING( "iso-8859-10" ) );

		case EVTTOOLS_CODEPAGE_ISO_8859_11:
			return( _LIBCSTRING_SYSTEM_STRING( "iso-8859-11" ) );

		case EVTTOOLS_CODEPAGE_ISO_8859_13:
			return( _LIBCSTRING_SYSTEM_STRING( "iso-8859-13" ) );

		case EVTTOOLS_CODEPAGE_ISO_8859_14:
			return( _LIBCSTRING_SYSTEM_STRING( "iso-8859-14" ) );

		case EVTTOOLS_CODEPAGE_ISO_8859_15:
			return( _LIBCSTRING_SYSTEM_STRING( "iso-8859-15" ) );

		case EVTTOOLS_CODEPAGE_ISO_8859_16:
			return( _LIBCSTRING_SYSTEM_STRING( "iso-8859-16" ) );

		case EVTTOOLS_CODEPAGE_KOI8_R:
			return( _LIBCSTRING_SYSTEM_STRING( "koi8-r" ) );

		case EVTTOOLS_CODEPAGE_KOI8_U:
			return( _LIBCSTRING_SYSTEM_STRING( "koi8-u" ) );

		case EVTTOOLS_CODEPAGE_WINDOWS_874:
			return( _LIBCSTRING_SYSTEM_STRING( "windows-874" ) );

		case EVTTOOLS_CODEPAGE_WINDOWS_932:
			return( _LIBCSTRING_SYSTEM_STRING( "windows-932" ) );

		case EVTTOOLS_CODEPAGE_WINDOWS_936:
			return( _LIBCSTRING_SYSTEM_STRING( "windows-936" ) );

		case EVTTOOLS_CODEPAGE_WINDOWS_949:
			return( _LIBCSTRING_SYSTEM_STRING( "windows-949" ) );

		case EVTTOOLS_CODEPAGE_WINDOWS_950:
			return( _LIBCSTRING_SYSTEM_STRING( "windows-950" ) );

		case EVTTOOLS_CODEPAGE_WINDOWS_1250:
			return( _LIBCSTRING_SYSTEM_STRING( "windows-1250" ) );

		case EVTTOOLS_CODEPAGE_WINDOWS_1251:
			return( _LIBCSTRING_SYSTEM_STRING( "windows-1251" ) );

		case EVTTOOLS_CODEPAGE_WINDOWS_1252:
			return( _LIBCSTRING_SYSTEM_STRING( "windows-1252" ) );

		case EVTTOOLS_CODEPAGE_WINDOWS_1253:
			return( _LIBCSTRING_SYSTEM_STRING( "windows-1253" ) );

		case EVTTOOLS_CODEPAGE_WINDOWS_1254:
			return( _LIBCSTRING_SYSTEM_STRING( "windows-1254" ) );

		case EVTTOOLS_CODEPAGE_WINDOWS_1255:
			return( _LIBCSTRING_SYSTEM_STRING( "windows-1255" ) );

		case EVTTOOLS_CODEPAGE_WINDOWS_1256:
			return( _LIBCSTRING_SYSTEM_STRING( "windows-1256" ) );

		case EVTTOOLS_CODEPAGE_WINDOWS_1257:
			return( _LIBCSTRING_SYSTEM_STRING( "windows-1257" ) );

		case EVTTOOLS_CODEPAGE_WINDOWS_1258:
			return( _LIBCSTRING_SYSTEM_STRING( "windows-1258" ) );

		default:
			break;
	}
	return( NULL );
}

