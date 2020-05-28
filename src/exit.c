/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/16 11:54:12 by rlucas        #+#    #+#                 */
/*   Updated: 2020/04/28 01:31:38 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <unistd.h>
#include <libft.h>

/*
** Functions to free up everything before exiting the program.
*/

void		error_exit(t_msh *prog, int err)
{
	ft_printf_fd(2, "Error %d - ", err);
	ft_printf_fd(2, error_lookup(err));
	env_clear(prog->env, &free);
	free(prog->line.cap_table);
	exit(err);
}

void		std_exit(t_msh *prog)
{
	env_clear(prog->env, &free);
	free(prog->line.cap_table);
	exit(0);
}
