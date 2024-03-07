/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_cmd1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:08:17 by rasamad           #+#    #+#             */
/*   Updated: 2024/03/07 15:55:08 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//Fonction : Retrieve the contents of infile then execute 
//cmd1 by redirecting its output to the writing end of the pipe.
static void	ft_child_cmd1(t_var *var, char **envp)
{
	if (dup2(var->fd_infile, STDIN_FILENO) == -1)
	{
		ft_close_fd(var);
		perror("dup2 infile failed ");
		exit (EXIT_FAILURE);
	}
	if (dup2(var->pipe_fd[1], STDOUT_FILENO) == -1)
	{
		ft_close_fd(var);
		perror("dup2 pipe 1 failed ");
		exit (EXIT_FAILURE);
	}
	ft_close_fd(var);
	var->split_flag = ft_split(var->cmd1, ' ');
	execve(var->path_cmd1, var->split_flag, envp);
	perror("pipex execve fail for cmd1");
	ft_free_split_flag(var);
	free(var->path_cmd1);
	free(var->path_cmd2);
	exit(EXIT_FAILURE);
}

int	ft_fork_cmd1(t_var *var, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork 1 failed ");
		free(var->path_cmd1);
		free(var->path_cmd1);
		ft_close_fd(var);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		ft_child_cmd1(var, envp);
	return (0);
}
