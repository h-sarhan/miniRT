/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 20:19:41 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/03 22:25:30 by hsarhan          ###   ########.fr       */
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
	scale_vec(&intersection->over_point, &intersection->normal, 0.001);
	add_vec(&intersection->over_point, &intersection->point, &intersection->over_point);
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
		i++;
	}
}

void	*render_scene(void *worker_ptr)
{
	t_intersections	arr;
	t_worker	*worker;
	int			x;
	int			y;
	t_color			light_color;
	t_color			color;
	t_intersect		*intersection;
	int pixel;
	unsigned int	shape_idx;
	t_ray			ray;

	x = 0;
	worker = worker_ptr;
	y = worker->y_start;
	arr.count = 0;
	pixel = worker->scene->mlx->bytes_per_pixel * y * worker->scene->render_w;
	while (y < worker->y_end)
	{
		x = 0;
		while (x < worker->scene->render_w)
		{
			*(unsigned int *)(worker->scene->mlx->addr + pixel) = 0;
			ray_for_pixel(&ray, &worker->scene->camera, x, y);
			shape_idx = 0;
			arr.count = 0;
			while (shape_idx < worker->scene->count.shape_count)
			{
				intersect(&worker->scene->shapes[shape_idx], &ray, &arr);
				shape_idx++;
			}
			intersection  = hit(&arr);
			if (intersection != NULL)
			{
				prepare_computations(intersection, &ray);
				ft_bzero(&color, sizeof(t_color));
				unsigned int light_idx = 0;
				while (light_idx < worker->scene->count.light_count)
				{
					light_color = lighting(intersection, worker->scene, light_idx);
					add_colors(&color, &color, &light_color);
					light_idx++;
				}
				intersection->shape->mlx_color = create_mlx_color(&color);
				*(unsigned int *)(worker->scene->mlx->addr + pixel) = intersection->shape->mlx_color;
			}
			pixel += worker->scene->mlx->bytes_per_pixel;
			x++;
		}
		y++;
	}
	return (NULL);
}


/**
 * @brief Draws a scene
 * @param scene A struct describing the current scene
 */
void draw_scene(t_scene *scene)
{
	int				x;
	int				y;
	t_mlx			*mlx;
	t_worker		workers[NUM_THREADS];
	pthread_t		threads[NUM_THREADS];

	mlx = scene->mlx;
	x = 0;
	y = 0;
	init_workers(workers, scene);
	
	int pixel = 0;
	
	struct timespec start, finish;
	double elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start);

	int i = 0;
	while (i < NUM_THREADS)
	{
		pthread_create(&threads[i], NULL, render_scene, &workers[i]);
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


	y = 0;
	pixel = 0;
	TICK(scale);
	while (y < scene->win_h)
	{
		x = 0;
		while (x < scene->win_w)
		{
			int src_x = roundf(((float)x / (float)scene->win_w) * scene->render_w);
			int src_y = roundf(((float)y / (float)scene->win_h) * scene->render_h);
			src_x = min(src_x, scene->render_w - 1);
			src_y = min(src_y, scene->render_h - 1);
			// printf("%d %d\n", src_x, src_y);
			*(unsigned int *)(mlx->display_addr + pixel) = *(unsigned int *)(mlx->addr + (src_y * mlx->line_length + src_x * (mlx->bytes_per_pixel)));
			pixel += mlx->bytes_per_pixel;
			x++;
		}
		y++;
	}
	TOCK(scale);

	mlx_put_image_to_window(mlx->mlx, mlx->mlx_win, mlx->display_img, 0, 0);
}
