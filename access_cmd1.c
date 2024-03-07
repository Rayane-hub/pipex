/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access_cmd1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 15:03:27 by rasamad           #+#    #+#             */
/*   Updated: 2024/03/07 15:52:58 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	display_error_cmd1(t_var *var)
{
	ft_putstr_fd("pipex: commande not found: ", 2);
	ft_putstr_fd(var->cmd1, 2);
	write(2, "\n", 1);
	free(var->cmd);
}

static void	display_such_cmd1(t_var *var)
{
	ft_putstr_fd("pipex: no such file or directory: ", 2);
	ft_putstr_fd(var->cmd1, 2);
	write(2, "\n", 1);
}

// Fonction : creat the correct order format for access()
//example : "cat -e" devient "/cat".
static char	*ft_change_cmd1(t_var *var)
{
	char	*cmd;
	int		i;
	int		j;

	i = 1;
	while (var->cmd1[i] != ' ' && var->cmd1[i])
		i++;
	cmd = malloc((i + 2) * sizeof(char *));
	j = 1;
	cmd[j - 1] = '/';
	while (j - 1 < i)
	{
		cmd[j] = var->cmd1[j - 1];
		j++;
	}
	cmd[j] = 0;
	return (cmd);
}

//Fonction: status = 1 check with path if (cmd1 == "/bin/cat") -> return SUCCESS
//else (cmd1 == " /bin/frg") ---> return FAILURE
//status == 0 check without path ---> (cmd1 = cat)
//Return : 0 SUCCESS or -1 FAILLURE (continue) or -2 FAILURE (stop with message)
static int	ft_check_access_cmd1(t_var *var, int status)
{
	if (status == 1)
	{
		if (ft_strchr(var->cmd1, '/') != NULL)
		{
			if (var->cmd1[0] != ' ')
			{
				var->split_flag = ft_split(var->cmd1, ' ');
				if (access(var->split_flag[0], F_OK) == 0)
				{
					var->path_cmd1 = ft_strdup(var->split_flag[0]);
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
		if (access(var->path_cmd1, F_OK) == 0)
			return (ft_free(var, 1), 0);
	}
	return (-1);
}

// Fonction : Find the good path necessary for 
//the 1st argument of the function execve(char *path)
//Return : 0 SUCCESS or -1 FAILLURE
int	ft_access_cmd1(char **envp, t_var *var)
{
	if (ft_check_access_cmd1(var, 1) == 0)
		return (0);
	else if (ft_check_access_cmd1(var, 1) == -2 || envp[0] == NULL)
		return (display_such_cmd1(var), -1);
	var->i = 0;
	var->cmd = ft_change_cmd1(var);
	if (var->cmd[1] == ' ')
		return (display_error_cmd1(var), -1);
	while (envp[var->i] && (ft_strncmp("PATH=", envp[var->i], 5) != 0))
		var->i++;
	if (envp[var->i] == NULL)
		return (display_error_cmd1(var), -1);
	var->split_path = ft_split(envp[var->i], ':');
	var->i = 0;
	while (var->split_path[var->i])
	{
		var->path_cmd1 = ft_strjoin(var->split_path[var->i], var->cmd);
		if (ft_check_access_cmd1(var, 0) == 0)
			return (0);
		free(var->path_cmd1);
		var->i++;
	}
	display_error_cmd1(var);
	ft_free(var, 0);
	return (-1);
}
