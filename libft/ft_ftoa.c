/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ftoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 15:31:54 by mkhan             #+#    #+#             */
/*   Updated: 2023/01/27 20:28:34 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*point_num(float num, int start, int len, char *str)
{
	int	decimal;
	int	i;

	decimal = 0;
	len = ft_strlen(str);
	str[len] = '.';
	num = num - (float)decimal;
	num = num - (float)start;
	num = fabs(num) * 100;
	decimal = (int)num;
	i = 1;
	while (i >= 0)
	{
		str[i + len + 1] = (decimal % 10) + '0';
		decimal /= 10;
		i--;
	}
	return (str);
}

static char	*dec_num(int is_negative, int len, int decimal, char *str)
{
	int	i;

	i = 0;
	if (is_negative)
		str[0] = '-';
	if (decimal == 0)
		len = 1;
	i = len - 1 + is_negative;
	while (i >= is_negative)
	{
		str[i] = (decimal % 10) + '0';
		decimal /= 10;
		i--;
	}
	return (str);
}

char	*ftoa(float num)
{
	char	*str;
	int		i[3];
	int		decimal[2];

	if (num < -2147483648 || num > 2147483647)
		return ("Invalid");
	i[2] = 0;
	if (num < 0)
	{
		i[2] = 1;
		num = -num;
	}
	decimal[0] = (int)num;
	decimal[1] = (int)num;
	i[1] = 0;
	i[0] = decimal[0];
	while (i[0] != 0)
	{
		i[0] /= 10;
		i[1]++;
	}
	str = (char *)ft_calloc(i[1] + 20, sizeof(char));
	dec_num(i[2], i[1], decimal[0], str);
	point_num(num, decimal[1], i[1], str);
	return (str);
}
