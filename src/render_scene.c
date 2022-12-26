/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 11:26:56 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/26 12:24:52 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// * Does not skip pixels
// void	*render_scene(t_worker *worker)
// {
// 	t_intersections	arr;
// 	int				x;
// 	int				y;
// 	unsigned int	shape_idx;
// 	t_ray			ray;
// 	t_color			color;
// 	int				line_counter;

// 	line_counter = 0;
// 	y = worker->y_start - 1;
// 	while (++y < worker->y_end)
// 	{
// 		x = -1;
// 		while (++x < worker->width)
// 		{
// 			set_color(worker, x, y, 0);
// 			ray_for_pixel(&ray, &worker->scene->camera, x, y);
// 			shape_idx = -1;
// 			arr.count = 0;
// 			while (++shape_idx < worker->scene->count.shapes)
// 			{
// 				intersect(&worker->scene->shapes[shape_idx], &ray, &arr);
// 			}
// 			color = calculate_lighting(&arr, worker->scene, &ray);
// 			set_color(worker, x, y, create_mlx_color(&color));
// 		}
// 		line_counter++;
// 		if (worker->scene->settings.edit_mode == false
// 			&& (line_counter == (worker->y_end - worker->y_start) / 5))
// 		{
// 			sem_post(worker->scene->sem_loading);
// 			line_counter = 0;
// 		}
// 	}
// 	return (NULL);
// }

void	render_pixel(int x, int y, t_intersections *arr, t_worker *worker)
{
	int		shape_idx;
	t_color	color;
	t_scene	*scene;
	t_ray	ray;

	set_color(worker, x, y, 0);
	scene = worker->scene;
	ray_for_pixel(&ray, &scene->camera, x, y);
	shape_idx = -1;
	arr->count = 0;
	while (++shape_idx < scene->count.shapes)
		intersect(&scene->shapes[shape_idx], &ray, arr);
	color = calculate_lighting(arr, scene, &ray);
	set_color(worker, x, y, create_mlx_color(&color));
}

void	fill_in_skipped_pixels_h(int x, int y, t_worker *worker, int threshold)
{
	int				c1;
	int				c4;
	t_intersections	arr;

	c1 = get_color(worker, x - 1, y);
	c4 = get_color(worker, x + 2, y);
	if (color_difference(c1, c4) > threshold)
	{
		render_pixel(x, y, &arr, worker);
		if (color_difference(get_color(worker, x, y), c4) > threshold)
			render_pixel(x + 1, y, &arr, worker);
		else
			set_color(worker, x + 1, y, color_avg(get_color(worker, x, y), c4));
	}
	else
	{
		set_color(worker, x, y, color_mix(c1, c4, 2.0 / 3.0));
		set_color(worker, x + 1, y, color_mix(c1, c4, 1.0 / 3.0));
	}
}

void	fill_in_skipped_pixels_v(int x, int y, t_worker *worker, int threshold)
{
	int				c1;
	int				c4;
	t_intersections	arr;

	c1 = get_color(worker, x, y - 1);
	c4 = get_color(worker, x, y + 2);
	if (color_difference(c1, c4) > threshold)
	{
		render_pixel(x, y, &arr, worker);
		if (color_difference(get_color(worker, x, y), c4) > threshold)
			render_pixel(x, y + 1, &arr, worker);
		else
			set_color(worker, x, y + 1, color_avg(get_color(worker, x, y), c4));
	}
	else
	{
		set_color(worker, x, y, color_mix(c1, c4, 2.0 / 3.0));
		set_color(worker, x, y + 1, color_mix(c1, c4, 1.0 / 3.0));
	}
}

void	fill_in_horizontal(t_worker *worker, int threshold)
{
	int				x;
	int				y;

	y = worker->y_start - 1;
	while (++y < worker->y_end)
	{
		x = 1;
		while (x < worker->width)
		{
			if (x + 2 < worker->width)
				fill_in_skipped_pixels_h(x, y, worker, threshold);
			else
			{
				if (x < worker->width)
					set_color(worker, x, y, get_color(worker, x - 1, y));
				if (x + 1 < worker->width)
					set_color(worker, x + 1, y, get_color(worker, x, y));
			}
			x += 3;
		}
	}
}

void	fill_in_vertical(t_worker *worker, int threshold)
{
	int				x;
	int				y;

	y = worker->y_start + 1;
	while (y < worker->y_end)
	{
		x = 1;
		while (x < worker->width)
		{
			if (y + 2 < worker->height)
				fill_in_skipped_pixels_v(x, y, worker, threshold);
			else
			{
				if (y < worker->height)
					set_color(worker, x, y, get_color(worker, x, y - 1));
				if (y + 1 < worker->height)
					set_color(worker, x, y + 1, get_color(worker, x, y));
			}
			x++;
		}
		y += 3;
	}
}

void	update_loading_bar(t_worker *worker, int *line_counter)
{
	if (worker->scene->settings.edit_mode == false
		&& (*line_counter == (worker->y_end - worker->y_start) / 15))
	{
		sem_post(worker->scene->sem_loading);
		*line_counter = 0;
	}
}

void	*render_scene_fast(t_worker *worker)
{
	t_intersections	arr;
	int				x;
	int				y;
	int				line_counter;

	line_counter = 0;
	y = worker->y_start;
	while (y < worker->y_end)
	{
		x = 0;
		while (x < worker->width)
		{
			render_pixel(x, y, &arr, worker);
			x += 3;
		}
		line_counter++;
		update_loading_bar(worker, &line_counter);
		y += 3;
	}
	fill_in_horizontal(worker, 10);
	fill_in_vertical(worker, 10);
	return (NULL);
}
