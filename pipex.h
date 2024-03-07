/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:52:38 by rasamad           #+#    #+#             */
/*   Updated: 2024/03/07 15:58:34 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
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
	char	**split_flag;
	char	*path_cmd1;
	char	*path_cmd2;
	int		check_access_cmd1;
	int		check_access_cmd2;
	int		open_in;
	int		open_out;
}	t_var;

void	ft_init(int ac, t_var *var, char **av);
void	ft_open_fd(t_var *var);
int		ft_access_cmd1(char **envp, t_var *var);
int		ft_access_cmd2(char **envp, t_var *var);
void	ft_free(t_var *var, int status);
int		ft_fork_cmd1(t_var *var, char **envp);
int		ft_fork_cmd2(t_var *var, char **envp);
void	ft_free_split_flag(t_var *var);
void	ft_close_fd(t_var *var);

#endif