/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_spot_light.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 22:40:56 by mkhan             #+#    #+#             */
/*   Updated: 2023/02/19 22:41:54 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	parse_spotlight_props2(t_light *light, char **splitted, t_scene *scene)
{
	bool	success;
	double	col_sum;

	success = true;
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
	col_sum = light->color.r + light->color.g + light->color.b;
	if (light->color.r != 0 && light->color.g != 0 && light->color.b != 0)
	{
		light->color.r /= col_sum;
		light->color.g /= col_sum;
		light->color.b /= col_sum;
	}
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
	light->intensity *= 1.8;
	parse_orientation(&light->init_direction, splitted[3],
		&scene->error_flags.light.orient);
	if (find_error(&scene->error_flags))
		return ;
	parse_spotlight_props2(light, splitted, scene);
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
