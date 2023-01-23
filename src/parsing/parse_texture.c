/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:12:54 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/23 16:44:58 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	read_ppm_header(int fd, int *w, int *h)
{
	char	**tokens;
	char	buff[1001];
	int		bytes;
	int		i;
	char	ch;

	bytes = read(fd, buff, 3);
	buff[3] = '\0';
	if (bytes <= 0 || ft_strcmp(buff, "P6\n") != 0)
	{
		exit(!printf("1ERROR READING FILE and not freeing what I am supposed to free\n"));
	}
	i = 0;
	bytes = read(fd, &ch, 1);
	while (bytes > 0 && ch != '\n' && i < 1000)
	{
		buff[i] = ch;
		bytes = read(fd, &ch, 1);
		i++;
	}
	if (i == 1000 || bytes == 0)
	{
		exit(!printf("ERROR READING FILE and not freeing what I am supposed to free\n"));
	}
	buff[i] = '\0';
	tokens = ft_split(buff, ' ');
	if (tokens == NULL)
	{
		exit(!printf("ERROR READING FILE and not freeing what I am supposed to free\n"));
	}
	if (split_count(tokens) != 2)
	{
		exit(!printf("ERROR READING FILE and not freeing what I am supposed to free\n"));
	}
	*w = ft_atoi(tokens[0]);
	*h = ft_atoi(tokens[1]);
	if (*w <= 0 || *h <= 0)
	{
		exit(!printf("ERROR READING FILE and not freeing what I am supposed to free\n"));
	}
	free_split_array(tokens);
	bytes = read(fd, buff, 4);
	if (bytes <= 0)
	{
		exit(!printf("ERROR READING FILE and not freeing what I am supposed to free\n"));
	}
}

t_color	read_ppm_color(unsigned char *buff, int idx)
{
	t_color			color;

	color.a = 0;
	color.r = buff[idx * 3] / 255.0;
	color.g = buff[idx * 3 + 1] / 255.0;
	color.b = buff[idx * 3 + 2] / 255.0;
	return (color);
}

t_color	**parse_texture(char *img_path, t_shape *shape)
{
	int				i;
	int				j;
	int				fd;
	t_color			**colors;
	unsigned char	*buff;
	int				col_idx;

	col_idx = 0;
	img_path = ft_strtrim(img_path, "\"");
	fd = open(img_path, O_RDONLY);
	free(img_path);
	if (fd == -1)
		exit(!printf(RED"ERROR OPENING TEXTURE FILE `%s`\n"RESET, img_path));
	read_ppm_header(fd, &shape->tex_width, &shape->tex_height);
	buff = malloc(shape->tex_height * shape->tex_width * 3 + 1);
	i = read(fd, buff, shape->tex_height * shape->tex_width * 3);
	close(fd);
	buff[i] = '\0';
	colors = malloc((shape->tex_height + 1) * sizeof(t_color *));
	colors[shape->tex_height] = NULL;
	i = 0;
	while (i < shape->tex_height)
		colors[i++] = malloc((shape->tex_width + 1) * sizeof(t_color));
	i = 0;
	while (i < shape->tex_height)
	{
		j = 0;
		while (j < shape->tex_width)
		{
			colors[i][j++] = read_ppm_color(buff, col_idx);
			col_idx++;
		}
		i++;
	}
	return (colors);
}
