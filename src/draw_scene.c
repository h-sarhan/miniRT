/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 20:19:41 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/05 14:19:06 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
	scale_vec(&intersection->over_point, &intersection->normal, 0.001);
	add_vec(&intersection->over_point, &intersection->point,
		&intersection->over_point);
	reflect(&intersection->reflect_vec, &ray->direction, &intersection->normal);
}

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

void	init_workers(t_worker *workers, t_scene *scene)
{
	unsigned int	i;

	i = 0;
	while (i < NUM_THREADS)
	{
		workers[i].max_workers = NUM_THREADS;
		workers[i].worker_id = i;
		workers[i].scene = scene;
		workers[i].y_start = (scene->render_h / (float)NUM_THREADS) * i;
		workers[i].y_end = (scene->render_h / (float)NUM_THREADS) * (i + 1);
		workers[i].y_scale_start = (scene->win_h / (float)NUM_THREADS) * i;
		workers[i].y_scale_end = (scene->win_h / (float)NUM_THREADS) * (i + 1);
		i++;
	}
}

void	calculate_lighting(t_intersections *arr, t_worker *worker, t_ray *ray,
	int pixel)
{
	t_intersect		*itx;
	unsigned int	light_idx;
	t_color			final_color;
	t_color			light_color;

	itx = hit(arr);
	if (itx != NULL)
	{
		prepare_computations(itx, ray);
		ft_bzero(&final_color, sizeof(t_color));
		light_idx = 0;
		while (light_idx < worker->scene->count.light_count)
		{
			light_color = lighting(itx, worker->scene, light_idx);
			add_colors(&final_color, &final_color, &light_color);
			light_idx++;
		}
		itx->shape->mlx_color = create_mlx_color(&final_color);
		*(int *)(worker->scene->mlx->addr + pixel) = itx->shape->mlx_color;
	}
}

void	*render_scene(t_worker *worker)
{
	t_intersections	arr;
	int				x;
	int				y;
	unsigned int	shape_idx;
	t_ray			ray;

	y = worker->y_start - 1;
	while (++y < worker->y_end)
	{
		x = -1;
		while (++x < worker->scene->render_w)
		{
			*(unsigned int *)(worker->scene->mlx->addr + \
				(y * worker->scene->render_w + x) * \
				worker->scene->mlx->bytes_per_pixel) = 0;
			ray_for_pixel(&ray, &worker->scene->camera, x, y);
			shape_idx = -1;
			arr.count = 0;
			while (++shape_idx < worker->scene->count.shape_count)
				intersect(&worker->scene->shapes[shape_idx], &ray, &arr);
			calculate_lighting(&arr, worker, &ray, (y * worker->scene->render_w \
				+ x) * worker->scene->mlx->bytes_per_pixel);
		}
	}
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
		while (++x < worker->scene->win_w)
		{
			src_x = round((x / (float)worker->scene->win_w) * \
			worker->scene->render_w);
			src_y = round((y / (float)worker->scene->win_h) * \
			worker->scene->render_h);
			src_x = min(src_x, worker->scene->render_w - 1);
			src_y = min(src_y, worker->scene->render_h - 1);
			*(unsigned int *)(worker->scene->mlx->display_addr + (y * \
			worker->scene->win_w + x) * worker->scene->mlx->bytes_per_pixel) = \
			*(unsigned int *)(worker->scene->mlx->addr + (src_y * \
			worker->scene->mlx->line_length + src_x * \
			(worker->scene->mlx->bytes_per_pixel)));
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
	t_mlx			*mlx;
	t_worker		workers[NUM_THREADS];
	pthread_t		threads[NUM_THREADS];
	struct timespec	start;
	struct timespec	finish;
	double			elapsed;
	int				i;

	mlx = scene->mlx;
	init_workers(workers, scene);
	clock_gettime(CLOCK_MONOTONIC, &start);
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_create(&threads[i], NULL, (void *)render_scene, &workers[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("render time is %f\n", elapsed);
	clock_gettime(CLOCK_MONOTONIC, &start);
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_create(&threads[i], NULL, (void *)nearest_neighbours_scaling,
			&workers[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("scale time is %f\n", elapsed);
	mlx_put_image_to_window(mlx->mlx, mlx->mlx_win, mlx->display_img, 0, 0);
}
