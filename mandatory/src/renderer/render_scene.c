/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:37:41 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/20 07:40:13 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	render_scene(t_scene *scene)
{
	t_intersections	arr;
	int				x;
	int				y;

	y = 0;
	while (y < scene->settings.disp_h)
	{
		x = 0;
		while (x < scene->settings.disp_w)
		{
			set_color(scene, x, y, 0);
			render_pixel(x, y, &arr, scene);
			x += 1;
		}
		y += 1;
	}
}

t_color	render_pixel(double x, double y, t_intersections *arr, t_scene *scene)
{
	int		shape_idx;
	t_ray	ray;
	t_color	color;

	set_color(scene, x, y, 0);
	ray_from_cam(&ray, &scene->cam, x + 0.5, y + 0.5);
	shape_idx = -1;
	arr->count = 0;
	while (++shape_idx < scene->count.shapes)
		intersect(&scene->shapes[shape_idx], &ray, arr);
	color = shade_point(arr, scene, &ray);
	set_color(scene, x, y, create_mlx_color(&color));
	return (color);
}

/**
 * @brief Draws a scene
 * @param scene A struct describing the current scene
 */
void	draw_scene(t_scene *scene)
{
	render_scene(scene);
	mlx_put_image_to_window(scene->disp->mlx, scene->disp->win,
		scene->disp->display_img, 0, 0);
}
