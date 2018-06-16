/* ba2/cinode - read and write ba2 archive chunk inodes
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

#define	MAGIC	0xBAADF00DUL	/* TODO: endianness */

int
ba2readcinode(BA2cinode *cinode, BA2inode inode, BA2hdr hdr, FILE *f)
{
	unsigned long	magic;

	inode.cinodelen -= 20;
	cinode->offset = ba2get64(f);
	cinode->zlen = ba2get32(f);
	cinode->len = ba2get32(f);
	if (hdr.type == BA2T_DX10) {
		inode.cinodelen -= 4;
		cinode->bmip = ba2get16(f);
		cinode->emip = ba2get16(f);
	}
	for (magic = MAGIC; magic; magic >>= 8)
		if (ba2get8(f) != magic & 0xFF)
			return -1;
	while (inode.cinodelen--)
		if (ba2get8(f) < 0)
			return -1;
	return 0;
}

int
ba2writecinode(const BA2cinode *cinode, BA2inode inode, BA2hdr hdr, FILE *f)
{
	inode.cinodelen -= 20;
	ba2put64(cinode->offset, f);
	ba2put32(cinode->zlen, f);
	ba2put32(cinode->len, f);
	if (hdr.type == BA2T_DX10) {
		inode.cinodelen -= 4;
		ba2put16(cinode->bmip, f);
		ba2put16(cinode->emip, f);
	}
	ba2put32(MAGIC, f);
	while (inode.cinodelen--)
		if (ba2put8(0, f) < 0)
			return -1;
	return 0;
}
