/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:00:17 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/18 14:11:12 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

/**
 * @brief Returns whether a character is whitespace
 * @param c Input character
 * @return True if whitespace
 */
static bool	is_whitespace(const char c)
{
	return (c == ' ' || c == '\f'
			|| c == '\n' || c == '\r'
			|| c == '\t' || c == '\v');
}

/**
 * @brief Returns whether a string is entirely made up of whitespace characters
 * @param str Input string
 * @return True if every character is whitespace
 */
static bool	all_whitespace(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (is_whitespace(str[i]) == false)
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief Returns whether a string is a number
 * @param str Input string
 * @return True if every character is a digit
 */
static bool	is_number(const char *str)
{
	size_t	i;
	size_t	dot_count;

	i = 0;
	dot_count = 0;
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]) == false && str[i] != '-' && str[i] != '+' && str[i] != '.')
			return (false);
		if (str[i] == '.')
			dot_count++;
		if ((str[i] == '-' || str[i] == '+') && i != 0)
			return (false);
		i++;
	}
	if (dot_count > 1)
		return (false);
	if ((str[0] == '-' || str[0] == '+') && ft_strlen(str) == 1)
		return (false);
	if ((str[0] == '-' || str[0] == '+') && str[1] == '.' && ft_strlen(str) == 2)
		return (false);
	return (true);
}

/**
 * @brief Frees an array allocated by split
 * @param arr Array to free
 */
