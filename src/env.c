/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 17:55:28 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/30 13:49:26 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

char		**make_envp(t_msh *prog)
{
	size_t		i;
	char		**envp;
	size_t		total;

	total = vector_total(&prog->env);
	envp = (char **)malloc(sizeof(char *) * (total + 1));
	if (!envp)
		return (NULL); // Mem fail - deal with later
	i = 0;
	while (i < total)
	{
		envp[i] = (char *)vector_get(&prog->env, i);
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

void		env_unset(t_msh *prog, const char *unsetvar)
{
	size_t		total;
	size_t		i;
	size_t		len;
	size_t		valindex;

	i = 0;
	total = vector_total(&prog->env);
	len = ft_strlen(unsetvar);
	while (i < total)
	{
		if (ft_strncmp(unsetvar, (char *)vector_get(&prog->env, i), len) == 0)
		{
			valindex = ft_strclen((char *)vector_get(&prog->env, i), '=') + 1;
			if (valindex - 1 == len)
			{
				free(vector_get(&prog->env, i));
				vector_delete(&prog->env, i);
				free(prog->envp);
				prog->envp = make_envp(prog);
				if (!prog->envp)
					return ; // Mem fail - deal with later
				return ;
			}
		}
		i++;
	}
}

void		env_export(t_msh *prog, char *newvar)
{
	size_t		valindex;
	char		*new;

	if (newvar[0] == '=')
		return ; // bash: export: `=test': not a valid identifier
	if (!ft_strchr(newvar, '='))
		return ;
	new = ft_strdup(newvar);
	if (!new)
		return ; // Mem fail - deal with later
	valindex = ft_strclen(newvar, '=') + 1;
	newvar[valindex - 1] = '\0';
	env_unset(prog, newvar);
	if (vector_add(&prog->env, new))
		return ; // Mem fail -deal with later
}

void		print_env(t_msh *prog)
{
	size_t		i;

	i = 0;
	while (prog->envp[i])
	{
		ft_printf("%s\n", prog->envp[i]);
		i++;
	}
}

char		*env_val_get(const char *name, t_msh *prog, size_t len)
{
	size_t		i;
	char		*val;
	size_t		valindex;
	size_t		total;

	i = 0;
	total = vector_total(&prog->env);
	while (i < total)
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
		if (vector_add(&prog->env, newval))
			return ; // Mem error - deal with later
		i++;
	}
	prog->envp = make_envp(prog);
}
