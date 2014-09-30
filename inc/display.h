#ifndef DISPLAY_H
#define DISPLAY_H

#define ROW1		0x0004
#define ROW2		0x0002
#define ROW3		0x0006
#define ROW4		0x0001
#define ROW5		0x0005
#define ROW6		0x0003
#define ROW7		0x0007

short rows[] = {ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7};

const char A[8] = {
	[0]	= 0x00,	// 00000
	[ROW1]	= 0x04,	// 00100
	[ROW2]	= 0x0a,	// 01010
	[ROW3]	= 0x11,	// 10001
	[ROW4]	= 0x1f,	// 11111
	[ROW5]	= 0x11,	// 10001
	[ROW6]	= 0x11,	// 10001
	[ROW7]	= 0x11,	// 10001
};

struct framebuffer {
	char			*glyph[16];
	short			*cur_row;
	int			idx;
	struct framebuffer	*next;
};

struct framebuffer test_buf = {
	.glyph		= {[0 ... 15] = A},
	.cur_row	= &rows[0],
	.idx		= 0,
	.next		= &test_buf,
};

struct framebuffer *cur_buf = &test_buf;

#endif /* DISPLAY_H */
