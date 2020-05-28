/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lists.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/16 13:17:24 by rlucas        #+#    #+#                 */
/*   Updated: 2020/04/16 14:22:32 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

t_node		*new_node(void *data)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->data = data;
	node->next = NULL;
	return (node);
}

int			new_node_at_front(void *data, t_node **head)
{
	t_node	*new;

	if (*head == NULL)
	{
		*head = new_node(data);
		if (*head == NULL)
			return (1);
		else
			return (0);
	}
	new = new_node(data);
	if (!new)
		return (1);
	new->next = *head;
	*head = new;
	return (0);
}

int			new_node_at_back(void *data, t_node **head)
{
	t_node	*current;
	t_node	*new;

	if (*head == NULL)
		return (new_node_at_front(data, head));
	new = new_node(data);
	if (!new)
		return (1);
	current = *head;
	while (current->next != NULL)
		current = current->next;
	current->next = new;
	return (0);
}
