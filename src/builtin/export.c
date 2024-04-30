/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdomnik <mdomnik@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 01:08:04 by mdomnik           #+#    #+#             */
/*   Updated: 2024/04/30 18:00:34 by mdomnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_export(t_shell *shell)
{
	int	i;

	i = 0;
	if (count_args(shell->parser->args) == 0)
	{
		while(shell->declare[i])
		{
			printf("declare -x ");
			printf("%s\n", shell->declare[i]);
			i++;
		}
		reset_loop(shell, NULL);
		return ;
	}
	else
		update_env_declare(shell);
	reset_loop(shell, NULL);
}

void	update_env_declare(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->parser->args[i])
	{
		if (valid_format(shell->parser->args[i]) == -1)
			printf("minishell: export: %s: not a valid identifier\n", shell->parser->args[i]);
		else if (valid_format(shell->parser->args[i]) == 1)
		{
			if (scan_declare(shell, shell->parser->args[i]) == 0)
				add_declare(shell, shell->parser->args[i], 0);
		}
		else
		{
			if (scan_env(shell, shell->parser->args[i]) == 0)
				add_env(shell, shell->parser->args[i]);
			if (scan_declare(shell, shell->parser->args[i]) == 0)
				add_declare(shell, shell->parser->args[i], 1);
		}
		i++;
	}
	sort_declare(shell);
}

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
			break;
		if (str[i] != '_' && !ft_isalnum(str[i]))
			return (-1);
		i++;
	}
	if (str[i] == '\0')
		return (1);
	return (0);
}

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