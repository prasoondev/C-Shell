Assumptions:

Max buffer size 4096 including file contents.

Maximum number of files in a directory 1024.

No file name starts with '-' however this constaint can be revoked by remove a single line of code.

Proclore takes one argument only which can be made for multiple.

How to run:

Open terminal in the directory type make and run.

Then do ./mp1 ( preferably sudo ./mp1 to read system files and access root directories. )

# Specifications

 ## hop 
 `hop` command changes the directory that the shell is currently in. It should also print the full path of working directory after changing. The directory path/name can be provided as argument to this command. Flags such as “.”, “..”, “~”, and “-” are supported in hop. It is similar to `cd` command in UNIX.
 ```
hop <flags> <path/name>
```

 ## reveal
 `reveal` command lists all the files and directories in the specified directories in lexicographic order (default reveal does not show hidden files). You should support the -a and -l flags.

    -l : displays extra information

    -a : displays all files, including hidden files

Similar to hop, “.”, “..”, “~”, and “-” symbols are supported.
```

reveal <flags> <path/name>

```

## Log commands
`log` -Similar to the actual history command in bash shows the most recent 15 commands executed across multiple sessions.

`log purge` -Clears all the log currently stored. Do not store this command in the log.

`log execute <index>` -Execute the command at position in log (ordered from most recent to oldest).

## System commands 
Shell is able to execute the other system commands present in Bash as well like emacs, gedit etc. This is possible in both foreground and background processes.

### Foreground Process

Executing a command in foreground means the shell will wait for that process to complete and regain control afterwards. Control of terminal is handed over to this process for the time being while it is running.

Time taken by the foreground process and the name of the process should be printed in the next prompt if process takes > 2 seconds to run. Round the time down to integer before printing in prompt.
```

<JohnDoe@SYS:~> sleep 5

# sleeps for 5 seconds

<JohnDoe@SYS:~ sleep : 5s>

```

### Background Process

Any command invoked with `&` is treated as a background command. This implies that shell will spawn that process but doesn’t hand the control of terminal to it. Shell will keep taking other user commands. Whenever a new background process is started, print the PID of the newly created background process on your shell also.
```
<JohnDoe@SYS:~> sleep 10 &

13027

<JohnDoe@SYS:~> sleep 20 & # After 10 seconds

Sleep exited normally (13027)

13054

<JohnDoe@SYS:~> echo "Lorem Ipsum" # After 20 seconds

Sleep exited normally (13054)

Lorem Ipsum

```

## Proclore

`proclore` is used to obtain information regarding a process. If an argument is missing, print the information of your shell.

`proclore <pid>`

Information required to print :

    pid

    Process Status (R/R+/S/S+/Z)

    Process group

    Virtual Memory

    Executable path of process

## seek
`seek` command looks for a file/directory in the specified target directory (or current if no directory is specified). It returns a list of relative paths (from target directory) of all matching files/directories (files in green and directories in blue) separated with a newline character.

Flags :

    -d : Only look for directories (ignore files even if name matches)

    -f : Only look for files (ignore directories even if name matches)

    -e : This flag is effective only when a single file or a single directory with the name is found. If only one file (and no directories) is found, then print it’s output. If only one directory (and no files) is found, then change current working directory to it. Otherwise, the flag has no effect. This flag should work with -d and -f flags.

```

<JohnDoe@SYS:~> seek newfolder

./newfolder

./doe/newfolder

./doe/newfolder/newfolder.txt

<JohnDoe@SYS:~> seek newfolder ./doe

./newfolder # This is relative to ./doe

./newfolder/newfolder.txt

<JohnDoe@SYS:~> seek -d newfolder ./doe

./newfolder

<JohnDoe@SYS:~> seek -f newfolder ./doe

./newfolder/newfolder.txt

<JohnDoe@SYS:~> seek newfolder ./john

No match found!

<JohnDoe@SYS:~> seek -d -f newfolder ./doe

Invalid flags!

<JohnDoe@SYS:~> seek -e -f newfolder ./doe

./newfolder/newfolder.txt

This is a new folder! # Content of newfolder.txt

<JohnDoe@SYS:~> seek -e -d newfolder ./doe

./newfolder/

<JohnDoe@SYS:~/doe/newfolder>


```

