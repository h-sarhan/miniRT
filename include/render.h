/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 11:29:17 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/26 00:23:38 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# define NUM_THREADS 16
# define REFLECTION_DEPTH 1

typedef struct s_scene		t_scene; // forward_declaration
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
	void	*edit_img;
	char	*edit_addr;
	void	*display_img;
	char	*display_addr;
	void	*render_img;
	char	*render_addr;
	void	*info_img;
	char	*info_addr;
	int		bytes_per_pixel;
	int		line_length;
	int		endian;
};

typedef struct s_worker		t_worker;

struct s_worker
{
	int		worker_id;
	int		y_start;
	int		y_end;
	int		y_scale_start;
	int		y_scale_end;
	int		max_workers;
	int		height;
	int		width;
	char	*addr;
	t_scene	*scene;
};

void	prepare_computations(t_scene *scene, t_intersect *intersection, t_ray *ray, t_intersections *xs);

t_color	lighting(t_intersect *itx, t_scene *scene, int light_idx);

bool	is_shadowed(t_scene *scene, int light_idx, t_vector *itx_point);

float	schlick(t_intersect *intersection);
t_color	reflected_color(t_scene *scene, t_intersect *intersection, int remaining, int light_idx);
t_color	refracted_color(t_scene *scene, t_intersect *intersection, int remaining, int light_idx);

void	*render_scene_faster(t_worker *worker);
#endif