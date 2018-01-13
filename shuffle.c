#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>


/* Linear Congruent Generator */
unsigned int lcg() {
	static unsigned int x = 0;

	x = 1664525L * x + 1013904223L;
	return x;
}

/* Shuffles memory starting at buf and extending len bytes,
 * at the granularity of card_size.
 */
void memshuffle(char *buf, int len, int card_size) {
	void *swap;
	unsigned int x, i, r, N;

	/* Checking the len is a multiple of card_size */
	if(len % card_size != 0) {
		fprintf(stderr, "memshuffle: len must be a multip of card_size");
		exit(EXIT_FAILURE);
	}

	N = len / card_size;
	swap = malloc(card_size);

	for(i = 0; i < N; i++) {
		r = i+(unsigned int)(1.0*(N-i)*lcg()/429467296LL);

		memcpy(swap, buf + i*card_size, card_size);
		memcpy(buf + i*card_size, buf + r*card_size, card_size);
		memcpy(buf + r*card_size, swap, card_size);
	}
}

int main(int argc, char **argv) {
	int fd, card_size;
	ssize_t len;
	void *buf;
	char *filename;

	if(argc < 3) {
		fprintf(stderr, "usage: shuffle filename card_size\n");
		exit(EXIT_FAILURE);
	}

	filename = argv[1];
	card_size = strol(argv[2], NULL, 10);

	fd = open(filename, O_RDWR);

	// get size of the whole file
	len = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	buf = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_FILE|MAP_SHARED, fd, 0);
	if(buf == (void *)-1){
		fprintf(stderr, "mmap failed.\n");
		exit(EXIT_FAILURE);
	}

	memshuffle(buf, len, card_size);
	munmap(buf, len);
	close(fd);
	
	return EXIT_SUCCESS;
}
