# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/10 19:18:28 by vitenner          #+#    #+#              #
#    Updated: 2024/04/10 19:18:28 by vitenner         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CC = gcc -g
CFLAGS = -Wall -Wextra -Werror -Isrc/
LDFLAGS = -lreadline

# Directories
SRC_DIR = src
LIBFT_DIR = libft
OBJ_DIR = obj

# Source files for minishell
MINISHELL_SRC = main.c \
tokens.c \
tokens_utils.c \
tokens_set_cmds.c \
signals.c \
commands.c \
commands2.c \
commands3.c \
debug_commands.c \
commands_cd.c \
commands_echo.c \
commands_env.c \
commands_exit.c \
commands_export.c \
commands_pwd.c \
commands_unset.c \
commands_utils.c \
externalcommands.c \
execution.c \
magicmalloc.c \
magicmalloc_terminate.c \
heredoc.c \
heredoc2.c \
parse_var.c \
pipex.c \
pipex2.c \
pipex3.c \
pipex4.c \
pipex_util.c \
pipex_utils2.c \
quotes.c \
quotes_double.c \
var_exp.c \
var_exp_utils.c \
var_operations.c \
var_operations_handlers.c \
var_operations_utils.c \
init.c \
modes.c \
modes_utils.c

LIBFT_FILES = ft_isdigit.c \
ft_isalnum.c \
ft_isalpha.c \
ft_isascii.c \
ft_isprint.c \
ft_isspace.c \
ft_strlen.c \
ft_memset.c \
ft_bzero.c \
ft_memcpy.c \
ft_memmove.c \
ft_strlcpy.c \
ft_strcpy.c \
ft_strncpy.c \
ft_strlcat.c \
ft_toupper.c \
ft_tolower.c \
ft_strchr.c \
ft_strrchr.c \
ft_strcmp.c \
ft_strncmp.c \
ft_memchr.c \
ft_memcmp.c \
ft_strnstr.c \
ft_atoi.c \
ft_atol.c \
ft_calloc.c \
ft_strdup.c \
ft_strndup.c \
ft_substr.c \
ft_strjoin.c \
ft_strtrim.c \
ft_split.c \
ft_itoa.c \
ft_strmapi.c \
ft_striteri.c \
ft_putchar_fd.c \
ft_putstr_fd.c \
ft_putstr.c \
ft_putendl_fd.c \
ft_putnbr_fd.c \
ft_putnbr_fd_ulong.c \
ft_putnbr_fd_longlong.c \
ft_lutoa.c \
ft_llutoa.c \
ft_ullonglen.c \
ft_ulonglen.c \
ft_abs.c \
ft_intlen.c \
ft_putnbr_base.c \
ft_putuintnbr_base.c \
ft_putulonglongnbr_base.c \
ft_printf.c \
ft_printf_processors_one.c \
ft_printf_processors_two.c \
ft_printf_utils.c \
get_next_line.c \
get_next_line_utils.c \
ft_lstadd.c \
ft_lstiter.c \
ft_lstnew.c \
ft_lstdelone.c \
ft_lstdel.c \
ft_lstmap.c \
ft_min.c \
ft_max.c \
old_gnl.c \
ft_memdel.c \
ft_strnew.c \
ft_strsub.c \
ft_strdel.c \
ft_strdel.c

# Adding directory prefix to source files
SRC = $(addprefix $(SRC_DIR)/, $(MINISHELL_SRC)) \
      $(addprefix $(LIBFT_DIR)/, $(LIBFT_FILES))

# Object files for minishell
OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

# Executable name for minishell
MINISHELL_EXEC = minishell

.PHONY: all clean fclean re

# Adjust the all rule to build minishell
all: $(MINISHELL_EXEC)

# Minishell executable rule
$(MINISHELL_EXEC): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Generic rule for object files, now correctly handles all sources
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(LIBFT_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(MINISHELL_EXEC)
	rm -rf $(OBJ_DIR)

re: fclean all
