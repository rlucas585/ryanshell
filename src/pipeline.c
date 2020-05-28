/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipeline.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/04 19:35:57 by tbruinem      #+#    #+#                 */
/*   Updated: 2020/05/24 14:25:38 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

extern int errno;

void	close_all(t_vec *fd_arr)
{
	int		*fds;
	size_t	i;

	i = 0;
	fds = (int *)fd_arr->store;
	while (i < fd_arr->index)
	{
		close(fds[i]);
		i++;
	}
}

void	close_ifnot(t_vec *fd_arr, int *iostream)
{
	size_t	i;
	int		*fds;

	fds = (int *)fd_arr->store;
	while (i < fd_arr->index)
	{
		if (fds[i] != -1 &&
			(fds[i] != iostream[READ] && fds[i] != iostream[WRITE]))
			close(fds[i]);
		i++;
	}
}

void	close_iostream(int *iostream)
{
	if (iostream[READ] != -1)
		close(iostream[READ]);
	if (iostream[WRITE] != -1)
		close(iostream[WRITE]);
}

int		is_redir(int type)
{
	return (type == APPENDFILE ||
		type == WRITEFILE ||
		type == INPUT_SENDER ||
		type == PIPEDCOMMAND);
}

void	print_filearr(t_vec *fd_arr)
{
	int		*fds;
	size_t	i;

	i = 0;
	fds = (int *)fd_arr->store;
	while (i < fd_arr->index)
	{
		dprintf(2, "[%d]%c", fds[i], (i + 1 < fd_arr->index) ? ' ' : '\n');
		i++;
	}
}

int		new_file(t_cmd *command, char **args, int type, t_vec *fd_arr)
{
	int	fd;

	if (type == TRUNC)
		fd = open(args[1], O_CREAT | O_WRONLY | O_TRUNC);
	else if (type == APPEND)
		fd = open(args[1], O_CREAT | O_WRONLY | O_APPEND);
	else if (type == IN_REDIR)
		fd = open(args[1], O_RDONLY);
	if (fd == -1 || !vec_add(fd_arr, &fd))
	{
//		dprintf(2, "ERRNO: %d | file(%d) failed to open: %s\n", errno, fd, args[1]);
		return (0);
	}
//	dprintf(2, "File's FD: %d\n", fd);
	if (type == IN_REDIR)
		command->iostream[READ] = fd;
	else
		command->iostream[WRITE] = fd;
//	free(args[0]);
//	free(args[1]);
//	args[0] = NULL;
//	args[1] = NULL;
	return (1);
}

int		new_stream(t_cmd *cmd, char **args, int type, t_vec *fd_arr)
{
	if (!args[1])
		return (0); //error
	return (new_file(cmd, args, type, fd_arr));
}

int		set_redirection(t_cmd *command, char **args, int *types, t_vec *fd_arr)
{
	size_t	i;

	i = 0;
	while (args[i])
	{
		if (is_redir(types[i]))
			if (!new_stream(command, &args[i], types[i], fd_arr))
				return (0);
		i++;
	}
	return (1);
}
