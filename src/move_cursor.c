/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   move_cursor.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 17:38:37 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/04 16:46:16 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

void	cursor_move_row(t_line *line, int c)
{
	if (c == DOWN_KEY)
	{
		if (line->inputrow == line->total_rows)
			return ;
		line->inputrow++;
		line->cursor.row++;
		if (line->inputrow * line->max.col + line->cursor.col - line->promptlen
				> vecstr_len(&line->cmd))
			cursor_end(line);
		return ;
	}
	if (c == UP_KEY)
	{
		if (line->inputrow == 0)
			return ;
		line->cursor.row--;
		line->inputrow--;
		if (line->inputrow * line->max.col + line->cursor.col < line->promptlen)
			cursor_home(line);
		return ;
	}
}

void	cursor_home(t_line *line)
{
	line->cursor.col = line->promptlen;
	line->cursor.row = line->cursor.row - line->inputrow;
	line->inputrow = 0;
}

void	cursor_end(t_line *line)
{
	line->cursor.row = line->cursor.row - line->inputrow + line->total_rows;
	line->cursor.col = (vecstr_len(&line->cmd) + line->promptlen)
		% line->max.col;
	line->inputrow = line->total_rows;
}

/*
** cursor_left() moves the cursor one position to the left
*/

void	cursor_left(t_line *line)
{
	if (line->inputrow == 0 && line->cursor.col == line->promptlen)
		return ;
	if (line->cursor.col == 0)
	{
		line->inputrow--;
		line->cursor.col = line->max.col - 1;
		line->cursor.row--;
	}
	else
		line->cursor.col--;
}

void	cursor_right(t_line *line)
{
	if (line->inputrow * line->max.col + (line->cursor.col + 1) >
			vecstr_len(&line->cmd) + line->promptlen)
		return ;
	line->cursor.col++;
	if (line->cursor.col >= line->max.col)
	{
		line->cursor.col = 0;
		line->cursor.row++;
		line->inputrow++;
	}
}
