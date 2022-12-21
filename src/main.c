/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:01:06 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/21 22:21:47 by hsarhan          ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	t_scene		*scene;
	int			fd;

	fd = open_file(argc, argv);
	if (fd == -1)
		return (EXIT_FAILURE);
	scene = parse_scene(fd);
	close(fd);
	if (scene == NULL)
		return (EXIT_FAILURE);
	scene->render_scale = 1;
	scene->edit_scale = 0.25;
	scene->render_w = 1920 * scene->render_scale;
	scene->render_h = 1080 * scene->render_scale;
	scene->edit_w = 1920 * scene->edit_scale;
	scene->edit_h = 1080 * scene->edit_scale;
	scene->display_w = 1920 * 0.5;
	scene->display_h = 1080 * 0.5;
	scene->reflection_depth = REFLECTION_DEPTH;
	
	scene->collisions = true;
	scene->shapes[0].highlighted = true;
	scene->shapes[0].transparency = 1;
	scene->shapes[0].reflectiveness = 1;
	scene->shapes[0].ior = 1.5;
	// scene->shapes[1].transparency = 1;
	// scene->shapes[1].reflectiveness = 0.0;
	// scene->shapes[1].ior = 1.5;
	// scene->refraction_depth = 5;
	scene->reflection_depth = 1;
	
	
	sem_unlink("/loading");
	scene->sem_loading = sem_open("/loading", O_CREAT, 0644, 0);
	t_mlx		mlx;
	mlx.mlx = mlx_init();
	mlx.mlx_win = mlx_new_window(mlx.mlx, scene->display_w, scene->display_h, "MiniRT");
	mlx.render_img = mlx_new_image(mlx.mlx, scene->render_w, scene->render_h);
	mlx.edit_img = mlx_new_image(mlx.mlx, 1920 * 0.75, 1080 * 0.75);
	mlx.display_img = mlx_new_image(mlx.mlx, scene->display_w, scene->display_h);
	mlx.render_addr = mlx_get_data_addr(mlx.render_img, &mlx.bytes_per_pixel,
		&mlx.line_length,&mlx.endian);
	mlx.display_addr = mlx_get_data_addr(mlx.display_img, &mlx.bytes_per_pixel,
		&mlx.line_length,&mlx.endian);
	mlx.edit_addr = mlx_get_data_addr(mlx.edit_img, &mlx.bytes_per_pixel,
		&mlx.line_length, &mlx.endian);
	mlx.info_img = mlx_new_image(mlx.mlx, scene->display_w * 0.16, scene->display_h);
	mlx.info_addr = mlx_get_data_addr(mlx.info_img, &mlx.bytes_per_pixel,
		&mlx.line_length, &mlx.endian);
	mlx.bytes_per_pixel /= 8;
	scene->mlx = &mlx;
	mlx_hook(mlx.mlx_win, 2, (1L << 0), set_key_down, scene);
	mlx_hook(mlx.mlx_win, 3, (1L << 1), set_key_up, scene);
	mlx_loop_hook(mlx.mlx, key_handler, scene);
	camera_init(&scene->camera, scene);
	scene->camera.theta = atan(scene->camera.dir.z / scene->camera.dir.x);
	scene->camera.phi = acos(scene->camera.dir.y);
	calculate_transforms(scene);
	draw_scene(scene);
	mlx_loop(mlx.mlx);
	// // ! Put this somewhere
	// // free_scene(scene);
	return (EXIT_SUCCESS);
}
