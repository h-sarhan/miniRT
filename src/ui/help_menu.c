/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 12:53:33 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/18 15:47:45 by hsarhan          ###   ########.fr       */
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
			*(int *)(scene->disp->disp_addr + ((y * scene->settings.disp_w) + x) * scene->disp->bpp) = 0x00;
			x++;
		}
		y++;
	}
}

void	draw_controls(t_scene *scene)
{
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		(scene->settings.disp_w * 0.12),
		(scene->settings.disp_h * 0.15),
		0xffffff, "[HELP MENU]");
	int i = 0;
	int	col_gap = scene->settings.disp_w * 0.18;
	int	row_gap = scene->settings.disp_h * 0.028;
	i = 0;
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.215 + row_gap * 0,
		0xffffff, "[EDIT MODE]"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 1,
		0xffffff, "- WASDQE: Translate shape"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 2,
		0xffffff, "- +/-: Scale shape"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 3,
		0xffffff, "- +/- w/shift: Scale height"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 4,
		0xffffff, "- XYZ: Scale cube sides"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 5,
		0xffffff, "- Arrow keys: Rotate shape"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 6,
		0xffffff, "- Left click: Select shape"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 7,
		0xffffff, "- Mouse drag: Rotate shape"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 8,
		0xffffff, "- O: Look at"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 9,
		0xffffff, "- J: Anti Aliasing"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 10,
		0xffffff, "- Return: Spawn shape"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 11,
		0xffffff, "- T: Toggle shape"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 12,
		0xffffff, "- 1/2 3/4 5/6: Change colors"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 13,
		0xffffff, "- Space: Toggle edit mode"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 14,
		0xffffff, "- Tab: Switch shapes"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 15,
		0xffffff, "- R: Toggle reflections"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 16,
		0xffffff, "- L: Toggle light mode"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 17,
		0xffffff, "- C: Toggle Camera mode"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 18,
		0xffffff, "- H: Toggle help"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 19,
		0xffffff, "- ESC: Quit miniRT"
	);
	i++;
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.215 + row_gap * 0,
		0xffffff, "[RENDER MODE]"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 1,
		0xffffff, "- Space: Toggle edit mode"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 2,
		0xffffff, "- ESC: Quit miniRT"
	);
	i++;
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.215 + row_gap * 0,
		0xffffff, "[CAMERA MODE]"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 1,
		0xffffff, "- WASDQE: Move camera"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 2,
		0xffffff, "- Arrow Keys: Orient Camera"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 3,
		0xffffff, "- C: Toggle Camera mode"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 4,
		0xffffff, "- Space: Toggle edit mode"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 5,
		0xffffff, "- ESC: Quit miniRT"
	);
	i++;
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.215 + row_gap * 0,
		0xffffff, "[LIGHT MODE]"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 1,
		0xffffff, "- WASDQE: Move light"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 2,
		0xffffff, "- +/-: Light intensity"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 3,
		0xffffff, "- +/- w/shift: Light beam width"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 4,
		0xffffff, "- TAB: Switch lights"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 5,
		0xffffff, "- C: Toggle Camera mode"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 6,
		0xffffff, "- Space: Toggle edit mode"
	);
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * 0.15 + col_gap * i,
		scene->settings.disp_h * 0.23 + row_gap * 7,
		0xffffff, "- ESC: Quit miniRT"
	);
}
