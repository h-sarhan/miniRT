/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 19:35:57 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/22 20:42:25 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Function to call when a key is pressed
 * @param key Key that was pressed
 * @param scene The scene struct
 */
int	transform_shape(int key, t_scene *scene)
{
	if (key == KEY_W)
		scene->shapes[0].origin.y += 0.05;
	if (key == KEY_A)
		scene->shapes[0].origin.x -= 0.05;
	if (key == KEY_S)
		scene->shapes[0].origin.y -= 0.05;
	if (key == KEY_D)
		scene->shapes[0].origin.x += 0.05;
	if (key == KEY_PLUS)
		scene->shapes[0].radius += 0.05;
	if (key == KEY_MINUS)
		scene->shapes[0].radius -= 0.05;
	if (key == KEY_A || key == KEY_S || key == KEY_D || key == KEY_PLUS
		|| key == KEY_MINUS || key == KEY_W)
	{
		calculate_transforms(scene);
		draw_scene(scene);
	}
	return (0);
}
