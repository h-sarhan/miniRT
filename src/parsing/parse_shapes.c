/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shapes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 16:29:40 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/05 16:44:08 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
	shape->props.reflectiveness = 0.1;
	shape->props.scale.x = shape->props.radius;
	shape->props.scale.y = shape->props.radius;
	shape->props.scale.z = shape->props.radius;
	shape->props.radius_squared = shape->props.radius * shape->props.radius;
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
	float	side_len;
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
	side_len = ft_atof(splitted[2], &success);
	if (find_error(&scene->error_flags) == false && success == false)
		scene->error_flags.shape.side_len_other = true;
	if (find_error(&scene->error_flags) == false && side_len <= 0.0)
		scene->error_flags.shape.side_len_range = true;
	parse_color(&shape->props.color, splitted[3],
		&scene->error_flags.shape.color);
	if (find_error(&scene->error_flags) == true)
		return ;
	ft_bzero(&shape->orientation, sizeof(t_vector));
	shape->orientation.y = 1;
	shape->props.reflectiveness = 0.1;
	shape->props.scale.x = side_len;
	shape->props.scale.y = side_len;
	shape->props.scale.z = side_len;
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
	shape->props.reflectiveness = 0.05;
	shape->props.distance_from_origin = \
		dot_product(&shape->orientation, &shape->origin);
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
	parse_orientation(&shape->orientation, splitted[2], &scene->error_flags.shape.orient);
	if (find_error(&scene->error_flags))
		return ;
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
	if (find_error(&scene->error_flags))
		return ;
	shape->props.scale.x = shape->props.radius;
	shape->props.scale.y = 1;
	shape->props.scale.z = shape->props.radius;
	shape->props.radius_squared = shape->props.radius * shape->props.radius;
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
// bool	parse_shape(t_scene *scene, char **splitted, size_t line_num,
// 		char *line)
bool	parse_shape(t_scene *scene, char **splitted)
{
	t_shape	*shape;

	if (scene->count.shapes == SHAPE_MAX)
	{
		scene->error_flags.shape.max_shapes = true;
		return (false);
	}
	if (scene->shapes == NULL)
		scene->shapes = ft_calloc(SHAPE_MAX, sizeof(t_shape));
	if (scene->shapes == NULL)
		return (false);
	shape = &scene->shapes[scene->count.shapes];
	if (ft_strcmp(splitted[0], "sp") == 0)
		parse_sphere(scene, shape, splitted);
	else if (ft_strcmp(splitted[0], "pl") == 0)
		parse_plane(scene, shape, splitted);
	else if (ft_strcmp(splitted[0], "cy") == 0)
		parse_cylinder(scene, shape, splitted);
	else if (ft_strcmp(splitted[0], "cu") == 0)
		parse_cube(scene, shape, splitted);
	else if (ft_strcmp(splitted[0], "co") == 0)
		parse_cone(scene, shape, splitted);
	if (find_error(&scene->error_flags))
		return (false);
	shape->id = scene->count.shapes;
	scene->count.shapes++;
	shape->props.diffuse = 0.9;
	shape->props.specular = 0.9;
	shape->props.shininess = 200;
	identity_matrix(&shape->added_rots);
	return (true);
}
