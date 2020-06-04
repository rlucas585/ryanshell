/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/16 10:35:55 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/04 17:24:04 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

void	refresh_prog(t_msh *prog)
{
	if (vecstr_reset(&prog->line.cmd))
		exit (-1); // Mem fail - deal with later
	free(prog->tokens);
	prog->tokens = NULL;
}

void	print_vecarr(t_vecarr *dy_arr)
{
	size_t		i;

	i = 0;
	while (i < dy_arr->total)
	{
		ft_printf("[%u]\t%d\n", i, *(pid_t *)vecarr_get(dy_arr, i));
		i++;
	}
}

int	msh_main(t_msh *prog)
{
	int		status;

	status = 1;
	init_readline(prog);
	while (status)
	{
		read_input(prog);
		prog->line.term.c_lflag |= ECHO;
		prog->line.term.c_lflag |= ICANON;
		tcsetattr(STDIN, TCSAFLUSH, &prog->line.term);
		tokenizer(prog, &prog->line.cmd);
		print_tokens(prog->tokens);
		status = r_execute(prog);
		refresh_prog(prog);
		prog->line.term.c_lflag &= ~(ECHO | ICANON);
		tcsetattr(STDIN, TCSAFLUSH, &prog->line.term);
		tcflush(STDIN, TCIFLUSH);
		print_vecarr(&g_pid);
	}
	std_exit(prog);
	return (0);
}

int	main(void)
{
	t_msh	prog;

	signal(SIGINT, sighandler);
	env_init(&prog);
	vecarr_init(&g_pid, sizeof(pid_t));
	return (msh_main(&prog));
}
