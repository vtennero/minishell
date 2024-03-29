/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 16:17:01 by cliew             #+#    #+#             */
/*   Updated: 2024/03/29 09:38:02 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void free_cmd(Command *cmd){
	free(cmd->cmd_path);
}
int builtin_cmd(Command *command,t_shell* shell)
{
	int exit_code; 
	exit_code=-99999;
	if (ft_strcmp(command->name, "cd") == 0) {
        exit_code=builtin_cd(shell, command->args, command->arg_count);
    } else if (ft_strcmp(command->name, "unset") == 0) {
		exit_code=builtin_unset(shell, command->args, command->arg_count);
	} else if (ft_strcmp(command->name, "export") == 0 && command->fout !=-99) {
		exit_code=builtin_export(shell, command->args, command->arg_count);
    } else if (ft_strcmp(command->name, "exit") == 0) {
        exit_code=builtin_exit(shell, command->args, command->arg_count);}
	if (exit_code==-99999)
		return 0;
	shell->last_exit_status=exit_code;
	return 1;
}



int custom_cmd(char** cmd_args,char* cmd_path,Command *cmd,t_shell *shell)
{
	int exit_code;

	exit_code=-999;
	if (ft_strcmp(cmd->name, "pwd") == 0) {
        exit_code=builtin_pwd();
    } else if (ft_strcmp(cmd->name, "echo") == 0) {
        exit_code=builtin_echo(shell, cmd->args, cmd->arg_count,cmd);
    } else if (ft_strcmp(cmd->name, "env") == 0) {
		exit_code=builtin_env(shell);}
	 else if (ft_strcmp(cmd->name, "export") == 0 && cmd->fout ==-99) {
		exit_code=builtin_export(shell, cmd->args, cmd->arg_count);

    } 
	if (exit_code!=-999)
		exit(exit_code);
	else if (cmd_path)
	{
		execve(cmd_path, cmd_args, shell->envp);
		free(cmd_path);
		free_array(cmd_args);
		exit(EXIT_FAILURE);
	}
	
	else
		return 0;
}

int	run_cmd(Command *command,t_shell* shell)
{
	int		status;

	status = 0;
	if (!custom_cmd(command->execv_args,command->cmd_path,command,shell))  // If succed,, wont come back out. If fail, exit from insidebuiltin.custom
		{
			shell->last_exit_status=status;
			return (ft_puterr(ft_strjoin_nconst(command->name, ERR_INVALID_CMD), 127));
		}
	return 0;
}
 



void check_finfout(int prev_pipe,Command *cmd,t_shell *shell)
{
	if (cmd->fin ==  -99 && prev_pipe != STDIN_FILENO)
		dup2(prev_pipe, STDIN_FILENO);
	else if (cmd->fin !=0)
	{
		dup2(cmd->fin, STDIN_FILENO);
		close(cmd->fin);
	}
	else if (cmd->fin==0)
		dup2(shell->std_in, STDIN_FILENO);
	if (cmd->fout ==  -99 && shell->pipefd[1] != STDOUT_FILENO)
	{
		dup2(shell->pipefd[1], STDOUT_FILENO);
		close(shell->pipefd[1]);
	}
	else if (cmd->fout !=1 && cmd->fout!=0 && cmd->fout !=-99)
		dup2(cmd->fout, STDOUT_FILENO);
	else if (cmd->fout==0)
		dup2(shell->std_out, STDOUT_FILENO);

	if (shell->pipefd[0] != STDOUT_FILENO && shell->pipefd[0] != STDIN_FILENO)
		close(shell->pipefd[0]);
}

void clean_fd(t_shell *shell,int std_in,int std_out,Command* cmd)
{
	close(shell->pipefd[0]);
	close(shell->pipefd[1]);
	if (dup2(std_in, STDIN_FILENO) == -1) {
			perror("dup2");
		}
	if (dup2(std_out, STDOUT_FILENO) == -1) {
			perror("dup2");
		}
	free_cmd(cmd);

}

void handle_status_error(int status,Command *cmd,t_shell *shell)
{
	int status2;

	if (WIFEXITED(status) && ft_strcmp(cmd->name,"exit")!=0 )
	{
			status2=  WEXITSTATUS(status);
			shell->last_exit_status=status2;
		}
		else
				shell->last_exit_status=status;
}


int assign_cmd_args(t_shell *shell,Command *command, char **envp)
{
	int i;
	char	**paths;

	i=1;
	paths = find_cmd_paths(envp);
	command->cmd_path = locate_cmd(paths, command->name);
	free_array(paths);

	command->execv_args=shell_malloc(shell,(command->arg_count+2) * sizeof(char *));
	// command->execv_args=(char **)malloc((command->arg_count+2) * sizeof(char *));
	command->execv_args[0]=command->name;
	while (i <= command->arg_count)
	{
		command->execv_args[i] = command->args[i-1];
		i++;	
	}
	command->execv_args[i]= 0;
	return 0;
}



int	execute_command_pipex(int prev_pipe,Command *cmd,t_shell *shell)
{
	pid_t	pid;
	char* error;

	error=NULL;
	assign_cmd_args(shell,cmd,shell->envp);
	if (builtin_cmd(cmd,shell))
		return 1;
	pid = fork();
	if (pid < 0)
		return (write(STDOUT_FILENO, "Error forking\n", 15));
	if (pid == 0)
	{		
		signal(SIGINT, SIG_DFL); // Reset SIGINT to default handling
		signal(SIGQUIT, SIG_DFL); // Reset SIGQUIT to default handling
		if (cmd ->fin ==-1)
			error=ft_strjoin_nconst(cmd->redirect_in, " : File not exists/permission error" );
		else if (cmd ->fout ==-1)
		    ft_puterr(ft_strjoin_nconst(cmd->redirect_out, " : File not exists/permission error" ), 1);
		if (error!=NULL)
		{
			ft_putstr_fd(error,2);	ft_puterr(error, 1);
			free(error);
			exit(1);
		}
		check_finfout(prev_pipe,cmd,shell);
		run_cmd(cmd,shell);
		exit(1);
	}
	else
		{
			shell->pid=pid;
			return 0;
		}		

}








int pipex(Command *cmd,t_shell *shell) {
	int status;
	int prev_pipe;

	prev_pipe = cmd->fin;
	while (cmd->next) 
	{
		if (pipe(shell->pipefd) < 0)
			return (write(STDOUT_FILENO, "Error creating pipe\n", 20));
		if (cmd->fout==0)
		{
			if (!execute_command_pipex(prev_pipe,cmd,shell))
				waitpid(shell->pid,&status,WUNTRACED);
		}
		else
			execute_command_pipex(prev_pipe,cmd,shell);
		prev_pipe=shell->pipefd[0];
 		// close(shell->pipefd[0]);    -> THIS TESTED TO NOT WORKING, WILL BLOCK PIPE
		close(shell->pipefd[1]);
		free_cmd(cmd);
		*cmd = *(cmd->next);
	}
	if (!execute_command_pipex(prev_pipe,cmd,shell))
	{
		waitpid(shell->pid,&status,WUNTRACED);
		handle_status_error(status,cmd,shell);
	}
	clean_fd(shell,shell->std_in,shell->std_out,cmd);
	return 0;
}





// int pipex(Command *cmd,t_shell *shell) {
// 	int status;
// 	int prev_pipe;
// 	int original_stdin = dup(STDIN_FILENO);
// 	int original_stdout = dup(STDOUT_FILENO);

// 	if (!find_env_var(shell->env_head, "PATH"))
//     {
//         perror("Command not found");
//         shell->last_exit_status = 127;
//         return 1;
//     }
// 	status = 0;
// 	prev_pipe = cmd->fin;
// 	while (cmd->next) {
// 		status = execute_command_pipex(prev_pipe,cmd,shell);
// 		prev_pipe = shell->pipefd[0];
// 		if (cmd ->fin ==-1)
// 		    ft_puterr(ft_strjoin_nconst(cmd->redirect_in, " : File not exists/permission error" ), 1);
// 		if (cmd ->fout ==-1)
// 		    ft_puterr(ft_strjoin_nconst(cmd->redirect_out, " : File not exists/permission error" ), 1);

// 		*cmd = *(cmd->next);
// 		if (status < 0)
// 			exit(-1);
// 	}

// 			// waitpid(0, NULL, WNOHANG | WUNTRACED);
// 		// waitpid(0, NULL, WUNTRACED);


// 		if (cmd->fin == -99)
// 		{
// 				dup2(prev_pipe, STDIN_FILENO);
// 				close(prev_pipe);
// 				// close(in.pipefd[1]);
// 		}



// 		else if (cmd->fin !=0)
// 		{

// 			dup2(cmd->fin, STDIN_FILENO);
// 			// close(cmd->fin);
// 		}
// 		if (cmd->fout !=0 && cmd->fout !=-99)
// 		{
// 			dup2(cmd->fout, STDOUT_FILENO);
// 			close(cmd->fout);
// 		}

// 		if (ft_strcmp(cmd->name,"exit")==0)
// 		{
// 			status = run_cmd(cmd, shell->envp,shell);
// 		}
	
// 	int pid2 = fork();
// 	if (pid2 < 0)
// 		return (write(STDOUT_FILENO, "Error forking\n", 15));
// 	if (pid2 == 0)
// 	{
// 		if (cmd ->fin ==-1)
// 		    ft_puterr(ft_strjoin_nconst(cmd->redirect_in, " : File not exists/permission error" ), 1);
// 		if (cmd ->fout ==-1)
// 		    ft_puterr(ft_strjoin_nconst(cmd->redirect_out, " : File not exists/permission error" ), 1);
// 		status = run_cmd(cmd, shell->envp,shell);
// 		exit(0);
// 	}
// 	// (void)shell;
// 	if (pid2 > 0)
// 	{
// 		// waitpid(0,  &status, WUNTRACED );
// 				// waitpid(0, &status, WNOHANG | WUNTRACED );
// 		// waitpid(0, &status, 0);
// 		waitpid(pid2, &status,  WUNTRACED );

// 		if (WIFEXITED(status) && ft_strcmp(cmd->name,"exit")!=0 )
// 		{

// 			// ft_putstr_fd(ft_strjoin_nconst("CMD STATUS IS ",ft_itoa((status))),2);

// 			int status2=  WEXITSTATUS(status);
// 			shell->last_exit_status=status2;
// 		}
// 		else
// 				shell->last_exit_status=0;

// 		// ft_putstr_fd(ft_strjoin_nconst("CMD STATUS IS ",ft_itoa(status)),2);

// 		// if (WIFEXITED(status))
// 		// 	status = WEXITSTATUS(status);

// 		// ft_putstr_fd(ft_strjoin_nconst("STATUS IS ",ft_itoa(shell->last_exit_status)),2);

// 		// waitpid(-1,  &status, WNOHANG );
// 		// waitpid(0, &status, 0 );
// 		if (prev_pipe!=STDIN_FILENO)
// 			close(prev_pipe);
// 		// close(in.pipefd[0]);
// 		// close(in.pipefd[1]);

// 		// close(cmd->fin);
// 		// close(cmd->fout);
// 	}
// 	if (dup2(original_stdin, STDIN_FILENO) == -1) {
// 			perror("dup2");
// 			exit(EXIT_FAILURE);
// 		}
// 		close(original_stdin);
// 	if (dup2(original_stdout, STDOUT_FILENO) == -1) {
// 			perror("dup2");
// 			exit(EXIT_FAILURE);
// 		}
// 		close(original_stdout);



// 	return  WEXITSTATUS(status);
// }


// Current state of pipes
// Initiialize t_in structure for the following 3
// in.argv = argv;
// 	in.envp = envp;
// 	in.argc = argc;
//  Pass in pointer to the first cmd in cmd table.
// Needed parameter in cmd table 

//     cmd1.name = "echo";
//     cmd1.args = (char*[]){ " hello", NULL}; // Note: args should be an array of strings
//     cmd1.fin = 0;
//     cmd1.fout = -99;
//     
//    fin and fout is the fd_in and fd_out. if 0, means theres no redirection (\n case). if -99, means theres pipe)
//    eg. if cmd1 | cmd2\ncmd3
//    then 
//          cmd1.fin  = 0;
//          cmd1.fout = -99;
//          cmd2.fin  = -99
//          cmd2.fout = 0;
//          cmd3.fin =  0; 
//          cmd3.fout  = 0;



//--------------------------------------------------------------------------
// int	main(int argc, char **argv, char **envp)
// // int	main()x`

// {
// 	t_in	in;
// 	int		status;

// 	Command cmd1;
// 	Command cmd2;
// 	Command cmd3;

//     // Initialize cmd1
//     cmd1.name = "echo";
//     cmd1.args = (char*[]){ " hello", NULL}; // Note: args should be an array of strings
//     cmd1.fin = 0;
//     cmd1.fout = -99;

// 	// cmd1.name = "echo";
//     // cmd1.args = (char*[]){ " hello", NULL}; // Note: args should be an array of strings
//     // cmd1.fin = 0;
//     // cmd1.fout = 0;

//     // // Initialize cmd2
//     // cmd2.name = "echo";
//     // cmd2.args = (char*[]){" world is great", NULL}; // Note: args should be an array of strings
//     // cmd2.fin = 0;
//     // cmd2.fout = -99;

// 	//    // Initialize cmd2
//     cmd2.name = "wc";
//     cmd2.args = (char*[]){"", NULL}; // Note: args should be an array of strings
//     cmd2.fin = -99;
//     cmd2.fout = 0;



// 	// cmd3.name= "wc";
//     // cmd3.args = (char*[]){" ", NULL}; // Note: args should be an array of strings
// 	// cmd3.fin=-99;
// 	// cmd3.fout=0;

// 	cmd3.name= "echo";
//     cmd3.args = (char*[]){" lalala", NULL}; // Note: args should be an array of strings
// 	cmd3.fin=0;
// 	cmd3.fout=0;


// 	// cmd3.name="wc";
// 	// cmd3.args='\0';
// 	// cmd3.fin=-99;
// 	// cmd3.fout=0;

// 	cmd1.next = &cmd2;
//     cmd2.next = &cmd3;
//     cmd3.next = NULL;
	
// 	printf("Command 1: %s %s\n", cmd1.name, cmd1.args[0]);

// 	// if (argc < 5)
// 	// 	return (ft_puterr(ERR_INVALID_PARAMS, 1));


// 	// if (ft_strcmp(argv[1],"no")==0)
// 	// 	in.fin=99;
// 	// else
// 	// 	in.fin = open(argv[1], O_RDONLY);
// 	// if (in.fin < 0)
// 	// {
// 	// 	perror("pipex: input file:" );
// 	// 	return (0);
// 	// }
// 	// if (ft_strcmp(argv[argc-1],"no")==0)
// 	// 	in.fout=99;
// 	// else
// 	// 	in.fout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	// if (in.fout < 0)
// 	// {
// 	// 	perror("pipex : output file: ");
// 	// 	return (1);
// 	// }
// 	in.argv = argv;
// 	in.envp = envp;
// 	in.argc = argc;
// 	status = pipex(in,cmd1);
// 	return (status);
// }

