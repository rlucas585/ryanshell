/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vecstr_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/01 21:39:01 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/01 21:41:57 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

int		vecstr_val(t_vecstr *v, size_t i)
{
	if (i > v->len)
		return (-1);
	return (v->str[i]);
}
