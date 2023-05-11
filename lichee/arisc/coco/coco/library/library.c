
#include "library_i.h"

static const char digit_string[] = "0123456789ABCDEF";

char *itoa(int value, char *string, int radix)
{
	char stack[16];
	int  negative = 0;          //defualt is positive value
	int  i;
	int  j;

	if (value == 0) {
		//zero
		string[0] = '0';
		string[1] = '\0';
		return string;
	}

	if (value < 0) {
		//'value' is negative, convert to postive first
		negative = 1;
		value = -value ;
	}

	for (i = 0; value > 0; ++i) {
		// characters in reverse order are put in 'stack'.
		stack[i] = digit_string[value % radix];
		value /= radix;
	}

	//restore reversed order result to user string
	j = 0;
	if (negative) {
		//add sign at first charset.
		string[j++] = '-';
	}
	for (--i; i >= 0; --i, ++j) {
		string[j] = stack[i];
	}
	//must end with '\0'.
	string[j] = '\0';

	return string;
}

char *utoa(unsigned int value, char *string, int radix)
{
	char stack[16];
	int  i;
	int  j;

	if (value == 0) {
		//zero
		string[0] = '0';
		string[1] = '\0';
		return string;
	}

	for (i = 0; value > 0; ++i) {
		// characters in reverse order are put in 'stack'.
		stack[i] = digit_string[value % radix];
		value /= radix;
	}

	//restore reversed order result to user string
	for (--i, j = 0; i >= 0; --i, ++j) {
		string[j] = stack[i];
	}
	//must end with '\0'.
	string[j] = '\0';

	return string;
}

void hexdump(const char* name, char* base, u32 len)
{
	u32 i;
	printk("%s :\n", name);
	for (i=0; i<len; i+=4) {
		if (!(i&0xf))
		printk("\n0x%8p : ", base + i);
		printk("%8x ", readl(base + i));
		}
	printk("\n");
}

unsigned int hstr2int(const char *str, unsigned int len)
{
	unsigned int i = 0, tmp = 0;

	if ((str[0] == '0') && (str[1] == 'x')) {
		str += 2;
		len -= 2;
	}
	while(len--) {
		if(str[i] >= '0' && str[i] <= '9')
			tmp = tmp * 16 + (str[i] - '0');
		else if(str[i] >= 'A' && str[i] <= 'F')
			tmp = tmp *16 + (str[i] - 'A') + 10;
		else if(str[i] >= 'a' && str[i] <= 'f')
			tmp = tmp *16 + (str[i] - 'a') + 10;
		else
			break;
		i++;
	}

	return tmp;
}

unsigned int dstr2int(const char *str, unsigned int len)
{
	unsigned int i = 0, tmp = 0;

	while (len--) {
		if(str[i] >= '0' && str[i] <= '9')
			tmp = tmp * 10 + (str[i] - '0');
		else
			break;
		i++;
	}

	return tmp;
}

