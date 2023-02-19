/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:37:41 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/20 00:57:42 by hsarhan          ###   ########.fr       */
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
	t_scene	*scene;
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

t_color	super_sample_pixel(double x, double y, t_intersections *arr,
			t_worker *worker)
{
	int				i;
	int				j;
	t_color			color;
	t_color			avg_color;

	if (worker->scene->settings.supersampling == false
		|| worker->scene->settings.edit_mode == false)
		return (render_pixel(x, y, arr, worker));
	ft_bzero(&avg_color, sizeof(t_color));
	color = render_pixel(x, y, arr, worker);
	add_colors(&avg_color, &avg_color, &color);
	i = -1;
	while (++i < 2)
	{
		j = -1;
		while (++j < 2)
		{
			color = render_pixel(x + (i + 0.5) / 2, y + (j + 0.5) / 2,
					arr, worker);
			add_colors(&avg_color, &avg_color, &color);
		}
	}
	mult_color(&avg_color, &avg_color, 1.0 / (5));
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
	fill_in_horizontal(worker, 25);
	fill_in_vertical(worker, 25);
	return (NULL);
}

/**
 * @brief Draws a scene
 * @param scene A struct describing the current scene
 */
void	draw_scene(t_scene *scene)
{
	t_worker		workers[NUM_THREADS];

	init_workers(workers, scene);
	run_workers(workers, scene, true, render_scene_fast);
	run_workers(workers, scene, false, nearest_neighbours_scaling);
	show_help_menu(scene);
	mlx_put_image_to_window(scene->disp->mlx, scene->disp->win,
		scene->disp->display_img, 0, 0);
	if (scene->help == true)
		draw_controls(scene);
	else
	{
		draw_shape_marker(scene);
		if (scene->settings.light_mode == true)
		{
			project_light_marker_on_screen(scene);
			show_light_banner(scene);
		}
		else if (scene->settings.camera_mode == false)
			draw_shape_info(scene);
	}
}
