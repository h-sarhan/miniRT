/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:32:41 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/29 18:44:19 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	show_light_banner(t_scene *scene)
{
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * .9, 20,
		0xffffff, "LIGHT MODE");
}

// NORM LATER
// Planning to change this significantly so I will norm this later
// Will need to write ftoa to avoid using sprintf
void	draw_shape_info(t_scene *scene)
{
	int			shape_idx;
	t_shape		*shape;
	t_vector	origin_proj;
	t_vector	origin;
	char		*str;

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
		if (shape->type == CONE)
			origin.y += shape->props.height / 2;
		if (shape->type == CUBE)
		{
			t_vector current_orientation;
			mat_vec_multiply(&current_orientation, &shape->added_rots, &shape->orientation);
			origin.y += shape->props.scale.y * fabs(current_orientation.y) + shape->props.scale.x * fabs(current_orientation.x) + shape->props.scale.z * fabs(current_orientation.z);
		}
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
			str = ftoa(shape->origin.x);
			str = ft_strjoin_free(" x: ", str, 2);
			mlx_string_put(scene->disp->mlx, scene->disp->win,
				(origin_proj.x * scene->settings.disp_w),
				(origin_proj.y - 0.10) * scene->settings.disp_h,
				0xffffff, str);
			free(str);
			str = ftoa(shape->origin.y);
			str = ft_strjoin_free(" y: ", str, 2);
			mlx_string_put(scene->disp->mlx, scene->disp->win,
				(origin_proj.x * scene->settings.disp_w),
				(origin_proj.y - 0.08) * scene->settings.disp_h,
				0xffffff, str);
			free(str);
			str = ftoa(shape->origin.z);
			str = ft_strjoin_free(" z: ", str, 2);
			mlx_string_put(scene->disp->mlx, scene->disp->win,
				(origin_proj.x * scene->settings.disp_w),
				(origin_proj.y - 0.06) * scene->settings.disp_h,
				0xffffff, str);
			free(str);
			if (shape->type == SPHERE || shape->type == CONE || shape->type == CYLINDER)
			{
				str = ftoa(shape->props.radius);
				str = ft_strjoin_free(" radius: ", str, 2);
				mlx_string_put(scene->disp->mlx, scene->disp->win,
					(origin_proj.x * scene->settings.disp_w),
					(origin_proj.y - 0.04) * scene->settings.disp_h,
					0xffffff, str);
				free(str);
			}
			if (shape->type == CYLINDER || shape->type == CONE)
			{
				str = ftoa(shape->props.height);
				str = ft_strjoin_free(" height: ", str, 2);
				mlx_string_put(scene->disp->mlx, scene->disp->win,
					(origin_proj.x * scene->settings.disp_w),
					(origin_proj.y - 0.02) * scene->settings.disp_h,
					0xffffff, str);
				free(str);
			}
		}
		shape_idx++;
	}
}

// ! Not currently using but could be helpful later
void	dda(t_scene *scene, float x1, float x2, float y1, float y2, int color)
{
	float	dy;
	float	dx;
	float	steps;
	int		i;
	char	*dst;

	dx = (x2 - x1);
	dy = (y2 - y1);
	if (fabs(dx) > fabs(dy))
		steps = fabs(dx);
	else
		steps = fabs(dy);
	i = 0;
	dx /= steps;
	dy /= steps;
	while (i <= steps)
	{
		dst = scene->disp->disp_addr + (int)(y1 * scene->settings.disp_w + x1) * scene->disp->bpp;
		*(unsigned int*)dst = color;
		y1 += dy;
		x1 += dx;
		i++;
	}
}
