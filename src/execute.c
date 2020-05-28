/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 22:22:24 by tbruinem      #+#    #+#                 */
/*   Updated: 2020/05/06 12:34:08 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

size_t		ft_str2len(char **str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

long long	ft_str2cmpstr(const char **str2, char *str)
{
	long long	i;

	i = 0;
	if (!str)
		return (-1);
	while (str2[i])
	{
		if (ft_strcmp((char *)str2[i], str) == 0)
			return (i);
		i++;
	}
	return (-1);
}

long long	is_builtin(char *program)
{
	static const char	*builtins[] = {
	[B_CD] = "cd",
	[B_ECHO] = "echo",
	[B_PWD] = "pwd",
	[B_EXPORT] = "export",
	[B_UNSET] = "unset",
	[B_ENV] = "env",
	[B_EXIT] = "exit",
	[7] = NULL,
	};

	return (ft_str2cmpstr(builtins, program));
}

int		is_executable(char *program, char **abspath_to_exe, t_var *env)
{
	char		*path;
	char		**entries;
	char		*tmp;
	struct stat	unused;
	size_t		i;

	i = 0;
	path = env_val_get("PATH", env);
	if (!path)
		return (0); //error
	entries = ft_split(path, ':');
	if (!entries)
		return (0); //error
	while (entries[i])
	{
		tmp = ft_str3join(entries[i], "/", program);
		if (stat(tmp, &unused) != -1)
		{
			*abspath_to_exe = tmp;
			ft_str2clear(entries);
			return (1);
		}
		i++;
		free(tmp);
	}
	ft_str2clear(entries);
	return (0);
}

int		run_program(t_msh *prog, t_cmd *cmd, char *abspath)
{
	int pid;

	if (!prog->envp)
		return (1); //error
//	dprintf(2, "abspath: %s\n", abspath);
	pid = fork();
	if (!pid)
	{
		close_ifnot(&prog->file_arr, cmd->iostream);
		if (cmd->iostream[READ] != -1 &&
			dup2(cmd->iostream[READ], STDIN) == -1)
			return (1);
		if (cmd->iostream[WRITE] != -1 &&
			dup2(cmd->iostream[WRITE], STDOUT) == -1)
			return (1);
		if (abspath && execve(abspath, cmd->args, prog->envp) == -1)
			return (1); //error
		if (!abspath && execve(cmd->args[0], cmd->args, prog->envp) == -1)
			return (1); //error
		close_iostream(cmd->iostream);
	}
	close_iostream(cmd->iostream);
	free(abspath);
	return (0);
}

int		run_builtin(t_msh *prog, t_cmd *cmd, int id)
{
	int						pid;
	static const t_builtin	builtins[] = {
	[B_ENV] = &ft_env,
	[B_PWD] = &ft_pwd,
	[B_ECHO] = &ft_echo,
	[B_EXPORT] = &ft_export,
	[B_UNSET] = &ft_unset,
	[B_EXIT] = &ft_exit,
	[B_CD] = &ft_cd
	};

//	dprintf(2, "IOSTREAM: [READ] = %d | [WRITE] = %d\n", cmd->iostream[0], cmd->iostream[1]);
	if (cmd->iostream[READ] == -1 && cmd->iostream[WRITE] == -1)
	{
		builtins[id](prog, ft_str2len(cmd->args), cmd->args);
		return (0);
	}
	pid = fork();
	if (!pid)
	{
		close_ifnot(&prog->file_arr, cmd->iostream);
		if (cmd->iostream[READ] != -1 &&
			dup2(cmd->iostream[READ], STDIN) == -1)
			return (1);
		if (cmd->iostream[WRITE] != -1 &&
			dup2(cmd->iostream[WRITE], STDOUT) == -1)
			return (1);
//		dprintf(2, "yeet\n");
		builtins[id](prog, ft_str2len(cmd->args), cmd->args);
		close_iostream(cmd->iostream);
		exit(0);
	}
	close_iostream(cmd->iostream);
	return (0);
}

int		execute(t_msh *prog, t_cmd *cmd)
{
	char	*abspath;
	int		builtin;

	abspath = NULL;
	builtin = is_builtin(cmd->args[0]);
//	dprintf(2, "BUILTIN: %d\n", builtin);
	if (builtin >= 0)
		return (run_builtin(prog, cmd, builtin));
	is_executable(cmd->args[0], &abspath, prog->env);
	return (run_program(prog, cmd, abspath));
}
