/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:46:46 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/19 11:27:32 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include "vector.h"
# include "matrix.h"
# include <pthread.h>

/**
 * @brief Type of light
 */
typedef enum e_light_type	t_light_type;
enum e_light_type
{
	POINT,
	SPOT
};

/**
 * @brief Represents a color.
 * @param r Red
 * @param g Green
 * @param b Blue
 * @param a Transparency
 */
typedef struct s_color		t_color;
struct s_color
{
	double	r;
	double	g;
	double	b;
	double	a;
};

// color.c
unsigned int	create_mlx_color(t_color *color);
void			add_colors(t_color *res, const t_color *c1, const t_color *c2);
void			sub_colors(t_color *res, const t_color *c1, const t_color *c2);
void			mult_color(t_color *res, const t_color *color, double val);
void			blend_colors(t_color *res, const t_color *c1,
					const t_color *c2);

/**
 * @brief A light source
 * @param type Type of light
 * @param position Origin of the light
 * @param intensity Strength of the light
 * @param color Color of the light
 */
typedef struct s_light		t_light;
struct s_light
{
	t_light_type	type;
	t_vector		position;
	double			intensity;
	t_color			color;
};

/**
 * @brief An approximation of indirect lighting
 * @param intensity How strong the light is
 * @param color The color of the light
 */
typedef struct s_ambient	t_ambient;
struct s_ambient
{
	double	intensity;
	t_color	color;
};

/**
 * @brief The camera defines the viewport position
 * @param position Origin of the camera
 * @param orientation Normalized vector representing the direction
 * where the camera is pointing
 * @param fov Field of view of the camera
 */
typedef struct s_camera		t_camera;
struct s_camera
{
	t_vector	position;
	t_vector	orientation;
	int			fov;
	double		pixel_size;
	double		half_width;
	double		half_height;
	t_mat4		transform;
	t_mat4		inv_trans;
	float		phi;
	float		theta;
};
void			camera_init(t_camera *camera, t_scene *scene);
void			view_transform(t_mat4 *res, const t_vector *from,
					const t_vector *up, const t_vector *forward);

/**
 * @brief Type of shape
 */
typedef enum e_shape_type	t_shape_type;
enum e_shape_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	CONE,
	CUBE,
	HYPERBOLOID,
	PARABOLOID,
	TRIANGLE,
};
/**
 * @brief Generic shape struct
 * @param type Type of shape. Sphere, plane, cylinder, etc.
 * @param radius Radius of the shape. Relevant for sphere, cylinder, and
 *  cone
 * @param height Height of the shape. Relevant for cylinder
 * @param origin Origin of the shape. Relevant for sphere, cylinder, and plane
 * @param orientation Orientation of the shape. Relevant for plane and cylinder
 * @param color Color of the shape
 */
typedef struct s_shape		t_shape;
struct s_shape
{
	t_shape_type	type;
	double			radius;
	double			height;
	t_vector		origin;
	t_vector		orientation;
	t_color			color;
	t_mat4			transf;
	t_mat4			inv_transf;
	t_mat4			norm_transf;
	unsigned int	mlx_color;
	double			diffuse;
	double			specular;
	double			shininess;
	double			reflectiveness;
	int				rot_x;
	int				rot_y;
	int				rot_z;
	int				scale_x;
	int				scale_y;
	int				scale_z;
	bool			highlighted;
};
void			reflect(t_vector *res, t_vector *in_vector, t_vector *normal);

/**
 * @brief Holds the number of elements in a scene
 * @param ambient_count Number of ambient lights. Should be 1
 * @param camera_count Number of cameras. Should be 1
 * @param light_count Number of lights
 * @param shape_count Number of shapes
 */
typedef struct s_el_count	t_el_count;
struct	s_el_count
{
	unsigned int	ambient_count;
	unsigned int	camera_count;
	unsigned int	light_count;
	unsigned int	shape_count;
};

typedef struct s_mlx		t_mlx;
typedef struct s_keys		t_keys;
struct s_keys
{
	bool	w;
	bool	a;
	bool	s;
	bool	d;
	bool	up;
	bool	down;
	bool	left;
	bool	right;
	bool	q;
	bool	e;
	bool	plus;
	bool	minus;
	bool	tab;
	bool	c;
};
int				set_key_down(int key, t_scene *scene);
int				set_key_up(int key, t_scene *scene);


typedef struct s_look_at		t_look_at;
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
void	perspective_projection(t_vector *point, const t_scene *scene);


// Waste of time
typedef struct s_mouse		t_mouse;
struct	s_mouse
{
	bool	active;
	bool	toggle;
	int		prev_x;
	int		prev_y;
	int		x;
	int		y;
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
	t_ambient	ambient;
	t_camera	camera;
	t_light		*lights;
	t_shape		*shapes;
	t_el_count	count;
	int			display_h;
	int			display_w;
	int			render_h;
	int			render_w;
	int			edit_h;
	int			edit_w;
	t_mlx		*mlx;
	t_keys		keys_held;
	int			shape_idx;
	bool		camera_mode;
	bool		edit_mode;
	bool		menu;
	int			reflection_depth;
	sem_t		*sem_loading;
	double		edit_scale;
	double		render_scale;
	t_look_at	look_at;
	t_mouse		mouse;
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
void	dda(t_scene *scene, double x1, double x2, double y1, double y2, int color);
bool	is_colliding(const t_shape *shape, const t_scene *scene);

#endif