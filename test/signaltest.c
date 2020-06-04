/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signaltest.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/04 17:14:16 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/04 17:17:34 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void	handler(int num)
{
	printf("I am inevitable\n");
}

int		main(void)
{
	signal(SIGINT, handler);
	while (1)
	{
		printf("Wasting time\n");
		sleep(1);
	}
}
