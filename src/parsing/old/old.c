/**
 * @brief Checks whether the orientation vector is valid and prints an
 *  appropriate error message is it is not
 * @param orientation The orientation vector to check
 * @param line_num The line number where the vector is located
 * @param line The line where the vector is located
 * @param element The type of element the vector belongs to
 * @return True if the vector is invalid
 */
bool	check_orientation(const t_vector *orientation, size_t line_num,
	const char *line, const char *element)
{
	if (orientation->x < -1.0 || orientation->x > 1.0
		|| orientation->y < -1.0 || orientation->y > 1.0
		|| orientation->z < -1.0 || orientation->z > 1.0
		|| vec_magnitude(orientation) <= EPSILON)
	{
		printf(YELLOW"Error with parsing %s orientation on line #%ld\n"
			RED"->\t%s\n"RESET, element, line_num, line);
		if (orientation->x < -1.0 || orientation->x > 1.0)
			printf(YELLOW"The x value is out of range\n"RESET);
		if (orientation->y < -1.0 || orientation->y > 1.0)
			printf(YELLOW"The y value is out of range\n"RESET);
		if (orientation->z < -1.0 || orientation->z > 1.0)
			printf(YELLOW"The z value is out of range\n"RESET);
		if (vec_magnitude(orientation) <= EPSILON)
			printf(YELLOW"The orientation vector cannot be the zero vector"
				"\n"RESET);
		return (true);
	}
	return (false);
}

/**
 * @brief Checks wheter the color is valid and prints an
 *  appropriate error message is it is not
 * @param color The color to check
 * @param line_num The line number where the color is located
 * @param line The line where the color is located
 * @param element The type of element the color belongs to
 * @return True if the color is invalid
 */
bool	check_color(const t_color *color, size_t line_num, const char *line,
	const char *element)
{
	if (color->r < 0 || color->r > 255
		|| color->g < 0 || color->g > 255
		|| color->b < 0 || color->b > 255)
	{
		printf(YELLOW"Error with parsing %s color on line #%ld\n"
			RED"->\t%s\n"RESET, element, line_num, line);
		if (color->r < 0 || color->r > 255)
			printf(YELLOW"The red value is out of range\n"RESET);
		if (color->g < 0 || color->g > 255)
			printf(YELLOW"The green value is out of range\n"RESET);
		if (color->b < 0 || color->b > 255)
			printf(YELLOW"The blue value is out of range\n"RESET);
		return (true);
	}
	return (false);
}


/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a point light
 * @param line Line where the parse error occured
 * @param line_num Number of the line where the error occured
 * @param scene Pointer to the scene struct
 * @param splitted Array to be freed
 * @param fd fd to be closed
 */
void	*light_parse_error(char *line, size_t line_num, t_scene *scene)
{
	t_light	*light;

	if (scene->lights == NULL)
		light = NULL;
	else
		light = &scene->lights[scene->count.lights];
	if (light != NULL && scene->count.lights >= LIGHT_MAX)
		printf(RED"Error: Scene contains more than %d lights\n"
			RESET, LIGHT_MAX);
	else if (light && (light->intensity < 0.0 || light->intensity > 1.0))
	{
		printf(YELLOW"Error with parsing light intensity on line #%ld\n"
			RED"->\t%s\n"RESET, line_num, line);
		printf(YELLOW"The intensity value is out of range\n"RESET);
	}
	else if (!light || !check_color(&light->color, line_num, line, "light"))
		printf(YELLOW"Error with parsing light on line #%ld\n"RED"->\t%s"RESET
			YELLOW"Correct syntax is \"L [origin] [intensity] [color]\"\n"RESET,
			line_num, line);
	free_scene(scene);
	get_next_line(-1);
	return (NULL);
}

/**
 * @brief Prints an error message if an identifier is unrecognized
 * @param line Line where the identifier was located
 * @param line_num Number of the line where the identifier was located
 * @param scene Scene struct to be freed
 * @param splitted Array to be freed
 * @param fd File descriptor to be closed
 */
