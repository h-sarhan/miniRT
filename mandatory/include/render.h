/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 11:29:17 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/20 00:52:49 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# define REFLECTION_DEPTH 1

void	prepare_computations(t_intersection *intersection, t_ray *ray);

t_color	phong(t_intersection *itx, t_scene *scene, int light_idx);

bool	is_shadowed(t_scene *scene, int light_idx,
			t_vector *itx_point, double *angle_ret);

t_color	cast_reflection_ray(t_scene *scene, t_intersection *intersection,
			int remaining, int light_idx);

int		get_color(t_scene *worker, int x, int y);
void	set_color(t_scene *scene, int x, int y, int color);

t_color	shade_point(t_intersections *arr, t_scene *scene, t_ray *ray);
t_color	render_pixel(double x, double y, t_intersections *arr,
			t_scene *worker);

void	*nearest_neighbours_scaling(t_scene *scene);
bool	get_specular_and_diffuse(t_scene *scene, int light_idx,
			t_intersection *itx, t_phong *phong);
t_color	get_shape_color(t_intersection *itx);
t_color	get_texture_color(t_intersection *itx);
void	calculate_specular(t_vector *reflect_v, t_intersection *itx,
			t_phong *phong, t_light *light);

#endif
