#include "../../includes/minishell.h"

void	close_extra_fds(void)
{
	int	last_open_fd;

	last_open_fd = open("/tmp/last_fd", O_RDWR | O_CREAT, 0666);
	if (last_open_fd == -1)
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", "/tmp/last_fd", strerror(errno));
		// print_perror_msg("open", "/tmp/last_fd");
	while (last_open_fd > STDERR_FILENO)
	{
		close(last_open_fd);
		last_open_fd--;
	}
}

void	close_all_fds(void)
{
	close_extra_fds();
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void	redirect_fd(int fd_to_redirect, int fd_location)
{
	dup2(fd_to_redirect, fd_location);
	close(fd_to_redirect);
}

void	redirect_fds(int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO)
		redirect_fd(fd_in, STDIN_FILENO);
	if (fd_out != STDOUT_FILENO)
		redirect_fd(fd_out, STDOUT_FILENO);
}

void	restore_original_fds(int original_fds[2])
{
	if (original_fds[IN] != -1)
		redirect_fd(original_fds[IN], STDIN_FILENO);
	if (original_fds[OUT] != -1)
		redirect_fd(original_fds[OUT], STDOUT_FILENO);
}


// ==66806== Process terminating with default action of signal 11 (SIGSEGV)
// ==66806==  Access not within mapped region at address 0x0
// ==66806==    at 0x4097AC: ft_strcmp (ft_strcmp.c:29)
// ==66806==    by 0x4048DE: is_redir_in (lexer_utils.c:5)
// ==66806==    by 0x404A04: is_redir (lexer_utils.c:33)
// ==66806==    by 0x4045AA: check_redirects (lexer.c:94)
// ==66806==    by 0x404893: lexer (lexer.c:181)
// ==66806==    by 0x403C17: parse (parser.c:97)
// ==66806==    by 0x403260: readline_and_handle_input (token.c:129)
// ==66806==    by 0x4067C7: main (main.c:35)
// ==66806==  If you believe this happened as a result of a stack
// ==66806==  overflow in your program's main thread (unlikely but
// ==66806==  possible), you can try to increase the size of the
// ==66806==  main thread stack using the --main-stacksize= flag.
// ==66806==  The main thread stack size used in this run was 8388608.