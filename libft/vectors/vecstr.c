/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vecstr.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/01 16:38:36 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/01 22:57:56 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

/*
** Return 1 on error. 0 on success.
*/

int		vecstr_resize(t_vecstr *v, size_t new_capacity)
{
	if (new_capacity > v->capacity)
		v->str = ft_realloc(v->str, sizeof(char *) * v->capacity,
				sizeof(char *) * new_capacity);
	else
		v->str = ft_realloc(v->str, sizeof(char *) * new_capacity,
				sizeof(char *) * new_capacity);
	if (!v->str)
		return (1);
	v->capacity = new_capacity;
	return (0);
}

size_t	vecstr_len(t_vecstr *v)
{
	return (v->len);
}

int		vecstr_init(t_vecstr *v)
{
	v->capacity = 4;
	v->len = 0;
	v->str = (char *)ft_calloc(v->capacity, sizeof(void *));
	if (!v->str)
		return (1);
	return (0);
}
