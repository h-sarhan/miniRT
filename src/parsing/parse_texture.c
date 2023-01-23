/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:12:54 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/23 12:23:58 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	read_ppm_header(int fd, int *w, int *h)
{
	// char	*header;
	char	**tokens;
	char	buff[10000];
	int		bytes;

	bytes = read(fd, buff, 3);
	// if (header == NULL || ft_strcmp(header, "P6\n") != 0)
	// 	exit(!printf("ERROR READING FILE\n"));
	// free(header);
	ft_bzero(buff, 10000);
	int i = 0;
	char ch;
	read(fd, &ch, 1);
	printf("%c\n", ch);
	while (ch != '\n')
	{
		buff[i] = ch;
		read(fd, &ch, 1);
		printf("%c\n", ch);
		i++;
	}
	buff[i] = '\0';
	// if (header == NULL)
	// 	exit(!printf("ERROR READING FILE\n"));
	// buff[bytes] = '\0';
	tokens = ft_split(buff, ' ');
	// free(header);
	if (tokens == NULL)
		exit(!printf("ERROR READING FILE\n"));
	*w = ft_atoi(tokens[0]);
	*h = ft_atoi(tokens[1]);
	free_split_array(tokens);
	bytes = read(fd, buff, 4);
	// if (header == NULL)
	// 	exit(!printf("ERROR READING FILE\n"));
	// free(header);
}

t_color	read_ppm_color(int fd)
{
	t_color			color;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	r = 0;
	g = 0;
	b = 0;
	if (read(fd, &r, 1) < 0)
		exit(!printf("ERROR READING FILE\n"));
	if (read(fd, &g, 1) < 0)
		exit(!printf("ERROR READING FILE\n"));
	if (read(fd, &b, 1) < 0)
		exit(!printf("ERROR READING FILE\n"));
	color.a = 0;
	color.r = r / 255.0;
	color.g = g / 255.0;
	color.b = b / 255.0;
	return (color);
}

t_color	**scale_texture(t_color **colors, int w, int h)
{
	int			x;
	int			y;
	int			src_x;
	int			src_y;

	t_color **colors_scaled = ft_calloc(600 + 1, sizeof(t_color *));
	int i = 0;
	while (i < 600)
		colors_scaled[i++] = ft_calloc(600 + 1, sizeof(t_color));
	y = 0;
	while (y < 600)
	{
		x = 0;
		while (x < 600)
		{
			src_x = round((x / 600.0) * w);
			src_y = round((y / 600.0) * h);
			src_x = min(src_x, w - 1);
			src_y = min(src_y, h - 1);
			colors_scaled[y][x] = colors[src_y][src_x];
			x++;
		}
		y++;
	}
	return (colors_scaled);
}

t_color	**parse_texture(char *img_path, t_shape *shape)
{
	int		i;
	int		j;
	int		fd;
	t_color	**colors;

	img_path = ft_strtrim(img_path, "\"");
	fd = open(img_path, O_RDONLY);
	free(img_path);
	if (fd == -1)
		exit(!printf(RED"ERROR OPENING TEXTURE FILE `%s`\n"RESET, img_path));
	read_ppm_header(fd, &shape->tex_width, &shape->tex_height);
	colors = ft_calloc(shape->tex_height + 1, sizeof(t_color *));
	i = 0;
	while (i < shape->tex_height)
		colors[i++] = ft_calloc(shape->tex_width + 1, sizeof(t_color));
	i = 0;
	while (i < shape->tex_height)
	{
		j = 0;
		while (j < shape->tex_width)
			colors[i][j++] = read_ppm_color(fd);
		i++;
	}
	close(fd);
	// intense memory leak
	colors = scale_texture(colors, shape->tex_width, shape->tex_height);
	shape->tex_height = 600;
	shape->tex_width = 600;
	return (colors);
}
