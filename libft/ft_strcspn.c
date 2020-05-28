/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strcspn.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/15 14:43:34 by rlucas        #+#    #+#                 */
/*   Updated: 2020/04/15 14:43:46 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

size_t	ft_strcspn(const char *s, const char *reject)
{
	size_t		i;
	size_t		j;

	i = 0;
	while (s[i])
	{
		j = 0;
		while (reject[j])
		{
			if (s[i] == reject[j])
				break ;
			else
				j++;
		}
		if (reject[j] != '\0')
			return (i);
		i++;
	}
	return (i);
}

