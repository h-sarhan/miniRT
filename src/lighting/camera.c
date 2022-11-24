/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 14:33:36 by mkhan             #+#    #+#             */
/*   Updated: 2022/11/24 15:04:39 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	view_transform(t_mat4 *res, t_vector *up, t_vector *to, t_vector *from)
{
	t_vector forward;
	t_vector left;
	t_vector true_up;
	t_vector upn;
	t_mat4	orientation;
	t_mat4	translation;
	
	sub_vec(&forward, to, from);
	normalize_vec(&forward);
	ft_memcpy(&upn, &up, sizeof(t_vector));
	normalize_vec(&upn);
	cross_product(&left, &forward, &upn);
	cross_product(&true_up, &left, &forward);
	ft_bzero(&orientation, sizeof(mat_4));
	orientation[0][0] = left.x;
	orientation[0][1] = left.y;
	orientation[0][2] = left.z;
	orientation[1][0] = true_up.x;
	orientation[1][1] = true_up.y;
	orientation[1][2] = true_up.z;
	orientation[2][0] = forward.x * -1;
	orientation[2][1] = forward.y * -1;
	orientation[2][2] = forward.z * -1;
	orientation[3][3] = 1;
	translate_matrix(&translation, from->x * -1, from->y * -1, from->z * -1);
	mat_multiply(res, &orientation, &translation);
}

void	camera_init(t_camera *camera, t_scene *scene)
{
	float	half_view;
	float	aspect;
	

	half_view = tanf(camera->fov / 2);
	aspect = scene->win_w / scene->win_h;
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
	camera->pixel_size = (camera->half_width * 2) / scene->win_w;
}


