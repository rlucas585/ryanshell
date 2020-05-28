/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   format_toks.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/19 14:30:29 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/20 17:39:16 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

/*
** Points the token to an env value. Not suitable if env value is within
** a double quote and needs concatenation with other values.
*/

/* void	point_to_env(t_ryantok *token, t_msh *prog) */
/* { */
/* 	char *newvalue; */
/*  */
/* 	newvalue = env_val_get(token->value, prog->env); */
/* 	if (!newvalue) */
/* 	{ */
/* 		while (*(token->value) != '\0') */
/* 			token->value++; */
/* 		return ; */
/* 	} */
/* 	token->value = newvalue; */
/* 	return ; */
/* } */

/* void	format_toks(t_ryantok *toks, t_msh *prog) */
/* { */
/* 	size_t		i; */
/*  */
/* 	i = 0; */
/* 	while (toks[i].value) */
/* 	{ */
/* 		if (toks[i].quotes == DOUBLE) */
/* 			evaluate_double_quotes(&toks[i], prog); */
/* 		if (toks[i].quotes == SINGLE) */
/* 			toks[i].quotes = NONE; */
/* 		if (toks[i].env == 1) */
/* 			point_to_env(&toks[i], prog); */
/* 	} */
/* } */
