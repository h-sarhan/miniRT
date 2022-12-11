/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_settings.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 10:20:48 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/11 17:06:14 by hsarhan          ###   ########.fr       */
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
	printf("PARSED SETTINGS \n|%s|\n", parsed_str);
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
		printf("\t%s\n", settings[i]);
		i++;
	}
	free_split_array(settings);
	free(parsed_str);
	return (true);
}
