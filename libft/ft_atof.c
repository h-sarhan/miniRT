/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:26:16 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/19 10:22:41 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static bool	is_whitespace(const char c)
{
	return (c == ' ' || c == '\f'
		|| c == '\n' || c == '\r'
		|| c == '\t' || c == '\v');
}

static int	atof_split(char c)
{
	char	*s;
	int		i;

	s = malloc (sizeof(char) * 2);
	s[0] = c;
	s[1] = '\0';
	i = ft_atoi(s);
	free(s);
	return (i);
}

double	ft_atof(const char *str, bool *success)
{
	double	i;
	int		sign;
	double	j;
	int		k;

	i = 0;
	j = 0;
	k = 1;
	if (str[0] == '.' && str[1] == '\0')
		*success = false;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str != '.' && ft_isdigit(*str))
		i = i * 10.0 + atof_split(*str++);
	if (*str == '.')
		str++;
	while (*str != '\0' && ft_isdigit(*str))
		j = j + atof_split(*str++) / pow(10.0, k++);
	if (*str != '\0' && !ft_isdigit(*str) && !is_whitespace(*str))
		*success = false;
	return (sign * (i + j));
}
