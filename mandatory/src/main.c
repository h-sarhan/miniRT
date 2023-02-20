/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:01:06 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/20 07:55:07 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Opens a file and checks for errors with the arguments provided to
 * the executable
 * @param argc The number of arguments provided to the executable
 * @param argv The arguments provided to the executable
 * @return The file descriptor of the file passed to the executable or -1 on 
 * error
 */
static int	open_file(int argc, char **argv)
{
	char	*file_name;
	int		fd;

	if (argc != 2)
	{
		printf("Please give me a .rt scene file\n");
		return (-1);
	}
	file_name = argv[1];
	if (ft_strnstr(file_name, ".rt", ft_strlen(file_name)) == NULL
		|| ft_strncmp(&file_name[ft_strlen(file_name) - 3], ".rt", 3) != 0)
	{
		printf(RED"Can only read .rt files\n"RESET);
		return (-1);
	}
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		printf(RED"Could not open file \"%s\"\n"RESET, file_name);
		return (-1);
	}
	return (fd);
}

void	init_display(t_display *disp, t_settings *settings)
{
	disp->mlx = mlx_init();
	if (disp->mlx == NULL)
		return ;
	disp->win = mlx_new_window(disp->mlx, settings->disp_w,
			settings->disp_h, "MiniRT");
	disp->display_img = mlx_new_image(disp->mlx, settings->disp_w,
			settings->disp_h);
	disp->disp_addr = mlx_get_data_addr(disp->display_img,
			&disp->bpp, &disp->line_length, &disp->endian);
	disp->bpp /= 8;
}

void	init_settings(t_settings *settings)
{
	settings->disp_w = 1920 * 0.8;
	settings->disp_h = 1080 * 0.8;
	settings->reflection_depth = REFLECTION_DEPTH;
}

int	close_window(t_scene *scene)
{
	printf("QUITTING PROGRAM!\n");
	free_scene(scene);
	exit(EXIT_SUCCESS);
	return (0);
}

int	key_press(int key, t_scene *scene)
{
	if (key == KEY_ESC)
		return (close_window(scene));
	return (0);
}

void	setup_hooks(t_scene *scene)
{
	int	i;

	i = 0;
	while (i < scene->count.shapes && scene->shapes[i].type == PLANE)
		i++;
	if (scene->shapes[i].type != PLANE)
	{
		scene->shapes[i].props.highlighted = true;
		scene->shape_idx = i;
	}
	if (i == scene->count.shapes)
	{
		scene->shapes[0].props.highlighted = false;
		scene->shape_idx = -1;
	}
	mlx_hook(scene->disp->win, 2, (1L << 0), key_press, scene);
	mlx_hook(scene->disp->win, 17, 0, close_window, scene);
}

int	main(int argc, char **argv)
{
	t_scene		*scene;
	t_display	disp;
	int			fd;

	fd = open_file(argc, argv);
	if (fd == -1)
		return (EXIT_FAILURE);
	scene = parse_scene(fd);
	if (scene == NULL)
		return (free_scene(scene), EXIT_FAILURE);
	close(fd);
	init_settings(&scene->settings);
	init_display(&disp, &scene->settings);
	if (disp.mlx == NULL)
		return (free_scene(scene), EXIT_FAILURE);
	scene->disp = &disp;
	setup_hooks(scene);
	camera_init(&scene->cam, scene);
	scene->cam.theta = atan(scene->cam.dir.z / scene->cam.dir.x);
	scene->cam.phi = acos(scene->cam.dir.y);
	calculate_transforms(scene);
	draw_scene(scene);
	mlx_loop(disp.mlx);
}
