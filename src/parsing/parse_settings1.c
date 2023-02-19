/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_settings1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 22:43:38 by mkhan             #+#    #+#             */
/*   Updated: 2023/02/19 22:44:13 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

char	*get_settings_str(size_t *line_num, int fd, const char *settings_start)
{
	char	*settings_str;
	char	*line;

	settings_str = ft_strtrim(settings_start, " \n\t");
	while (ft_strnstr(settings_str, "}", ft_strlen(settings_str)) == NULL)
	{
		line = ft_strtrim_free(get_next_line(fd), " \t\n");
		*line_num += 1;
		if (line == NULL)
			break ;
		while ((ft_strncmp(line, "//", 2) == 0
				|| ft_strncmp(line, "#", 1) == 0) && line != NULL)
		{
			free(line);
			line = ft_strtrim_free(get_next_line(fd), " \t\n");
			*line_num += 1;
		}
		if (line == NULL)
			break ;
		settings_str = ft_strjoin_free(ft_strtrim_free(settings_str, " \n\t"),
				line, 3);
	}
	settings_str = check_settings_str(settings_str, line_num);
	settings_str = ft_strtrim_free(settings_str, "{}");
	return (settings_str);
}

bool	check_key_val_split(char **key_val, char **settings, int line_idx)
{
	if (!key_val[0] || !*key_val[0] || !key_val[1] || !*key_val[1])
	{
		if (key_val[0] == NULL)
			printf(INVALID_KEY, settings[line_idx], key_val[0]);
		else
			printf(INVALID_PROP_VALUE, key_val[0], key_val[1], key_val[1]);
		return (free_split_array(key_val), false);
	}
	return (true);
}

bool	check_key_vals(char **key_val, char **settings, t_scene *scene,
			int line_idx)
{
	if (!key_val[0] || !*key_val[0] || !key_val[1] || !*key_val[1])
	{
		if (key_val[0] == NULL)
			printf(INVALID_KEY, settings[line_idx], key_val[0]);
		else
			printf(INVALID_PROP_VALUE, key_val[0], key_val[1], key_val[1]);
		return (free_split_array(key_val), false);
	}
	if (!is_valid_key(key_val[0]) || !is_valid_val(key_val[0], key_val[1]))
	{
		if (!is_valid_key(key_val[0]))
			printf(INVALID_KEY, settings[line_idx], key_val[0]);
		return (free_split_array(key_val), false);
	}
	if (!parse_setting(&scene->shapes[scene->count.shapes - 1], key_val))
		return (free_split_array(key_val), false);
	return (true);
}
