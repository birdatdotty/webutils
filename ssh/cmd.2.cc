#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
int cmd(const char* host, const char* login, const char* password, const char* cmd)
{
  int state, hlen;
  unsigned char *hash = NULL;
  char *hexa;
  char buf[10];
  state = ssh_is_server_known(session);
  hlen = ssh_get_pubkey_hash(session, &hash);
  if (hlen < 0)
    return -1;
  switch (state)
  {
    case SSH_SERVER_KNOWN_OK:
      std::cout << "\t\tSSH_SERVER_KNOWN_OK" << '\n';
      break; /* ok */
    case SSH_SERVER_KNOWN_CHANGED:
      fprintf(stderr, "Host key for server changed: it is now:\n");
      ssh_print_hexa("Public key hash", hash, hlen);
      fprintf(stderr, "For security reasons, connection will be stopped\n");
      free(hash);
      return -1;
    case SSH_SERVER_FOUND_OTHER:
      fprintf(stderr, "The host key for this server was not found but an other"
        "type of key exists.\n");
      fprintf(stderr, "An attacker might change the default server key to"
        "confuse your client into thinking the key does not exist\n");
      free(hash);
      return -1;
    case SSH_SERVER_FILE_NOT_FOUND:
      fprintf(stderr, "Could not find known host file.\n");
      fprintf(stderr, "If you accept the host key here, the file will be"
       "automatically created.\n");
      /* fallback to SSH_SERVER_NOT_KNOWN behavior */
    case SSH_SERVER_NOT_KNOWN:
      hexa = ssh_get_hexa(hash, hlen);
      fprintf(stderr,"The server is unknown. Do you trust the host key?\n");
      fprintf(stderr, "Public key hash: %s\n", hexa);
      free(hexa);
      if (fgets(buf, sizeof(buf), stdin) == NULL)
      {
        free(hash);
        return -1;
      }
      if (strncasecmp(buf, "yes", 3) != 0)
      {
        free(hash);
        return -1;
      }
      if (ssh_write_knownhost(session) < 0)
      {
        fprintf(stderr, "Error %s\n", strerror(errno));
        free(hash);
        return -1;
      }
      break;
    case SSH_SERVER_ERROR:
      fprintf(stderr, "Error %s", ssh_get_error(session));
      free(hash);
      return -1;
  }
  free(hash);
  return 0;
//     ssh_session my_ssh_session;
//     int rc;
//     int port = 22;
//     int verbosity = SSH_LOG_PROTOCOL;
//     // char *password;
//     // Open session and set options
//     my_ssh_session = ssh_new();
//     if (my_ssh_session == NULL)
//         exit(-1);
//     // ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, "192.168.0.254");
//     ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, addr);
//     ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, "gluck");
//     ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
//     ssh_options_set(my_ssh_session, SSH_OPTIONS_CIPHERS_C_S,"aes128-ctr");
//
//     //ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
//     // Connect to server
//     rc = ssh_connect(my_ssh_session);
//     if (rc != SSH_OK)
//     {
//         fprintf(stderr, "Error: %s\n", ssh_get_error(my_ssh_session)); //HERE IS WHERE I GET THE ERROR
//         ssh_free(my_ssh_session);
//         exit(-1);
//     }
//
//     std::cout << "/* login:" << login << '\n';
//     std::cout << "/* password:" << password << '\n';
//     std::cout << "/* cmd:" << cmd << '\n';
//     // Authenticate ourselves
//     // password = "qwewerzzt";
//     rc = ssh_userauth_password(my_ssh_session, NULL, password);
//     if (rc != SSH_AUTH_SUCCESS)
//     {
//         fprintf(stderr, "Error authenticating with password: %s\n",
//             ssh_get_error(my_ssh_session));
//         ssh_disconnect(my_ssh_session);
//         ssh_free(my_ssh_session);
//         exit(-1);
//     }
//     else {
//
//       std::cout << "/* message */ Connect success!!!!!!!!!!!!!!!!!!!!!!!" << '\n';
// // /system scheduler add name=REBOOT interval=5s on-event="/system scheduler remove REBOOT;/system reboot"
//       ssh_channel channel;
//       channel = ssh_channel_new(my_ssh_session);
//       rc = ssh_channel_request_exec(channel, cmd );
//       // rc = ssh_channel_request_exec(channel, "/system reboot");
//       if (rc != SSH_OK)
//       {
//         ssh_channel_close(channel);
//         ssh_channel_free(channel);
//         // return rc;
//       }
//
//     }
//
//
//         ssh_disconnect(my_ssh_session);
//     ssh_free(my_ssh_session);
}
