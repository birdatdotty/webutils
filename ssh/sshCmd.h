#ifndef _SSH_CMD_H_
#define _SSH_CMD_H_

// int cmd(const char* addr, const char* login, const char* password, const char* cmd);
int sshCmd( const char* host,
            int port,
            const char* login,
            const char* password,
            const char* cmd);

#endif
