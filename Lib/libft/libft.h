/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:40:49 by sgmih             #+#    #+#             */
/*   Updated: 2025/02/28 15:13:37 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>

size_t	ft_strlen(const char *s);
int		ft_atoi(const char *str);
char	*ft_strdup(const char *s1);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strrchr(const char *s, int c);

#endif