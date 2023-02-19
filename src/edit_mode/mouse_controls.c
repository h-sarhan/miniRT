/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_controls.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 10:20:14 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/05 21:04:53 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	mouse_select(t_scene *scene, double x, double y)
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
	t_intersections	arr;
	t_intersection	*itx;
	t_ray			mouse_selection;
	
	if (scene->mouse.key != LEFT_MOUSE_DOWN || scene->keys_held.shift == true)
		return ;
	scene->mouse.prev_x = scene->mouse.x;
	scene->mouse.prev_y = scene->mouse.y;
	mlx_mouse_get_pos(scene->disp->mlx, scene->disp->win, &scene->mouse.x,
		&scene->mouse.y);
	ray_from_cam(&mouse_selection, &scene->cam,
		((double)scene->mouse.prev_x * scene->settings.edit_w / scene->settings.disp_w) + .5,
		((double)scene->mouse.prev_y * scene->settings.edit_h / scene->settings.disp_h) + .5);
	int shape_idx = -1;
	arr.count = 0;
	while (++shape_idx < scene->count.shapes)
		intersect(&scene->shapes[shape_idx], &mouse_selection, &arr);
	itx = hit(&arr);
	if (itx == NULL || itx->shape->type == PLANE)
		return ;
	
	t_vector	plane_origin;
	t_vector	plane_orientation;
	double		distance_from_origin;
	negate_vec(&plane_orientation, &scene->cam.dir);
	plane_origin = scene->shapes[scene->shape_idx].origin;
	ray_from_cam(&mouse_selection, &scene->cam,
		((double)scene->mouse.x * scene->settings.edit_w / scene->settings.disp_w) + .5,
		((double)scene->mouse.y * scene->settings.edit_h / scene->settings.disp_h) + .5);
	distance_from_origin = dot_product(&plane_orientation, &plane_origin);
	double denom = dot_product(&mouse_selection.dir, &plane_orientation);
	if (fabs(denom) < 0.00001)
		return ;
	double t = -(dot_product(&mouse_selection.origin, &plane_orientation) \
		- distance_from_origin) / denom;
	t_vector	position_on_plane;
	ray_position(&position_on_plane, &mouse_selection, t);
	// t_vector	pos_diff;
	// sub_vec(&pos_diff, &scene->shapes[scene->shape_idx].origin, &position_on_plane);
	// add_vec(&scene->shapes[scene->shape_idx].origin, &scene->shapes[scene->shape_idx].origin, &pos_diff);
	scene->shapes[scene->shape_idx].origin = position_on_plane;
	// if (scene->settings.collisions == true)
	// 	collide(scene, true, 10, &scene->shapes[scene->shape_idx]);
	calculate_transforms(scene);
	draw_scene(scene);
}

int	mouse_rotate(t_scene *scene)
{
	if (scene->mouse.key != LEFT_MOUSE_DOWN || !scene->settings.edit_mode || scene->keys_held.shift == false)
		return (0);
	scene->mouse.prev_x = scene->mouse.x;
	scene->mouse.prev_y = scene->mouse.y;
	mlx_mouse_get_pos(scene->disp->mlx, scene->disp->win, &scene->mouse.x,
		&scene->mouse.y);
	if (abs(scene->mouse.x - scene->mouse.prev_x) > 1
		|| abs(scene->mouse.y - scene->mouse.prev_y) > 1)
	{
		rotate_along_mouse_axis(scene);
		// if (scene->settings.collisions == true)
		// 	collide(scene, true, 10, &scene->shapes[scene->shape_idx]);
		calculate_transforms(scene);
		draw_scene(scene);
	}
	return (0);
}
