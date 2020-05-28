/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strtok_r.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/15 14:45:18 by rlucas        #+#    #+#                 */
/*   Updated: 2020/04/15 14:45:44 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strtok_r(char *str, const char *delim, char **save_ptr)
{
	char	*end;

	if (str == NULL)
		str = *save_ptr;
	if (str == NULL || *str == '\0')
	{
		*save_ptr = NULL;
		return (NULL);
	}
	str += ft_strspn(str, delim);
	if (*str == '\0')
	{
		save_ptr = NULL;
		return (NULL);
	}
	end = str + ft_strcspn(str, delim);
	if (*end == '\0')
	{
		*save_ptr = NULL;
		return (str);
	}
	*end = '\0';
	*save_ptr = end + 1;
	return (str);
}
