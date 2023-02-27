/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hassanAsarhan@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 12:12:04 by hsarhan           #+#    #+#             */
/*   Updated: 2022/05/15 12:50:39 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*handle_edge_case(int n)
{
	char	*str;

	if (n == INT_MIN)
	{
		str = malloc(sizeof(char) * 12);
		if (str == NULL)
			return (NULL);
		ft_strlcpy(str, "-2147483648", 12);
	}
	else
	{
		str = malloc(sizeof(char) * 2);
		if (str == NULL)
			return (NULL);
		ft_strlcpy(str, "0", 2);
	}
	return (str);
}

static int	get_num_digits(int num)
{
	int	num_digits;

	num_digits = 0;
	while (num > 0)
	{
		num /= 10;
		num_digits++;
	}
	return (num_digits);
}

static char	*fill_digits(char *str, int num, int num_digits, int is_neg)
{
	int		i;

	i = num_digits + is_neg - 1;
	while (i >= 0)
	{
		str[i--] = num % 10 + '0';
		num /= 10;
	}
	if (is_neg)
		str[0] = '-';
	str[num_digits + is_neg] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	int		num_digits;
	char	*str;
	int		is_neg;

	is_neg = 0;
	if (n == 0 || n == INT_MIN)
		return (handle_edge_case(n));
	else if (n < 0)
	{
		n *= -1;
		is_neg = 1;
	}
	num_digits = get_num_digits(n);
	str = malloc(sizeof(char) * (num_digits + is_neg + 1));
	if (str == NULL)
		return (NULL);
	fill_digits(str, n, num_digits, is_neg);
	return (str);
}
