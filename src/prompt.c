/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prompt.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 16:42:01 by rlucas        #+#    #+#                 */
/*   Updated: 2020/04/29 17:15:04 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

/*
** Creates the prompt for minishell using the USER environment variable - or
** an empty prompt if no such variable exists.
*/

char		*prompt(t_msh *prog, t_line *line)
{
	char	*user;
	char	*prompt;

	user = env_val_get("USER", prog->env);
	if (user != NULL)
	{
		line->promptlen = ft_strlen(user) + 4;
		prompt = ft_str3join("\033[32m", user, "\033[39m:~$ ");
		if (!prompt)
			return (NULL);
		return (prompt);
	}
	else
		return (ft_strdup(":~$ "));
}
