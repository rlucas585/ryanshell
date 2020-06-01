/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/16 10:35:55 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/01 23:39:53 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>
#include <fcntl.h>
#include <stdio.h>

void	refresh_prog(t_msh *prog)
{
	if (vecstr_truncate(&prog->line.cmd, 0))
		exit (-1); // Mem fail - deal with later
	free(prog->tokens);
	prog->tokens = NULL;
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
