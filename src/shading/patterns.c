/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patterns.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 13:23:32 by mkhan             #+#    #+#             */
/*   Updated: 2023/01/23 13:52:25 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"


t_color	get_texture_color(t_intersection *itx)
{
	t_vector	shape_point;
	double		u;
	double		v;

	mat_vec_multiply(&shape_point, &itx->shape->inv_transf, &itx->over_point);
	if (shape_point.x > 1 || shape_point.y > 1 || shape_point.x < -1 || shape_point.y < -1)
		return (itx->shape->props.color);
	shape_point.x += 1;
	shape_point.y += 1;
	shape_point.x /= 2;
	shape_point.y /= 2;
	if (shape_point.x < 0|| shape_point.y < 0)
		return (itx->shape->props.color);
	// shape_point.y = 1 - shape_point.y;
	u = (int)(shape_point.x * (itx->shape->tex_height - 1));
	v = (int)(shape_point.y * (itx->shape->tex_width - 1));
	if (u >= itx->shape->tex_height || v >= itx->shape->tex_width)
		return (itx->shape->props.color);
	return (itx->shape->diffuse_tex[(int)u][(int)v]);
}

t_color	get_texture_color2(t_intersection *itx)
{
	t_vector	shape_point;
	double		u;
	double		v;

	mat_vec_multiply(&shape_point, &itx->shape->inv_transf, &itx->over_point);
	// shape_point.y = 1 - shape_point.y;
	if (itx->shape->type == CYLINDER || itx->shape->type == CONE)
	{
		shape_point.x /= itx->shape->props.height;
		shape_point.y /= itx->shape->props.height;
		shape_point.y -= 0.5;
		cylindrical_map(&u, &v, &shape_point);
	}
	else if (itx->shape->type ==  SPHERE)
	{
		if (shape_point.x > 1 || shape_point.y > 1 || shape_point.x < -1 || shape_point.y < -1)
			return (itx->shape->props.color);
		spherical_map(&u, &v, &shape_point);
	}
	else //(itx->shape->type == CUBE)
	{
		// if (shape_point.x > 1 || shape_point.y > 1 || shape_point.x < -1 || shape_point.y < -1)
		// 	return (itx->shape->props.color);
		cubical_map(&u, &v, &shape_point);
	}
	// shape_point.x += 1;
	// shape_point.y += 1;
	// shape_point.x /= 2;
	// shape_point.y /= 2;
	
	if (u < 0|| v < 0)
		return (itx->shape->props.color);
	if (itx->shape->tex_tile != 0)
	{
		u = (int)floor(u * (itx->shape->tex_height - 1) * itx->shape->tex_tile) % itx->shape->tex_height;
		v = (int)floor(v * (itx->shape->tex_width - 1) * itx->shape->tex_tile) % itx->shape->tex_width;
	}
	else
	{
		u = (int)floor(u * (itx->shape->tex_height - 1));
		v = (int)floor(v * (itx->shape->tex_width - 1));
	}
	if (u >= itx->shape->tex_height || v >= itx->shape->tex_width)
		return (itx->shape->props.color);
	
	return (itx->shape->diffuse_tex[(int)u][(int)v]);
}

t_color	get_shape_color(t_intersection *itx)
{
	if (itx->shape->diffuse_tex != NULL)
		return (get_texture_color2(itx));
	if (itx->shape->props.pattern_type == NONE)
		return (itx->shape->props.color);
	if (itx->shape->props.pattern_type == STRIPE)
		return (stripe_pattern(itx, itx->over_point, int_to_color(0xffffff),
				int_to_color(0xff0000)));
	else if (itx->shape->props.pattern_type == CHECKER_BOARD)
		return (checker_pattern(itx, &itx->over_point));
	else if (itx->shape->props.pattern_type == GRADIENT)
		return (gradient_pattern(itx, itx->over_point, int_to_color(0xff0000),
				int_to_color(0x0000ff)));
	else if (itx->shape->props.pattern_type == RING)
		return (ring_pattern(itx, itx->over_point, int_to_color(0xff0000),
				int_to_color(0x0000ff)));
	return (itx->shape->props.color);
	
}

t_color	stripe_pattern(t_intersection *itx, t_vector point, t_color a, t_color b)
{
	t_vector	transf_point;
	t_mat4		pattern_transf;

	scaling_matrix(&pattern_transf, itx->shape->props.scale.x *4., itx->shape->props.scale.y *4., itx->shape->props.scale.z*4.0);
	mat_vec_multiply(&transf_point, &itx->shape->inv_transf, &point);
	mat_vec_multiply(&transf_point, &pattern_transf, &transf_point);
	transf_point.x += 0.5;
	transf_point.y += 0.5;
	transf_point.z += 0.5;
	if ((int) floorf(transf_point.x) % 2 == 0)
		return (b);
	return (a);
}

t_color	ring_pattern(t_intersection *itx, t_vector point, t_color a, t_color b)
{
	t_vector	transf_point;
	t_mat4		pattern_transf;
	
	scaling_matrix(&pattern_transf, itx->shape->props.scale.x *4., itx->shape->props.scale.y *4., itx->shape->props.scale.z*4.0);
	mat_vec_multiply(&transf_point, &itx->shape->inv_transf, &point);
	mat_vec_multiply(&transf_point, &pattern_transf, &transf_point);
	if ((int) floorf(sqrtf(((transf_point.x * transf_point.x) + (transf_point.z * transf_point.z)))) % 2 == 0)
		return (b);
	return (a);
}

t_color	gradient_pattern(t_intersection *itx, t_vector point, t_color a, t_color b)
{
	t_color		color;
	double		fraction;
	t_vector	transf_point;
	t_mat4		pattern_transf;
	
	scaling_matrix(&pattern_transf, 0.5, 0.5, 0.5);
	mat_vec_multiply(&transf_point, &itx->shape->inv_transf, &point);
	mat_vec_multiply(&transf_point, &pattern_transf, &transf_point);
	transf_point.x += 0.5;
	transf_point.y += 0.5;
	transf_point.z += 0.5;
	sub_colors(&color, &b, &a);
	fraction = transf_point.x - floorf(transf_point.x);
	mult_color(&color, &color, fraction);	
	add_colors(&color, &color, &a);
	return (color);
}

void	cube_map_right(double *u, double *v, t_vector *point)
{
	*u = (fmod((1 - point->z), 2.0)) / 2.0;
	*v = (fmod((point->y + 1), 2.0)) / 2.0;
}

void	cube_map_left(double *u, double *v, t_vector *point)
{
	*u = (fmod((point->z + 1), 2.0)) / 2.0;
	*v = (fmod((point->y + 1), 2.0)) / 2.0;
}

void	cube_map_up(double *u, double *v, t_vector *point)
{
	*u = (fmod((1 - point->x), 2.0)) / 2.0;
	*v = (fmod((1- point->z), 2.0)) / 2.0;
}
void	cube_map_down(double *u, double *v, t_vector *point)
{
	*u = (fmod((1 - point->x), 2.0)) / 2.0;
	*v = (fmod((point->z + 1), 2.0)) / 2.0;
}
void	cube_map_front(double *u, double *v, t_vector *point)
{
	*u = (fmod((point->x + 1), 2.0)) / 2.0;
	*v = (fmod((point->y + 1), 2.0)) / 2.0;
}
void	cube_map_back(double *u, double *v, t_vector *point)
{
	*u = (fmod((1 - point->x), 2.0)) / 2.0;
	*v = (fmod((point->y + 1), 2.0)) / 2.0;
}

void	cubical_map(double *u, double *v, t_vector *point)
{
	double	abs_x;
	double	abs_y;
	double	abs_z;
	double	coord;

	abs_x = fabs(point->x);
	abs_y = fabs(point->y);
	abs_z = fabs(point->z);
	coord = max3(abs_x, abs_y, abs_z);
	if (coord == point->x)
		cube_map_right(u, v, point);
	else if (coord == -point->x)
		cube_map_left(u, v, point);
	else if (coord == point->y)
		cube_map_up(u, v, point);
	else if (coord == -point->y)
		cube_map_down(u, v, point);
	else if (coord == point->z)
		cube_map_front(u, v, point);
	else if (coord == -point->z)
		cube_map_back(u, v, point);
}

void	spherical_map(double *u, double *v, t_vector *point)
{
	double		theta;
	t_vector	vec;
	double		radius;
	double		phi;

	vec = *point;
	vec.w = 0;
	theta = atan2(point->x, point->z);
	radius = vec_magnitude(&vec);
	phi = acos(point->y / radius);
	*u = 1 - ((theta / (2 * M_PI)) + 0.5);
	*v = 1 - (phi / M_PI);
}

void	cylindrical_map(double *u, double *v, t_vector *point)
{
	double	theta;

	theta = atan2(point->x, point->z);
	*u = 1 - (theta / (2 * M_PI) + 0.5);
	*v = point->y - floor(point->y);
}

t_color	checker_pattern(t_intersection *itx, t_vector *point)
{
	double		u2;
	double		v2;
	double		u;
	double		v;
	t_vector	transf_point;

	mat_vec_multiply(&transf_point, &itx->shape->inv_transf, point);
	if (itx->shape->type == CYLINDER || itx->shape->type == CONE)
		cylindrical_map(&u, &v, &transf_point);
	else
		spherical_map(&u, &v, &transf_point);
	u2 = floor(u * 40);
	v2 = floor(v * 20);
	if ((int)(u2 + v2) % 2 == 0)
		return (int_to_color(0x0));
	return (int_to_color(0xffffff));
}
