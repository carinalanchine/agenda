#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Contato
{
    string nome;
    string telefone;
    string relacao;

    public:
    Contato(string n, string t, string r)
        : nome{n}, telefone{t}, relacao{r} {}

    virtual ~Contato() {}

    virtual void alteraObj(string n, string t, string r) {
        nome = n;
        telefone = t;
        relacao = r;
    }

    virtual void salvarObj(ostream& a) {
        a << nome << "|" << telefone << "|" << relacao << "|";
    }

    virtual void exibirObj() {
        cout << "\nNome: " << nome << "\nTelefone: " << telefone 
        << "\nRelacao: " << relacao;
    }

    string retornaNome() {
        return nome;
    }
};

class Pessoal: public Contato
{
    string endereco;

    void definirEnd() {
        cout << "Endereco: ";
        getline(cin >> ws,endereco);
    }

    public:
    Pessoal(string n, string t, string e, string r)
        : Contato{n,t,r}, endereco{e} {}

    Pessoal(string n, string t, string r)
        : Contato{n,t,r} { definirEnd(); }

    ~Pessoal() {}

    void exibirObj() {
        Contato::exibirObj();
        cout << "\nEndereco: " << endereco << "\n";
    }

    void alteraObj(string n, string t, string r) {
        Contato::alteraObj(n,t,r);
        definirEnd();
    }

    void salvarObj(ostream& a) {
        Contato::salvarObj(a);
        a << "1|" << endereco << "\n"; 
    }
};

class Profissional: public Contato
{
    string email;

    void definirEmail() {
        cout << "E-mail: ";
        getline(cin >> ws,email);
    }

    public:
    Profissional(string n, string t, string e, string r)
        : Contato{n,t,r}, email{e} {}

    Profissional(string n, string t, string r)
        : Contato{n,t,r} { definirEmail(); }

    ~Profissional() {}

    void exibirObj() {
        Contato::exibirObj();
        cout << "\nE-mail: " << email << "\n";
    }

    void alteraObj(string n, string t, string r) {
        Contato::alteraObj(n,t,r);
        definirEmail();
    }

    void salvarObj(ostream& a) {
        Contato::salvarObj(a);
        a << "2|" << email << "\n";
    }
};

class Agenda
{
    struct Lista {
        Contato* info;
        Lista* prox;
    };

    Lista* contatos;
    Lista* aux;

    Lista* buscar(string n) 
    {
        aux = contatos;
        while (aux != nullptr) {
            if ((aux->info->retornaNome()).find(n) != string::npos)
                return aux;

            aux = aux->prox;
        }

        return nullptr;
    }

    void adicionarDireto(string tipo, string n, string t, string r, string e)
    {
        Lista* novo = new Lista;

        if (tipo == "1")
            novo->info = new Pessoal{n,t,e,r};

        else 
            novo->info = new Profissional{n,t,e,r};

        novo->prox = contatos;
        contatos = novo;
    }

    void alterar(Lista* obj, string n, string t, string r) {
        obj->info->alteraObj(n,t,r);
    }

    public:
    Agenda() 
        : contatos{nullptr} {}

    ~Agenda()
    {
        Lista* temp;
        aux = contatos;
        while (aux != nullptr) {
            delete aux->info;
            temp = aux;
            aux = aux->prox;
            delete temp;
        }
    }

    void adicionar(string n, string t, string r, string tipo)
    {
        aux = buscar(n);
        if (aux == nullptr) {
            Lista* novo = new Lista;

            if (tipo == "1")
                novo->info = new Pessoal{n,t,r};

            else
                novo->info = new Profissional{n,t,r};

            novo->prox = contatos;
            contatos = novo;
        }

        else
            alterar(aux,n,t,r);
    }

    void exibe(string n)
    {
        aux = buscar(n);
        if (aux != nullptr)
            aux->info->exibirObj();

        else
            cout << "\nContato nao existe\n";
    }

    bool existe(string n) {
        return (buscar(n) != nullptr);
    }

