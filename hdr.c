/* ba2/hdr - read and write ba2 archive headers
 * Copyright (C) 2018  Oskar Sveinsen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include	<stdio.h>

#include	<ba2.h>

#define	MAGIC	0x58445442UL	/* BTDX (reverse) */

int
ba2readhdr(BA2hdr *hdr, FILE *f)
{
	unsigned long	magic;

	for (magic = MAGIC; magic; magic >>= 8)
		if (ba2get8(f) != magic & 0xFF)
			return -1;

	hdr->v = ba2get32(f);
	hdr->type = ba2get32(f);
	hdr->nfiles = ba2get32(f);
	hdr->nameoff = ba2get64(f);
	return -(feof(f) || ferror(f));
}

int
ba2writehdr(const BA2hdr *hdr, FILE *f)
{
	unsigned long	magic;

	for (magic = MAGIC; magic; magic >>= 8)
		ba2put8(magic, f);

	ba2put32(hdr->v, f);
	ba2put32(hdr->type, f);
	ba2put32(hdr->nfiles, f);
	return -(ba2put64(hdr->nameoff, f) < 0);
}
