/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 16:32:52 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/20 16:32:59 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

// ! COMMENT THIS LATER
bool	parse_light(t_scene *scene, char **splitted, char *line,
	size_t line_num)
{
	t_light	*light;
	bool	success;

	success = true;
	if (scene->count.light_count == LIGHT_MAX || split_count(splitted) != 4)
		return (light_parse_error(line, line_num, scene));
	if (scene->lights == NULL)
		scene->lights = ft_calloc(LIGHT_MAX, sizeof(t_light));
	if (scene->lights == NULL)
		return (light_parse_error(line, line_num, scene));
	light = &scene->lights[scene->count.light_count];
	parse_coordinates(&light->position, splitted[1], &success);
	if (success == false)
		return (light_parse_error(line, line_num, scene));
	light->intensity = ft_atof(splitted[2], &success);
	if (success == false || light->intensity < 0.0 || light->intensity > 1.0)
		return (light_parse_error(line, line_num, scene));
	parse_color(&light->color, splitted[3], &success);
	if (success == false)
		return (light_parse_error(line, line_num, scene));
	scene->count.light_count++;
	return (true);
}

// ! COMMENT LATER
bool	parse_ambient(t_scene *scene, char **splitted, size_t line_num,
		char *line)
{
	bool	success;

	success = true;
	if (split_count(splitted) != 3)
	{
		ambient_parse_error(line, line_num, scene);
		return (false);
	}
	scene->ambient.intensity = ft_atof(splitted[1], &success);
	if (success == false || scene->ambient.intensity < 0.0
		|| scene->ambient.intensity > 1.0)
	{
		ambient_parse_error(line, line_num, scene);
		return (false);
	}
	parse_color(&scene->ambient.color, splitted[2], &success);
	if (success == false)
	{
		ambient_parse_error(line, line_num, scene);
		return (false);
	}
	scene->count.ambient_count++;
	return (true);
}

// ! COMMENT LATER
bool	parse_camera(t_scene *scene, char **splitted, char *line,
		size_t line_num)
{
	bool	success;

	success = true;
	if (split_count(splitted) != 4)
		return (camera_parse_error(line, line_num, scene));
	parse_coordinates(&scene->camera.position, splitted[1], &success);
	if (success == false)
		return (camera_parse_error(line, line_num, scene));
	parse_orientation(&scene->camera.orientation, splitted[2], &success);
	if (success == false)
		return (camera_parse_error(line, line_num, scene));
	scene->camera.fov = ft_atol(splitted[3], &success);
	if (success == false || scene->camera.fov < 0 || scene->camera.fov > 180)
		return (camera_parse_error(line, line_num, scene));
	scene->count.camera_count++;
	return (true);
}
