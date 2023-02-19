/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 15:04:57 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/19 15:40:11 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	get_tb(t_vector *normal, t_vector *tb)
{
	t_vector	up;
	t_vector	z_up;

	ft_bzero(&up, sizeof(t_vector));
	ft_bzero(&z_up, sizeof(t_vector));
	up.y = 1;
	z_up.z = 1;
	cross_product(&tb[0], normal, &up);
	if (vec_magnitude(&tb[0]) < 0.000001)
		cross_product(&tb[0], normal, &z_up);
	normalize_vec(&tb[0]);
	cross_product(&tb[1], normal, &tb[0]);
	normalize_vec(&tb[1]);
}

t_vector	normal_map(t_vector *normal, const t_shape *shape,
				const t_vector *itx_point)
{
	t_vector		tb[2];
	const t_vector	tex_normal = normal_from_texture(shape, itx_point);
	t_mat4			tbn;
	t_vector		final_normal;
	t_vector		world_normal;

	get_tb(normal, tb);
	ft_bzero(&tbn, sizeof(t_mat4));
	tbn[0][0] = tb[0].x;
	tbn[0][1] = tb[1].x;
	tbn[0][2] = normal->x;
	tbn[1][0] = tb[0].y;
	tbn[1][1] = tb[1].y;
	tbn[1][2] = normal->y;
	tbn[2][0] = tb[0].z;
	tbn[2][1] = tb[1].z;
	tbn[2][2] = normal->z;
	tbn[3][3] = 1;
	mat_vec_multiply(&final_normal, &tbn, &tex_normal);
	normalize_vec(&final_normal);
	mat_vec_multiply(&world_normal, &shape->norm_transf, &final_normal);
	world_normal.w = 0;
	normalize_vec(&world_normal);
	return (world_normal);
}
