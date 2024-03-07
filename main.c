/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:08:24 by rasamad           #+#    #+#             */
/*   Updated: 2024/03/07 14:18:27 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	ft_fork_cmd(t_var *var, char **envp)
{
	if (var->check_access_cmd1 == 0 && var->open_in == 0)
	{
		ft_fork_cmd1(var, envp);
		free(var->path_cmd1);
	}
	close(var->pipe_fd[1]);
	if (var->check_access_cmd2 == 0 && var->open_out == 0)
	{
		ft_fork_cmd2(var, envp);
		free(var->path_cmd2);
	}
	close(var->pipe_fd[0]);
}

int	main(int ac, char **av, char **envp)
{
	t_var	var;

	ft_init(ac, &var, av);
	ft_open_fd(&var);
	if (pipe(var.pipe_fd) == -1)
	{
		perror("pipex");
		ft_close_fd(&var);
		exit(EXIT_FAILURE);
	}
	if (var.open_in == 0)
		var.check_access_cmd1 = ft_access_cmd1(envp, &var);
	if (var.open_out == 0)
		var.check_access_cmd2 = ft_access_cmd2(envp, &var);
	ft_fork_cmd(&var, envp);
	while (wait(NULL) > 0)
		;
	ft_close_fd(&var);
	if (var.open_out == -1)
		return (1);
	else if (var.check_access_cmd2 == -1)
		return (127);
	return (0);
}
