/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:12:54 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/23 13:15:04 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	read_ppm_header(int fd, int *w, int *h)
{
	// char	*header;
	char	**tokens;
	char	buff[1000];
	int		bytes;

	bytes = read(fd, buff, 3);
	// if (header == NULL || ft_strcmp(header, "P6\n") != 0)
	// 	exit(!printf("ERROR READING FILE\n"));
	// free(header);
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

t_color	read_ppm_color(unsigned char *buff, int idx)
{
	t_color			color;

	// 	exit(!printf("ERROR READING FILE\n"));
	// if (read(fd, &g, 1) < 0)
	// 	exit(!printf("ERROR READING FILE\n"));
	// if (read(fd, &b, 1) < 0)
	// 	exit(!printf("ERROR READING FILE\n"));
	color.a = 0;
	color.r = buff[idx * 3] / 255.0;
	color.g = buff[idx * 3 + 1] / 255.0;
	color.b = buff[idx * 3 + 2] / 255.0;
	return (color);
}

t_color	**parse_texture(char *img_path, t_shape *shape)
{
	int		i;
	int		j;
	int		fd;
	t_color	**colors;
	unsigned char	*buff;
	int		col_idx;
	col_idx = 0;
	img_path = ft_strtrim(img_path, "\"");
	fd = open(img_path, O_RDONLY);
	free(img_path);
	if (fd == -1)
		exit(!printf(RED"ERROR OPENING TEXTURE FILE `%s`\n"RESET, img_path));
	read_ppm_header(fd, &shape->tex_width, &shape->tex_height);
	buff = malloc(shape->tex_height * shape->tex_width * 3 + 1);
	i = read(fd, buff, shape->tex_height * shape->tex_width * 3);
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
	close(fd);
	return (colors);
}
