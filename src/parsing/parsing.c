/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:00:17 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/17 16:15:07 by hsarhan          ###   ########.fr       */
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

t_scene	*parse(const char *file_name, bool *success)
{
	t_scene	*scene = ft_calloc(1, sizeof(t_scene));
	if (scene == NULL)
	{
		*success = false;
		return (NULL);
	}
	int fd = open(file_name, O_RDONLY);
	char *line = get_next_line(fd);
	while (line != NULL)
	{
		if (ft_strncmp(line, "A", 1) == 0 && is_whitespace(line[1]))
		{
			printf("Ambient light\n");
		}
		else if (ft_strncmp(line, "C", 1) == 0 && is_whitespace(line[1]))
		{
			printf("Camera\n");
		}
		else if (ft_strncmp(line, "L", 1) == 0 && is_whitespace(line[1]))
		{
			printf("Point Light\n");
		}
		else if (ft_strncmp(line, "sp", 2) == 0 && is_whitespace(line[2]))
		{
			printf("Sphere\n");
		}
		else if (ft_strncmp(line, "pl", 2) == 0 && is_whitespace(line[2]))
		{
			printf("Plane\n");
		}
		else if (ft_strncmp(line, "cy", 2) == 0 && is_whitespace(line[2]))
		{
			printf("Cylinder\n");
		}
		else if (ft_strncmp(line, "cu", 2) == 0 && is_whitespace(line[2]))
		{
			printf("Cube\n");
		}
		else if (*line != '\0' && all_whitespace(line) == false && ft_strncmp(line, "//", 2) != 0 && ft_strncmp(line, "#", 1 != 0))
		{
			*success = false;
			free(line);
			free(scene);
			return (NULL);
		}
		if (*line != '\0' && all_whitespace(line) == false && ft_strncmp(line, "//", 2) != 0 && ft_strncmp(line, "#", 1 != 0))
		{
			printf("%s\n", line);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (scene);
}
