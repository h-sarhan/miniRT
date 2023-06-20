/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 22:23:11 by mkhan             #+#    #+#             */
/*   Updated: 2023/02/19 22:23:23 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	move_cam(t_scene *scene)
{
	t_vector	vec_x;
	t_vector		vec_y;
	t_vector	vec_z;

	ft_bzero(&vec_x, sizeof(t_vector));
	ft_bzero(&vec_y, sizeof(t_vector));
	ft_bzero(&vec_z, sizeof(t_vector));
	if (scene->keys_held.w == true)
		sphere_to_xyz(&vec_z, scene->cam.phi, scene->cam.theta, CAM_SPEED);
	if (scene->keys_held.s == true)
		sphere_to_xyz(&vec_z, scene->cam.phi, scene->cam.theta, -CAM_SPEED);
	if (scene->keys_held.a == true)
		sphere_to_xyz(&vec_x, M_PI_2, scene->cam.theta - M_PI_2, -CAM_SPEED);
	if (scene->keys_held.d == true)
		sphere_to_xyz(&vec_x, M_PI_2, scene->cam.theta - M_PI_2, CAM_SPEED);
	if (scene->keys_held.q == true)
		vec_y.y = 0.35;
	if (scene->keys_held.e == true)
		vec_y.y = -0.35;
	if (scene->keys_held.w == true || scene->keys_held.a == true
		|| scene->keys_held.s == true || scene->keys_held.d == true
		|| scene->keys_held.q == true || scene->keys_held.e == true)
	{
		add_vec(&scene->cam.position, &scene->cam.position, &vec_x);
		add_vec(&scene->cam.position, &scene->cam.position, &vec_y);
		add_vec(&scene->cam.position, &scene->cam.position, &vec_z);
	}
}

void	camera_controls(t_scene *scene)
{
	if (scene->keys_held.w || scene->keys_held.a || scene->keys_held.s
		|| scene->keys_held.d || scene->keys_held.q || scene->keys_held.e)
		move_cam(scene);
	if (scene->keys_held.up == true && scene->cam.phi > 0.2)
		scene->cam.phi -= 0.05;
	if (scene->keys_held.down == true && scene->cam.phi < M_PI - 0.2)
		scene->cam.phi += 0.05;
	if (scene->keys_held.left == true)
		scene->cam.theta += 0.10;
	if (scene->keys_held.right == true)
		scene->cam.theta -= 0.10;
	if (scene->keys_held.up || scene->keys_held.left || scene->keys_held.right
		|| scene->keys_held.down)
		sphere_to_xyz(&scene->cam.dir, scene->cam.phi,
			scene->cam.theta, 1);
}

void	move_object_fwd(t_scene *scene, t_shape *shape)
{
	t_vector	offset;

	ft_bzero(&offset, sizeof(t_vector));
	if (scene->keys_held.w)
	{
		sphere_to_xyz(&offset, M_PI / 2, scene->cam.theta, 0.2);
	}
	if (scene->keys_held.s)
	{
		sphere_to_xyz(&offset, M_PI / 2, scene->cam.theta, -0.2);
	}
	add_vec(&shape->origin, &shape->origin, &offset);
}

void	move_object_h(t_scene *scene, t_shape *shape)
{
	t_vector	offset;

	ft_bzero(&offset, sizeof(t_vector));
	if (scene->keys_held.a)
	{
		sphere_to_xyz(&offset, M_PI_2, scene->cam.theta + M_PI_2, 0.2);
	}
	if (scene->keys_held.d)
	{
		sphere_to_xyz(&offset, M_PI_2, scene->cam.theta - M_PI_2, 0.2);
	}
	add_vec(&shape->origin, &shape->origin, &offset);
}

void	move_object_v(t_scene *scene, t_shape *shape)
{
	t_vector	offset;

	ft_bzero(&offset, sizeof(t_vector));
	if (scene->keys_held.q)
	{
		offset.y = 0.2;
	}
	if (scene->keys_held.e)
	{
		offset.y = -0.2;
	}
	add_vec(&shape->origin, &shape->origin, &offset);
}
