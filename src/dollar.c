/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:28:11 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:32:25 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_valid_dollar(int *i, char *word, char **var);
static int	ft_expand_var(char *var, t_envp *env, char **word, int *i);
static int	ft_change_word(char *var, char **word, int *i, t_data *data);
static int	ft_exit_extension(char *var, char **word, int *i, t_data *data);

/*
The `ft_dollar` function processes the occurrence of an environment variable in a word.  
It checks the validity of the variable by calling `ft_valid_dollar` and retrieves its value.  
If there is no variable to process, it returns `-1` and frees the memory.  
When the variable exists, the index `i` is updated accordingly.  
The function then tries to expand the environment variable (using `ft_expand_var`) and,  
if successful, it calls `ft_change_word` to update the word.  
If any of these processes fail (`value == -1`), the function returns `-1`.  
Otherwise, when everything runs correctly, it returns `0`.
*/
int	ft_dollar(int *i, char **word, t_data *data)
{
	char	*var;
	int		value;

	value = ft_valid_dollar(i, *word, &var);
	if (value == -1)
	{
		free(word);
		return (-1);
	}
	else if (value >= 1)
		*i = *i + value;
	else
	{
		value = ft_expand_var(var, data->envp, word, i);
		if (value == 0)
		{
			value = ft_change_word(var, word, i, data);
			if (value == -1)
				return (-1);
		}
		else if (value == -1)
			return (-1);
	}
	return (0);
}

/*
The `ft_valid_dollar` function checks the syntax validity of an environment variable following the `$` sign in the given string `word`.  
It handles various cases, such as `$$`, `$?`, invalid characters, or alphanumeric variables.  
The value `n` specifies the length of the sequence.  
When the variable is valid, it allocates memory for `var`, copying the variable's name.  
If there is an allocation error, it returns `-1`,  
and in other cases, it returns the length of the processed characters or an error code.
*/
static int	ft_valid_dollar(int *i, char *word, char **var)
{
	int	n;

	n = 1;
	if (word[*i + n] && word[*i + n] == '$')
	{
		while (word[*i + n] && word[*i + n++] == '$')
			return (n);
	}
	if (word[*i + n] == '?')
		n++;
	else if (!(word[*i + n]) || (!ft_isalpha(word[*i + n])
			&& word[*i + n] != '_' ))
		return (1);
	else
	{
		while (word[*i + n] && (ft_isalnum(word[*i + n])
				|| word[*i + n] == '_'))
			n++;
	}
	*var = malloc(sizeof(char) * n);
	if (!(*var))
		return (ft_perror_message());
	ft_strlcpy(*var, &word[1 + (*i)], n);
	return (0);
}

/*
The function ft_expand_var replaces the environment variable in the given string word (starting with $) 
with its corresponding value from the environment list env.
It first locates the variable in the list, then allocates a new string, replacing the variable name with its value.
If the variable doesn’t exist or has no value, it returns 0.
In case of allocation errors, it returns -1.
The function updates the pointer to the processed string and adjusts the index i accordingly.
*/
static int	ft_expand_var(char *var, t_envp *env, char **word, int *i)
{
	t_envp	*env_var;
	char	*str;
	int		n;
	int		value_len;

	n = ft_strlen(var);
	env_var = fetch_envp_node(env, var);
	if (!env_var || !env_var->value[n + 1])
		return (0);
	free(var);
	value_len = ft_strlen(&env_var->value[n + 1]);
	str = malloc(sizeof(char) * (ft_strlen(*word) - n + value_len));
	if (!str)
	{
		free(word);
		return (ft_perror_message());
	}
	ft_strlcpy(str, *word, *i + 1);
	ft_strlcpy(&str[*i], &env_var->value[n + 1], value_len + 1);
	ft_strlcpy(&str[*i + value_len], &(*word)[*i + n + 1],
		ft_strlen(*word) - *i - n + 1);
	free(*word);
	*word = str;
	*i = *i + value_len -1;
	return (1);
}

/*
The function ft_change_word removes an environment variable starting with $ from the string word, replacing it with an empty string, unless the variable is $?, which is handled separately by ft_exit_extension.
It calculates the lengths of the variable and the original string, allocates a new string without the variable, copies the appropriate parts of the text, updates the pointer word, and then frees the old memory.
In case of allocation errors, it returns -1.
*/
static int	ft_change_word(char *var, char **word, int *i, t_data *data)
{
	char	*new_word;
	int		word_len;
	int		v_len;

	if (var[0] == '?')
	{
		if (ft_exit_extension(var, word, i, data) == -1)
			return (-1);
		return (0);
	}
	v_len = ft_strlen(var);
	word_len = ft_strlen(*word);
	new_word = malloc(sizeof(char) * (word_len - v_len + 1));
	if (!new_word)
	{
		free(var);
		free(*word);
		return (ft_perror_message());
	}
	ft_strlcpy(new_word, *word, *i + 1);
	ft_strlcpy(&new_word[*i], &(*word)[*i + v_len + 1], word_len - *i - v_len);
	free(var);
	free(*word);
	*word = new_word;
	return (0);
}

/*
The function `ft_exit_extension` replaces the `$?` 
variable in the string `word` with the exit status code of the last command, 
stored in `data->cmd_exit_status`. It first frees the memory for the `var` variable, 
then converts the exit code to a string using `ft_itoa`. It allocates a new string where `$?` 
is replaced with the exit code, updates the pointer `word`, and moves the index `i`. If a memory allocation error occurs, 
the function frees the resources and returns `-1`.
*/
static int	ft_exit_extension(char *var, char **word, int *i, t_data *data)
{
	char	*new_word;
	char	*exit_code;
	int		exit_len;
	int		word_len;

	free(var);
	exit_code = ft_itoa(data->cmd_exit_status);
	if (!exit_code)
		return (ft_perror_free(NULL, *word, NULL));
	exit_len = ft_strlen(exit_code);
	word_len = ft_strlen(*word);
	new_word = malloc(sizeof(char) * (word_len + exit_len - 1));
	if (!new_word)
		return (ft_perror_free(NULL, *word, exit_code));
	ft_strlcpy(new_word, *word, *i + 1);
	ft_strlcpy(&new_word[*i], exit_code, exit_len + 1);
	ft_strlcpy(&new_word[*i + exit_len], &(*word)[*i + 2], word_len - *i - 1);
	free(*word);
	free(exit_code);
	*word = new_word;
	*i = *i + exit_len - 1;
	return (0);
}
