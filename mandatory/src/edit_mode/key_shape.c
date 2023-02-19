/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_shape.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 22:19:02 by mkhan             #+#    #+#             */
/*   Updated: 2023/02/19 22:20:11 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	default_shape_props(t_props *props)
{
	ft_bzero(&props->color, sizeof(t_color));
	props->diffuse = 0.9;
	props->highlighted = true;
	props->radius = 0.7;
	props->radius_squared = 0.7 * 0.7;
	props->scale.x = 0.7;
	props->scale.y = 0.7;
	props->scale.z = 0.7;
	props->reflectiveness = 0.2;
	props->shininess = 50;
	props->specular = 0.8;
}

void	spawn_shape(t_scene *scene)
{
	if (scene->count.shapes >= SHAPE_MAX - 1)
		return ;
	scene->shapes[scene->shape_idx].props.highlighted = false;
	scene->shapes[scene->count.shapes].origin.x = scene->cam.dir.x * 5 \
		+ scene->cam.position.x;
	scene->shapes[scene->count.shapes].origin.y = scene->cam.dir.y + \
		scene->cam.position.y;
	scene->shapes[scene->count.shapes].origin.z = scene->cam.dir.z * 5 + \
		scene->cam.position.z;
	scene->shapes[scene->count.shapes].origin.w = 1;
	default_shape_props(&scene->shapes[scene->count.shapes].props);
	scene->shapes[scene->count.shapes].type = SPHERE;
	scene->shapes[scene->count.shapes].id = scene->count.shapes;
	ft_bzero(&scene->shapes[scene->count.shapes].orientation, sizeof(t_vector));
	scene->shapes[scene->count.shapes].orientation.y = 1;
	identity_matrix(&scene->shapes[scene->count.shapes].added_rots);
	scene->count.shapes++;
	scene->shape_idx = scene->count.shapes - 1;
}

void	toggle_keys_held2(int key, t_scene *scene, bool on_off)
{
	if (key == KEY_W)
		scene->keys_held.w = on_off;
	if (key == KEY_A)
		scene->keys_held.a = on_off;
	if (key == KEY_S)
		scene->keys_held.s = on_off;
	if (key == KEY_D)
		scene->keys_held.d = on_off;
	if (key == KEY_UP)
		scene->keys_held.up = on_off;
	if (key == KEY_DOWN)
		scene->keys_held.down = on_off;
	if (key == KEY_LEFT)
		scene->keys_held.left = on_off;
	if (key == KEY_RIGHT)
		scene->keys_held.right = on_off;
}

void	toggle_keys_held(int key, t_scene *scene, bool on_off)
{
	if (key == KEY_Q)
		scene->keys_held.q = on_off;
	if (key == KEY_E)
		scene->keys_held.e = on_off;
	if (key == KEY_PLUS)
		scene->keys_held.plus = on_off;
	if (key == KEY_MINUS)
		scene->keys_held.minus = on_off;
	if (key == KEY_SHIFT)
		scene->keys_held.shift = on_off;
	if (key == KEY_X)
		scene->keys_held.x = on_off;
	if (key == KEY_Y)
		scene->keys_held.y = on_off;
	if (key == KEY_Z)
		scene->keys_held.z = on_off;
	if (key == KEY_O)
		scene->keys_held.o = on_off;
	toggle_keys_held2(key, scene, on_off);
}

void	select_shape(int key, t_scene *scene)
{
	if (key == KEY_TAB && scene->settings.edit_mode == true
		&& scene->settings.light_mode == false)
	{
		scene->shapes[scene->shape_idx].props.highlighted = false;
		scene->shape_idx = (scene->shape_idx + 1) % scene->count.shapes;
		while (scene->shapes[scene->shape_idx].type == PLANE)
			scene->shape_idx = (scene->shape_idx + 1) % scene->count.shapes;
		scene->shapes[scene->shape_idx].props.highlighted = true;
	}
}