void	*unknown_identifier(char *line, size_t line_num, t_scene *scene,
		char **splitted)
{
	printf(YELLOW"Unknown identifier \"%s\" on line #%ld\n"RED"->\t%s"RESET,
		splitted[0], line_num, line);
	free_scene(scene);
	get_next_line(-1);
	return (NULL);
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing an ambient light
 * @param line Line where the parse error occured
 * @param line_num Number of the line where the error occured
 * @param scene Pointer to the scene struct
 * @param splitted Array to be freed
 * @param fd fd to be closed
 */
void	*ambient_parse_error(char *line, size_t line_num, t_scene *scene)
{
	bool	color;

	color = check_color(&scene->ambient.color, line_num, line, "ambient light");
	if (!color && (scene->ambient.intensity < 0.0
			|| scene->ambient.intensity > 1.0))
	{
		printf(YELLOW"Ambient light intensity out of range on line #%ld\n"
			RED"->\t%s"RESET"Range is [0.0 -> 1.0]\n", line_num, line);
	}
	else if (!color)
	{
		printf(YELLOW"Error with parsing ambient light on line #%ld\n"
			RED"->\t%s"YELLOW"Correct syntax is \"A [intensity] [color]\"\n"
			RESET, line_num, line);
	}
	free_scene(scene);
	get_next_line(-1);
	return (NULL);
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a camera
 * @param line Line where the parse error occured
 * @param line_num Number of the line where the error occured
 * @param scene Pointer to the scene struct
 * @param splitted Array to be freed
 * @param fd fd to be closed
 */
void	*camera_parse_error(char *line, size_t line_num, t_scene *scene,
	bool invalid_coords)
{
	bool	orientation;

	orientation = false;
	if (!invalid_coords)
		orientation = check_orientation(&scene->camera.dir, line_num, line,
				"camera");
	if (!invalid_coords && vec_magnitude(&scene->camera.dir) == 1
		&& scene->camera.dir.y == 1)
	{
		printf(YELLOW"Error with parsing camera orientation on line #%ld\n"
			RED"->\t%s\n"RESET, line_num, line);
		printf(YELLOW"Camera orientation cannot be the up vector (0, 1, 0)\n"
			RESET);
	}
	else if (!invalid_coords && !orientation
		&& (scene->camera.fov < 0 || scene->camera.fov > 180))
	{
		printf(YELLOW"Error with parsing camera fov on line #%ld\n"RED"->\t%s\n"
			RESET, line_num, line);
		printf(YELLOW"The fov value is out of range\n"RESET);
	}
	else if (invalid_coords || !orientation)
		printf(YELLOW"Error with parsing camera on line #%ld\n"RED"->\t%s"RESET
			YELLOW"Correct syntax is \"C [origin] [orientation] [fov]\"\n"RESET,
			line_num, line);
	free_scene(scene);
	get_next_line(-1);
	return (NULL);
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a sphere
 * @param shape The sphere being parsed
 * @param line_num The number of the line containing the sphere
 * @param line The line containing the sphere
 */
static void	sphere_parse_error(const t_shape *shape, size_t line_num,
	const char *line)
{
	bool	color_check;

	color_check = check_color(&shape->props.color, line_num, line, "sphere");
	if (shape->props.radius <= 0)
	{
		printf(YELLOW"Error with sphere diameter on line #%ld\n"RED"->\t%s"
			RESET, line_num, line);
		printf(YELLOW"Diameter has to be a positive number\n"RESET);
	}
	else if (!color_check)
	{
		printf(YELLOW"Error with parsing sphere on line #%ld\n"RED"->\t%s"
			YELLOW"Correct syntax is \"sp [origin] [diameter] [color]\"\n"
			RESET, line_num, line);
	}
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a plane
 * @param shape The plane being parsed
 * @param line_num The number of the line containing the plane
 * @param line The line containing the plane
 */
static void	plane_parse_error(const t_shape *shape, size_t line_num,
	const char *line)
{
	bool	orientation_check;
	bool	color_check;

	orientation_check = check_orientation(&shape->orientation, line_num, line,
			"plane");
	color_check = check_color(&shape->props.color, line_num, line, "plane");
	if (!color_check && !orientation_check)
	{
		printf(YELLOW"Error with parsing plane on line #%ld\n"RED"->\t%s"
			YELLOW"Correct syntax is \"pl [origin] [orientation]"
			" [color]\"\n"RESET, line_num, line);
	}
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a cylinder
 * @param shape The cylinder being parsed
 * @param line_num The number of the line containing the cylinder
 * @param line The line containing the cylinder
 */
static void	cylinder_parse_error(const t_shape *shape, size_t line_num,
	const char *line)
{
	bool	orientation_check;
	bool	color_check;

	orientation_check = check_orientation(&shape->orientation, line_num, line,
			"cylinder");
	color_check = check_color(&shape->props.color, line_num, line, "cylinder");
	if (!color_check && !orientation_check && shape->props.radius <= 0)
	{
		printf(YELLOW"Error with cylinder diameter on line #%ld\n"
			RED"->\t%s"RESET, line_num, line);
		printf(YELLOW"Diameter has to be a positive number\n"RESET);
	}
	else if (!color_check && !orientation_check && shape->props.height <= 0)
	{
		printf(YELLOW"Error with cylinder height on line #%ld\n"
			RED"->\t%s"RESET, line_num, line);
		printf(YELLOW"Height has to be a positive number\n"RESET);
	}
	else if (!orientation_check && !color_check)
		printf(YELLOW"Error with parsing cylinder on line #%ld\n"RED"->\t%s\n"
			YELLOW"Correct syntax is "
			"\"cy [origin] [orientation] [diameter] [height] [color]\"\n"
			RESET, line_num, line);
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a cube
 * @param shape The cube being parsed
 * @param line_num The number of the line containing the cube
 * @param line The line containing the cube
 */
static void	cube_parse_error(const t_shape *shape, size_t line_num,
	const char *line)
{
	bool	color_check;

	color_check = check_color(&shape->props.color, line_num, line, "cube");
	if (!color_check && shape->props.scale.x <= 0)
	{
		printf(YELLOW"Error with cube side length on line #%ld\n"
			RED"->\t%s\n"RESET, line_num, line);
		printf(YELLOW"Side length has to be a positive number\n"RESET);
	}
	else if (!color_check)
		printf(YELLOW"Error with parsing cube on line #%ld\n"RED"->\t%s\n"
			YELLOW"Correct syntax is "
			"\"cu [origin] [side length] [color]\"\n"
			RESET, line_num, line);
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a shape
 * @param line Line where the parse error occured
 * @param line_num Number of the line where the error occured
 * @param scene Pointer to the scene struct
 * @param splitted Array to be freed
 */
void	*shape_parse_error(char *line, size_t line_num, t_scene *scene,
		char **splitted)
{
	t_shape	*shape;

	if (scene->shapes == NULL)
		shape = NULL;
	else
		shape = &scene->shapes[scene->count.shapes];
	if (shape && scene->count.shapes >= SHAPE_MAX)
		printf(RED"Error: Scene contains more than %d shapes\n"RESET,
			SHAPE_MAX);
	else if (shape && shape->type == SPHERE && split_count(splitted) == 4)
		sphere_parse_error(shape, line_num, line);
	else if (shape && shape->type == PLANE && split_count(splitted) == 4)
		plane_parse_error(shape, line_num, line);
	else if (shape && shape->type == CYLINDER && split_count(splitted) == 6)
		cylinder_parse_error(shape, line_num, line);
	else if (shape && shape->type == CUBE && split_count(splitted) == 4)
		cube_parse_error(shape, line_num, line);
	else
		printf(YELLOW"Error with parsing shape on line #%ld\n"RED"->\t%s"RESET,
			line_num, line);
	free_scene(scene);
	get_next_line(-1);
	return (NULL);
}
