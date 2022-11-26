/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:01:06 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/26 16:28:33 by hsarhan          ###   ########.fr       */
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
	t_scene	*scene;
	int		fd;
	t_mlx	mlx;

	fd = open_file(argc, argv);
	if (fd == -1)
		return (EXIT_FAILURE);
	scene = parse_scene(fd);
	close(fd);
	if (scene == NULL)
		return (EXIT_FAILURE);
	calculate_transforms(scene);

	print_scene(scene);
	scene->win_w = 700;
	scene->win_h = 700;
	mlx.mlx = mlx_init();
	mlx.mlx_win = mlx_new_window(mlx.mlx, scene->win_w, scene->win_h, "Hello world!");
	mlx.img = mlx_new_image(mlx.mlx, scene->win_w, scene->win_h);
	mlx.addr = mlx_get_data_addr(mlx.img, &mlx.bytes_per_pixel, &mlx.line_length,
								&mlx.endian);
	mlx.bytes_per_pixel /= 8;
	scene->mlx = &mlx;
	mlx_hook(mlx.mlx_win, 2, (1L << 0), transform_shape, scene);

	draw_scene(scene);
	// run_lighting_tests(scene);
	// ! Put this somewhere
	// free_scene(scene);
	mlx_loop(mlx.mlx);
	return (EXIT_SUCCESS);
}
