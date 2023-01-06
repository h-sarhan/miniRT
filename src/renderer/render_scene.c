/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:37:41 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/06 17:56:40 by hsarhan          ###   ########.fr       */
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
// 			ray_from_cam(&ray, &worker->scene->camera, x, y);
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

t_color	render_pixel(float x, float y, t_intersections *arr, t_worker *worker)
{
	int		shape_idx;
	t_scene	*scene;
	t_ray	ray;
	t_color	color;

	set_color(worker, x, y, 0);
	scene = worker->scene;
	ray_from_cam(&ray, &scene->cam, x + 0.5, y + 0.5);
	shape_idx = -1;
	arr->count = 0;
	while (++shape_idx < scene->count.shapes)
		intersect(&scene->shapes[shape_idx], &ray, arr);
	color = shade_point(arr, scene, &ray);
	set_color(worker, x, y, create_mlx_color(&color));
	return (color);
}

t_color	super_sample_pixel(float x, float y, t_intersections *arr,
			t_worker *worker)
{
	float	i;
	float	j;
	t_color	color;
	t_color	avg_color;

	if (worker->scene->supersampling == false)
		return (render_pixel(x, y, arr, worker));
	ft_bzero(&avg_color, sizeof(t_color));
	i = 0;
	while (i <= 0.75)
	{
		j = 0;
		while (j <= 0.75)
		{
			color = render_pixel(x + i, y + j, arr, worker);
			mult_color(&color, &color, 1 / 16.0);
			add_colors(&avg_color, &avg_color, &color);
			j += 0.25;
		}
		i += 0.25;
	}
	set_color(worker, x, y, create_mlx_color(&avg_color));
	return (avg_color);
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
			set_color(worker, x, y, 0);
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

void	*nearest_neighbours_scaling(t_worker *worker)
{
	int			x;
	int			y;
	int			src_x;
	int			src_y;

	y = worker->y_scale_start - 1;
	while (++y < worker->y_scale_end)
	{
		x = -1;
		while (++x < worker->scene->settings.disp_w)
		{
			src_x = round((x / (float)worker->scene->settings.disp_w) * \
			worker->width);
			src_y = round((y / (float)worker->scene->settings.disp_h) * \
			worker->height);
			src_x = min(src_x, worker->width - 1);
			src_y = min(src_y, worker->height - 1);
			*(unsigned int *)(worker->scene->disp->disp_addr + (y * \
			worker->scene->settings.disp_w + x) * worker->scene->disp->bpp) \
			= *(unsigned int *)(worker->addr + (src_y * \
			worker->width + src_x) * worker->scene->disp->bpp);
		}
	}
	return (NULL);
}

/**
 * @brief Draws a scene
 * @param scene A struct describing the current scene
 */
void	draw_scene(t_scene *scene)
{
	t_worker		workers[NUM_THREADS];
	struct timespec	start;
	struct timespec	finish;
	float			elapsed;

	init_workers(workers, scene);
	clock_gettime(CLOCK_MONOTONIC, &start);
	run_workers(workers, scene, true, render_scene_fast);
	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("render time is %f\n", elapsed);
	run_workers(workers, scene, false, nearest_neighbours_scaling);
	if (scene->settings.edit_mode == false)
		mlx_put_image_to_window(scene->disp->mlx, scene->disp->win,
			scene->disp->display_img, 0, 0);
	else
	{
		draw_shape_marker(scene);
		if (scene->settings.camera_mode == false)
			draw_shape_info(scene);
	}
}
