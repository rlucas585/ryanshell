/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putchar.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/18 11:53:09 by rlucas        #+#    #+#                 */
/*   Updated: 2020/04/18 12:08:55 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

/*
** ft_putchar written as int type function to conform to the tputs function
** of the termcap library.
*/

int		ft_putchar(int c)
{
	ssize_t		err;

	err = write(1, &c, 1);
	if (err == -1)
		return (-1);
	return (c);
}
