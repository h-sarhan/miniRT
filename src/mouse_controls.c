/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_controls.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 10:20:14 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/20 21:05:14 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// int	handle_mouse_down(int key_code, int x, int y, t_scene *scene)
// {
// 	scene->mouse_key = key_code;
// 	return (0);
// }

// int	handle_mouse_up(int key_code, int x, int y, t_scene *scene)
// {

// 	(void)x;
// 	(void)y;
// 	(void)key_code;
// 	scene->mouse_key = 0;
// 	return (0);
// }

int	mouse_rotate(t_scene *scene)
{
	double sensitivity = 0.06;
	if (scene->mouse.toggle == true && scene->edit_mode == true)
	{
		scene->mouse.prev_x = scene->mouse.x;
		scene->mouse.prev_y = scene->mouse.y;
		// mlx_mos
		mlx_mouse_get_pos(scene->mlx->mlx_win, &scene->mouse.x, &scene->mouse.y);
		printf("x = %d\n", scene->mouse.x);
		printf("y = %d\n", scene->mouse.y);
		if (scene->mouse.x != scene->mouse.prev_x || scene->mouse.y != scene->mouse.prev_y)
		{
			scene->mouse.active = true;
			if (scene->mouse.prev_x < scene->mouse.x)
			{
				scene->camera.theta -= sensitivity;
			}
			else if (scene->mouse.prev_x > scene->mouse.x)
			{
				scene->camera.theta += sensitivity;
			}
			if (scene->mouse.prev_y < scene->mouse.y)
			{
				if (scene->camera.phi < M_PI - 0.5)
					scene->camera.phi += 0.05;
			}
			else if (scene->mouse.prev_y > scene->mouse.y)
			{
				if (scene->camera.phi > 0.5)
					scene->camera.phi -= 0.05;
			}
			scene->camera.dir.x = sin(scene->camera.phi) * cos(scene->camera.theta);
			scene->camera.dir.z = sin(scene->camera.phi) * sin(scene->camera.theta);
			scene->camera.dir.y = cos(scene->camera.phi);
			calculate_transforms(scene);
			draw_scene(scene);
			scene->mouse.active = false;
		}
	}
	return (0);
}