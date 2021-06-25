#include <iostream>
#include <ctime>
#include <list>
#include <string.h>
#include <sqlite3.h>
#include <cstdint>
#include <stdlib.h>
#include <stdbool.h>
#include "classes/entidades.h"

using namespace std;

int table_index;

string table_names[] = {"Livro", "Emprestimo", "Associado"};

list<Livro *> livros;
list<Emprestimo *> emprestimos;
list<Associado *> associados;

static int sql_callback(void *table, int field_number, char **column_data, char **column_name)
{
    if (table_index != -1)
    {
        switch (table_index)
        {
        case 0:
            livros.push_back(new Livro(column_data));
            break;
        case 1:
            emprestimos.push_back(new Emprestimo(column_data));
            break;
        case 2:
            associados.push_back(new Associado(column_data));
        default:
            break;
        }
    }

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
    void findAll(int table);
    void execute(string statement);
    void execSql(string sql);
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

void Database::findAll(int table)
{
    string sql = "SELECT * FROM " + table_names[table] + " ORDER BY ID";
    table_index = table;

    this->execSql(sql);
}

void Database::execute(string statement)
{
    table_index = -1;

    this->execSql(statement);
}

void Database::execSql(string s)
{
    char sql[210];
    strcpy(sql, s.c_str());

    this->rc = sqlite3_exec(db, sql, sql_callback, 0, &zErrMsg);

    if (this->rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

void clear()
{
#if defined _WIN32
    system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#endif
}

void resolveEmprestimoRelations()
{
    for (auto const &emprestimo : emprestimos)
    {
        emprestimo->resolveRelations(&associados, &livros);
    }
}

bool hasExemplaresDisponiveis(Livro *livro)
{
    int count = 0;

    for (auto const &emprestimo : emprestimos)
    {
        if (emprestimo->livro->id == livro->id && emprestimo->dataDevolucao == "")
        {
            count++;
        }
    }

    return count < livro->numero_exemplares;
}

bool hasLivroSameIsbn(Livro *livro)
{
    for (auto const &l : livros)
    {
        if (l->isbn == livro->isbn)
        {
            return true;
        }
    }

    return false;
}

void cadastrarLivro(Database *database)
{
    int last_id;

    if (livros.size() == 0)
    {
        last_id = 0;
    }
    else
    {
        last_id = livros.back()->id;
    }

    Livro *livro = new Livro();

    livro->id = last_id + 1;

    cin.get();

    cout << ("Código ISBN: ");
    cin >> livro->isbn;

    if (hasLivroSameIsbn(livro))
    {
        cout << "Já existe um livro cadastrado com o mesmo Isbn!\n";
        return;
    }

    cout << ("Insira o seu título: ");
    getline(cin, livro->titulo);

    cout << ("Insira a sua editora: ");
    getline(cin, livro->editora);

    cout << ("Insira o seu local de edição: ");
    getline(cin, livro->localEdicao);

    cout << ("Insira o seu assunto: ");
    getline(cin, livro->assunto);

    cout << ("Insira o seus autores (separados por vírgula): ");
    getline(cin, livro->autores);

    cout << ("Quantidade de exemplares disponíveis: ");
    cin >> livro->numero_exemplares;

    database->execute(livro->save());
    livros.push_back(livro);

    cout << "Livro inserido com sucesso!\n";
}

void cadastrarAssociado(Database *database)
{
    int last_id;

    if (associados.size() == 0)
    {
        last_id = 0;
    }
    else
    {
        last_id = associados.back()->id;
    }

    Associado *associado = new Associado();

    associado->id = last_id + 1;

    cin.get();

    cout << ("Insira o seu nome: ");
    getline(cin, associado->nome);

    cout << ("Insira o seu endereco: ");
    getline(cin, associado->endereco);

    cout << ("Insira o seu telefone: ");
    getline(cin, associado->telefone);

    cout << ("Insira o seu email: ");
    getline(cin, associado->email);

    database->execute(associado->save());
    associados.push_back(associado);

    cout << "Associado inserido com sucesso!\n";
}

class Listagem
{
public:
    static void Associados()
    {
        Associado::header();
        for (auto const &associado : associados)
        {
            associado->print();
        }
        cout << endl;
    }

    static void Livros()
    {
        Livro::header();
        for (auto const &livro : livros)
        {
            livro->print();
        }
        cout << endl;
    }

    static void Emprestimos()
    {
        Emprestimo::header();
        for (auto const &emprestimo : emprestimos)
        {
            emprestimo->print();
        }
        cout << endl;
    }
};

class Finder
{
public:
    static Emprestimo *emprestimo(int id)
    {
        for (auto const &e : emprestimos)
        {
            if (e->id == id)
            {
                return e;
            }
        }

        return NULL;
    }

    static Livro *livro(int id)
    {
        for (auto const &l : livros)
        {
            if (l->id == id)
            {
                return l;
            }
        }

        return NULL;
    }

    static Associado *associado(int id)
    {
        for (auto const &a : associados)
        {
            if (a->id == id)
            {
                return a;
            }
        }

        return NULL;
    }

    static bool livroHasEmprestimos(Database *database, Livro *livro)
    {
        for (auto const &emprestimo : emprestimos)
        {
            if (emprestimo->livro->id == livro->id)
            {
                return true;
            }
        }

        return false;
    }

    static bool associadoHasEmprestimos(Database *database, Associado *associado)
    {
        for (auto const &emprestimo : emprestimos)
        {
            if (emprestimo->associado->id == associado->id)
            {
                return true;
            }
        }

        return false;
    }
};

void cadastrarEmprestimo(Database *database)
{
    int last_id;

    if (emprestimos.size() == 0)
    {
        last_id = 0;
    }
    else
    {
        last_id = emprestimos.back()->id;
    }

    cin.get();

    Emprestimo *emprestimo = new Emprestimo();
    emprestimo->id = last_id + 1;

    cout << ("Digite seu id de associado: ");
    cin >> emprestimo->idAssociado;

    cout << ("Digite o id do livro que deseja realizar empréstimo: ");
    cin >> emprestimo->idLivro;

    emprestimo->resolveRelations(&associados, &livros);

    if (emprestimo->livro == NULL)
    {
        cout << "Livro com id " << emprestimo->idLivro << " não encontrado!";
        return;
    }
    else if (emprestimo->associado == NULL)
    {
        cout << "Associado com id " << emprestimo->idAssociado << " não encontrado!";
        return;
    }

    if (hasExemplaresDisponiveis(emprestimo->livro))
    {
        cout << "Digite a data de retirada(dd-mm-aaaa): ";
        cin >> emprestimo->dataEmprestimo;

        database->execute(emprestimo->save());
        emprestimos.push_back(emprestimo);

        cout << "Empréstimo cadastrado com sucesso!";
    }
    else
    {
        cout << "Sem exemplares disponíveis!";
    }
}

void devolverLivro(Database *database)
{
    int id;

    Listagem::Livros();

    cin.get();

    cout << "Digite o id do emprestimo a ser devolvido (-1 para cancelar): ";
    cin >> id;

    if (id == -1)
        return;

    Emprestimo *emprestimo = Finder::emprestimo(id);

    if (emprestimo != NULL)
    {
        cout << "Digite a data de devolução (dd-mm-aaaa): ";
        cin >> emprestimo->dataDevolucao;

        database->execute(emprestimo->updateDataDevolucao());
        cout << "Devolução concluída com sucesso!\n";
    }
    else
    {
        cout << "Empréstimo não encontrado!\n";
    }
}

void excluirLivro(Database *database)
{
    Listagem::Livros();

    int id;

    cout << "Digite o id do livro que deseja excluir: ";
    cin >> id;

    Livro *livro = Finder::livro(id);

    if (livro != NULL)
    {
        if (!Finder::livroHasEmprestimos(database, livro))
        {
            database->execute(livro->deleteEntity());
        }
        else
        {
            cout << "Livro não pode ser excluido pois há empréstimos relacionados!\n";
        }
    }
    else
    {
        cout << "Livro não encontrado!\n";
    }
}

void excluirAssociado(Database *database)
{
    Listagem::Associados();

    int id;

    cout << "Digite o nome do associado que deseja excluir: ";
    cin >> id;

    Associado *associado = Finder::associado(id);

    if (associado != NULL)
    {
        if (!Finder::associadoHasEmprestimos(database, associado))
        {
            database->execute(associado->deleteEntity());
        }
        else
        {
            cout << "Associado não pode ser excluído pois há empréstimos relacionados!\n";
        }
    }
    else
    {
        cout << "Associado não encontrado!\n";
    }
}

void menuPrincipal()
{
    cout << "\nMenu de Opções:\n\t1. Listagem\n\t2. Cadastros\n\t3. Devolução de empréstimo\n\t4. Exclusão de dados\n\t5. Sair do programa\nEscolha: ";
}

int main()
{
    int op;

    Database *db = new Database();

    db->findAll(0);
    db->findAll(1);
    db->findAll(2);

    resolveEmprestimoRelations();

    while (true)
    {
        menuPrincipal();
        cin >> op;

        clear();

        if (op == 1)
        {
            while (op != 4)
            {
                cout << "\nListagem:\n\t1. Livros\n\t2. Associados\n\t3. Empréstimos\n\t4. Voltar ao menu anterior\nEscolha: ";
                cin >> op;

                clear();

                if (op == 1)
                {
                    Listagem::Livros();
                }
                else if (op == 2)
                {
                    Listagem::Associados();
                }
                else if (op == 3)
                {
                    Listagem::Emprestimos();
                }
            }
        }
        else if (op == 2)
        {
            while (op != 4)
            {
                cout << "\nCadastros:\n\t1. Livros\n\t2. Associados\n\t3. Empréstimos\n\t4. Voltar ao menu anterior\nEscolha: ";
                cin >> op;

                if (op == 1)
                {
                    cadastrarLivro(db);
                }
                else if (op == 2)
                {
                    cadastrarAssociado(db);
                }
                else if (op == 3)
                {
                    cadastrarEmprestimo(db);
                }
            }
        }
        else if (op == 3)
        {
            devolverLivro(db);
        }
        else if (op == 4)
        {
            while (op != 3)
            {
                cout << "Excluir:\n\t1. Livros\n\t2. Associados\n\t3. Voltar ao menu anterior\nEscolha: ";
                cin >> op;

                clear();

                if (op == 1)
                {
                    excluirLivro(db);
                }
                else if (op == 2)
                {
                    excluirAssociado(db);
                }
            }
        }
        else if (op == 5)
        {
            cout << "Saindo do programa. Agradecomos o seu uso.\n";
            return 0;
        }
        else
        {
            cout << "Opção inválida! Tente novamente.\n";
        }
    };

    return 0;
}