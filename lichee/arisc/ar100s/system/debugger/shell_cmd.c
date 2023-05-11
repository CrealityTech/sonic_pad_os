#include "include.h"

#define     DEBUG_SHELL 0

#define     MAX_CMD_LEN         64      /* max command length */
#define     MAX_CMD_ARGS        5       /* max arguments nr for one command */

/* Build in commands prototype */
typedef int (*builtin_func_ptr)(const char **);

struct builtincmd {
	const char *name;
	builtin_func_ptr builtinfunc;
};

static void parseargs(char *argstr, s32 *argc_p, char **argv);
static builtin_func_ptr inbuilt(const char *s);
static s32 execute_command(const char **argv);
static void execute_one_command(char *buf);
static int do_echo(const char **args);
static int do_set_debug_level(const char **args);
static int do_set_baudrate(const char **args);
static int do_cat(const char **args);
static int do_help(const char **args);
static int do_print(const char **args);

/* builtin commands table, shell execute commands */
const struct builtincmd Commands_Table[] = {
	{ "echo", do_echo},
	{ "set_debug_level", do_set_debug_level},
	{ "set_baudrate", do_set_baudrate},
	{ "cat", do_cat},
	{ "print", do_print},
	{ "help", do_help},
	{ NULL, NULL},
};

/* parse token state */
enum parseState {
	PS_WHITESPACE,  /* space find */
	PS_TOKEN,       /* valid parameters find */
	PS_STRING,
	PS_ESCAPE
};

char cmd_buf[MAX_CMD_LEN];

/*
 * getcmd - Get the command input
 * @parg: cmd_buf--Input command string
 */
s32 getcmd(void *parg)
{
	char c;
	u32 count;
	static u32 curpos;  /* current position - index __s32o cmd_buf */

/*
* two scenes:
* 1. input character one by one, the 'count' is 1, 'c' is the input.
* 2. input chatacters burstly, 'count' is number of characters, 'c' is the \
*    last word, usualy is '\r', without '0x08', '0x7E'.
*/
	count = debugger_get(cmd_buf + curpos);
	c = *(cmd_buf + curpos + count - 1);
#if DEBUG_SHELL
	LOG("uart received:%x\n", c);
	do {
		int i;
		LOG("CMDBUF:");
		for (i = 0; i < curpos; i++)
			LOG("%x", *(cmd_buf + i));
		LOG("\n");
	} while (0);
	return OK;
#endif
	switch (c) {
	/* backspace */
	case 0x08:
			/* we're not at the beginning of the line */
			if (count && curpos--) { /* not & */

				debugger_putc(0x08);    /* go backwards */
				debugger_putc(' ');     /* overwrite the char */
				debugger_putc(0x08);    /* go back again */
			}
			cmd_buf[curpos] = '\0';
			break;
	/* delete */
	case 0x7E:
			debugger_putc('\r');
			for (count = 0; count < curpos; count++)
				debugger_putc(' ');
			debugger_putc('\r');
			curpos = 0;
			break;
	/*end of command*/
	case '\r':
	case '\n':
	case '\0':
			/* send message(ucos used) or execute command */
			debugger_putc('\r');
			debugger_putc('\n');
			cmd_buf[curpos] = 0;
			if (curpos > 1)
				execute_one_command(cmd_buf);
			debugger_putc('$');
			debugger_putc(' ');

			curpos = 0;
			break;
	default:
			/*
			 * echo it back out to the screen,
			 * and adjust buffer index
			 */
			if (curpos + count < (MAX_CMD_LEN - 1))
				for (; count > 0; count--)
					debugger_putc(*(cmd_buf + curpos++));
			break;
		}

	return OK;
}

/*
 * execute_one_command - parse and execute a string
 * @buf: cmd_buf--execute string pointer
 */
static void execute_one_command(char *buf)
{
	char *argv[MAX_CMD_ARGS]; /* only used for save address, not data */
	s32 i;
	s32 argc;

	/* init argv pointer table */
	for (i = 0; i < MAX_CMD_ARGS; i++)
		argv[i] = NULL;

	/* parse input string */
	parseargs(buf, &argc, argv);
	if (argc > 0)
		execute_command((const char **)argv);
}

/*
 * execute_command - execute one command
 * @agrv: execute command parater pointer
 * @returns: Comand execute function pointer except
 */
static s32 execute_command(const char **argv)
{
	builtin_func_ptr bltin = NULL;
	int DoResult = FAIL; /* defult failed, is not a command */

	if (NULL == argv)
		goto Error_Command;

	/* find execute command */
	bltin = inbuilt(argv[0]);
	if (bltin) {
		/* execute builtin command */
		DoResult = bltin(argv);

		return DoResult;
	}

Error_Command:
	ERR("\"%s\" is not a command, type \"help\" for more information!\n", argv[0]);

	return DoResult;
}

/*
 * parseargs - Parse user command line
 * @argstr: parameters
 * @agrgc_p: parameters
 * @argv: parameters
 */
