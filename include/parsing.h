/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:45:41 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/19 17:48:43 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# define RED "\x1b[31m"
# define RESET "\x1b[0m"
# define YELLOW "\x1b[33m"
# define MAGENTA "\e[0;35m"
# define GREEN "\x1B[32m"
# define LIGHT_MAX 20
# define SHAPE_MAX 100

typedef struct s_color_error		t_color_error;
struct s_color_error
{
	bool	r;
	bool	g;
	bool	b;
	bool	other;
};

typedef struct s_orient_error		t_orient_error;
struct s_orient_error
{
	bool	x;
	bool	y;
	bool	z;
	bool	zero;
	bool	other;
};

typedef struct s_ambient_errors		t_ambient_errors;
struct s_ambient_errors
{
	bool			intensity_other;
	bool			intensity_range;
	bool			other;
	t_color_error	color;
};

typedef struct s_cam_errors			t_cam_errors;
struct s_cam_errors
{
	bool			other;
	bool			coords;
	bool			fov_other;
	bool			fov_range;
	bool			up_vector;
	t_orient_error	dir;
};

typedef struct s_light_errors		t_light_errors;
struct s_light_errors
{
	bool			intensity_other;
	bool			intensity_range;
	bool			max_lights;
	bool			other;
	bool			coords;
	t_orient_error	orient;
	bool			angle_other;
	bool			angle_range;
	t_color_error	color;
};

typedef struct s_shape_errors		t_shape_errors;
struct s_shape_errors
{
	bool			max_shapes;
	bool			diameter_range;
	bool			diameter_other;
	bool			height_range;
	bool			height_other;
	bool			origin;
	bool			side_len_other;
	bool			side_len_range;
	bool			other;
	t_color_error	color;
	t_orient_error	orient;
};

typedef struct s_parse_error_flags	t_error_flags;
struct	s_parse_error_flags
{
	t_ambient_errors	ambient;
	t_cam_errors		cam;
	t_light_errors		light;
	t_shape_errors		shape;
	bool				unknown_identifier;
	bool				settings_err;
};

bool	find_error(t_error_flags *errors);

# define GENERIC_ERROR "\x1b[33mError with parsing %s on \
line #%d\n\n\x1b[31m->\t%s\n\n"

