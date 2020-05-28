/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ryantokens.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/05 23:24:42 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/27 23:41:08 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

void		conv_tokens(t_ryantok *tokens, size_t totaltokens, t_vec *args,
		t_vec *types)
{
	size_t i;
	void	*nullptr;
	int		separator;

	nullptr = NULL;
	separator = SEPARATOR;
	i = 0;
	vec_new(args, sizeof(char *));
	vec_new(types, sizeof(int));
	while (i < totaltokens)
	{
		if ( i && (tokens[i].type == COMMAND || tokens[i].type == PIPEDCOMMAND))
		{
			vec_add(args, &nullptr);
			vec_add(types, &separator);
		}
		vec_add(args, &tokens[i].value);
		vec_add(types, &tokens[i].type);
		i++;
	}
	vec_add(args, &nullptr);
}

void		assign_token_indexes(char *line, t_ryantok *tokens)
{
	size_t		i;

	i = 0;
	while (tokens[i].value)
	{
		tokens[i].value = line + tokens[i].index;
		i++;
	}
}

void		tokenizer(t_msh *prog, t_vec *args, t_vec *types)
{
	t_ryantok		*tokens;
	size_t			totaltokens;

	totaltokens = sum_tokens(prog->line.cmd);
	ft_printf("sum = %u\n", totaltokens);
	tokens = (t_ryantok *)malloc(sizeof(t_ryantok) * (totaltokens + 1));
	gen_tokens(&tokens, prog);
	assign_token_indexes(prog->line.cmd, tokens);
	print_tokens(tokens);
	conv_tokens(tokens, totaltokens, args, types);
    free(tokens);
}
