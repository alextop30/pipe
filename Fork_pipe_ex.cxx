#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
/*
 * 
 * Purpose: Create a program in c which will use 
 * the unics system calls to create a pipe which is to operate
 * similar to the bash pipe operator feeding the output of one
 * program into the input of another. Pipe is unidirectional
 * where the pipe will a write on one side and will read off of the
 * other. All ouput produced is outputted on the screen using the 
 * programs being used. User specifies which programs are run through
 * standard input on the console! quit exits the program!
 * 
 * */

using namespace std;

int main(int argc, char* argv[])
{

    char command1 [80] = "command1";   //read in buffer 1st input
    char command2 [80] ="command2";   //read in buffer 2nd input
    
    //array to check against for the quit command
    char quit[]="quit";
  
    
    //ptrs for parsing user input
    char * ptr;
    char * ptr1;
    
    //store the parsed command in char pointer array
    char *args[10];
    char *args2[10];
    
    //initialization of pipe and rs  
    int pipefd[2], rs;
    
   	//while user input on eqch command is not equal to quit
	while ((strcmp(command1,quit)!= 0) && (strcmp(command2,quit) !=0) )
	{
		//counter variables for each command
		int i=1;
		int c=1;
		
		//obtain user input - assign it to string and check if it is
		//quit - if it is exit the program
		cout << "Enter command 1 (incl. args) or quit: ";
		cin.getline(command1,80); 
		if((strcmp(command1,quit)== 0))
		return 0;
		
		//do the same for second input
		cout << "Enter command 2 (incl. args) or quit: ";
		cin.getline(command2,80); 
		if ((strcmp(command2,quit)== 0))
		return 0;
		
		//create the pipe
		rs = pipe(pipefd);
	
		//parse arguments and save in char pointer array array
		ptr = strtok(command1, " ");
		args[0] = ptr;   //save the command in 1st element
			
		//while ptr is not 0 continue where it left off
		while(ptr != NULL)
		{
			ptr = strtok(NULL, " ");
			args[i] = ptr;
			
			i++;
		}
			
		//repeat the same procedure for the second input
		//parsing each command and arguments		
		ptr1 = strtok(command2, " ");
		args2[0] = ptr1;
		
		while(ptr1 != NULL)
		{
			ptr1 = strtok(NULL, " ");
			args2[c] = ptr1;
			
			c++;
		}
		
		//check if pipe has failed
		if (rs < 0) 
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}

		// fork the process into two
		rs = fork();
		
		//check if fork failed
		if(rs < 0) 
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		
		//check for parent or child
		if(rs == 0) 
		{ 
			//Now in the child process
			
			//close unused write end of pipe - child will only read
			//from the parent
			close(pipefd[1]);
			
			// close std in
			close(0);

			// duplicate read end of pipe
			// into std in
			dup(pipefd[0]);
			
			// close read end of pipe
			close(pipefd[0]);
		
			//run user 2nd command
			rs = execvp(args2[0], args2);
			
			//check if exec failed
			if (rs < 0) 
			{
				perror("execvp");
				exit(EXIT_FAILURE);
			}
		}
		else 
		{ 
			// Now into Parent process
			
			
			//declarations to set up new fork
			int rs2;
			
			//fork again into rs2
			rs2 = fork();
			
			//check if for failed
			if (rs2 == -1)
			{
				perror("fork");
				exit(EXIT_FAILURE);
			}
			
			//if fork did not fail check for child process
			if (rs2 == 0)
			{
				// Now in second child process!
							
				// close unused read end of pipe -- this child writes
				// into the pipe which is read by child
				close(pipefd[0]);
				
				// close std out
				close(1);

				//duplicate write end of pipe
				// into std out
				dup(pipefd[1]);
				
				// close the write end
				close(pipefd[1]);
				
				//run user 1st command command	
				rs = execvp(args[0], args);
				
				//check if exec failed
				if(rs < 0) 
				{
					perror("execvp");
					exit(EXIT_FAILURE);
				}
			}
			else 
			{
				//back into the parent process!
				
				//close both ends off the pipe -- they are not used
				//at the parent at all!
				close(pipefd[0]);
				close(pipefd[1]);
					
				//wait for the two children to finish
				wait(&rs2);
				wait(&rs);
			}
			
		}
	
	}
	
	//terminate program without abend.
	return 0;
}
		
		
		
		










	

	
	

