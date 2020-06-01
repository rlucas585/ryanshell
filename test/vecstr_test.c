#include <libft.h>

int		main(void)
{
	t_vecstr	line;
	char		*tmp;
	size_t		i;

	vecstr_init(&line);
	vecstr_insert_str(&line, vecstr_len(&line), "A vector string");
	ft_printf("%s\n", vecstr_get(&line));
	vecstr_insert_str(&line, vecstr_len(&line), " that has now become longer");
	ft_printf("%s\n", vecstr_get(&line));
	vecstr_slice_by_needle(&line, " longer");
	ft_printf("%s\n", vecstr_get(&line));
	vecstr_slice_by_needle(&line, "word");
	ft_printf("%s\n", vecstr_get(&line));
	vecstr_slice_by_needle(&line, "string ");
	ft_printf("%s\n", vecstr_get(&line));
	vecstr_slice_by_needle(&line, "A ");
	vecstr_slice_by_needle(&line, " that has now become");
	ft_printf("%s\n", vecstr_get(&line));
	vecstr_insert_str(&line, 3, "tors are really cool! I love vec");
	ft_printf("%s\n", vecstr_get(&line));
	ft_printf("len = %u\n", ft_strlen(vecstr_get(&line)));
	ft_printf("len = %u\n", vecstr_len(&line));
	i = 3;
	ft_printf("val at %u: %c\n", i, vecstr_val(&line, i));
	return (0);
}
