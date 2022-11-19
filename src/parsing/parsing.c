/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:00:17 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/19 10:02:31 by hsarhan          ###   ########.fr       */
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
 * @brief Count the number of commas in a string
 * @param str Input strng
 * @return Number of commas in string
 */
static size_t	count_commas(const char *str)
{
	size_t	i;
	size_t	comma_count;

	i = 0;
	comma_count = 0;
	while (str[i] != '\0')
	{
		if (str[i] == ',')
			comma_count++;
		i++;
	}
	return (comma_count);
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
	if (color_strs == NULL || count_commas(str) != 2 || split_count(color_strs) != 3
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

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a camera
 * @param line Line where the parse error occured
 * @param line_num Number of the line where the error occured
 * @param scene Pointer to the scene struct
 * @param splitted Array to be freed
 * @param fd fd to be closed
 */
void	*camera_parse_error(char *line, size_t line_num, t_scene *scene, char **splitted, int fd)
{
	if (scene->camera.orientation.x < 0.0 || scene->camera.orientation.x > 1.0
		|| scene->camera.orientation.y < 0.0 || scene->camera.orientation.y > 1.0
		|| scene->camera.orientation.z < 0.0 || scene->camera.orientation.z > 1.0)
	{
		printf(YELLOW"Error with parsing camera orientation on line #%ld\n"RED"->\t%s\n"RESET, line_num, line);
		if (scene->camera.orientation.x < 0.0 || scene->camera.orientation.x > 1.0)
			printf(YELLOW"The x value is out of range\n"RESET);
		if (scene->camera.orientation.y < 0.0 || scene->camera.orientation.y > 1.0)
			printf(YELLOW"The y value is out of range\n"RESET);
		if (scene->camera.orientation.z < 0.0 || scene->camera.orientation.z > 1.0)
			printf(YELLOW"The z value is out of range\n"RESET);
	}
	else if (scene->camera.fov < 0 || scene->camera.fov > 180)
	{
		printf(YELLOW"Error with parsing camera fov on line #%ld\n"RED"->\t%s\n"RESET, line_num, line);
		printf(YELLOW"The fov value is out of range\n"RESET);
	}
	else
		printf(YELLOW"Error with parsing camera on line #%ld\n"RED"->\t%s"RESET
			YELLOW"Correct syntax is \"C [origin] [orientation] [fov]\"\n"RESET, line_num, line);
	free(line);
	free_scene(scene);
	free_split_array(splitted);
	get_next_line(-1);
	close(fd);
	return (NULL);
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
void	*ambient_parse_error(char *line, size_t line_num, t_scene *scene, char **splitted, int fd)
{
	if (scene->ambient.intensity < 0.0 || scene->ambient.intensity > 1.0)
		printf(YELLOW"Ambient light intensity out of range on line #%ld\n"RED"->\t%s"RESET"Range is [0.0 -> 1.0]\n", line_num, line);
	else if (scene->ambient.color.r < 0 || scene->ambient.color.r > 255
		|| scene->ambient.color.g < 0 || scene->ambient.color.g > 255
		|| scene->ambient.color.b < 0 || scene->ambient.color.b > 255)
	{
		printf(YELLOW"Error with parsing ambient light color on line #%ld\n"RED"->\t%s\n"RESET, line_num, line);
		if (scene->ambient.color.r < 0 || scene->ambient.color.r > 255)
			printf(YELLOW"The red value is out of range\n"RESET);
		if (scene->ambient.color.g < 0 || scene->ambient.color.g > 255)
			printf(YELLOW"The green value is out of range\n"RESET);
		if (scene->ambient.color.b < 0 || scene->ambient.color.b > 255)
			printf(YELLOW"The blue value is out of range\n"RESET);
	}
	else
		printf(YELLOW"Error with parsing ambient light on line #%ld\n"RED"->\t%s"
			YELLOW"Correct syntax is \"A [intensity] [color]\"\n"RESET, line_num, line);
	free(line);
	free_scene(scene);
	get_next_line(-1);
	free_split_array(splitted);
	close(fd);
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
void	*unknown_identifier_error(char *line, size_t line_num, t_scene *scene, char **splitted, int fd)
{
	printf(YELLOW"Unknown identifier \"%s\" on line #%ld\n"RED"->\t%s"RESET, splitted[0], line_num, line);
	free(line);
	free_scene(scene);
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
	if (splitted == NULL || count_commas(str) != 2 || split_count(splitted) != 3
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
	if (*success == false)
	{
		free_split_array(splitted);
		return ;
	}
	res = ft_atof(splitted[1], success);
	position->y = res;
	if (*success == false)
	{
		free_split_array(splitted);
		return ;
	}
	res = ft_atof(splitted[2], success);
	position->z = res;
	if (*success == false)
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
	if (splitted == NULL || count_commas(str) != 2 || split_count(splitted) != 3)
	{
		*success = false;
		free_split_array(splitted);
		return ;
	}
	// this can be in a loop
	res = ft_atof(splitted[0], success);
	orientation->x = res;
	if (*success == false || res < -1.0 || res > 1.0)
	{
		*success = false;
		free_split_array(splitted);
		return ;
	}
	res = ft_atof(splitted[1], success);
	orientation->y = res;
	if (*success == false || res < -1.0 || res > 1.0)
	{
		*success = false;
		free_split_array(splitted);
		return ;
	}
	res = ft_atof(splitted[2], success);
	orientation->z = res;
	if (*success == false || res < -1.0 || res > 1.0)
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
	size_t	i;

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
	i = 0;
	while (i < scene->count.light_count)
	{
		printf("Light #%lu configuration:\n", i + 1);
		printf("  Position:\n");
		print_vector(&scene->lights[i].position);
		printf("  Light intensity:\n");
		printf("\t%.2f intenseness\n", scene->lights[i].intensity);
		printf("  Light color:\n");
		print_color(&scene->lights[i].color);
		i++;
	}
	i = 0;
	while (i < scene->count.shape_count)
	{
		printf("Shape #%lu configuration:\n", i + 1);
		printf("  Type:\n");
		if (scene->shapes[i].type == SPHERE)
			printf("\tSphere\n");
		else if (scene->shapes[i].type == PLANE)
			printf("\tPlane\n");
		else if (scene->shapes[i].type == CYLINDER)
			printf("\tCylinder\n");
		printf("  Position:\n");
		print_vector(&scene->shapes[i].origin);
		if (scene->shapes[i].type == SPHERE || scene->shapes[i].type == CYLINDER)
		{
			printf("  Radius:\n");
			printf("\t%.2f\n", scene->shapes[i].radius);
		}
		if (scene->shapes[i].type == CYLINDER)
		{
			printf("  Height:\n");
			printf("\t%.2f\n", scene->shapes[i].height);
		}
		if (scene->shapes[i].type == CYLINDER || scene->shapes[i].type == PLANE)
		{
			printf("  Orientation:\n");
			print_vector(&scene->shapes[i].orientation);
		}
		printf("  Color:\n");
		print_color(&scene->shapes[i].color);
		i++;
	}
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
void	*shape_parse_error(char *line, size_t line_count, t_scene *scene, char **splitted, int fd)
{
	t_shape	*shape;

	if (scene->shapes == NULL)
	{
		printf(YELLOW"Error with parsing shape on line #%ld\n"RED"->\t%s"RESET, line_count, line);
		free(line);
		free_scene(scene);
		free_split_array(splitted);
		get_next_line(-1);
		close(fd);
		return (NULL);
	}
	shape = &scene->shapes[scene->count.shape_count];
	if (scene->count.shape_count >= SHAPE_MAX)
	{
		printf(RED"Error: Scene contains more than %d shapes\n"RESET, SHAPE_MAX);
	}
	else if (shape->type == SPHERE)
	{
		if (shape->radius <= 0)
		{
			printf(YELLOW"Error with sphere diameter on line #%ld\n"RED"->\t%s"RESET, line_count, line);
			printf(YELLOW"Diameter has to be a positive number\n"RESET);
		}
		else if (shape->color.r < 0 || shape->color.r > 255
				|| shape->color.g < 0 || shape->color.g > 255
				|| shape->color.b < 0 || shape->color.b > 255)
		{
			printf(YELLOW"Error with parsing sphere color on line #%ld\n"RED"->\t%s\n"RESET, line_count, line);
			if (shape->color.r < 0 || shape->color.r > 255)
				printf(YELLOW"The red value is out of range\n"RESET);
			if (shape->color.g < 0 || shape->color.g > 255)
				printf(YELLOW"The green value is out of range\n"RESET);
			if (shape->color.b < 0 || shape->color.b > 255)
				printf(YELLOW"The blue value is out of range\n"RESET);
		}
		else
			printf(YELLOW"Error with parsing sphere on line #%ld\n"RED"->\t%s"RESET
				YELLOW"Correct syntax is \"sp [origin] [diameter] [color]\"\n"RESET, line_count, line);
	}
	else if (shape->type == PLANE)
	{
		if (shape->orientation.x < 0.0 || shape->orientation.x > 1.0
			|| shape->orientation.y < 0.0 || shape->orientation.y > 1.0
			|| shape->orientation.z < 0.0 || shape->orientation.z > 1.0)
		{
			printf(YELLOW"Error with parsing plane orientation on line #%ld\n"RED"->\t%s\n"RESET, line_count, line);
			if (scene->camera.orientation.x < 0.0 || scene->camera.orientation.x > 1.0)
				printf(YELLOW"The x value is out of range\n"RESET);
			if (scene->camera.orientation.y < 0.0 || scene->camera.orientation.y > 1.0)
				printf(YELLOW"The y value is out of range\n"RESET);
			if (scene->camera.orientation.z < 0.0 || scene->camera.orientation.z > 1.0)
				printf(YELLOW"The z value is out of range\n"RESET);
		}
		else if (shape->color.r < 0 || shape->color.r > 255
			|| shape->color.g < 0 || shape->color.g > 255
			|| shape->color.b < 0 || shape->color.b > 255)
		{
			printf(YELLOW"Error with parsing plane color on line #%ld\n"RED"->\t%s\n"RESET, line_count, line);
			if (shape->color.r < 0 || shape->color.r > 255)
				printf(YELLOW"The red value is out of range\n"RESET);
			if (shape->color.g < 0 || shape->color.g > 255)
				printf(YELLOW"The green value is out of range\n"RESET);
			if (shape->color.b < 0 || shape->color.b > 255)
				printf(YELLOW"The blue value is out of range\n"RESET);
		}
		else
			printf(YELLOW"Error with parsing plane on line #%ld\n"RED"->\t%s"RESET
				YELLOW"Correct syntax is \"pl [origin] [orientation] [color]\"\n"RESET, line_count, line);
	}
	else if (shape->type == CYLINDER)
	{
		if (shape->orientation.x < 0.0 || shape->orientation.x > 1.0
			|| shape->orientation.y < 0.0 || shape->orientation.y > 1.0
			|| shape->orientation.z < 0.0 || shape->orientation.z > 1.0)
		{
			printf(YELLOW"Error with parsing cylinder orientation on line #%ld\n"RED"->\t%s\n"RESET, line_count, line);
			if (scene->camera.orientation.x < 0.0 || scene->camera.orientation.x > 1.0)
				printf(YELLOW"The x value is out of range\n"RESET);
			if (scene->camera.orientation.y < 0.0 || scene->camera.orientation.y > 1.0)
				printf(YELLOW"The y value is out of range\n"RESET);
			if (scene->camera.orientation.z < 0.0 || scene->camera.orientation.z > 1.0)
				printf(YELLOW"The z value is out of range\n"RESET);
		}
		else if (shape->radius <= 0)
		{
			printf(YELLOW"Error with cylinder diameter on line #%ld\n"RED"->\t%s"RESET, line_count, line);
			printf(YELLOW"Diameter has to be a positive number\n"RESET);
		}
		else if (shape->height <= 0)
		{
			printf(YELLOW"Error with cylinder height on line #%ld\n"RED"->\t%s"RESET, line_count, line);
			printf(YELLOW"Height has to be a positive number\n"RESET);
		}
		else if (shape->color.r < 0 || shape->color.r > 255
			|| shape->color.g < 0 || shape->color.g > 255
			|| shape->color.b < 0 || shape->color.b > 255)
		{
			printf(YELLOW"Error with parsing cylinder color on line #%ld\n"RED"->\t%s\n"RESET, line_count, line);
			if (shape->color.r < 0 || shape->color.r > 255)
				printf(YELLOW"The red value is out of range\n"RESET);
			if (shape->color.g < 0 || shape->color.g > 255)
				printf(YELLOW"The green value is out of range\n"RESET);
			if (shape->color.b < 0 || shape->color.b > 255)
				printf(YELLOW"The blue value is out of range\n"RESET);
		}
		else
			printf(YELLOW"Error with parsing cylinder on line #%ld\n"RED"->\t%s"RESET
				YELLOW"Correct syntax is \"cy [origin] [orientation] [diameter] [height] [color]\"\n"RESET, line_count, line);
	}
	free(line);
	free_scene(scene);
	free_split_array(splitted);
	get_next_line(-1);
	close(fd);
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
void	*light_parse_error(char *line, size_t line_count, t_scene *scene, char **splitted, int fd)
{
	t_light	*light;

	if (scene->lights == NULL)
	{
		printf(YELLOW"Error with parsing light on line #%ld\n"RED"->\t%s"RESET
			YELLOW"Correct syntax is \"L [origin] [intensity] [color\"\n"RESET, line_count, line);
		free(line);
		free_scene(scene);
		free_split_array(splitted);
		get_next_line(-1);
		close(fd);
		return (NULL);
	}
	light = &scene->lights[scene->count.light_count];
	if (scene->count.light_count >= LIGHT_MAX)
	{
		printf(RED"Error: Scene contains more than %d lights\n"RESET, LIGHT_MAX);
	}
	else if (light->intensity < 0.0 || light->intensity > 1.0)
	{
		printf(YELLOW"Error with parsing light intensity on line #%ld\n"RED"->\t%s\n"RESET, line_count, line);
		printf(YELLOW"The intensity value is out of range\n"RESET);
	}
	else if (light->color.r < 0 || light->color.r > 255
		|| light->color.g < 0 || light->color.g > 255
		|| light->color.b < 0 || light->color.b > 255)
	{
		printf(YELLOW"Error with parsing light color on line #%ld\n"RED"->\t%s\n"RESET, line_count, line);
		if (light->color.r < 0 || light->color.r > 255)
			printf(YELLOW"The red value is out of range\n"RESET);
		if (light->color.g < 0 || light->color.g > 255)
			printf(YELLOW"The green value is out of range\n"RESET);
		if (light->color.b < 0 || light->color.b > 255)
			printf(YELLOW"The blue value is out of range\n"RESET);
	}
	else
		printf(YELLOW"Error with parsing light on line #%ld\n"RED"->\t%s"RESET
			YELLOW"Correct syntax is \"L [origin] [intensity] [color\"\n"RESET, line_count, line);
	free(line);
	free_scene(scene);
	free_split_array(splitted);
	get_next_line(-1);
	close(fd);
	return (NULL);
}

/**
 * @brief Frees a scene struct
 * @param scene Pointer to scene struct to be freed
 */
void	free_scene(t_scene *scene)
{
	if (scene->lights)
		free(scene->lights);
	if (scene->shapes)
		free(scene->shapes);
	free(scene);
}

/**
 * @brief Parses a point light
 * @param scene Pointer to scene struct
 * @param splitted The line containing the point light configuration split into tokens
 * @param success Boolean pointer that is set to false on error
 */
void	parse_light(t_scene *scene, char **splitted, bool *success)
{
	t_light	*light;

	if (scene->count.light_count == LIGHT_MAX)
	{
		*success = false;
		return ;
	}
	if (split_count(splitted) != 4)
	{
		*success = false;
		return ;
	}
	if (scene->lights == NULL)
	{
		scene->lights = ft_calloc(LIGHT_MAX, sizeof(t_light));
		if (scene->lights == NULL)
		{
			*success = false;
			return ;
		}
	}
	light = &scene->lights[scene->count.light_count];
	parse_coordinates(&light->position, splitted[1], success);
	if (*success == false)
	{
		return ;
	}
	light->intensity = ft_atof(splitted[2], success);
	if (*success == false || light->intensity < 0.0 || light->intensity > 1.0)
	{
		*success = false;
		return ;
	}
	parse_color(&light->color, splitted[3], success);
	if (*success == false)
	{
		return ;
	}
}

/**
 * @brief Returns whether an identifier is shape
 * @param identifier 
 * @return True if the identifier is a shape
 */
static bool	is_shape(const char *identifier)
{
	return (ft_strncmp(identifier, "sp", ft_strlen(identifier)) == 0
			|| ft_strncmp(identifier, "pl", ft_strlen(identifier)) == 0
			|| ft_strncmp(identifier, "cy", ft_strlen(identifier)) == 0
			|| ft_strncmp(identifier, "cu", ft_strlen(identifier)) == 0);
}

/**
 * @brief Parses a shape
 * @param scene Pointer to scene struct
 * @param splitted The line containing the shape configurations split into tokens
 * @param success Boolean pointer that is set to false on error
 */
void	parse_shape(t_scene *scene, char **splitted, bool *success)
{
	t_shape	*shape;

	if (scene->count.shape_count == SHAPE_MAX)
	{
		*success = false;
		return ;
	}
	if (scene->shapes == NULL)
	{
		scene->shapes = ft_calloc(SHAPE_MAX, sizeof(t_shape));
		if (scene->shapes == NULL)
		{
			*success = false;
			return ;
		}
	}
	shape = &scene->shapes[scene->count.shape_count];
	if (ft_strncmp(splitted[0], "sp", ft_strlen(splitted[0])) == 0)
	{
		shape->type = SPHERE;
		if (split_count(splitted) != 4)
		{
			*success = false;
			return ;
		}
		shape->radius = ft_atof(splitted[2], success) / 2;
		if (*success == false || shape->radius < 0.0 || shape->radius == 0.0)
		{
			*success = false;
			return ;
		}
		parse_coordinates(&shape->origin, splitted[1], success);
		if (*success == false)
		{
			return ;
		}
		parse_color(&shape->color, splitted[3], success);
		if (*success == false)
		{
			return ;
		}
	}
	else if (ft_strncmp(splitted[0], "pl", ft_strlen(splitted[0])) == 0)
	{
		shape->type = PLANE;
		if (split_count(splitted) != 4)
		{
			*success = false;
			return ;
		}
		parse_coordinates(&shape->origin, splitted[1], success);
		if (*success == false)
		{
			return ;
		}
		parse_orientation(&shape->orientation, splitted[2], success);
		if (*success == false)
		{
			return ;
		}
		parse_color(&shape->color, splitted[3], success);
		if (*success == false)
		{
			return ;
		}
	}
	else if (ft_strncmp(splitted[0], "cy", ft_strlen(splitted[0])) == 0)
	{
		shape->type = CYLINDER;
		if (split_count(splitted) != 6)
		{
			*success = false;
			return ;
		}
		shape->radius = ft_atof(splitted[3], success) / 2;
		if (*success == false || shape->radius <= 0.0)
		{
			*success = false;
			return ;
		}
		shape->height = ft_atof(splitted[4], success);
		if (*success == false || shape->height <= 0.0)
		{
			*success = false;
			return ;
		}
		parse_coordinates(&shape->origin, splitted[1], success);
		if (*success == false)
		{
			return ;
		}
		parse_orientation(&shape->orientation, splitted[2], success);
		if (*success == false)
		{
			return ;
		}
		parse_color(&shape->color, splitted[5], success);
		if (*success == false)
		{
			return ;
		}
	}
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
			if (split_count(splitted) != 3)
				return (ambient_parse_error(line, line_count, scene, splitted, fd));
			scene->ambient.intensity = ft_atof(splitted[1], &success);
			if (success == false || scene->ambient.intensity < 0.0 || scene->ambient.intensity > 1.0)
				return (ambient_parse_error(line, line_count, scene, splitted, fd));
			parse_color(&scene->ambient.color, splitted[2], &success);
			if (success == false)
				return (ambient_parse_error(line, line_count, scene, splitted, fd));
			scene->count.ambient_count++;
		}
		else if (ft_strncmp(splitted[0], "C", ft_strlen(splitted[0])) == 0)
		{
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
			// if (split_count(splitted) != 4)
			// 	return (camera_parse_error(line, line_count, scene, splitted, fd));
			parse_light(scene, splitted, &success);
			if (success == false)
				return (light_parse_error(line, line_count, scene, splitted, fd));
			scene->count.light_count++;
		}
		else if (is_shape(splitted[0]))
		{
			parse_shape(scene, splitted, &success);
			if (success == false)
				return (shape_parse_error(line, line_count, scene, splitted, fd));
			scene->count.shape_count++;
		}
		else
			return (unknown_identifier_error(line, line_count, scene, splitted, fd));
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
		free_scene(scene);
		close(fd);
		return (NULL);
	}
	if (scene->count.camera_count > 1 || scene->count.camera_count == 0)
	{
		if (scene->count.camera_count > 1)
			printf(RED"Error: Scene contains more than one camera\n"RESET);
		else
			printf(RED"Error: Scene contains no cameras\n"RESET);
		free_scene(scene);
		close(fd);
		return (NULL);
	}
	close(fd);
	print_scene(scene);
	free_scene(scene);
	return (scene);
}
