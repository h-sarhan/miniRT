/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_whitespace.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 17:14:07 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/17 17:26:46 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static bool	is_whitespace(const char c)
{
	return (c == ' ' || c == '\f'
			|| c == '\n' || c == '\r'
			|| c == '\t' || c == '\v');
}

static int	count_words(char const *str)
{
	int	i;
	int	num_words;

	i = 0;
	num_words = 0;
	while (str[i] != '\0')
	{
		while (is_whitespace(str[i]) == true && str[i] != '\0')
			i++;
		if (str[i] == '\0')
			break ;
		num_words++;
		while (is_whitespace(str[i]) == false && str[i] != '\0')
			i++;
	}
	return (num_words);
}

static char	*create_word(char const *str, int word_start, int word_end)
{
	char	*word;

	word = malloc(sizeof(char) * (word_end - word_start + 2));
	if (word == NULL)
		return (NULL);
	ft_strlcpy(word, &str[word_start], word_end - word_start + 2);
	return (word);
}

char	**ft_split_whitespace(char const *s)
{
	char	**words;
	int		i;
	int		word_start;
	int		word_count;

	words = malloc(sizeof(char *) * (count_words(s) + 1));
	if (words == NULL)
		return (NULL);
	i = 0;
	word_count = 0;
	while (s[i] != '\0')
	{
		while (is_whitespace(s[i]) == true && s[i] != '\0')
			i++;
		if (s[i] == '\0')
			break ;
		word_start = i;
		while (is_whitespace(s[i]) == false && s[i] != '\0')
			i++;
		words[word_count] = create_word(s, word_start, i - 1);
		if (words[word_count++] == NULL)
			return (NULL);
	}
	words[word_count] = NULL;
	return (words);
}
