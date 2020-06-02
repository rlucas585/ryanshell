/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vecstr_insert.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/01 20:59:07 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/02 10:48:08 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

/*
** Return 1 on error. 0 on success.
*/

int		vecstr_append_c(t_vecstr *v, int c)
{
	if (v->capacity - 1 == v->len)
		if (vecstr_resize(v, v->capacity * 2))
			return (1);
	v->str[v->len] = c;
	v->len++;
	v->str[v->len] = '\0';
	return (0);
}

int		vecstr_insert_str(t_vecstr *v, size_t index, const char *str2)
{
	size_t	multiplier;

	multiplier = 2;
	if (index > v->len)
		return (0);
	if (ft_strlen(str2) + v->len > v->capacity)
	{
		while (v->capacity * multiplier < ft_strlen(str2) + v->len + 1)
			multiplier *= 2;
		if (vecstr_resize(v, v->capacity * 2))
			return (1);
	}
	ft_memmove(v->str + index + ft_strlen(str2), v->str + index,
			v->len - index);
	ft_memmove(v->str + index, str2, ft_strlen(str2));
	v->str[v->len + ft_strlen(str2)] = '\0';
	v->len += ft_strlen(str2);
	return (0);
}

