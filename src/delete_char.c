/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   delete_char.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 17:37:30 by rlucas        #+#    #+#                 */
/*   Updated: 2020/04/30 14:25:17 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>
#include <termcap.h>
#include <termios.h>
#include <msh_term.h>

/*
** delete_char() will reallocate a new length for the command string, and
** remove the last character from it.
*/

static void	remove_char(t_line *line)
{
	size_t		index;

	index = line->inputrow * line->max.col + line->cursor.col - line->promptlen;
	ft_memmove(line->cmd + index, line->cmd + index + 1, line->alloced_cmd -
			index - 1);
}

int			esc_delete(t_line *line)
{
	size_t		index;
	size_t		index2;
	int			charfound;

	// Check if our cursor is already at the very beginning.
	if (line->inputrow == 0 && line->cursor.col == line->promptlen)
		return (0);

	index = line->inputrow * line->max.col + line->cursor.col - line->promptlen;
	index2 = 1;
	charfound = 0;
	while (1)
	{
		if (index - index2 == 0)
		{
			index2++;
			break ;
		}
		if (line->cmd[index - index2] != ' ' && charfound == 0)
			charfound = 1;
		if (line->cmd[index - index2] == ' ' && charfound == 1)
			break ;
		index2++;
	}
	while (index2 > 1)
	{
		if (delete_char(line) == -1)
			return (-1);
		index2--;
	}
	return (0);
}

int			delete_char(t_line *line)
{
	size_t		row;
	size_t		index;

	// Check if our cursor is already at the very beginning.
	if (line->inputrow == 0 && line->cursor.col == line->promptlen)
		return (0);

	// Decrease length of command.
	line->cmd_len--;

	// Assign total rows that command spans
	line->total_rows = (line->cmd_len + line->promptlen) / line->max.col;

	// Move cursor, to previous row if at start of row.
	if (line->cursor.col == 0)
	{
		line->inputrow--;
		line->cursor.col = line->max.col - 1;
		line->cursor.row--;
	}
	else
		line->cursor.col--;

	// Internal component - remove character from stored array.
	remove_char(line);

	// Display component - Delete character from terminal display.
	termcmd(MOVE_COLROW, line->cursor.col, line->cursor.row, 1);
	termcmd(DELETE_START, 0, 0, 1);
	termcmd(DELETE_CHAR, 0, 0, 1);
	termcmd(DELETE_END, 0, 0, 1);

	row = line->inputrow;
	while (row < line->total_rows)
	{
		termcmd(MOVE_COLROW, 0, line->cursor.row - line->inputrow + row + 1, 1);
		termcmd(DELETE_START, 0, 0, 1);
		termcmd(DELETE_CHAR, 0, 0, 1);
		termcmd(DELETE_END, 0, 0, 1);
		termcmd(MOVE_COLROW, line->max.col, line->cursor.row - line->inputrow +
				row, 1);
		index = row * line->max.col + line->max.col - line->promptlen - 1;
		if (index > line->cmd_len)
			break ;
		termcmd(INSERT_START, 0, 0, 1);
		ft_printf("%c", line->cmd[index]);
		termcmd(INSERT_END, 0, 0, 1);
		row++;
	}
	return (0);
}
