#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "md5.h"

void print_hash(uint8_t *p){
    for(unsigned int i = 0; i < 16; ++i){
        printf("%02x", p[i]);
    }
    printf("\n");
}

int main(int argc, char **argv) {

	uint8_t	hash[MD5_SIZE];
	md5_t	ctx;

	if (argc != 2) {
		printf("Usage %s <string>\n", argv[0]);
		return 1;
	}
	md5_init(&ctx);
	md5_update(&ctx, (const uint8_t *)argv[1], strlen(argv[1]));
	md5_finish(&ctx, hash);

	print_hash(hash);

	return 0;
}
