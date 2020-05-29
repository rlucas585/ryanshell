/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_init.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 18:44:45 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/29 18:45:06 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

/*
** Return 1 on error. 0 on success.
*/

int		vector_init(t_vector *v)
{
	v->capacity = 4;
	v->total = 0;
	v->items = malloc(sizeof(void *) * v->capacity);
	if (!v->items)
		return (1);
	return (0);
}
