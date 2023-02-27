/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 10:17:37 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/05 17:38:10 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// ! THIS FILE WILL NOT BE SUBMITTED SO NORM ISNT AN ISSUE

/**
 * @brief Print the red, green, and blue values of a color
 * @param color Color to print
 */
void	print_color(const t_color *color)
{
	printf("\tR: %f\n", color->r);
	printf("\tG: %f\n", color->g);
	printf("\tB: %f\n", color->b);
}

/**
 * @brief Prints the x, y, z values of a vector
 * @param vector Vector to be printed
 */
void	print_vector(const t_vector *vector)
{
	printf("\tX: %.2f\n", vector->x);
	printf("\tY: %.2f\n", vector->y);
	printf("\tZ: %.2f\n", vector->z);
}

/**
 * @brief Prints the contents of a scene
 * @param scene Scene to be printed
 */
void	print_scene(const t_scene *scene)
{
	int	i;

	printf("Ambient light configuration:\n");
	printf("  Intensity: %.2f\n", scene->ambient.intensity);
	printf("  Color:\n");
	print_color(&scene->ambient.color);
	printf("Camera configuration:\n");
	printf("  Position:\n");
	print_vector(&scene->cam.position);
	printf("  Orientation:\n");
	print_vector(&scene->cam.dir);
	printf("  Fov:\n");
	printf("\t%f degrees\n", scene->cam.fov);
	i = 0;
	while (i < scene->count.lights)
	{
		printf("Light #%d configuration:\n", i + 1);
		printf("  Position:\n");
		print_vector(&scene->lights[i].position);
		printf("  Light intensity:\n");
		printf("\t%.2f intenseness\n", scene->lights[i].intensity);
		printf("  Light color:\n");
		print_color(&scene->lights[i].color);
		i++;
	}
	i = 0;
	while (i < scene->count.shapes)
	{
		printf("Shape #%d configuration:\n", i + 1);
		printf("  Type:\n");
		if (scene->shapes[i].type == SPHERE)
			printf("\tSphere\n");
		else if (scene->shapes[i].type == PLANE)
			printf("\tPlane\n");
		else if (scene->shapes[i].type == CYLINDER)
			printf("\tCylinder\n");
		printf("  Position:\n");
		print_vector(&scene->shapes[i].origin);
		if (scene->shapes[i].type == SPHERE ||
			scene->shapes[i].type == CYLINDER)
		{
			printf("  Radius:\n");
			printf("\t%.2f\n", scene->shapes[i].props.radius);
		}
		if (scene->shapes[i].type == CYLINDER)
		{
			printf("  Height:\n");
			printf("\t%.2f\n", scene->shapes[i].props.height);
		}
		if (scene->shapes[i].type == CYLINDER || scene->shapes[i].type == PLANE)
		{
			printf("  Orientation:\n");
			print_vector(&scene->shapes[i].orientation);
		}
		printf("  Color:\n");
		print_color(&scene->shapes[i].props.color);
		i++;
	}
}

void	print_mat4(const t_mat4 *mat)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%.3f ", (*mat)[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void	print_mat3(const t_mat3 *mat)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("%.1f ", (*mat)[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void	print_mat2(const t_mat2 *mat)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			printf("%.1f ", (*mat)[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
