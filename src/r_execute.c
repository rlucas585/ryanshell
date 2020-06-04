/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   r_execute.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/28 13:45:00 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/04 17:10:04 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void	ft_echo(t_msh *prog, t_ryancmd cmd, int cmd_num)
{
	size_t		i;
	int			n_opt;
	int			output;

	i = 0;
	(void)cmd;
	n_opt = 0;
	output = 0;
	while (prog->tokens[i].cmd_num != cmd_num)
		i++;
	while (prog->tokens[i].value && prog->tokens[i].cmd_num == cmd_num)
	{
		if (prog->tokens[i].type == STANDARD)
		{
			if (output == 1 && ft_strlen(prog->tokens[i].value) > 0)
				ft_printf(" ");
			if (output == 0)
				if (ft_strcmp(prog->tokens[i].value, "-n") == 0)
				{
					n_opt = 1;
					i++;
					continue ;
				}
			ft_printf("%s", prog->tokens[i].value);
			output = 1;
		}
		i++;
	}
	if (!n_opt)
		ft_printf("\n");
}

void	ft_unset(t_msh *prog, t_ryancmd cmd, int cmd_num)
{
	size_t		i;

	i = 0;
	(void)cmd;
	while (prog->tokens[i].cmd_num != cmd_num)
		i++;
	while (prog->tokens[i].value && prog->tokens[i].cmd_num == cmd_num)
	{
		if (prog->tokens[i].type == STANDARD)
			env_unset(prog, prog->tokens[i].value);
		i++;
	}
}

void	ft_export(t_msh *prog, t_ryancmd cmd, int cmd_num)
{
	size_t		i;

	i = 0;
	(void)cmd;
	while (prog->tokens[i].cmd_num != cmd_num)
		i++;
	while (prog->tokens[i].value && prog->tokens[i].cmd_num == cmd_num)
	{
		if (prog->tokens[i].type == STANDARD)
			env_export(prog, prog->tokens[i].value);
		i++;
	}
}

void	ft_env(t_msh *prog, t_ryancmd cmd, int cmd_num)
{
	size_t		i;

	i = 0;
	(void)cmd;
	while (prog->tokens[i].cmd_num != cmd_num)
		i++;
	while (prog->tokens[i].value && prog->tokens[i].cmd_num == cmd_num)
	{
		if (prog->tokens[i].type == STANDARD)
		{
			ft_printf_fd(2, "minishell: env: too many arguments\n");
			return ;
		}
		i++;
	}
	print_env(prog);
}

void	ft_pwd(t_msh *prog, t_ryancmd cmd, int cmd_num)
{
	(void)prog;
	(void)cmd;
	(void)cmd_num;
	ft_printf("%s\n", getcwd(NULL, 0));
}

void	ft_cd(t_msh *prog, t_ryancmd cmd, int cmd_num)
{
	size_t		i;
	char		*dest_dir;

	(void)cmd;
	i = 0;
	dest_dir = NULL;
	while (prog->tokens[i].cmd_num != cmd_num)
		i++;
	while (prog->tokens[i].value && prog->tokens[i].cmd_num == cmd_num)
	{
		if (prog->tokens[i].type == STANDARD)
		{
			if (dest_dir != NULL)
			{
				ft_printf_fd(2, "minishell: cd: too many arguments\n");
				return ;
			}
			dest_dir = prog->tokens[i].value;
		}
		i++;
	}
	if (dest_dir == NULL || ft_strcmp("~", dest_dir) == 0)
		dest_dir = env_val_get("HOME", prog, ft_strlen("HOME"));
	if (dest_dir == NULL)
		return ;
	if (chdir(dest_dir) == -1)
		ft_printf("cd: %s: %s\n", strerror(errno), dest_dir);
}

int		get_exit_code(char *str)
{
	size_t		i;
	int			ret;

	i = 0;
	while (str[i])
	{
		if (i == 0 && str[i] == '-')
			;
		else if (!ft_isdigit(str[i]))
			return (-1);
		i++;
	}
	ret = ft_atoi(str);
	if (ret < 0)
	{
		ret = ft_abs(ret % 256);
		ret = 256 - ret;
		return (ret);
	}
	return (ft_atoi(str) % 256);
}

