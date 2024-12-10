#include <stdio.h>
#include <signal.h>

/* Prototypes. */
void send_to_monitor(const char *to_whom, char *cmd);

#include "forms.h"
#include "products.h"

/* Globals */
extern FD_products *fd_products;
extern const char *computer_str;
extern int my_port;

void do_exit_button(FL_OBJECT *ob, long n)
{
  kill(-getpid(), SIGINT);
  exit(0);
}
void do_select_computer_button(FL_OBJECT *ob, long n)
{
  char cmd[100];

  /* Send 'del product_gui' to *computer_str */
  sprintf(cmd,"del product_gui %s.%d\n", computer_str, my_port);
  send_to_monitor(computer_str, cmd);
  computer_str = fl_show_input("Enter another computer", computer_str);
  fl_set_object_label(fd_products->computer_text, computer_str);

  /* Now, add this product_gui to the new computer. */
  sprintf(cmd,"add product_gui %s.%d\n", computer_str, my_port);
  send_to_monitor(computer_str, cmd);
}

void do_sigio_button(FL_OBJECT *ob, long n)
{
  kill(getpid(), SIGIO);
}