# define ORIENT_X_OOR "\x1b[33mThe x value is out of range \
[-1.0 -> 1.0]\n\x1b[0m"

# define ORIENT_Y_OOR "\x1b[33mThe y value is out of range \
[-1.0 -> 1.0]\n\x1b[0m"

# define ORIENT_Z_OOR "\x1b[33mThe z value is out of range \
[-1.0 -> 1.0]\n\x1b[0m"

# define ORIENT_ZERO "\x1b[33mThe orientation vector cannot be the \
zero vector\n\x1b[0m"

# define RED_OOR "\x1b[33mThe red value is out of range \
[0 -> 255]\n\x1b[0m"

# define GREEN_OOR "\x1b[33mThe green value is out of range \
[0 -> 255]\n\x1b[0m"

# define BLUE_OOR "\x1b[33mThe blue value is out of range \
[0 -> 255]\n\x1b[0m"

# define MULTI_ELEMENT_ERROR "\x1b[31mError: Scene contains multiple \
%s\n\x1b[0m"

# define NO_ELEMENT_ERROR "\x1b[31mError: Scene contains no \
%s\n\x1b[0m"

# define MAX_ELEMENT_ERROR "\x1b[31mError: Scene contains more than \
%d %s\n\x1b[0m"

# define UNKNOWN_IDENTIFIER "\x1b[33mUnknown identifier \"%s\" on \
line #%d\n\x1b[31m->\t%s\n\x1b[0m"

# define LIGHT_INTENSITY_OOR "\x1b[33m%s intensity value is out of range on \
line #%d\n\n\x1b[31m->\t%s\n\n\x1b[0m"

# define CAMERA_UP_VECTOR "\x1b[33mCamera orientation cannot be the up vector \
(0, 1, 0)\n\x1b[0m"

# define CAMERA_FOV_OOR "\x1b[33mThe fov value is out of range \
[1 -> 180]\n\x1b[0m"

# define POSITIVE_VALUE "\x1b[33m%s has to be a positive number on line \
#%d\n\n\x1b[31m->\t%s\n\n\x1b[0m"

# define LIGHT_SYNTAX "\x1b[33mCorrect syntax is \"L [origin] [intensity] \
[color]\"\n\x1b[0m"

# define AMBIENT_LIGHT_SYNTAX "\x1b[33mCorrect syntax is \"A [intensity] \
[color]\"\n\x1b[0m"

# define CAMERA_SYNTAX "\x1b[33mCorrect syntax is \"C [origin] [orientation] \
[fov]\"\n\x1b[0m"

# define SPHERE_SYNTAX "\x1b[33mCorrect syntax is \"sp [origin] [diameter] \
[color]\"\n\x1b[0m"

# define PLANE_SYNTAX "\x1b[33mCorrect syntax is \"pl [origin] [orientation] \
[color]\"\n\x1b[0m"

# define CYLINDER_SYNTAX "\x1b[33mCorrect syntax is \"cy [origin] \
[orientation] [diameter] [height] [color]\"\n\x1b[0m"

# define CONE_SYNTAX "\x1b[33mCorrect syntax is \"co [origin] [orientation] \
[diameter] [height] [color]\"\n\x1b[0m"

# define CUBE_SYNTAX "\x1b[33mCorrect syntax is \"cu [origin] [side length] \
[color]\"\n\x1b[0m"

# define SPOTLIGHT_SYNTAX "\x1b[33mCorrect syntax is \"SL [origin] [intensity] \
[orientation] [beam width] [color]\"\n\x1b[0m"

# define SPOTLIGHT_ANGLE_OOR "\x1b[33mThe spotlight beam width value is out \
of range [1 -> 180] on line #%d\n\n\x1b[31m->\t%s\n\n\x1b[0m"

# define SETTINGS_NO_SHAPE "\x1b[31mSettings at line %ld do not belong to any \
shape\n\x1b[0m"

# define UNTERMINATED_SETTINGS "\x1b[31mUnterminated shape settings starting \
at line %ld\n\x1b[0m"

# define EMPTY_SETTINGS "\x1b[31mEmpty shape settings starting at line \
%ld\n\x1b[0m"

# define EXTRA_OPENING_BRACE "\x1b[31mShape settings starting at line %ld \
contains an extra opening brace\n\x1b[0m"

# define EXTRA_CLOSING_BRACE "\x1b[31mShape settings starting at line %ld \
contains an extra closing brace\n\x1b[0m"

# define INVALID_TERMINATION "\x1b[31mShape settings starting at line %ld is \
not terminated correctly\n\x1b[0m"

# define INVALID_PROPERTY "\x1b[33mError with parsing this property \
\n\x1b[31m->\t%s\n\x1b[33mCorrect syntax is KEY : VALUE\n\x1b[0m"

# define INVALID_KEY "\x1b[33mError with parsing this property\n\x1b \
[31m->\t%s\n\x1b[33m`%s` is not a valid key\n\x1b[0m"

# define COLOR_SETTING_ERROR "\x1b[33mError with parsing this property\n\x1b \
[31m->\t%s : %s\n\x1b[33m`%s` is not a valid value\n\e[0;35mAvailable colors \
are BLUE, RED, PURPLE, GREEN, YELLOW, PINK, BLACK, GRAY\n\x1b[0m"

# define INVALID_PROP_VALUE "\x1b[33mError with parsing this property\n \
\x1b[31m->\t%s : %s\n\x1b[33m`%s` is not a valid value\n\x1b[0m"

# define INVALID_PROPERTY_RANGE "\x1b[33mError with parsing this property \
\n\x1b[31m->\t%s : %s\n\x1b[33m%s has to be between %.2f and %.2f\n\x1b[0m"

bool	parse_ambient(t_scene *scene, char **splitted);
void	parse_camera(t_scene *scene, char **splitted);
void	parse_light(t_scene *scene, char **splitted);

bool	parse_shape(t_scene *scene, char **splitted);

bool	parse_settings(t_scene *scene, const char *settings_start,
			size_t *line_num, int fd);

void	parse_coordinates(t_vector *position, const char *str, bool *success);
void	parse_orientation(t_vector *orientation, const char *str,
			t_orient_error *err);

void	parse_color(t_color *color, const char *str, t_color_error *errs);

bool	is_settings(const char *line);
bool	is_shape(const char *identifier);

bool	is_num(const char *str, bool decimal);
size_t	count_commas(const char *str);
size_t	split_count(char **split);
bool	all_whitespace(const char *str);
void	print_error(t_scene *scene, const char *line, int line_num,
			const char *identifer);
t_scene	*parse_scene(int fd);
void	init_shape(t_shape *shape, t_scene *scene);
void	parse_sphere(t_scene *scene, t_shape *shape, char **splitted);
void	parse_cube(t_scene *scene, t_shape *shape, char **splitted);
void	parse_plane(t_scene *scene, t_shape *shape, char **splitted);
bool	print_shape_error(t_shape *shape, t_shape_errors *err, const char *line,
			int line_num);
bool	print_color_error(t_color_error *err, const char *line, int line_num,
			char *element);
bool	print_orient_error(t_orient_error *err, const char *line, int line_num,
			char *element);
bool	print_ambient_light_error(t_ambient_errors *err, const char *line,
			int line_num);

bool	print_cam_error(t_cam_errors *err, const char *line, int line_num);
bool	print_light_error(t_light_errors *err, const char *line, int line_num);
bool	print_sphere_error(t_shape_errors *err, const char *line, int line_num);
bool	print_cube_error(t_shape_errors *err, const char *line, int line_num);
bool	print_plane_error(t_shape_errors *err, const char *line, int line_num);
bool	print_cylinder_error(t_shape_errors *err, const char *line,
			int line_num);
bool	print_cone_error(t_shape_errors *err, const char *line, int line_num);

bool	is_settings(const char *line);
bool	is_valid_key(const char *key);
bool	is_valid_color(const char *color);
bool	check_value(const char *key, const char *val, double min, double max);
bool	is_valid_val(const char *key, const char *val);
t_color	parse_color_value(const char *str);
bool	parse_setting(t_shape *shape, char **key_val);
bool	check_braces(char *settings_str, size_t *line_num);
char	*check_settings_str(char *settings_str, size_t *line_num);
char	*get_settings_str(size_t *line_num, int fd, const char *settings_start);
bool	check_colons(char *line);
bool	parse_split_settings(t_scene *scene, char **settings);
bool	parse_settings(t_scene *scene, const char *settings_start,
			size_t *line_num, int fd);

t_color	**parse_texture(char *img_path, t_shape *shape);
void	parse_spotlight(t_scene *scene, char **splitted);

#endif
