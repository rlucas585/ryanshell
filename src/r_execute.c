/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   r_execute.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/28 13:45:00 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/29 23:37:53 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

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
			if (output == 1)
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

typedef void	(*t_rbin)(t_msh *prog, t_ryancmd cmd, int cmd_num);

t_rbin	builtin_funcs(int code)
{
	static const t_rbin	builtins[] = {
	/* [B_CD] = "cd", */
	[B_ECHO] = &ft_echo,
	/* [B_PWD] = "pwd", */
	/* [B_EXPORT] = "export", */
	/* [B_UNSET] = "unset", */
	/* [B_ENV] = "env", */
	/* [B_EXIT] = "exit", */
	/* [7] = NULL, */
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

int			r_runcmd(t_msh *prog, int cmd_num)
{
	t_ryancmd	cmd;

	cmd = format_cmd(prog, cmd_num);
	/* print_cmd(cmd); */
	execute_cmd(prog, cmd);
	return (cmd.fork);
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
	while (prog->tokens[i].cmd_num == cmd.cmd_num)
	{
		if (prog->tokens[i].type == STANDARD)
			argc++;
		i++;
	}
	i = cmd_start;
	argv = (char **)malloc(sizeof(char *) * argc + 1);
	if (!argv)
		return (NULL); // Mem fail - deal with later
	argv[0] = executable;
	while (prog->tokens[i].cmd_num == cmd.cmd_num)
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

char		**make_envp(t_msh *prog)
{
	size_t		i;
	char		**envp;
	size_t		total;

	total = vector_total(&prog->env);
	envp = (char **)malloc(sizeof(char *) * total + 1);
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

void		external_exec(t_msh *prog, t_ryancmd cmd)
{
	char		*executable;
	char		**argv;
	char		**envp;

	executable = find_binary(prog, cmd);
	if (!executable)
	{
		ft_printf_fd(STDERR, "%s: command not found\n", cmd.command);
		return ;
	}
	argv = make_argv(prog, cmd, executable);
	envp = make_envp(prog);
	execve(executable, argv, envp);
	free(argv);
	free(envp);
}

void		standard_command(t_msh *prog, t_ryancmd cmd)
{
	t_rbin	builtin;
	int		code;
	pid_t	child_pid;

	code = ft_is_builtin(cmd.command);
	child_pid = fork();
	if (child_pid < 0)
		exit (-1); // Piping error
	else if (child_pid > 0)
		wait(NULL);
	else if (child_pid == 0)
	{
		if (code != -1)
		{
			builtin = builtin_funcs(code);
			builtin(prog, cmd, cmd.cmd_num);
		}
		else
			external_exec(prog, cmd);
		exit(0);
	}
}

void		fork_command(t_msh *prog, t_ryancmd cmd)
{
	int		fd[2];
	pid_t	child_pid;
	int		saved_stdout;

	saved_stdout = dup(1);
	if (pipe(fd) == -1)
		exit (-1); // Piping error
	child_pid = fork();
	if (child_pid < 0)
		exit (-1); // Piping error
	else if (child_pid > 0) // Parent process
	{
		close(fd[READ_END]);
		dup2(fd[WRITE_END], STDOUT); // Redirect stdout to pipe.
		standard_command(prog, cmd);
		close(fd[WRITE_END]); // Close the stream to child process.
		dup2(saved_stdout, 1);
		close(saved_stdout);
		wait(NULL); // Now parent waits for all child processes to complete.
	}
	else if (child_pid == 0) // Child process
	{
		close(fd[WRITE_END]); // Children don't write to parent in '|' pipes
		dup2(fd[READ_END], STDIN); // Replace STDIN with input from pipe.
		r_runcmd(prog, cmd.cmd_num + 1);
		close(fd[READ_END]); // Reading should be complete.
		exit(0); // Exit child process
	}
}

void		execute_cmd(t_msh *prog, t_ryancmd cmd)
{
	if (cmd.fork)
		fork_command(prog, cmd);
	else
		standard_command(prog, cmd);
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
