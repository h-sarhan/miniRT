/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:06:31 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/27 13:08:31 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLISIONS_H
# define COLLISIONS_H

typedef struct s_scene t_scene;

#include "shape.h"

bool	collide(t_scene *scene, bool resolve, int depth,
			t_shape *transformed_shape);
void	collide_after_transform(t_scene *scene);
bool	gjk(t_shape *s1, t_shape *s2);

#endif