void	ft_exit(t_msh *prog, t_ryancmd cmd, int cmd_num)
{
	size_t		i;
	int			exitstatus;
	int			first;

	(void)cmd;
	i = 0;
	exitstatus = 0;
	first = 0;
	while (prog->tokens[i].cmd_num != cmd_num)
		i++;
	while (prog->tokens[i].value && prog->tokens[i].cmd_num == cmd_num)
	{
		if (prog->tokens[i].type == STANDARD)
		{
			if (first == 1)
			{
				ft_printf_fd(2, "minishell: exit: too many arguments\n");
				return ;
			}
			first = 1;
			exitstatus = get_exit_code(prog->tokens[i].value);
			if (exitstatus == -1)
			{
				ft_printf_fd(2, "minishell: exit: %s: "
						"numeric argument required\n", prog->tokens[i].value);
				exit(2); // Should of course be an exit that frees everything
			}
		}
		i++;
	}
	ft_printf("exit\n");
	exit(exitstatus); // Should of course be an exit that frees everything
}

t_rbin	builtin_funcs(int code)
{
	static const t_rbin	builtins[] = {
	[B_CD] = &ft_cd,
	[B_ECHO] = &ft_echo,
	[B_PWD] = &ft_pwd,
	[B_EXPORT] = &ft_export,
	[B_UNSET] = &ft_unset,
	[B_ENV] = &ft_env,
	[B_EXIT] = &ft_exit,
	[7] = NULL,
	};

	return (builtins[code]);
}

int		ft_is_builtin(const char *command)
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
	int					i;

	i = 0;	
	while (builtins[i])
	{
		if (ft_strcmp(builtins[i], command) == 0)
			return (i);
		i++;
	}
	return (-1);
}

/* void	r_runcmd(t_ryancmd cmd, t_msh *prog, int cmd_num) */
/* { */
/* 	t_rbin	builtin; */
/* 	int		code; */
/*  */
/* 	code = ft_is_builtin(cmd.command); */
/* 	if (code != -1) */
/* 	{ */
/* 		builtin = builtin_funcs(code); */
/* 		builtin(prog, cmd, cmd_num); */
/* 	} */
/* } */

void	update_cmd(char *value, int type, t_ryancmd *cmd)
{
	if (type == COMMAND || type == PIPEDCOMMAND)
		cmd->command = value;
	if (type == WRITEFILE)
	{
		cmd->output = value;
		cmd->append = 0;
	}
	if (type == APPENDFILE)
	{
		cmd->output = value;
		cmd->append = 1;
	}
	if (type == INPUT_SENDER)
		cmd->input = value;
}

void	check_pipe(t_msh *prog, t_ryancmd *cmd, int cmd_num)
{
	size_t		i;

	i = 0;
	while (prog->tokens[i].cmd_num != cmd_num + 1)
		i++;
	while (prog->tokens[i].value)
	{
		if (prog->tokens[i].type == COMMAND)
			return ;
		if (prog->tokens[i].type == PIPEDCOMMAND)
		{
			cmd->fork += 1;
			if (cmd->output == NULL)
				cmd->output = (char *)1;
		}
		i++;
	}
	return ;
}

char		*find_binary(t_msh *prog, t_ryancmd cmd)
{
	char		*path;
	char		*tok;
	char		*bin;
	int			exist;
	struct stat	buffer;

	path = ft_strdup(env_val_get("PATH", prog, ft_strlen("PATH")));
	if (!path)
		return (NULL); // Mem error - deal with later
	tok = ft_strtok(path, ":");
	while (tok)
	{
		bin = ft_str3join(tok, "/", cmd.command);
		if (!bin)
			return (NULL); // Mem error - deal with later exit with prog.
		exist = stat(bin, &buffer);
		if (exist == 0)
			break ;
		tok = ft_strtok(NULL, ":");
		free(bin);
		bin = NULL;
	}
	free(path);
	return (bin);
}

