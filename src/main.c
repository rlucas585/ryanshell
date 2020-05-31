/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/16 10:35:55 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/31 23:38:58 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>
#include <fcntl.h>
#include <stdio.h>

char	**g_termbuff;

void	refresh_prog(t_msh *prog)
{
	free(prog->line.cmd);
	prog->line.cmd = NULL;
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
		if (read_input(prog) == -1)
			error_exit(prog, MEM_FAIL);
		prog->line.term.c_lflag |= ECHO;
		prog->line.term.c_lflag |= ICANON;
		tcsetattr(STDIN, TCSAFLUSH, &prog->line.term);
		tokenizer(prog);
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
