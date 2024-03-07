/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:37:34 by rasamad           #+#    #+#             */
/*   Updated: 2024/03/07 14:29:12 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_init(int ac, t_var *var, char **av)
{
	if (ac != 5)
	{
		ft_printf("Error\ninvalid argument\n");
		exit(0);
	}
	var->infile = av[1];
	var->cmd1 = av[2];
	var->cmd2 = av[3];
	var->outfile = av[4];
	var->check_access_cmd1 = 0;
	var->check_access_cmd2 = 0;
	var->path_cmd1 = NULL;
	var->path_cmd2 = NULL;
	var->open_in = 0;
	var->open_out = 0;
}

void	ft_open_fd(t_var *var)
{
	var->fd_infile = open(var->infile, O_RDONLY);
	if (var->fd_infile == -1)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		write(2, ": ", 2);
		ft_putstr_fd(var->infile, 2);
		write(2, "\n", 1);
		var->open_in = -1;
	}
	var->fd_outfile = open(var->outfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (var->fd_outfile == -1)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		write(2, ": ", 2);
		ft_putstr_fd(var->outfile, 2);
		write(2, "\n", 1);
		var->open_out = -1;
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

void	ft_free(t_var *var, int status)
{
	int	i;

	i = 0;
	while (var->split_path[i] != NULL)
	{
		free(var->split_path[i]);
		i++;
	}
	free(var->split_path);
	if (status == 1)
		free(var->cmd);
}

void	ft_free_split_flag(t_var *var)
{
	int	i;

	i = 0;
	while (var->split_flag[i] != NULL)
	{
		free(var->split_flag[i]);
		i++;
	}
	free(var->split_flag);
}
