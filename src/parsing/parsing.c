/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:00:17 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/20 16:27:45 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

/**
 * @brief Parse the red, green, and blue values of a color
 * @param color Color struct to be filled with RGB values
 * @param str Raw string in the form
 * @param success Boolean pointer that is set to false on error
 */
void	parse_color(t_color *color, const char *str, bool *success)
{
	char	**rgb;
	long	res[3];
	bool	parse_success;
	size_t	i;

	parse_success = true;
	rgb = ft_split(str, ',');
	if (!rgb || count_commas(str) != 2 || split_count(rgb) != 3)
	{
		*success = false;
		free_split_array(rgb);
		return ;
	}
	i = -1;
	while (rgb[++i] != NULL)
	{
		res[i] = ft_atol(rgb[i], success);
		if (!is_num(rgb[i], false) || res[i] < 0 || res[i] > 255 || !*success)
			parse_success = false;
	}
	free_split_array(rgb);
	color->r = res[0];
	color->g = res[1];
	color->b = res[2];
	*success = parse_success;
}

/**
 * @brief Parses given coordinates into a vector
 * @param position Pointer to a vector that will be filled with coordinates
 * @param str Raw string to be parsed
 * @param success Boolean pointer that will be set to false on error
 */
void	parse_coordinates(t_vector *position, const char *str, bool *success)
{
	float	res[3];
	char	**coords;
	bool	parse_success;
	size_t	i;

	parse_success = true;
	coords = ft_split(str, ',');
	if (!coords || count_commas(str) != 2 || split_count(coords) != 3)
	{
		*success = false;
		free_split_array(coords);
		return ;
	}
	i = -1;
	while (coords[++i] != NULL)
	{
		res[i] = ft_atof(coords[i], success);
		if (!is_num(coords[i], true) || *success == false)
			parse_success = false;
	}
	free_split_array(coords);
	position->x = res[0];
	position->y = res[1];
	position->z = res[2];
	*success = parse_success;
}

/**
 * @brief Parses given orientation into a vector
 * @param position Pointer to a vector that will be filled with orientation
 * @param str Raw string to be parsed
 * @param success Boolean pointer that will be set to false on error
 */
void	parse_orientation(t_vector *orientation, const char *str, bool *success)
{
	float	res[3];
	char	**coords;
	size_t	i;
	bool	parse_success;

	parse_success = true;
	coords = ft_split(str, ',');
	if (coords == NULL || count_commas(str) != 2 || split_count(coords) != 3)
	{
		*success = false;
		free_split_array(coords);
		return ;
	}
	i = -1;
	while (coords[++i] != NULL)
	{
		res[i] = ft_atof(coords[i], success);
		if (!is_num(coords[i], true) || !*success || res[i] < -1 || res[i] > 1)
			parse_success = false;
	}
	free_split_array(coords);
	orientation->x = res[0];
	orientation->y = res[1];
	orientation->z = res[2];
	*success = parse_success;
}

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

// ! COMMENT THIS LATER
void	parse_sphere(t_shape *shape, char **splitted, bool *success)
{
	bool	parse_success;

	parse_success = true;
	shape->type = SPHERE;
	if (split_count(splitted) != 4)
	{
		*success = false;
		shape->radius = 0.1;
		return ;
	}
	shape->radius = ft_atof(splitted[2], success) / 2;
	if (*success == false || shape->radius <= 0.0)
		parse_success = false;
	parse_coordinates(&shape->origin, splitted[1], success);
	if (*success == false)
		parse_success = false;
	parse_color(&shape->color, splitted[3], success);
	if (*success == false)
		parse_success = false;
	*success = parse_success;
}

// ! COMMENT THIS LATER
void	parse_plane(t_shape *shape, char **splitted, bool *success)
{
	bool	parse_success;

	parse_success = true;
	shape->type = PLANE;
	if (split_count(splitted) != 4)
	{
		*success = false;
		return ;
	}
	parse_coordinates(&shape->origin, splitted[1], success);
	if (*success == false)
		parse_success = false;
	parse_orientation(&shape->orientation, splitted[2], success);
	if (*success == false)
		parse_success = false;
	parse_color(&shape->color, splitted[3], success);
	if (*success == false)
		parse_success = false;
	*success = parse_success;
}

// ! COMMENT THIS LATER
void	parse_cylinder(t_shape *shape, char **splitted, bool *success)
{
	bool	parse_success;

	parse_success = true;
	shape->type = CYLINDER;
	if (split_count(splitted) != 6)
		*success = false;
	if (*success == false)
		return ;
	shape->radius = ft_atof(splitted[3], success) / 2;
	if (*success == false || shape->radius <= 0.0)
		parse_success = false;
	shape->height = ft_atof(splitted[4], success);
	if (*success == false || shape->height <= 0.0)
		parse_success = false;
	parse_coordinates(&shape->origin, splitted[1], success);
	if (*success == false)
		parse_success = false;
	parse_orientation(&shape->orientation, splitted[2], success);
	if (*success == false)
		parse_success = false;
	parse_color(&shape->color, splitted[5], success);
	if (*success == false)
		parse_success = false;
	*success = parse_success;
}

