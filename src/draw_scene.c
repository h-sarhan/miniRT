/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 20:19:41 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/23 16:50:04 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	my_mlx_pixel_put(t_mlx *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bytes_per_pixel));
	*(unsigned int*)dst = color;
}

/**
 * @brief Draws a simple scene *for now*
 * @param scene A struct describing the current scene
 */
void draw_scene(t_scene *scene)
{
	float	wall_z;
	float	wall_size;
	float	pixel_size;
	float	half;
	float	world_y;
	float	world_x;
	t_vector position;
	t_vector ray_origin;
	t_ray	ray;
	t_intersections arr;
	t_intersect intersect_arr[100];
	int		x;
	int		y;
	t_mlx	*mlx;
	t_vector point;
	t_vector normal;
	t_vector eye;
	t_color	color;
	unsigned int		i;
	
	mlx = scene->mlx;
	x = 0;
	y = 0;
	wall_z = 10;
	wall_size = 7.0;
	ray_origin.x = 0;
	ray_origin.y = 0;
	ray_origin.z = -5;
	ray_origin.w = 1;
	pixel_size = wall_size / (float)scene->win_h;
	half = wall_size / 2;
	arr.arr = intersect_arr;
	arr.count = 0;
	int pixel = 0;
	TICK(render);
	while (y < scene->win_h)
	{
		world_y = half - (pixel_size * y);
		x = 0;
		while (x < scene->win_h)
		{
			world_x = (half * -1) + (pixel_size * x);
			position.x = world_x;
			position.y = world_y;
			position.z = wall_z;
			position.w = 1;
			ray.origin = ray_origin;
			sub_vec(&ray.direction, &position, &ray_origin);
			i = 0;
			while (i < scene->count.shape_count)
			{
				ray.direction.w = 0;
				arr.count = 0;
				intersect(&scene->shapes[i], &ray, &arr);
				t_intersect *intersection  = hit(&arr);
				if (intersection != NULL)
				{
					ray_position(&point, &ray, intersection->time);
					normal = normal_at(intersection->shape, &point);
					normal.w = 0;
					normalize_vec(&ray.direction);
					negate_vec(&eye, &ray.direction);
					eye.w = 0;
					color = lighting(intersection->shape, scene, &point, &eye, &normal);
					intersection->shape->mlx_color = create_mlx_color(&color);
					*(unsigned int *)(mlx->addr + pixel) = intersection->shape->mlx_color;
					// my_mlx_pixel_put(mlx, x, y, intersection->shape->mlx_color);
				}
				// else
				// 	*(unsigned int *)(mlx->addr + pixel) = 0;
				i++;
			}
			pixel += mlx->bytes_per_pixel;
			x++;
		}
		y++;
	}
	TOCK(render);
	mlx_put_image_to_window(mlx->mlx, mlx->mlx_win, mlx->img, 0, 0);
}
