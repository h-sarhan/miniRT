/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_controls.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 10:20:14 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/28 14:38:05 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	mouse_select(t_scene *scene, float x, float y)
{
	t_intersections	arr;
	t_intersection	*itx;
	t_ray			mouse_selection;
	int				shape_idx;

	ray_from_cam(&mouse_selection, &scene->cam,
		(x * scene->settings.edit_w / scene->settings.disp_w) + .5,
		(y * scene->settings.edit_h / scene->settings.disp_h) + .5);
	shape_idx = -1;
	arr.count = 0;
	while (++shape_idx < scene->count.shapes)
		intersect(&scene->shapes[shape_idx], &mouse_selection, &arr);
	itx = hit(&arr);
	if (itx == NULL || itx->shape->type == PLANE)
		return ;
	scene->shapes[scene->shape_idx].props.highlighted = false;
	scene->shape_idx = itx->shape->id;
	itx->shape->props.highlighted = true;
}

int	mouse_down(int key_code, int x, int y, t_scene *scene)
{
	if (x < 0 || y < 0 || x >= scene->settings.disp_w
		|| y >= scene->settings.disp_h || scene->settings.edit_mode == false
		|| key_code != LEFT_MOUSE_DOWN)
		return (0);
	scene->mouse.key = key_code;
	scene->mouse.x = x;
	scene->mouse.y = y;
	mouse_select(scene, x, y);
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

void	rotate_along_mouse_axis(t_scene *scene)
{
	if (scene->mouse.prev_x < scene->mouse.x)
	{
		rotate_y(scene, &scene->shapes[scene->shape_idx].added_rots,
			DEG_TO_RAD * abs(scene->mouse.x - scene->mouse.prev_x) * 0.2);
	}
	else if (scene->mouse.prev_x > scene->mouse.x)
	{
		rotate_y(scene, &scene->shapes[scene->shape_idx].added_rots,
			-DEG_TO_RAD * abs(scene->mouse.x - scene->mouse.prev_x) * 0.2);
	}
	if (scene->mouse.prev_y < scene->mouse.y)
	{
		rotate_x(scene, &scene->shapes[scene->shape_idx].added_rots,
			-DEG_TO_RAD * abs(scene->mouse.y - scene->mouse.prev_y) * 0.2);
	}
	else if (scene->mouse.prev_y > scene->mouse.y)
	{
		rotate_x(scene, &scene->shapes[scene->shape_idx].added_rots,
			DEG_TO_RAD * abs(scene->mouse.y - scene->mouse.prev_y) * 0.2);
	}
}


void	mouse_move(t_scene *scene)
{
	(void)scene;
	// t_intersections	arr;
	// t_intersection	*itx;
	// t_ray			mouse_selection;
	// int				shape_idx;

	// ray_from_cam(&mouse_selection, &scene->cam,
	// 	((float)scene->mouse.x * scene->settings.edit_w / scene->settings.disp_w) + .5,
	// 	((float)scene->mouse.y * scene->settings.edit_h / scene->settings.disp_h) + .5);
	// shape_idx = -1;
	// arr.count = 0;
	// while (++shape_idx < scene->count.shapes)
	// 	intersect(&scene->shapes[shape_idx], &mouse_selection, &arr);
	// itx = hit(&arr);
	// if (itx == NULL || itx->shape->type == PLANE)
	// 	return ;
}

int	mouse_rotate(t_scene *scene)
{
	if (scene->mouse.key != LEFT_MOUSE_DOWN || !scene->settings.edit_mode || scene->keys_held.shift == true)
		return (0);
	scene->mouse.prev_x = scene->mouse.x;
	scene->mouse.prev_y = scene->mouse.y;
	mlx_mouse_get_pos(scene->disp->mlx, scene->disp->win, &scene->mouse.x,
		&scene->mouse.y);
	if (abs(scene->mouse.x - scene->mouse.prev_x) > 1
		|| abs(scene->mouse.y - scene->mouse.prev_y) > 1)
	{
		rotate_along_mouse_axis(scene);
		if (scene->settings.collisions == true)
			collide(scene, true, 100, &scene->shapes[scene->shape_idx]);
		calculate_transforms(scene);
		draw_scene(scene);
	}
	return (0);
}
