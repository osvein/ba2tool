/* ba2 - read and write ba2 archives
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

#define	BA2T_GNRL	0x474E524CUL
#define	BA2T_DX10	0x44583130UL

#define	ba2get8(f)	((unsigned char)getc(f))
#define	ba2get16(f)	(ba2get8(f) | (unsigned short)ba2get8(f) << 8)
#define	ba2get32(f)	(ba2get16(f) | (unsigned long)ba2get16(f) << 16)
#define	ba2get64(f)	(ba2get32(f) | (unsigned long long)ba2get32(f) << 32)
#define ba2put8(c, f)	(putc((unsigned char)c, f))
#define	ba2put16(c, f)	(ba2put8((unsigned short)c >> 8, f), ba2put8(c, f))
#define	ba2put32(c, f)	(ba2put16((unsigned long)c >> 16, f), ba2put16(c, f))
#define ba2put64(c, f)	(ba2put32((unsigned long long)c >> 32, f), ba2put32(c, f))

typedef	struct BA2hdr	BA2hdr;
typedef	struct BA2inode	BA2inode;
typedef	struct BA2cinode	BA2cinode;

struct BA2hdr {
	unsigned long	v;	/* ba2 version */
	unsigned long	type;	/* BA2T enum */
	unsigned long	nfiles;
	unsigned long long	nameoff;	/* nametable offset from start of file */
};

struct BA2inode {
	char	ext[4];
	unsigned char	nchunks;
	unsigned short	cinodelen;	/* size (in bytes) of one cinode */
	unsigned short	height;
	unsigned short	width;
	unsigned char	nmips;
	unsigned char	format;	/* DXGI_FORMAT enum */
};

struct BA2cinode {
	unsigned long long	offset;	/* data offset from start of file */
	unsigned long	zlen;	/* size (in bytes) of compressed data */
	unsigned long	len;	/* size (in bytes) of uncompressed data */
	unsigned short	bmip;
	unsigned short	emip;
};

int	ba2readhdr(BA2hdr *out, FILE *in);
int	ba2writehdr(const BA2hdr *in, FILE *out);
int	ba2readinode(BA2inode *out, BA2hdr, FILE *in);
int	ba2writeinode(const BA2inode *in, BA2hdr, FILE *out);
int	ba2readcinode(BA2cinode *out, BA2inode, BA2hdr, FILE *in);
int	ba2writecinode(const BA2cinode *in, BA2inode, BA2hdr, FILE *out);
