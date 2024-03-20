/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdomnik <mdomnik@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 12:12:05 by mdomnik           #+#    #+#             */
/*   Updated: 2024/03/20 19:39:49 by mdomnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

#include "minishell.h"

//prototyping prompt struct
typedef struct s_prompt t_prompt;

//main_utils.c
t_prompt *init_prompt(t_prompt *prompt);
char	**double_dup(char **str);
void	free_double(char **double_str);
void	print_lexer(t_prompt *prompt);

//lexer_struct.c
t_lexer	*lexernew_ms(char *word);
void	lexeraddback_ms(t_lexer **lst, t_lexer *new);
t_lexer *lexerfreelist_ms(t_lexer **lst);
void delete_node_at_index(t_lexer **lexer, int index);

//lexer_utils.c
char *trim_whitespace(char *str);
int is_whitespace_null(char c);
int is_quote(char c);
char	*append_char_env(char *str, char c);

#endif