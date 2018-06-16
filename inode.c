/* ba2/inode - read and write ba2 archive inodes
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

int
ba2readinode(BA2inode *inode, BA2hdr hdr, FILE *f)
{
	ba2get32(f);	/* TODO */
	fread(inode->ext, sizeof(*inode->ext), sizeof(inode->ext) / sizeof(*inode->ext), f);
	ba2get64(f);	/* TODO */
	if (hdr.type == BA2T_DX10) {
		inode->nchunks = ba2get8(f);
		inode->cinodelen = ba2get16(f);
		inode->height = ba2get16(f);
		inode->width = ba2get16(f);
		inode->nmips = ba2get8(f);
		inode->format = ba2get8(f);
	} else {
		inode->nchunks = 1;
		inode->cinodelen = 20;
		inode->height = 0;
		inode->width = 0;
		inode->nmips = 0;
		inode->format = 0;
	}
	return -(feof(f) || ferror(f));
}

int
ba2writeinode(const BA2inode *inode, BA2hdr hdr, FILE *f)
{
	ba2put32(0UL, f);	/* TODO */
	fwrite(inode->ext, sizeof(*inode->ext), sizeof(inode->ext) / sizeof(*inode->ext), f);
	ba2put64(0ULL, f);	/* TODO */
	if (hdr.type == BA2T_DX10) {
		ba2put8(inode->nchunks, f);
		ba2put16(inode->cinodelen, f);
		ba2put16(inode->height, f);
		ba2put16(inode->width, f);
		ba2put8(inode->nmips, f);
		ba2put8(inode->format, f);
	}
	return -!!ferror(f);
}
