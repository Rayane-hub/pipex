/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:08:24 by rasamad           #+#    #+#             */
/*   Updated: 2024/03/01 18:01:31 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int ac, char **av, char **envp)
{
	t_var	var;

	if (ac != 5)
	{
		ft_printf("Error\ninvalid argument\n");
		return (-1);
	}
	ft_init(&var, av);
	ft_open_fd(&var);
	if (pipe(var.pipe_fd) == -1)
	{
		perror("pipe failed ");
		ft_close_fd(&var);
		exit(EXIT_FAILURE);
	}
	ft_fork_cmd1(&var, envp);
	ft_fork_cmd2(&var, envp);
	ft_close_fd(&var);
	return (0);
}
