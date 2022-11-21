/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 17:16:48 by mkhan             #+#    #+#             */
/*   Updated: 2022/11/21 18:00:26 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAY_H
# define RAY_H

# include <vector.h>

typedef struct s_ray t_ray; 

struct s_ray {
	t_vector origin;
	t_vector direction;
};



#endif