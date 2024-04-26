# Name of the executable
NAME = minishell
NAME_BONUS = minishell_bonus
NAME_BONUS = minishell_bonus

# Compilation flags
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I ./includes/ -I $(LIBFT_PATH)/ -I $(FTPRINTF_PATH)/

# Libft
LIBFT_PATH	=	./libs/libft
LIBFT		=	$(LIBFT_PATH)/libft.a

# Libftprintf
FTPRINTF_PATH	= ./libs/ft_printf
FTPRINTF		=	$(FTPRINTF_PATH)/libftprintf.a


SRC = 	./src/builtins/cd.c \
		./src/builtins/echo.c \
		./src/builtins/env.c \
		./src/builtins/export.c \
		./src/builtins/exec_builtins.c \
		./src/builtins/exit.c \
		./src/builtins/pwd.c \
		./src/builtins/unset.c \
		./src/token/token.c \
		./src/token/token_utils.c \
		./src/parse/parser.c \
		./src/parse/parser_utils.c \
		./src/parse/parser_utils2.c \
		./src/lexer/lexer.c \
		./src/lexer/lexer_utils.c \
		./src/parse/parser_utils2.c \
		./src/lexer/lexer.c \
		./src/lexer/lexer_utils.c \
		./src/expand/expander.c \
		./src/expand/expander_utils.c \
		./src/expand/heredoc_expander.c \
		./src/signals/signals.c \
		./src/expand/heredoc_expander.c \
		./src/signals/signals.c \
		./src/main/main.c \
		./src/main/main_utils.c \
		./src/redirections/handle_redir.c \
		./src/redirections/redir.c \
		./src/redirections/redir_utils.c \
		./src/execution/exec.c \
		./src/execution/exec_utils.c \
		./src/execution/exec_one_process.c \
		./src/execution/exec_processes.c

# Objects
OBJ_PATH = src/build/
OBJ = $(addprefix $(OBJ_PATH), $(notdir $(SRC:.c=.o)))
OBJ_PATH_BONUS = ./build_bonus/
OBJ_BONUS = $(addprefix $(OBJ_PATH_BONUS), $(notdir $(SRC_BONUS:.c=.o)))
OBJ_PATH_BONUS = ./build_bonus/
OBJ_BONUS = $(addprefix $(OBJ_PATH_BONUS), $(notdir $(SRC_BONUS:.c=.o)))

