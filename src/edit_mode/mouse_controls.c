/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_controls.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 10:20:14 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/05 14:40:58 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	mouse_down(int key_code, int x, int y, t_scene *scene)
{
	t_ray			mouse_selection;
	t_intersections	arr;
	t_intersection	*itx;
	int				shape_idx;

	if (scene->settings.edit_mode == false)
		return (0);
	if (x < 0 || y < 0 || x >= scene->settings.disp_w
		|| y >= scene->settings.disp_h)
		return (0);
	scene->mouse.key = key_code;
	scene->mouse.x = x;
	scene->mouse.y = y;
	ray_from_cam(&mouse_selection, &scene->camera,
		(scene->mouse.x * scene->settings.edit_w / scene->settings.disp_w) + 0.5,
		(scene->mouse.y * scene->settings.edit_h / scene->settings.disp_h) + 0.5);
	shape_idx = -1;
	arr.count = 0;
	while (++shape_idx < scene->count.shapes)
		intersect(&scene->shapes[shape_idx], &mouse_selection, &arr);
	itx = hit(&arr);
	if (itx->shape->type != PLANE)
	{
		scene->shapes[scene->shape_idx].props.highlighted = false;
		scene->shape_idx = itx->shape->id;
		itx->shape->props.highlighted = true;
	}
	draw_scene(scene);
	return (0);
}

int	mouse_up(int key_code, int x, int y, t_scene *scene)
{
	(void)x;
	(void)y;
	(void)key_code;
	scene->mouse.key = 0;
	return (0);
}

int	mouse_rotate(t_scene *scene)
{
	float	x_diff;
	float	y_diff;

	if (scene->mouse.key == LEFT_MOUSE_DOWN && scene->settings.edit_mode)
	{
		scene->mouse.prev_x = scene->mouse.x;
		scene->mouse.prev_y = scene->mouse.y;
		mlx_mouse_get_pos(scene->disp->mlx, scene->disp->win,
			&scene->mouse.x, &scene->mouse.y);
		if (abs(scene->mouse.x - scene->mouse.prev_x) > 1
			|| abs(scene->mouse.y - scene->mouse.prev_y) > 1)
		{
			x_diff = abs(scene->mouse.x - scene->mouse.prev_x) * 0.2;
			y_diff = abs(scene->mouse.y - scene->mouse.prev_y) * 0.2;
			if (scene->mouse.prev_x < scene->mouse.x)
				rotate_object_y(scene, &scene->shapes[scene->shape_idx],
					DEG_TO_RAD * x_diff);
			else if (scene->mouse.prev_x > scene->mouse.x)
				rotate_object_y(scene, &scene->shapes[scene->shape_idx],
					-DEG_TO_RAD * x_diff);
			if (scene->mouse.prev_y < scene->mouse.y)
				rotate_object_x(scene, &scene->shapes[scene->shape_idx],
					-DEG_TO_RAD * y_diff);
			else if (scene->mouse.prev_y > scene->mouse.y)
				rotate_object_x(scene, &scene->shapes[scene->shape_idx],
					DEG_TO_RAD * y_diff);
			if (scene->settings.collisions == true)
				collide(scene, true, 100, &scene->shapes[scene->shape_idx]);
			calculate_transforms(scene);
			draw_scene(scene);
		}
	}
	return (0);
}
