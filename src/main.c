/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:08:10 by mbany             #+#    #+#             */
/*   Updated: 2024/12/21 19:05:25 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// LIBFT

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (-1);
	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	if (n == 0)
		return (0);
	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0' && i < n - 1)
	{
		i++;
	}
	if (s1[i] == s2[i])
		return (0);
	else
		return (s1[i] - s2[i]);
}

char	*ft_strdup(const char *s)
{
	char	*s2;
	int		s_len;
	int		i;

	s_len = 0;
	while (s[s_len] != '\0')
		s_len++;
	s2 = (char *)malloc(sizeof(char) * (s_len + 1));
	if (!s2)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		s2[i] = s[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}
size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	unsigned int	i;

	if (!dst)
		return (0);
	i = 0;
	while (src[i] != '\0' && i + 1 < size)
	{
		dst[i] = src[i];
		i++;
	}
	if (size == 1)
		dst[0] = '\0';
	else if (size > 1)
		dst[i] = '\0';
	while (src[i] != '\0')
		i++;
	return (i);
}
int	ft_atoi(const char *nptr)
{
	int	result;
	int	sign;
	int	i;

	result = 0;
	sign = 1;
	i = 0;
	while (nptr[i] == ' ' || (nptr[i] >= '\t' && nptr[i] <= '\r'))
		i++;
	if (nptr[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (sign * result);
}

static int	get_num_digits(int n)
{
	int			num_digits;
	long int	temp;

	num_digits = 0;
	temp = (long int)n;
	if (n == 0)
		num_digits = 1;
	else
	{
		while (temp != 0)
		{
			num_digits++;
			temp /= 10;
		}
	}
	return (num_digits);
}

static int	is_negative(int n)
{
	if (n < 0)
		return (1);
	else
		return (0);
}

static void	convert_n(int n, char *str)
{
	int	i;
	int	num_digits;
	int	n_cp;

	n_cp = n;
	num_digits = get_num_digits(n);
	i = num_digits - 1 + is_negative(n);
	if (is_negative(n))
	{
		str[0] = '-';
		n_cp = -n;
	}
	while (i >= is_negative(n))
	{
		str[i] = '0' + (n_cp % 10);
		n_cp /= 10;
		i--;
	}
	str[num_digits + is_negative(n)] = '\0';
}

char	*ft_itoa(int n)
{
	char	*str;
	int		num_digits;

	num_digits = get_num_digits(n);
	str = (char *)malloc((num_digits + is_negative(n) + 1) * sizeof(char));
	if (!str)
		return (NULL);
	if (n == -2147483648)
	{
		ft_strlcpy(str, "-2147483648", 12);
		return (str);
	}
	convert_n(n, str);
	return (str);
}

char	*ft_strchr(const char *s, int c)
{
	const char	*p;
	int			i;

	p = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
		{
			p = &s[i];
			break ;
		}
		i++;
	}
	if (c == '\0')
		p = &s[i];
	return ((char *)p);
}




//ENVP

void	free_envp(t_envp *head)
{
	t_envp	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp->value);
		free(tmp);
	}
}

t_envp	*fetch_envp_node(t_envp *head, char *key)
{
	t_envp*node;
	size_t	key_len;

	key_len = ft_strlen(key);
	node = head;
	while (node != NULL)
	{
		if (ft_strncmp(key, node->value, key_len) == 0 && node->value[key_len] == '=')
			return (node);
		node = node->next;
	}
	return (NULL);
}

void increment_shlvl(t_envp *head)
{
	t_envp *node;
	char	*shlvl;
	int	shlvl_nb;

	node = fetch_envp_node(head, "SHLVL");
	shlvl = ft_strchr(node->value, '=') + 1;
	shlvl_nb = ft_atoi(shlvl);
	shlvl_nb++;
	shlvl = ft_itoa(shlvl_nb);
	if (!shlvl)
		perror("ft_itoa");
	free(node->value);
	if (!node->value)
		perror("ft_strjoin");
	free(shlvl);
}


t_envp *fetch_envp (char **envp)
{
	t_envp	*envp_node;
	t_envp	*envp_head;
	t_envp	*envp_node_prev;

	envp_node = NULL;
	envp_head = NULL;
	envp_node_prev = NULL;
	while (*envp != NULL)
	{
		envp_node = malloc(sizeof(t_envp));
		envp_node->value = ft_strdup(*envp);
		if(!envp_node || !envp_node->value)
		{
			free_envp(envp_head);
			return (NULL);
		}
		envp_node->next = NULL;
		if (envp_node_prev != NULL)
			envp_node_prev->next = envp_node;
		else
			envp_head = envp_node;
		envp_node_prev = envp_node;
		envp++;
	}
	return (envp_head);
}


// MAIN
void	init(t_data *data,int argc,char **argv,char **envp)
{
	(void)argv;
	if(argc != 1)
	{
		perror(MANY_ARGS_ERR);
		exit(-1);
	}
	if (envp == NULL || *envp == NULL)
	{
		perror(NO_ENVP_ERR);
		exit(-1);
	}
	data->envp = fetch_envp(envp);
	increment_shlvl(data->envp);
	data->cmd = NULL;
	data->envp_arr = NULL;
	data->line = NULL;
	data->cmd_exit_status = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	
	init(&data, argc, argv, envp);

}