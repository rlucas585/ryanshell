/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vecstr_get_set.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/01 22:06:58 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/01 22:07:58 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	vecstr_set(t_vecstr *v, size_t index, int c)
{
	if (index > v->len)
		return ;
	v->str[index] = c;
}

char	*vecstr_get(t_vecstr *v)
{
	return (v->str);
}
