/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gjk.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 14:23:32 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/01 18:19:04 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

extern t_vector *point_to_draw_1;
extern t_vector *point_to_draw_2;

int	sign(float num)
{
	if (num < 0)
		return (-1);
	return (1);
}


t_vector	box_support_function(const t_vector *dir, const t_shape *box)
{
	t_vector	vtx[8];
	t_vector	transf_vtx[8];
	ft_bzero(vtx, sizeof(t_vector) * 8);
	vtx[0].x =  + 1;
	vtx[0].y =  + 1;
	vtx[0].z =  + 1;
	vtx[0].w = 1;
	
	vtx[1].x =  + 1;
	vtx[1].y =  + 1;
	vtx[1].z =  - 1;
	vtx[1].w = 1;

	vtx[2].x =  + 1;
	vtx[2].y =  - 1;
	vtx[2].z =  + 1;
	vtx[2].w = 1;
	
	vtx[3].x =  + 1;
	vtx[3].y =  - 1;
	vtx[3].z =  - 1;
	vtx[3].w = 1;
	
	vtx[4].x =  - 1;
	vtx[4].y =  + 1;
	vtx[4].z =  + 1;
	vtx[4].w = 1;
	
	vtx[5].x =  - 1;
	vtx[5].y =  - 1;
	vtx[5].z =  + 1;
	vtx[5].w = 1;
	
	vtx[6].x =  - 1;
	vtx[6].y =  - 1;
	vtx[6].z =  - 1;
	vtx[6].w = 1;
	
	vtx[7].x =  - 1;
	vtx[7].y =  + 1;
	vtx[7].z =  - 1;
	vtx[7].w = 1;
	
	for (int i = 0; i < 8; i++)
		mat_vec_multiply(&transf_vtx[i], &box->transf, &vtx[i]);
	
	int	i = 0;
	float	largest = - INFINITY;
	int	largest_idx = 0;
	float	res;
	while (i < 8)
	{
		res = dot_product(dir, &transf_vtx[i]);
		if (res > largest)
		{
			largest = res;
			largest_idx = i;
		}
		i++;
	}
	return (transf_vtx[largest_idx]);
}

// from: https://www.cas.mcmaster.ca/~carette/SE3GB3/2006/notes/gjk1_pres.pdf
t_vector	cylinder_support_function(const t_vector *dir, const t_shape *cyl)
{
	t_vector	support_point;
	t_vector	w;
	t_vector	cyl_normal;
	t_vector	up_vector;
	
	ft_bzero(&up_vector, sizeof(t_vector));
	up_vector.y = 1;
	mat_vec_multiply(&cyl_normal, &cyl->transf, &up_vector);
	normalize_vec(&cyl_normal);
	float	u_d = dot_product(&cyl_normal, dir);
	scale_vec(&w, &cyl_normal, -u_d);
	add_vec(&w, &w, dir);
	t_vector	rhs;
	scale_vec(&rhs, &cyl_normal, sign(dot_product(&cyl_normal, dir)) * cyl->props.height / 2);
	add_vec(&support_point, &cyl->origin, &rhs);
	if (vec_magnitude(&w) < 0.01)
	{
		support_point.w = 1;
		return (support_point);
	}
	normalize_vec(&w);
	scale_vec(&w, &w, cyl->props.radius);
	add_vec(&support_point, &support_point, &w);
	support_point.w = 1;
	return (support_point);
}
