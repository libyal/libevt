/*
 * Notification functions
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

#if !defined( _LIBEVT_NOTIFY_H )
#define _LIBEVT_NOTIFY_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "libevt_extern.h"
#include "libevt_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBEVT )

LIBEVT_EXTERN \
void libevt_notify_set_verbose(
      int verbose );

LIBEVT_EXTERN \
int libevt_notify_set_stream(
     FILE *stream,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_notify_stream_open(
     const char *filename,
     libcerror_error_t **error );

LIBEVT_EXTERN \
int libevt_notify_stream_close(
     libcerror_error_t **error );

#endif /* !defined( HAVE_LOCAL_LIBEVT ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVT_NOTIFY_H ) */

