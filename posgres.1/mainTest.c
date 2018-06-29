#include "pre.c"

int main(void)
{
    struct pqconn_s s;

    pgsql_connection_start("dbname=db1 user=user1 password=password1 hostaddr=10.0.0.1 port=5432", &s, 15000);

    while ((s.state != ERROR) && (s.state != READY))
    {
        pgsql_event_loop(&s);
    }

    if (s.state == ERROR)
    {
        perror("DB connection failed \n");
        return 1;
    }

    pgsql_send_query(&s, "SELECT * FROM history;", 50000);
    while ((s.state != ERROR) && (s.state != READY))
    {
        pgsql_event_loop(&s);
    }

    if (s.state == ERROR)
    {
        perror("DB query failed \n");
        return 1;
    }

    PGresult        *res;
    int             rec_count;
    int             row;
    int             col;

    res = PQgetResult(s.conn);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
               perror("We did not get any data!\n");
               return 1;
    }

    rec_count = PQntuples(res);

    printf("Received %d records.\n", rec_count);

    for (row=0; row<rec_count; row++)
    {
        for (col=0; col<3; col++)
        {
            printf("%s\t", PQgetvalue(res, row, col));
        }
        puts("");
    }

    PQclear(res);
}
