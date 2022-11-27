/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 20:19:41 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/27 14:14:00 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	my_mlx_pixel_put(t_mlx *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bytes_per_pixel));
	*(unsigned int*)dst = color;
}

void	prepare_computations(t_intersect *intersection, t_ray *ray)
{
	ray_position(&intersection->point, ray, intersection->time);
	intersection->normal = normal_at(intersection->shape, &intersection->point);
	negate_vec(&intersection->eye, &ray->direction);
	intersection->eye.w = 0;
	if (dot_product(&intersection->normal, &intersection->eye) < 0)
	{
		intersection->inside = true;
		negate_vec(&intersection->normal, &intersection->normal);
	}
	else
		intersection->inside = false;
}

/**
 * @brief Draws a scene
 * @param scene A struct describing the current scene
 */
void draw_scene(t_scene *scene)
{
	t_intersections	arr;
	t_ray			ray;
	int				x;
	int				y;
	t_mlx			*mlx;
	t_color			color;
	unsigned int	shape_idx;
	t_intersect		*intersection;
	t_color			light_color;

	mlx = scene->mlx;
	x = 0;
	y = 0;
	arr.count = 0;
	int pixel = 0;
	TICK(render);
	while (y < scene->win_h)
	{
		x = 0;
		while (x < scene->win_w)
		{
			*(unsigned int *)(mlx->addr + pixel) = 0;
			ray_for_pixel(&ray, &scene->camera, x, y);
			shape_idx = 0;
			arr.count = 0;
			while (shape_idx < scene->count.shape_count)
			{
				intersect(&scene->shapes[shape_idx], &ray, &arr);
				intersection  = hit(&arr);
				if (intersection != NULL)
				{
					prepare_computations(intersection, &ray);
					ft_bzero(&color, sizeof(t_color));
					unsigned int light_idx = 0;
					while (light_idx < scene->count.light_count)
					{
						light_color = lighting(intersection, scene, light_idx);
						add_colors(&color, &color, &light_color);
						light_idx++;
					}
					intersection->shape->mlx_color = create_mlx_color(&color);
					*(unsigned int *)(mlx->addr + pixel) = intersection->shape->mlx_color;
				}
				shape_idx++;
			}
			pixel += mlx->bytes_per_pixel;
			x++;
		}
		y++;
	}
	TOCK(render);
	mlx_put_image_to_window(mlx->mlx, mlx->mlx_win, mlx->img, 0, 0);
}
