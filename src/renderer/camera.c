/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:39:44 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/20 18:55:51 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	calculate_camera_transform(t_scene *scene)
{
	t_vector		from;
	t_vector		up;

	from.x = scene->cam.position.x;
	from.y = scene->cam.position.y;
	from.z = scene->cam.position.z;
	from.w = 1;
	normalize_vec(&scene->cam.dir);
	up.x = 0;
	up.y = 1;
	up.z = 0;
	up.w = 0;
	view_transform(&scene->cam.transform, &from, &up,
		&scene->cam.dir);
	mat_inverse(&scene->cam.inv_trans, &scene->cam.transform);
}

void	view_transform(t_mat4 *res, const t_vector *from, const t_vector *up,
	const t_vector *forward)
{
	t_vector	left;
	t_vector	true_up;
	t_vector	upn;
	t_mat4		orientation;
	t_mat4		translation;

	upn = *up;
	normalize_vec(&upn);
	cross_product(&left, forward, &upn);
	normalize_vec(&left);
	cross_product(&true_up, &left, forward);
	normalize_vec(&true_up);
	ft_bzero(&orientation, sizeof(t_mat4));
	orientation[0][0] = left.x;
	orientation[0][1] = left.y;
	orientation[0][2] = left.z;
	orientation[1][0] = true_up.x;
	orientation[1][1] = true_up.y;
	orientation[1][2] = true_up.z;
	orientation[2][0] = forward->x * -1;
	orientation[2][1] = forward->y * -1;
	orientation[2][2] = forward->z * -1;
	orientation[3][3] = 1;
	translate_matrix(&translation, from->x * -1, from->y * -1, from->z * -1);
	mat_multiply(res, &orientation, &translation);
}

void	camera_init(t_camera *camera, t_scene *scene)
{
	double	half_view;
	double	aspect;
	double	h;
	double	w;

	h = scene->settings.render_h;
	w = scene->settings.render_w;
	if (scene->settings.edit_mode == true)
	{
		h = scene->settings.edit_h;
		w = scene->settings.edit_w;
	}
	half_view = tan((camera->fov / 2.0f) * M_PI / 180.0f);
	aspect = w / h;
	if (aspect >= 1)
	{
		camera->half_width = half_view;
		camera->half_height = half_view / aspect;
	}
	else
	{
		camera->half_width = half_view * aspect;
		camera->half_height = half_view;
	}
	camera->pixel_size = (camera->half_width * 2) / w;
}
