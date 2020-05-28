/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/16 10:51:49 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/27 23:36:30 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define RIGHT -1
# define LEFT 1

/*
** Key codes
*/

# define CTRL_D 4
# define CTRL_L 12
# define CTRL_U 21
# define DEL 127
# define NEWLINE 10
# define ESC 27
# define CTRL 49
# define LEFT_KEY 68
# define RIGHT_KEY 67
# define UP_KEY 65
# define DOWN_KEY 66
# define HOME 72
# define END 70

# define MAX_TOKSIZE 300

# include <unistd.h>
# include <termios.h>

enum			e_toktype
{
	COMMAND = 0,
	STANDARD,
	WRITEFILE,
	APPENDFILE,
	SEPARATOR,
	PIPEDCOMMAND,
	INPUT_SENDER
};

enum			e_fsm
{
	NORMAL = 0,
	WHITESPACE,
	INDOUBLEQUOTE,
	INSINGLEQUOTE,
	INBACKTICK,
	SEMICOLON,
	OREDIRECT,
	IREDIRECT,
	OAPPEND,
	PIPE_PIPE,
	ENV
};

extern char	**g_termbuff;

typedef struct s_vec	t_vec;

struct	s_vec
{
	char			*store;
	size_t			type_size;
	size_t			capacity;
	size_t			index;
};

typedef struct s_var	t_var;

struct	s_var
{
	char		*name;
	char		*val;
	size_t		len;
	t_var		*next;
};

typedef struct s_cmd	t_cmd;

struct			s_cmd
{
	char	**args;
	int		*argtypes;
	int		cmdtype;
	int		iostream[2];
	int		cmdpipe[2];
	t_cmd	*next;
};

typedef struct	s_ryantok	t_ryantok;

struct			s_ryantok
{
	int			type;
	size_t		index;
	char		*value;
	int			cmd_num;
};

typedef struct	s_ryanlexer
{
	size_t		i;
	size_t		j;
	int			prevstate;
	int			state;
	int			escape;
	size_t		tokeni;
	int			nexttype;
	int			cmd_num;
	int			cmd_present;
	int			pipe;
}				t_ryanlexer;

typedef struct	s_ryancmd
{
	char		**args;
	char		*output;
	char		*input;
	char		*command;
}				t_ryancmd;

typedef struct	s_lexer
{
	char		token[MAX_TOKSIZE];
	char		*raw;
	size_t		index;
}				t_lexer;

typedef struct s_token	t_token;

struct			s_token
{
	char		*content;
	int			type;
	t_token		*next;
};

typedef struct	s_coord
{
	size_t		row;
	size_t		col;
}				t_coord;

typedef struct	s_line
{
	char			*prompt;
	size_t			promptlen;
	char			*cmd;
	size_t			cmd_len;
	size_t			alloced_cmd;
	size_t			inputrow;
	t_coord			max;
	t_coord			cursor;
	size_t			total_rows;
	int				escmode;
	char			*termtype;
	char			*cap_table;
	struct termios	term;
}				t_line;

typedef	struct	s_msh
{
	t_vec		process_arr;
	t_vec		file_arr;
	t_vec		argtypes;
	t_vec		args;
	t_cmd		*commands;
	char		**envp;
	t_var		*env;
	t_line		line;
	size_t		argc;
}				t_msh;

enum			e_error
{
	MEM_FAIL,
	TERM_FAIL,
	CAP_FAIL
};

enum			e_builtins
{
	B_CD,
	B_ECHO,
	B_PWD,
	B_EXPORT,
	B_ENV,
	B_UNSET,
	B_EXIT,
};

enum			e_pipe
{
	READ,
	WRITE,
};

enum			e_tokentypes
{
	PIPE,
	APPEND,
	TRUNC,
	IN_REDIR,
	EOC,
	DEFAULT,
};

typedef void	(*t_builtin)(t_msh *prog, int argc, char **argv);
typedef int		(*t_inputf)(t_line *line, char buf[6]);

/*
** Utility functions in utils.c
*/

char			*ft_realloc(char *str, size_t newsize);
char			*ft_str3join(const char *s1, const char *s2, const char *s3);
void			print_tokens(t_ryantok *tokens);

/*
** Add a prompt to the shell, in prompt.c 
*/

