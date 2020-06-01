/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   gen_tokens.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/26 13:10:59 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/01 22:34:08 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

static int	state_action(char *line, t_ryanlexer *lex)
{
	if (lex->state == SEMICOLON || lex->state == PIPE_PIPE)
	{
		lex->cmd_num++;
		lex->cmd_present = 0;
		lex->nexttype = COMMAND;
		if (lex->state == PIPE_PIPE)
			lex->pipe = 1;
	}
	if (lex->state == OREDIRECT)
	{
		if (lex->nexttype == WRITEFILE || lex->nexttype == APPENDFILE)
			return (1); /* Parsing error */
		if (line[lex->i + 1] == '>')
		{
			lex->nexttype = APPENDFILE;
			line[lex->i] = '\0';
			lex->i++;
		}
		else
			lex->nexttype = WRITEFILE;
	}
	if (lex->state == IREDIRECT)
		lex->nexttype = INPUT_SENDER;
	lex->state = WHITESPACE;
	return (0);
}

void		add_env_value(t_ryanlexer *lex, t_vecstr *line, size_t env_name_len,
		t_msh *prog)
{
	char		*env_value;

	env_value = env_val_get(vecstr_get(line) + lex->i + 1,
			prog, env_name_len);
	vecstr_slice(line, lex->i, lex->i + 1);
	if (!env_value) {
		if (lex->state != INDOUBLEQUOTE)
		{
			vecstr_insert_str(line, lex->i, " ");
			lex->state = checkstate(vecstr_val(line, lex->i), *lex);
			vecstr_set(line, lex->i, '\0');
		}
		return ;
	}
	else
		vecstr_insert_str(line, lex->i, env_value);
	lex->i = lex->i + ft_strlen(env_value) - 1;
	if (lex->state != INDOUBLEQUOTE)
		lex->state = checkstate(vecstr_val(line, lex->i), *lex);
}

size_t		env_strclen(char *line, const char *chars)
{
	size_t		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\\')
		{
			if (!line[i + 1])
				return (i + 1);
			i += 2;
			continue;
		}
		if (ft_strchr(chars, line[i]))
			return (i);
		i++;
	}
	return (i);
}

void		expand_env_value(t_ryanlexer *lex, t_vecstr *line, t_msh *prog)
{
	size_t		env_name_len;
	char		*env_name;

	env_name = vecstr_get(line) + lex->i + 1;
	env_name_len = env_strclen(env_name, " \";<>.|\'");
	if (lex->state == INDOUBLEQUOTE &&
			ft_strclen(env_name, '"') < env_name_len)
		env_name_len = ft_strclen(env_name, '"');
	add_env_value(lex, line, env_name_len, prog);
}

void		evaluate_env(t_ryanlexer *lex, t_vecstr *line, t_msh *prog)
{
	int		c;

	c = vecstr_val(line, lex->i);
	if (c == 0)
		return ;
	if (c == '(')
		(void)line; /* Initiate subshell */
	if (c == '$')
		(void)line; /* Get process id of shell */
	if (ft_isalpha(c) || c == '_')
		expand_env_value(lex, line, prog);
}

void		quote_toks(t_ryantok **tokens, t_ryanlexer *lex, t_vecstr *line,
		t_msh *prog)
{
	vecstr_slice(line, lex->i, lex->i + 1);
	if (lex->prevstate == WHITESPACE)
		create_token((*tokens) + lex->tokeni, lex);
	while (vecstr_val(line, lex->i))
	{
		if (check_esc_char(line, lex, 1))
			continue ;
		if (!lex->escape && vecstr_val(line, lex->i) == '$' &&
				lex->state != INSINGLEQUOTE)
		{
			evaluate_env(lex, line, prog);
			continue ;
		}
		update_lexer(vecstr_get(line), lex);
		if (lex->state == NORMAL)
			break ;
		lex->escape = 0;
		lex->i++;
	}
	vecstr_slice(line, lex->i, lex->i + 1);
	lex->i--;
}

void		gen_tokens(t_ryantok **tokens, t_vecstr *line, t_msh *prog)
{
	t_ryanlexer		lex;

	init_lexer(&lex);
	while (vecstr_val(line, lex.i))
	{
		if (check_esc_char(line, &lex, 1))
			continue ;
		update_lexer(vecstr_get(line), &lex);
		/* print_state(prog->line.cmd[lex.i], lex); #<{(| Troubleshooting |)}># */
		if (!lex.escape && lex.state >= INDOUBLEQUOTE &&
				lex.state <= INSINGLEQUOTE)
			quote_toks(tokens, &lex, line, prog);
		if (!lex.escape && lex.state >= SEMICOLON && lex.state <= PIPE_PIPE)
			if(state_action(vecstr_get(line), &lex))
				exit(1); /* Error in parsing */
		if (lex.state != WHITESPACE && lex.prevstate == WHITESPACE)
			create_token((*tokens) + lex.tokeni, &lex);
		if (!lex.escape && vecstr_val(line, lex.i) == '$' &&
				lex.state != INSINGLEQUOTE)
			evaluate_env(&lex, line, prog);
		if (lex.state == WHITESPACE)
			vecstr_set(line, lex.i, '\0');
		lex.escape = 0;
		lex.i++;
	}
	(*tokens)[lex.tokeni].value = NULL;
}
