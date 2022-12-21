/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_input.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 19:44:18 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/21 18:25:34 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEY_INPUT_H
# define KEY_INPUT_H

# include "scene.h"
# define ON_DESTROY 17
# define PI           3.14159265358979323846F
# ifdef __linux__
#  define KEY_ESC 65307
#  define KEY_W 119
#  define KEY_A 97
#  define KEY_S 115
#  define KEY_D 100
#  define KEY_O 111
#  define KEY_LEFT 65361
#  define KEY_RIGHT 65363
#  define KEY_UP 65362
#  define KEY_DOWN 65364
#  define KEY_Q 113
#  define KEY_E 101
#  define KEY_PLUS 61
#  define KEY_MINUS 45
#  define KEY_P 112
#  define KEY_C 99
#  define KEY_TAB 65289
#  define KEY_SPACE 32
#  define KEY_R 114
#  define KEY_M 109
#  define KEY_1 49
#  define KEY_2 50
#  define KEY_3 51
#  define KEY_4 52
#  define KEY_5 53
#  define KEY_6 54
#  define KEY_7 55
#  define KEY_8 56
#  define KEY_9 57
#  define KEY_RETURN 65293
#  define KEY_SHIFT 0
# else
#  define KEY_ESC 53
#  define KEY_W 13
#  define KEY_A 0
#  define KEY_S 1
#  define KEY_D 2
#  define KEY_O 31
#  define KEY_LEFT 123
#  define KEY_RIGHT 124
#  define KEY_UP 126
#  define KEY_DOWN 125
#  define KEY_Q 12
#  define KEY_E 14
#  define KEY_PLUS 24
#  define KEY_MINUS 27
#  define KEY_P 35
#  define KEY_C 8
#  define KEY_TAB 48
#  define KEY_SPACE 49
#  define KEY_R 15
#  define KEY_M 46
#  define KEY_1 18
#  define KEY_2 19
#  define KEY_3 20
#  define KEY_4 21
#  define KEY_5 23
#  define KEY_6 22
#  define KEY_7 26
#  define KEY_8 28
#  define KEY_9 25
#  define KEY_RETURN 36
#  define KEY_SHIFT 257
# endif
int	key_handler(t_scene *scene);
int	mouse_rotate(t_scene *scene);

#endif