/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   inputconcat.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/06 21:15:27 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/26 12:42:19 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

/* static void	concatenate_quotes(char *line) */
/* { */
/* 	t_ryanlexer		lex; */
/*  */
/* 	init_lexer(&lex); */
/* 	while (line[lex.i]) */
/* 	{ */
/* 		if (check_esc_char(line, &lex)) */
/* 			continue ; */
/* 		if (!lex.escape) */
/* 			update_lexer(line, &lex); */
/* 		if ((lex.prevstate == INDOUBLEQUOTE || lex.prevstate == INSINGLEQUOTE) */
/* 				&& lex.state == NORMAL) */
/* 			if (line[lex.i + 1] == '\"' || line[lex.i + 1] == '\'') */
/* 			{ */
/* 				(*ft_strchr(line + lex.i + 2, line[lex.i + 1])) = line[lex.i]; */
/* 				ft_memmove(line + lex.i, line + lex.i + 2, ft_strlen(line + */
/* 							lex.i + 2)); */
/* 				lex.j = ft_strlen(line); */
/* 				line[lex.j - 1] = '\0'; */
/* 				line[lex.j - 2] = '\0'; */
/* 				lex.i = 0; */
/* 				continue ; */
/* 			} */
/* 		lex.escape = 0; */
/* 		lex.i++; */
/* 	} */
/* } */
/*  */
/* static int	last_word_concat(char *line, t_ryanlexer *lex, unsigned char c) */
/* { */
/* 	lex->j = ft_strlen(line + lex->i + 1); */
/* 	ft_memmove(line + lex->i, line + lex->i + 1,  lex->j); */
/* 	line[lex->i + lex->j] = c; */
/* 	return (1); */
/* } */
/*  */
/* static int	concat_no_spaces(char *line, t_ryanlexer *lex) */
/* { */
/* 	unsigned char	c; */
/*  */
/* 	c = line[lex->i]; */
/* 	if (!ft_strchr(line + lex->i + 1, ' ')) */
/* 	{ */
/* 		if (!ft_strchr(line + lex->i + 1, line[lex->i])) */
/* 			return (last_word_concat(line, lex, c)); */
/* 		lex->j = ft_strchr(line + lex->i + 1, line[lex->i]) - line - 1; */
/* 		ft_memmove(line + lex->i, line + lex->i + 1, */
/* 				ft_strlen(line + lex->i + 1)); */
/* 		ft_memmove(line + lex->j, line + lex->j + 1, */
/* 				ft_strlen(line + lex->j + 1)); */
/* 		lex->j = ft_strlen(line); */
/* 		line[lex->j - 1] = '\0'; */
/* 		line[lex->j - 2] = '\0'; */
/* 		return (1); */
/* 	} */
/* 	lex->j = ft_strchr(line + lex->i + 1, ' ') - line - 1; */
/* 	ft_memmove(line + lex->i, line + lex->i + 1, lex->j - lex->i); */
/* 	line[lex->j] = c; */
/* 	lex->state = (lex->prevstate == INDOUBLEQUOTE) ? INDOUBLEQUOTE : */
/* 		INSINGLEQUOTE; */
/* 	ft_printf("line = %s\n", line); */
/* 	return (0); */
/* } */
/*  */
/* void		concatenate_input(char *line) */
/* { */
/* 	t_ryanlexer		lex; */
/*  */
/* 	init_lexer(&lex); */
/* 	while (line[lex.i]) */
/* 	{ */
/* 		if (check_esc_char(line, &lex)) */
/* 			continue ; */
/* 		update_lexer(line, &lex); */
/* 		if (!lex.escape && lex.state == NORMAL && */
/* 				(lex.prevstate == INDOUBLEQUOTE || lex.prevstate == */
/* 				 INSINGLEQUOTE)) */
/* 			if (line[lex.i + 1] && !ft_is_whitespace(line[lex.i + 1]) && */
/* 					line[lex.i + 1] != ';') */
/* 				if (concat_no_spaces(line, &lex)) */
/* 					continue ; */
/* 		concatenate_quotes(line); */
/* 		lex.escape = 0; */
/* 		lex.i++; */
/* 	} */
/* 	ft_printf("concat line result: %s\n", line); */
/* } */