## .myshrc

The myshrc file is a customized shell configuration file, similar to .bashrc, tailored to enhance your command-line experience. It includes useful aliases and functions to streamline your workflow and improve efficiency. In this you need to create your own .bashrc file which will use aliases and functions.

##  I/O Redirection

Similar to how UNIX handles I/O redirection 

    > : Outputs to the filename following “>”.

    >> : Similar to “>” but appends instead of overwriting if the file already exists.

    < : Reads input from the filename following “<”.

## Pipes

Pipes are used to pass information between commands. It takes the output from command on left and passes it as standard input to the command on right. Your shell should support any number of pipes.

```
<JohnDoe@SYS:~> echo "Lorem Ipsum" | wc

1 2 12 # extra spaces can be present
```

## activities
Prints a list of all the processes currently running that were spawned by your shell in lexicographic order. This list should contain the following information about all processes :

    Command Name

    pid

    state : running or stopped
```
<JohnDoe@SYS:~> activities

221 : emacs new.txt - Running

430 : vim - Stopped

620 : gedit - Stopped
```
## Signals
```
ping <pid> <signal_number>
```
ping command is used to send signals to processes. Take the pid of a process and send a signal to it which corresponds to the signal number (which is provided as an argument). Print error “No such process found”, if process with given pid does not exist. Check man page for signal for an exhaustive list of all signals present.
```
<JohnDoe@SYS:~> activities

221 : emacs new.txt - Running

430 : vim - Running

620 : gedit - Stopped

<JohnDoe@SYS:~> ping 221 9 					# 9 is for SIGKILL

Sent signal 9 to process with pid 221

<JohnDoe@SYS:~> activities

430 : vim - Running

620 : gedit - Stopped

<JohnDoe@SYS:~> ping 430 47

Sent signal 15 to process with pid 430 		# 15 is for SIGTERM

<JohnDoe@SYS:~> activities

430 : vim - Stopped

620 : gedit - Stopped
```

Following 3 commands are direct keyboard input where Ctrl is Control key on keyboard (or it’s equivalent).

Ctrl - C

Interrupt any currently running foreground process by sending it the SIGINT signal. It has no effect if no foreground process is currently running.

Ctrl - D

Log out of your shell (after killing all processes) while having no effect on the actual terminal.

Ctrl - Z

Push the (if any) running foreground process to the background and change it’s state from “Running” to “Stopped”. It has no effect on the shell if no foreground process is running.

## fg and bg 
`fg <pid>`
Brings the running or stopped background process with corresponding pid to foreground, handing it the control of terminal. Print “No such process found”, if no process with given pid exists.
```
<JohnDoe@SYS:~> activities

620 : gedit - Stopped

<JohnDoe@SYS:~> fg 620

# brings gedit [620] to foreground and change it's state to Running
```
`bg <pid>`
Changes the state of a stopped background process to running (in the background). If a process with given pid does not exist, print “No such process found” to the terminal.
```
<JohnDoe@SYS:~> activities

620 : gedit - Stopped

<JohnDoe@SYS:~> bg 620

# Changes [620] gedit's state to Running (in the background)
```

## Neonate
`neonate -n [time_arg]`
The command prints the Process-ID of the most recently created process on the system , this pid will be printed every [time_arg] seconds until the key `x` is pressed.
```
<JohnDoe@SYS:~> neonate -n 4

# A line containing the pid should be printed

# every 4 seconds until the user

# presses the key: 'x'.

11810

11811

11811

11812

11813 # key 'x' is pressed at this moment terminating the printing
```

## iMan
iMan fetches man pages from the internet using sockets and outputs it to the terminal (stdout). In this case,we are using the website : http://man.he.net/ to get the man pages.
`iMan <command_name>`
```
"iMan sleep" is the same as "iMan sleep extra"

```
