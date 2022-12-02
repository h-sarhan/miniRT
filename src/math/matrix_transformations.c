/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_transformations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:41:22 by mkhan             #+#    #+#             */
/*   Updated: 2022/12/02 23:08:42 by hsarhan          ###   ########.fr       */
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

void	translate_matrix(t_mat4 *mat, double x, double y, double z)
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
void	scaling_matrix(t_mat4 *mat, double x, double y, double z)
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
void	rotation_matrix_x(t_mat4 *mat, double r)
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
void	rotation_matrix_y(t_mat4 *mat, double r)
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
void	rotation_matrix_z(t_mat4 *mat, double r)
{
	ft_bzero(mat, sizeof(t_mat4));
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
 * @return double The converted value in degrees
 */
double	rad_to_deg(double r)
{
	return(r * (180 / M_PI));
}

/**
 * @brief Convert Degrees to Radians
 * @param r The degree value
 * @return double The converted value in radians
 */
double	deg_to_rad(double r)
{
	return(r * (M_PI / 180));
}


// void	setup_room(t_scene *scene)
// {
// 	t_mat4	transform1;
// 	t_mat4	transform2;
// 	t_mat4	transform_res;

// 	scene->shapes[0].specular = 0.8;
// 	identity_matrix(&scene->shapes[0].transf);

// 	scaling_matrix(&transform1, 10, 0.01, 10);
// 	mat_multiply(&scene->shapes[0].transf, &transform1, &scene->shapes[0].transf);

// 	// translate_matrix(&transform, scene->shapes[i].origin.x, scene->shapes[i].origin.y, scene->shapes[i].origin.z);

// 	// sphere_trans =  translation * sphere_trans
// 	// mat_multiply(&scene->shapes[i].transf, &transform, &scene->shapes[i].transf);

// 	mat_inverse(&scene->shapes[0].inv_transf, &scene->shapes[0].transf);
// 	ft_memcpy(&scene->shapes[0].norm_transf, &scene->shapes[0].inv_transf, sizeof(t_mat4));
// 	transpose_matrix(&scene->shapes[0].norm_transf);
	
// 	scene->shapes[1].specular = 0.8;
// 	identity_matrix(&scene->shapes[1].transf);
// 	translate_matrix(&transform1, 0, 0, 5);
// 	rotation_matrix_y(&transform2, -M_PI_4);
// 	mat_multiply(&transform_res, &transform1, &transform2);
// 	ft_memcpy(&transform1, &transform_res, sizeof(t_mat4));
// 	rotation_matrix_x(&transform2, M_PI_2);
// 	mat_multiply(&transform_res, &transform1, &transform2);
// 	ft_memcpy(&transform1, &transform_res, sizeof(t_mat4));
// 	scaling_matrix(&transform2, 10, 0.01, 10);
// 	mat_multiply(&scene->shapes[1].transf, &transform1, &transform2);
	
// 	mat_inverse(&scene->shapes[1].inv_transf, &scene->shapes[1].transf);
// 	ft_memcpy(&scene->shapes[1].norm_transf, &scene->shapes[1].inv_transf, sizeof(t_mat4));
// 	transpose_matrix(&scene->shapes[1].norm_transf);

// 	scene->shapes[2].specular = 0;
// 	identity_matrix(&scene->shapes[2].transf);
// 	translate_matrix(&transform1, 0, 0, 5);
// 	rotation_matrix_y(&transform2, M_PI_4);
// 	mat_multiply(&transform_res, &transform1, &transform2);
// 	ft_memcpy(&transform1, &transform_res, sizeof(t_mat4));
// 	rotation_matrix_x(&transform2, M_PI_2);
// 	mat_multiply(&transform_res, &transform1, &transform2);
// 	ft_memcpy(&transform1, &transform_res, sizeof(t_mat4));
// 	scaling_matrix(&transform2, 10, 0.01, 10);
// 	mat_multiply(&scene->shapes[2].transf, &transform1, &transform2);
	
// 	mat_inverse(&scene->shapes[2].inv_transf, &scene->shapes[2].transf);
// 	ft_memcpy(&scene->shapes[2].norm_transf, &scene->shapes[2].inv_transf, sizeof(t_mat4));
// 	transpose_matrix(&scene->shapes[2].norm_transf);
	
// 	scene->shapes[3].diffuse = 0.7;
// 	scene->shapes[3].specular = 0.6;
// 	scene->shapes[4].diffuse = 0.7;
// 	scene->shapes[4].specular = 0.3;
// 	scene->shapes[5].diffuse = 0.7;
// 	scene->shapes[5].specular = 0.3;
// }

void	axis_angle(t_mat4 *rot_mat, const t_vector *ax, double angle)
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

/**
 * @brief Calculates the transformation matrices for every object in the scene
 * The chosen transformation order is scale -> rotation -> translation
 * @param scene The scene struct containing the objects
 */
void	calculate_transforms(t_scene *scene)
{
	unsigned int	i;
	t_mat4			scale_transform;
	t_mat4			rot_transform;
	t_mat4			translate_transform;
	// t_mat4			temp;
	t_vector		from;
	// t_vector		to;
	t_vector		up;

	from.x = scene->camera.position.x;
	from.y = scene->camera.position.y;
	from.z = scene->camera.position.z;
	from.w = 1;
	normalize_vec(&scene->camera.orientation);
	// to.x = scene->camera.orientation.x;
	// to.y = scene->camera.orientation.y;
	// to.z = scene->camera.orientation.z;
	// to.w = 1;

	up.x = 0;
	up.y = 1;
	up.z = 0;
	up.w = 0;

	view_transform(&scene->camera.transform, &from, &up, &scene->camera.orientation);
	mat_inverse(&scene->camera.inv_trans, &scene->camera.transform);
	i = 0;
	while (i < scene->count.shape_count)
	{
		identity_matrix(&scene->shapes[i].transf);
		identity_matrix(&scale_transform);
		identity_matrix(&rot_transform);
		identity_matrix(&translate_transform);
		// SCALE FIRST
		if (scene->shapes[i].type == SPHERE)
		{
			scaling_matrix(&scale_transform, scene->shapes[i].radius, scene->shapes[i].radius, scene->shapes[i].radius);
		}

		if (scene->shapes[i].type == PLANE)
		{
			// ROTATE HERE
			t_vector ax;
			ax.w = 0;

			up.x = 0;
			up.y = 0;
			up.z = 1;
			normalize_vec(&scene->shapes[i].orientation);
			cross_product(&ax, &scene->shapes[i].orientation, &up);
			double angle = acos(dot_product(&scene->shapes[i].orientation, &up));
			axis_angle(&rot_transform, &ax, angle);
		}
		// TRANSLATE LAST
		// sphere_trans =  translation * sphere_trans
		
		translate_matrix(&translate_transform, scene->shapes[i].origin.x, scene->shapes[i].origin.y, scene->shapes[i].origin.z);
		mat_multiply(&scene->shapes[i].transf, &translate_transform, &rot_transform);
		ft_memcpy(&translate_transform, &scene->shapes[i].transf, sizeof(t_mat4));
		mat_multiply(&scene->shapes[i].transf, &translate_transform, &scale_transform);


		// * Calculate inverse transform here
		mat_inverse(&scene->shapes[i].inv_transf, &scene->shapes[i].transf);
		ft_memcpy(&scene->shapes[i].norm_transf, &scene->shapes[i].inv_transf, sizeof(t_mat4));
		transpose_matrix(&scene->shapes[i].norm_transf);
		i++;
	}
	// setup_room(scene);
}
