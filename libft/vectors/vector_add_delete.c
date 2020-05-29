/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_add_delete.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 18:47:19 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/29 20:11:31 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <libft_types.h>

/*
** Return 1 on error. 0 on success.
*/

static int	vector_resize(t_vector *v, size_t capacity)
{
	void	**items;

	items = ft_realloc(v->items, sizeof(void *) * capacity);
	if (!items)
		return (1);
	v->items = items;
	v->capacity = capacity;
	return (0);
}

int			vector_add(t_vector *v, void *item)
{
	if (v->capacity == v->total)
		if (vector_resize(v, v->capacity * 2))
			return (1);
	v->items[v->total] = item;
	v->total++;
	return (0);
}

int			vector_delete(t_vector *v, size_t index)
{
	size_t		i;

	i = index;
	if (index >= v->total)
		return (0);
	v->items[index] = NULL;
	while (i < v->total)
	{
		v->items[i] = v->items[i + 1];
		v->items[i + 1] = NULL;
		i++;
	}
	v->total--;
	if (v->total > 0 && v->total == v->capacity / 4)
		if (vector_resize(v, v->capacity / 2))
			return (1);
	return (0);
}
