/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   header.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 15:16:43 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/13 12:40:04 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

#include <stdlib.h>
#include <unistd.h>
#include <libft_types.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

# define MALLOC_FAIL -1
# define INVALID_FD_OR_MALLOC_FAIL -1
# define END_OF_FILE 0
# define SUCCESSFUL_READ 1

size_t				ft_strlen(const char *string);
int					ft_isupper(int c);
int					ft_islower(int c);
void				*ft_memset(void *b, int c, size_t len);
void				ft_bzero(void *s, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memccpy(void *dest, const void *src, int c, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
size_t				ft_strlcpy(char *dest, const char *src, size_t n);
size_t				ft_strlcat(char *dest, const char *src, size_t destsize);
char				*ft_strchr(const char *s, int c);
char				*ft_strrchr(const char *str, int c);
char				*ft_strnstr(const char *haystack,
		const char *needle, size_t len);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_atoi(const char *nptr);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isalpha(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);
int					ft_toupper(int c);
int					ft_tolower(int c);
void				*ft_calloc(size_t count, size_t size);
char				*ft_strdup(const char *s1);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *s, char c);
char				*ft_itoa(int n);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **alst, t_list *new);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **alst, t_list *new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
		void (*del)(void *));
int					ft_printf(char *str, ...);
int					get_next_line(int fd, char **line);
size_t				ft_strclen(const char *string, int c);
int					get_next_line(int fd, char **line);
int					ft_hex(const char *str);
long				ft_power(long n, int pow);
int					ft_printf_fd(int fd, const char *str, ...);
int					ft_abs(int x);
double				ft_abs_d(double x);
size_t				ft_strspn(const char *s, const char *accept);
size_t				ft_strcspn(const char *s, const char *reject);
char				*ft_strtok_r(char *str, const char *delim, char **save_ptr);
char				*ft_strtok(char *str, const char *delim);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_putchar(int c);
int					ft_is_whitespace(int c);
void				*ft_realloc(void *ptr, size_t oldsize, size_t newsize);

/*
** Vector Functions
*/

void				vector_map(t_vector *v, void (*f)(void *));
int					vector_add(t_vector *v, void *item);
int					vector_delete(t_vector *v, size_t index);
void				vector_free(t_vector *v);
int					vector_init(t_vector *v);
void				vector_set(t_vector *v, int index, void *item);
void				*vector_get(t_vector *v, int index);
size_t				vector_total(t_vector *v);

/*
** Vector string functions
*/

int					vecstr_resize(t_vecstr *v, size_t new_capacity);
int					vecstr_append_c(t_vecstr *v, int c);
int					vecstr_append_str(t_vecstr *v, char *str2);
int					vecstr_slice(t_vecstr *v, size_t start, size_t end);
int					vecstr_init(t_vecstr *v);
char				*vecstr_get(t_vecstr *v);
void				vecstr_set(t_vecstr *v, size_t index, int c);
int					vecstr_slice_by_needle(t_vecstr *v, const char *needle);
int					vecstr_insert_c(t_vecstr *v, size_t index, int c);
int					vecstr_insert_str(t_vecstr *v,
		size_t index, const char *str2);
size_t				vecstr_len(t_vecstr *v);
int					vecstr_truncate(t_vecstr *v, size_t truncpoint);
int					vecstr_reset(t_vecstr *v);
int					vecstr_val(t_vecstr *v, size_t i);

/*
** Vector array functions
*/

int					vecarr_resize(t_vecarr *v, size_t new_capacity);
int					vecarr_add(t_vecarr *v, void *new_elem);
int					vecarr_delete(t_vecarr *v, size_t index);
int					vecarr_init(t_vecarr *v, size_t elem_size);
void				*vecarr_get(t_vecarr *v, size_t index);
void				vecarr_reset(t_vecarr *v);
int					vecarr_delete_by_val(t_vecarr *v, void *val);

#endif
