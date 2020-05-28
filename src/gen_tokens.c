/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   gen_tokens.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/26 13:10:59 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/28 09:52:04 by rlucas        ########   odam.nl         */
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

void		mash_string(char *line, size_t dest, size_t src)
{
	size_t		tail;

	tail = ft_strlen(line + src) + src - (src - dest);
	ft_memmove(line + dest, line + src, ft_strlen(line + src));
	while (line[tail])
	{
		line[tail] = '\0';
		tail++;
	}
}

char		*env_val_get_n(const char *name, t_var *env, size_t len)
{
	t_var		*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, name, len) == 0)
			return (tmp->val);
		tmp = tmp->next;
	}
	return (NULL);
}

void		dangerous_print(char *start, size_t len)
{
	size_t		i;

	i = 0;
	while (i <= len)
	{
		if (start[i] == '\0')
			ft_putchar('0');
		else
			ft_putchar(start[i]);
		i++;
	}
}

void		insert_into_string(t_ryanlexer *lex, t_msh *prog, char *env_value)
{
	size_t		end;
	size_t		env_value_len;

	env_value_len = ft_strlen(env_value);
	end = ft_strlen(prog->line.cmd + lex->i) + lex->i;
	if (end + env_value_len > prog->line.alloced_cmd)
	{
		prog->line.cmd = ft_realloc(prog->line.cmd, end + env_value_len + 1);
		prog->line.alloced_cmd = end + env_value_len;
		/* ft_bzero(prog->line.cmd + end, prog->line.alloced_cmd - end + 1); */
	}
	ft_memmove(prog->line.cmd + lex->i + env_value_len, prog->line.cmd + lex->i,
			ft_strlen(prog->line.cmd + lex->i));
	ft_memmove(prog->line.cmd + lex->i, env_value, ft_strlen(env_value));
}

void		add_env_value(t_ryanlexer *lex, t_msh *prog, size_t env_name_len)
{
	char		*env_value;

	env_value = env_val_get_n(prog->line.cmd + lex->i + 1,
			prog->env, env_name_len);
	mash_string(prog->line.cmd, lex->i, lex->i + env_name_len + 1);
	if (!env_value)
	{
		if (lex->state != INDOUBLEQUOTE)
		{
			insert_into_string(lex, prog, " ");
			lex->state = checkstate(prog->line.cmd[lex->i], *lex);
			prog->line.cmd[lex->i] = '\0';
		}
		return ;
	}
	else
		insert_into_string(lex, prog, env_value);
	lex->i = lex->i + ft_strlen(env_value) - 1;
	if (lex->state != INDOUBLEQUOTE)
		lex->state = checkstate(prog->line.cmd[lex->i], *lex);
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

void		expand_env_value(t_ryanlexer *lex, t_msh *prog)
{
	size_t		env_name_len;

	env_name_len = env_strclen(prog->line.cmd + lex->i + 1, " \";<>.|\'");
	if (lex->state == INDOUBLEQUOTE &&
			ft_strclen(prog->line.cmd + lex->i + 1, '"') < env_name_len)
		env_name_len = ft_strclen(prog->line.cmd + lex->i + 1, '"');
	add_env_value(lex, prog, env_name_len);
}

void		evaluate_env(t_ryanlexer *lex, t_msh *prog)
{
	if (!prog->line.cmd[lex->i + 1])
		return ;
	if (prog->line.cmd[lex->i + 1] == '(')
		(void)prog; /* Initiate subshell */
	if (prog->line.cmd[lex->i + 1] == '$')
		(void)prog; /* Get process id of shell */
	if (ft_isalpha(prog->line.cmd[lex->i + 1]) || prog->line.cmd[lex->i + 1] ==
			'_')
		expand_env_value(lex, prog);
}

void		quote_toks(t_ryantok **tokens, t_ryanlexer *lex,
		t_msh *prog)
{
	mash_string(prog->line.cmd, lex->i, lex->i + 1);
	if (lex->prevstate == WHITESPACE)
		create_token((*tokens) + lex->tokeni, lex);
	while (prog->line.cmd[lex->i])
	{
		if (check_esc_char(prog->line.cmd, lex, 1))
			continue ;
		if (!lex->escape && prog->line.cmd[lex->i] == '$' &&
				lex->state != INSINGLEQUOTE)
		{
			evaluate_env(lex, prog);
			continue ;
		}
		update_lexer(prog->line.cmd, lex);
		if (lex->state == NORMAL)
			break ;
		lex->escape = 0;
		lex->i++;
	}
	mash_string(prog->line.cmd, lex->i, lex->i + 1);
	lex->i--;
}

void		gen_tokens(t_ryantok **tokens, t_msh *prog)
{
	t_ryanlexer		lex;

	init_lexer(&lex);
	while (prog->line.cmd[lex.i])
	{
		if (check_esc_char(prog->line.cmd, &lex, 1))
			continue ;
		update_lexer(prog->line.cmd, &lex);
		/* print_state(prog->line.cmd[lex.i], lex); #<{(| Troubleshooting |)}># */
		if (!lex.escape && lex.state >= INDOUBLEQUOTE &&
				lex.state <= INSINGLEQUOTE)
			quote_toks(tokens, &lex, prog);
		if (!lex.escape && lex.state >= SEMICOLON && lex.state <= PIPE_PIPE)
			if(state_action(prog->line.cmd, &lex))
				exit(1); /* Error in parsing */
		if (lex.state != WHITESPACE && lex.prevstate == WHITESPACE)
			create_token((*tokens) + lex.tokeni, &lex);
		if (!lex.escape && prog->line.cmd[lex.i] == '$' &&
				lex.state != INSINGLEQUOTE)
			evaluate_env(&lex, prog);
		if (lex.state == WHITESPACE)
			prog->line.cmd[lex.i] = '\0';
		lex.escape = 0;
		lex.i++;
	}
	(*tokens)[lex.tokeni].value = NULL;
}
