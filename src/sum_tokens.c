/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sum_tokens.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/26 12:30:45 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/27 22:58:41 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

void			print_state(int c, t_ryanlexer lex)
{
	static const char	*state_table[] = {
		[NORMAL] = "NORMAL",
		[WHITESPACE] = "WHITESPACE",
		[INDOUBLEQUOTE] = "INDOUBLEQUOTE",
		[INSINGLEQUOTE] = "INSINGLEQUOTE",
		[INBACKTICK] = "INBACKTICK",
		[SEMICOLON] = "SEMICOLON",
		[OREDIRECT] = "OREDIRECT",
		[IREDIRECT] = "IREDIRECT",
		[OAPPEND] = "OAPPEND",
		[PIPE_PIPE] = "PIPE_PIPE",
		[ENV] = "ENV"
	};

	ft_printf("%c:\tnum = %d\tescape: %d\tstate: %s\n",
			c, lex.state, lex.escape, state_table[lex.state]);
}

size_t			sum_tokens(char *line)
{
	t_ryanlexer		lex;
	size_t			sum;

	init_lexer(&lex);
	sum = 0;
	while (line[lex.i])
	{
		if (check_esc_char(line, &lex, 0))
			continue ;
		update_lexer(line, &lex);
		/* print_state((*line)[lex.i], lex); #<{(| Troubleshooting |)}># */
		if (lex.state >= SEMICOLON && lex.state <= PIPE_PIPE)
			lex.state = WHITESPACE;
		if (lex.state != WHITESPACE && lex.prevstate == WHITESPACE)
			sum++;
		lex.escape = 0;
		lex.i++;
	}
	return (sum);
}
