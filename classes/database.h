#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <sqlite3.h>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

class Database
{
private:
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

public:
    Database();
    void execSql(char *sql);
};

Database::Database()
{
    this->rc = sqlite3_open("data/dados.db", &this->db);

    if (this->rc == SQLITE_OK)
    {
        cout << "Conexão com banco de dados concluída!";
    }
    else
    {
        cout << sqlite3_errmsg(this->db);
    }
}

void Database::execSql(char *sql)
{
    this->rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if (this->rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

#endif