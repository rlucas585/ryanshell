/* **************************************************************************
 * */
/*                                                                            */
/*                                                        ::::::::            */
/*   listutils1.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/16 14:32:03 by rlucas        #+#    #+#                 */
/*   Updated: 2020/04/16 14:45:08 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

char	*get_env_value(const char *targetvar, t_node *head)
{
	t_node		*current;
	char		*value;

	current = head;
	while (current != NULL)
	{
		value = (char *)current->data;
		if(ft_strncmp(targetvar, value, ft_strlen(targetvar)) == 0)
			if (value[ft_strlen(targetvar)] == '=')
				return (ft_strchr(value, '=') + 1);
		current = current->next;
	}
	return (NULL);
}
