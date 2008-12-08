void kmain( void* mbd, unsigned int magic )
{
	/* Write your kernel here. Example: */
	mbd = 0;
	magic = 1;
	unsigned char *videoram = (unsigned char *) 0xb8000;
	videoram[0] = 66; /* character 'A' */
	videoram[1] = 0x07; /* forground, background color. */
}
