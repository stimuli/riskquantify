/*
** bh_strcpy.h
**
** safe strcpy(3)
** 
** Written by Brett Hutley - brett@hutley.net
** 
** Copyright (C) 2003 Brett Hutley
** 
** This file is part of the Risk Quantify Library
** 
** Risk Quantify is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public License
** as published by the Free Software Foundation; either version 2 of
** the License, or (at your option) any later version.
** 
** Risk Quantify is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
** 
** You should have received a copy of the GNU Library General Public
** License along with Risk Quantify; if not, write to the Free
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
** USA
*/ 
#ifndef bh_strcpy_h
#define bh_strcpy_h

int bh_strcpy(char *dest, const char *src, unsigned maxdestlen);
int bh_memcpy(char *dest, const char *src, unsigned numtocopy, unsigned maxdestlen);

/** This function copies numtocopy bytes from the source buffer to the 
   destination buffer, and then null-terminates the destination buffer
*/
int bh_memzcpy(char *dest, const char *src, unsigned numtocopy, unsigned maxdestlen);

#endif
