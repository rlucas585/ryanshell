/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   add_char.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 17:34:51 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/13 14:25:12 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>
#include <termcap.h>
#include <termios.h>
#include <msh_term.h>

/*
** add_char() will reallocate a new length for the command string, then
** add the newly input character to it.
*/

static void	scroll_lines(t_line *line)
{
	int			tmp;
	int			tmp2;

	if (line->total_rows - line->inputrow + line->cursor.row >= line->max.row)
	{
		tmp = line->cursor.row - 1;
		tmp2 = line->cursor.col;
		line->cursor.row = line->max.row - 1;
		line->cursor.col = 0;
		termcmd(MOVE_COLROW, line->cursor.col, line->cursor.row, 1);
		termcmd(SCROLL_LINE, 0, 0, 1);
		line->cursor.row = tmp;
		line->cursor.col = tmp2;
		termcmd(MOVE_COLROW, line->cursor.col, line->cursor.row, 1);
	}
}

int			add_char(t_line *line, char c)
{
	size_t		row;
	size_t		index;

	if (c == '\n')
		c = ' ';
	line->total_rows = (vecstr_len(&line->cmd) + line->promptlen)
		/ line->max.col;
	scroll_lines(line);
	if (vecstr_insert_c(&line->cmd, line->inputrow * line->max.col
				+ line->cursor.col - line->promptlen, c))
		return (-1);
	termcmd(INSERT_START, 0, 0, 1);
	ft_printf("%c", c);
	termcmd(INSERT_END, 0, 0, 1);
	row = line->inputrow;
	while (row < line->total_rows)
	{
		termcmd(MOVE_COLROW, 0, line->cursor.row - line->inputrow + row + 1, 1);
		index = row * line->max.col + line->max.col - line->promptlen;
		if (index > vecstr_len(&line->cmd))
			break ;
		/* if (vecstr_get(&line->cmd)[index] >= 32 && vecstr_get(&line->cmd)[index] */
		/* 		<= 126) */
		/* { */
			termcmd(INSERT_START, 0, 0, 1);
			ft_printf("%c", vecstr_val(&line->cmd, index));
			termcmd(INSERT_END, 0, 0, 1);
		/* } */
		row++;
	}
	line->cursor.col++;
	if (line->cursor.col >= line->max.col)
	{
		line->cursor.col = 0;
		line->cursor.row++;
		line->inputrow++;
	}
	return (0);
}
