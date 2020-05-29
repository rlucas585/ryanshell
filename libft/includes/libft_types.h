/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   libft_types.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 20:03:02 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/29 20:07:27 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_TYPES_H
# define LIBFT_TYPES_H

# include <stddef.h>

typedef struct		s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct		s_gnlopts
{
	int				ret;
	int				more;
	char			*buf;
}					t_gnlopts;

typedef struct		s_vector
{
	void			**items;
	size_t			capacity;
	size_t			total;
}					t_vector;

#endif
