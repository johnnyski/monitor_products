#include "comm.h"
#include "ports.h"

void send_to_monitor(const char *to_whom, char *cmd)
{
  int s;
  s = call_socket((char *)to_whom, MONITORPS_PORT);
  write(s, cmd, strlen(cmd));
  close(s);
}

