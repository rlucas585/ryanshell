/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_realloc.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 18:41:57 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/02 10:46:58 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

/*
** ft_realloc() creates a new allocated region of memory, copies the 
** contents from the previous memory, then frees the previous memory.
*/

void	*ft_realloc(void *ptr, size_t oldsize, size_t newsize)
{
	char	*new;

	new = (char *)ft_calloc(1, newsize);
	if (!new)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (new);
	ft_memcpy(new, ptr, oldsize);
	free(ptr);
	return (new);
}
