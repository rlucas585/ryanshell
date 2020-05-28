/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/03 20:49:54 by tbruinem      #+#    #+#                 */
/*   Updated: 2020/05/04 20:59:47 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int		vec_new(t_vec *vector, size_t type_size)
{
	vector->capacity = 4;
	vector->index = 0;
	vector->type_size = type_size;
	vector->store = malloc(sizeof(type_size) * (vector->capacity));
	if (!vector->store)
		return (0);
	return (1);
}

int		vec_add(t_vec *vector, void *buffer)
{
	char	*new_store;

	if (vector->index >= vector->capacity)
	{
		vector->capacity *= 2;
		new_store = malloc(sizeof(vector->type_size) * (vector->capacity));
		if (!new_store)
			return (0);
		ft_memcpy(new_store, vector->store,
			(vector->type_size * vector->index));
		free(vector->store);
		vector->store = new_store;
	}
	ft_memcpy(&vector->store[vector->index * vector->type_size],
		buffer, vector->type_size);
	vector->index++;
	return (1);
}

int		vec_get(t_vec *vector, void *buffer, size_t index)
{
	if (!vector || !buffer || index > vector->index)
		return (0);
	ft_memcpy(buffer,
		&vector->store[index * vector->type_size], vector->type_size);
	return (1);
}

int		vec_getref(t_vec *vector, void **buffer, size_t index)
{
	if (!vector || !buffer || index > vector->index)
		return (0);
	*buffer = &vector->store[index * vector->type_size];
	return (1);
}

void	vec_destroy(t_vec *vector, void (*del)(void *))
{
	if (del)
		del(vector);
	free(vector->store);
	vector->capacity = 0;
	vector->index = 0;
	vector->type_size = 0;
	vector->store = NULL;
}
