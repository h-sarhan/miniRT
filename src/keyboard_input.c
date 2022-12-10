/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 19:35:57 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/10 10:15:58 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	set_key_down(int key, t_scene *scene)
{
	if (key == KEY_TAB)
	{
		scene->shape_idx++;
		while (scene->shapes[scene->shape_idx % scene->count.shape_count].type == PLANE)
		{
			scene->shape_idx++;
		}
	}
	if (key == KEY_C)
		scene->camera_mode = !scene->camera_mode;
	if (key == KEY_W)
		scene->keys_held.w = true;
	if (key == KEY_A)
		scene->keys_held.a = true;
	if (key == KEY_S)
		scene->keys_held.s = true;
	if (key == KEY_D)
		scene->keys_held.d = true;
	if (key == KEY_UP)
		scene->keys_held.up = true;
	if (key == KEY_DOWN)
		scene->keys_held.down = true;
	if (key == KEY_LEFT)
		scene->keys_held.left = true;
	if (key == KEY_RIGHT)
		scene->keys_held.right = true;
	if (key == KEY_Q)
		scene->keys_held.q = true;
	if (key == KEY_E)
		scene->keys_held.e = true;
	if (key == KEY_PLUS)
		scene->keys_held.plus = true;
	if (key == KEY_MINUS)
		scene->keys_held.minus = true;
	return (0);
}

int	set_key_up(int key, t_scene *scene)
{
	if (key == KEY_C)
		scene->keys_held.c = false;
	if (key == KEY_TAB)
		scene->keys_held.tab = false;
	if (key == KEY_W)
		scene->keys_held.w = false;
	if (key == KEY_A)
		scene->keys_held.a = false;
	if (key == KEY_S)
		scene->keys_held.s = false;
	if (key == KEY_D)
		scene->keys_held.d = false;
	if (key == KEY_UP)
		scene->keys_held.up = false;
	if (key == KEY_DOWN)
		scene->keys_held.down = false;
	if (key == KEY_LEFT)
		scene->keys_held.left = false;
	if (key == KEY_RIGHT)
		scene->keys_held.right = false;
	if (key == KEY_Q)
		scene->keys_held.q = false;
	if (key == KEY_E)
		scene->keys_held.e = false;
	if (key == KEY_PLUS)
		scene->keys_held.plus = false;
	if (key == KEY_MINUS)
		scene->keys_held.minus = false;
	return (0);
}

int	key_handler(t_scene *scene)
{
	if (scene->camera_mode == true)
	{
		if (scene->keys_held.w == true)
		{
			scene->camera.position.x += 0.5 *sin(scene->camera.phi)*cos(scene->camera.theta);
			scene->camera.position.z += 0.5 *sin(scene->camera.phi)*sin(scene->camera.theta);
			scene->camera.position.y += 0.5 *cos(scene->camera.phi);
		}
		if (scene->keys_held.a == true)
		{
			scene->camera.position.x += 0.5 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
			scene->camera.position.z += 0.5 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
			scene->camera.position.y += 0.5 *cos(M_PI / 2);
		}
		if (scene->keys_held.s == true)
		{
			scene->camera.position.x -= 0.5 *sin(scene->camera.phi)*cos(scene->camera.theta);
			scene->camera.position.z -= 0.5 *sin(scene->camera.phi)*sin(scene->camera.theta);
			scene->camera.position.y -= 0.5 *cos(scene->camera.phi);
		}
		if (scene->keys_held.d == true)
		{
			scene->camera.position.x += 0.5 *sin(M_PI / 2)*cos(scene->camera.theta - M_PI / 2);
			scene->camera.position.z += 0.5 *sin(M_PI / 2)*sin(scene->camera.theta - M_PI / 2);
			scene->camera.position.y += 0.5 *cos(M_PI / 2);
		}
		if (scene->keys_held.up == true)
		{
			if (scene->camera.phi > 0.5)
				scene->camera.phi -= 0.05;
			scene->camera.orientation.x = sin(scene->camera.phi) * cos(scene->camera.theta);
			scene->camera.orientation.z = sin(scene->camera.phi) * sin(scene->camera.theta);
			scene->camera.orientation.y = cos(scene->camera.phi);
		}
		if (scene->keys_held.down == true)
		{
			if (scene->camera.phi < M_PI - 0.5)
				scene->camera.phi += 0.05;
			scene->camera.orientation.x = sin(scene->camera.phi) * cos(scene->camera.theta);
			scene->camera.orientation.z = sin(scene->camera.phi) * sin(scene->camera.theta);
			scene->camera.orientation.y = cos(scene->camera.phi);
		}
		if (scene->keys_held.left == true)
		{
			scene->camera.theta += 0.15;
			scene->camera.orientation.x = sin(scene->camera.phi) * cos(scene->camera.theta);
			scene->camera.orientation.z = sin(scene->camera.phi) * sin(scene->camera.theta);
			scene->camera.orientation.y = cos(scene->camera.phi);
		}
		if (scene->keys_held.right == true)
		{
			scene->camera.theta -= 0.15;
			scene->camera.orientation.x = sin(scene->camera.phi) * cos(scene->camera.theta);
			scene->camera.orientation.z = sin(scene->camera.phi) * sin(scene->camera.theta);
			scene->camera.orientation.y = cos(scene->camera.phi);
		}
		if (scene->keys_held.q == true)
			scene->camera.position.y += 0.35;
		if (scene->keys_held.e == true)
			scene->camera.position.y -= 0.35;
	}
	else
	{
		if (scene->keys_held.w == true)
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.y += 0.1;
		if (scene->keys_held.a == true)
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.x -= 0.1;
		if (scene->keys_held.s == true)
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.y -= 0.1;
		if (scene->keys_held.d == true)
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.x += 0.1;
		if (scene->keys_held.plus == true)
			scene->shapes[scene->shape_idx % scene->count.shape_count].radius += 0.1;
		if (scene->keys_held.minus == true)
		{
			if (scene->shapes[scene->shape_idx % scene->count.shape_count].radius > 0.25)
				scene->shapes[scene->shape_idx % scene->count.shape_count].radius -= 0.1;
		}
		if (scene->keys_held.up == true)
			scene->lights[0].position.y += 0.3;
		if (scene->keys_held.down == true)
			scene->lights[0].position.y -= 0.3;
		if (scene->keys_held.left == true)
			scene->lights[0].position.x -= 0.3;
		if (scene->keys_held.right == true)
			scene->lights[0].position.x += 0.3;
		if (scene->keys_held.q == true)
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.z += 0.1;
		if (scene->keys_held.e == true)
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.z -= 0.1;
	}
	if (scene->keys_held.w
		|| scene->keys_held.a
		|| scene->keys_held.s
		|| scene->keys_held.d
		|| scene->keys_held.up
		|| scene->keys_held.down
		|| scene->keys_held.left
		|| scene->keys_held.right
		|| scene->keys_held.q
		|| scene->keys_held.e
		|| scene->keys_held.plus
		|| scene->keys_held.minus
		)
	{
		calculate_transforms(scene);
		draw_scene(scene);
	}
	return (0);
}
