/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shapes2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 17:13:25 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/27 20:56:08 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	init_shape(t_shape *shape, t_scene *scene)
{
	shape->props.scale.x = shape->props.radius;
	shape->props.scale.y = shape->props.radius;
	shape->props.scale.z = shape->props.radius;
	shape->props.radius_squared = shape->props.radius * shape->props.radius;
	if (shape->type == CYLINDER)
	{
		ft_bzero(&shape->orientation, sizeof(t_vector));
		shape->orientation.y = 1;
		shape->props.scale.y = 1;
	}
	if (shape->type == PLANE)
		shape->props.distance_from_origin = \
			(dot_product(&shape->orientation, &shape->origin));
	shape->props.reflectiveness = 0.1;
	shape->id = scene->count.shapes;
	scene->count.shapes++;
	shape->props.diffuse = 0.9;
	shape->props.specular = 0.9;
	shape->props.shininess = 200;
	shape->props.pattern_type = NONE;
	shape->tex_tile = 1;
	identity_matrix(&shape->added_rots);
}

/**
 * @brief Parses a sphere object
 * @param shape The shape to parse
 * @param splitted The tokens containing the sphere configuration
 * @param success A boolean pointer set to false if there are any errors while
 *  parsing
 */
void	parse_sphere(t_scene *scene, t_shape *shape, char **splitted)
{
	bool	success;

	success = true;
	shape->type = SPHERE;
	if (split_count(splitted) != 4)
	{
		scene->error_flags.shape.other = true;
		return ;
	}
	parse_coordinates(&shape->origin, splitted[1], &success);
	if (!find_error(&scene->error_flags) && success == false)
		scene->error_flags.shape.origin = true;
	shape->props.radius = ft_atof(splitted[2], &success) / 2;
	if (!find_error(&scene->error_flags) && success == false)
		scene->error_flags.shape.diameter_other = true;
	if (!find_error(&scene->error_flags) && shape->props.radius <= 0.0)
		scene->error_flags.shape.diameter_range = true;
	parse_color(&shape->props.color, splitted[3],
		&scene->error_flags.shape.color);
	if (find_error(&scene->error_flags))
		return ;
}

/**
 * @brief Parses a cube object
 * @param shape The shape to parse
 * @param splitted The tokens containing the sphere configuration
 * @param success A boolean pointer set to false if there are any errors while
 *  parsing
 */
void	parse_cube(t_scene *scene, t_shape *shape, char **splitted)
{
	bool	success;

	success = true;
	shape->type = CUBE;
	if (split_count(splitted) != 4)
	{
		scene->error_flags.shape.other = true;
		return ;
	}
	parse_coordinates(&shape->origin, splitted[1], &success);
	if (find_error(&scene->error_flags) == false && success == false)
		scene->error_flags.shape.origin = true;
	shape->props.radius = ft_atof(splitted[2], &success);
	if (find_error(&scene->error_flags) == false && success == false)
		scene->error_flags.shape.side_len_other = true;
	if (find_error(&scene->error_flags) == false && shape->props.radius <= 0.0)
		scene->error_flags.shape.side_len_range = true;
	parse_color(&shape->props.color, splitted[3],
		&scene->error_flags.shape.color);
	if (find_error(&scene->error_flags) == true)
		return ;
}

/**
 * @brief Parses a plane object
 * @param shape The shape to parse
 * @param splitted The tokens containing the plane configuration
 * @param success A boolean pointer set to false if there are any errors while
 *  parsing
 */
void	parse_plane(t_scene *scene, t_shape *shape, char **splitted)
{
	bool	success;

	success = true;
	shape->type = PLANE;
	if (split_count(splitted) != 4)
	{
		scene->error_flags.shape.other = true;
		return ;
	}
	parse_coordinates(&shape->origin, splitted[1], &success);
	if (find_error(&scene->error_flags) == false && success == false)
		scene->error_flags.shape.origin = true;
	parse_orientation(&shape->orientation, splitted[2],
		&scene->error_flags.shape.orient);
	if (find_error(&scene->error_flags) == true)
		return ;
	parse_color(&shape->props.color, splitted[3],
		&scene->error_flags.shape.color);
	if (find_error(&scene->error_flags) == true)
		return ;
}

/**
 * @brief Returns whether an identifier is shape
 * @param identifier 
 * @return True if the identifier is a shape
 */
bool	is_shape(const char *identifier)
{
	return (ft_strcmp(identifier, "sp") == 0
		|| ft_strcmp(identifier, "pl") == 0
		|| ft_strcmp(identifier, "cy") == 0
		|| ft_strcmp(identifier, "cu") == 0
		|| ft_strcmp(identifier, "co") == 0);
}
