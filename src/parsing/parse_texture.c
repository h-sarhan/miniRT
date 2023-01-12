/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:12:54 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/12 16:29:58 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	read_ppm_header(int fd, int *w, int *h)
{
	char			*header;
	char			**tokens;

	header = get_next_line(fd);
	if (header == NULL || ft_strcmp(header, "P6\n") != 0)
		exit(!printf("ERROR READING FILE\n"));
	free(header);
	header = get_next_line(fd);
	if (header == NULL)
		exit(!printf("ERROR READING FILE\n"));
	tokens = ft_split(header, ' ');
	free(header);
	if (tokens == NULL)
		exit(!printf("ERROR READING FILE\n"));
	*w = ft_atoi(tokens[0]);
	*h = ft_atoi(tokens[1]);
	free_split_array(tokens);
	header = get_next_line(fd);
	if (header == NULL)
		exit(!printf("ERROR READING FILE\n"));
	free(header);
}

t_color	read_ppm_color(int fd)
{
	t_color	color;
	int		r;
	int		g;
	int		b;

	if (read(fd, &g, 1) < 0)
		exit(!printf("ERROR READING FILE\n"));
	if (read(fd, &b, 1) < 0)
		exit(!printf("ERROR READING FILE\n"));
	if (read(fd, &r, 1) < 0)
		exit(!printf("ERROR READING FILE\n"));
	color.a = 0;
	color.r = r / 255.0;
	color.g = g / 255.0;
	color.b = b / 255.0;
	return (color);
}

t_color	**parse_texture(char *img_path)
{
	int		i;
	int		j;
	int		fd;
	int		num_rows;
	int		num_cols;
	t_color	**colors;

	img_path = ft_strtrim(img_path, "\"");
	fd = open(img_path, O_RDONLY);
	free(img_path);
	if (fd == -1)
		exit(!printf(RED"ERROR OPENING TEXTURE FILE `%s`\n"RESET, img_path));
	read_ppm_header(fd, &num_cols, &num_rows);
	colors = ft_calloc(num_rows, sizeof(t_color));
	i = 0;
	while (i < num_rows)
	{
		j = 0;
		colors[i] = ft_calloc(num_cols, sizeof(t_color));
		while (j < num_cols)
			colors[i][j++] = read_ppm_color(fd);
		i++;
	}
	close(fd);
	return (colors);
}
