/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexutils.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/06 21:18:20 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/27 22:57:59 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

void		init_lexer(t_ryanlexer *lex)
{
	lex->i = 0;
	lex->j = 0;
	lex->state = WHITESPACE;
	lex->escape = 0;
	lex->tokeni = 0;
	lex->nexttype = COMMAND;
	lex->cmd_present = 0;
	lex->cmd_num = 0;
	lex->pipe = 0;
}

void		update_lexer(char *line, t_ryanlexer *lex)
{
	lex->prevstate = lex->state;
	lex->state = checkstate(line[lex->i], *lex);
	if (lex->prevstate == NORMAL && lex->state != NORMAL)
		lex->j = lex->i;
}

int			check_esc_char(char *line, t_ryanlexer *lex, int gen_true)
{
	if (line[lex->i] == '\\' && lex->escape == 0 && lex->state != INSINGLEQUOTE)
	{
		if (gen_true)
			mash_string(line, lex->i, lex->i + 1);
		lex->escape = 1;
		return (1);
	}
	return (0);
}

static void	assign_tok_type(t_ryantok *token, t_ryanlexer *lex)
{
	if (lex->nexttype == COMMAND)
	{
		lex->cmd_present = 1;
		if (lex->pipe == 1)
			lex->nexttype = PIPEDCOMMAND;
		lex->pipe = 0;
	}
	token->type = lex->nexttype;
	if (lex->cmd_present == 0)
		lex->nexttype = COMMAND;
	else
		lex->nexttype = STANDARD;
}

void	create_token(t_ryantok *token, t_ryanlexer *lex)
{
	token->index = lex->i;
	token->value = (char *)1;
	token->cmd_num = lex->cmd_num;
	assign_tok_type(token, lex);
	lex->tokeni++;
}
