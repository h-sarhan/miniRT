/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:07:05 by mkhan             #+#    #+#             */
/*   Updated: 2022/11/21 20:41:25 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <miniRT.h>

void	my_mlx_pixel_put(t_mlx *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

void	position(t_vector *pos, t_ray *ray, float time)
{
	scale_vec(pos, &ray->direction, time);
	add_vec(pos, pos, &ray->origin);
}

bool	intersect(t_shape *shape, t_ray *ray, t_intersections *xs)
{
	float		a;
	float		b;
	float		c;
	float		discriminant;

	
	a = dot_product(&ray->direction, &ray->direction);
	b = 2 * dot_product(&ray->direction, &ray->origin);
	c = dot_product(&ray->origin, &ray->origin) - 1;
	discriminant = (b * b) - (4 * a * c);
	if (discriminant < 0)
		return (false);
	discriminant = sqrt(discriminant);
	xs->arr[xs->count].time = ((b * -1) - discriminant) / (2 * a);
	xs->arr[xs->count].shape = shape;
	xs->arr[xs->count + 1].time = ((b * -1) + discriminant) / (2 * a);
	xs->arr[xs->count + 1].shape = shape;
	xs->count += 2;
	return (true);
}

t_intersections	*hit(t_intersections *xs)
{
	float	min_time;
	int		i;
	int 	idx;

	i = 0;
	min_time = INFINITY;
	while (i < xs->count)
	{
		if (xs->arr[i].time >= 0 && xs->arr[i].time < min_time)
		{
			min_time = xs->arr[i].time;
			idx = i;
		}
		i++;
	}
	if (min_time == INFINITY)
		return (NULL);
	return (&xs->arr[idx]);
}

void draw_scene(t_scene *scene)
{
	t_mlx	img;
	
	(void)scene;
	img.mlx = mlx_init();
	img.mlx_win = mlx_new_window(img.mlx, 1920, 1080, "Hello world!");
	img.img = mlx_new_image(img.mlx, 1920, 1080);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
								&img.endian);
	my_mlx_pixel_put(&img, 5, 5, 0x00FF0000);
	mlx_put_image_to_window(img.mlx, img.mlx_win, img.img, 0, 0);
	mlx_loop(img.mlx);
}