char		**make_argv(t_msh *prog, t_ryancmd cmd, char *executable)
{
	size_t		i;
	size_t		j;
	size_t		cmd_start;
	size_t		argc;
	char		**argv;

	i = 0;
	argc = 1;
	j = 1;
	while (prog->tokens[i].cmd_num != cmd.cmd_num)
		i++;
	cmd_start = i;
	while (prog->tokens[i].value && prog->tokens[i].cmd_num == cmd.cmd_num)
	{
		if (prog->tokens[i].type == STANDARD)
			argc++;
		i++;
	}
	i = cmd_start;
	argv = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		return (NULL); // Mem fail - deal with later
	argv[0] = executable;
	while (prog->tokens[i].value && prog->tokens[i].cmd_num == cmd.cmd_num)
	{
		if (prog->tokens[i].type == STANDARD)
		{
			argv[j] = prog->tokens[i].value;
			j++;
		}
		i++;
	}
	argv[j] = NULL;
	return (argv);
}

int			check_if_path(t_ryancmd cmd)
{
	struct stat	buffer;

	if (stat(cmd.command, &buffer) == 0)
		return (1);
	return (0);
}

void		external_exec(t_msh *prog, t_ryancmd cmd)
{
	char		*executable;
	char		**argv;

	if (check_if_path(cmd))
	{
		argv = make_argv(prog, cmd, cmd.command);
		execve(cmd.command, argv, prog->envp);
		free(argv);
		return ;
	}
	executable = find_binary(prog, cmd);
	if (!executable)
	{
		ft_printf_fd(STDERR, "%s: command not found\n", cmd.command);
		exit(errno);
	}
	argv = make_argv(prog, cmd, executable);
	execve(executable, argv, prog->envp);
	free(argv);
}

void		run_command(t_msh *prog, t_ryancmd cmd, int code)
{
	t_rbin	builtin;
	pid_t	pid;
	
	if (code != -1)
	{
		builtin = builtin_funcs(code);
		builtin(prog, cmd, cmd.cmd_num);
	}
	else
	{
		pid = fork();
		if (pid < 0)
			return ; // Forking error
		if (pid)
			wait(NULL);
		else
			external_exec(prog, cmd);
	}
}

t_ryancmd	format_cmd(t_msh *prog, int cmd_num)
{
	t_ryancmd	cmd;
	size_t		i;

	i = 0;
	cmd.output = NULL;
	cmd.input = NULL;
	cmd.command = NULL;
	cmd.append = 0;
	cmd.cmd_num = cmd_num;
	cmd.fork = 0;
	while (prog->tokens[i].value && prog->tokens[i].cmd_num != cmd_num)
		i++;
	while (prog->tokens[i].value && prog->tokens[i].cmd_num == cmd_num)
	{
		update_cmd(prog->tokens[i].value, prog->tokens[i].type, &cmd);
		i++;
	}
	if (prog->tokens[i].value)
		check_pipe(prog, &cmd, cmd_num);
	return (cmd);
}

void		print_cmd(t_ryancmd cmd)
{
	ft_printf("Cmd_num = %d\n", cmd.cmd_num);
	ft_printf("Command = %s\n", cmd.command);
	if (cmd.output == (char *)1)
		ft_printf("Output = Piped STDOUT\n");
	else if (cmd.output == NULL)
		ft_printf("Output = Standard STDOUT\n");
	else
		ft_printf("Output = file named %s\n", cmd.output);
	if (cmd.input == NULL)
		ft_printf("Input = STDIN\n");
	else
		ft_printf("Input = file named %s\n", cmd.input);
	if (cmd.append)
		ft_printf("Appending output = TRUE\n");
	else
		ft_printf("Appending output = FALSE\n");
	if (cmd.fork)
		ft_printf("Start new child process? TRUE\n\n");
	else
		ft_printf("Start new child process? FALSE\n\n");
}

