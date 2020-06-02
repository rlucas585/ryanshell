/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vecstr_delete.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/01 21:00:44 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/02 10:20:51 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

/*
** Return 1 on error. 0 on success.
*/

int		vecstr_slice(t_vecstr *v, size_t start, size_t end)
{
	size_t		move_len;
	size_t		removed;

	move_len = v->len - end;
	removed = end - start;
	if (start >= v->len || end >= v->len + 1 || start >= end)
		return (0);
	ft_memmove(v->str + start, v->str + end, move_len);
	v->str[v->len - removed] = '\0';
	v->len -= removed;
	if (v->len <= v->capacity / 4)
		if (vecstr_resize(v, v->capacity / 2))
			return (1);
	return (0);
}

int		vecstr_slice_by_needle(t_vecstr *v, const char *needle)
{
	char		*tmp;

	tmp = ft_strnstr(v->str, needle, v->len);
	if (!tmp)
		return (0);
	return (vecstr_slice(v, tmp - v->str, tmp - v->str + ft_strlen(needle)));
}

int		vecstr_truncate(t_vecstr *v, size_t truncpoint)
{
	if (truncpoint >= v->len)
		return (0);
	v->str[truncpoint] = '\0';
	v->len = truncpoint;
	if (v->len <= v->capacity / 4)
		if (vecstr_resize(v, v->capacity / 2))
			return (1);
	return (0);
}

int		vecstr_reset(t_vecstr *v)
{
	v->capacity = 4;
	v->len = 0;
	v->str = ft_realloc(v->str, sizeof(char *) * v->capacity,
			sizeof(char *) * v->capacity);
	if (!v->str)
		return (1);
	ft_bzero(v->str, v->capacity);
	return (0);
}
