/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   commands.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 19:22:44 by tbruinem      #+#    #+#                 */
/*   Updated: 2020/05/24 14:37:41 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include "libft.h"
#include <stdio.h>

void	print_command(t_cmd *command)
{
	size_t				i;
	static const char	*commandtypes[] = {
	[EOC] = "END_OF_COMMAND",
	[PIPE] = "PIPE",
	[APPEND] = "APPEND",
	[TRUNC] = "TRUNC",
	[IN_REDIR] = "IN_REDIR",
	[DEFAULT] = "DEFAULT"
	};

	i = 0;
	dprintf(2, "type: %s\n", commandtypes[command->cmdtype]);
	while (command->args[i])
	{
		dprintf(2, "%s%c", command->args[i], command->args[i + 1] ? ' ' : '\n');
		i++;
	}
	/* sleep(5); */
}

char	**ft_str2clear(char **str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

t_cmd	*new_command(size_t type, char **argstart, int *types, t_vec *fd_arr)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	if (type == PIPEDCOMMAND)
	{
		if (pipe(new->cmdpipe) == -1)
		{
			free(new);
			return (NULL); //error
		}
		vec_add(fd_arr, &new->cmdpipe[0]);
		vec_add(fd_arr, &new->cmdpipe[1]);
	}
	new->argtypes = types;
	new->cmdtype = type;
	new->args = argstart;
	new->next = NULL;
	return (new);
}

t_cmd	*push_command(t_cmd **commands, t_cmd *new)
{
	t_cmd	*iter;

	if (!new)
		return (NULL);
	if (!*commands)
	{
		*commands = new;
		return (new);
	}
	iter = *commands;
	while (iter && iter->next)
		iter = iter->next;
	iter->next = new;
	return (new);
}

t_cmd	*clear_commands(t_cmd *commands)
{
	t_cmd	*iter;
	t_cmd	*del;

	iter = commands;
	while (iter)
	{
		del = iter;
		iter = iter->next;
		free(del);
	}
	return (NULL);
}

t_cmd	*get_commands(t_vec *args, int *types, t_vec *fd_arr)
{
	t_cmd	*commands;
	char	**argv;
	size_t	i;

	i = 0;
	argv = (char **)args->store;
	commands = NULL;	
	vec_new(fd_arr, sizeof(int));
	while (i < args->index)
	{
		if (types[i] == PIPEDCOMMAND || types[i] == COMMAND)
		{
			if (!push_command(&commands,
				new_command(types[i], &argv[i], &types[i], fd_arr)))
				return (clear_commands(commands));
		}
		i++;
	}
	return (commands);
}
