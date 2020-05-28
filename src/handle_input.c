/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_input.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 17:58:45 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/04 12:13:20 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>
#include <termcap.h>
#include <termios.h>
#include <msh_term.h>

/*
** Create a jumptable for directing input.
*/

static void	create_jumptable(t_inputf func_table[128]){
	size_t			i;

	func_table[0] = &send_EOF;
	func_table[CTRL_D] = &send_EOF;
	func_table[CTRL_L] = &clear_screen;
	func_table[CTRL_U] = &clear_input;
	func_table[NEWLINE] = &send_input;
	func_table[ESC] = &special_command;
	i = 32;
	while (i <= 126)
	{
		func_table[i] = &char_input;
		i++;
	}
	func_table[i] = &backspace;
}

static int	is_valid_input(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	if (c == NEWLINE)
		return (1);
	if (c == ESC)
		return (1);
	if (c == CTRL_D)
		return (1);
	if (c == DEL)
		return (1);
	if (c == CTRL_U)
		return (1);
	if (c == CTRL_L)
		return (1);
	return (0);
}

int			handle_input(t_line *line, char buf[6])
{
	static t_inputf	func_table[128];
	t_inputf		func;
	int				c;

	/*
	** For testing of key input values
	*/

	/* (void)line; */
	/* (void)buf; */
	/* ft_printf("entered key = {%d,%d,%d,%d,%d,%d}\n\n", buf[0], buf[1], buf[2], */
	/* 		buf[3], buf[4], buf[5]); */
	/* return (0); */

	c = buf[0];
	if (line->escmode == 1 && c != DEL)
	{
		line->escmode = 0;
		return (0);
	}
	if (!is_valid_input(c))
		return (0);
	if (func_table[0] == 0)
		create_jumptable(func_table);
	func = func_table[c];
	return (func(line, buf));
}
