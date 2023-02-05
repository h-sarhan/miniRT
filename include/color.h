/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 23:17:02 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/05 21:04:53 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_H
# define COLOR_H

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

unsigned int	create_mlx_color(t_color *color);
void			add_colors(t_color *res, const t_color *c1, const t_color *c2);
void			mult_color(t_color *res, const t_color *color, double val);
void			blend_colors(t_color *res, const t_color *c1,
					const t_color *c2);

int				color_mix(int c1, int c2, double mix);
int				color_difference(int c1, int c2);
void			sub_colors(t_color *res, const t_color *c1, const t_color *c2);

typedef struct s_phong	t_phong;
struct	s_phong
{
	t_color	effective_color;
	t_color	diffuse;
	t_color	specular;
	t_color	ambient;
};

#endif
