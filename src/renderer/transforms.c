/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transforms.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:45:14 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/02 22:12:30 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	calculate_orientation(t_mat4 *rot_transform, t_shape *shape)
{
	t_vector	up;
	t_vector	ax;
	float		angle;

	if (shape->orientation.x == 0 && fabs(shape->orientation.y - 1) < 0.001
		&& shape->orientation.z == 0)
		return ;
	if (shape->orientation.x == 0 && fabs(shape->orientation.y + 1) < 0.001
		&& shape->orientation.z == 0)
	{
		rotation_matrix_x(rot_transform, -M_PI);
		return ;
	}
	up.x = 0;
	up.y = 1;
	up.z = 0;
	up.w = 0;
	ax.w = 0;
	normalize_vec(&shape->orientation);
	normalize_vec(&up);
	cross_product(&ax, &up, &shape->orientation);
	normalize_vec(&ax);
	angle = acos(dot_product(&shape->orientation, &up));
	axis_angle(rot_transform, &ax, angle);
}

void	multiply_transforms(t_shape *shape, t_mat4 *scale, t_mat4 *rot,
	t_mat4 *translate)
{
	t_mat4	temp;

	// if (shape->type == CONE)
	// {
	// 	translate_matrix(&temp, 0, shape->props.height / 2, 0);
	// 	mat_multiply(translate, &temp, translate);
	// }
	mat_multiply(&shape->transf, translate, &shape->added_rots);
	ft_memcpy(&temp, &shape->transf, sizeof(t_mat4));
	mat_multiply(&shape->transf, &temp, rot);
	// if (shape->type == CONE)
	// {
	// 	translate_matrix(&temp, 0, shape->props.height / 2, 0);
	// 	mat_multiply(&shape->transf, &temp, &shape->transf);
	// }
	ft_memcpy(translate, &shape->transf, sizeof(t_mat4));
	mat_multiply(&shape->transf, translate, scale);
	mat_inverse(&shape->inv_transf, &shape->transf);
	ft_memcpy(&shape->norm_transf, &shape->inv_transf, sizeof(t_mat4));
	transpose_matrix(&shape->norm_transf);
}

/**
 * @brief Calculates the transformation matrices for every object in the scene
 * The chosen transformation order is scale -> rotation -> translation
 * @param scene The scene struct containing the objects
 */
void	calculate_transforms(t_scene *scene)
{
	int		i;
	t_mat4	scale;
	t_mat4	rot;
	t_mat4	translate;
	// float	
	calculate_camera_transform(scene);
	i = 0;
	while (i < scene->count.shapes)
	{
		identity_matrix(&scene->shapes[i].transf);
		identity_matrix(&scale);
		identity_matrix(&rot);
		identity_matrix(&translate);
		if (scene->shapes[i].type == CUBE || scene->shapes[i].type == SPHERE)
			scaling_matrix(&scale, scene->shapes[i].props.scale.x,
				scene->shapes[i].props.scale.y, scene->shapes[i].props.scale.z);
		if (scene->shapes[i].type == CYLINDER || scene->shapes[i].type == CONE)
			scaling_matrix(&scale, scene->shapes[i].props.scale.x,
				1, scene->shapes[i].props.scale.z);
		if (scene->shapes[i].type == PLANE
			|| scene->shapes[i].type == CYLINDER
			|| scene->shapes[i].type == CONE)
			calculate_orientation(&rot, &scene->shapes[i]);
		translate_matrix(&translate, scene->shapes[i].origin.x,
			scene->shapes[i].origin.y, scene->shapes[i].origin.z);
		
		multiply_transforms(&scene->shapes[i], &scale, &rot, &translate);
		i++;
	}
}
