/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_utils.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/23 15:07:00 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/26 16:46:46 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

size_t		ft_strclen(const char *string, int c)
{
	size_t		count;

	count = 0;
	while (string[count] && string[count] != c)
		count++;
	return (count);
}
