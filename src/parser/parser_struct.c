/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_struct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdomnik <mdomnik@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:37:38 by mdomnik           #+#    #+#             */
/*   Updated: 2024/04/18 14:41:18 by mdomnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_parser *parsernew_ms(char **args, char **io, char **files)
{
	t_parser		*element;

	element = (t_parser *)malloc(sizeof(t_parser));
	if (!element)
		return (NULL);
	element->index = (reset_increment_k(1) - 1);
	
	if (args[0] != NULL)
		element->cmd = ft_strdup(args[0]);
	else
		element->cmd = NULL;
	element->args = remove_first(args);

	if (io[0] == NULL)
	{
		if (element->index == 0)
		{
			element->input = I_STDIN;
			element->i_str = "STDIN";
		}
		else
		{
			element->input = I_PIPE;
			element->i_str = "PIPE";
		}
		
	}
	else
	{
		element->input = find_redir(io[0]);
		element->i_str = ft_strdup(io[1]);
	}
	if (io[2] == NULL)
	{
		element->output = 0;
		element->o_str = NULL;
	}
	else
	{
		element->output = find_redir(io[2]);
		element->o_str = ft_strdup(io[3]);
	}
	
	element->files = double_dup(files);


	element->prev = NULL;
	element->next = NULL;

	free_double(args);
	free_double(io);
	free_double(files);
	
	return (element);
}

int	reset_increment_k(int x)
{
	static int k = 0;
	if (x == 0)
		k = 0;
	else
		k++;
	return (k);
}

//adds new node to the back of the parser linked list
void	parseraddback_ms(t_parser **lst, t_parser *new)
{
	t_parser		*temp;

	temp = *lst;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
	new->prev = temp;	
}

t_parser *parserfreelist_ms(t_parser **lst)
{
	t_parser *temp;

	temp = *lst;
	if (!(*lst))
		return (NULL);
	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->cmd)
			free ((*lst)->cmd);
		if ((*lst)->args != NULL)
			free_double((*lst)->args);
		if ((*lst)->files != NULL)
			free_double((*lst)->files);
		free(*lst);
		*lst = temp;
	}
	*lst = NULL;
	reset_increment_k(0);
	return (NULL);
}