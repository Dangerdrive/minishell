/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 11:57:43 by gde-souz          #+#    #+#             */
/*   Updated: 2024/03/08 13:23:06 by gde-souz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_global	g_data;

int	main(void)
{
	char	*str;

	while (*__environ)
	{
		printf("%s\n", *__environ);
		__environ++;
	}
	str = readline("hello");
	printf("%s", str);
}