int			io_files(t_msh *prog, t_ryancmd cmd, int fd[2])
{
	(void)prog; // Fix later.
	if (cmd.input != NULL && cmd.input != (char *)1)
	{
		fd[0] = open(cmd.input, O_RDONLY);
		if (fd[0] == -1)
		{
			ft_printf("minishell: %s: %s\n", cmd.input, strerror(errno));
			return (1);
		}
		dup2(fd[0], STDIN); // Exit if fail
	}
	if (cmd.output != NULL && cmd.output != (char *)1)
	{
		if (cmd.append)
			fd[1] = open(cmd.output, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else
			fd[1] = open(cmd.output, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd[1] == -1)
		{
			ft_printf("minishell: %s: %s\n", cmd.output, strerror(errno));
			return (1);
		}
		dup2(fd[1], STDOUT); // free prog if error.
	}
	return (0);
}

void		single_cmd(t_msh *prog, t_ryancmd cmd)
{
	int		code;
	pid_t	pid;
	int		fd[2];

	fd[0] = 0;
	fd[1] = 0;
	if (cmd.input != NULL || cmd.output != NULL)
		if (io_files(prog, cmd, fd))
			return ;
	code = ft_is_builtin(cmd.command);
	/* if (code >= B_CD) */
	if (cmd.fork == 0)
	{
		run_command(prog, cmd, code);
	}
	else
	{
		pid = fork();
		if (pid < 0)
			exit (-1); // Forking error
		if (pid > 0)
			wait(NULL);
		if (pid == 0)
		{
			run_command(prog, cmd, code);
			if (fd[0])
				close(fd[0]);
			if (fd[1])
				close(fd[1]);
			exit(0);
		}
	}
	if (fd[0])
		close(fd[0]);
	if (fd[1])
		close(fd[1]);
}

int			r_runcmd(t_msh *prog, int cmd_num)
{
	t_ryancmd	cmd;
	int			saved_stdout;
	int			saved_stdin;

	saved_stdout = dup(STDOUT);
	saved_stdin = dup(STDIN);
	cmd = format_cmd(prog, cmd_num);
	/* print_cmd(cmd); */
	if (cmd.fork)
		run_fork(prog, cmd);
	else
		single_cmd(prog, cmd);
	dup2(saved_stdout, STDOUT);
	dup2(saved_stdin, STDIN);
	close(saved_stdout);
	close(saved_stdin);
	return (cmd.fork);
}

void		fork_and_pipe(t_msh *prog, int fd[2], pid_t *pid)
{
	(void)prog;
	if (pipe(fd) == -1)
		exit (-1); // Piping error
	*pid = fork();
	if (*pid < 0)
		exit (-1); // Forking error
}

void		run_fork(t_msh *prog, t_ryancmd cmd)
{
	int			fd[2];
	pid_t		pid;
	int			child_exit;

	fork_and_pipe(prog, fd, &pid);
	if (pid > 0)
	{
		vecarr_add(&g_pid, &pid);
		close(fd[READ_END]);
		dup2(fd[WRITE_END], STDOUT); // Add error checking
		single_cmd(prog, cmd);
		close(fd[WRITE_END]);
		close(STDOUT);
		waitpid(pid, &child_exit, 0);
		vecarr_delete_by_val(&g_pid, &pid);
	}
	if (pid == 0)
	{
		close(fd[WRITE_END]);
		dup2(fd[READ_END], STDIN); // Add error checking
		r_runcmd(prog, cmd.cmd_num + 1);
		close(fd[READ_END]);
		exit(0);
	}
}

int		r_execute(t_msh *prog)
{
	size_t		i;
	int			cmd_num;

	i = 0;
	cmd_num = 0;
	while (prog->tokens[i].value)
	{
		cmd_num += r_runcmd(prog, cmd_num) + 1;
		while (prog->tokens[i].value && prog->tokens[i].cmd_num != cmd_num)
			i++;
		if (!prog->tokens[i].value)
			break ;
	}
	return (1);
}
