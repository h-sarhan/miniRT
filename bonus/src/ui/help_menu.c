/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 12:53:33 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/19 20:34:42 by hsarhan          ###   ########.fr       */
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
		0xffffff, str
		);
}

void	draw_help_header(t_scene *scene)
{
	const int	col_gap = scene->settings.disp_w * 0.18;
	const int	row_gap = scene->settings.disp_h * 0.028;

	mlx_string_put(scene->disp->mlx, scene->disp->win,
		(scene->settings.disp_w * 0.12),
		(scene->settings.disp_h * 0.15),
		0xffffff, "[HELP MENU]");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * 0,
		scene->settings.disp_h * 0.215 + row_gap * 0,
		0xffffff, "[EDIT MODE]");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * 1,
		scene->settings.disp_h * 0.215 + row_gap * 0,
		0xffffff, "[RENDER MODE]");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * 2,
		scene->settings.disp_h * 0.215 + row_gap * 0,
		0xffffff, "[CAMERA MODE]");
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * 3,
		scene->settings.disp_h * 0.215 + row_gap * 0,
		0xffffff, "[LIGHT MODE]");
}

void	draw_help_col1(t_scene *scene)
{
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
}

void	draw_controls(t_scene *scene)
{
	draw_help_header(scene);
	draw_help_col1(scene);
	draw_cell(scene, 1, 1, "- Space: Toggle edit mode");
	draw_cell(scene, 1, 2, "- ESC: Quit miniRT");
	draw_cell(scene, 2, 1, "- WASDQE: Move camera");
	draw_cell(scene, 2, 2, "- Arrow Keys: Orient Camera");
	draw_cell(scene, 2, 3, "- C: Toggle Camera mode");
	draw_cell(scene, 2, 4, "- Space: Toggle edit mode");
	draw_cell(scene, 2, 5, "- ESC: Quit miniRT");
	draw_cell(scene, 3, 1, "- WASDQE: Move light");
	draw_cell(scene, 3, 2, "- +/-: Light intensity");
	draw_cell(scene, 3, 3, "- +/- w/shift: Light beam width");
	draw_cell(scene, 3, 4, "- TAB: Switch lights");
	draw_cell(scene, 3, 5, "- C: Toggle Camera mode");
	draw_cell(scene, 3, 6, "- TAB: Switch lights");
	draw_cell(scene, 3, 7, "- Space: Toggle edit mode");
	draw_cell(scene, 3, 8, "- ESC: Quit miniRT");
}
