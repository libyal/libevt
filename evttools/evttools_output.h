/*
 * Common output functions for the evttools
 *
 * Copyright (C) 2011-2019, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _EVTTOOLS_OUTPUT_H )
#define _EVTTOOLS_OUTPUT_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "evttools_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int evttools_output_initialize(
     int stdio_mode,
     libcerror_error_t **error );

void evtoutput_copyright_fprint(
      FILE *stream );

void evtoutput_version_fprint(
      FILE *stream,
      const char *program );

void evtoutput_version_detailed_fprint(
      FILE *stream,
      const char *program );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EVTTOOLS_OUTPUT_H ) */

