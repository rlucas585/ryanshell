/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/04 17:18:23 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/04 17:28:17 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <libft.h>
#include <minishell.h>
#include <sys/types.h>

void	sighandler(int signal)
{
	size_t		i;
	int			c;

	i = 0;
	c = '\n';
	while (i < g_pid.total)
	{
		kill(*(pid_t *)vecarr_get(&g_pid, i), signal);
		i++;
	}
	write(STDOUT, &c, 1);
}
