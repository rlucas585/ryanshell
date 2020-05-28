/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strncmp.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/30 08:57:35 by rlucas        #+#    #+#                 */
/*   Updated: 2020/01/17 14:45:00 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

int			ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int			i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] != '\0' && n > 0)
	{
		if (s1[i] != s2[i])
			break ;
		n--;
		if (n == 0)
			break ;
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
