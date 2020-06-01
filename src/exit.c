/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/16 11:54:12 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/01 23:28:03 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <unistd.h>
#include <libft.h>

/*
** Functions to free up everything before exiting the program.
*/

void		error_exit(t_msh *prog, int err, int stage)
{
	ft_printf_fd(2, "Error %d - ", err);
	ft_printf_fd(2, error_lookup(err));
	if (stage == PRE_ENV)
		exit(err);
	if (stage == IN_ENV || stage == IN_TERM)
		clear_env(prog);
	exit(err);
}

void		std_exit(t_msh *prog)
{
	clear_env(prog);
	exit(0);
}
