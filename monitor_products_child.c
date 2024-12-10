#include "comm.h"
#include "ports.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

sigset_t lsigset, old_lsigset;
sigset_t zeromask;
int ears_socket;
int verbose = 1;
int need_to_send_sigio = 0;

FILE *input_pipe (FILE *fp, char *cmd);
FILE *output_pipe (FILE *fp, char *cmd);

/***********************************************************************/
/*                                                                     */
/*                    do_something                                     */
/*                                                                     */
/***********************************************************************/
void do_something(int s, char *from_who, int ppid)
{
  char buf[1000];
  int i, j, n;
  char *ch, *filename, *gif;
  FILE *fp = NULL;

  /*
   * 's' is the socket to read.  The protocol is:
   *
   *       <prod tape job#>:<image data>      '<>' are not part of protocol
   *  -or  <DATE prod mm/dd/yy hh:mm>
   *
   * In other words, look for ':' to separate the image filename from
   * the actual image data.  Fill in spaces with '_' to create a unix filename.
   */
  
  for (i=0; (n = read_data(s, buf, sizeof(buf))) > 0; i++) {
	fprintf(stderr, "read_data returned %d bytes.\n", n);
	if (strncmp(buf, "DATE", 4) == 0) {
	  buf[n] = '\0'; /* Just in case. */
	  fprintf(stderr, "MONITORPS says %s\n", buf);
	  printf("%s\n", buf);
	  fflush(stdout);
	  kill(ppid, SIGIO);
	  return;
	}
	if (i==0) {
	  /* Look for the <filename> specification. */
	  for (j=0; j<n; j++) {
		ch = &buf[j];
		if (*ch == ':') break;
	  }
	  if (j == n) break; /* Can't find ':' */
	  *ch = '\0';
	  gif = ch+1;
	  filename = (char *)strdup(buf);
	  for (j=0; j<strlen(filename); j++)
		if (filename[j] == ' ') filename[j] = '_';

	  fprintf(stderr, "MONITORPS says FILENAME <%s>\n", filename);
	  fprintf(stderr, "bytes for image is %d\n", n-strlen(filename)-1);

	  fp = fopen(filename, "w");
	  fp = output_pipe(fp, "gzip -d -f --stdout");
	  fwrite(gif, sizeof(char), n-strlen(filename)-1, fp);
	} else {
	  gif = buf;
	  fwrite(gif, sizeof(char), n, fp);
	}
  }
  if (fp) {
	pclose(fp);
	/* Image is now an uncompressed xpm. 
     * Is there a faster display method?
	 */
	printf("%s\n", filename); /* Fill parent's pipe and send SIGIO */
	fflush(stdout);
	kill(ppid, SIGIO);
	free(filename);
  }
  return;
}


/***********************************************************************/
/*                                                                     */
/*                    read_socket                                      */
/*                                                                     */
/***********************************************************************/
void read_socket(int s, char *from_who)
{
  int t;
  struct sockaddr_in from;
  int from_len;
  int ppid;

/* 't' is a complete 5-tuple address specification.
 * 's' is only a 3-tuple address specification; ie. it is unknown
 *     who is connecting to us.
 *
 * We can comunicate on 't' and listen on 's' simultaneously.
 */
  while((t= get_connection(s, &from, &from_len)) < 0) { /* get a connection */
	if (errno == EINTR) {
	  continue;   /* EINTR might happen on accept(), */
	}
	if (errno == EWOULDBLOCK) {
	  if (verbose) perror("read_socket");
	  return;
	}
	if (verbose) perror("accept: read_socket");               /* bad */
	return;
  }
  /*  fprintf(stderr, "ready to do_something. from_who <%s>\n", from_who);*/
  ppid = getppid();
  do_something(t, from_who, ppid);
  close(t);
  return;
}

/***********************************************************************/
/*                                                                     */
/*                    timer                                            */
/*                                                                     */
/***********************************************************************/
void timer(int n)
{
  int j;

  /* block signals */
  sigprocmask(SIG_SETMASK, &lsigset, &old_lsigset);

  signal(SIGALRM, timer);

  if (need_to_send_sigio) {
	kill(getpid(), SIGIO);
	need_to_send_sigio = 0;
  }
  sigprocmask(SIG_SETMASK, &old_lsigset, NULL);
  return;

}

void child_death(int n)
{
  int pid;

  sigprocmask(SIG_SETMASK, &lsigset, &old_lsigset);
  fprintf(stderr, "\nCHILD DIED. CHILD DIED.\n");
  while ((pid =waitpid(-1, NULL, WNOHANG)) > 0) {
	/* Ok, let another child be born. */
	continue;
  }
  signal(SIGCHLD, (void(*)(int))child_death);
  sigprocmask(SIG_SETMASK, &old_lsigset, NULL);
}

/***********************************************************************/
/*                                                                     */
/*                      io_handler                                     */
/*                                                                     */
/***********************************************************************/
void io_handler(int n)
{
  fd_set read_template;
  struct timeval wait;
  int rc;

  /* This section is critical. */

  sigprocmask(SIG_SETMASK, &lsigset, &old_lsigset);
  /*  fprintf(stderr, "in io_handler\n");*/
  wait.tv_sec = 1;
  wait.tv_usec = 0; /* Don't wait, there *is* something pending. */
  FD_ZERO(&read_template);
  FD_SET(ears_socket, &read_template);
#ifdef __hpux
#define SELTYP int *
#else
#define SELTYP fd_set *
#endif
  rc = select(FD_SETSIZE, (SELTYP) &read_template, (SELTYP) 0, (SELTYP) 0, &wait);
  
  signal(SIGIO, (void(*)(int))io_handler);
  signal(SIGCHLD, (void(*)(int))child_death);
  /* Who caused this interrupt? */
  if (rc <= 0) {
	sigprocmask(SIG_SETMASK, &old_lsigset, NULL);
	return;
  }
  
  if (FD_ISSET(ears_socket, &read_template)) {
	/* Howdy monitorps. */
	read_socket(ears_socket, "monitorps");
  }

  need_to_send_sigio = 1;
  sigprocmask(SIG_SETMASK, &old_lsigset, NULL);
  /*  fprintf(stderr, "leaving io_handler\n");*/
  return; /* Parent or error. */
}



int main(int argc, char **argv)
{
  char myname[50];
  char cmd[100];
  int my_port;

  gethostname(myname, sizeof(myname));

  /* Begin listening for connections. */
  ears_socket = establish_async(0);
  my_port = portno(ears_socket);

  printf("PORT %d\n", my_port);
  fflush(stdout);
  kill(getppid(), SIGIO);

  /* Set signals to block */
  sigemptyset(&lsigset);
  sigemptyset(&zeromask);
  sigaddset(&lsigset, SIGIO); 
  /*  sigaddset(&lsigset, SIGCHLD);  */

  /* Setup the SIGIO interupt routines. */
  signal(SIGIO, (void(*)(int))io_handler);
  signal(SIGCHLD, (void(*)(int))child_death);

  /* Tell monitorps we've got our ears on. */
  /* Telling on the NEIGHBOR port keeps it local to that machine,
   * meaning no other computer is told of our presence.
   * We only want to talk to one computer's monitorps.
   */

  sprintf(cmd, "add product_gui %s.%d\n", myname, my_port);
  send_to_monitor(myname, cmd);

  /* Process SIGIO interrupts. */
  timer(0);
  while(1) {
	if (need_to_send_sigio) alarm(1);
	sigsuspend(&zeromask);
  }

}
