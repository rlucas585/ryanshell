#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>

#define STDIN 0

/* void	init_term(struct termios *term) */
/* { */
/* 	tcgetattr(STDIN, term); */
/* 	#<{(| term->c_lflag &= ~(ECHO | ICANON); |)}># */
/* 	term->c_cc[VMIN] = 1; */
/* 	term->c_cc[VTIME] = 0; */
/* 	cfsetispeed(term, B9600); */
/* 	cfsetospeed(term, B9600); */
/* 	tcsetattr(STDIN, TCSAFLUSH, term); */
/* } */
/*  */
int main(void)
{
	int			i;
	/* char		**args; */
	/* struct termios	term; */
    /*  */
	/* init_term(&term); */
	/* args = (char **)malloc(sizeof(char *) * 2); */
	/* execve("/usr/bin/R", args, NULL); */

	i = -257;
	printf("%d %% 256 = %d\n", i, i % 256);
	return (0);
}

/* /usr/bin/R */
