/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/16 10:51:49 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/04 17:26:17 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define READ_END 0
# define WRITE_END 1

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
# include <libft_types.h>

enum			e_toktype
{
	COMMAND = 0,
	STANDARD,
	WRITEFILE,
	APPENDFILE,
	PIPEDCOMMAND,
	INPUT_SENDER,
	READ_COMMAND
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
	char		*output;
	char		*input;
	char		*command;
	int			append;
	int			cmd_num;
	int			fork;
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
	t_vecstr		cmd;
	size_t			alloced_cmd;
	size_t			inputrow;
	t_coord			max;
	t_coord			cursor;
	size_t			total_rows;
	int				escmode;
	char			*termtype;
	char			cap_table[2048];
	struct termios	term;
}				t_line;

typedef	struct	s_msh
{
	t_ryantok	*tokens;
	t_vector	env;
	char		**envp;
	t_line		line;
	size_t		argc;
}				t_msh;

enum			e_error
{
	MEM_FAIL,
	TERM_FAIL,
	CAP_FAIL,
	CMD_NOT_FOUND
};

enum			e_stages
{
	PRE_ENV,
	IN_ENV,
	IN_TERM,
	IN_INPUT
};

enum			e_builtins
{
	B_ECHO,
	B_ENV,
	B_CD,
	B_PWD,
	B_EXPORT,
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

/*
** Globals
*/

t_vecarr		g_pid;

typedef int		(*t_inputf)(t_line *line, char buf[6]);
typedef void	(*t_rbin)(t_msh *prog, t_ryancmd cmd, int cmd_num);

/*
** Utility functions in utils.c
*/

char			*ft_str3join(const char *s1, const char *s2, const char *s3);
void			print_tokens(t_ryantok *tokens);

/*
** Add a prompt to the shell, in prompt.c 
*/

size_t			ft_no_ansi_strlen(const char *str);
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

void			error_exit(t_msh *prog, int err, int stage);
void			std_exit(t_msh *prog);

void			env_init(t_msh *prog);
char			*env_val_get(const char *name, t_msh *prog, size_t len);
void			print_env(t_msh *prog);
void			env_unset(t_msh *prog, const char *unsetvar);
void			env_export(t_msh *prog, char *newvar);
char			**make_envp(t_msh *prog);
void			clear_env(t_msh *prog);

/*
** New token functions - creates tokens using the same
** allocated string from input.
*/

void			tokenizer(t_msh *prog, t_vecstr *line);
size_t			sum_tokens(t_vecstr *line);
void			gen_tokens(t_ryantok **tokens, t_vecstr *line, t_msh *prog);

/*
** New execution based on new tokens.
*/

int				r_execute(t_msh *prog);
void			print_cmd(t_ryancmd cmd);
t_ryancmd		format_cmd(t_msh *prog, int cmd_num);
void			check_pipe(t_msh *prog, t_ryancmd *cmd, int cmd_num);
void			update_cmd(char *value, int type, t_ryancmd *cmd);
int				ft_is_builtin(const char *command);
t_rbin			builtin_funcs(int code);
void			ft_echo(t_msh *prog, t_ryancmd cmd, int cmd_num);
int				r_runcmd(t_msh *prog, int cmd_num);
void			run_fork(t_msh *prog, t_ryancmd cmd);
void			single_cmd(t_msh *prog, t_ryancmd cmd);
void			run_command(t_msh *prog, t_ryancmd cmd, int code);

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

int				check_esc_char(t_vecstr *line, t_ryanlexer *lex, int gen_true);
void			init_lexer(t_ryanlexer *lex);
void			update_lexer(char *line, t_ryanlexer *lex);
void			create_token(t_ryantok *token, t_ryanlexer *lex);
void			concatenate_input(char *line);


/*
** Signal functions.
*/

void			sighandler(int signal);

/* Troubleshooting */
void			print_state(int c, t_ryanlexer lex);

#endif
