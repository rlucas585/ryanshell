/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 17:55:28 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/29 22:53:36 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

void		print_env(t_msh *prog)
{
	size_t		i;
	size_t		total;

	i = 0;
	total = vector_total(&prog->env);
	while (i < total)
	{
		ft_printf("[%u]\t%s\n", i, (char *)vector_get(&prog->env, i));
		i++;
	}
}

char		*env_val_get(const char *name, t_msh *prog, size_t len)
{
	size_t		i;
	char		*val;
	size_t		valindex;

	i = 0;
	while (i < vector_total(&prog->env))
	{
		if (ft_strncmp(name, (char *)vector_get(&prog->env, i), len) == 0)
		{
			valindex = ft_strclen((char *)vector_get(&prog->env, i), '=') + 1;
			if (valindex - 1 == len)
			{
				val = (char *)vector_get(&prog->env, i) + valindex;
				return (val);
			}
		}
		i++;
	}
	return (NULL);
}

void	env_init(t_msh *prog)
{
	extern char	**environ;
	size_t		i;
	char		*newval;

	i = 0;
	if (vector_init(&prog->env))
		return ; // Mem error - deal with later
	while (environ[i])
	{
		newval = ft_strdup(environ[i]);
		if (!newval)
			return ; // Mem error - deal with later
		vector_add(&prog->env, newval);
		i++;
	}
}
