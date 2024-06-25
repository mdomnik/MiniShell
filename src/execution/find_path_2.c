/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdomnik <mdomnik@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 17:59:53 by mdomnik           #+#    #+#             */
/*   Updated: 2024/06/25 20:44:27 by mdomnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	exec_external(t_shell *shell, char *cmd, char **path)
{
	// if (ft_strchr(cmd, '/') == 0)
	// {
	// 	printf("im herea	\n");
	// 	ft_putstr_fd(cmd, 2);
	// 	ft_putstr_fd(": command not found\n", 2);
	// 	free_double(path);
	// 	free(cmd);
	// 	free_shell(shell);
	// 	exit(EXIT_FAILURE);
	// }
	// else
	// {
	// 	if (is_directory(cmd) == 1)
	// 	{
	// 		ft_putstr_fd(cmd, 2);
	// 		ft_putendl_fd(": Is a directory", 2);
	// 		free_double(path);
	// 		free(cmd);
	// 		free_shell(shell);
	// 		exit(126);
	// 	}
	// 	else if (access(cmd, F_OK) == -1)
	// 	{
	// 		ft_putstr_fd(cmd, 2);
	// 		ft_putstr_fd(": No such file or directory\n", 2);
	// 		free_double(path);
	// 		free(cmd);
	// 		free_shell(shell);
	// 		exit(127);
	// 	}
	// }
	if (execve(cmd, shell->exec->token, shell->env) == -1)
	{
		terminate_bin_path(shell, cmd, shell->exec, path);
	}
}

int	ft_exec_external(t_shell *shell, char *cmd, char **path)
{
	exec_external(shell, cmd, path);
	return (0);
}

int	find_path(t_shell *shell, t_exec *exec)
{
	char		*bin_path;
	char		**paths;
	char		*cmd;

	bin_path = exec->token[0];
	paths = get_paths(shell);
	if (!paths)
		exit(127);
	bin_path = get_bin_path(exec->token[0], paths);
	if (!bin_path)
	{
		if (access(exec->token[0], X_OK) == 0)
		{
			cmd = ft_strdup(exec->token[0]);
			return (ft_exec_external(shell, cmd, paths));
		}
		else
		{
			cmd = ft_strdup(exec->token[0]);
			terminate_bin_path(shell, cmd, exec, paths);
		}
	}
	if (execve(bin_path, exec->token, shell->env) == -1)
	{
		free(bin_path);
		exit_path(shell, paths, exec->token[0], 127);
	}
	return (0);
}

void	terminate_bin_path(t_shell *shell, char *cmd, t_exec *exec, char **paths)
{
	(void)exec;
	if (ft_strchr(cmd, '/') == 0)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		free_double(paths);
		free(cmd);
		free_shell(shell);
		exit(127);
	}
	if (is_directory(cmd) == 1)
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": Is a directory", 2);
		free_double(paths);
		free(cmd);
		free_shell(shell);
		exit(126);
	}
	else
	{
		if (access(cmd, F_OK) == -1)
		{
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			free_double(paths);
			free(cmd);
			free_shell(shell);
			exit(127);
		}
		else
		{
			ft_putstr_fd(" Permission denied\n", 2);
			free_double(paths);
			free(cmd);
			free_shell(shell);
			exit(126);
		}
	}
	free(cmd);
	exit_path(shell, paths, NULL, 127);
}

int	is_directory(char *path)
{
	struct stat	stats;

	if (stat(path, &stats) == -1)
		return (0);
	if (S_ISDIR(stats.st_mode))
		return (1);
	return (0);
}
