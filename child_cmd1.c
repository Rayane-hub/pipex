/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_cmd1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:08:17 by rasamad           #+#    #+#             */
/*   Updated: 2024/03/01 18:30:58 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Fonction : Cree une str qui a le bon format pour la verification d'access
//exemple "cat -e" devient "/cat".
static char	*ft_change_cmd1(t_var *var)
{
	char	*cmd;
	int		i;
	int		j;

	i = 0;
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

// Fonction : Trouve le bon path necessaire pour 
//le 1er argument de la fonction execve(char *path)
static void	ft_find_path_cmd1(char **envp, t_var *var)
{
	var->i = 0;
	var->cmd = ft_change_cmd1(var);
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
			perror("pipex: command not found");
			ft_close_fd(var);
			exit(EXIT_FAILURE);
		}
		var->i++;
		var->path_cmd = ft_strjoin(var->split_path[var->i], var->cmd);
	}
	ft_free(var->split_path, var->cmd, 0);
}

//Fonction : Recupere le contenu de infile puis execute la cmd1 
//en redirigeant sa sortie vers l'extremiter d'ecriture du pipe.
static void	ft_child_cmd1(t_var *var, char **envp)
{
	if (dup2(var->fd_infile, STDIN_FILENO) == -1)
	{
		perror("dup2 infile failed ");
		ft_close_fd(var);
		exit (EXIT_FAILURE);
	}
	close(var->fd_infile);
	var->fd_infile = -1;
	if (dup2(var->pipe_fd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2 pipe 1 failed ");
		ft_close_fd(var);
		exit (EXIT_FAILURE);
	}
	close(var->pipe_fd[1]);
	var->pipe_fd[1] = -1;
	ft_find_path_cmd1(envp, var);
	var->split_flag = ft_split(var->cmd1, ' ');
	execve(var->path_cmd, var->split_flag, envp);
	free(var->path_cmd);
	free(var->split_flag);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	perror("execve 1 failed ");
	ft_close_fd(var);
	exit(EXIT_FAILURE);
}

// Fonction : Cree un processus enfant pour la cmd1 
int	ft_fork_cmd1(t_var *var, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork 1 failed ");
		ft_close_fd(var);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		ft_child_cmd1(var, envp);
	else
	{
		waitpid(pid, &status, 0);
		if (status != 0)
		{
			ft_close_fd(var);
			exit(EXIT_FAILURE);
		}
	}
	return (0);
}
