/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   r_execute.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/19 14:22:15 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/19 14:48:59 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

void	assign_command_value(t_ryantok tok, t_ryancmd *cmd_struct)
{
	if (tok.type == COMMAND)
		cmd_struct->command = tok.value;
}

int		r_execute(t_ryantok *toks)
{
	t_ryancmd	cmd_struct;
	size_t		i;
	int			current_cmd;

	i = 0;
	while (toks[i].value)
	{
		current_cmd = toks[i].cmd_num;
		while (toks[i].value)
		{
			assign_command_value(toks[i], &cmd_struct);
		}
		(void)current_cmd;
	}
	return 69;
}
