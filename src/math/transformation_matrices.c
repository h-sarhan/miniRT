/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_transformations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:41:22 by mkhan             #+#    #+#             */
/*   Updated: 2023/01/02 17:46:16 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Initialize the translation matrix.
 * @param mat Matrix to be initialized.
 * @param x The value along the x-axis to be translated.
 * @param y The value along the y-axis to be translated.
 * @param z The value along the z-axis to be translated.
 */
void	translate_matrix(t_mat4 *mat, float x, float y, float z)
{
	ft_bzero(mat, sizeof(t_mat4));
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
	ft_bzero(mat, sizeof(t_mat4));
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
	ft_bzero(mat, sizeof(t_mat4));
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
	ft_bzero(mat, sizeof(t_mat4));
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
	ft_bzero(mat, sizeof(t_mat4));
	(*mat)[0][0] = cos(r);
	(*mat)[0][1] = sin(r) * -1;
	(*mat)[1][0] = sin(r);
	(*mat)[1][1] = cos(r);
	(*mat)[2][2] = 1;
	(*mat)[3][3] = 1;
}

void	axis_angle(t_mat4 *rot_mat, const t_vector *ax, float angle)
{
	(*rot_mat)[0][0] = cos(angle) + ax->x * ax->x * (1 - cos(angle));
	(*rot_mat)[0][1] = ax->x * ax->y * (1 - cos(angle)) - ax->z * sin(angle);
	(*rot_mat)[0][2] = ax->x * ax->z * (1 - cos(angle)) + ax->y * sin(angle);
	(*rot_mat)[0][3] = 0;
	(*rot_mat)[1][0] = ax->x * ax->y * (1 - cos(angle)) + ax->z * sin(angle);
	(*rot_mat)[1][1] = cos(angle) + ax->y * ax->y * (1 - cos(angle));
	(*rot_mat)[1][2] = ax->y * ax->z * (1 - cos(angle)) - ax->x * sin(angle);
	(*rot_mat)[1][3] = 0;
	(*rot_mat)[2][0] = ax->z * ax->x * (1 - cos(angle)) - ax->y * sin(angle);
	(*rot_mat)[2][1] = ax->z * ax->y * (1 - cos(angle)) + ax->x * sin(angle);
	(*rot_mat)[2][2] = cos(angle) + ax->z * ax->z * (1 - cos(angle));
	(*rot_mat)[2][3] = 0;
	(*rot_mat)[3][0] = 0;
	(*rot_mat)[3][1] = 0;
	(*rot_mat)[3][2] = 0;
	(*rot_mat)[3][3] = 1;
}
