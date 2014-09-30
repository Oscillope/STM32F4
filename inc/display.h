#ifndef DISPLAY_H
#define DISPLAY_H

#define ROW1		0x0004
#define ROW2		0x0002
#define ROW3		0x0006
#define ROW4		0x0001
#define ROW5		0x0005
#define ROW6		0x0003
#define ROW7		0x0007

const short rows[] = {ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7};

struct framebuffer {
	char			glyph[16][7][5];
	short			*cur_row;
	int			idx;
	struct framebuffer	*next;
};

struct framebuffer *cur_buf;

const struct framebuffer test_buf = {
	.glyph		= {A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A},
	.cur_row	= &rows[0],
	.idx		= 0,
	.next		= &test_buf
};

const char A[8][5] = {
	[ROW1]	= {0, 0, 1, 0, 0},
	[ROW2]	= {0, 1, 0, 1, 0},
	[ROW3]	= {1, 1, 0, 1, 1},
	[ROW4]	= {1, 0, 0, 0, 1},
	[ROW5]	= {1, 1, 1, 1, 1},
	[ROW6]	= {1, 0, 0, 0, 1},
	[ROW7]	= {1, 0, 0, 0, 1}
};

#endif /* DISPLAY_H */
