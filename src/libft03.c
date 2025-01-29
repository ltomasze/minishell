/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft03.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 16:20:20 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/25 18:47:00 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	int		l;

	ptr = 0;
	if (!s)
		return (NULL);
	if (ft_strlen(s) < start || !len)
	{
		ptr = malloc(sizeof(char));
		if (!ptr)
			return (NULL);
		*ptr = 0;
		return (ptr);
	}
	if (len <= ft_strlen(s) - start)
		l = len + 1;
	else
		l = ft_strlen(s) - start + 1;
	ptr = (char *)malloc(sizeof(char) * l);
	if (!ptr)
		return (NULL);
	ft_strlcpy(ptr, s + start, l);
	return (ptr);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s_join;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	s_join = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!s_join)
		return (NULL);
	ft_strlcpy(s_join, s1, ft_strlen(s1) + 1);
	i = 0;
	while (i < ft_strlen(s2))
	{
		s_join[ft_strlen(s1) + i] = s2[i];
		i++;
	}
	s_join[ft_strlen(s1) + i] = '\0';
	return (s_join);
}

static int	num_words_count(char const *s, char c)
{
	int	num_words;
	int	i;

	num_words = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			num_words++;
		i++;
	}
	return (num_words);
}

static void	split_words(char **result, char const *s, char c, int i)
{
	int	start;
	int	j;
	int	k;

	j = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			start = i;
			while (s[i] && s[i] != c)
				i++;
			result[j] = malloc((i - start + 1) * sizeof(char));
			if (!result[j])
				return ;
			k = 0;
			while (start < i)
				result[j][k++] = s[start++];
			result[j++][k] = '\0';
		}
		else
			i++;
	}
}

char	**ft_split(char const *s, char c)
{
	int		num_words;
	char	**result;
	int		i;

	i = 0;
	num_words = num_words_count(s, c);
	result = (char **)malloc((num_words + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	split_words(result, s, c, i);
	result[num_words] = NULL;
	return (result);
}
