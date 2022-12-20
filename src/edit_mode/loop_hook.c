/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 18:50:31 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/20 19:23:53 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vector	*sphere_to_xyz(t_vector *vec, double phi, double theta, double r)
{
	vec->x = r * sin(phi) * cos(theta);
	vec->z = r * sin(phi) * sin(theta);
	vec->y = r * cos(phi);
	vec->w = 0;
	return (vec);
}

void	move_cam(t_scene *scene)
{
	t_vector	vec;

	ft_bzero(&vec, sizeof(t_vector));
	if (scene->keys_held.w == true)
		sphere_to_xyz(&vec, scene->camera.phi, scene->camera.theta, CAM_SPEED);
	if (scene->keys_held.a == true)
		sphere_to_xyz(&vec, M_PI_2, scene->camera.theta - M_PI_2, -CAM_SPEED);
	if (scene->keys_held.s == true)
		sphere_to_xyz(&vec, scene->camera.phi, scene->camera.theta, -CAM_SPEED);
	if (scene->keys_held.d == true)
		sphere_to_xyz(&vec, M_PI_2, scene->camera.theta - M_PI_2, CAM_SPEED);
	if (scene->keys_held.q == true)
		vec.y = 0.35;
	if (scene->keys_held.e == true)
		vec.y = -0.35;
	add_vec(&scene->camera.position, &scene->camera.position, &vec);
}

void	camera_controls(t_scene *scene)
{
	if (scene->keys_held.w || scene->keys_held.a || scene->keys_held.s
		|| scene->keys_held.d || scene->keys_held.q || scene->keys_held.e)
		move_cam(scene);
	if (scene->keys_held.up == true && scene->camera.phi > 0.5)
		scene->camera.phi -= 0.05;
	if (scene->keys_held.down == true && scene->camera.phi < M_PI - 0.5)
		scene->camera.phi += 0.05;
	if (scene->keys_held.left == true)
		scene->camera.theta += 0.10;
	if (scene->keys_held.right == true)
		scene->camera.theta -= 0.10;
	if (scene->keys_held.up || scene->keys_held.left || scene->keys_held.right
		|| scene->keys_held.down)
		sphere_to_xyz(&scene->camera.orientation, scene->camera.phi,
			scene->camera.theta, 1);
}

