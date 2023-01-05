/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   marker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:30:18 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/05 17:38:10 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	draw_marker(t_scene *scene, int x, int y, int color)
{
	char	*dst;
	int		width;

	width = scene->settings.disp_w;
	if (x <= 0 || y <= 0 || x >= width || y >= scene->settings.disp_h)
		return ;
	dst = scene->disp->disp_addr + (y * width + x) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + ((y + 1) * width + x) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + ((y - 1) * width + x) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + (y * width + x + 1) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + ((y + 1) * width + x + 1) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + ((y - 1) * width + x + 1) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + (y * width + x - 1) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + ((y - 1) * width + x - 1) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + ((y + 1) * width + x - 1) * scene->disp->bpp;
	*(unsigned int *)dst = color;
}

void	perspective_projection(t_vector *point, const t_scene *scene)
{
	point->x /= -point->z;
	point->y /= -point->z;
	point->x = (point->x + scene->cam.half_width) \
		/ (scene->cam.half_width * 2);
	point->y = (point->y + scene->cam.half_height) \
		/ (scene->cam.half_height * 2);
	point->x = 1 - point->x;
	point->y = 1 - point->y;
}

void	project_marker_on_screen(t_scene *scene, t_shape *shape)
{
	t_vector	origin_proj;

	mat_vec_multiply(&origin_proj, &scene->cam.transform, &shape->origin);
	perspective_projection(&origin_proj, scene);
	if (shape->type == SPHERE || shape->type == CYLINDER || shape->type == CUBE
		|| shape->type == CONE)
	{
		if (origin_proj.z < 0)
			draw_marker(scene, origin_proj.x * scene->settings.disp_w,
				origin_proj.y * scene->settings.disp_h, 0x00ffff);
		mlx_put_image_to_window(scene->disp->mlx, scene->disp->win,
			scene->disp->display_img, 0, 0);
		draw_arrow(scene, origin_proj.x * scene->settings.disp_w,
			origin_proj.y * scene->settings.disp_h, origin_proj.z);
	}
}

void	draw_shape_marker(t_scene *scene)
{
	int			shape_idx;
	t_shape		*shape;

	shape_idx = 0;
	if (scene->settings.edit_mode == false)
		return ;
	while (shape_idx < scene->count.shapes)
	{
		shape = &scene->shapes[shape_idx];
		if (shape->props.highlighted == false)
		{
			shape_idx++;
			continue ;
		}
		project_marker_on_screen(scene, shape);
		return ;
	}
	mlx_put_image_to_window(scene->disp->mlx, scene->disp->win,
		scene->disp->display_img, 0, 0);
}
