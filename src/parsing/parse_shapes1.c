/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shapes1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 16:29:40 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/05 17:16:22 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	parse_cylinder_props(t_shape *shape, t_scene *scene, char **splitted)
{
	bool	success;

	success = true;
	shape->props.radius = ft_atof(splitted[3], &success) / 2;
	if (find_error(&scene->error_flags) == false && success == false)
		scene->error_flags.shape.diameter_other = true;
	if (find_error(&scene->error_flags) == false && shape->props.radius <= 0.0)
		scene->error_flags.shape.diameter_range = true;
	shape->props.height = ft_atof(splitted[4], &success);
	if (find_error(&scene->error_flags) == false && success == false)
		scene->error_flags.shape.height_other = true;
	if (find_error(&scene->error_flags) == false && shape->props.height <= 0.0)
		scene->error_flags.shape.height_range = true;
	parse_color(&shape->props.color, splitted[5],
		&scene->error_flags.shape.color);
}

/**
 * @brief Parses a cylinder object
 * @param shape The shape to parse
 * @param splitted The tokens containing the cylinder configuration
 * @param success A boolean pointer set to false if there are any errors while
 *  parsing
 */
void	parse_cylinder(t_scene *scene, t_shape *shape, char **splitted)
{
	bool	success;

	success = true;
	shape->type = CYLINDER;
	if (split_count(splitted) != 6)
	{
		scene->error_flags.shape.other = true;
		return ;
	}
	parse_coordinates(&shape->origin, splitted[1], &success);
	if (find_error(&scene->error_flags) == false && success == false)
		scene->error_flags.shape.origin = true;
	parse_orientation(&shape->orientation, splitted[2],
		&scene->error_flags.shape.orient);
	if (find_error(&scene->error_flags))
		return ;
	parse_cylinder_props(shape, scene, splitted);
	if (find_error(&scene->error_flags))
		return ;
}

/**
 * @brief Parses a cone object
 * @param shape The shape to parse
 * @param splitted The tokens containing the cone configuration
 * @param success A boolean pointer set to false if there are any errors while
 *  parsing
 */
void	parse_cone(t_scene *scene, t_shape *shape, char **splitted)
{
	parse_cylinder(scene, shape, splitted);
	shape->type = CONE;
}

/**
 * @brief Parses a shape
 * @param scene Pointer to scene struct
 * @param splitted The line containing the shape configurations
 * @param line_num The number of the line containing the shape
 * @param line The line containing the shape
 * @return Whether the parsing succeeded or not
 */
bool	parse_shape(t_scene *scene, char **splitted)
{
	if (scene->count.shapes == SHAPE_MAX)
	{
		scene->error_flags.shape.max_shapes = true;
		return (false);
	}
	if (scene->shapes == NULL)
		scene->shapes = ft_calloc(SHAPE_MAX, sizeof(t_shape));
	if (scene->shapes == NULL)
		return (false);
	if (ft_strcmp(splitted[0], "sp") == 0)
		parse_sphere(scene, &scene->shapes[scene->count.shapes], splitted);
	else if (ft_strcmp(splitted[0], "pl") == 0)
		parse_plane(scene, &scene->shapes[scene->count.shapes], splitted);
	else if (ft_strcmp(splitted[0], "cy") == 0)
		parse_cylinder(scene, &scene->shapes[scene->count.shapes], splitted);
	else if (ft_strcmp(splitted[0], "cu") == 0)
		parse_cube(scene, &scene->shapes[scene->count.shapes], splitted);
	else if (ft_strcmp(splitted[0], "co") == 0)
		parse_cone(scene, &scene->shapes[scene->count.shapes], splitted);
	if (find_error(&scene->error_flags))
		return (false);
	init_shape(&scene->shapes[scene->count.shapes], scene);
	return (true);
}
