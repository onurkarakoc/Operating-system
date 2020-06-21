# Operating-system

* P1: 
    1) Written a shell script that takes a single command line argument which is a file containing one integer per line
    2) Writtten a shell script that reads integers (one-per-line) from the user until it reads the word “end”. Afterward, it will report the maximum number given by the user.
    3) Written a shell script that takes an optional argument as a directory, and removes all zero length ordinary files in the directory. If directory argument is not given, it should remove all zero-length files in the current directory.
    
* P2: 
    Written a C program that implements a “task queue”.
    The program begins by generating tasks. A task is either an insert, a delete, or a search a value in a sorted list (in ascending order) with no duplicates. The program assigns a task_num starting from 0 and incremented by 1 for each task generation, a task_type which is insert, delete, or search task in the list, and a value for the list operation, randomly. Each time it generates a new task, it adds it to a task queue. After the completion of task generation, your program pulls the tasks from the queue and processes them one by one, by inserting, deleting, or searching from the list.
    The program gets the number of tasks from the user, and display the task generation and completion information. In the final phase, it prints the contents of the list.
    
* P3:
    Written a C program similar to a shell.
    The shell-like program displays the command line prompt “myshell>” and waits for the user's command. It reads the user’s next command, parses it into separate tokens that are used to fill the argument vector for the command to be executed, and executes it.
    The shell-like program supports the following built-in commands (which are not part of the regular shell, the functionality is built directly into your shell itself):
    
    1) cd <directory> : change the current directory to <directory>
    2) dir : print the current working directory
    3) history : print 10 most recently entered commands in your shell
    4) findloc <command> : print the location of the executable file corresponding to the given command
    5) bye : terminate your shell process.
    
    The shell-like program supports pipe operator between two processes.
    
* P4: 
    Written a Pthreads program that implements a “task queue.”
    The main thread begins by starting a user-specified number of threads that immediately go to sleep in a condition wait (the worker threads are idle at first).
    The main thread generates tasks to be carried out by the other threads. A task is either an insert, a delete, or a search a value in a sorted list (in ascending order) with no duplicates. Each time the main thread generates a new task by adding it to a task queue, it awakens a thread with a condition signal. When a thread is awakened, it pulls a task from the queue and processes it. When a thread finishes executing its task, it returns to a condition wait. When the main thread completes generating tasks, it sets a global variable indicating that there will be no more tasks, and awakens all the threads with a condition broadcast.
