/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   terminal.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 17:17:50 by rlucas        #+#    #+#                 */
/*   Updated: 2020/04/29 17:57:09 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <stdlib.h>
#include <termcap.h>
#include <termios.h>
#include <libft.h>

static int	init_caps(t_line *line)
{
	line->termtype = getenv("TERM");
	if (!line->termtype)
		return (-1);
	line->cap_table = malloc(sizeof(char) * (2048));
	if (!line->cap_table)
		return (-1);
	if (tgetent(line->cap_table, line->termtype) != 1)
		return (-1);
	g_termbuff = &line->cap_table;
	return (0);
}

int			init_term(struct termios *term)
{
	if (!isatty(STDIN) || tcgetattr(STDIN, term) < 0)
		return (1);
	/* term->c_iflag &= ~(INLCR) */
	term->c_iflag &= ~(IMAXBEL); //change certain attributes in termios 1/4
	/* INLCR - When \n is input, program recieves it as \r. */
	/* IMAXBEL - If input buffer is filled, send a BEL to the terminal. */
	term->c_lflag &= ~(ECHO | ICANON); //change certain attributes in termios 2/4
	/* ECHO - display input characters. */
	/* ICANON - NON-CANONICAL Input. This is required to allow our program to */
	/* 	control input editing facilities. */
	term->c_cc[VMIN] = 1; //change certain attributes in termios 3/4
	term->c_cc[VTIME] = 0; //change certain attributes in termios  4/4

	/* The change occurs after all output written to the file descriptor has been */
	/* 	transmitted, and all input so far received but not read is discarded */
	/* 	before the change is made */
	if (cfsetispeed(term, B9600) < 0 || cfsetospeed(term, B9600) < 0 ||
		tcsetattr(STDIN, TCSAFLUSH, term) < 0)
		return (1);
	return (0);
}

void	init_readline(t_msh *prog)
{
	prog->line = (t_line){0};
	if (init_term(&prog->line.term) || init_caps(&prog->line) == -1)
		error_exit(prog, CAP_FAIL);
	prog->line.prompt = prompt(prog, &prog->line);
	prog->line.max.col = tgetnum("co");
	prog->line.max.row = tgetnum("li");
}

void	termcmd(char *command, int p1, int p2, int lines_affected)
{
	tputs(tgoto(tgetstr(command, NULL), p1, p2),
		lines_affected, &ft_putchar);
}
