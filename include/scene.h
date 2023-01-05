/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:46:46 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/05 18:22:20 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include "parsing.h"

/**
 * @brief Holds the number of elements in a scene
 * @param ambient_lights Number of ambient lights. Should be 1
 * @param camera_count Number of cameras. Should be 1
 * @param lights Number of lights
 * @param shape_count Number of shapes
 */
typedef struct s_el_count	t_el_count;
struct	s_el_count
{
	int	ambient_lights;
	int	cameras;
	int	lights;
	int	shapes;
};

typedef struct s_look_at	t_look_at;
struct s_look_at
{
	t_vector	final_pos;
	t_vector	initial_orientation;
	t_vector	final_dir;
	t_vector	current_dir;
	t_vector	pos_diff;
	t_vector	dir_diff;
	bool		trigger;
	int			step_num;
	int			step_amount;
};

typedef struct s_mouse		t_mouse;
struct	s_mouse
{
	bool	active;
	bool	toggle;
	int		prev_x;
	int		prev_y;
	int		x;
	int		y;
	int		key;
};

typedef struct s_settings	t_settings;
struct s_settings
{
	int			disp_h;
	int			disp_w;
	int			render_h;
	int			render_w;
	int			edit_h;
	int			edit_w;
	bool		camera_mode;
	bool		edit_mode;
	int			reflection_depth;
	float		edit_scale;
	float		render_scale;
	bool		collisions;
	bool		help_menu;
};

/**
 * @brief A description of a 3D scene
 * @param ambient Ambient lighting information
 * @param camera Camera information
 * @param lights Array of lights
 * @param shapes Array of shapes
 */
typedef struct s_scene		t_scene;
struct s_scene
{
	t_ambient		ambient;
	t_camera		camera;
	t_light			*lights;
	t_shape			*shapes;
	t_el_count		count;
	t_display		*disp;
	t_keys			keys_held;
	t_settings		settings;
	int				shape_idx;
	sem_t			*sem_loading;
	t_look_at		look_at;
	t_mouse			mouse;
	t_error_flags	error_flags;
	bool			supersampling;
};

void	free_scene(t_scene *scene);

void	calculate_transforms(t_scene *scene);
void	draw_scene(t_scene *scene);

// ui.h
void	draw_marker(t_scene *scene, int x, int y, int color);
void	draw_arrow(t_scene *scene, int x, int y, float z);
void	perspective_projection(t_vector *point, const t_scene *scene);
void	draw_shape_info(t_scene *scene);
void	draw_shape_marker(t_scene *scene);
void	display_loading_bar(t_scene *scene);

#endif
