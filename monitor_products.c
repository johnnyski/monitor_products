#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include "forms.h"
#include "products.h"

/*
 * 4/22/97 
 *   1. Speed improvements for displaying images.  This is done
 *      by now accepting gzip-ed *.xpm files.  The database (the one
 *      monitorps controls) stores gzip-ed *.xpm files for all the 
 *      products.
 *
 * TODO:
 *   1. Make the gunzip a pipe inside the code.  Currently, it is 
 *      a system call.
 *
 *   2. Provide image size parameters.  This will slow display because
 *      the *.xpm must be resized via ppm functions.
 *
 *   3. Request images from monitorps.  Requires mods in monitorps protocol.
 */


FILE *fpipe;
sigset_t lsigset, old_lsigset;
sigset_t zeromask;
FD_products *fd_products;
const char *computer_str;
int my_port;
int need_to_send_sigio = 0;

void timer(int n)
{
  sigprocmask(SIG_SETMASK, &lsigset, &old_lsigset);
  signal(SIGALRM, (void(*)(int))timer);
  if (need_to_send_sigio) {
	need_to_send_sigio = 0;
	alarm(1000000);
	fprintf(stderr,"kill(getpid(), SIGIO);\n");
	kill(getpid(), SIGIO);
  }
  sigprocmask(SIG_SETMASK, &old_lsigset, NULL);
}  

void io_handler(int n)
{
  char buf[100];
  char cmd[100];
  /* This section is critical. */

  sigprocmask(SIG_SETMASK, &lsigset, &old_lsigset);
  signal(SIGIO, (void(*)(int))io_handler);
  /* Read data on pipe until empty; it would block. */
  if (fgets(buf, sizeof(buf), fpipe)) {
	buf[strlen(buf)-1] = '\0';
	/* Test, take a look. */
	/*
	sprintf(cmd, "xv %s", buf);
	system(cmd);
	*/

	/*------------------------------*/
	/* Do with 'buf' as you please. */
	/*------------------------------*/
	printf("Child sent <%s>\n", buf);
	if (strncmp(buf, "LAST_1C-51", 10) == 0)
	  fl_set_pixmap_file(fd_products->image_1c51, buf);
	else if (strncmp(buf, "LAST_1B-51", 10) == 0)
	  fl_set_pixmap_file(fd_products->image_1b51, buf);
	else if (strncmp(buf, "LAST_2A-54", 10) == 0)
	  fl_set_pixmap_file(fd_products->image_2a54, buf);
	else if (strncmp(buf, "LAST_2A-53", 10) == 0)
	  fl_set_pixmap_file(fd_products->image_2a53, buf);
	else if (strncmp(buf, "PORT", 4) == 0) {
	  sscanf(&buf[4], "%d", &my_port);
	  fl_set_object_label(fd_products->port_text, &buf[4]);
	} else if (strncmp(buf, "DATE", 4) == 0) {
	  char *pstr, *dstr;
	  pstr = &buf[5];
	  pstr = strtok(pstr, " ");
	  dstr = strtok(NULL, "");
	  if (strncmp(pstr, "1B-51", 5) == 0)
		fl_set_object_label(fd_products->date1b51_text, dstr);
	  else if (strncmp(pstr, "1C-51", 5) == 0)
		fl_set_object_label(fd_products->date1c51_text, dstr);
	  else if (strncmp(pstr, "2A-54", 5) == 0)
		fl_set_object_label(fd_products->date2a54_text, dstr);
	  else if (strncmp(pstr, "2A-53", 5) == 0)
		fl_set_object_label(fd_products->date2a53_text, dstr);
	  else
		;

	} else
	  ;
	need_to_send_sigio = 1;
  }

  alarm(3);
  sigprocmask(SIG_SETMASK, &old_lsigset, NULL);
  return; /* Parent or error. */
}

void exit_now(int n)
{
  do_exit_button(NULL, 0); /* The real killer. */
}

int main(int argc, char **argv)
{

  /* BEGIN forms interface. */
  fl_initialize(&argc, argv, 0, 0, 0);
  fd_products = create_form_products();
  
  /* fill-in form initialization code */
  computer_str = "raindrop1";
  fl_set_object_label(fd_products->computer_text, computer_str);
  
  /* show the first form */
  fl_show_form(fd_products->products,FL_PLACE_CENTER,FL_FULLBORDER,"products");


  /* This pipe stays open and we recieve SIGIO messages when 
   * we are to read from it.  The contents will be filenames
   * of images to display.  Parse the name to determine which
   * image it is.
   */

  fpipe = popen("monitor_products_child", "r");
  if (fcntl(fileno(fpipe), F_SETFL, O_NONBLOCK) < 0) {
	perror("fcntl(s,F_SETFL, FNDELAY)");
	return -1;
	}

  /* Setup signals to block during critical sections. */
  sigemptyset(&lsigset);
  sigemptyset(&zeromask);
  sigaddset(&lsigset, SIGIO); 
  sigaddset(&lsigset, SIGALRM); 

  /* Setup the SIGIO interupt routines. */
  signal(SIGIO, (void(*)(int))io_handler);
  signal(SIGALRM, (void(*)(int))timer);
  signal(SIGINT, (void(*)(int))exit_now);

  while(1)
	fl_do_forms();

  return 0;
}


