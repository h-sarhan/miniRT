/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:00:17 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/17 18:11:17 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

static bool	is_whitespace(const char c)
{
	return (c == ' ' || c == '\f'
			|| c == '\n' || c == '\r'
			|| c == '\t' || c == '\v');
}

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

static bool	all_digits(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]) == false)
			return (false);
		i++;
	}
	return (true);
}

void	free_split_array(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	parse_color(t_color *color, const char *str, bool *success)
{
	char	**color_strs;
	long	res;

	color_strs = ft_split(str, ',');
	if (color_strs[0] == NULL || color_strs[1] == NULL || color_strs[2] == NULL || color_strs[3] != NULL)
	{
		*success = false;
		free_split_array(color_strs);
		return ;
	}
	if (all_digits(color_strs[0]) == false || all_digits(color_strs[1]) == false || all_digits(color_strs[2]) == false)
	{
		*success = false;
		free_split_array(color_strs);
		return ;
	}
	res = ft_atol(color_strs[0], success);
	if (res < 0 || res > 255 || *success == false)
	{
		*success = false;
		return ;
	}
	color->r = res;
	res = ft_atol(color_strs[1], success);
	if (res < 0 || res > 255 || *success == false)
	{
		*success = false;
		return ;
	}
	color->g = res;
	res = ft_atol(color_strs[2], success);
	if (res < 0 || res > 255 || *success == false)
	{
		*success = false;
		return ;
	}
	color->b = res;
}

void	*ambient_parse_error(char *line, size_t line_count, t_scene *scene, char **splitted, int fd)
{
	printf("Error with parsing ambient light on line #%ld\n%s"
		"Correct syntax is \"A [0.0 -> 1.0] [0 -> 255],[0 -> 255],[0 -> 255]\"\n", line_count, line);
	free(line);
	free(scene);
	free_split_array(splitted);
	close(fd);
	return (NULL);
}

void	*ambient_parse_error2(char *line, size_t line_count, t_scene *scene, char **splitted, int fd)
{
	if (scene->ambient.intensity < 0.0 || scene->ambient.intensity > 1.0)
		printf("Ambient light intensity out of range on line #%ld\n%sRange is [0.0 -> 1.0]\n", line_count, line);
	else
		printf("Error with parsing ambient light on line #%ld\n%s"
			"Correct syntax is \"A [0.0 -> 1.0] [0 -> 255],[0 -> 255],[0 -> 255]\"\n", line_count, line);
	free(line);
	free(scene);
	free_split_array(splitted);
	close(fd);
	return (NULL);
}

void	*unknwn_identifier_error(char *line, size_t line_count, t_scene *scene, char **splitted, int fd)
{
	printf("Unknown identifier \"%s\" on line #%ld\n", splitted[0], line_count);
	free(line);
	free(scene);
	free_split_array(splitted);
	close(fd);
	return (NULL);
}


t_scene	*parse_map(const char *file_name)
{
	size_t	line_count;
	t_scene	*scene;
	int		fd;
	char	**splitted;
	char	*line;
	bool	success;

	line_count = 1;
	success = true;
	scene = ft_calloc(1, sizeof(t_scene));
	if (scene == NULL)
		return (NULL);
	fd = open(file_name, O_RDONLY);
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
		if (ft_strncmp(splitted[0], "A", 1) == 0)
		{
			printf("Ambient light\n");
			if (splitted[1] == NULL || splitted[2] == NULL || splitted[3] != NULL)
				return (ambient_parse_error(line, line_count, scene, splitted, fd));
			scene->ambient.intensity = ft_atof(splitted[1], &success);
			if (success == false || scene->ambient.intensity < 0.0 || scene->ambient.intensity > 1.0)
				return (ambient_parse_error2(line, line_count, scene, splitted, fd));
			printf("Intensity: %.2f\n", scene->ambient.intensity);
			parse_color(&scene->ambient.color, splitted[2], &success);
			if (success == false)
				return (ambient_parse_error(line, line_count, scene, splitted, fd));
			print_color(&scene->ambient.color);
		}
		else if (ft_strncmp(splitted[0], "C", 1) == 0)
		{
			printf("Camera\n");
		}
		else if (ft_strncmp(splitted[0], "L", 1) == 0)
		{
			printf("Point Light\n");
		}
		else if (ft_strncmp(splitted[0], "sp", 2) == 0)
		{
			printf("Sphere\n");
		}
		else if (ft_strncmp(splitted[0], "pl", 2) == 0)
		{
			printf("Plane\n");
		}
		else if (ft_strncmp(splitted[0], "cy", 2) == 0)
		{
			printf("Cylinder\n");
		}
		else if (ft_strncmp(splitted[0], "cu", 2) == 0)
		{
			printf("Cube\n");
		}
		else
			return (unknwn_identifier_error(line, line_count, scene, splitted, fd));
		printf("Original string: %s\n", line);
		free(line);
		free_split_array(splitted);
		line = get_next_line(fd);
		line_count++;
	}
	close(fd);
	return (scene);
}
