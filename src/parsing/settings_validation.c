/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings_validation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:19:56 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/18 18:00:13 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

bool	is_settings(const char *line)
{
	size_t	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '{')
			return (true);
		if (is_space(line[i]) == false)
			return (false);
		i++;
	}
	return (false);
}

bool	is_valid_key(const char *key)
{
	if (key != NULL && (ft_strcmp(key, "reflectiveness") == 0
			|| ft_strcmp(key, "diffuse") == 0
			|| ft_strcmp(key, "specular") == 0
			|| ft_strcmp(key, "shininess") == 0
			|| ft_strcmp(key, "rotX") == 0
			|| ft_strcmp(key, "rotY") == 0
			|| ft_strcmp(key, "rotZ") == 0
			|| ft_strcmp(key, "scaleX") == 0
			|| ft_strcmp(key, "scaleY") == 0
			|| ft_strcmp(key, "scaleZ") == 0
			|| ft_strcmp(key, "color") == 0
			|| ft_strcmp(key, "normal_texture") == 0
			|| ft_strcmp(key, "diffuse_texture") == 0
			))
		return (true);
	return (false);
}

bool	is_valid_color(const char *color)
{
	if (ft_strcmp_case(color, "blue") == 0
		|| ft_strcmp_case(color, "red") == 0
		|| ft_strcmp_case(color, "purple") == 0
		|| ft_strcmp_case(color, "green") == 0
		|| ft_strcmp_case(color, "yellow") == 0
		|| ft_strcmp_case(color, "pink") == 0
		|| ft_strcmp_case(color, "black") == 0
		|| ft_strcmp_case(color, "gray") == 0
		|| ft_strcmp_case(color, "white") == 0
		|| ft_strcmp_case(color, "cyan") == 0
		|| ft_strcmp_case(color, "orange") == 0)
		return (true);
	printf(COLOR_SETTING_ERROR, "color", color, color);
	return (false);
}

bool	check_value(const char *key, const char *val, float min, float max)
{
	bool	success;
	float	parsed_value;

	success = true;
	if (ft_strcmp_case(key, "diffuse_texture") == 0 || ft_strcmp_case(key, "normal_texture") == 0)
	{
		if (ft_strnstr(val, ".ppm\"", ft_strlen(val)) == NULL
			|| ft_strcmp(&val[ft_strlen(val) - 5], ".ppm\"") != 0)
		{
			printf(INVALID_PROPERTY_VALUE, key, val, val);
			printf(YELLOW"Only valid .ppm files are supported\n"RESET);
			return (false);
		}
		return (true);
	}
	if (is_num(val, true) == false)
	{
		printf(INVALID_PROPERTY_VALUE, key, val, val);
		return (false);
	}
	parsed_value = ft_atof(val, &success);
	if (success == false || parsed_value < min || parsed_value > max)
	{
		printf(INVALID_PROPERTY_RANGE, key, val, key, min, max);
		return (false);
	}
	return (true);
}

bool	is_valid_val(const char *key, const char *val)
{
	float	min;
	float	max;

	if (val == NULL || ft_strlen(val) == 0)
		return (false);
	min = 0.0;
	max = 1.0;
	if (ft_strcmp(key, "shininess") == 0)
	{
		min = 10;
		max = 400;
	}
	if (ft_strncmp(key, "rot", 3) == 0)
		max = 360;
	if (ft_strncmp(key, "scale", 5) == 0)
	{
		min = 0.1;
		max = 50;
	}
	if (ft_strcmp(key, "color") == 0)
		return (is_valid_color(val));
	return (check_value(key, val, min, max));
}
