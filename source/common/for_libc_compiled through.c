//


#if 0

int *read(int *p)
{
	asm("nop");
	asm("break 1,1");
	return 0;
}

int *write(int *p)
{
	asm("nop");
	asm("break 1,1");
	return 0;
}

int *open(int *p)
{
	asm("nop");
	asm("break 1,1");
	return 0;
}

int *close(int *p)
{
	asm("nop");
	asm("break 1,1");
	return 0;
}

int lseek(int *p)
{
	asm("nop");
	//asm("break 1,1");
	return 0;
}

int sbrk(int *p)
{
	asm("nop");
	asm("break 1,1");
	return 0;
}

int times(int *p)
{
	asm("nop");
	asm("break 1,1");
}

int unlink(int *p)
{
	asm("nop");
	asm("break 1,1");
}

#endif
