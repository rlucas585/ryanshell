/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prompt.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 16:42:01 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/29 23:51:10 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

size_t		ft_no_ansi_strlen(const char *str)
{
	size_t		i;
	size_t		len;
	int			inansi;

	i = 0;
	len = 0;
	inansi = 0;
	while (str[i])
	{
		if (str[i] == '\033')
			inansi = 1;
		if (!inansi)
			len++;
		if (inansi == 1 && str[i] == 'm')
			inansi = 0;
		i++;
	}
	return (len);
}

/*
** Creates the prompt for minishell using the USER environment variable - or
** an empty prompt if no such variable exists.
*/

char		*prompt(t_msh *prog, t_line *line)
{
	char	*user;
	char	*prompt;

	user = env_val_get("USER", prog, ft_strlen("USER"));
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
