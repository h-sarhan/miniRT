/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_settings.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 10:20:48 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/10 19:01:01 by hsarhan          ###   ########.fr       */
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

bool	parse_settings(t_scene *scene, char *settings_start, size_t line_num, int fd)
{
	(void)scene;
	(void)line_num;
	(void)fd;
	char	*parsed_str;
	char	*line;

	parsed_str = ft_strdup(settings_start);
	while (ft_strnstr(parsed_str, "}", ft_strlen(parsed_str)) == NULL && line != NULL)
	{
		// line = get_next_line(fd);
		line = ft_strtrim_free(get_next_line(fd), " \t\n");
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
	// Check that it starts with only one curly brace and ends with one as well
	
	if (parsed_str[0] != '{' || parsed_str[1] == '{' || parsed_str[ft_strlen(parsed_str) - 1] != '}')
	{

	}
	free(parsed_str);
	return (true);
}
