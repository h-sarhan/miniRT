/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gjk.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 14:23:32 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/05 15:32:50 by hsarhan          ###   ########.fr       */
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


t_vector	box_furthest_point(const t_vector *dir, const t_shape *box)
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
t_vector	cylinder_furthest_point(const t_vector *dir, const t_shape *cyl)
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

// http://uu.diva-portal.org/smash/get/diva2:343820/FULLTEXT01.pdf
t_vector	cone_furthest_point(const t_vector *dir, t_shape *cone)
{
	t_vector	support_point;
	t_vector	w;
	t_vector	cyl_normal;
	t_vector	up_vector;
	
	ft_bzero(&up_vector, sizeof(t_vector));
	up_vector.y = 1;
	mat_vec_multiply(&cyl_normal, &cone->transf, &up_vector);
	normalize_vec(&cyl_normal);
	float	u_d = dot_product(&cyl_normal, dir);
	scale_vec(&w, &cyl_normal, -u_d);
	add_vec(&w, &w, dir);
	t_vector	rhs;
	scale_vec(&rhs, &cyl_normal, sign(dot_product(&cyl_normal, dir)) * cone->props.height / 2);
	add_vec(&support_point, &cone->origin, &rhs);
	scale_vec(&rhs, &cyl_normal, 0);
	add_vec(&support_point, &cone->origin, &rhs);
	if (vec_magnitude(&w) < 0.01)
	{
		support_point.w = 1;
		return (support_point);
	}
	normalize_vec(&w);
	scale_vec(&w, &w, cone->props.radius);
	add_vec(&support_point, &support_point, &w);
	support_point.w = 1;
	return (support_point);
}

t_vector	gjk_support(t_shape *shape1,  t_shape *shape2, const t_vector *dir)
{
	t_vector	s1_furthest;
	t_vector	s2_furthest;
	t_vector	dir2;
	t_vector	res;

	negate_vec(&dir2, dir);
	if (shape1->type == CYLINDER)
		s1_furthest = cylinder_furthest_point(dir, shape1);
	if (shape1->type == CUBE)
		s1_furthest = box_furthest_point(dir, shape1);
	// if (shape1->type == CUBE)
	// 	s1_furthest = cone_furthest_point(dir, shape1);
	if (shape2->type == CYLINDER)
		s2_furthest = cylinder_furthest_point(&dir2, shape2);
	if (shape2->type == CUBE)
		s2_furthest = box_furthest_point(&dir2, shape2);
	// if (shape2->type == CONE)
	// 	s2_furthest = cone_furthest_point(&dir2, shape2);
	sub_vec(&res, &s1_furthest, &s2_furthest);
	return (res);
}

void	add_to_simplex(t_vector *simplex, t_vector *point, int *simplex_size)
{
	if (*simplex_size == 0)
		simplex[0] = *point;
	else if (*simplex_size == 1)
	{
		simplex[1] = simplex[0];
		simplex[0] = *point;
	}
	else if (*simplex_size == 2)
	{
		simplex[2] = simplex[1];
		simplex[1] = simplex[0];
		simplex[0] = *point;
	}
	else if (*simplex_size == 3)
	{
		simplex[3] = simplex[2];
		simplex[2] = simplex[1];
		simplex[1] = simplex[0];
		simplex[0] = *point;
	}
	else
		exit(!printf("This shouldnt happen\n"));
	*simplex_size += 1;
}


t_vector	triple_cross_product(const t_vector *v1, const t_vector *v2, const t_vector *v3)
{
	t_vector	v12;
	t_vector	res;

	cross_product(&v12, v1, v2);
	cross_product(&res, &v12, v3);
	return (res);
}

bool	line_contains_origin(t_vector *simplex, t_vector *dir, int *simplex_size)
{
	t_vector	ab;
	t_vector	ao;

	sub_vec(&ab, &simplex[1], &simplex[0]);
	negate_vec(&ao, &simplex[0]);
	if (dot_product(&ab, &ao) > 0)
	{
		*dir = triple_cross_product(&ab, &ao, &ab);
	}
	else
	{
		*simplex_size = 1;
		*dir = ao;
	}
	normalize_vec(dir);
	return (false);
}

