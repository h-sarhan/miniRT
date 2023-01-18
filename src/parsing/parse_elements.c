/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 16:32:52 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/18 17:18:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	parse_light_props(t_scene *scene, t_light *light, char **splitted)
{
	bool	success;

	success = true;
	light->type = POINT;
	parse_color(&light->color, splitted[3], &scene->error_flags.light.color);
	if (find_error(&scene->error_flags))
		return ;
	if (light->color.r != 0 && light->color.g != 0 && light->color.b != 0)
	{
		light->color.r = light->color.r / (light->color.r + light->color.g + light->color.b);
		light->color.g = light->color.g / (light->color.r + light->color.g + light->color.b);
		light->color.b = light->color.b / (light->color.r + light->color.g + light->color.b);
	}
	parse_coordinates(&light->position, splitted[1], &success);
	if (find_error(&scene->error_flags) == false && success == false)
		scene->error_flags.light.coords = true;
	light->intensity = ft_atof(splitted[2], &success);
	if (find_error(&scene->error_flags) == false && success == false)
		scene->error_flags.light.intensity_other = true;
	if (find_error(&scene->error_flags) == false
		&& (light->intensity < 0.0 || light->intensity > 1.0))
		scene->error_flags.light.intensity_range = true;
	light->intensity *= 1.3;
}

/**
 * @brief Parses a point light according to the specification provided in the
 *  scene file
 * @param scene The scene struct to fill with the light configuration
 * @param splitted The tokenized line containing the configuration of the light
 * @param line The line where the light is located
 * @param line_num The line number where the light is located
 * @return True if the light was parsed correctly
 */
void	parse_light(t_scene *scene, char **splitted)
{
	if (scene->count.lights == LIGHT_MAX)
	{
		scene->error_flags.light.max_lights = true;
		return ;
	}
	if (split_count(splitted) != 4)
	{
		scene->error_flags.light.other = true;
		return ;
	}
	if (scene->lights == NULL)
		scene->lights = ft_calloc(LIGHT_MAX, sizeof(t_light));
	if (scene->lights == NULL)
		return ;
	parse_light_props(scene, &scene->lights[scene->count.lights], splitted);
	identity_matrix(&scene->lights[scene->count.lights].added_rots);
	scene->count.lights++;
}

void	parse_spotlight_props(t_scene *scene, t_light *light, char **splitted)
{
	bool	success;

	success = true;
	light->type = SPOT;
	parse_coordinates(&light->position, splitted[1], &success);
	if (find_error(&scene->error_flags) == false && success == false)
		scene->error_flags.light.coords = true;
	light->intensity = ft_atof(splitted[2], &success);
	if (find_error(&scene->error_flags) == false && success == false)
		scene->error_flags.light.intensity_other = true;
	if (find_error(&scene->error_flags) == false
		&& (light->intensity < 0.0 || light->intensity > 1.0))
		scene->error_flags.light.intensity_range = true;
	light->intensity *= 1.3;
	parse_orientation(&light->init_direction, splitted[3], &scene->error_flags.light.orient);
	if (find_error(&scene->error_flags))
		return ;
	light->theta = ft_atof(splitted[4], &success);
	if (find_error(&scene->error_flags) == false && success == false)
		scene->error_flags.light.angle_other = true;
	if (find_error(&scene->error_flags) == false
		&& (light->theta < 0.0 || light->theta > 180))
		scene->error_flags.light.angle_range = true;
	light->theta *= DEG_TO_RAD;
	parse_color(&light->color, splitted[5], &scene->error_flags.light.color);
	if (find_error(&scene->error_flags))
		return ;
	if (light->color.r != 0 && light->color.g != 0 && light->color.b != 0)
	{
		light->color.r = light->color.r / (light->color.r + light->color.g + light->color.b);
		light->color.g = light->color.g / (light->color.r + light->color.g + light->color.b);
		light->color.b = light->color.b / (light->color.r + light->color.g + light->color.b);
	}
}

/**
 * @brief Parses a point light according to the specification provided in the
 *  scene file
 * @param scene The scene struct to fill with the light configuration
 * @param splitted The tokenized line containing the configuration of the light
 * @param line The line where the light is located
 * @param line_num The line number where the light is located
 * @return True if the light was parsed correctly
 */
void	parse_spotlight(t_scene *scene, char **splitted)
{
	if (scene->count.lights == LIGHT_MAX)
	{
		scene->error_flags.light.max_lights = true;
		return ;
	}
	if (split_count(splitted) != 6)
	{
		scene->error_flags.light.other = true;
		return ;
	}
	if (scene->lights == NULL)
		scene->lights = ft_calloc(LIGHT_MAX, sizeof(t_light));
	if (scene->lights == NULL)
		return ;
	parse_spotlight_props(scene, &scene->lights[scene->count.lights], splitted);
	identity_matrix(&scene->lights[scene->count.lights].added_rots);
	scene->count.lights++;
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
	if (scene->ambient.color.r != 0 && scene->ambient.color.g != 0 && scene->ambient.color.b != 0)
	{
		scene->ambient.color.r /= (scene->ambient.color.r + scene->ambient.color.g + scene->ambient.color.b);
		scene->ambient.color.g /= (scene->ambient.color.r + scene->ambient.color.g + scene->ambient.color.b);
		scene->ambient.color.b /= (scene->ambient.color.r + scene->ambient.color.g + scene->ambient.color.b);
	}
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
void	parse_camera(t_scene *scene, char **split)
{
	bool	success;

	success = true;
	if (split_count(split) != 4)
	{
		scene->error_flags.cam.other = true;
		return ;
	}
	parse_coordinates(&scene->cam.position, split[1], &success);
	if (find_error(&scene->error_flags) == false && success == false)
		scene->error_flags.cam.coords = true;
	parse_orientation(&scene->cam.dir, split[2], &scene->error_flags.cam.dir);
	if (find_error(&scene->error_flags))
		return ;
	if (!find_error(&scene->error_flags) && fabs(scene->cam.dir.x) < 0.01
		&& fabs(scene->cam.dir.y) > 0.01 && fabs(scene->cam.dir.z) < 0.01)
		scene->error_flags.cam.up_vector = true;
	scene->cam.fov = ft_atof(split[3], &success);
	if (!find_error(&scene->error_flags)
		&& (!success || !is_num(split[3], true)))
		scene->error_flags.cam.fov_other = true;
	if (find_error(&scene->error_flags) == false
		&& (scene->cam.fov < 1 || scene->cam.fov > 180))
		scene->error_flags.cam.fov_range = true;
	scene->count.cameras++;
}
