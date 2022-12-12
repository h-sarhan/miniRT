/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_settings.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 10:20:48 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/12 11:44:22 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

char	*ft_strjoin_free(char *s1, char *s2, int fre)
{
	char	*str;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	str = ft_strjoin(s1, s2);
	if (fre == 1 || fre == 3)
		free(s1);
	if (fre == 2 || fre == 3)
		free(s2);
	return (str);
}

char	*ft_strtrim_free(char *s, char *set)
{
	char	*str;
	
	if (s == NULL)
		return (NULL);
	str = ft_strtrim(s, set);
	free(s);
	return (str);
}

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
	if (key != NULL && (ft_strcmp(key, "reflectiveness") == 0 ||
		ft_strcmp(key, "diffuse") == 0 ||
		ft_strcmp(key, "specular") == 0 ||
		ft_strcmp(key, "shininess") == 0 ||
		ft_strcmp(key, "rotX") == 0 ||
		ft_strcmp(key, "rotY") == 0 ||
		ft_strcmp(key, "rotZ") == 0 ||
		ft_strcmp(key, "scaleX") == 0 ||
		ft_strcmp(key, "scaleY") == 0 ||
		ft_strcmp(key, "scaleZ") == 0 ||
		ft_strcmp(key, "color") == 0))
		return (true);
	return (false);
}

bool	is_valid_color(const char *color)
{
	if (
		ft_strcmp_case(color, "blue") == 0 ||
		ft_strcmp_case(color, "red") == 0 ||
		ft_strcmp_case(color, "purple") == 0 ||
		ft_strcmp_case(color, "green") == 0 ||
		ft_strcmp_case(color, "yellow") == 0 ||
		ft_strcmp_case(color, "pink") == 0 ||
		ft_strcmp_case(color, "black") == 0 ||
		ft_strcmp_case(color, "gray") == 0
	)
		return (true);
	printf(YELLOW"Error with parsing this property\n"RED"->\t%s : %s\n"
				YELLOW"`%s` is not a valid value\n"MAGENTA
				"Available colors are BLUE, RED, PURPLE, GREEN, YELLOW, PINK, BLACK, GRAY\n"RESET, "color", color, color);
	return (false);
}

bool	is_valid_val(const char *key, const char *val)
{
	bool	success;
	double	parsed_value;

	success = true;
	if (val == NULL || ft_strlen(val) == 0)
		return (false);
	if (ft_strcmp(key, "reflectiveness") == 0 ||
		ft_strcmp(key, "diffuse") == 0 ||
		ft_strcmp(key, "specular") == 0)
	{
		if (is_num(val, true) == false)
		{
			printf(YELLOW"Error with parsing this property\n"RED"->\t%s : %s\n"
				YELLOW"`%s` is not a valid value\n"RESET, key, val, val);
			return (false);
		}
		else
		{
			parsed_value = ft_atof(val, &success);
			if (success == false || parsed_value < 0.0 || parsed_value > 1.0)
			{
				printf(YELLOW"Error with parsing this property\n"RED"->\t%s : %s\n"
					YELLOW"%s has to be between 0.0 and 1.0\n"RESET, key, val, key);
				return (false);
			}
		}
	}
	if (ft_strcmp(key, "shininess") == 0)
	{
		if (is_num(val, true) == false)
		{
			printf(YELLOW"Error with parsing this property\n"RED"->\t%s : %s\n"
				YELLOW"`%s` is not a valid value\n"RESET, key, val, val);
			return (false);
		}
		else
		{
			parsed_value = ft_atof(val, &success);
			if (success == false || parsed_value < 10 || parsed_value > 200)
			{
				printf(YELLOW"Error with parsing this property\n"RED"->\t%s : %s\n"
					YELLOW"%s has to be between 10.0 and 200.0\n"RESET, key, val, key);
				return (false);
			}
		}
	}
	if (ft_strcmp(key, "rotX") == 0 ||
		ft_strcmp(key, "rotY") == 0 ||
		ft_strcmp(key, "rotZ") == 0)
	{
		if (is_num(val, false) == false)
		{
			printf(YELLOW"Error with parsing this property\n"RED"->\t%s : %s\n"
				YELLOW"`%s` is not a valid value\n"RESET, key, val, val);
			return (false);
		}
		else
		{
			parsed_value = ft_atol(val, &success);
			if (success == false || parsed_value < 0 || parsed_value > 360)
			{
				printf(YELLOW"Error with parsing this property\n"RED"->\t%s : %s\n"
					YELLOW"%s has to be between 0 and 360 degrees\n"RESET, key, val, key);
				return (false);
			}
		}
	}
	if (ft_strcmp(key, "scaleX") == 0 ||
		ft_strcmp(key, "scaleY") == 0 ||
		ft_strcmp(key, "scaleZ") == 0)
	{
		if (is_num(val, true) == false)
		{
			printf(YELLOW"Error with parsing this property\n"RED"->\t%s : %s\n"
				YELLOW"`%s` is not a valid value\n"RESET, key, val, val);
			return (false);
		}
		else
		{
			parsed_value = ft_atof(val, &success);
			if (success == false || parsed_value < 0.1 || parsed_value > 50)
			{
				printf(YELLOW"Error with parsing this property\n"RED"->\t%s : %s\n"
					YELLOW"%s has to be between 0.1 and 50\n"RESET, key, val, key);
				return (false);
			}
		}
	}
	if (ft_strcmp(key, "color") == 0)
	{
		return (is_valid_color(val));
	}

	return (true);
}

