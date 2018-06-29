#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
int main()
{
    ssh_session my_ssh_session;
    int rc;
    int port = 22;
    int verbosity = SSH_LOG_PROTOCOL;
    char *password;
    // Open session and set options
    my_ssh_session = ssh_new();
    if (my_ssh_session == NULL)
        exit(-1);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, "192.168.0.254");
    ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, "gluck");
    ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_CIPHERS_C_S,"aes128-ctr");

    //ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
    // Connect to server
    rc = ssh_connect(my_ssh_session);
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Error: %s\n", ssh_get_error(my_ssh_session)); //HERE IS WHERE I GET THE ERROR
        ssh_free(my_ssh_session);
        exit(-1);
    }
    // Verify the server's identity
    // For the source code of verify_knowhost(), check previous example
/*  if (verify_knownhost(my_ssh_session) < 0)
    {
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        exit(-1);
    }
*/
    // Authenticate ourselves
    password = "qwewerzzt";
    rc = ssh_userauth_password(my_ssh_session, NULL, password);
    if (rc != SSH_AUTH_SUCCESS)
    {
        fprintf(stderr, "Error authenticating with password: %s\n",
            ssh_get_error(my_ssh_session));
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        exit(-1);
    }
    else {

      std::cout << "/* message */ Connect success!!!!!!!!!!!!!!!!!!!!!!!" << '\n';
// /system scheduler add name=REBOOT interval=5s on-event="/system scheduler remove REBOOT;/system reboot"
      ssh_channel channel;
      channel = ssh_channel_new(my_ssh_session);
      rc = ssh_channel_request_exec(channel, "/system scheduler add name=REBOOT interval=5s on-event=\"/system scheduler remove REBOOT;/system reboot\"" );
      // rc = ssh_channel_request_exec(channel, "/system reboot");
      if (rc != SSH_OK)
      {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return rc;
      }

    }


        ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
}
