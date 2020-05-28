/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/16 10:35:55 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/28 10:22:07 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>
#include <fcntl.h>
#include <stdio.h>

char	**g_termbuff;

void	redirection_apply(char **args, int *inout)
{
	int			fds[100];
	size_t		i;
	size_t		fd_index;

	i = 0;
	fd_index = 0;
	while (args[i])
	{
//		ft_printf("%ld | %s\n", i, args[i]);
		if (ft_strncmp(args[i], ">>", 3) == 0)
		{
			if (args[i + 1])
			{
				fds[fd_index] = open(args[i + 1], O_CREAT | O_WRONLY | O_APPEND);
				inout[WRITE] = fds[fd_index];
//				dprintf(2, ">> detected, setting OUT.\n");
//				free(args[i]);
//				args[i] = NULL;
//				free(args[i + 1]);
//				args[i + 1] = NULL;
				i++;
				fd_index++;
			}
			else
				break ;
		}
		else if (ft_strncmp(args[i], ">", 2) == 0)
		{
			if (args[i + 1])
			{
				fds[fd_index] = open(args[i + 1], O_CREAT | O_WRONLY | O_TRUNC);
				inout[WRITE] = fds[fd_index];
//				dprintf(2, "> detected, setting OUT.\n");
//				free(args[i]);
//				args[i] = NULL;
//				free(args[i + 1]);
//				args[i + 1] = NULL;
				i++;
				fd_index++;
			}
			else
				break ;
		}
		else if (ft_strncmp(args[i], "<", 2) == 0)
		{
			if (args[i + 1])
			{
				fds[fd_index] = open(args[i + 1], O_RDONLY);
				inout[READ] = fds[fd_index];
//				dprintf(2, "< detected, setting IN.\n");
//				free(args[i]);
//				args[i] = NULL;
//				free(args[i + 1]);
//				args[i + 1] = NULL;
				i++;
				fd_index++;
			}
			else
				break ;
		}
		i++;
	}
}

int		in_out_redirection(t_msh *prog, t_cmd *command)
{
	command->iostream[0] = -1;
	command->iostream[1] = -1;
	if (command->cmdtype == PIPEDCOMMAND)
		command->iostream[READ] = command->cmdpipe[READ];
	if (command->next && command->next->cmdtype == PIPEDCOMMAND)
		command->iostream[WRITE] = command->next->cmdpipe[WRITE];
	(void)prog;
	//this has to do with why we're printing stuff after the command is ran
	if (!set_redirection(command, command->args,
		command->argtypes, &prog->file_arr))
		return (0);
	return (1);
}

int		run_commands(t_msh *prog, t_cmd *commands)
{
	while (commands)
	{
		if (!in_out_redirection(prog, commands))
			return (1);
//		print_filearr(&prog->file_arr);
//		print_command(commands);
		(void)execute(prog, commands);
		commands = commands->next;
	}
	close_all(&prog->file_arr);
	vec_destroy(&prog->file_arr, NULL);
	return (1);
}

void	debug_commands(t_cmd *commands)
{
	size_t	i;
	size_t	cmd;

	cmd = 0;
	ft_printf("start of command debug\n");
	while (commands)
	{
		i = 0;
		while (commands->args[i])
		{
			ft_printf("%ld | %s\n", cmd, commands->args[i]);
			i++;
		}
		cmd++;
		commands = commands->next;
	}
}

int	msh_main(t_msh *prog)
{
	t_cmd	*commands;
	t_vec	args;
	t_vec	argtypes;
	int		status;
	/* char	buf[8]; */

	status = 1;
	init_readline(prog);
	while (status)
	{
		if (read_input(prog) == -1)
			error_exit(prog, MEM_FAIL);
		tokenizer(prog, &args, &argtypes);
		commands = get_commands(&args, (int *)argtypes.store, &(prog->file_arr));
		debug_commands(commands);
		status = run_commands(prog, commands);
	/* This helps calibrate cursor following command output for some reason */
		/* ft_printf_fd(STDOUT, "\033[6n"); */
		/* read(STDOUT, buf, 8); */
		/* buf[8] = '\0'; */
		/* ft_printf("\nbuf = %s\n", buf); */
	}
	std_exit(prog);
	return (0);
}

int	main(void)
{
	t_msh	prog;

	env_init(&prog);
	return (msh_main(&prog));
}
