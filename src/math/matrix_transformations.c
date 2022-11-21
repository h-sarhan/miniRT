/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_transformations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:41:22 by mkhan             #+#    #+#             */
/*   Updated: 2022/11/21 22:40:16 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <matrix.h>
#include <scene.h>

/**
 * @brief Initialize the translation matrix.
 * @param mat Matrix to be initialized.
 * @param x The value along the x-axis to be translated.
 * @param y The value along the y-axis to be translated.
 * @param z The value along the z-axis to be translated.
 */

void	translate_matrix(t_mat4 *mat, float x, float y, float z)
{
	ft_bzero(mat, 16 * sizeof(float));
	(*mat)[0][0] = 1;
	(*mat)[1][1] = 1;
	(*mat)[2][2] = 1;
	(*mat)[3][3] = 1;
	(*mat)[0][3] = x;
	(*mat)[1][3] = y;
	(*mat)[2][3] = z;
}

/**
 * @brief Initialize the scaling matrix
 * @param mat The matrix to be initialized
 * @param x The value along the x-axis to be scaled.
 * @param y The value along the y-axis to be scaled.
 * @param z The value along the z-axis to be scaled.
 */
void	scaling_matrix(t_mat4 *mat, float x, float y, float z)
{
	ft_bzero(mat, 16 * sizeof(float));
	(*mat)[0][0] = x;
	(*mat)[1][1] = y;
	(*mat)[2][2] = z;
	(*mat)[3][3] = 1;
}

/**
 * @brief Initialize the matrix for rotation along x-axis
 * @param mat The matrix to be initialized
 * @param r angle in radians
 */
void	rotation_matrix_x(t_mat4 *mat, float r)
{
	ft_bzero(mat, 16 * sizeof(float));
	(*mat)[0][0] = 1;
	(*mat)[1][1] = cos(r);
	(*mat)[1][2] = sin(r) * -1;
	(*mat)[2][1] = sin(r);
	(*mat)[2][2] = cos(r);
	(*mat)[3][3] = 1;
}

/**
 * @brief Initialize the matrix for rotation along y-axis
 * @param mat The matrix to be initialized
 * @param r angle in radians
 */
void	rotation_matrix_y(t_mat4 *mat, float r)
{
	ft_bzero(mat, 16 * sizeof(float));
	(*mat)[0][0] = cos(r);
	(*mat)[0][2] = sin(r);
	(*mat)[1][1] = 1;
	(*mat)[2][0] = sin(r) * -1;
	(*mat)[2][2] = cos(r);
	(*mat)[3][3] = 1;
}

/**
 * @brief Initialize the matrix for rotation along z-axis
 * @param mat The matrix to be initialized
 * @param r angle in radians
 */
void	rotation_matrix_z(t_mat4 *mat, float r)
{
	ft_bzero(mat, 16 * sizeof(float));
	(*mat)[0][0] = cos(r);
	(*mat)[0][1] = sin(r) * -1;
	(*mat)[1][0] = sin(r);
	(*mat)[1][1] = cos(r);
	(*mat)[2][2] = 1;
	(*mat)[3][3] = 1;	
}

/**
 * @brief Convert Radians to Degrees
 * @param r The radian value
 * @return float The converted value in degrees
 */
float	rad_to_deg(float r)
{
	return(r * (180 / M_PI));
}

/**
 * @brief Convert Degrees to Radians
 * @param r The degree value
 * @return float The converted value in radians
 */
float	deg_to_rad(float r)
{
	return(r * (M_PI / 180));
}

void	calculate_transforms(t_scene *scene)
{
	unsigned int	i;
	t_mat4			transform;

	i = 0;
	while (i < scene->count.shape_count)
	{
		if (scene->shapes[i].type == SPHERE)
		{
			identity_matrix(&scene->shapes[i].trans);
			translate_matrix(&transform, scene->shapes[i].origin.x,
				scene->shapes[i].origin.y, scene->shapes[i].origin.z);
			// sphere_trans =  translation * sphere_trans
			// ! REVERSE THIS MULTIPLICATION
			mat_multiply(&scene->shapes[i].trans, &scene->shapes[i].trans,
				&transform);
			scaling_matrix(&transform, scene->shapes[i].radius,
				scene->shapes[i].radius, scene->shapes[i].radius);
			// sphere_trans = scaling * sphere_trans
			// ! REVERSE THIS MULTIPLICATION
			mat_multiply(&scene->shapes[i].trans, &scene->shapes[i].trans,
				&transform);
			print_mat4(&scene->shapes[i].trans);
			// rotation is irrelevant for a sphere
			// * Calculate inverse transform here
			mat_inverse(&scene->shapes[i].inv_trans, &scene->shapes[i].trans);
		}
		i++;
	}
}
