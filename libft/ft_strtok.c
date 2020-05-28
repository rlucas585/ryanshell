/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strtok.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/15 13:36:10 by rlucas        #+#    #+#                 */
/*   Updated: 2020/04/15 14:46:19 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strtok(char *str, const char *delim)
{
	static char *oldstr;
	return (ft_strtok_r(str, delim, &oldstr));
}
