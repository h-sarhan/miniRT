/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:32:41 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/09 07:00:01 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// NORM LATER
// Planning to change this significantly so I will norm this later
// Will need to write ftoa to avoid using sprintf
void	draw_shape_info(t_scene *scene)
{
	int			shape_idx;
	t_shape		*shape;
	t_vector	origin_proj;
	t_vector	origin;
	char		str[1000];

	shape_idx = 0;
	if (scene->settings.edit_mode == false)
		return ;
	while (shape_idx < scene->count.shapes)
	{
		shape = &scene->shapes[shape_idx];
		if (shape->props.highlighted == false)
		{
			shape_idx++;
			continue ;
		}
		origin = shape->origin;
		origin.x -= 0.2;
		if (shape->type == SPHERE || shape->type == CYLINDER)
			origin.y += shape->props.radius;
		if (shape->type == CUBE)
			origin.y += shape->props.scale.y;
		if (shape->type == CONE)
			origin.y += shape->props.height /2;
		mat_vec_multiply(&origin_proj, &scene->cam.transform, &origin);
		if (origin_proj.z > 0)
			return ;
		perspective_projection(&origin_proj, scene);
		if (shape->type == SPHERE || shape->type == CYLINDER
			|| shape->type == CUBE || shape->type == CONE)
		{
			if (shape->type == SPHERE)
				mlx_string_put(scene->disp->mlx, scene->disp->win,
					(origin_proj.x * scene->settings.disp_w),
					(origin_proj.y - 0.12) * scene->settings.disp_h,
					0xffffff, "Sphere");
			if (shape->type == CYLINDER)
				mlx_string_put(scene->disp->mlx, scene->disp->win,
					(origin_proj.x * scene->settings.disp_w),
					(origin_proj.y - 0.12) * scene->settings.disp_h,
					0xffffff, "Cylinder");
			if (shape->type == CUBE)
				mlx_string_put(scene->disp->mlx, scene->disp->win,
					(origin_proj.x * scene->settings.disp_w),
					(origin_proj.y - 0.12) * scene->settings.disp_h,
					0xffffff, "Cube");
			if (shape->type == CONE)
				mlx_string_put(scene->disp->mlx, scene->disp->win,
					(origin_proj.x * scene->settings.disp_w),
					(origin_proj.y - 0.12) * scene->settings.disp_h,
					0xffffff, "Cone");
			sprintf(str, "x: % 9.2f", shape->origin.x);
			mlx_string_put(scene->disp->mlx, scene->disp->win,
				(origin_proj.x * scene->settings.disp_w),
				(origin_proj.y - 0.10) * scene->settings.disp_h,
				0xffffff, str);
			sprintf(str, "y: % 9.2f", shape->origin.y);
			mlx_string_put(scene->disp->mlx, scene->disp->win,
				(origin_proj.x * scene->settings.disp_w),
				(origin_proj.y - 0.08) * scene->settings.disp_h,
				0xffffff, str);
			sprintf(str, "z: % 9.2f", shape->origin.z);
			mlx_string_put(scene->disp->mlx, scene->disp->win,
				(origin_proj.x * scene->settings.disp_w),
				(origin_proj.y - 0.06) * scene->settings.disp_h,
				0xffffff, str);
			sprintf(str, "radius: %.2f", shape->props.radius);
			mlx_string_put(scene->disp->mlx, scene->disp->win,
				(origin_proj.x * scene->settings.disp_w),
				(origin_proj.y - 0.04) * scene->settings.disp_h,
				0xffffff, str);
		}
		shape_idx++;
	}
}

// ! Not currently using but could be helpful later
// void	dda(t_scene *scene, float x1, float x2, float y1, float y2, int color)
// {
// 	float	dy;
// 	float	dx;
// 	float	steps;
// 	int		i;
// 	char	*dst;

// 	dx = (x2 - x1);
// 	dy = (y2 - y1);
// 	if (fabs(dx) > fabs(dy))
// 		steps = fabs(dx);
// 	else
// 		steps = fabs(dy);
// 	i = 0;
// 	dx /= steps;
// 	dy /= steps;
// 	while (i <= steps)
// 	{
// dst = scene->disp->disp_addr + (int)(y1 * scene->settings.disp_w + x1) 
// * scene->disp->bpp;
// 		*(unsigned int*)dst = color;
// 		y1 += dy;
// 		x1 += dx;
// 		i++;
// 	}
// }
