/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 09:48:04 by fvonsovs          #+#    #+#             */
/*   Updated: 2024/02/20 19:49:58 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_map   parser(char *filename)
{
    t_map   *map;
    int     fd;
    char    *line;

    fd = open_file(filename);
    line = get_next_line(fd);

    while (line != NULL)
	{
        map = parse_line(&line, &map);
        free(line);
        line = get_next_line(fd);
    }
    
    close(fd);
    return (map);
}

t_map   parse_line(char *line, t_map *map)
{



}