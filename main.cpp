#include <iostream>
#include <ctime>
#include <list>
#include "classes/database.h"


using namespace std;

class Associado
{
public:
    int id;
    string nome;
    string endereco;
    string telefone;
    string email;
    Associado();
};

Associado::Associado()
{
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
    list<string> autores;
    Livro();
};

Livro::Livro()
{
}

class Exemplar
{
public:
    int id;
    int codigo;
    Livro *livro;
    Exemplar(int id, int codigo, Livro *livro);
};

Exemplar::Exemplar(int id, int codigo, Livro *livro)
{
    this->id = id;
    this->codigo = codigo;
    this->livro = livro;
}

class Emprestimo
{
public:
    int id;
    Exemplar *livro;
    string dataEmprestimo;
    string dataDevolucao;
};

int main()
{
    int op;
    list<Livro *> livros;

    Database *db = new Database();

    db->execSql((char*) "select * from livro");

    while (true)
    {
        cout << "Opcao 1 - Cadastro Livro\nOpcao 2 - Cadastro Associado\nOpcao 9 - Encerrar o Programa\n";
        cin >> op;

        if (op == 1)
        {

            Livro *livro = new Livro();
            cout << "Insira o titulo do livro: ";
            getline(cin, livro->titulo);

            cout << "Insira a editora do livro: ";
            getline(cin, livro->editora);

            cout << "Inisira o local da Edicao: ";
            getline(cin, livro->localEdicao);
        }
        else if (op == 2)
        {

            Associado *associado = new Associado();

            cout << ("Insira o seu nome: ");
            getline(cin, associado->nome);

            cout << ("Insira o seu endereco: ");
            getline(cin, associado->endereco);

            cout << ("Insira o seu telefone: ");
            getline(cin, associado->telefone);

            cout << ("Insira o seu email: ");
            getline(cin, associado->email);
        }
        else if (op == 9)
        {

            cout << "Saindo do programa. Agradecomos o seu uso.";
            return 0;
        }
    };

    return 0;
}