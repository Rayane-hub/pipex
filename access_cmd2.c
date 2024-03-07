/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access_cmd2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 15:02:24 by rasamad           #+#    #+#             */
/*   Updated: 2024/03/07 15:40:58 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	display_error_cmd2(t_var *var)
{
	ft_putstr_fd("pipex: commande not found: ", 2);
	ft_putstr_fd(var->cmd2, 2);
	write(2, "\n", 1);
	free(var->cmd);
}

static void	display_such_cmd2(t_var *var)
{
	ft_putstr_fd("pipex: no such file or directory: ", 2);
	ft_putstr_fd(var->cmd2, 2);
	write(2, "\n", 1);
}

static char	*ft_change_cmd2(t_var *var)
{
	char	*cmd;
	int		i;
	int		j;

	i = 1;
	while (var->cmd2[i] != ' ' && var->cmd2[i])
		i++;
	cmd = malloc((i + 2) * sizeof(char *));
	j = 1;
	cmd[j - 1] = '/';
	while (j - 1 < i)
	{
		cmd[j] = var->cmd2[j - 1];
		j++;
	}
	cmd[j] = 0;
	return (cmd);
}

static int	ft_check_access_cmd2(t_var *var, int status)
{
	if (status == 1)
	{
		if (ft_strchr(var->cmd2, '/') != NULL)
		{
			if (var->cmd2[0] != ' ')
			{
				var->split_flag = ft_split(var->cmd2, ' ');
				if (access(var->split_flag[0], F_OK) == 0)
				{
					var->path_cmd2 = ft_strdup(var->split_flag[0]);
					return (ft_free_split_flag(var), 0);
				}
				else
					return (ft_free_split_flag(var), -2);
			}
			else
				return (-2);
		}
	}
	else
	{
		if (access(var->path_cmd2, F_OK) == 0)
			return (ft_free(var, 1), 0);
	}
	return (-1);
}

int	ft_access_cmd2(char **envp, t_var *var)
{
	if (ft_check_access_cmd2(var, 1) == 0)
		return (0);
	else if (ft_check_access_cmd2(var, 1) == -2 || envp[0] == NULL)
		return (display_such_cmd2(var), -1);
	var->i = 0;
	var->cmd = ft_change_cmd2(var);
	if (var->cmd[1] == ' ')
		return (display_error_cmd2(var), -1);
	while (envp[var->i] && (ft_strncmp("PATH=", envp[var->i], 5) != 0))
		var->i++;
	if (envp[var->i] == NULL)
		return (display_error_cmd2(var), -1);
	var->split_path = ft_split(envp[var->i], ':');
	var->i = 0;
	while (var->split_path[var->i])
	{
		var->path_cmd2 = ft_strjoin(var->split_path[var->i], var->cmd);
		if (ft_check_access_cmd2(var, 0) == 0)
			return (0);
		free(var->path_cmd2);
		var->i++;
	}
	display_error_cmd2(var);
	ft_free(var, 0);
	return (-1);
}
