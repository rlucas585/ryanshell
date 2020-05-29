/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 16:46:14 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/29 18:17:24 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <libft.h>
#include <minishell.h>

/*
** Used to report Env output and to create the prompt.
*/

char		*ft_str3join(const char *s1, const char *s2, const char *s3)
{
	char		*newstring;

	if (!s1 || !s2 || !s3)
		return (NULL);
	newstring = (char *)ft_calloc(ft_strlen(s1) + ft_strlen(s2) +
			ft_strlen(s3) + 1, sizeof(char));
	if (!newstring)
		return (NULL);
	ft_strlcpy(newstring, s1, ft_strlen(s1) + 1);
	ft_strlcpy(newstring + ft_strlen(s1), s2, ft_strlen(s2) + 1);
	ft_strlcpy(newstring + ft_strlen(s1) + ft_strlen(s2), s3, ft_strlen(s3) + 1);
	return (newstring);
}

void		print_tokens(t_ryantok *tokens)
{
	size_t		i;
	static const char	*toktypes[] = {
		[COMMAND] = "COMMAND",
		[STANDARD] = "STANDARD",
		[WRITEFILE] = "WRITEFILE",
		[APPENDFILE] = "APPENDFILE",
		[INPUT_SENDER] = "INPUT_SENDER",
		[PIPEDCOMMAND] = "PIPEDCOMMAND"
	};

	i = 0;
	ft_printf("ID\tToken value\tType\tCommand_Num\n");
	while (tokens[i].value)
	{
		if (ft_strlen(toktypes[tokens[i].type]) > 7)
			ft_printf("[%u]\t%s\t\t%s\t%d\n", i, tokens[i].value,
					toktypes[tokens[i].type],
					tokens[i].cmd_num);
		else
			ft_printf("[%u]\t%s\t\t%s\t\t%d\n", i, tokens[i].value,
					toktypes[tokens[i].type],
					tokens[i].cmd_num);
		i++;
	}
}
