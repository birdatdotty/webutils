#include <iostream>
#include "sshCmd.h"

int main () {
  sshCmd ("192.168.0.200", 22, "gluck", "qwewerzzt", "touch qq");

  return 0;
}
