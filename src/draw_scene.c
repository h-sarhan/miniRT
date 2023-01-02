/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 20:19:41 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/02 16:25:18 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	prepare_computations(t_intersection *intersection,
			t_ray *ray)
{
	ray_position(&intersection->point, ray, intersection->time);
	intersection->normal = normal_at(intersection->shape, &intersection->point);
	negate_vec(&intersection->eye, &ray->direction);
	intersection->eye.w = 0;
	intersection->inside = false;
	if (dot_product(&intersection->normal, &intersection->eye) < 0)
	{
		intersection->inside = true;
		negate_vec(&intersection->normal, &intersection->normal);
	}
	scale_vec(&intersection->over_point, &intersection->normal, EPSILON);
	add_vec(&intersection->over_point, &intersection->point,
		&intersection->over_point);
	reflect(&intersection->reflect_vec, &ray->direction, &intersection->normal);
}

void	init_workers(t_worker *workers, t_scene *scene)
{
	unsigned int	i;

	i = 0;
	while (i < NUM_THREADS)
	{
		workers[i].height = scene->settings.render_h;
		workers[i].width = scene->settings.render_w;
		workers[i].addr = scene->disp->render_addr;
		if (scene->settings.edit_mode == true)
		{
			workers[i].height = scene->settings.edit_h;
			workers[i].width = scene->settings.edit_w;
			workers[i].addr = scene->disp->edit_addr;
		}
		workers[i].max_workers = NUM_THREADS;
		workers[i].worker_id = i;
		workers[i].scene = scene;
		workers[i].y_start = (workers[i].height / (float)NUM_THREADS) * i;
		workers[i].y_end = (workers[i].height / (float)NUM_THREADS) * (i + 1);
		workers[i].y_scale_start = (scene->settings.disp_h \
			/ (float)NUM_THREADS) * i;
		workers[i].y_scale_end = (scene->settings.disp_h \
			/ (float)NUM_THREADS) * (i + 1);
		i++;
	}
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

void	display_loading_bar(t_scene *scene)
{
	int	sem_counter;
	int	load;

	sem_counter = 0;
	if (scene->settings.edit_mode == false)
	{
		ft_putstr_fd("[", 1);
		load = 0;
		while (load < NUM_THREADS * 5)
		{
			ft_putstr_fd(RED".", 1);
			load++;
		}
		ft_putstr_fd(RESET"]", 1);
		ft_putstr_fd("\r", 1);
		ft_putstr_fd("[", 1);
		while (sem_counter < NUM_THREADS * 5)
		{
			ft_putstr_fd(GREEN"=", 1);
			sem_wait(scene->sem_loading);
			sem_counter++;
		}
		ft_putstr_fd(RESET"]\n", 1);
	}
}

void	run_threads(t_worker *workers, t_scene *scene, bool loading, void *func)
{
	pthread_t	threads[NUM_THREADS];
	int			i;

	i = -1;
	while (++i < NUM_THREADS)
		pthread_create(&threads[i], NULL, func, &workers[i]);
	if (loading == true)
		display_loading_bar(scene);
	i = -1;
	while (++i < NUM_THREADS)
		pthread_join(threads[i], NULL);
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
	run_threads(workers, scene, true, render_scene_fast);
	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("render time is %f\n", elapsed);
	run_threads(workers, scene, false, nearest_neighbours_scaling);
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
