/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 09:48:04 by fvonsovs          #+#    #+#             */
/*   Updated: 2024/03/21 13:46:04 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// free map later!
t_map   *parser(char *filename)
{
    t_map   *map;
    int     fd;
    char    *line;

    map = malloc_map();
    fd = open_file(filename);

    line = get_next_line(fd);
    while (line != NULL)
    {
        line = sanitize(line);
        parse_line(map, line);
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return (map);
}

char *sanitize(char *line)
{
    int i;
    char *str;

    i = -1;
    str = line;
    while (str && str[++i])
    {
        if (str[i] == '\t' || str[i] == '\n')
            str[i] = ' ';
    }
    line = ft_strtrim(str, " ");
    free(str);
    return (line);
}

int   parse_line(t_map *map, char *line)
{
    if (ft_strncmp(line, "A", 1) == 0)
        return (parse_ambient(map, line));
    if (ft_strncmp(line, "C", 1) == 0)
        return (parse_coords(map, line));
    return (0);
}

int parse_ambient(t_map *map, char *line)
{
    int     i;
    char    **params;

    i = 0;
    params = ft_split(line, ' ');
    if (array_length(params) != 3)
        error_throw("Invalid number of ambient params");
    map->amb = (t_amb *)malloc(sizeof(t_amb));
    if (map->amb == NULL)
        error_throw("Cannot malloc ambient lighting");
    ft_bzero(map->amb, sizeof(t_amb));
    while (params && params[++i])
    {
        if (i == 1 && parse_float(params[i], &map->amb->lum))
        {
            free(map->amb);
            return (error_throw("Cannot parse float"));
        }
        if (i == 2 && parse_color(params[i], &map->amb->col))
        {
            free(map->amb);
            return (error_throw("Cannot parse color"));
        }
    }
    free_array(params);
    return (0);
}

int parse_coords(t_map *map, char *line)
{
    int     i;
    char    **params;

    i = 0;
    params = ft_split(line, ' ');
    if (array_length(params) != 3)
        error_throw("Invalid number of camera params");
    map->cam = (t_cam *)malloc(sizeof(t_cam));
    if (map->cam == NULL)
        error_throw("Cannot malloc camera");
    ft_bzero(map->cam, sizeof(t_cam));
    while (params && params[++i])
    {
        if (i == 1 && parse_xyz(params[i], &map->cam->pos))
        {
            free(map->cam);
            return (error_throw("Cannot parse cam position"));
        }
        if (i == 2 && parse_color(params[i], &map->cam->col))
        {
            free(map->cam);
            return (error_throw("Cannot parse cam vector"));
        }
    }
    free_array(params);
    return (0);
}


int parse_xyz(char *str, t_float_3 *coord)
{
    char    **coords;
    float     xyz[3];
    int     i;
    int     x;

    i = 0;
    x = 0;
    coords = ft_split(str, ',');
    if (array_length(coords) < 1)
        return (0);
    while (coords && coords[i])
    {
        xyz[x] = ft_atoi(coords[i]);
        x++;
        i++;
    }
    coord->x = xyz[0];
    coord->y = xyz[1];
    coord->z = xyz[2];
    free_array(coords);
    return (0);
}

int	parse_float(char *str, float *num)
{
	if (!is_float(str))
		return (1);
	*num = str_to_float(str);
	return (0);
}

int	parse_ulong(char *str, size_t *num)
{
	if (!is_ulong(str))
		return (1);
	*num = (size_t)ft_atoi(str);
	return (0);
}
