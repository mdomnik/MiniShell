/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdomnik <mdomnik@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 01:08:04 by mdomnik           #+#    #+#             */
/*   Updated: 2024/06/24 20:00:22 by mdomnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * Executes the built-in export command in the minishell.
 * If no arguments are provided, it prints all the environment variables.
 * Otherwise, it updates the environment variables with the provided arguments.
 *
 * @param shell The minishell structure.
 */
int	builtin_export(t_shell *shell, t_exec *exec)
{
	int	i;

	i = 0;
	if (exec->token_count == 1)
	{
		while (shell->declare[i])
		{
			printf("declare -x ");
			printf("%s\n", shell->declare[i]);
			i++;
		}
		return (0);
	}
	else
		return (update_env_declare(shell, exec));
}

/**
 * Updates the environment and declares
 * new variables based on the arguments provided.
 * 
 * @param shell The shell structure containing
 * the parser and other relevant data.
 */
int	update_env_declare(t_shell *shell, t_exec *exec)
{
	int	i;

	i = 1;
	while (exec->token[i])
	{
		if (valid_format(exec->token[i]) == -1)
			return (ft_putendl_fd("export: not a valid identifier", 2), -1);
		else if (valid_format(exec->token[i]) == 1)
		{
			if (scan_declare(shell, exec->token[i]) == 0)
				add_declare(shell, exec->token[i], 0);
		}
		else
		{
			if (scan_env(shell, exec->token[i]) == 0)
				add_env(shell, exec->token[i]);
			if (scan_declare(shell, exec->token[i]) == 0)
				add_declare(shell, exec->token[i], 1);
		}
		i++;
	}
	sort_declare(shell);
	return (0);
}

/**
 * Checks if the given string has a valid format for an exported variable.
 * A valid format consists of a letter or underscore as the first character,
 * followed by zero or more alphanumeric characters or underscores, and
 * optionally followed by an equal sign (=).
 *
 * @param str The string to be checked.
 * @return -1 if the format is invalid, 0 if the format is valid but does not
 *         contain an equal sign, and 1 if the format is valid and contains an
 *         equal sign.
 */
int	valid_format(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '_' && !ft_isalpha(str[i]))
		return (-1);
	i++;
	while (str[i] != '=' || str[i] != '\0')
	{
		if (str[i] == '=' || str[i] == '\0')
			break ;
		if (str[i] != '_' && !ft_isalnum(str[i]) && str[i])
			return (-1);
		i++;
	}
	if (str[i] == '\0')
		return (1);
	return (0);
}

/**
 * Adds a new declaration to the shell's declare array.
 * 
 * @param shell The shell structure.
 * @param str The string to be added as a declaration.
 * @param var The variable indicating whether the string\
 * is a variable (1) or not (0).
 */
void	add_declare(t_shell *shell, char *str, int var)
{
	char	**copy;
	char	*u_str;
	int		i;

	i = 0;
	u_str = add_value_quotes(str, 0);
	copy = malloc(sizeof(char *) * (count_args(shell->declare) + 2));
	if (!copy)
		return ;
	while (shell->declare[i])
	{
		copy[i] = ft_strdup(shell->declare[i]);
		i++;
	}
	copy[i] = ft_strdup(u_str);
	if (u_str && var == 1)
		free(u_str);
	copy[i + 1] = NULL;
	free_double(shell->declare);
	shell->declare = double_dup(copy);
	free_double(copy);
}

/**
 * Adds a new environment variable to the shell's environment.
 *
 * @param shell The shell structure.
 * @param str The string representing the new environment variable.
 */
void	add_env(t_shell *shell, char *str)
{
	char	**copy;
	int		i;

	i = 0;
	copy = malloc(sizeof(char *) * (count_args(shell->env) + 2));
	if (!copy)
		return ;
	while (shell->env[i])
	{
		copy[i] = ft_strdup(shell->env[i]);
		i++;
	}
	copy[i] = ft_strdup(str);
	copy[i + 1] = NULL;
	free_double(shell->env);
	shell->env = copy;
}
