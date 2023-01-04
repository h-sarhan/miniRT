/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 16:32:52 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/04 22:24:51 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Parses a point light according to the specification provided in the
 *  scene file
 * @param scene The scene struct to fill with the light configuration
 * @param splitted The tokenized line containing the configuration of the light
 * @param line The line where the light is located
 * @param line_num The line number where the light is located
 * @return True if the light was parsed correctly
 */
// bool	parse_light(t_scene *scene, char **splitted, char *line,
// 	size_t line_num)
bool	parse_light(t_scene *scene, char **splitted)
{
	t_light	*light;
	bool	success;

	success = true;
	if (scene->count.lights == LIGHT_MAX)
	{
		scene->error_flags.light.max_lights = true;
		return (false);
	}
	if (split_count(splitted) != 4)
	{
		scene->error_flags.light.other = true;
		return (false);
	}
	if (scene->lights == NULL)
		scene->lights = ft_calloc(LIGHT_MAX, sizeof(t_light));
	if (scene->lights == NULL)
		return (false);
	light = &scene->lights[scene->count.lights];
	parse_coordinates(&light->position, splitted[1], &success);
	if (success == false)
	{
		scene->error_flags.light.coords = true;
		return (false);
	}
	light->intensity = ft_atof(splitted[2], &success);
	if (success == false)
	{
		scene->error_flags.light.intensity_other = true;
		return (false);
	}
	if (light->intensity < 0.0 || light->intensity > 1.0)
	{
		scene->error_flags.light.intensity_range = true;
		return (false);
	}
	parse_color(&light->color, splitted[3], &scene->error_flags.light.color);
	if (find_error(&scene->error_flags))
	{
		return (false);
	}
	scene->count.lights++;
	return (true);
}

/**
 * @brief Parses an ambient light according to the specification provided in the
 *  scene file
 * @param scene The scene struct to fill with the ambient light configuration
 * @param splitted The tokenized line containing the configuration of the 
 * ambient light
 * @param line_num The line number where the ambient light is located
 * @param line The line where the ambient light is located
 * @return True if the ambient light was parsed correctly
 */
bool	parse_ambient(t_scene *scene, char **splitted)
// bool	parse_ambient(t_scene *scene, char **splitted, size_t line_num,
// 		char *line)
{
	bool	success;

	success = true;
	if (split_count(splitted) != 3)
	{
		scene->error_flags.ambient.other = true;
		return (false);
	}
	scene->ambient.intensity = ft_atof(splitted[1], &success);
	if (success == false || scene->ambient.intensity < 0.0
		|| scene->ambient.intensity > 1.0)
	{
		if (success == true)
			scene->error_flags.ambient.intensity_range = true;
		else
			scene->error_flags.ambient.intensity_other = true;
		return (false);
	}
	parse_color(&scene->ambient.color, splitted[2],
		&scene->error_flags.ambient.color);
	if (find_error(&scene->error_flags))
		return (false);
	scene->count.ambient_lights++;
	return (true);
}

/**
 * @brief Parses a camera according to the specification provided in the
 *  scene file
 * @param scene The scene struct to fill with the camera information
 * @param splitted The tokenized line containing the camera configuration
 * @param line The line containing the camera configuration
 * @param line_num The line number containing the camera
 * @return True if the camera was parsed correctly
 */
// bool	parse_camera(t_scene *scene, char **splitted, char *line,
// 		size_t line_num)
bool	parse_camera(t_scene *scene, char **splitted)
{
	bool	success;

	success = true;
	if (split_count(splitted) != 4)
	{
		// other error
		scene->error_flags.cam.other = true;
		return (false);
	}
	parse_coordinates(&scene->camera.position, splitted[1], &success);
	scene->camera.position.w = 1;
	if (success == false)
	{
		scene->error_flags.cam.coords = true;
		return (false);
	}
	parse_orientation(&scene->camera.dir, splitted[2], &scene->error_flags.cam.orient);
	if (find_error(&scene->error_flags))
	{
		return (false);
	}
	if (fabs(scene->camera.dir.x) < 0.001 && fabs(scene->camera.dir.y) > 0.001 && fabs(scene->camera.dir.z) < 0.001)
	{
		scene->error_flags.cam.up_vector = true;
		return (false);
	}
	scene->camera.fov = ft_atof(splitted[3], &success);
	if (success == false || is_num(splitted[3], true) == false)
	{
		scene->error_flags.cam.fov_other = true;
		return (false);
	}
	if (scene->camera.fov < 1 || scene->camera.fov > 180)
	{
		scene->error_flags.cam.fov_range = true;
		return (false);
	}
	scene->count.cameras++;
	return (true);
}