    void remover(string n)
    {
        if (existe(n)) {
            aux = contatos;
            Lista* ant{nullptr};
            while (aux != nullptr)
            {
                if ((aux->info->retornaNome()).find(n) != string::npos) {
                    delete aux->info;

                    if (ant == nullptr) 
                        contatos = aux->prox;

                    else 
                        ant->prox = aux->prox;

                    delete aux;
                    break;
                }
                ant = aux;
                aux = aux->prox;
            }
        }

        else
            cout << "\nContato nao existe\n";
    }

    void alterar(string n, string novo, string t, string r)
    {
        aux = buscar(n);
        aux->info->alteraObj(novo,t,r);
    }

    void listar()
    {
        if (contatos == nullptr)
            cout << "\nAgenda vazia\n";

        else {
            aux = contatos;
            while (aux != nullptr) {
                aux->info->exibirObj();
                aux = aux->prox;
            }
        }
    }

    void salvar(string nomeArq)
    {
        ofstream arquivo(nomeArq,ofstream::out);
        aux = contatos;

        while (aux != nullptr) {
            aux->info->salvarObj(arquivo);
            aux = aux->prox;
        }

        arquivo.close();
    }

    void recuperar(string nomeArq)
    {
        string n, t, r, e, tipo;
        ifstream arquivo(nomeArq,ifstream::in);

        if (!arquivo.good())
            printf("\nArquivo nao existe\n");

        else {
            while (true) {
                getline(arquivo,n,'|');
                getline(arquivo,t,'|');
                getline(arquivo,r,'|');
                getline(arquivo,tipo,'|');
                getline(arquivo,e,'\n');

                if (arquivo.good())
                    adicionarDireto(tipo,n,t,r,e);

                else 
                    break;
            }
        }
        arquivo.close();
    }
};

/*------------------------------------------------------------------------------------------------*/

int main ()
{
    int menu;
    string nomeArq, nome, tel, rel, aux;
    Agenda agenda;

    cout << "1) Recuperar agenda\n2) Nova agenda\n";
    cin >> menu;
    cout << "\nNome do arquivo: "; 
    getline(cin >> ws, nomeArq);
    nomeArq += ".txt";

    if (menu == 1) 
        agenda.recuperar(nomeArq);

    do {
        cout << "\n1) Buscar\n2) Inserir\n3) Alterar\n4) Remover\n5) Listar\n6) Salvar\n7) Sair\n";
        cin >> menu;

        switch (menu)
        {
            case 1: {
                cout << "\nNome: "; 
                getline(cin >> ws,nome);

                agenda.exibe(nome);

            } break;

            case 2: 
            {
                cout << "\n1) Contato pessoal\n2) Contato profissional\n";
                getline(cin >> ws,aux);

                cout << "\nNome: "; 
                getline(cin >> ws,nome);
                cout << "Telefone: "; 
                getline(cin >> ws,tel);
                cout << "Relacao: "; 
                getline(cin >> ws,rel);

                agenda.adicionar(nome,tel,rel,aux);

            } break;

            case 3: 
            {
                cout << "\nNome: "; 
                getline(cin >> ws,nome);

                if (!agenda.existe(nome))
                    cout << "\nContato nao existe\n";

                else {    
                    cout << "\nNovo nome: "; 
                    getline(cin >> ws,aux); 
                    cout << "Telefone: "; 
                    getline(cin >> ws,tel);
                    cout << "Relacao: "; 
                    getline(cin >> ws,rel);

                    agenda.alterar(nome,aux,tel,rel);
                }

            } break;

            case 4: {
                cout << "\nNome: "; 
                getline(cin >> ws,nome);

                agenda.remover(nome);

            } break;

            case 5:
                agenda.listar();
                break;

            case 6: {
                agenda.salvar(nomeArq);
                cout << "\nAgenda salva\n";
                
            } break;

            case 7:
                break;

            default:
                cout << "\nEntrada invalida\n";
        }

    } while (menu != 7);

    return 0;
}