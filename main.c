#ifndef AUXILIARY
#define AUXILIARY
#include"auxiliary.h"
#endif
#include"compress.h"
#include"decompress.h"
#include"mirror.h"
#include"overlay.h"

int main(int argc, char *argv[])
{
	int factor;
	/*compress*/
	if(strcmp(argv[1], "-c") == 0)
	{
		factor = atoi(argv[2]);
		compress(argv[3], argv[4], factor);
	}
	/*decompress*/
	else if(strcmp(argv[1], "-d") == 0)
		decompress(argv[2], argv[3]);
	/*mirror*/
	else if(strcmp(argv[1], "-m") == 0)
	{
		factor = atoi(argv[3]);
		mirror(argv[4], argv[5], argv[2][0], factor);
	}
	/*overlay*/
	else if(strcmp(argv[1], "-o") == 0)
	{
		factor = atoi(argv[2]);
		overlay(argv[3], argv[4], argv[5], factor);
	}
	else
	{
		printf("Invalid arguments!\n");
		exit(1);
	}
	return 0;
}
