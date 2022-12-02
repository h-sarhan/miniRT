/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 19:35:57 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/02 20:00:38 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Function to transform a shape when a key is pressed
 * @param key Key that was pressed
 * @param scene The scene struct
 */
int	transform_shape(int key, t_scene *scene)
{
	if (key == KEY_W)
		scene->shapes[0].origin.y += 0.1;
	if (key == KEY_A)
		scene->shapes[0].origin.x -= 0.1;
	if (key == KEY_S)
		scene->shapes[0].origin.y -= 0.1;
	if (key == KEY_D)
		scene->shapes[0].origin.x += 0.1;
	if (key == KEY_PLUS)
		scene->shapes[0].radius += 0.1;
	if (key == KEY_MINUS)
		scene->shapes[0].radius -= 0.1;
	if (key == KEY_UP)
		scene->lights[0].position.y += 0.3;
	if (key == KEY_DOWN)
		scene->lights[0].position.y -= 0.3;
	if (key == KEY_LEFT)
		scene->lights[0].position.x -= 0.3;
	if (key == KEY_RIGHT)
		scene->lights[0].position.x += 0.3;
	if (key == KEY_Q)
		scene->lights[0].intensity += 0.1;
	if (key == KEY_E)
		scene->lights[0].intensity -= 0.1;
	if (key == KEY_A || key == KEY_S || key == KEY_D || key == KEY_PLUS
		|| key == KEY_MINUS || key == KEY_W || key == KEY_UP || key == KEY_DOWN
		|| key == KEY_LEFT || key == KEY_RIGHT || key == KEY_E || key == KEY_Q)
	{
		calculate_transforms(scene);
		draw_scene(scene);
	}
	return (0);
}

int	set_key_down(int key, t_scene *scene)
{
	if (key == KEY_W)
	{
		scene->keys_held.w = true;
	}
	if (key == KEY_A)
	{
		scene->keys_held.a = true;
	}
	if (key == KEY_S)
	{
		scene->keys_held.s = true;
	}
	if (key == KEY_D)
	{
		scene->keys_held.d = true;
	}
	if (key == KEY_UP)
	{
		scene->keys_held.up = true;
	}
	if (key == KEY_DOWN)
	{
		scene->keys_held.down = true;
	}
	if (key == KEY_LEFT)
	{
		scene->keys_held.left = true;
	}
	if (key == KEY_RIGHT)
	{
		scene->keys_held.right = true;
	}
	if (key == KEY_Q)
	{
		scene->keys_held.q = true;
	}
	if (key == KEY_E)
	{
		scene->keys_held.e = true;
	}
	return (0);
}

int	set_key_up(int key, t_scene *scene)
{
	if (key == KEY_W)
	{
		scene->keys_held.w = false;
	}
	if (key == KEY_A)
	{
		scene->keys_held.a = false;
	}
	if (key == KEY_S)
	{
		scene->keys_held.s = false;
	}
	if (key == KEY_D)
	{
		scene->keys_held.d = false;
	}
	if (key == KEY_UP)
	{
		scene->keys_held.up = false;
	}
	if (key == KEY_DOWN)
	{
		scene->keys_held.down = false;
	}
	if (key == KEY_LEFT)
	{
		scene->keys_held.left = false;
	}
	if (key == KEY_RIGHT)
	{
		scene->keys_held.right = false;
	}
	if (key == KEY_Q)
	{
		scene->keys_held.q = false;
	}
	if (key == KEY_E)
	{
		scene->keys_held.e = false;
	}
	return (0);
}

int	transform_camera(t_scene *scene)
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
			scene->camera.phi -= 0.08;
		scene->camera.orientation.x = sin(scene->camera.phi) * cos(scene->camera.theta);
		scene->camera.orientation.z = sin(scene->camera.phi) * sin(scene->camera.theta);
		scene->camera.orientation.y = cos(scene->camera.phi);
	}
	if (scene->keys_held.down == true)
	{
		if (scene->camera.phi < M_PI - 0.5)
			scene->camera.phi += 0.08;
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
	{
		scene->lights[0].position.x -= 0.8;
	}
	if (scene->keys_held.e == true)
	{
		scene->lights[0].position.x += 0.8;
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
		|| scene->keys_held.e)
	{
		calculate_transforms(scene);
		draw_scene(scene);
	}
	return (0);
}
