/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:37:34 by rasamad           #+#    #+#             */
/*   Updated: 2024/03/01 17:57:44 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_init(t_var *var, char **av)
{
	var->infile = av[1];
	var->cmd1 = av[2];
	var->cmd2 = av[3];
	var->outfile = av[4];
}

void	ft_open_fd(t_var *var)
{
	var->fd_infile = open(var->infile, O_RDONLY);
	if (var->fd_infile == -1)
	{
		perror("open infile failed ");
		exit(EXIT_FAILURE);
	}
	var->fd_outfile = open(var->outfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (var->fd_outfile == -1)
	{
		perror("open outfile failed ");
		exit(EXIT_FAILURE);
	}
}

void	ft_close_fd(t_var *var)
{
	if (var->fd_infile != -1)
	{
		close(var->fd_infile);
		var->fd_infile = -1;
	}
	if (var->pipe_fd[1] != -1)
	{
		close(var->pipe_fd[1]);
		var->pipe_fd[1] = -1;
	}
	if (var->pipe_fd[0] != -1)
	{
		close(var->pipe_fd[0]);
		var->pipe_fd[0] = -1;
	}
	if (var->fd_outfile != -1)
	{
		close(var->fd_outfile);
		var->fd_outfile = -1;
	}
}

void	ft_free(char **split_path, char *cmd, int status)
{
	int	i;

	i = 0;
	while (split_path[i] != NULL)
	{
		free(split_path[i]);
		i++;
	}
	free(split_path);
	free(cmd);
	if (status == 1)
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
	}
}
