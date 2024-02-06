#include <stdint.h>
#include <stddef.h>

#include "md5.h"

#ifdef _DEBUG_
# include <stdio.h>
# define DEBUG_PRINTF(...) {printf(__VA_ARGS__);}
#else
# define DEBUG_PRINTF(...) {}
#endif

static const uint32_t r[] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};
static uint32_t k[] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};
static uint8_t padding[] = {
	(1 << 7), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static inline size_t congruant_448_mod_512(size_t len) {
	// need to add bit 1 in anycase
	if (len == 448 || len % 512 == 448)
		return 512;
	return ((448 - (len % 512) + 512) % 512);
}
// fuck macro, inline asm better
static inline uint32_t left_rotate(uint32_t x, uint32_t n) {
	uint32_t r;
	asm ("mov %%eax, %[in1]\n"
		 "rol %%eax, %[in2]\n"
		 "mov %[out], %%eax\n"
		 : [out] "=r" (r)
		 : [in1] "r" (x), [in2] "r" (n)
		 : "eax"
	);
	return r;
}


void md5_init(md5_t *ctx) {
	ctx->size     = 0;
	ctx->h0       = 0x67452301;
	ctx->h1       = 0xEFCDAB89;
	ctx->h2       = 0x98BADCFE;
	ctx->h3       = 0x10325476;
	ctx->tmp_size = 0;
}
static void md5_step(md5_t *ctx, uint32_t w[16]) {
	uint32_t a = ctx->h0;
	uint32_t b = ctx->h1;
	uint32_t c = ctx->h2;
	uint32_t d = ctx->h3;
	uint32_t f, g, tmp;

	for (size_t j = 0; j < 64; j++) {
		if (j < 16) {
			f = (b & c) | ((~b) & d);
			g = j;
		} else if (j < 32) {
			f = (b & d) | (c & (~d));
			g = ((5*j) + 1) % 16;
		} else if (j < 48) {
			f = b ^ c ^ d;
			g = ((3*j) + 5) % 16;
		} else {
			f = c ^ (b | (~d));
			g = (7*j) % 16;
		}
		tmp = d;
		d = c;
		c = b;
		b = left_rotate(a + f + k[j] + w[g], r[j]) + b;
		a = tmp;
	}
	ctx->h0 += a;
	ctx->h1 += b;
	ctx->h2 += c;
	ctx->h3 += d;
}
void md5_update(md5_t *ctx, const uint8_t *input, size_t input_size) {
	ctx->size += input_size;

	DEBUG_PRINTF("[DEBUG] UPDATE %lu bytes : %lu bits\n", input_size, input_size*8);

	for (size_t i = 0; i < input_size; i++) {
		ctx->tmp_buf[ctx->tmp_size] = input[i];
		ctx->tmp_size++;

		if (ctx->tmp_size >= 64) {
			uint32_t w[16];

#ifdef _DEBUG_
			DEBUG_PRINTF("[DEBUG] 512bits BLOCK:\n");
			for (int k = 0; k < 64; k++) {
				DEBUG_PRINTF("%02X ", ctx->tmp_buf[k]);
				if (k == 15 || k == 31 || k == 47 || k == 63)
					puts("");
			}
			DEBUG_PRINTF("\n[DEBUG] PERFORM STEP\n");
#endif
			for (int j = 0; j < 16; j++)
				w[j] = ((uint32_t *)ctx->tmp_buf)[j];
			md5_step(ctx, w);
			ctx->tmp_size = 0;
		}
	}
	DEBUG_PRINTF("[DEBUG] UPDATED, total size %lu bytes : %lu bits\n", ctx->size, ctx->size*8);
}
void md5_finish(md5_t *ctx, uint8_t hash[MD5_SIZE]) {

	uint8_t	last_input[8];
	size_t	padding_len = congruant_448_mod_512(ctx->size*8)/8;

	*((uint64_t *)last_input) = ctx->size*8;

	DEBUG_PRINTF("[DEBUG] FINISH, make %lu (bits) congruant 448 mod 512\n", ctx->size*8);
	DEBUG_PRINTF("[DEBUG] FINISH, padding length %lu bytes : %lu bits\n", padding_len, padding_len*8);
	DEBUG_PRINTF("[DEBUG] FINISH, original message length %lu bytes : %lu bits\n", ctx->size, ctx->size*8);

	md5_update(ctx, padding, padding_len);
	md5_update(ctx, last_input, sizeof(last_input));

	((uint32_t *)hash)[0] = ctx->h0;
	((uint32_t *)hash)[1] = ctx->h1;
	((uint32_t *)hash)[2] = ctx->h2;
	((uint32_t *)hash)[3] = ctx->h3;
}
