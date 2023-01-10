/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrow.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:31:39 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/10 15:42:31 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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

void	draw_arrow(t_scene *scene, int x, int y, float z)
{
	if (z > 0)
	{
		y = scene->settings.disp_h - y;
		x = scene->settings.disp_w - x;
		left_right_arrows(scene, x, y);
		up_down_arrows(scene, x, y);
		bottom_corner_arrows(scene, x, y);
		top_corner_arrows(scene, x, y);
		if (x >= 0 && x < scene->settings.disp_w && y >= 0
			&& y < scene->settings.disp_h)
		{
			y = scene->settings.disp_h;
			up_down_arrows(scene, x, y);
		}
		return ;
	}
	left_right_arrows(scene, x, y);
	up_down_arrows(scene, x, y);
	bottom_corner_arrows(scene, x, y);
}