bool	triangle_contains_origin(t_vector *simplex, t_vector *dir, int *simplex_size)
{
	t_vector	ab;
	t_vector	ac;
	t_vector	ao;
	t_vector	abc;
	t_vector	abc_ac;
	t_vector	ab_abc;

	sub_vec(&ab, &simplex[1], &simplex[0]);
	sub_vec(&ac, &simplex[2], &simplex[0]);
	negate_vec(&ao, &simplex[0]);
	
	cross_product(&abc, &ab, &ac);

	cross_product(&abc_ac, &abc, &ac);
	if (dot_product(&abc_ac, &ao) > 0)
	{
		if (dot_product(&ac, &ao) > 0)
		{
			*simplex_size = 2;
			simplex[1] = simplex[2];
			*dir = triple_cross_product(&ac, &ao, &ac);
		}
		else
		{
			*simplex_size = 2;
			return (line_contains_origin(simplex, dir, simplex_size));
		}
	}
	else
	{
		cross_product(&ab_abc, &ab, &abc);
		if (dot_product(&ab_abc, &ao) > 0)
		{
			*simplex_size = 2;
			return (line_contains_origin(simplex, dir, simplex_size));
		}
		else
		{
			if (dot_product(&abc, &ao) > 0)
			{
				*dir = abc;
			}
			else
			{
				t_vector	temp;
				temp = simplex[1];
				simplex[1] = simplex[2];
				simplex[2] = temp;
				*simplex_size = 3;
				*dir = abc;
				negate_vec(dir, &abc);
			}
		}
	}
	normalize_vec(dir);
	return false;
}


bool	tetrahedron_contains_origin(t_vector *simplex, t_vector *dir, int *simplex_size)
{
	t_vector ab; // = b - a;
	t_vector ac; // = c - a;
	t_vector ad; // = d - a;
	t_vector ao; // =   - a;
 
	sub_vec(&ab, &simplex[1], &simplex[0]);
	sub_vec(&ac, &simplex[2], &simplex[0]);
	sub_vec(&ad, &simplex[3], &simplex[0]);
	negate_vec(&ao, &simplex[0]);
	
	t_vector abc; // = ab.cross(ac);
	t_vector acd; // = ac.cross(ad);
	t_vector adb; // = ad.cross(ab);
	cross_product(&abc, &ab, &ac);
	cross_product(&acd, &ab, &ac);
	if (dot_product(&abc, &ao) > 0)
	{
		*simplex_size = 3;
		return (triangle_contains_origin(simplex, dir, simplex_size));
	}
	if (dot_product(&acd, &ao) > 0)
	{
		*simplex_size = 3;
		simplex[1] = simplex[2];
		simplex[2] = simplex[3];
		return (triangle_contains_origin(simplex, dir, simplex_size));
	}

	// if (SameDirection(adb, ao))
	if (dot_product(&adb, &ao) > 0)
	{
		*simplex_size = 3;
		t_vector	temp;
		temp = simplex[1];
		simplex[1] = simplex[3];
		simplex[2] = temp;
		return (triangle_contains_origin(simplex, dir, simplex_size));
	}
	return true;
}

bool	handle_simplex(t_vector *simplex, t_vector *dir, int *simplex_size)
{
	if (*simplex_size == 2)
	{
		return (line_contains_origin(simplex, dir, simplex_size));
	}
	else if (*simplex_size == 3)
	{
		return (triangle_contains_origin(simplex, dir,  simplex_size));
	}
	else if (*simplex_size == 4)
	{
		return (tetrahedron_contains_origin(simplex, dir,  simplex_size));
	}
	else
		exit(!printf("This shouldnt happen\n"));
}

// GJK algorithm based on: https://blog.winter.dev/2020/gjk-algorithm/
bool	gjk(t_shape *s1, t_shape *s2)
{
	t_vector	dir;
	t_vector	simplex[4];
	int			simplex_size;

	simplex_size = 0;
	sub_vec(&dir, &s2->origin, &s1->origin);
	normalize_vec(&dir);
	t_vector	support = gjk_support(s1, s2, &dir);
	add_to_simplex(simplex, &support, &simplex_size);
	negate_vec(&dir, &support);
	normalize_vec(&dir);
	// int	safety = 1000;
	int	i = 0;
	while (1)
	{
		support = gjk_support(s1, s2, &dir);
		if (dot_product(&support, &dir) <= 0)
			return (false);
		add_to_simplex(simplex, &support, &simplex_size);
		if (handle_simplex(simplex, &dir, &simplex_size) == true)
			return (true);
		i++;
	}
	return (false);
}
