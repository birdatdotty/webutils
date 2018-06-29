// Компилировать будем как-то так: gcc pgtest4.c -I/usr/include/postgresql -lpq
#include <libpq-fe.h>   //< Си библиотека для работы PostgreSQL
#include <sys/socket.h> //< setsockopt() и некоторые константы
#include <sys/select.h> //< select()
#include <time.h>   //< gettimeoftheday()
// #include <sys/time.h>   //< gettimeoftheday()
#include <unistd.h>     //< usleep() тоже может пригодиться

#define SOCK_POLL_TIMEOUT   100 // таймаут ожидания освобождения сокета (на сколько можно максимально блокировать основной поток?) в мс

typedef enum {
    DISCONNECTED = 0,
    CONN_POLLING,
    CONN_READING,
    CONN_WRITING,
    READY,
    QUERY_SENT,
    QUERY_FLUSHING,
    QUERY_BUSY,
    QUERY_READING,
    CLOSING,
    ERROR
} pq_state;

typedef enum {
    NO_ERROR = 0,
    ALLOCATION_FAIL,
    POLLING_FAIL,
    READING_FAIL,
    WRITING_FAIL,
    TIMEOUT_FAIL
} pq_error;

struct pqconn_s{
    pq_state state;         //< текущее действие
    PGconn*  conn;          //< указатель на структуру с данными о соединении
    unsigned long start;    //< время начала текущей операции (для таймаута)
    long timeout;           //< таймаут текущей операции
    pq_error error;         //< если случится что-то не то, сюда прилетит код ошибки
};

/**
 * @brief  получить текущеем время
 * @return время в мс
 */
unsigned long time_ms(void)
{
    struct timespec tp;
    // gettimeoftheday() тут использовать нельзя, оно может плавать
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return (tp.tv_sec * 1000 + tp.tv_nsec / 1000000);
}

/**
 * @brief проверить готовность (свободность) сокета к записи/чтению
 * @param socket_fd - дескриптор интересующего сокета
 * @param rw - 0 если проверяем на чтение, 1 если на запись
 * @return как и select(): -1 = ошибка, 0 - свободен (готов), 1 - занят
 */
int try_socket(int socket_fd, int rw)
{
    fd_set fset;
    struct timeval sock_timeout;

    sock_timeout.tv_sec = 0;
    sock_timeout.tv_usec = SOCK_POLL_TIMEOUT;

    FD_ZERO(&fset);
    FD_SET(socket_fd, &fset);

    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&sock_timeout, sizeof(struct timeval));
    //здесь кстати возможно не помешает еще выставить SO_SNDTIMEO. экспериментируйте.

    return select(socket_fd + 1, ((!rw) ? &fset : NULL),  ((rw) ? &fset : NULL), NULL, &sock_timeout);

}

/**
 * @brief начать процесс подключения к серверу БД
 * @param conninfo - строка подключения к БД
 * @param s - указатель на структуру pqconn_s с данными о подключении и текущем состоянии
 * @param timeout - таймаут операции в мс
 * @return 0 - ошибка (можно узнать ее код в s->error), 1 - успех
 */
int pgsql_connection_start(const char* conninfo, struct pqconn_s* s, long timeout)
{
    if (!s) return 0;

    if (!conninfo)
    {
        s->error = ALLOCATION_FAIL;
        return 0;
    }

    s->conn = PQconnectStart(conninfo);
    s->state = CONN_POLLING;
    s->start = time_ms();
    s->timeout = timeout;
    s->error = NO_ERROR;

    ConnStatusType status;

    status = PQstatus(s->conn);
    if (status == CONNECTION_BAD)
    {
        s->state = ERROR;
        s->error = POLLING_FAIL;
        return 0;
    }

    return 1;
}

/**
 * @brief начать отправку запроса на сервер БД и получение ответа
 * @param command - SQL-запрос
 * @param s - указатель на структуру pqconn_s с данными о подключении и текущем состоянии
 * @param timeout - таймаут операции в мс
 * @return 0 - ошибка, 1 - успех
 */
int pgsql_send_query(struct pqconn_s* s, const char *command, long timeout)
{
    if (s->state != READY)
    {
        return 0;
    }

    if (!PQsendQuery(s->conn, command))
    {
        return 0;
    }

    PQsetnonblocking(s->conn, 0);

    s->state = QUERY_FLUSHING;
    s->start = time_ms();
    s->timeout = timeout;
    s->error = NO_ERROR;

    return 1;
}

/**
 * @brief основной цикл, метод должен вызываться периодично
  * @param s - указатель на структуру pqconn_s с данными о подключении и текущем состоянии
  */
void pgsql_event_loop(struct pqconn_s* s)
{
    if ((s->state == DISCONNECTED) || (s->state == READY))
        return;

    if ((time_ms() - s->start) > s->timeout)
    {
        s->state = CLOSING;
        s->error = TIMEOUT_FAIL;
    }

    if (s->state == CONN_POLLING)
    {
        PostgresPollingStatusType poll_result;
        poll_result = PQconnectPoll(s->conn);

        if  (poll_result == PGRES_POLLING_WRITING)
            s->state = CONN_WRITING;

        if  (poll_result == PGRES_POLLING_READING)
            s->state = CONN_READING;

        if  (poll_result == PGRES_POLLING_FAILED)
        {
            s->state = ERROR;
            s->error = POLLING_FAIL;
        }

        if  (poll_result == PGRES_POLLING_OK)
            s->state = READY;
    }

    if (s->state == CONN_READING)
    {
        int sock_state = try_socket(PQsocket(s->conn), 0);
        if (sock_state == -1)
        {
            s->error = READING_FAIL;
            s->state = CLOSING;
        }
        if (sock_state > 0)
            s->state = CONN_POLLING;
    }

    if (s->state == CONN_WRITING)
    {
        int sock_state = try_socket(PQsocket(s->conn), 1);
        if (sock_state == -1)
        {
            s->error = WRITING_FAIL;
            s->state = CLOSING;
        }
        if (sock_state > 0)
            s->state = CONN_POLLING;
    }

    if (s->state == CLOSING)
    {
        PQfinish(s->conn);
        s->state = ERROR;
    }

    if (s->state == QUERY_FLUSHING)
    {
        int flush_res = PQflush(s->conn);
        if (0 == flush_res)
            s->state = QUERY_READING;
        if (-1 == flush_res)
        {
            s->error = WRITING_FAIL;
            s->state = CLOSING;
        }

    }

    if (s->state == QUERY_READING)
    {
        int sock_state = try_socket(PQsocket(s->conn), 0);
        if (sock_state == -1)
        {
            s->error = READING_FAIL;
            s->state = CLOSING;
        }
        if (sock_state > 0)
            s->state = QUERY_BUSY;
    }

    if (s->state == QUERY_BUSY)
    {
        if (!PQconsumeInput(s->conn))
        {
            s->error = READING_FAIL;
            s->state = CLOSING;
        }
        if (PQisBusy(s->conn))
            s->state = QUERY_READING;
        else
            s->state = READY;
    }
}
