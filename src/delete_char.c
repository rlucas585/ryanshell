/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   delete_char.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 17:37:30 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/01 21:46:18 by rlucas        ########   odam.nl         */
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
		if (vecstr_val(&line->cmd, index - index2) != ' ' && charfound == 0)
			charfound = 1;
		if (vecstr_val(&line->cmd, index - index2) == ' ' && charfound == 1)
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

	if (line->inputrow == 0 && line->cursor.col == line->promptlen)
		return (0);
	if (vecstr_slice(&line->cmd, vecstr_len(&line->cmd) - 1,
				vecstr_len(&line->cmd)))
		return (-1); // Mem fail - deal with later
	line->total_rows = (vecstr_len(&line->cmd) + line->promptlen) /
		line->max.col;
	if (line->cursor.col == 0)
	{
		line->inputrow--;
		line->cursor.col = line->max.col - 1;
		line->cursor.row--;
	}
	else
		line->cursor.col--;
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
		if (index > vecstr_len(&line->cmd))
			break ;
		termcmd(INSERT_START, 0, 0, 1);
		ft_printf("%c", vecstr_val(&line->cmd, index));
		termcmd(INSERT_END, 0, 0, 1);
		row++;
	}
	return (0);
}
