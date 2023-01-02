/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 12:48:35 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/02 16:18:24 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	draw_marker(t_scene *scene, int x, int y, int color)
{
	char	*dst;
	int		width;

	width = scene->settings.disp_w;
	if (x <= 0 || y <= 0 || x >= width || y >= scene->settings.disp_h)
		return ;
	dst = scene->disp->disp_addr + (y * width + x) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + ((y + 1) * width + x) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + ((y - 1) * width + x) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + (y * width + x + 1) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + ((y + 1) * width + x + 1) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + ((y - 1) * width + x + 1) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + (y * width + x - 1) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + ((y - 1) * width + x - 1) * scene->disp->bpp;
	*(unsigned int *)dst = color;
	dst = scene->disp->disp_addr + ((y + 1) * width + x - 1) * scene->disp->bpp;
	*(unsigned int *)dst = color;
}

void	left_right_arrows(t_scene *scene, int x, int y)
{
	void	*img;
	int		w;
	int		h;

	img = NULL;
	if (x <= 0 && y > 0 && y < scene->settings.disp_h)
	{
		img = mlx_xpm_file_to_image(scene->disp->mlx,
				"./assets/left_arrow.xpm", &w, &h);
		x = 0;
		y = y - h / 2;
	}
	else if (x >= scene->settings.disp_w && y > 0 && y < scene->settings.disp_h)
	{
		img = mlx_xpm_file_to_image(scene->disp->mlx,
				"./assets/right_arrow.xpm", &w, &h);
		x = scene->settings.disp_w - 25;
		y = y - h / 2;
	}
	if (img != NULL)
	{
		mlx_put_image_to_window(scene->disp->mlx, scene->disp->win, img, x, y);
		mlx_destroy_image(scene->disp->mlx, img);
	}
}

void	up_down_arrows(t_scene *scene, int x, int y)
{
	void	*img;
	int		w;
	int		h;

	img = NULL;
	if (y <= 0 && x >= 0 && x < scene->settings.disp_w)
	{
		img = mlx_xpm_file_to_image(scene->disp->mlx,
				"./assets/up_arrow.xpm", &w, &h);
		x = x - w / 2;
		y = 0;
	}
	else if (y >= scene->settings.disp_h && x > 0 && x < scene->settings.disp_w)
	{
		img = mlx_xpm_file_to_image(scene->disp->mlx,
				"./assets/down_arrow.xpm", &w, &h);
		x = x - w / 2;
		y = scene->settings.disp_h - 25;
	}
	if (img != NULL)
	{
		mlx_put_image_to_window(scene->disp->mlx, scene->disp->win, img, x, y);
		mlx_destroy_image(scene->disp->mlx, img);
	}
}

void	bottom_corner_arrows(t_scene *scene, int x, int y)
{
	void	*img;
	int		w;
	int		h;

	img = NULL;
	if (x <= 0 && y >= scene->settings.disp_h)
	{
		img = mlx_xpm_file_to_image(scene->disp->mlx,
				"./assets/bottom_left.xpm", &w, &h);
		x = 0;
		y = scene->settings.disp_h - 35;
	}
	else if (x >= scene->settings.disp_w && y >= scene->settings.disp_h)
	{
		img = mlx_xpm_file_to_image(scene->disp->mlx,
				"./assets/bottom_right.xpm", &w, &h);
		x = scene->settings.disp_w - 35;
		y = scene->settings.disp_h - 35;
	}
	if (img != NULL)
	{
		mlx_put_image_to_window(scene->disp->mlx, scene->disp->win, img, x, y);
		mlx_destroy_image(scene->disp->mlx, img);
	}
}

void	top_corner_arrows(t_scene *scene, int x, int y)
{
	void	*img;
	int		w;
	int		h;

	img = NULL;
	if (x >= scene->settings.disp_w && y <= 0)
	{
		img = mlx_xpm_file_to_image(scene->disp->mlx,
				"./assets/top_right.xpm", &w, &h);
		x = scene->settings.disp_w - 35;
		y = 0;
	}
	else if (x <= 0 && y <= 0)
	{
		img = mlx_xpm_file_to_image(scene->disp->mlx,
				"./assets/top_left.xpm", &w, &h);
		x = 0;
		y = 0;
	}
	if (img != NULL)
	{
		mlx_put_image_to_window(scene->disp->mlx, scene->disp->win, img, x, y);
		mlx_destroy_image(scene->disp->mlx, img);
	}
}

void	draw_arrow(t_scene *scene, int x, int y)
{
	left_right_arrows(scene, x, y);
	up_down_arrows(scene, x, y);
	bottom_corner_arrows(scene, x, y);
	top_corner_arrows(scene, x, y);
}

void	perspective_projection(t_vector *point, const t_scene *scene)
{
	point->x /= -point->z;
	point->y /= -point->z;
	point->x = (point->x + scene->camera.half_width) \
		/ (scene->camera.half_width * 2);
	point->y = (point->y + scene->camera.half_height) \
		/ (scene->camera.half_height * 2);
	point->x = 1 - point->x;
	point->y = 1 - point->y;
}

// NORM LATER
// Planning to change this significantly so I will norm this later
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
		if (shape->type == SPHERE)
		{
			origin.x -= 0.2;
			origin.y += shape->props.radius;
		}
		if (shape->type == CYLINDER)
		{
			origin.x -= 0.2;
			origin.y += shape->props.height / 2;
		}
		mat_vec_multiply(&origin_proj, &scene->camera.transform, &origin);
		if (origin_proj.z > 0)
			return ;
		perspective_projection(&origin_proj, scene);
		if (shape->type == SPHERE || shape->type == CYLINDER
			|| shape->type == CUBE)
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

void	project_marker_on_screen(t_scene *scene, t_shape *shape)
{
	t_vector	origin_proj;

	mat_vec_multiply(&origin_proj, &scene->camera.transform, &shape->origin);
	perspective_projection(&origin_proj, scene);
	if (shape->type == SPHERE || shape->type == CYLINDER || shape->type == CUBE
		|| shape->type == CONE)
	{
		if (origin_proj.z < 0)
			draw_marker(scene, origin_proj.x * scene->settings.disp_w,
				origin_proj.y * scene->settings.disp_h, 0x00ffff);
		mlx_put_image_to_window(scene->disp->mlx, scene->disp->win,
			scene->disp->display_img, 0, 0);
		draw_arrow(scene, origin_proj.x * scene->settings.disp_w,
			origin_proj.y * scene->settings.disp_h);
	}
}

void	draw_shape_marker(t_scene *scene)
{
	int			shape_idx;
	t_shape		*shape;

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
		project_marker_on_screen(scene, shape);
		shape_idx++;
	}
}
