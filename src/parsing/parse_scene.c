/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:00:17 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/23 15:34:54 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "miniRT.h"
#include "scene.h"

/**
 * @brief Checks if the amount of ambient lights and cameras in a struct
 * is correct
 * @param scene The scene struct to check
 * @return True if there are no issues with the element count
 */
static bool	check_element_count(t_scene *scene)
{
	if (scene->count.ambient_lights > 1 || scene->count.ambient_lights == 0)
	{
		if (scene->count.ambient_lights > 1)
			printf(RED"Error: Scene contains multiple ambient light\n"RESET);
		else
			printf(RED"Error: Scene contains no ambient lights\n"RESET);
		free_scene(scene);
		return (false);
	}
	if (scene->count.cameras > 1 || scene->count.cameras == 0)
	{
		if (scene->count.cameras > 1)
			printf(RED"Error: Scene contains more than one camera\n"RESET);
		else
			printf(RED"Error: Scene contains no cameras\n"RESET);
		free_scene(scene);
		return (false);
	}
	return (true);
}

/**
 * @brief Skips parsing a line if it is empty, only contains whitespace, or
 *  if it is a comment
 * @param line The line to skip
 * @param fd The file descriptor of the scene file
 * @param line_count A pointer to the current line number. This will be
 *  incremented if a line is skipped
 * @return True if a line is skipped
 */
bool	skip_line(char **line, int fd, size_t *line_count)
{
	if (ft_strlen(*line) == 0 || all_whitespace(*line) == true
		|| ft_strncmp(*line, "//", 2) == 0 || ft_strncmp(*line, "#", 1) == 0)
	{
		free(*line);
		*line = ft_strtrim_free(get_next_line(fd), " \t\n");
		*line_count += 1;
		return (true);
	}
	return (false);
}

/**
 * @brief Parses a single line from the scene file
 * @param scene The scene struct to fill
 * @param line The line to parse
 * @param line_num The current line number
 * @return True if the line was parsed successfully
 */
static bool	parse_line(t_scene *scene, char *line, size_t *line_num, int fd)
{
	char	**splitted;

	splitted = ft_split_whitespace(line);
	if (ft_strcmp(splitted[0], "A") == 0)
		parse_ambient(scene, splitted);
	else if (ft_strcmp(splitted[0], "C") == 0)
		parse_camera(scene, splitted);
	else if (ft_strcmp(splitted[0], "L") == 0)
		parse_light(scene, splitted);
	else if (ft_strcmp(splitted[0], "SL") == 0)
		parse_spotlight(scene, splitted);
	else if (is_shape(splitted[0]))
		parse_shape(scene, splitted);
	else if (is_settings(line) == true)
		parse_settings(scene, line, line_num, fd);
	else
		scene->error_flags.unknown_identifier = true;
	if (find_error(&scene->error_flags))
		print_error(scene, line, *line_num, splitted[0]);
	free(line);
	free_split_array(splitted);
	return (!find_error(&scene->error_flags));
}

t_scene	*check_elements(t_scene *scene)
{
	if (check_element_count(scene) == false)
		return (NULL);
	return (scene);
}

/**
 * @brief Parses a .rt file into a scene struct
 * @param fd The file descriptor of a .rt file
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
		success = parse_line(scene, line, &line_count, fd);
		if (success == false)
		{
			get_next_line(-1);
			free_scene(scene);
			return (NULL);
		}
		line = get_next_line(fd);
		line_count++;
	}
	return (check_elements(scene));
}
