/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   msh_term.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/28 01:36:41 by tbruinem      #+#    #+#                 */
/*   Updated: 2020/04/29 19:23:49 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_TERM_H
# define MSH_TERM_H

/*
** Enter commands to the terminal with termcaps. In terminal.c.
*/

void	termcmd(char *command, int p1, int p2, int lines_affected);

/*
**	TERMINAL COMMANDS:
*/

# define INSERT_START "im"
# define INSERT_END "ei"
# define MOVE_HORIZONTAL "ch"
# define MOVE_COLROW "cm"
# define DELETE_START "dm"
# define DELETE_END "ed"
# define SCROLL_REV "rs"
# define CLEAR_SCREEN "cl"
# define CARRIAGE_RETURN "cr"
# define DELETE_LINE "dl"
# define DELETE_LINES "DL"
# define MOVE_ROWUP "up"
# define DELETE_CHAR "dc"
# define DELETE_CHARS "DC"
# define SCROLL_LINE "sf"
# define SCROLL_LINES "SF"

#endif
