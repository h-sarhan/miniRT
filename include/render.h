/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 11:29:17 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/02 15:30:53 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# define NUM_THREADS 16
# define REFLECTION_DEPTH 1

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

void	prepare_computations(t_intersection *intersection,
			t_ray *ray);

t_color	lighting(t_intersection *itx, t_scene *scene, int light_idx);

bool	is_shadowed(t_scene *scene, int light_idx, t_vector *itx_point);

t_color	reflected_color(t_scene *scene, t_intersection *intersection,
			int remaining, int light_idx);

void	*render_scene_fast(t_worker *worker);
int		get_color(t_worker *worker, int x, int y);
void	set_color(t_worker *worker, int x, int y, int color);

t_color	calculate_lighting(t_intersections *arr, t_scene *scene, t_ray *ray);

#endif
