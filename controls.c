/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 19:24:39 by fvonsovs          #+#    #+#             */
/*   Updated: 2024/08/14 16:49:44 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	handle_destroy_notify(t_win *win)
{
	mlx_destroy_image(win->mlx, win->img);
	mlx_destroy_window(win->mlx, win->win);
	exit(0);
}

int	print_controls(t_win *win)
{
	mlx_string_put(win->mlx, win->win, 10, 10, 0xFFFFFF, "miniRT");
	return (0);
}

// quaternion rotation

// Function to create a quaternion from an axis and an angle
t_quaternion quaternion_from_axis_angle(t_float_3 axis, float angle)
{
	t_quaternion q;
	float half_angle = angle / 2.0f;
	float s = sin(half_angle);

	q.w = cos(half_angle);
	q.x = axis.x * s;
	q.y = axis.y * s;
	q.z = axis.z * s;

	return (q);
}

// Function to normalize a quaternion
t_quaternion quaternion_normalize(t_quaternion q)
{
	float magnitude = sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
	q.w /= magnitude;
	q.x /= magnitude;
	q.y /= magnitude;
	q.z /= magnitude;

	return (q);
}

// Function to multiply two quaternions
t_quaternion quaternion_multiply(t_quaternion q1, t_quaternion q2)
{
	t_quaternion result;

	result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
	result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;

	return (result);
}

// Function to rotate a vector by a quaternion
t_float_3 quaternion_rotate_vector(t_quaternion q, t_float_3 v)
{
	t_quaternion qv = {0, v.x, v.y, v.z};
	t_quaternion q_conjugate = {q.w, -q.x, -q.y, -q.z};

	t_quaternion q_result = quaternion_multiply(quaternion_multiply(q, qv), q_conjugate);

	return (t_float_3){q_result.x, q_result.y, q_result.z};
}

// arrow keys and k,l (z axis) to rotate objects
// WASDQE to move objects
int handle_keypress(int keysym, t_win *win)
{
	t_obj			*obj;
	t_quaternion	q;
	t_float_3		axis;
	float angle = RADIANS(1); // 1 degrees rotation

	if(keysym == KEY_ESC)
		handle_destroy_notify(win);
	else if (keysym == KEY_UP)
	{
		axis = (t_float_3){1.0, 0.0, 0.0}; // Rotate around x-axis
		q = quaternion_from_axis_angle(axis, angle);
		obj = win->map->objects;
		while(obj)
		{
			if (obj->type == SPHERE)
				((t_sp *)obj->object)->pos = quaternion_rotate_vector(q, ((t_sp *)obj->object)->pos);
			else if (obj->type == PLANE)
				((t_pl *)obj->object)->pos = quaternion_rotate_vector(q, ((t_pl *)obj->object)->pos);
			else if (obj->type == CYLINDER)
				((t_cy *)obj->object)->pos = quaternion_rotate_vector(q, ((t_cy *)obj->object)->pos);
			obj = obj->next;
		}
	}
		else if (keysym == KEY_DOWN)
	{
		axis = (t_float_3){-1.0, 0.0, 0.0}; // Rotate around x-axis in the opposite direction
		q = quaternion_from_axis_angle(axis, angle);
		obj = win->map->objects;
		while (obj)
		{
			if (obj->type == SPHERE)
				((t_sp *)obj->object)->pos = quaternion_rotate_vector(q, ((t_sp *)obj->object)->pos);
			else if (obj->type == PLANE)
				((t_pl *)obj->object)->pos = quaternion_rotate_vector(q, ((t_pl *)obj->object)->pos);
			else if (obj->type == CYLINDER)
				((t_cy *)obj->object)->pos = quaternion_rotate_vector(q, ((t_cy *)obj->object)->pos);
			obj = obj->next;
		}
	}
	else if (keysym == KEY_LEFT)
	{
		axis = (t_float_3){0.0, 1.0, 0.0}; // Rotate around y-axis
		q = quaternion_from_axis_angle(axis, angle);
		obj = win->map->objects;
		while (obj)
		{
			if (obj->type == SPHERE)
				((t_sp *)obj->object)->pos = quaternion_rotate_vector(q, ((t_sp *)obj->object)->pos);
			else if (obj->type == PLANE)
				((t_pl *)obj->object)->pos = quaternion_rotate_vector(q, ((t_pl *)obj->object)->pos);
			else if (obj->type == CYLINDER)
				((t_cy *)obj->object)->pos = quaternion_rotate_vector(q, ((t_cy *)obj->object)->pos);
			obj = obj->next;
		}
	}
	else if (keysym == KEY_RIGHT)
	{
		axis = (t_float_3){0.0, -1.0, 0.0}; // Rotate around y-axis in the opposite direction
		q = quaternion_from_axis_angle(axis, angle);
		obj = win->map->objects;
		while (obj)
		{
			if (obj->type == SPHERE)
				((t_sp *)obj->object)->pos = quaternion_rotate_vector(q, ((t_sp *)obj->object)->pos);
			else if (obj->type == PLANE)
				((t_pl *)obj->object)->pos = quaternion_rotate_vector(q, ((t_pl *)obj->object)->pos);
			else if (obj->type == CYLINDER)
				((t_cy *)obj->object)->pos = quaternion_rotate_vector(q, ((t_cy *)obj->object)->pos);
			obj = obj->next;
		}
	}
	else if (keysym == KEY_K)
	{
		axis = (t_float_3){0.0, 0.0, 1.0}; // Rotate around z-axis
		q = quaternion_from_axis_angle(axis, angle);
		obj = win->map->objects;
		while (obj)
		{
			if (obj->type == SPHERE)
				((t_sp *)obj->object)->pos = quaternion_rotate_vector(q, ((t_sp *)obj->object)->pos);
			else if (obj->type == PLANE)
				((t_pl *)obj->object)->pos = quaternion_rotate_vector(q, ((t_pl *)obj->object)->pos);
			else if (obj->type == CYLINDER)
				((t_cy *)obj->object)->pos = quaternion_rotate_vector(q, ((t_cy *)obj->object)->pos);
			obj = obj->next;
		}
	}
	else if (keysym == KEY_L)
	{
		axis = (t_float_3){0.0, 0.0, -1.0}; // Rotate around z-axis in the opposite direction
		q = quaternion_from_axis_angle(axis, angle);
		obj = win->map->objects;
		while (obj)
		{
			if (obj->type == SPHERE)
				((t_sp *)obj->object)->pos = quaternion_rotate_vector(q, ((t_sp *)obj->object)->pos);
			else if (obj->type == PLANE)
				((t_pl *)obj->object)->pos = quaternion_rotate_vector(q, ((t_pl *)obj->object)->pos);
			else if (obj->type == CYLINDER)
				((t_cy *)obj->object)->pos = quaternion_rotate_vector(q, ((t_cy *)obj->object)->pos);
			obj = obj->next;
		}
	}
	else if (keysym == KEY_W)
	{
		// win->map->cam.vec.y -= 0.1;
		// win->map->light.pos.y -= 5.0;
		obj = win->map->objects;
		while (obj)
        {
            if (obj->type == SPHERE)
                ((t_sp *)obj->object)->pos.y -= 0.1;
            else if (obj->type == PLANE)
                ((t_pl *)obj->object)->pos.y -= 0.1;
            else if (obj->type == CYLINDER)
                ((t_cy *)obj->object)->pos.y -= 0.1;
            obj = obj->next;
        }
	}
	else if (keysym == KEY_S)
	{
		// win->map->cam.vec.y += 0.1;
		// win->map->light.pos.y += 5.0;
		obj = win->map->objects;
		while (obj)
        {
            if (obj->type == SPHERE)
                ((t_sp *)obj->object)->pos.y += 0.1;
            else if (obj->type == PLANE)
                ((t_pl *)obj->object)->pos.y += 0.1;
            else if (obj->type == CYLINDER)
                ((t_cy *)obj->object)->pos.y += 0.1;
            obj = obj->next;
        }
	}
    else if (keysym == KEY_A)
    {
        // win->map->cam.vec.x -= 0.1;
        // win->map->light.pos.x -= 0.1;
        obj = win->map->objects;
        while (obj)
        {
            if (obj->type == SPHERE)
                ((t_sp *)obj->object)->pos.x -= 0.1;
            else if (obj->type == PLANE)
                ((t_pl *)obj->object)->pos.x -= 0.1;
            else if (obj->type == CYLINDER)
                ((t_cy *)obj->object)->pos.x -= 0.1;
            obj = obj->next;
        }
    }
    else if (keysym == KEY_D)
    {
        // win->map->cam.vec.x += 0.1;
        // win->map->light.pos.x += 0.1;
        obj = win->map->objects;
        while (obj)
        {
            if (obj->type == SPHERE)
                ((t_sp *)obj->object)->pos.x += 0.1;
            else if (obj->type == PLANE)
                ((t_pl *)obj->object)->pos.x += 0.1;
            else if (obj->type == CYLINDER)
                ((t_cy *)obj->object)->pos.x += 0.1;
            obj = obj->next;
        }
    }
    else if (keysym == KEY_Q)
    {
        // win->map->cam.vec.z -= 0.1;
        // win->map->light.pos.z -= 0.1;
        obj = win->map->objects;
        while (obj)
        {
            if (obj->type == SPHERE)
                ((t_sp *)obj->object)->pos.z -= 0.1;
            else if (obj->type == PLANE)
                ((t_pl *)obj->object)->pos.z -= 0.1;
            else if (obj->type == CYLINDER)
                ((t_cy *)obj->object)->pos.z -= 0.1;
            obj = obj->next;
        }
    }
    else if (keysym == KEY_E)
    {
        // win->map->cam.vec.z += 0.1;
        // win->map->light.pos.z += 0.1;
        obj = win->map->objects;
        while (obj)
        {
            if (obj->type == SPHERE)
                ((t_sp *)obj->object)->pos.z += 0.1;
            else if (obj->type == PLANE)
                ((t_pl *)obj->object)->pos.z += 0.1;
            else if (obj->type == CYLINDER)
                ((t_cy *)obj->object)->pos.z += 0.1;
            obj = obj->next;
        }
    }
	return (0);
}