/**
 * @brief Parses a shape
 * @param scene Pointer to scene struct
 * @param splitted The line containing the shape configurations
 * @return Whether the parsing succeeded or not
 */
bool	parse_shape(t_scene *scene, char **splitted, size_t line_num,
		char *line)
{
	t_shape	*shape;
	bool	success;

	success = true;
	if (scene->count.shape_count == SHAPE_MAX)
		return (shape_parse_error(line, line_num, scene, splitted));
	if (scene->shapes == NULL)
		scene->shapes = ft_calloc(SHAPE_MAX, sizeof(t_shape));
	if (scene->shapes == NULL)
		return (shape_parse_error(line, line_num, scene, splitted));
	shape = &scene->shapes[scene->count.shape_count];
	if (ft_strcmp(splitted[0], "sp") == 0)
		parse_sphere(shape, splitted, &success);
	else if (ft_strcmp(splitted[0], "pl") == 0)
		parse_plane(shape, splitted, &success);
	else if (ft_strcmp(splitted[0], "cy") == 0)
		parse_cylinder(shape, splitted, &success);
	if (success == false)
		return (shape_parse_error(line, line_num, scene, splitted));
	scene->count.shape_count++;
	return (true);
}

// ! DOCUMENT LATER
bool	check_element_count(t_scene *scene)
{
	if (scene->count.ambient_count > 1 || scene->count.ambient_count == 0)
	{
		if (scene->count.ambient_count > 1)
			printf(RED"Error: Scene contains multiple ambient light\n"RESET);
		else
			printf(RED"Error: Scene contains no ambient lights\n"RESET);
		free_scene(scene);
		return (false);
	}
	if (scene->count.camera_count > 1 || scene->count.camera_count == 0)
	{
		if (scene->count.camera_count > 1)
			printf(RED"Error: Scene contains more than one camera\n"RESET);
		else
			printf(RED"Error: Scene contains no cameras\n"RESET);
		free_scene(scene);
		return (false);
	}
	return (true);
}

// ! DOCUMENT THIS
bool	skip_line(char **line, int fd, size_t *line_count)
{
	if (ft_strlen(*line) == 0 || all_whitespace(*line) == true
		|| ft_strncmp(*line, "//", 2) == 0 || ft_strncmp(*line, "#", 1) == 0)
	{
		free(*line);
		*line = get_next_line(fd);
		*line_count += 1;
		return (true);
	}
	return (false);
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

// ! DOCUMENT THIS LATER
bool	parse_line(t_scene *scene, char *line, size_t line_num)
{
	bool	success;
	char	**splitted;

	success = true;
	splitted = ft_split_whitespace(line);
	if (ft_strcmp(splitted[0], "A") == 0)
		success = parse_ambient(scene, splitted, line_num, line);
	else if (ft_strcmp(splitted[0], "C") == 0)
		success = parse_camera(scene, splitted, line, line_num);
	else if (ft_strcmp(splitted[0], "L") == 0)
		success = parse_light(scene, splitted, line, line_num);
	else if (is_shape(splitted[0]))
		success = parse_shape(scene, splitted, line_num, line);
	else
		success = unknown_identifier(line, line_num, scene, splitted);
	free(line);
	free_split_array(splitted);
	return (success);
}

/**
 * @brief Parses a .rt file into a scene struct
 * @param file_name Name of the .rt file
 * @return A scene struct with the shapes, lights, and camera configuration
 * specified in the file
 */
t_scene	*parse_scene(int fd)
{
	size_t	line_count;
	t_scene	*scene;
	char	*line;
	bool	success;

	line_count = 1;
	scene = ft_calloc(1, sizeof(t_scene));
	if (scene == NULL)
		return (NULL);
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (skip_line(&line, fd, &line_count) == true)
			continue ;
		success = parse_line(scene, line, line_count);
		if (success == false)
			return (NULL);
		line = get_next_line(fd);
		line_count++;
	}
	if (check_element_count(scene) == false)
		return (NULL);
	return (scene);
}

// void	parse_color
// void	parse_coordinates
// void	parse_orientation
// bool	parse_light
// void	parse_sphere
// void	parse_plane
// void	parse_cylinder
// bool	parse_shape
// bool	check_element_count
// bool	skip_line
// bool	parse_ambient
// bool	parse_camera
// bool	parse_line
// t_scene	*parse_scene