#include "stdio.h"

/*LINTLIBRARY*/

#ifndef lint
#ifndef NOID
static char	elsieid[] = "%W%";
#endif /* !defined lint */
#endif /* !defined NOID */

#include "ctype.h"
#include "string.h"

#ifdef __STDC__
extern char *	imalloc(int n);
extern void	ifree(char * p);
#else /* !defined __STDC__ */
extern char *	imalloc();
extern void	ifree();
#endif /* !defined __STDC__ */

char *
scheck(string, format)
char *	string;
char *	format;
{
	register char *	fbuf;
	register char *	fp;
	register char *	tp;
	register int	c;
	register char *	result;
	char		dummy;

	result = "";
	if (string == NULL || format == NULL)
		return result;
	fbuf = imalloc(2 * strlen(format) + 4);
	if (fbuf == NULL)
		return result;
	fp = format;
	tp = fbuf;
	while ((*tp++ = c = *fp++) != '\0') {
		if (c != '%')
			continue;
		if (*fp == '%') {
			*tp++ = *fp++;
			continue;
		}
		*tp++ = '*';
		if (*fp == '*')
			++fp;
		while (isascii(*fp) && isdigit(*fp))
			*tp++ = *fp++;
		if (*fp == 'l' || *fp == 'h')
			*tp++ = *fp++;
		else if (*fp == '[')
			do *tp++ = *fp++;
				while (*fp != '\0' && *fp != ']');
		if ((*tp++ = *fp++) == '\0')
			break;
	}
	*(tp - 1) = '%';
	*tp++ = 'c';
	*tp = '\0';
	if (sscanf(string, fbuf, &dummy) != 1)
		result = format;
	ifree(fbuf);
	return result;
}
