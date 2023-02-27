/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 20:56:57 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/19 20:57:09 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	free_textures(t_scene *scene)
{
	int	i;

	i = -1;
	while (++i < scene->count.shapes && scene->shapes != NULL)
	{
		free_texture(&scene->shapes[i], scene->shapes[i].diffuse_tex);
		free_texture(&scene->shapes[i], scene->shapes[i].normal_tex);
	}
}

void	free_texture(t_shape *shape, t_color **texture)
{
	int	i;

	if (texture == NULL)
		return ;
	i = 0;
	while (i < shape->tex_height)
	{
		free(texture[i]);
		i++;
	}
	free(texture);
}