# Color codes of output on terminal
GREEN = \e[1;32m
WHITE = \033[1;37m
CYAN= \033[0;36m
END = \033[0m

# Default target to compile the mandatory part
all: $(NAME)

# Target to build the mandatory part
$(NAME): $(OBJ) $(LIBFT) $(FTPRINTF)
# 	@cp $(LIBFT) $(NAME)
# 	$(CC) $(OBJ) $(LIBFT) -L$(PATH_LIBFT) -o $(NAME)
	@$(CC) $(CFLAGS) $(OBJ) -o $@ -L$(FTPRINTF_PATH) -lftprintf -L$(LIBFT_PATH) -lft -L ../../../../usr/include -lreadline
#	@$(CC) $(CFLAGS) $(OBJ) -o $@ -L$(FTPRINTF_PATH) -lftprintf -L$(LIBFT_PATH) -lft
	@echo "$(GREEN)--------------------------------------------------$(END)"
	@echo "$(GREEN)The [$(CYAN)MINI-SHELL$(GREEN)] has been compiled! 🐚🌊$(END)"
	@echo "$(GREEN)--------------------------------------------------$(END)"

valgrind: all
	@echo "Running the program with valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --track-origins=yes --track-fds=yes --suppressions=readline.supp ./minishell

run: all
	@echo "Running the program..."
	./minishell

bonus: $(NAME_BONUS)

# Target to build the bonus part
$(NAME_BONUS): $(OBJ_BONUS) $(LIBFT) $(FTPRINTF)
# 	@cp $(LIBFT) $(NAME_BONUS)
# 	$(CC) $(OBJ) $(LIBFT) -L$(PATH_LIBFT) -o $(NAME_BONUS)
	@$(CC) $(CFLAGS) $(OBJ) -o $@ -L$(FTPRINTF_PATH) -lftprintf -L$(LIBFT_PATH) -lft -L ../../../../usr/include -lreadline
#	@$(CC) $(CFLAGS) $(OBJ) -o $@ -L$(FTPRINTF_PATH) -lftprintf -L$(LIBFT_PATH) -lft
	@echo "$(GREEN)--------------------------------------------------$(END)"
	@echo "$(GREEN)The [$(CYAN)MINI-SHELL$(GREEN)] has been compiled! 🐚🌊$(END)"
	@echo "$(GREEN)--------------------------------------------------$(END)"

bonus: $(NAME_BONUS)

# Target to build the bonus part
$(NAME_BONUS): $(OBJ_BONUS) $(LIBFT) $(FTPRINTF)
# 	@cp $(LIBFT) $(NAME_BONUS)
# 	$(CC) $(OBJ) $(LIBFT) -L$(PATH_LIBFT) -o $(NAME_BONUS)
	@$(CC) $(CFLAGS) $(OBJ) -o $@ -L$(FTPRINTF_PATH) -lftprintf -L$(LIBFT_PATH) -lft -L ../../../../usr/include -lreadline
#	@$(CC) $(CFLAGS) $(OBJ) -o $@ -L$(FTPRINTF_PATH) -lftprintf -L$(LIBFT_PATH) -lft
	@echo "$(GREEN)--------------------------------------------------$(END)"
	@echo "$(GREEN)The [$(CYAN)MINI-SHELL$(GREEN)] has been compiled! 🐚🌊$(END)"
	@echo "$(GREEN)--------------------------------------------------$(END)"

# Builds dependencies
$(LIBFT):
	@make -C $(LIBFT_PATH)
$(FTPRINTF):
	@make -C $(FTPRINTF_PATH)

# Builds mandatory object files
$(OBJ_PATH)%.o: src/**/%.c
	@mkdir -p $(OBJ_PATH)
	@mkdir -p .tmp/
	$(CC) -c $(CFLAGS) $< -o $@ $(INCLUDES)

# Builds bonus object files
$(OBJ_PATH_BONUS)%.o: src/**/%.c
	@mkdir -p $(OBJ_PATH_BONUS)
	@mkdir -p .tmp/
	$(CC) -c $(CFLAGS) $< -o $@ $(INCLUDES)

# Builds bonus object files
$(OBJ_PATH_BONUS)%.o: src/**/%.c
	@mkdir -p $(OBJ_PATH_BONUS)
	$(CC) -c $(CFLAGS) $< -o $@ $(INCLUDES)

# Cleans object files and dependencies
clean:
	@rm -f $(OBJ)
	@rm -rf $(OBJ_PATH)
	@make -C $(LIBFT_PATH) clean
	@make -C $(FTPRINTF_PATH) clean
	@echo "$(WHITE)-----------------------------------$(END)"
	@echo "$(WHITE)    Object files were cleaned!$(END)"
	@echo "$(WHITE)-----------------------------------$(END)"

# Cleans the executable file and pre-compiled header
fclean: clean
	@echo "$(WHITE)-----------------------------------$(END)"
	@echo "$(WHITE)        Cleaning all... 🧹$(END)"
	@echo "$(WHITE)-----------------------------------$(END)"
	@rm -f $(NAME)
	@make -C $(LIBFT_PATH) fclean
	@make -C $(FTPRINTF_PATH) fclean
	@echo "$(WHITE)-----------------------------------$(END)"
	@echo "$(WHITE)    ✨ Cleaning complete! ✨$(END)"
	@echo "$(WHITE)-----------------------------------$(END)"

# Cleans everything generated by the Makefile and rebuild it all
re: fclean all

# Phony targets
.PHONY: all bonus clean fclean re run valgrind
