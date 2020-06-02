/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   move_cursor2.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/30 16:37:19 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/02 09:59:30 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

static size_t	find_index(t_line *line, size_t index, int dir)
{
	size_t		index2;
	int			charfound;

	if (dir == LEFT)
		index2 = 1;
	else
		index2 = -1;
	charfound = 0;
	while (1)
	{
		if (vecstr_val(&line->cmd, index - index2) == ' ' && charfound == 1)
			break ;
		if (charfound == 0 && vecstr_val(&line->cmd, index - index2) != ' ')
			charfound = 1;
		if ((index - index2 == 0 && dir == LEFT) ||
				(index - index2 == vecstr_len(&line->cmd) && dir == RIGHT))
		{
			if (dir == RIGHT)
				return (index - index2);
			return (index - index2 - dir + 1);
		}
		index2 = index2 + dir;
	}
	return (index - index2 + 1);
}

static void	cursor_word_left(t_line *line)
{
	size_t	index;
	size_t	original_row;

	index = line->inputrow * line->max.col + line->cursor.col - line->promptlen;
	original_row = line->inputrow;
	if (line->inputrow == 0 && line->cursor.col == line->promptlen)
		return ;
	index = find_index(line, index, LEFT);
	line->cursor.col = (index + line->promptlen) % line->max.col;
	line->inputrow = (index + line->promptlen) / line->max.col;
	line->cursor.row += line->inputrow - original_row;
}

static void	cursor_word_right(t_line *line)
{
	size_t	index;
	size_t	original_row;

	index = line->inputrow * line->max.col + line->cursor.col - line->promptlen;
	original_row = line->inputrow;
	if (line->inputrow * line->max.col + line->cursor.col - line->promptlen >=
			vecstr_len(&line->cmd))
		return ;
	index = find_index(line, index, RIGHT);
	line->cursor.col = (index + line->promptlen) % line->max.col;
	line->inputrow = (index + line->promptlen) / line->max.col;
	line->cursor.row += line->inputrow - original_row;
}

void		cursor_move_word(t_line *line, int c)
{
	if (c == LEFT_KEY)
		cursor_word_left(line);
	if (c == RIGHT_KEY)
		cursor_word_right(line);
}