bool	parse_settings(t_scene *scene, const char *settings_start, size_t line_num, int fd)
{
	(void)scene;
	(void)line_num;
	(void)fd;
	char	*parsed_str;
	char	*line;

	parsed_str = ft_strtrim(settings_start, " \n\t");
	while (ft_strnstr(parsed_str, "}", ft_strlen(parsed_str)) == NULL)
	{
		line = ft_strtrim_free(get_next_line(fd), " \t\n");
		if (line == NULL)
			break ;
		while (ft_strncmp(line, "//", 2) == 0 && line != NULL)
		{
			free(line);
			line = ft_strtrim_free(get_next_line(fd), " \t\n");
		}
		if (line == NULL)
			break ;
		parsed_str = ft_strjoin_free(ft_strtrim_free(parsed_str, " \n\t"),
			line, 1);
	}
	if (ft_strnstr(parsed_str, "}", ft_strlen(parsed_str)) == NULL)
	{
		printf(RED"Unterminated shape settings starting at line %ld\n"RESET,
			line_num);
		free(parsed_str);
		return (false);
	}
	if (ft_strlen(parsed_str) == 2)
	{
		printf(RED"Empty shape settings starting at line %ld\n"RESET, line_num);
		free(parsed_str);
		return (false);
	}
	// Count number of opening and closing braces
	int	opening = 0;
	int	closing = 0;
	int	i = 0;
	while (parsed_str[i] != '\0')
	{
		if (parsed_str[i] == '{')
			opening++;
		if (parsed_str[i] == '}')
			closing++;
		i++;
	}
	if (opening > 1)
	{
		printf(RED"Shape settings starting at line %ld"
			" contains an extra opening brace\n"RESET, line_num);
		free(parsed_str);
		return (false);
	}
	if (closing > 1)
	{
		printf(RED"Shape settings starting at line %ld"
			" contains an extra closing brace\n"RESET, line_num);
		free(parsed_str);
		return (false);
	}
	// Check that the parsed_string ends with a closing brace
	if (parsed_str[ft_strlen(parsed_str) - 1] != '}')
	{
		printf(RED"Shape settings starting at line %ld is not terminated"
			" correctly\n"RESET, line_num);
		free(parsed_str);
		return (false);
	}
	// Removing braces
	parsed_str = ft_strtrim_free(parsed_str, "{}");

	// Splitting on commas
	char	**settings;
	settings = ft_split(parsed_str, ',');
	i = 0;
	while (settings[i] != NULL)
	{
		int	colon_count = 0;
		int	idx = 0;
		while (settings[i][idx] != '\0')
		{
			if (settings[i][idx] == ':')
				colon_count++;
			idx++;
		}
		if (colon_count != 1)
		{
			printf(YELLOW"Error with parsing this property\n"RED"->\t%s\n"
				YELLOW"Correct syntax is KEY : VALUE\n"RESET, settings[i]);
			free_split_array(settings);
			free(parsed_str);
			return (false);
		}
		char	**key_val = ft_split(settings[i], ':');
		
		key_val[0] = ft_strtrim_free(key_val[0], " \n\t");
		key_val[1] = ft_strtrim_free(key_val[1], " \n\t");
		if (is_valid_key(key_val[0]) == false)
		{
			printf(YELLOW"Error with parsing this property\n"RED"->\t%s\n"
				YELLOW"`%s` is not a valid key\n"RESET, settings[i], key_val[0]);
			free_split_array(key_val);
			free_split_array(settings);
			free(parsed_str);
			return (false);
		}
		if (is_valid_val(key_val[0], key_val[1]) == false)
		{
			free_split_array(key_val);
			free_split_array(settings);
			free(parsed_str);
			return (false);
		}
		printf("Key == |%s|\n", key_val[0]);
		printf("Val == |%s|\n", key_val[1]);
		free_split_array(key_val);
		i++;
	}
	free_split_array(settings);
	free(parsed_str);
	return (true);
}
