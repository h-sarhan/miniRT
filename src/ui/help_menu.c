/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 12:53:33 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/19 19:51:18 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	show_help_menu(t_scene *scene)
{
	int	y;
	int	x;

	if (scene->help == false)
		return ;
	y = scene->settings.disp_h * 0.1;
	while (y < scene->settings.disp_h * 0.9)
	{
		x = scene->settings.disp_w * 0.1;
		while (x < scene->settings.disp_w * 0.9)
		{
			*(int *)(scene->disp->disp_addr + ((y * scene->settings.disp_w) \
				+ x) * scene->disp->bpp) = 0x00;
			x++;
		}
		y++;
	}
}

void	draw_cell(t_scene *scene, int row_num, int col_num, char *str)
{
	const int	col_gap = scene->settings.disp_w * 0.18;
	const int	row_gap = scene->settings.disp_h * 0.028;

	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * row_num,
		scene->settings.disp_h * 0.23 + row_gap * col_num,
		0xffffff, "- WASDQE: Translate shape"
		);
}

void	draw_controls(t_scene *scene)
{
	const int	col_gap = scene->settings.disp_w * 0.18;
	const int	row_gap = scene->settings.disp_h * 0.028;
	int			i;

	i = 0;
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		(scene->settings.disp_w * 0.12),
		(scene->settings.disp_h * 0.15),
		0xffffff, "[HELP MENU]");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.215 + row_gap * 0,
		0xffffff, "[EDIT MODE]");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.215 + row_gap * 0,
		0xffffff, "[RENDER MODE]");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.215 + row_gap * 0,
		0xffffff, "[CAMERA MODE]");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.215 + row_gap * 0,
		0xffffff, "[LIGHT MODE]");
	draw_cell(scene, 0, 1, "- WASDQE: Translate shape");
	draw_cell(scene, 0, 2, "- +/-: Scale shape");
	draw_cell(scene, 0, 3, "- +/- w/shift: Scale height");
	draw_cell(scene, 0, 4, "- XYZ: Scale cube sides");
	draw_cell(scene, 0, 5, "- Arrow keys: Rotate shape");
	draw_cell(scene, 0, 6, "- Left click: Select shape");
	draw_cell(scene, 0, 7, "- Mouse drag: Rotate shape");
	draw_cell(scene, 0, 8, "- O: Look at");
	draw_cell(scene, 0, 9, "- J: Anti Aliasing");
	draw_cell(scene, 0, 10, "- Return: Spawn shape");
	draw_cell(scene, 0, 11, "- T: Toggle shape");
	draw_cell(scene, 0, 12, "- 1/2 3/4 5/6: Change colors");
	draw_cell(scene, 0, 13, "- Space: Toggle edit mode");
	draw_cell(scene, 0, 14, "- Tab: Switch shapes");
	draw_cell(scene, 0, 15, "- R: Toggle reflections");
	draw_cell(scene, 0, 16, "- L: Toggle light mode");
	draw_cell(scene, 0, 17, "- C: Toggle Camera mode");
	draw_cell(scene, 0, 18, "- H: Toggle help");
	draw_cell(scene, 0, 19, "- ESC: Quit miniRT");
	// col = 1
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 1,
		0xffffff, "- Space: Toggle edit mode");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 2,
		0xffffff, "- ESC: Quit miniRT");
	// col = 2

	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 1,
		0xffffff, "- WASDQE: Move camera");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 2,
		0xffffff, "- Arrow Keys: Orient Camera");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 3,
		0xffffff, "- C: Toggle Camera mode");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 4,
		0xffffff, "- Space: Toggle edit mode");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 5,
		0xffffff, "- ESC: Quit miniRT");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 1,
		0xffffff, "- WASDQE: Move light");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 2,
		0xffffff, "- +/-: Light intensity");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 3,
		0xffffff, "- +/- w/shift: Light beam width");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 4,
		0xffffff, "- TAB: Switch lights");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 5,
		0xffffff, "- C: Toggle Camera mode");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 6,
		0xffffff, "- Space: Toggle edit mode");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 7,
		0xffffff, "- ESC: Quit miniRT");
}
