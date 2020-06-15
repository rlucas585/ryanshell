/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vec_arr.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/02 20:49:58 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/05 11:23:46 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

/*
** Returns 1 on error, 0 on success.
*/

int		vecarr_resize(t_vecarr *v, size_t new_capacity)
{
	void	*data;

	if (new_capacity > v->capacity)
		data = ft_realloc(v->data, v->capacity * v->elem_size,
				new_capacity * v->elem_size);
	else
		data = ft_realloc(v->data, new_capacity * v->elem_size,
				v->capacity * v->elem_size);
	if (!data)
		return (1);
	v->data = data;
	v->capacity = new_capacity;
	return (0);
}

int		vecarr_add(t_vecarr *v, void *new_item)
{
	size_t		index;

	if (v->capacity == v->total)
		if (vecarr_resize(v, v->capacity *2))
			return (1);
	index = v->total * v->elem_size;
	ft_memcpy(v->data + index, new_item, v->elem_size);
	v->total++;
	return (0);
}

int		vecarr_delete(t_vecarr *v, size_t index)
{
	size_t		vec_index;

	if (index >= v->total)
		return (0);
	vec_index = index * v->elem_size;
	ft_memcpy(v->data + vec_index, v->data + vec_index + v->elem_size,
			(v->total - index - 1) * v->elem_size);
	v->total--;
	if (v->total > 0 && v->total == v->capacity / 4)
		if (vecarr_resize(v, v->capacity / 2))
			return (1);
	return (0);
}

int		vecarr_delete_by_val(t_vecarr *v, void *val)
{
	size_t		i;

	i = 0;
	while (i < v->total)
	{
		if (ft_memcmp(vecarr_get(v, i), val, v->elem_size) == 0)
			return (vecarr_delete(v, i));
		i++;
	}
	return (0);
}

void	*vecarr_get(t_vecarr *v, size_t index)
{
	size_t		vec_index;

	if (index > v->total)
		return (NULL);
	vec_index = index * v->elem_size;
	return (v->data + vec_index);
}

int		vecarr_init(t_vecarr *v, size_t elem_size)
{
	v->capacity = 4;
	v->total = 0;
	v->elem_size = elem_size;
	v->data = malloc(v->capacity * v->elem_size);
	if (!v->data)
		return (1);
	return (0);
}

void	vecarr_reset(t_vecarr *v)
{
	free(v->data);
	v->capacity = 4;
	v->total = 0;
}
