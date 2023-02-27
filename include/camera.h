/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 23:21:42 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/27 12:26:07 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "mathRT.h"

typedef struct s_scene	t_scene;

/**
 * @brief The camera defines the viewport position
 * @param position Origin of the camera
 * @param orientation Normalized vector representing the direction
 * where the camera is pointing
 * @param fov Field of view of the camera
 */
typedef struct s_camera	t_camera;
struct s_camera
{
	t_vector	position;
	t_vector	dir;
	double		fov;
	double		pixel_size;
	double		half_width;
	double		half_height;
	t_mat4		transform;
	t_mat4		inv_trans;
	double		phi;
	double		theta;
};

void	view_transform(t_mat4 *res, const t_vector *from, const t_vector *up,
			const t_vector *forward);

void	camera_init(t_camera *camera, t_scene *scene);
void	calculate_camera_transform(t_scene *scene);

#endif