static void parseargs(char *argstr, s32 *argc_p, char **argv)
{
	char c;
	s32 argc = 0;
	enum parseState stackedState = PS_WHITESPACE;
	enum parseState lastState = PS_WHITESPACE;

	/* tokenize the argstr */
	c = *argstr; /* optimize read order */
	while ((c != 0) && (argc <= MAX_CMD_ARGS)) {
		if (c == ';' && lastState != PS_STRING && lastState != PS_ESCAPE)
			break;

		if (lastState == PS_ESCAPE) {
			lastState = stackedState;
		} else if (lastState == PS_STRING) {
			if (c == '"') {
				lastState = PS_WHITESPACE;
				*argstr = 0;
			} else
				lastState = PS_STRING;
		} else if ((c == ' ') || (c == '\r') || (c == '\n') || (c == '\t')) {
			/* whitespace character or 'enter'*/
			*argstr = 0;
			lastState = PS_WHITESPACE;
		} else if (c == '"') {
			lastState = PS_STRING;
			*argstr++ = 0;
			argv[argc++] = argstr;
		} else { /* token */
			if (lastState == PS_WHITESPACE)
				argv[argc++] = argstr; /* address save */
			lastState = PS_TOKEN;
		}
		c = *(++argstr);
	}
	argv[argc] = NULL;
	if (argc_p != NULL)
		*argc_p = argc;

	if (*argstr == ';')
		*argstr++ = '\0';
}

/*
 * inbuilt - find one command execute function
 * @s: name of command
 * @return: Comand execute function pointer except
 */
static builtin_func_ptr inbuilt(const char *s)
{
	const struct builtincmd *bp;

	/* first seach command table */
	for (bp = Commands_Table; bp->name; bp++) {
		if (0 == strcmp(bp->name, s))
			return bp->builtinfunc;
	}

	return NULL;
}

/*
 * do_echo - output the input
 * @args: parameters
 */
static int do_echo(const char **args)
{
	u32 add;
	u32 wda;
	/* input arguments check */
	if ((NULL == args[1]) || (NULL == args[3]) || ('>' != *args[2]) || \
	    (NULL != args[4])) {
		ERR("invalid argument!\n");
		LOG("echo 0xdata > 0xaddr\n");

		return -EINVAL;
	}
	wda = hstr2int(args[1], strlen(args[1]));
	add = hstr2int(args[3], strlen(args[3]));
	if (add & 0x03) {
		LOG("address should be align 4B\n");

		return -EINVAL;
	}
	writel(wda, add);
	LOG("write: 0x%x to add: 0x%x\n", wda, add);

	return OK;
}

/*
 * do_cat - read a register
 * @args: parameters
 */
static int do_cat(const char **args)
{
	u32 *add;
	u32 len;
	u32 i;

	/* input arguments check */
	if ((NULL == args[1]) || (NULL != args[3])) {
		ERR("invalid argument!\n");
		LOG("cat addr [len]\n");

		return -EINVAL;
	}
	add = (u32 *)hstr2int(args[1], strlen(args[1]));
	if ((u32)add & 0x03) {
		LOG("address should be align 4B\n");

		return -EINVAL;
	}
	if (NULL != args[2]) {
		len = dstr2int(args[2], strlen(args[2]));
		if (!len)
			len = hstr2int(args[2], strlen(args[2]));
		LOG("cat start:%x, length:%d\n[0x%8x]", add, len--, add);
		for (i = 0; i < len; i++) {
			LOG("0x%8x  ", readl(add++));
				if ((i & 3) == 3)
					LOG("\n[0x%8x]", add);
		}
		LOG("0x%8x  \n", readl(add));

		return OK;
	}
	LOG("address: 0x%8x is: 0x%8x\n", add, readl(add));

	return OK;
}

/*
 * do_set_debug_level - output the input
 * @args: parameters
 */
static int do_set_debug_level(const char **args)
{
	u32 debug_level = 0;

	/* input arguments check */
	if ((NULL == args[1]) || (NULL != args[2])) {
		ERR("invalid argument!\n");
		LOG("set_debug_level 3\n");

		return -EINVAL;
	}
	debug_level =  dstr2int(args[1], strlen(args[1]));
	LOG("the debug level set to be %d\n", debug_level);
	set_debug_level(debug_level);

	return OK;
}

/*
 * do_set_baudrate - output the input
 * @args: parameters
 */
static int do_set_baudrate(const char **args)
{
	u32 baud = 115200;

	/* input arguments check */
	if ((NULL == args[1]) || (NULL != args[2])) {
		ERR("invalid argument!\n");

		return -EINVAL;
	}
	baud = dstr2int(args[1], strlen(args[1]));
	uart_set_baudrate(baud);
	LOG("the baudrate set to be %d\n", baud);

	return OK;
}

/*
 * do_print - print buf, usually __log_buf
 * @args: parameters
 */
static int do_print(const char **args)
{
	char *add;
	u32 len;

	/* input arguments check */
	if ((NULL == args[1]) || (NULL != args[3])) {
		ERR("invalid argument!\n");

		return -EINVAL;
	}
	add = (char *)hstr2int(args[1], strlen(args[1]));
	if (NULL != args[2]) {
		len = dstr2int(args[2], strlen(args[2]));
		if (!len)
			len = hstr2int(args[2], strlen(args[2]));
		LOG("print start:%8x, length:%d\n", add, len--);
		cpucfg_set_little_endian_address((void *)add, (void *)((int)(add + len + (1<<10)) & ~0x01FF));
		debugger_puts(add);
		time_mdelay(10);
		cpucfg_remove_little_endian_address(add, 0);

		return OK;
	}

	return OK;
}

/*
 * do_help - show all commands
 * @args: parameters
 */
static int do_help(const char **args)
{
	const struct builtincmd *bp;

	LOG("support commands:\n");
	/* first seach command table */
	for (bp = Commands_Table; bp->name; bp++)
		LOG("\"%s\"\n", bp->name);

	return OK;
}

