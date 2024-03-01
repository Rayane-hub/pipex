/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:52:38 by rasamad           #+#    #+#             */
/*   Updated: 2024/03/01 17:58:23 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include "ft_printf/ft_printf.h"
# include "libft/libft.h"

typedef struct s_var
{
	char	*infile;
	char	*cmd1;
	char	*cmd2;
	char	*outfile;	
	int		fd_infile;
	int		fd_outfile;
	int		pipe_fd[2];
	int		i;
	char	*cmd;
	char	**split_path;
	char	*path_cmd;
	char	**split_flag;
}	t_var;

void	ft_init(t_var *var, char **av);
void	ft_open_fd(t_var *var);
int		ft_fork_cmd1(t_var *var, char **envp);
int		ft_fork_cmd2(t_var *var, char **envp);
void	ft_close_fd(t_var *var);
void	ft_free(char **split_path, char *cmd, int status);

#endif