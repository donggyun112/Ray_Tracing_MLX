#include "../includes/struct.h"
#include "../includes/minirt.h"
#include "../includes/vector.h"

t_world	parse(char *av[])
{
	int fd;
	char *line;
	char **tmp;
	int i;

	i = 0;
	t_world	data;
	fd = open(av[1], O_RDONLY);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		tmp = ft_split(line, "		 \n,");
		i = 0;
		while (tmp[i])
		{
			if (!ft_strncmp(tmp[0], "R", 1))
			{
				data.width = ft_atoi(tmp[1]);
				data.height = ft_atoi(tmp[2]);
			}
			else if (!ft_strncmp(tmp[0], "sp", 2))
			{
				data.sphere.sphere.x = ft_atoi(tmp[1]);
				data.sphere.sphere.y = ft_atoi(tmp[2]);
				data.sphere.sphere.z = ft_atoi(tmp[3]);
				data.sphere.r = ft_atoi(tmp[4]);
			}
			i++;
		}
		i = 0;
		while (tmp[i])
		{
			free(tmp[i]);
			i++;
		}
		free(tmp);
		free(line);
	}
	int a = 3;
}

int main(int ac, char *av[])
{
	(void)ac;
	parse(av);
	// t_ray3	ray;

	// for (int j = 0; j < /* 세로 */; j++)
	// {
	// 	for (int i = 0; i < /* 가로 */; i++)
	// 	{
	// 		double u = double(i) / (/* 가로 - 1 */);
	// 		double v = double(j) / (/* 세로 - 1 */);
	// 		ray = create_ray(cam, u, v);
	// 	}
	// }
}