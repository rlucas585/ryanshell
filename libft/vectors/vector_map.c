/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_map.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 18:49:06 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/29 20:10:19 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft_types.h>

void		vector_map(t_vector *v, void (*f)(void *))
{
	size_t	i;

	i = 0;
	while (i < v->total)
	{
		f(v->items[i]);
		i++;
	}
}
