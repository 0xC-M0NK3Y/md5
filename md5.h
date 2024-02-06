#ifndef MD5_H__
# define MD5_H__

#include <stdint.h>

typedef struct md5 {
	uint64_t	size;
	uint32_t	h0;
	uint32_t	h1;
	uint32_t	h2;
	uint32_t	h3;
	uint8_t		tmp_buf[64];
	size_t		tmp_size;
}	md5_t;

#define MD5_SIZE (128/8)

void md5_init(md5_t *ctx);
void md5_update(md5_t *ctx, const uint8_t *input, size_t input_size);
void md5_finish(md5_t *ctx, uint8_t hash[MD5_SIZE]);

#endif /* md5.h */
