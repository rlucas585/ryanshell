/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tok.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/14 17:15:28 by tbruinem      #+#    #+#                 */
/*   Updated: 2020/05/24 13:28:19 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdio.h>

void	tokprint(t_token *list)
{
	size_t				i;
	static const char	*tokentypes[] = {
	[EOC] = "END_OF_COMMAND",
	[IN_REDIR] = "IN_REDIR",
	[PIPE] = "PIPE",
	[APPEND] = "APPEND",
	[TRUNC] = "TRUNC",
	[DEFAULT] = "DEFAULT"
	};

	i = 1;
	while (list)
	{
		ft_printf("token %ld = %12s | %s\n", i, list->content, tokentypes[list->type]);
		list = list->next;
		i++;
	}
}

void	tokclear(t_token *list, void (*del)(void *))
{
	t_token *clear;

	while (list)
	{
		clear = list;
		list = list->next;
		del(clear->content);
		free(clear);
	}
}

void	toksuffix(t_token **list, t_token *add)
{
	t_token	*tmp;

	if (!list)
		return ;
	tmp = *list;
	if (!tmp)
	{
		*list = add;
		return ;
	}
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = add;
}

int		new_tok(t_vec *args, t_vec *argtypes, char *tok, int type)
{
	if (!vec_add(args, &tok))
		return (2);
	if (!vec_add(argtypes, &type))
		return (2);
	return (0);
}

void	add_to_tok(t_lexer *lex, char *last, char *exception)
{
	if (!ft_strchr(exception, *lex->raw))
	{
		lex->token[lex->index] = *lex->raw;
		lex->index++;
	}
	*last = *lex->raw;
	lex->raw++;
}

void	escaper(t_lexer *lex, char *last)
{
	add_to_tok(lex, last, "");
}

void	stringquote(t_lexer *lex)
{
	char	last;

	last = 0;
	lex->raw++;
	while (*lex->raw && (*lex->raw != '"' || last == '\\'))
	{
		if (last == '\\')
			escaper(lex, &last);
		else if (*(lex->raw + 1) == '\"')
			add_to_tok(lex, &last, "\\");
		else
			add_to_tok(lex, &last, "");
	}
	if (*lex->raw != '"')
	{
//		parse_error;
		perror("Unterminated stringquote");
		exit(1);
	}
	lex->raw++;
}

void	literalquote(t_lexer *lex)
{
	char	last;

	last = 0;
	lex->raw++;
	while (*lex->raw && *lex->raw != '\'')
	{
		if (last == '\\')
			escaper(lex, &last);
		else
			add_to_tok(lex, &last, "\'");
	}
	if (*lex->raw != '\'')
	{
//		parse_error;
		perror("Unterminated literalquote");
		exit(1);
	}
	lex->raw++;
}

int		special_tok(t_vec *args, t_vec *argtypes, t_lexer *lex)
{
	char	*raw;

	raw = lex->raw;
	lex->raw++;
	if (raw[0] == '>' && raw[1] == '>')
	{
		lex->raw++;
		return (new_tok(args, argtypes, ft_strdup(">>"), APPEND));
	}
	if (raw[0] == '&' && raw[1] == '&')
	{
		lex->raw++;
		return (new_tok(args, argtypes, ft_strdup("&&"), EOC));
	}
	if (raw[0] == '>')
		return (new_tok(args, argtypes, ft_strdup(">"), TRUNC));
	if (raw[0] == '|')
		return (new_tok(args, argtypes, ft_strdup("|"), PIPE));
	if (raw[0] == '<')
		return (new_tok(args, argtypes, ft_strdup("<"), IN_REDIR));
	if (raw[0] == ';')
		return (new_tok(args, argtypes, ft_strdup(";"), EOC));
	return (2); //error
}

void	token_creation(t_lexer *lex, char *last)
{
	if (*last == '\\')
		escaper(lex, last);
	else if (*lex->raw == '"')
		stringquote(lex);
	else if (*lex->raw == '\'')
		literalquote(lex);
	else
		add_to_tok(lex, last, "\\");
}

int		gen_token(t_vec *args, t_vec *argtypes, char *raw)
{
	char			last;
	static t_lexer	lex;

	last = 0;
	if (raw)
		lex.raw = raw;
	lex.index = 0;
	if (ft_strchr(";|><&", *lex.raw))
		return (special_tok(args, argtypes, &lex));
	while (lex.index < MAX_TOKSIZE && *lex.raw &&
	(!ft_strchr("; |><&", *lex.raw) || last == '\\'))
		token_creation(&lex, &last);
	lex.token[lex.index] = '\0';
	if (lex.index == 0 && *lex.raw != ' ')
		return (1);
	if (*lex.raw == ' ')
		lex.raw++;
	return ((lex.index) ?
		new_tok(args, argtypes, ft_strdup(lex.token), DEFAULT) :
		gen_token(args, argtypes, NULL));
}

int		tokenize(t_vec *args, t_vec *argtypes, char *raw)
{
	int		done;
	char	*temp;

	temp = NULL;
	if (!vec_new(args, sizeof(char *)))
		*temp = 0;
	if (!vec_new(argtypes, sizeof(int)))
		*temp = 0;
	done = gen_token(args, argtypes, raw);
	while (!done)
		done = gen_token(args, argtypes, NULL);
	if (new_tok(args, argtypes, NULL, EOC) == 2)
		*temp = 0;
	return (1);
}
