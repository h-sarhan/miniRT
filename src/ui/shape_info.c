/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:32:41 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/19 19:26:33 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	show_light_banner(t_scene *scene)
{
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * .9, 20,
		0xffffff, "LIGHT MODE");
}

void	draw_shape_type(t_scene *scene, t_shape *shape, t_vector *origin_proj)
{
	if (shape->type == SPHERE)
		mlx_string_put(scene->disp->mlx, scene->disp->win,
			(origin_proj->x * scene->settings.disp_w),
			(origin_proj->y - 0.12) * scene->settings.disp_h,
			0xffffff, "Sphere");
	if (shape->type == CYLINDER)
		mlx_string_put(scene->disp->mlx, scene->disp->win,
			(origin_proj->x * scene->settings.disp_w),
			(origin_proj->y - 0.12) * scene->settings.disp_h,
			0xffffff, "Cylinder");
	if (shape->type == CUBE)
		mlx_string_put(scene->disp->mlx, scene->disp->win,
			(origin_proj->x * scene->settings.disp_w),
			(origin_proj->y - 0.12) * scene->settings.disp_h,
			0xffffff, "Cube");
	if (shape->type == CONE)
		mlx_string_put(scene->disp->mlx, scene->disp->win,
			(origin_proj->x * scene->settings.disp_w),
			(origin_proj->y - 0.12) * scene->settings.disp_h,
			0xffffff, "Cone");
}

void	draw_shape_coordinates(t_shape *shape, t_scene *scene,
			t_vector *origin_proj)
{
	char	*str;

	str = ftoa(shape->origin.x);
	str = ft_strjoin_free(" x: ", str, 2);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		(origin_proj->x * scene->settings.disp_w),
		(origin_proj->y - 0.10) * scene->settings.disp_h,
		0xffffff, str);
	free(str);
	str = ftoa(shape->origin.y);
	str = ft_strjoin_free(" y: ", str, 2);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		(origin_proj->x * scene->settings.disp_w),
		(origin_proj->y - 0.08) * scene->settings.disp_h,
		0xffffff, str);
	free(str);
	str = ftoa(shape->origin.z);
	str = ft_strjoin_free(" z: ", str, 2);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		(origin_proj->x * scene->settings.disp_w),
		(origin_proj->y - 0.06) * scene->settings.disp_h,
		0xffffff, str);
	free(str);
}

void	draw_shape_properties(t_shape *shape, t_scene *scene,
			t_vector *origin_proj)
{
	char	*str;

	if (shape->type == SPHERE || shape->type == CONE || shape->type == CYLINDER)
	{
		str = ftoa(shape->props.radius);
		str = ft_strjoin_free(" radius: ", str, 2);
		mlx_string_put(scene->disp->mlx, scene->disp->win,
			(origin_proj->x * scene->settings.disp_w),
			(origin_proj->y - 0.04) * scene->settings.disp_h,
			0xffffff, str);
		free(str);
	}
	if (shape->type == CYLINDER || shape->type == CONE)
	{
		str = ftoa(shape->props.height);
		str = ft_strjoin_free(" height: ", str, 2);
		mlx_string_put(scene->disp->mlx, scene->disp->win,
			(origin_proj->x * scene->settings.disp_w),
			(origin_proj->y - 0.02) * scene->settings.disp_h,
			0xffffff, str);
		free(str);
	}
}

t_vector	get_projected_origin(t_shape *shape, t_scene *scene)
{
	t_vector	origin;
	t_vector	orient;
	t_vector	origin_proj;

	origin = shape->origin;
	origin.x -= 0.2;
	if (shape->type == SPHERE || shape->type == CYLINDER)
		origin.y += shape->props.radius;
	if (shape->type == CONE)
		origin.y += shape->props.height / 2;
	if (shape->type == CUBE)
	{
		mat_vec_multiply(&orient, &shape->added_rots, &shape->orientation);
		origin.y += shape->props.scale.y * fabs(orient.y) + \
			shape->props.scale.x * fabs(orient.x) + shape->props.scale.z * \
			fabs(orient.z);
	}
	mat_vec_multiply(&origin_proj, &scene->cam.transform, &origin);
	return (origin_proj);
}

void	draw_shape_info(t_scene *scene)
{
	int			shape_idx;
	t_shape		*shape;
	t_vector	origin_proj;

	shape_idx = -1;
	if (scene->settings.edit_mode == false)
		return ;
	while (++shape_idx < scene->count.shapes)
	{
		shape = &scene->shapes[shape_idx];
		if (shape->props.highlighted == false)
			continue ;
		origin_proj = get_projected_origin(shape, scene);
		if (origin_proj.z > 0)
			return ;
		perspective_projection(&origin_proj, scene);
		if (shape->type == SPHERE || shape->type == CYLINDER
			|| shape->type == CUBE || shape->type == CONE)
		{
			draw_shape_type(scene, shape, &origin_proj);
			draw_shape_coordinates(shape, scene, &origin_proj);
			draw_shape_properties(shape, scene, &origin_proj);
		}
	}
}
