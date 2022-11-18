/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:00:17 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/18 11:20:28 by hsarhan          ###   ########.fr       */
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
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]) == false && str[i] != '-' && str[i] != '+')
			return (false);
		if ((str[i] == '-' || str[i] == '+') && i != 0)
			return (false);
		if ((str[i] == '-' || str[i] == '+') && ft_strlen(str) == 1)
			return (false);
		i++;
	}
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
	if (color_strs == NULL || is_number(color_strs[0]) == false
		|| is_number(color_strs[1]) == false ||
		is_number(color_strs[2]) == false || color_strs[3] != NULL)
	{
		*success = false;
		free_split_array(color_strs);
		return ;
	}
	res = ft_atol(color_strs[0], success);
	if (res < 0 || res > 255 || *success == false)
		*success = false;
	color->r = res;
	res = ft_atol(color_strs[1], success);
	if (res < 0 || res > 255 || *success == false)
		*success = false;
	color->g = res;
	res = ft_atol(color_strs[2], success);
	if (res < 0 || res > 255 || *success == false)
		*success = false;
	color->b = res;
}

// ! COMMENT THIS LATER
void	*camera_parse_error(char *line, size_t line_count, t_scene *scene, char **splitted, int fd)
{
	printf("Error with parsing camera on line #%ld\n%s"
		"Correct syntax is \"C x,y,z [-1.0 -> 1.0],[-1.0 -> 1.0],[-1.0 -> 1.0] [0 - 180]\"\n", line_count, line);
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
		printf("Ambient light intensity out of range on line #%ld\n%sRange is [0.0 -> 1.0]\n", line_count, line);
	else if (scene->ambient.color.r < 0 || scene->ambient.color.r > 255
		|| scene->ambient.color.g < 0 || scene->ambient.color.g > 255
		|| scene->ambient.color.b < 0 || scene->ambient.color.b > 255)
	{
		printf("Error with parsing ambient light color on line #%ld\n%s\n", line_count, line);
		if (scene->ambient.color.r < 0 || scene->ambient.color.r > 255)
			printf("The red value is out of range\n");
		if (scene->ambient.color.g < 0 || scene->ambient.color.g > 255)
			printf("The green value is out of range\n");
		if (scene->ambient.color.b < 0 || scene->ambient.color.b > 255)
			printf("The blue value is out of range\n");
	}
	else
		printf("Error with parsing ambient light on line #%ld\n%s"
			"Correct syntax is \"A [0.0 -> 1.0] [0 -> 255],[0 -> 255],[0 -> 255]\"\n", line_count, line);
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
	printf("Unknown identifier \"%s\" on line #%ld\n", splitted[0], line_count);
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
size_t	split_count(const char **split)
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
 * @brief Parses given given coordinates into a vector
 * @param position Pointer to a vector that will be filled with coordinates
 * @param str Raw string to be parsed
 * @param success Boolean pointer that will be set to false on error
 */
void	parse_coordinates(t_vector *position, const char *str, bool *success)
{
	float	res;
	char	**splitted;

	splitted = ft_split(str, ',');
	if (splitted == NULL || split_count(splitted) != 3)
	{
		free_split_array(splitted);
		*success = false;
		return ;
	}
	res = ft_atof(splitted[0], success);
	if (success == false)
	{
		free_split_array(splitted);
		return ;
	}
	position->x = res;
	res = ft_atof(splitted[1], success);
	if (success == false)
	{
		free_split_array(splitted);
		return ;
	}
	position->y = res;
	res = ft_atof(splitted[2], success);
	if (success == false)
	{
		free_split_array(splitted);
		return ;
	}
	position->z = res;
	free_split_array(splitted);
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
		printf("Can only read .rt files\n");
		return (NULL);
	}
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		printf("Could not open file \"%s\"\n", file_name);
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
			if (splitted[1] == NULL || splitted[2] == NULL || splitted[3] != NULL)
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
			printf("Camera\n");
			if (split_count(splitted) != 4)
				return (camera_parse_error(line, line_count, scene, splitted, fd));
			parse_coordinates(&scene->camera.position, splitted[1], &success);
			if (success == false)
				return (camera_parse_error(line, line_count, scene, splitted, fd));
			// parse_orientation(&scene->camera.orientation, splitted[2], &success);
			// if (success == false)
			// 	return (camera_parse_error(line, line_count, scene, splitted, fd));
			// parse_fov(&scene->camera.fov, splitted[3], &success);
			// if (success == false)
			// 	return (camera_parse_error(line, line_count, scene, splitted, fd));
		}
		else if (ft_strncmp(splitted[0], "L", ft_strlen(splitted[0])) == 0)
		{
			printf("Point Light\n");
		}
		else if (ft_strncmp(splitted[0], "sp", ft_strlen(splitted[0])) == 0)
		{
			printf("Sphere\n");
		}
		else if (ft_strncmp(splitted[0], "pl", ft_strlen(splitted[0])) == 0)
		{
			printf("Plane\n");
		}
		else if (ft_strncmp(splitted[0], "cy", ft_strlen(splitted[0])) == 0)
		{
			printf("Cylinder\n");
		}
		else if (ft_strncmp(splitted[0], "cu", ft_strlen(splitted[0])) == 0)
		{
			printf("Cube\n");
		}
		else
			return (unknown_identifier_error(line, line_count, scene, splitted, fd));
		printf("Original string: %s\n", line);
		free(line);
		free_split_array(splitted);
		line = get_next_line(fd);
		line_count++;
	}
	if (scene->count.ambient_count > 1 || scene->count.ambient_count == 0)
	{
		if (scene->count.ambient_count > 1)
			printf("Error: Scene contains more than one ambient light\n");
		else
			printf("Error: Scene contains no ambient lights\n");
		free(scene);
		close(fd);
		return (NULL);
	}
	close(fd);
	return (scene);
}
