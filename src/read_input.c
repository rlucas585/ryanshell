/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   read_input.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/16 10:50:53 by rlucas        #+#    #+#                 */
/*   Updated: 2020/04/30 13:41:02 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "minishell.h"
#include <termcap.h>
#include <termios.h>
#include "msh_term.h"

static void	refresh_cursor(t_line *line)
{
	if (line->cursor.row >= line->max.row)
	{
		line->cursor.row -= 1;
		termcmd(SCROLL_LINE, 0, 0, 1);
	}
	termcmd(MOVE_COLROW, line->cursor.col, line->cursor.row, 1);
}

static int	get_row(void)
{
	char		buf[8];
	int			row;

	ft_printf_fd(STDOUT, "\033[6n");
	read(STDIN, buf, 8);
	row = ft_atoi(buf + 2);
	return (row);
}

static int	initialize_line_editor(t_line *line)
{
	line->cmd = (char *)ft_calloc(1, 100);
	if (!line->cmd)
		return (-1);
	line->cursor.col = line->promptlen;
	line->cursor.row = get_row() - 1;
	line->total_rows = 0;
	line->alloced_cmd = 100;
	line->cmd_len = 0;
	line->inputrow = 0;
	line->escmode = 0;
	return (0);
}

int		read_input(t_msh *prog)
{
	t_line		*line;
	char		buf[6];
	int			send;

	line = &prog->line;
	if (initialize_line_editor(line) == -1)
		return (-1);
	ft_printf("%s", line->prompt);
	send = 0;
	refresh_cursor(line);
	while (!send)
	{
		ft_bzero(buf, 6);
		read(STDIN, buf, 6);
		send = handle_input(line, buf);
		if (send < 0)
			error_exit(prog, MEM_FAIL);
		if (send == CTRL_D)
			std_exit(prog);
		refresh_cursor(line);
	}
	line->cursor.row = line->cursor.row + line->total_rows + 1;
	line->cursor.col = 0;
	refresh_cursor(line);
	return (1);
}