int	key_handler(t_scene *scene)
{
	if (scene->camera_mode == true && scene->edit_mode == true)
		camera_controls(scene);
	else if (scene->edit_mode == true)
	{
		if (scene->keys_held.w == true)
		{
			scene->shapes[scene->shape_idx].origin.x += 0.2 *sin(scene->camera.phi)*cos(scene->camera.theta);
			scene->shapes[scene->shape_idx].origin.z += 0.2 *sin(scene->camera.phi)*sin(scene->camera.theta);
			t_vector	offset;
			offset.x = -0.0001 *sin(scene->camera.phi)*cos(scene->camera.theta);
			offset.y = 0;
			offset.z = -0.0001 *sin(scene->camera.phi)*sin(scene->camera.theta);
			offset.w = 0;
			while (scene->collisions &&is_colliding(&scene->shapes[scene->shape_idx], scene, &offset, false) == true)
			{
				scene->shapes[scene->shape_idx].origin.x -= 0.0001 *sin(scene->camera.phi)*cos(scene->camera.theta);
				scene->shapes[scene->shape_idx].origin.z -= 0.0001 *sin(scene->camera.phi)*sin(scene->camera.theta);
			}
		}
		if (scene->keys_held.a == true)
		{
			scene->shapes[scene->shape_idx].origin.x += 0.2 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
			scene->shapes[scene->shape_idx].origin.z += 0.2 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
			t_vector	offset;
			offset.x = -0.0001 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
			offset.y = 0;
			offset.z = -0.0001 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
			offset.w = 0;
			while (scene->collisions &&is_colliding(&scene->shapes[scene->shape_idx], scene, &offset, false) == true)
			{
				scene->shapes[scene->shape_idx].origin.x -= 0.0001 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
				scene->shapes[scene->shape_idx].origin.z -= 0.0001 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
			}
		}
		if (scene->keys_held.s == true)
		{
			scene->shapes[scene->shape_idx].origin.x -= 0.2 *sin(scene->camera.phi)*cos(scene->camera.theta);
			scene->shapes[scene->shape_idx].origin.z -= 0.2 *sin(scene->camera.phi)*sin(scene->camera.theta);
			t_vector	offset;
			offset.x = 0.0001 *sin(scene->camera.phi)*cos(scene->camera.theta);
			offset.y = 0;
			offset.z = 0.0001 *sin(scene->camera.phi)*sin(scene->camera.theta);
			offset.w = 0;
			while (scene->collisions &&is_colliding(&scene->shapes[scene->shape_idx], scene, &offset, false) == true)
			{
				scene->shapes[scene->shape_idx].origin.x += 0.0001 *sin(scene->camera.phi)*cos(scene->camera.theta);
				scene->shapes[scene->shape_idx].origin.z += 0.0001 *sin(scene->camera.phi)*sin(scene->camera.theta);
			}
		}
		if (scene->keys_held.d == true)
		{
			scene->shapes[scene->shape_idx].origin.x -= 0.2 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
			scene->shapes[scene->shape_idx].origin.z -= 0.2 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
			t_vector	offset;
			
			offset.x = 0.0001 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
			offset.y = 0;
			offset.z = 0.0001 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
			offset.w = 0;
			while (scene->collisions && is_colliding(&scene->shapes[scene->shape_idx], scene, &offset, false) == true)
			{
				scene->shapes[scene->shape_idx].origin.x += 0.0001 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
				scene->shapes[scene->shape_idx].origin.z += 0.0001 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
			}
		}
		if (scene->keys_held.plus == true)
		{
			scene->shapes[scene->shape_idx].radius += 0.04;
			t_vector	offset;
			ft_bzero(&offset, sizeof(t_vector));
			offset.x = 0.04;
			while (scene->collisions &&is_colliding(&scene->shapes[scene->shape_idx], scene, &offset, true) == true)
			{
				scene->shapes[scene->shape_idx].radius -= 0.002;
			}

		}
		if (scene->keys_held.minus == true)
		{
			if (scene->shapes[scene->shape_idx].radius > 0.3)
			{
				scene->shapes[scene->shape_idx].radius -= 0.04;
				// t_vector	offset;
				// ft_bzero(&offset, sizeof(t_vector));
				// while (is_colliding(&scene->shapes[scene->shape_idx], scene, &offset) == true)
				// {
				// 	scene->shapes[scene->shape_idx].radius += 0.002;
				// }
			}
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
		{
			scene->shapes[scene->shape_idx].origin.y += 0.1;
			t_vector	offset;
			ft_bzero(&offset, sizeof(t_vector));
			offset.y = -0.01;
			while (scene->collisions &&is_colliding(&scene->shapes[scene->shape_idx], scene, &offset, false) == true)
			{
				scene->shapes[scene->shape_idx].origin.y -= 0.01;
			}
		}
		if (scene->keys_held.e == true)
		{
			scene->shapes[scene->shape_idx].origin.y -= 0.1;
			t_vector	offset;
			ft_bzero(&offset, sizeof(t_vector));
			offset.y = 0.01;
			while (scene->collisions &&is_colliding(&scene->shapes[scene->shape_idx], scene, &offset, false) == true)
			{
				scene->shapes[scene->shape_idx].origin.y += 0.01;
			}

		}
	}
	if (scene->look_at.trigger == true && scene->edit_mode == true)
	{
		if (scene->look_at.step_num != scene->look_at.step_amount)
		{
			scene->camera.position.x += scene->look_at.pos_diff.x / scene->look_at.step_amount;
			scene->camera.position.y += scene->look_at.pos_diff.y / scene->look_at.step_amount;
			scene->camera.position.z += scene->look_at.pos_diff.z / scene->look_at.step_amount;
		}
		double x_diff = scene->look_at.current_dir.x - scene->look_at.final_dir.x;
		double y_diff = scene->look_at.current_dir.y - scene->look_at.final_dir.y;
		double z_diff = scene->look_at.current_dir.z - scene->look_at.final_dir.z;
		if (fabs(x_diff) > 0.01)
		{
			if (x_diff < 0)
			{
				scene->look_at.current_dir.x += 0.04;
				if (fabs(x_diff) < 0.1)
					scene->look_at.current_dir.x = scene->look_at.final_dir.x;
			}
			if (x_diff > 0)
			{
				scene->look_at.current_dir.x -= 0.04;
				if (fabs(x_diff) < 0.1)
					scene->look_at.current_dir.x = scene->look_at.final_dir.x;
			}
		}
		if (fabs(y_diff) > 0.01)
		{
			if (y_diff < 0)
			{
				scene->look_at.current_dir.y += 0.04;
				if (fabs(y_diff) < 0.1)
					scene->look_at.current_dir.y = scene->look_at.final_dir.y;
			}
			if (y_diff > 0)
			{
				scene->look_at.current_dir.y -= 0.04;
				if (fabs(y_diff) < 0.1)
					scene->look_at.current_dir.y = scene->look_at.final_dir.y;
			}
		}
		if (fabs(z_diff) > 0.01)
		{
			if (z_diff < 0)
			{
				scene->look_at.current_dir.z += 0.04;
				if (fabs(z_diff) < 0.1)
					scene->look_at.current_dir.z = scene->look_at.final_dir.z;
			}
			if (z_diff > 0)
			{
				scene->look_at.current_dir.z -= 0.04;
				if (fabs(z_diff) < 0.1)
					scene->look_at.current_dir.z = scene->look_at.final_dir.z;
			}
		}
		scene->camera.orientation = scene->look_at.current_dir;
		calculate_transforms(scene);
		draw_scene(scene);
		x_diff = scene->look_at.current_dir.x - scene->look_at.final_dir.x;
		y_diff = scene->look_at.current_dir.y - scene->look_at.final_dir.y;
		z_diff = scene->look_at.current_dir.z - scene->look_at.final_dir.z;
		if (scene->look_at.step_num < scene->look_at.step_amount)
			scene->look_at.step_num++;
		if (scene->look_at.step_num == scene->look_at.step_amount && fabs(x_diff) < 0.01 && fabs(y_diff) < 0.01 && fabs(z_diff) < 0.01)
		{
			if (scene->camera.orientation.x > 0)
			{
				scene->camera.theta = atan(scene->camera.orientation.z / scene->camera.orientation.x);
			}
			else if (scene->camera.orientation.x < 0 && scene->camera.orientation.z >= 0)
			{
				scene->camera.theta = atan(scene->camera.orientation.z / scene->camera.orientation.x) + M_PI;
			}
			else if (scene->camera.orientation.x < 0 && scene->camera.orientation.z < 0)
			{
				scene->camera.theta = atan(scene->camera.orientation.z / scene->camera.orientation.x) - M_PI;
			}
			scene->camera.phi = acos(scene->camera.orientation.y);
			scene->look_at.trigger = false;
			scene->look_at.step_num = 0;
		}
	}
	if (scene->mouse.active == false && scene->edit_mode == true
		&& (scene->keys_held.w || scene->keys_held.a || scene->keys_held.s
		|| scene->keys_held.d
		|| scene->keys_held.up
		|| scene->keys_held.down
		|| scene->keys_held.left
		|| scene->keys_held.right
		|| scene->keys_held.q
		|| scene->keys_held.e
		|| scene->keys_held.plus
		|| scene->keys_held.minus)
		)
	{
		calculate_transforms(scene);
		draw_scene(scene);
	}
	return (0);
}
