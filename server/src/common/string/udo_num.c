#include "udo_num.h"


unsigned char udo_number_char(unsigned char num)
{
	unsigned char base = 0;
	unsigned char add_base=10;
	if (num <= 'z'&&num >= 'a')
	{
		base = 'a';

	}
	else if (num <= 'Z'&&num >= 'A')
	{
		base = 'A';
	}
	else
	{
		add_base = 0;
		base = '0';
	}
	return num - base + add_base;
}

