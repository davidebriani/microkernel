#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct initrd_header
{
	unsigned char magic;
	char name[64];
	unsigned int offset;
	unsigned int length;
};

int main(int argc, char **argv)
{

	int i, nheaders;
	nheaders = argc - 1;
	struct initrd_header headers[64];
	printf("size of header: %d\n", sizeof(struct initrd_header));
	unsigned int off = sizeof(struct initrd_header) * 64 + sizeof(int);

	for (i = 1; i <= nheaders; i++)
	{
		printf("writing file %s -> %s at 0x%x\n", argv[i], argv[i], off);
		strcpy(headers[i-1].name, argv[i]);
		headers[i-1].offset = off;
		FILE *stream = fopen(argv[i], "r");
		if (stream == 0)
		{
			printf("Error: file not found: %s\n", argv[i]);
			return 1;
		}
		fseek(stream, 0, SEEK_END);
		headers[i-1].length = ftell(stream);
		off += headers[i-1].length;
		fclose(stream);
		headers[i-1].magic = 0xBF;
	}

	FILE *wstream = fopen("./initrd.img", "w");
	unsigned char *data = (unsigned char *)malloc(off);
	fwrite(&nheaders, sizeof(int), 1, wstream);
	fwrite(headers, sizeof(struct initrd_header), 64, wstream);

	for (i = 1; i <= nheaders; i++)
	{
		FILE *stream = fopen(argv[i], "r");
		unsigned char *buf = (unsigned char *)malloc(headers[i-1].length);
		fread(buf, 1, headers[i-1].length, stream);
		fwrite(buf, 1, headers[i-1].length, wstream);
		fclose(stream);
		free(buf);
	}

	fclose(wstream);
	free(data);

	return 0;
}
