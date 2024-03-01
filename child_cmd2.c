/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_cmd2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:08:53 by rasamad           #+#    #+#             */
/*   Updated: 2024/03/01 18:22:18 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Fonction : Cree une str qui a le bon format pour la verification d'access
//exemple "cat -e" devient "/cat".
static char	*ft_change_cmd2(t_var *var)
{
	char	*cmd;
	int		i;
	int		j;

	i = 0;
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

// Fonction : Trouve le bon path necessaire pour 
//le 1er argument de la fonction execve(char *path)
static void	ft_find_path_cmd2(char **envp, t_var *var)
{
	var->i = 0;
	var->cmd = ft_change_cmd2(var);
	while (envp[var->i][0] != 'P' || envp[var->i][1] != 'A')
		var->i++;
	var->split_path = ft_split(envp[var->i], ':');
	var->i = 0;
	var->path_cmd = ft_strjoin(var->split_path[var->i], var->cmd);
	while (access(var->path_cmd, F_OK) != 0)
	{
		free(var->path_cmd);
		if (var->i > 8)
		{
			ft_free(var->split_path, var->cmd, 1);
			perror("cmd2 failed ");
			ft_close_fd(var);
			exit(EXIT_FAILURE);
		}
		var->i++;
		var->path_cmd = ft_strjoin(var->split_path[var->i], var->cmd);
	}
	ft_free(var->split_path, var->cmd, 0);
}

//Fonction : Chemin de l'enfant, redirige l'extremiter de lecture du pipe 
//vers stdin et le stdout vers le contenue de outfile une fois cmd2 executer
static void	ft_child_way_cmd2(t_var *var, char **envp)
{
	if (dup2(var->pipe_fd[0], STDIN_FILENO) == -1)
	{
		ft_close_fd(var);
		perror("dup2 pipe 0 failed ");
		exit(EXIT_FAILURE);
	}
	close(var->pipe_fd[1]);
	var->pipe_fd[1] = -1;
	if (dup2(var->fd_outfile, STDOUT_FILENO) == -1)
	{
		perror("dup2 outfile failed ");
		ft_close_fd(var);
		exit(EXIT_FAILURE);
	}
	close(var->fd_outfile);
	var->fd_outfile = -1;
	ft_find_path_cmd2(envp, var);
	var->split_flag = ft_split(var->cmd2, ' ');
	execve(var->path_cmd, var->split_flag, envp);
	free (var->path_cmd);
	free(var->split_flag);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	ft_close_fd(var);
	perror("execve 2 failed ");
	exit(EXIT_FAILURE);
}

//Fonction : Creation du processus enfant pour la cmd1
int	ft_fork_cmd2(t_var *var, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork 2 failed ");
		ft_close_fd(var);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		ft_child_way_cmd2(var, envp);
	else if (pid > 0)
	{
		close(var->pipe_fd[1]);
		var->pipe_fd[1] = -1;
		waitpid(pid, &status, 0);
		if (status != 0)
		{
			ft_close_fd(var);
			exit(EXIT_FAILURE);
		}
	}
	return (0);
}
