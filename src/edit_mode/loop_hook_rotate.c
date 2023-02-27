/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook_rotate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 22:24:04 by mkhan             #+#    #+#             */
/*   Updated: 2023/02/27 13:06:03 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	rotate_x(t_scene *scene, t_mat4 *rot_mat, double deg)
{
	t_mat4		rot;
	t_vector	ax;
	t_vector	up;
	t_mat4		mat_copy;

	up.x = 0;
	up.y = 1;
	up.z = 0;
	up.w = 0;
	cross_product(&ax, &up, &scene->cam.dir);
	normalize_vec(&ax);
	if (scene->keys_held.down == true)
		axis_angle(&rot, &ax, -deg);
	else
		axis_angle(&rot, &ax, deg);
	ft_memcpy(&mat_copy, rot_mat, sizeof(t_mat4));
	mat_multiply(rot_mat, &rot, &mat_copy);
}

void	rotate_y(t_scene *scene, t_mat4 *rot_mat, double deg)
{
	t_mat4	rot;
	t_mat4	mat_copy;

	if (scene->keys_held.left == true)
		rotation_matrix_y(&rot, deg);
	else
		rotation_matrix_y(&rot, -deg);
	ft_memcpy(&mat_copy, rot_mat, sizeof(t_mat4));
	mat_multiply(rot_mat, &rot, &mat_copy);
}

void	rotate_z(t_scene *scene, t_mat4 *rot_mat, double deg)
{
	t_mat4	rot;
	t_mat4	mat_copy;

	if (scene->keys_held.left == true)
		axis_angle(&rot, &scene->cam.dir, deg);
	else
		axis_angle(&rot, &scene->cam.dir, -deg);
	ft_memcpy(&mat_copy, rot_mat, sizeof(t_mat4));
	mat_multiply(rot_mat, &rot, &mat_copy);
}

void	rotate_object(t_scene *scene)
{
	if (scene->keys_held.shift == false
		&& (scene->keys_held.left == true || scene->keys_held.right == true))
		rotate_y(scene, &scene->shapes[scene->shape_idx].added_rots,
			DEG_TO_RAD * 5);
	if (scene->keys_held.shift == true
		&& (scene->keys_held.left == true || scene->keys_held.right == true))
		rotate_z(scene, &scene->shapes[scene->shape_idx].added_rots,
			DEG_TO_RAD * 5);
	if (scene->keys_held.up == true || scene->keys_held.down == true)
		rotate_x(scene, &scene->shapes[scene->shape_idx].added_rots,
			DEG_TO_RAD * 5);
}

void	transform_object(t_scene *scene)
{
	if (scene->keys_held.w == true || scene->keys_held.s == true)
		move_object_fwd(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.a == true || scene->keys_held.d == true)
		move_object_h(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.q == true || scene->keys_held.e == true)
		move_object_v(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.shift == false
		&& (scene->keys_held.plus || scene->keys_held.minus))
		scale_object(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.shift == true
		&& (scene->keys_held.plus == true || scene->keys_held.minus == true))
		change_height(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.shift == false
		&& (scene->keys_held.x || scene->keys_held.y || scene->keys_held.z))
		scale_cube_sides(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.shift == true
		&& (scene->keys_held.x || scene->keys_held.y || scene->keys_held.z))
		scale_cube_sides(scene, &scene->shapes[scene->shape_idx]);
	rotate_object(scene);
	collide_after_transform(scene);
}
