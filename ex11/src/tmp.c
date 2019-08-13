#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <strings.h>
#include <string.h>
#include "../include/minishell.h"
#include "hash.h"
# define TOKEN_BUFF 64
# define SPACE " \t\r\n\a"
# define PWD_BUFF 500
# define BUFF_SIZE 32

char	*ft_strnew(size_t size)
{
	size_t	i;
	char	*str;

	i = 0;
	if (!(str = (char *)malloc(sizeof(char) * size + 1)))
		return (NULL);
	while (i <= size)
	{
		str[i] = '\0';
		i++;
	}
	return (str);
}

void	ft_strdel(char **as)
{
	if (!(as))
		return ;
	free(*as);
	*as = NULL;
}

char		*saving(char *save, char *buff)
{
	size_t	a;
	size_t	b;
	char	*ptr;

	a = 0;
	b = 0;
	if (save)
		a = strlen(save);
	if (buff)
		b = strlen(buff);
	ptr = (char *)malloc(sizeof(*ptr) * (a + b + 1));
	bzero(ptr, a + b + 1);
	strncpy(ptr, save, a);
	strcat(ptr, buff);
	free(buff);
	free(save);
	return (ptr);
}

int			check(char **save, char **buff, char **line)
{
	char	*tmp;
	char	*s;
	int		next;

	tmp = saving(*save, *buff);
	*save = tmp;
	s = strchr(*save, '\n');
	if (s != NULL)
	{
		next = s - *save;
		memset(*save + next, '\0', 1);
		*line = strdup(*save);
		*save = strdup(*save + next + 1);
		free(tmp);
		return (1);
	}
	return (0);
}

int			get_next_line(int const fd, char **line)
{
	static char *save[32000];
	char		*buff;
	int			nextline;
	int			a;

    a = 0;
	if (!line || BUFF_SIZE < 1 || fd < 0 || !(buff = ft_strnew(BUFF_SIZE))
		|| read(fd, buff, 0) < 0)
		return (-1);
	while ((a = read(fd, buff, BUFF_SIZE)) > 0)
	{
		nextline = check(&save[fd], &buff, line);
		if (nextline == 1)
			return (1);
		buff = ft_strnew(BUFF_SIZE);
	}
	if ((nextline = check(&save[fd], &buff, line)))
		return (1);
	if (strlen(save[fd]) > 0)
	{
		*line = strdup(save[fd]);
		ft_strdel(&save[fd]);
		return (1);
	}
	return (0);
}

int shell_cd(char **args)
{
    if (args[1] == NULL)
        fprintf(stderr, "cd: no arguments!\n");
    else
    {
        if (chdir(args[1]) != 0)
            perror("cd");
    }
    return (1);
}

int shell_exit(char **args)
{
  return (0);
}

int shell_echo(char **args)
{
    int i;

    i = 1;
    while (args[i])
    {
        printf("%s ", args[i]);
        i++;
    } 
    printf("\n");
    return (1);
}

int shell_pwd(char **args)
{
    char buff[PWD_BUFF];

    getcwd(buff, FILENAME_MAX);
    printf("%s\n", buff);
    return (1);
}


int         shell_export(char **args, hashtable_t *ht)
{
    int     i;
    int     assign;
    char    *key;
    char    *value;

    i = -1;
    assign = 0;
    if (args[2])
    {
        dprintf(0, "Error too many arguments\n");
        return (1);
    }
    while (args[1][i++])
    {
        if (args[1][i] == ' ')
            dprintf(0, "Bad assignment\n");
        if (args[1][i] == '=')
            assign++;
    }
    key = strtok(args[1], "=");
    value = strtok(NULL, " ");
    
    //hash_export(key, value);
    return (1);
}

int         shell_launch(char **args)
{
    pid_t   pid;
    int     status;
    
    pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
            perror("shell");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
        perror("shell");
    else
    {
        do
        {
            waitpid(pid, &status, WUNTRACED);
        }
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return (1);
}

int         shell_execute(char **args, hashtable_t *ht)
{
    if (args[0] == NULL)
        return 1;
    else if (strcmp(args[0], "echo") == 0)
        return(shell_echo(args));
    else if (strcmp(args[0], "cd") == 0)
        return(shell_cd(args));
    else if (strcmp(args[0], "pwd") == 0)
        return(shell_pwd(args));
    else if (strcmp(args[0], "exit") == 0)
        return(shell_exit(args));
    else if (strcmp(args[0], "export") == 0)
        return(shell_export(args, ht));
    else
    { 
        dprintf(0, "fake command!\n");
        return(shell_launch(args));
    }
}

char        **shell_split_line(char *line)
{
  int       bufsize;
  int       position;
  char      **tokens;
  char      *token;
  char      **tokens_backup;

  bufsize = TOKEN_BUFF;
  position = 0;
  tokens = malloc(bufsize * sizeof(char *));
  if (!tokens)
  {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, SPACE);
  while (token != NULL)
  {
    tokens[position] = token;
    position++;

    if (position >= bufsize)
    {
        bufsize += TOKEN_BUFF;
        tokens_backup = tokens;
        tokens = realloc(tokens, bufsize * sizeof(char*));
        if (!tokens)
        {
		    free(tokens_backup);
            fprintf(stderr, "shell: allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
    token = strtok(NULL, SPACE);
  }
  tokens[position] = NULL;
  return tokens;
}

void shell_loop(void)
{
    char *line;
    char **args;
    int status;
    hashtable_t *ht;

    status = 1;
    ht = hash_create(128);
    while (status)
    {
        dprintf(0, "$> ");
        get_next_line(0, &line);
        args = shell_split_line(line);
        status = shell_execute(args, ht);

        free(line);
        free(args);
    }

}
int main()
{
    shell_loop();

    return (0);
}

