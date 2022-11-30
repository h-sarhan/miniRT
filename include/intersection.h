/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:19:12 by mkhan             #+#    #+#             */
/*   Updated: 2022/11/30 15:07:58 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERSECTION_H
# define INTERSECTION_H

# include "scene.h"

/**
 * @brief A ray being cast from the screen
 * @param origin The ray starting point
 * @param direction The direction where the ray is being projected
 */
typedef struct s_ray			t_ray;
struct s_ray
{
	t_vector	origin;
	t_vector	direction;
};
void	ray_for_pixel(t_ray *ray, const t_camera *cam, int x, int y);

/**
 * @brief Struct containing information relevant for mlx and the window
 * generated by it
 * @param mlx Pointer to mlx instance
 * @param mlx_win Pointer to mlx window
 * @param img Pointer to image that we will write the pixels to
 * @param addr Pointer to the first pixel of the image
 * @param bytes_per_pixel Bytes per pixel
 * @param line_length Number of pixels in a row
 * @param endian idk
 */
typedef struct s_mlx			t_mlx;
struct	s_mlx
{
	void	*mlx;
	void	*mlx_win;
	void	*img;
	void	*display_img;
	char	*addr;
	char	*display_addr;
	int		bytes_per_pixel;
	int		line_length;
	int		endian;
};

/**
 * @brief Contains the time value of an intersection of a ray with an object,
 * along with the object that it intersected with
 * @param time The t value of the intersection
 * @param shape A pointer to the object that the ray intersected with
 */
typedef struct s_intersect		t_intersect;
struct s_intersect
{	
	double	time;
	t_shape	*shape;
	t_vector point;
	t_vector normal;
	t_vector eye;
	t_vector over_point;
	bool	inside;
};

/**
 * @brief A wrapper around an array of intersections
 * @param arr Array of intersections
 * @param count Number of intersections in the array
 */
typedef struct s_intersections	t_intersections;
struct s_intersections
{
	t_intersect	arr[100];
	int			count;
};
// draw_scene.c
void		my_mlx_pixel_put(t_mlx *data, int x, int y, int color);
void		draw_scene(t_scene *scene);

// intersections.c
void		ray_position(t_vector *pos, const t_ray *ray, double time);
void	transform_ray(t_ray *transformed_ray, const t_ray *ray, const t_shape *shape);
bool		intersect(t_shape *shape, const t_ray *ray, t_intersections *xs);
t_intersect	*hit(t_intersections *xs);
t_vector	normal_at(const t_shape *shape, const t_vector *intersection_point);
t_color		lighting(t_intersect *intersection, t_scene *scene, int light_idx);
bool	is_shadowed(t_scene *scene, int	light_idx, t_vector *intersection_point);

#endif
