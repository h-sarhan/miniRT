/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 19:35:57 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/26 19:43:41 by hsarhan          ###   ########.fr       */
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

int	transform_room(int key, t_scene *scene)
{
	if (key == KEY_W)
		scene->camera.position.y += 0.35;
	if (key == KEY_A)
		scene->camera.position.x -= 0.35;
	if (key == KEY_S)
		scene->camera.position.y -= 0.35;
	if (key == KEY_D)
		scene->camera.position.x += 0.35;
	if (key == KEY_UP)
		scene->lights[0].position.y += 0.5;
	if (key == KEY_DOWN)
		scene->lights[0].position.y -= 0.5;
	if (key == KEY_LEFT)
		scene->lights[0].position.x -= 0.5;
	if (key == KEY_RIGHT)
		scene->lights[0].position.x += 0.5;
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
