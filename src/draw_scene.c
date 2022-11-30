/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 20:19:41 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/30 15:59:34 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	my_mlx_pixel_put(t_mlx *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bytes_per_pixel));
	*(unsigned int*)dst = color;
}

void	prepare_computations(t_intersect *intersection, t_ray *ray)
{
	ray_position(&intersection->point, ray, intersection->time);
	intersection->normal = normal_at(intersection->shape, &intersection->point);
	negate_vec(&intersection->eye, &ray->direction);
	intersection->eye.w = 0;
	if (dot_product(&intersection->normal, &intersection->eye) < 0)
	{
		intersection->inside = true;
		negate_vec(&intersection->normal, &intersection->normal);
	}
	else
		intersection->inside = false;
	scale_vec(&intersection->over_point, &intersection->normal, 0.001);
	add_vec(&intersection->over_point, &intersection->point, &intersection->over_point);
}

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

/**
 * @brief Draws a scene
 * @param scene A struct describing the current scene
 */
void draw_scene(t_scene *scene)
{
	t_intersections	arr;
	t_ray			ray;
	int				x;
	int				y;
	t_mlx			*mlx;
	t_color			color;
	unsigned int	shape_idx;
	t_intersect		*intersection;
	t_color			light_color;

	mlx = scene->mlx;
	x = 0;
	y = 0;
	arr.count = 0;
	int pixel = 0;
	TICK(render);
	while (y < scene->render_h)
	{
		x = 0;
		while (x < scene->render_w)
		{
			*(unsigned int *)(mlx->addr + pixel) = 0;
			ray_for_pixel(&ray, &scene->camera, x, y);
			shape_idx = 0;
			arr.count = 0;
			while (shape_idx < scene->count.shape_count)
			{
				intersect(&scene->shapes[shape_idx], &ray, &arr);
				intersection  = hit(&arr);
				if (intersection != NULL)
				{
					prepare_computations(intersection, &ray);
					ft_bzero(&color, sizeof(t_color));
					unsigned int light_idx = 0;
					while (light_idx < scene->count.light_count)
					{
						light_color = lighting(intersection, scene, light_idx);
						add_colors(&color, &color, &light_color);
						light_idx++;
					}
					intersection->shape->mlx_color = create_mlx_color(&color);
					*(unsigned int *)(mlx->addr + pixel) = intersection->shape->mlx_color;
				}
				shape_idx++;
			}
			pixel += mlx->bytes_per_pixel;
			x++;
		}
		y++;
	}
	TOCK(render);
	// def nearestNeighborScaling( source, newWid, newHt ):
    // target = makeEmptyPicture(newWid, newHt)
    // width = getWidth( source )
    // height = getHeight( source )
    // for x in range(0, newWid):  
    //   for y in range(0, newHt):
    //     srcX = int( round( float(x) / float(newWid) * float(width) ) )
    //     srcY = int( round( float(y) / float(newHt) * float(height) ) )
    //     srcX = min( srcX, width-1)
    //     srcY = min( srcY, height-1)
    //     tarPix = getPixel(target, x, y )
    //     srcColor = getColor( getPixel(source, srcX, srcY) )
    //     setColor( tarPix, srcColor)
	y = 0;
	pixel = 0;
	TICK(scale);
	while (y < scene->win_h)
	{
		x = 0;
		while (x < scene->win_w)
		{
			int src_x = roundf(((float)x / (float)scene->win_w) * scene->render_w);
			int src_y = roundf(((float)y / (float)scene->win_h) * scene->render_h);
			src_x = min(src_x, scene->render_w - 1);
			src_y = min(src_y, scene->render_h - 1);
			// printf("%d %d\n", src_x, src_y);
			*(unsigned int *)(mlx->display_addr + pixel) = *(unsigned int *)(mlx->addr + (src_y * mlx->line_length + src_x * (mlx->bytes_per_pixel)));
			pixel += mlx->bytes_per_pixel;
			x++;
		}
		y++;
	}
	TOCK(scale);
    // return target


	mlx_put_image_to_window(mlx->mlx, mlx->mlx_win, mlx->display_img, 0, 0);
}
