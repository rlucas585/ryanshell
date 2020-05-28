/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/17 23:13:43 by tbruinem      #+#    #+#                 */
/*   Updated: 2020/05/05 13:06:58 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include "libft.h"
#include <stdio.h>

void	env_del(t_var *delete)
{
	free(delete->name);
	free(delete->val);
	free(delete);
}

void	env_unset(t_var **env, char *name)
{
	t_var	*iter;
	t_var	*last;

	iter = *env;
	last = iter;
	if (ft_strcmp(name, (*env)->name) == 0)
	{
		*env = (*env)->next;
		env_del(iter);
		return ;
	}
	while (iter)
	{
		if (ft_strcmp(name, iter->name) == 0)
		{
			last->next = iter->next;
			env_del(iter);
			iter = last;
		}
		last = iter;
		iter = iter->next;
	}
}

t_var	*env_new(const char *name, const char *val)
{
	t_var	*new;

	new = malloc(sizeof(t_var));
	if (!new)
		return (NULL);
	new->len = ft_strlen(name) + ft_strlen(val) + 1;
	new->name = ft_strdup(name);
	if (!new->name)
		return (NULL);//error
	new->val = ft_strdup(val);
	if (!new->val)
		return (NULL);//error
	new->next = NULL;
	return (new);
}

t_var	*env_val_set(const char *name, t_var *env, const char *val)
{
	while (env && env->next)
	{
		if (ft_strcmp(env->name, name) == 0)
		{
			free(env->val);
			env->val = ft_strdup(val);
			if (!env->val)
				return (NULL);
			return (env);
		}
		env = env->next;
	}
	env->next = env_new(name, val);
	return (NULL);
}

char	*env_val_get(const char *name, t_var *env)
{
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
			return (env->val);
		env = env->next;
	}
	return (NULL);
}

void	env_clear(t_var *env, void (*del)(void *))
{
	t_var	*delete;

	while (env)
	{
		delete = env;
		env = env->next;
		del(delete->name);
		del(delete->val);
	}
}

void	env_print(t_var *env)
{
	while (env)
	{
		ft_printf("%s=%s\n", env->name, env->val);
		env = env->next;
	}
}

char	*var_name(char **str)
{
	char	*new;
	char	*end_of_name;
	size_t	i;

	i = 0;
	end_of_name = ft_strchr(*str, '=');
	if (!end_of_name)
		return (NULL); //error
	new = malloc(sizeof(char) * ((end_of_name - *str) + 1));
	if (!new)
		return (NULL); //error
	while (**str && *str < end_of_name)
	{
		new[i] = **str;
		(*str)++;
		i++;
	}
	new[i] = 0;
	(*str)++;
	return (new);
}

size_t	env_size(t_var *env)
{
	size_t	len;

	len = 0;
	while (env)
	{
		len++;
		env = env->next;
	}
	return (len);
}

t_var	*var_init(char *str)
{
	t_var	*new;

	new = malloc(sizeof(t_var));
	if (!new)
		return (NULL);
	new->len = ft_strlen(str);
	new->name = var_name(&str);
	if (!new->name)
		return (NULL);//error
	new->val = ft_strdup(str);
	if (!new->val)
		return (NULL);//error
	new->next = NULL;
	return (new);
}

char	**env_convert(t_var *env)
{
	size_t	i;
	size_t	len;
	char	**envp;

	i = 0;
	len = env_size(env);
	envp = malloc(sizeof(char *) * (len + 1));
	if (!envp)
		return (NULL);
	while (i < len)
	{
		envp[i] = ft_str3join(env->name, "=", env->val);
		if (!envp[i])
			return (ft_str2clear(envp));
		env = env->next;
		i++;
	}
	envp[i] = 0;
	return (envp);
}

void	env_update(t_msh *prog)
{
	ft_str2clear(prog->envp);
	prog->envp = env_convert(prog->env);
}

void	env_init(t_msh *prog)
{
	extern char	**environ;
	t_var		*env;
	size_t		i;

	i = 0;
	prog->env = var_init(environ[i]);
	if (!prog->env)
		return (env_clear(prog->env, &free));//error
	env = prog->env;
	while (environ[i])
	{
		env->next = var_init(environ[i]);
		if (!env->next)
			return (env_clear(prog->env, &free));//error
		env = env->next;
		i++;
	}
	prog->envp = env_convert(prog->env);
}
