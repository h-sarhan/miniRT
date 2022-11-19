/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 10:10:43 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/19 11:32:20 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

/**
 * @brief Returns whether a character is whitespace
 * @param c Input character
 * @return True if whitespace
 */
bool	is_whitespace(const char c)
{
	return (c == ' ' || c == '\f'
		|| c == '\n' || c == '\r'
		|| c == '\t' || c == '\v');
}

/**
 * @brief Returns whether a string is entirely made up of whitespace characters
 * @param str Input string
 * @return True if every character is whitespace
 */
bool	all_whitespace(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (is_whitespace(str[i]) == false)
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief Count the number of commas in a string
 * @param str Input strng
 * @return Number of commas in string
 */
size_t	count_commas(const char *str)
{
	size_t	i;
	size_t	comma_count;

	i = 0;
	comma_count = 0;
	while (str[i] != '\0')
	{
		if (str[i] == ',')
			comma_count++;
		i++;
	}
	return (comma_count);
}

/**
 * @brief Returns whether a string is a number
 * @param str Input string
 * @param decimal Whether or not the number can be a floating point value
 * @return True if every character is a digit
 */
bool	is_num(const char *str, bool decimal)
{
	size_t	i;
	size_t	dot_count;

	i = 0;
	dot_count = 0;
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]) == false && str[i] != '-' && str[i] != '+'
			&& str[i] != '.')
			return (false);
		if (str[i] == '.')
			dot_count++;
		if ((str[i] == '-' || str[i] == '+') && i != 0)
			return (false);
		i++;
	}
	if (dot_count > 1 || (dot_count > 0 && decimal == false))
		return (false);
	if ((str[0] == '-' || str[0] == '+') && ft_strlen(str) == 1)
		return (false);
	if ((str[0] == '-' || str[0] == '+') && str[1] == '.'
		&& ft_strlen(str) == 2)
		return (false);
	return (true);
}

/**
 * @brief Counts the number of elements in an array returned by ft_split
 * @param split An array returned by ft_split
 * @return Number of elements in the array
 */
size_t	split_count(char **split)
{
	size_t	len;

	if (split == NULL)
		return (0);
	len = 0;
	while (split[len] != NULL)
		len++;
	return (len);
}

/**
 * @brief Returns whether an identifier is shape
 * @param identifier 
 * @return True if the identifier is a shape
 */
bool	is_shape(const char *identifier)
{
	return (ft_strncmp(identifier, "sp", ft_strlen(identifier)) == 0
		|| ft_strncmp(identifier, "pl", ft_strlen(identifier)) == 0
		|| ft_strncmp(identifier, "cy", ft_strlen(identifier)) == 0
		|| ft_strncmp(identifier, "cu", ft_strlen(identifier)) == 0);
}
