/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 12:15:55 by vitenner          #+#    #+#             */
/*   Updated: 2024/01/25 14:27:24 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include "libft.h"
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdarg.h>

/*
** ------------------------- CORE -------------------------
*/

int					ft_atoi(const char *str);
void				*ft_bzero(void *b, size_t n);
void				*ft_calloc(size_t nitems, size_t size);
int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				*ft_memcpy(void *dst, const void *src, size_t n);
void				*ft_memmove(void *dst, const void *src, size_t len);
void				*ft_memset(void *b, int c, size_t n);
char				*ft_strchr(const char *s, int c);
char				*ft_strdup(const char *s1);
size_t				ft_strlcat(char *dst, const char *src, size_t size);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
size_t				ft_strlen(const char *s);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strnstr(const char *big, const char *litl, size_t len);
char				*ft_strrchr(const char *s, int c);
int					ft_tolower(int c);
int					ft_toupper(int c);

/*
** ------------------------- ADDITIONAL -------------------------
*/
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *s, char c);
char				*ft_itoa(int n);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *str, int fd);
void				ft_putendl_fd(char const *s, int fd);
void				ft_putnbr_fd(int n, int fd);
int					ft_strcmp(char *s1, char *s2);
long				ft_atol(const char *str);
int					ft_max(int a, int b);
int					ft_min(int a, int b);

/*
** ------------------------- MINE -------------------------
*/

int					ft_abs(int n);
void				ft_putstr(char *str);
void				ft_putnbr_fd_ulong(unsigned long long nb, int fd);
void				ft_putnbr_fd_longlong(long long nb, int fd);
char				*ft_lutoa(unsigned long n);
char				*ft_llutoa(unsigned long long n);
int					ft_intlen(int n);
int					ft_ulonglen(unsigned long n);
int					ft_ullonglen(unsigned long long n);
void				ft_putnbr_base(int nbr, char *base);
void				ft_putuintnbr_base(unsigned int nbr, char *base);
void				ft_putulonglongnbr_base(unsigned long long nbr, char *base);
char				*ft_strncpy(char *dest, char *src, unsigned int n);
char				*ft_strndup(const char *src, int n);

/*
** ------------------------- FT_PRINTF -------------------------
*/

typedef struct s_struct
{
	int			(*printfunc)(va_list lst);
}	t_struct;

int					ft_printf(const char *format, ...);

void				ft_set_g_formats(void);
int					ft_is_s(va_list arg);
int					ft_is_p(va_list arg);
int					ft_is_d(va_list arg);
int					ft_is_i(va_list arg);
int					ft_is_u(va_list arg);
int					ft_is_x(va_list arg);
int					ft_is_cap_x(va_list arg);
int					ft_is_c(va_list arg);
int					ft_is_percentage(va_list arg);
char				*ft_uint_to_hex(unsigned int num, int caps);
int					ft_hex_length(unsigned int num);
int					ft_build_address(unsigned long long num);
int					ft_putchar_count(char c);

/*
** ------------------------- GNL -------------------------
*/

char				*get_next_line(int fd);
void				*ft_calloc_set_zero(size_t nmemb, size_t size);

/*
** ------------------------- LISTS -------------------------
*/

typedef struct s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
}					t_list;

void				ft_lstadd(t_list **alst, t_list *new);
void				ft_lstiter(t_list *lst, void (*f)(t_list *elem));
t_list				*ft_lstnew(void const *content, size_t content_size);
void				ft_lstdelone(t_list **alst, void (*del)(void *, size_t));
void				ft_lstdel(t_list **alst, void (*del)(void *, size_t));
t_list				*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));

#endif