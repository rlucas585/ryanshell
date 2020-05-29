/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   forks.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 12:52:09 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/29 13:10:32 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** Proof of concept on how to use fork() and pipe().
*/

/* fork() creates child processes:
 * return of < 0: Failure to create child process.
 * return of 0: The return value in the child process.
 * return of > 0: The PID of the child that the parent receives.
*/

/* pipe() passes information from one prcoess to another.
 * pipe() is unidirectional - for two-way communication between processes,
 * two pipes are set up, one for each direction.
 *
 * int fd[2];
 * pipe(fd);
 * fd[0]; for the read end of the pipe.
 * fd[1]; for the write end of the pipe.
*/

/* In parent:
 * - First, close the reading end of the first pipe fd1[0].
 * - Then, write information through the writing end of the pipe fd1[1].
 * - After finishing writing, wait until the child process is finished.
 * - Next, close the write end of the second pipe fd2[1].
 * - And read info through the reading end of the pipe fd2[0].
 */

/* In child:
 * - First, read info sent by parent by closing the write end of the pipe fd1[1]
 * - After reading, process info, and send back to parent via the fd2 pipe.
 * - Exit the child process.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int		main(void)
{
	int		fd1[2];
	int		fd2[2];

	char	fixed_str[] = "Ryan is great!";
	char	input_str[100];
	pid_t	p;

	if (pipe(fd1) == -1)
	{
		fprintf(stderr, "Pipe Failed");
		return (1);
	}
	if (pipe(fd2) == -1)
	{
		fprintf(stderr, "Pipe Failed");
		return (1);
	}

	scanf("%s", input_str);
	p = fork();

	if (p < 0)
	{
		fprintf(stderr, "Fork failed");
		return (1);
	}
	else if (p > 0) // Parent process
	{
		char	concat_str[100];

		close(fd1[0]); // Close reading end of first pipe.

		// Write input string, then close write end of first pipe.
		write(fd1[1], input_str, strlen(input_str));
		close(fd1[1]);

		// Wait for child to send a string
		wait(NULL);

		close(fd2[1]); // Close writing end of second pipe.

		// Read string from child, print and close reading end of second pipe.
		read(fd2[0], concat_str, 100);
		printf("Concat string = %s\n", concat_str);
	}
	else // Child process
	{
		close(fd1[1]); // Close writing end of first pipe.

		// Read string using first pipe.
		char	concat_str[100];
		read(fd1[0], concat_str, 100);

		int k = strlen(concat_str);
		int i;
		for (i = 0; i < strlen(fixed_str); i++)
			concat_str[k++] = fixed_str[i];

		concat_str[k] = '\0';

		// Close both reading ends
		close(fd1[0]);
		close(fd2[0]);

		// Write concatenated string into the second pipe's write end.
		write(fd2[1], concat_str, strlen(concat_str));
		close(fd2[1]);

		// Exit the child process
		exit(0);
	}
	return (0);
}
