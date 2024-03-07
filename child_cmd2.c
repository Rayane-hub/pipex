/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_cmd2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:08:53 by rasamad           #+#    #+#             */
/*   Updated: 2024/03/07 15:57:47 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//Fonction : redirect the reading end of the pipe to stdin
//and stdout to the contents of the outfile then run cmd2
static void	ft_child_way_cmd2(t_var *var, char **envp)
{
	if (dup2(var->pipe_fd[0], STDIN_FILENO) == -1 && var->open_in == 0)
	{
		ft_close_fd(var);
		perror("dup2 pipe 0 failed ");
		exit(EXIT_FAILURE);
	}
	if (dup2(var->fd_outfile, STDOUT_FILENO) == -1)
	{
		ft_close_fd(var);
		perror("dup2 outfile failed ");
		exit(EXIT_FAILURE);
	}
	ft_close_fd(var);
	var->split_flag = ft_split(var->cmd2, ' ');
	execve(var->path_cmd2, var->split_flag, envp);
	perror("pipex execve fail for cmd2");
	ft_free_split_flag(var);
	free(var->path_cmd2);
	exit(EXIT_FAILURE);
}

int	ft_fork_cmd2(t_var *var, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork 2 failed ");
		free(var->path_cmd2);
		ft_close_fd(var);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		ft_child_way_cmd2(var, envp);
	return (0);
}