char			*prompt(t_msh *prog, t_line *line);

/*
** Initialise terminal, in terminal.c
*/

int				init_term(struct termios *term);
void			init_readline(t_msh *prog);

/*
** Functions to handle input and line editing. In add_char.c, delete_char.c,
** and move_cursor.c.
*/

int				add_char(t_line *line, char c);
int				esc_delete(t_line *line);
int				delete_char(t_line *line);
void			cursor_left(t_line *line);
void			cursor_right(t_line *line);
void			cursor_home(t_line *line);
void			cursor_end(t_line *line);
void			cursor_move_row(t_line *line, int c);
void			cursor_move_word(t_line *line, int c);

/*
** Lookup tables in tables.c.
*/

char			*error_lookup(int err);

/*
** Functions to free all allocated memory before exiting - erroneously or
** normally.
*/

typedef void	(*t_escapef)(t_lexer *lex, char *last);

int				execute(t_msh *prog, t_cmd *cmd);
char			**ft_str2clear(char **str);
t_cmd			*clear_commands(t_cmd *commands);
t_cmd			*get_commands(t_vec *args, int *types, t_vec *fd_arr);
void			print_command(t_cmd *command);
int				set_redirection(t_cmd *command, char **args,
								int *types, t_vec *fd_arr);

void			error_exit(t_msh *prog, int err);
void			std_exit(t_msh *prog);

int				vec_add(t_vec *vector, void *buffer);
int				vec_new(t_vec *vector, size_t type_size);
void			vec_destroy(t_vec *vector, void (*del)(void *));

void			tokclear(t_token *list, void (*del)(void *));
void			tokprint(t_token *list);
int				tokenize(t_vec *args, t_vec *argtypes, char *raw);

void			close_all(t_vec *fd_arr);
void			close_iostream(int *iostream);
void			close_ifnot(t_vec *fd_arr, int *iostream);
void			print_filearr(t_vec *fd_arr);

void			ft_cd(t_msh *prog, int argc, char **argv);
void			ft_pwd(t_msh *prog, int argc, char **argv);
void			ft_env(t_msh *prog, int argc, char **argv);
void			ft_echo(t_msh *prog, int argc, char **argv);
void			ft_unset(t_msh *prog, int argc, char **argv);
void			ft_exit(t_msh *prog, int argc, char **argv);
void			ft_export(t_msh *prog, int argc, char **argv);

void			env_update(t_msh *prog);
void			env_unset(t_var **env, char *name);
t_var			*env_val_set(const char *name, t_var *env, const char *val);
char			**env_convert(t_var *env);
void			env_init(t_msh *prog);
char			*env_val_get(const char *name, t_var *env);
void			env_clear(t_var *env, void (*del)(void *));
void			env_print(t_var *env);

/*
** New token functions - creates tokens using the same
** allocated string from input.
*/

void			tokenizer(t_msh *prog, t_vec *args, t_vec *types);
size_t			sum_tokens(char *line);
void			gen_tokens(t_ryantok **tokens, t_msh *prog);
void			mash_string(char *line, size_t dest, size_t src);

/*
** Functions to read input and handle line-editing. In read_input.c,
** handle_input.c, and input*.c.
*/

int				handle_input(t_line *line, char buf[6]);
int				read_input(t_msh *prog);
int				send_EOF(t_line *line, char buf[6]);
int				clear_screen(t_line *line, char buf[6]);
int				clear_input(t_line *line, char buf[6]);
int				cursor_move(t_line *line, int c);
int				special_command(t_line *line, char buf[6]);
int				send_input(t_line *line, char buf[6]);
int				char_input(t_line *line, char buf[6]);
int				backspace(t_line *line, char buf[6]);

/*
** Finite state machine function.
*/

int				checkstate(int c, t_ryanlexer lex);

/*
** Lexing utilities.
*/

int				check_esc_char(char *line, t_ryanlexer *lex, int gen_true);
void			init_lexer(t_ryanlexer *lex);
void			update_lexer(char *line, t_ryanlexer *lex);
void			create_token(t_ryantok *token, t_ryanlexer *lex);
void			concatenate_input(char *line);

/* Troubleshooting */
void			print_state(int c, t_ryanlexer lex);

#endif
