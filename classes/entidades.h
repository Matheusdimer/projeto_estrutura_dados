#ifndef ENTIDADES_H
#define ENTIDADES_H

#include <iostream>
#include <list>
#include <string>
#include <iomanip>

using namespace std;

template <typename T>
void printElement(T t, const int &width)
{
    cout << left << setw(width) << setfill(' ') << t;
}

class Date
{
public:
    int year, month, day;

    Date(string date)
    {
        this->parseDate(date);
    }

    void parseDate(string date)
    {
        this->day = stoi(date.substr(0, 2));
        this->month = stoi(date.substr(3, 2));
        this->year = stoi(date.substr(6, 4));
    }

    string toString()
    {
        return "";
    }
};

class Associado
{
public:
    int id;
    string nome;
    string endereco;
    string telefone;
    string email;
    int numero_emprestimos = 0;
    Associado();
    Associado(char **data);
    static void header(bool newLine = true);
    void print(bool newLine = true);
    string save();
    string deleteEntity();
    string updateNumeroEmprestimos();
};

Associado::Associado()
{
}

Associado::Associado(char **data)
{
    this->id = atoi(data[0]);
    this->nome = data[1];
    this->endereco = data[2];
    this->telefone = data[3];
    this->email = data[4];
    this->numero_emprestimos = atoi(data[5]);
}

void Associado::header(bool newLine)
{
    cout << "\033[1;31m";
    printElement("Id", 10);
    printElement("Nome", 40);
    printElement("Endereco", 30);
    printElement("Telefone", 25);
    printElement("Email", 30);
    printElement("Emp. Atuais", 2);
    cout << "\033[1;0m";
    if (newLine) cout << endl;
}

void Associado::print(bool newLine)
{
    printElement(this->id, 10);
    printElement(this->nome, 40);
    printElement(this->endereco, 30);
    printElement(this->telefone, 25);
    printElement(this->email, 30);
    printElement(this->numero_emprestimos, 2);
    if (newLine) cout << endl;
}

string Associado::save()
{
    string insert = "INSERT INTO Associado VALUES(";
    insert += "'" + to_string(this->id) + "',";
    insert += "'" + this->nome + "',";
    insert += "'" + this->endereco + "',";
    insert += "'" + this->telefone + "',";
    insert += "'" + this->email + "',";
    insert += "'" + to_string(this->numero_emprestimos) + "'";
    insert += ")";
    return insert;
}

string Associado::deleteEntity()
{
    return "DELETE FROM Associado WHERE id = '" + to_string(this->id) + "'";
}

string Associado::updateNumeroEmprestimos()
{
    return "UPDATE Associado SET numero_emprestimos = '" + to_string(this->numero_emprestimos) + "' WHERE id = '" + to_string(this->id) + "'";
}

class Livro
{
public:
    int id;
    int isbn;
    string titulo;
    string editora;
    string localEdicao;
    string assunto;
    string autores;
    int numero_exemplares;
    Livro();
    Livro(char **data);
    static void header(bool newLine = true);
    void print(bool newLine = true);
    string save();
    string deleteEntity();
};

Livro::Livro()
{
}

Livro::Livro(char **data)
{
    this->id = atoi(data[0]);
    this->isbn = atoi(data[1]);
    this->titulo = data[2];
    this->editora = data[3];
    this->localEdicao = data[4];
    this->assunto = data[5];
    this->numero_exemplares = atoi(data[6]);
    this->autores = data[7];
}

void Livro::header(bool newLine)
{
    cout << "\033[1;31m";
    printElement("Id", 5);
    printElement("Isbn", 8);
    printElement("Titulo", 30);
    printElement("Editora", 20);
    printElement("Local", 25);
    printElement("Assunto", 25);
    printElement("Autores", 30);
    printElement("N. Exemplares", 10);
    cout << "\033[1;0m";
    if (newLine) cout << endl;
}

void Livro::print(bool newLine)
{
    printElement(this->id, 5);
    printElement(this->isbn, 8);
    printElement(this->titulo, 30);
    printElement(this->editora, 20);
    printElement(this->localEdicao, 25);
    printElement(this->assunto, 25);
    printElement(this->autores, 30);
    printElement(this->numero_exemplares, 10);
    if (newLine) cout << endl;
}

string Livro::save()
{
    string insert = "INSERT INTO Livro VALUES(";
    insert += "'" + to_string(this->id) + "',";
    insert += "'" + to_string(this->isbn) + "',";
    insert += "'" + this->titulo + "',";
    insert += "'" + this->editora + "',";
    insert += "'" + this->localEdicao + "',";
    insert += "'" + this->assunto + "',";
    insert += "'" + to_string(this->numero_exemplares) + "',";
    insert += "'" + this->autores + "'";
    insert += ")";
    return insert;
}

string Livro::deleteEntity()
{
    return "DELETE FROM Livro WHERE id = '" + to_string(this->id) + "'";
}

class Emprestimo
{
public:
    int id;
    int idLivro;
    Livro *livro = NULL;
    string dataEmprestimo;
    string dataDevolucao;
    int idAssociado;
    Associado *associado = NULL;
    Emprestimo();
    Emprestimo(char **data);
    static void header();
    void print();
    string save();
    string updateDataDevolucao();
    void resolveRelations(list<Associado *> *associados, list<Livro *> *livros);
};

Emprestimo::Emprestimo()
{
}

Emprestimo::Emprestimo(char **data)
{
    this->id = atoi(data[0]);
    this->idLivro = atoi(data[1]);
    this->dataEmprestimo = data[2];
    this->dataDevolucao = data[3];
    this->idAssociado = atoi(data[4]);
}

void Emprestimo::header()
{
    cout << "\033[1;31m";
    printElement("Id", 10);
    printElement("Exemplar", 35);
    printElement("Associado", 40);
    printElement("Data de emprestimo", 25);
    printElement("Data de devolucão", 25);
    cout << "\033[1;0m" << endl;
}

void Emprestimo::print()
{
    printElement(this->id, 10);
    printElement(this->livro->titulo, 35);
    printElement(this->associado->nome, 40);
    printElement(this->dataEmprestimo, 25);
    printElement(this->dataDevolucao == "" ? "Não devolvido" : this->dataDevolucao, 25);
    cout << endl;
}

string Emprestimo::save()
{
    string insert = "INSERT INTO Emprestimo VALUES(";
    insert += "'" + to_string(this->id) + "',";
    insert += "'" + to_string(this->livro->id) + "',";
    insert += "'" + this->dataEmprestimo + "',";
    insert += "'" + this->dataDevolucao + "',";
    insert += "'" + to_string(this->associado->id) + "'";
    insert += ")";
    return insert;
}

string Emprestimo::updateDataDevolucao()
{
    return "UPDATE Emprestimo SET data_devolucao = '" + this->dataDevolucao + "' WHERE id = '" + to_string(this->id) + "'";
}

void Emprestimo::resolveRelations(list<Associado *> *associados, list<Livro *> *livros)
{
    for (auto const &associado : *associados)
    {
        if (this->idAssociado == associado->id)
        {
            this->associado = associado;
        }
    }

    for (auto const &livro : *livros)
    {
        if (this->idLivro == livro->id)
        {
            this->livro = livro;
        }
    }
}

#endif