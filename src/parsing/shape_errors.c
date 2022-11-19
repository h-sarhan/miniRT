/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 11:24:42 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/19 15:44:48 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

// ! COMMENT THIS LATER
void	sphere_parse_error(const t_shape *shape, size_t line_num,
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

// ! COMMENT THIS LATER
void	plane_parse_error(const t_shape *shape, size_t line_num,
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

// ! COMMENT THIS LATER
void	cylinder_parse_error(const t_shape *shape, size_t line_num,
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
		printf(YELLOW"Error with parsing cylinder on line #%ld\n"RED"->\t%s"
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
		shape = &scene->shapes[scene->count.shape_count];
	if (shape && scene->count.shape_count >= SHAPE_MAX)
		printf(RED"Error: Scene contains more than %d shapes\n"RESET,
			SHAPE_MAX);
	else if (shape && shape->type == SPHERE)
		sphere_parse_error(shape, line_num, line);
	else if (shape && shape->type == PLANE)
		plane_parse_error(shape, line_num, line);
	else if (shape && shape->type == CYLINDER)
		cylinder_parse_error(shape, line_num, line);
	else
		printf(YELLOW"Error with parsing shape on line #%ld\n"RED"->\t%s"RESET,
			line_num, line);
	free(line);
	free_scene(scene);
	free_split_array(splitted);
	get_next_line(-1);
	return (NULL);
}
