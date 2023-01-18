/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_settings.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 10:20:48 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/18 17:58:15 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_color	parse_color_value(const char *str)
{
	int		hex_color;

	if (ft_strcmp_case(str, "blue") == 0)
		hex_color = 0x228be6;
	if (ft_strcmp_case(str, "red") == 0)
		hex_color = 0xf03e3e;
	if (ft_strcmp_case(str, "purple") == 0)
		hex_color = 0x7048e8;
	if (ft_strcmp_case(str, "green") == 0)
		hex_color = 0x37b24d;
	if (ft_strcmp_case(str, "yellow") == 0)
		hex_color = 0xf59f00;
	if (ft_strcmp_case(str, "pink") == 0)
		hex_color = 0xe64980;
	if (ft_strcmp_case(str, "black") == 0)
		hex_color = 0x212529;
	if (ft_strcmp_case(str, "gray") == 0)
		hex_color = 0x6d656d;
	if (ft_strcmp_case(str, "white") == 0)
		hex_color = 0xf8f9fa;
	if (ft_strcmp_case(str, "cyan") == 0)
		hex_color = 0x15aabf;
	if (ft_strcmp_case(str, "orange") == 0)
		hex_color = 0xf76707;
	return (int_to_color(hex_color));
}

void	parse_setting(t_shape *shape, char **key_val)
{
	bool	success;

	if (ft_strcmp("reflectiveness", key_val[0]) == 0)
		shape->props.reflectiveness = ft_atof(key_val[1], &success);
	if (ft_strcmp("diffuse", key_val[0]) == 0)
		shape->props.diffuse = ft_atof(key_val[1], &success);
	if (ft_strcmp("specular", key_val[0]) == 0)
		shape->props.specular = ft_atof(key_val[1], &success);
	if (ft_strcmp("shininess", key_val[0]) == 0)
		shape->props.shininess = ft_atof(key_val[1], &success);
	if (ft_strcmp("rotX", key_val[0]) == 0)
		shape->props.rot.x = ft_atol(key_val[1], &success);
	if (ft_strcmp("rotY", key_val[0]) == 0)
		shape->props.rot.y = ft_atol(key_val[1], &success);
	if (ft_strcmp("rotZ", key_val[0]) == 0)
		shape->props.rot.z = ft_atol(key_val[1], &success);
	if (ft_strcmp("scaleX", key_val[0]) == 0)
		shape->props.scale.x = ft_atof(key_val[1], &success);
	if (ft_strcmp("scaleY", key_val[0]) == 0)
		shape->props.scale.y = ft_atof(key_val[1], &success);
	if (ft_strcmp("scaleZ", key_val[0]) == 0)
		shape->props.scale.z = ft_atof(key_val[1], &success);
	if (ft_strcmp("color", key_val[0]) == 0)
		shape->props.color = parse_color_value(key_val[1]);
	if (ft_strcmp("diffuse_texture", key_val[0]) == 0)
		shape->diffuse_tex = parse_texture(key_val[1], shape);
	if (ft_strcmp("normal_texture", key_val[0]) == 0)
		shape->normal_tex = parse_texture(key_val[1], shape);
}

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
		while (ft_strncmp(line, "//", 2) == 0 && line != NULL)
		{
			free(line);
			line = ft_strtrim_free(get_next_line(fd), " \t\n");
			*line_num += 1;
		}
		if (line == NULL)
			break ;
		settings_str = ft_strjoin_free(ft_strtrim_free(settings_str, " \n\t"),
				line, 1);
	}
	settings_str = check_settings_str(settings_str, line_num);
	settings_str = ft_strtrim_free(settings_str, "{}");
	return (settings_str);
}

bool	parse_split_settings(t_scene *scene, char **settings)
{
	int		line_idx;
	char	**key_val;

	line_idx = 0;
	while (settings[line_idx] != NULL)
	{
		if (check_colons(settings[line_idx]) == false)
			return (false);
		key_val = ft_split(settings[line_idx], ':');
		key_val[0] = ft_strtrim_free(key_val[0], " \n\t");
		key_val[1] = ft_strtrim_free(key_val[1], " \n\t");
		if (!is_valid_key(key_val[0]) || !is_valid_val(key_val[0], key_val[1]))
		{
			if (!is_valid_key(key_val[0]))
				printf(INVALID_KEY, settings[line_idx], key_val[0]);
			free_split_array(key_val);
			return (false);
		}
		parse_setting(&scene->shapes[scene->count.shapes - 1], key_val);
		free_split_array(key_val);
		line_idx++;
	}
	return (true);
}

bool	parse_settings(t_scene *scene, const char *settings_start,
	size_t *line_num, int fd)
{
	char	*settings_str;
	char	**settings;

	if (scene->count.shapes == 0)
	{
		printf(SETTINGS_NO_SHAPE, *line_num);
		scene->error_flags.settings_err = true;
		return (false);
	}
	settings_str = get_settings_str(line_num, fd, settings_start);
	if (settings_str == NULL)
	{
		scene->error_flags.settings_err = true;
		return (false);
	}
	settings = ft_split(settings_str, ',');
	free(settings_str);
	if (parse_split_settings(scene, settings) == false)
	{
		scene->error_flags.settings_err = true;
		return (false);
	}
	free_split_array(settings);
	return (true);
}
