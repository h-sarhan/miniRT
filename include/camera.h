/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:29:56 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/17 19:56:00 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include <vector.h>

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
	t_vector	orientation;
	int			fov;
};

#endif