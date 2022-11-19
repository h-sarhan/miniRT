/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 11:24:42 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/19 15:02:34 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

// ! WRITE COMMENTS LATER
bool	check_orientation(const t_vector *orientation, size_t line_num,
	const char *line, const char *element)
{
	if (orientation->x < 0.0 || orientation->x > 1.0
		|| orientation->y < 0.0 || orientation->y > 1.0
		|| orientation->z < 0.0 || orientation->z > 1.0)
	{
		printf(YELLOW"Error with parsing %s orientation on line #%ld\n"
			RED"->\t%s\n"RESET, element, line_num, line);
		if (orientation->x < 0.0 || orientation->x > 1.0)
			printf(YELLOW"The x value is out of range\n"RESET);
		if (orientation->y < 0.0 || orientation->y > 1.0)
			printf(YELLOW"The y value is out of range\n"RESET);
		if (orientation->z < 0.0 || orientation->z > 1.0)
			printf(YELLOW"The z value is out of range\n"RESET);
		return (true);
	}
	return (false);
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a camera
 * @param line Line where the parse error occured
 * @param line_num Number of the line where the error occured
 * @param scene Pointer to the scene struct
 * @param splitted Array to be freed
 * @param fd fd to be closed
 */
void	*camera_parse_error(char *line, size_t line_num, t_scene *scene,
		char **splitted)
{
	bool	orientation;

	orientation = check_orientation(&scene->camera.orientation, line_num, line,
			"camera");
	if (!orientation && (scene->camera.fov < 0 || scene->camera.fov > 180))
	{
		printf(YELLOW"Error with parsing camera fov on line #%ld\n"RED"->\t%s\n"
			RESET, line_num, line);
		printf(YELLOW"The fov value is out of range\n"RESET);
	}
	else if (!orientation)
		printf(YELLOW"Error with parsing camera on line #%ld\n"RED"->\t%s"RESET
			YELLOW"Correct syntax is \"C [origin] [orientation] [fov]\"\n"RESET,
			line_num, line);
	free(line);
	free_scene(scene);
	free_split_array(splitted);
	get_next_line(-1);
	return (NULL);
}

// ! COMMENT THIS LATER
bool	check_color(const t_color *color, size_t line_num, const char *line,
	const char *element)
{
	if (color->r < 0 || color->r > 255
		|| color->g < 0 || color->g > 255
		|| color->b < 0 || color->b > 255)
	{
		printf(YELLOW"Error with parsing %s color on line #%ld\n"
			RED"->\t%s\n"RESET, element, line_num, line);
		if (color->r < 0 || color->r > 255)
			printf(YELLOW"The red value is out of range\n"RESET);
		if (color->g < 0 || color->g > 255)
			printf(YELLOW"The green value is out of range\n"RESET);
		if (color->b < 0 || color->b > 255)
			printf(YELLOW"The blue value is out of range\n"RESET);
		return (true);
	}
	return (false);
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing an ambient light
 * @param line Line where the parse error occured
 * @param line_num Number of the line where the error occured
 * @param scene Pointer to the scene struct
 * @param splitted Array to be freed
 * @param fd fd to be closed
 */
void	*ambient_parse_error(char *line, size_t line_num, t_scene *scene,
		char **splitted)
{
	bool	color;

	color = check_color(&scene->ambient.color, line_num, line, "ambient light");
	if (!color && (scene->ambient.intensity < 0.0
			|| scene->ambient.intensity > 1.0))
	{
		printf(YELLOW"Ambient light intensity out of range on line #%ld\n"
			RED"->\t%s"RESET"Range is [0.0 -> 1.0]\n", line_num, line);
	}
	else if (!color)
	{
		printf(YELLOW"Error with parsing ambient light on line #%ld\n"
			RED"->\t%s"YELLOW"Correct syntax is \"A [intensity] [color]\"\n"
			RESET, line_num, line);
	}
	free(line);
	free_scene(scene);
	get_next_line(-1);
	free_split_array(splitted);
	return (NULL);
}

/**
 * @brief Prints an error message if an identifier is unrecognized
 * @param line Line where the identifier was located
 * @param line_num Number of the line where the identifier was located
 * @param scene Scene struct to be freed
 * @param splitted Array to be freed
 * @param fd File descriptor to be closed
 */
void	*unknown_identifier_error(char *line, size_t line_num, t_scene *scene,
		char **splitted)
{
	printf(YELLOW"Unknown identifier \"%s\" on line #%ld\n"RED"->\t%s"RESET,
		splitted[0], line_num, line);
	free(line);
	free_scene(scene);
	get_next_line(-1);
	free_split_array(splitted);
	return (NULL);
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a shape
 * @param line Line where the parse error occured
 * @param line_num Number of the line where the error occured
 * @param scene Pointer to the scene struct
 * @param splitted Array to be freed
 * @param fd fd to be closed
 */
void	*shape_parse_error(char *line, size_t line_num, t_scene *scene,
		char **splitted)
{
	t_shape	*shape;
	bool	color;
	bool	orientation;

	if (scene->shapes == NULL)
		shape = NULL;
	else
		shape = &scene->shapes[scene->count.shape_count];
	if (shape && scene->count.shape_count >= SHAPE_MAX)
		printf(RED"Error: Scene contains more than %d shapes\n"RESET,
			SHAPE_MAX);
	else if (shape && shape->type == SPHERE)
	{
		color = check_color(&shape->color, line_num, line, "sphere");
		if (shape->radius <= 0)
		{
			printf(YELLOW"Error with sphere diameter on line #%ld\n"RED"->\t%s"
				RESET, line_num, line);
			printf(YELLOW"Diameter has to be a positive number\n"RESET);
		}
		else if (!color)
			printf(YELLOW"Error with parsing sphere on line #%ld\n"RED"->\t%s"
				YELLOW"Correct syntax is \"sp [origin] [diameter] [color]\"\n"
				RESET, line_num, line);
	}
	else if (shape && shape->type == PLANE)
	{
		orientation = check_orientation(&shape->orientation, line_num, line,
				"plane");
		color = check_color(&shape->color, line_num, line, "plane");
		if (!color && !orientation)
			printf(YELLOW"Error with parsing plane on line #%ld\n"RED"->\t%s"
				YELLOW"Correct syntax is \"pl [origin] [orientation]"
				" [color]\"\n"RESET, line_num, line);
	}
	else if (shape && shape->type == CYLINDER)
	{
		orientation = check_orientation(&shape->orientation, line_num, line,
				"cylinder");
		color = check_color(&shape->color, line_num, line, "cylinder");
		if (!orientation && shape->radius <= 0)
		{
			printf(YELLOW"Error with cylinder diameter on line #%ld\n"
				RED"->\t%s"RESET, line_num, line);
			printf(YELLOW"Diameter has to be a positive number\n"RESET);
		}
		else if (!color && !orientation && shape->height <= 0)
		{
			printf(YELLOW"Error with cylinder height on line #%ld\n"
				RED"->\t%s"RESET, line_num, line);
			printf(YELLOW"Height has to be a positive number\n"RESET);
		}
		else if (!orientation && !color)
			printf(YELLOW"Error with parsing cylinder on line #%ld\n"RED"->\t%s"
				YELLOW"Correct syntax is "
				"\"cy [origin] [orientation] [diameter] [height] [color]\"\n"
				RESET, line_num, line);
	}
	else
		printf(YELLOW"Error with parsing shape on line #%ld\n"RED"->\t%s"RESET,
			line_num, line);
	free(line);
	free_scene(scene);
	free_split_array(splitted);
	get_next_line(-1);
	return (NULL);
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a point light
 * @param line Line where the parse error occured
 * @param line_num Number of the line where the error occured
 * @param scene Pointer to the scene struct
 * @param splitted Array to be freed
 * @param fd fd to be closed
 */
void	*light_parse_error(char *line, size_t line_num, t_scene *scene,
		char **splitted)
{
	t_light	*light;

	if (scene->lights == NULL)
		light = NULL;
	else
		light = &scene->lights[scene->count.light_count];
	if (light != NULL && scene->count.light_count >= LIGHT_MAX)
		printf(RED"Error: Scene contains more than %d lights\n"
			RESET, LIGHT_MAX);
	else if (light && (light->intensity < 0.0 || light->intensity > 1.0))
	{
		printf(YELLOW"Error with parsing light intensity on line #%ld\n"
			RED"->\t%s\n"RESET, line_num, line);
		printf(YELLOW"The intensity value is out of range\n"RESET);
	}
	else if (!light || !check_color(&light->color, line_num, line, "light"))
		printf(YELLOW"Error with parsing light on line #%ld\n"RED"->\t%s"RESET
			YELLOW"Correct syntax is \"L [origin] [intensity] [color\"\n"RESET,
			line_num, line);
	free(line);
	free_scene(scene);
	free_split_array(splitted);
	get_next_line(-1);
	return (NULL);
}
