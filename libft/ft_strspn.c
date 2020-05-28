/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strspn.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/15 14:42:30 by rlucas        #+#    #+#                 */
/*   Updated: 2020/04/15 14:42:58 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

size_t	ft_strspn(const char *s, const char *accept)
{
	size_t		i;
	size_t		j;

	i = 0;
	while (s[i])
	{
		j = 0;
		while (accept[j])
		{
			if (s[i] != accept[j])
				j++;
			else
			{
				i++;
				break ;
			}
		}
		if (accept[j] == '\0')
			return (i);
	}
	return (i);
}
