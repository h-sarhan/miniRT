/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 11:24:42 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/20 18:35:28 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a sphere
 * @param shape The sphere being parsed
 * @param line_num The number of the line containing the sphere
 * @param line The line containing the sphere
 */
static void	sphere_parse_error(const t_shape *shape, size_t line_num,
	const char *line)
{
	bool	color_check;

	color_check = check_color(&shape->color, line_num, line, "sphere");
	if (shape->radius <= 0)
	{
		printf(YELLOW"Error with sphere diameter on line #%ld\n"RED"->\t%s"
			RESET, line_num, line);
		printf(YELLOW"Diameter has to be a positive number\n"RESET);
	}
	else if (!color_check)
	{
		printf(YELLOW"Error with parsing sphere on line #%ld\n"RED"->\t%s"
			YELLOW"Correct syntax is \"sp [origin] [diameter] [color]\"\n"
			RESET, line_num, line);
	}
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a plane
 * @param shape The plane being parsed
 * @param line_num The number of the line containing the plane
 * @param line The line containing the plane
 */
static void	plane_parse_error(const t_shape *shape, size_t line_num,
	const char *line)
{
	bool	orientation_check;
	bool	color_check;

	orientation_check = check_orientation(&shape->orientation, line_num, line,
			"plane");
	color_check = check_color(&shape->color, line_num, line, "plane");
	if (!color_check && !orientation_check)
	{
		printf(YELLOW"Error with parsing plane on line #%ld\n"RED"->\t%s"
			YELLOW"Correct syntax is \"pl [origin] [orientation]"
			" [color]\"\n"RESET, line_num, line);
	}
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a cylinder
 * @param shape The cylinder being parsed
 * @param line_num The number of the line containing the cylinder
 * @param line The line containing the cylinder
 */
static void	cylinder_parse_error(const t_shape *shape, size_t line_num,
	const char *line)
{
	bool	orientation_check;
	bool	color_check;

	orientation_check = check_orientation(&shape->orientation, line_num, line,
			"cylinder");
	color_check = check_color(&shape->color, line_num, line, "cylinder");
	if (!color_check && !orientation_check && shape->radius <= 0)
	{
		printf(YELLOW"Error with cylinder diameter on line #%ld\n"
			RED"->\t%s"RESET, line_num, line);
		printf(YELLOW"Diameter has to be a positive number\n"RESET);
	}
	else if (!color_check && !orientation_check && shape->height <= 0)
	{
		printf(YELLOW"Error with cylinder height on line #%ld\n"
			RED"->\t%s"RESET, line_num, line);
		printf(YELLOW"Height has to be a positive number\n"RESET);
	}
	else if (!orientation_check && !color_check)
		printf(YELLOW"Error with parsing cylinder on line #%ld\n"RED"->\t%s\n"
			YELLOW"Correct syntax is "
			"\"cy [origin] [orientation] [diameter] [height] [color]\"\n"
			RESET, line_num, line);
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a shape
 * @param line Line where the parse error occured
 * @param line_num Number of the line where the error occured
 * @param scene Pointer to the scene struct
 * @param splitted Array to be freed
 */
void	*shape_parse_error(char *line, size_t line_num, t_scene *scene,
		char **splitted)
{
	t_shape	*shape;

	if (scene->shapes == NULL)
		shape = NULL;
	else
		shape = &scene->shapes[scene->count.shapes];
	if (shape && scene->count.shapes >= SHAPE_MAX)
		printf(RED"Error: Scene contains more than %d shapes\n"RESET,
			SHAPE_MAX);
	else if (shape && shape->type == SPHERE && split_count(splitted) == 4)
		sphere_parse_error(shape, line_num, line);
	else if (shape && shape->type == PLANE && split_count(splitted) == 4)
		plane_parse_error(shape, line_num, line);
	else if (shape && shape->type == CYLINDER && split_count(splitted) == 6)
		cylinder_parse_error(shape, line_num, line);
	else
		printf(YELLOW"Error with parsing shape on line #%ld\n"RED"->\t%s"RESET,
			line_num, line);
	free_scene(scene);
	get_next_line(-1);
	return (NULL);
}
