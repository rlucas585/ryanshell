/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_set_get.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 18:48:38 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/29 20:12:14 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft_types.h>

void		vector_set(t_vector *v, size_t index, void *item)
{
	if (index < v->total)
		v->items[index] = item;
}

void		*vector_get(t_vector *v, size_t index)
{
	if (index < v->total)
		return (v->items[index]);
	return (NULL);
}