void	free_split_array(char **arr)
{
	size_t	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/**
 * @brief Parse the red, green, and blue values of a color
 * @param color Color struct to be filled with RGB values
 * @param str Raw string in the form
 * @param success Boolean pointer that is set to false on error
 */
void	parse_color(t_color *color, const char *str, bool *success)
{
	char	**color_strs;
	long	res;

	color_strs = ft_split(str, ',');
	if (color_strs == NULL || split_count(color_strs) != 3
		|| is_number(color_strs[0]) == false
		|| is_number(color_strs[1]) == false
		|| is_number(color_strs[2]) == false)
	{
		*success = false;
		free_split_array(color_strs);
		return ;
	}
	// This can be in a loop
	res = ft_atol(color_strs[0], success);
	color->r = res;
	if (res < 0 || res > 255 || *success == false)
	{
		*success = false;
		return ;
	}
	res = ft_atol(color_strs[1], success);
	color->g = res;
	if (res < 0 || res > 255 || *success == false)
	{
		*success = false;
		return ;
	}
	res = ft_atol(color_strs[2], success);
	color->b = res;
	if (res < 0 || res > 255 || *success == false)
	{
		*success = false;
		return ;
	}
}

// ! COMMENT THIS LATER
void	*camera_parse_error(char *line, size_t line_count, t_scene *scene, char **splitted, int fd)
{
	if (scene->camera.orientation.x < 0.0 || scene->camera.orientation.x > 1.0
		|| scene->camera.orientation.y < 0.0 || scene->camera.orientation.y > 1.0
		|| scene->camera.orientation.z < 0.0 || scene->camera.orientation.z > 1.0)
	{
		printf(YELLOW"Error with parsing camera orientation on line #%ld\n"RED"->\t%s\n"RESET, line_count, line);
		if (scene->camera.orientation.x < 0.0 || scene->camera.orientation.x > 1.0)
			printf(YELLOW"The x value is out of range\n"RESET);
		if (scene->camera.orientation.y < 0.0 || scene->camera.orientation.y > 1.0)
			printf(YELLOW"The y value is out of range\n"RESET);
		if (scene->camera.orientation.z < 0.0 || scene->camera.orientation.z > 1.0)
			printf(YELLOW"The z value is out of range\n"RESET);
	}
	else if (scene->camera.fov < 0 || scene->camera.fov > 180)
	{
		printf(YELLOW"Error with parsing camera fov on line #%ld\n"RED"->\t%s\n"RESET, line_count, line);
		printf(YELLOW"The fov value is out of range\n"RESET);
	}
	else
		printf(YELLOW"Error with parsing camera on line #%ld\n"RED"->\t%s"RESET
			YELLOW"Correct syntax is \"C x,y,z [-1.0 -> 1.0],[-1.0 -> 1.0],[-1.0 -> 1.0] [0 - 180]\"\n"RESET, line_count, line);
	free(line);
	free(scene);
	free_split_array(splitted);
	get_next_line(-1);
	close(fd);
	return (NULL);
}

// ! COMMENT THIS LATER
void	*ambient_parse_error(char *line, size_t line_count, t_scene *scene, char **splitted, int fd)
{
	if (scene->ambient.intensity < 0.0 || scene->ambient.intensity > 1.0)
		printf(YELLOW"Ambient light intensity out of range on line #%ld\n"RED"->\t%s"RESET"Range is [0.0 -> 1.0]\n", line_count, line);
	else if (scene->ambient.color.r < 0 || scene->ambient.color.r > 255
		|| scene->ambient.color.g < 0 || scene->ambient.color.g > 255
		|| scene->ambient.color.b < 0 || scene->ambient.color.b > 255)
	{
		printf(YELLOW"Error with parsing ambient light color on line #%ld\n"RED"->\t%s\n"RESET, line_count, line);
		if (scene->ambient.color.r < 0 || scene->ambient.color.r > 255)
			printf(YELLOW"The red value is out of range\n"RESET);
		if (scene->ambient.color.g < 0 || scene->ambient.color.g > 255)
			printf(YELLOW"The green value is out of range\n"RESET);
		if (scene->ambient.color.b < 0 || scene->ambient.color.b > 255)
			printf(YELLOW"The blue value is out of range\n"RESET);
	}
	else
		printf(YELLOW"Error with parsing ambient light on line #%ld\n"RED"->\t%s"
			YELLOW"Correct syntax is \"A [0.0 -> 1.0] [0 -> 255],[0 -> 255],[0 -> 255]\"\n"RESET, line_count, line);
	free(line);
	free(scene);
	get_next_line(-1);
	free_split_array(splitted);
	close(fd);
	return (NULL);
}

// ! COMMENT THIS LATER
void	*unknown_identifier_error(char *line, size_t line_count, t_scene *scene, char **splitted, int fd)
{
	printf(YELLOW"Unknown identifier \"%s\" on line #%ld\n"RED"->\t%s"RESET, splitted[0], line_count, line);
	free(line);
	free(scene);
	get_next_line(-1);
	free_split_array(splitted);
	close(fd);
	return (NULL);
}

/**
 * @brief Counts the number of elements in an array returned by ft_split
 * @param split An array returned by ft_split
 * @return Number of elements in the array
 */
size_t	split_count(char **split)
{
	size_t	len;
	
	if (split == NULL)
		return (0);
	len = 0;
	while (split[len] != NULL)
		len++;
	return (len);
}

/**
 * @brief Parses given coordinates into a vector
 * @param position Pointer to a vector that will be filled with coordinates
 * @param str Raw string to be parsed
 * @param success Boolean pointer that will be set to false on error
 */
void	parse_coordinates(t_vector *position, const char *str, bool *success)
{
	float	res;
	char	**splitted;

	splitted = ft_split(str, ',');
	if (splitted == NULL || split_count(splitted) != 3
		|| is_number(splitted[0]) == false || is_number(splitted[1]) == false
		|| is_number(splitted[2]) == false)
	{
		free_split_array(splitted);
		*success = false;
		return ;
	}
	// this can be in a loop
	res = ft_atof(splitted[0], success);
	position->x = res;
	if (success == false)
	{
		free_split_array(splitted);
		return ;
	}
	res = ft_atof(splitted[1], success);
	position->y = res;
	if (success == false)
	{
		free_split_array(splitted);
		return ;
	}
	res = ft_atof(splitted[2], success);
	position->z = res;
	if (success == false)
	{
		free_split_array(splitted);
		return ;
	}
	free_split_array(splitted);
}

/**
 * @brief Parses given orientation into a vector
 * @param position Pointer to a vector that will be filled with orientation
 * @param str Raw string to be parsed
 * @param success Boolean pointer that will be set to false on error
 */
void	parse_orientation(t_vector *orientation, const char *str, bool *success)
{
	float	res;
	char	**splitted;

	splitted = ft_split(str, ',');
	if (splitted == NULL || split_count(splitted) != 3)
	{
		*success = false;
		free_split_array(splitted);
		return ;
	}
	// this can be in a loop
	res = ft_atof(splitted[0], success);
	orientation->x = res;
	if (success == false || res < -1.0 || res > 1.0)
	{
		*success = false;
		free_split_array(splitted);
		return ;
	}
	res = ft_atof(splitted[1], success);
	orientation->y = res;
	if (success == false || res < -1.0 || res > 1.0)
	{
		*success = false;
		free_split_array(splitted);
		return ;
	}
	res = ft_atof(splitted[2], success);
	orientation->z = res;
	if (success == false || res < -1.0 || res > 1.0)
	{
		*success = false;
		free_split_array(splitted);
		return ;
	}
	free_split_array(splitted);
}

/**
 * @brief Prints the x, y, z values of a vector
 * @param vector Vector to be printed
 */
void	print_vector(const t_vector *vector)
{
	printf("\tX: %.2f\n", vector->x);
	printf("\tY: %.2f\n", vector->y);
	printf("\tZ: %.2f\n", vector->z);
}

/**
 * @brief Prints the contents of a scene
 * @param scene Scene to be printed
 */
void	print_scene(const t_scene *scene)
{
	printf("Ambient light configuration:\n");
	printf("  Intensity: %.2f\n", scene->ambient.intensity);
	printf("  Color:\n");
	print_color(&scene->ambient.color);
	printf("Camera configuration:\n");
	printf("  Position:\n");
	print_vector(&scene->camera.position);
	printf("  Orientation:\n");
	print_vector(&scene->camera.orientation);
	printf("  Fov:\n");
	printf("\t%d degrees\n", scene->camera.fov);
}


/**
 * @brief Parses a .rt file into a scene struct
 * @param file_name Name of the .rt file
 * @return A scene struct with the shapes, lights, and camera configuration
 * specified in the file
 */
t_scene	*parse_scene(const char *file_name)
{
	size_t	line_count;
	t_scene	*scene;
	int		fd;
	char	**splitted;
	char	*line;
	bool	success;

	line_count = 1;
	success = true;
	if (ft_strnstr(file_name, ".rt", ft_strlen(file_name)) == NULL
		|| ft_strncmp(&file_name[ft_strlen(file_name) - 3], ".rt", 3) != 0)
	{
		printf(RED"Can only read .rt files\n"RESET);
		return (NULL);
	}
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		printf(RED"Could not open file \"%s\"\n"RESET, file_name);
		return (NULL);
	}
	scene = ft_calloc(1, sizeof(t_scene));
	if (scene == NULL)
	{
		close(fd);
		return (NULL);
	}
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (*line == '\0' || all_whitespace(line) == true || ft_strncmp(line, "//", 2) == 0 || ft_strncmp(line, "#", 1) == 0)
		{
			free(line);
			line = get_next_line(fd);
			line_count++;
			continue;
		}
		splitted = ft_split_whitespace(line);
		if (ft_strncmp(splitted[0], "A", ft_strlen(splitted[0])) == 0)
		{
			// printf("Ambient light\n");
			if (split_count(splitted) != 3)
				return (ambient_parse_error(line, line_count, scene, splitted, fd));
			scene->ambient.intensity = ft_atof(splitted[1], &success);
			if (success == false || scene->ambient.intensity < 0.0 || scene->ambient.intensity > 1.0)
				return (ambient_parse_error(line, line_count, scene, splitted, fd));
			// printf("Intensity: %.2f\n", scene->ambient.intensity);
			parse_color(&scene->ambient.color, splitted[2], &success);
			if (success == false)
				return (ambient_parse_error(line, line_count, scene, splitted, fd));
			// print_color(&scene->ambient.color);
			scene->count.ambient_count++;
		}
		else if (ft_strncmp(splitted[0], "C", ft_strlen(splitted[0])) == 0)
		{
			// printf("Camera\n");
			if (split_count(splitted) != 4)
				return (camera_parse_error(line, line_count, scene, splitted, fd));
			parse_coordinates(&scene->camera.position, splitted[1], &success);
			if (success == false)
				return (camera_parse_error(line, line_count, scene, splitted, fd));
			parse_orientation(&scene->camera.orientation, splitted[2], &success);
			if (success == false)
				return (camera_parse_error(line, line_count, scene, splitted, fd));
			scene->camera.fov = ft_atol(splitted[3], &success);
			if (success == false || scene->camera.fov < 0 || scene->camera.fov > 180)
				return (camera_parse_error(line, line_count, scene, splitted, fd));
			scene->count.camera_count++;
		}
		else if (ft_strncmp(splitted[0], "L", ft_strlen(splitted[0])) == 0)
		{
			// printf("Point Light\n");
		}
		else if (ft_strncmp(splitted[0], "sp", ft_strlen(splitted[0])) == 0)
		{
			// printf("Sphere\n");
		}
		else if (ft_strncmp(splitted[0], "pl", ft_strlen(splitted[0])) == 0)
		{
			// printf("Plane\n");
		}
		else if (ft_strncmp(splitted[0], "cy", ft_strlen(splitted[0])) == 0)
		{
			// printf("Cylinder\n");
		}
		else if (ft_strncmp(splitted[0], "cu", ft_strlen(splitted[0])) == 0)
		{
			// printf("Cube\n");
		}
		else
			return (unknown_identifier_error(line, line_count, scene, splitted, fd));
		// printf("Original string: %s\n", line);
		free(line);
		free_split_array(splitted);
		line = get_next_line(fd);
		line_count++;
	}
	if (scene->count.ambient_count > 1 || scene->count.ambient_count == 0)
	{
		if (scene->count.ambient_count > 1)
			printf(RED"Error: Scene contains more than one ambient light\n"RESET);
		else
			printf(RED"Error: Scene contains no ambient lights\n"RESET);
		free(scene);
		close(fd);
		return (NULL);
	}
	if (scene->count.camera_count > 1 || scene->count.camera_count == 0)
	{
		if (scene->count.camera_count > 1)
			printf(RED"Error: Scene contains more than one camera\n"RESET);
		else
			printf(RED"Error: Scene contains no cameras\n"RESET);
		free(scene);
		close(fd);
		return (NULL);
	}
	close(fd);
	print_scene(scene);
	return (scene);
}
