#include <bits/stdc++.h>
using namespace std;

struct no
{

    string caracter;
    int frequencia;
    struct no *esq, *dir;

    no(string c, int f)
    {
        this->caracter = c;
        this->frequencia = f;
        this->dir = nullptr;
        this->esq = nullptr;
    }

} typedef no;

struct comp
{
    bool operator()(const no *a, const no *b) const
    {
        return a->frequencia > b->frequencia;
    }
};

void treeTraversal(no *raiz, unordered_map<string, vector<bool>> &mapaCodigos, vector<bool> &codigo)
{

    if (raiz->dir == nullptr && raiz->esq == nullptr && raiz->caracter != "")
    {
        mapaCodigos[raiz->caracter] = codigo;
        return;
    }

    if (raiz->esq != nullptr)
    {
        codigo.push_back(0);
        treeTraversal(raiz->esq, mapaCodigos, codigo);
        codigo.pop_back();
    }
    if (raiz->dir != nullptr)
    {

        codigo.push_back(1);
        treeTraversal(raiz->dir, mapaCodigos, codigo);
        codigo.pop_back();
    }
}

int main()
{

    // Abrindo arquivo a ser compactado
    ifstream arquivo("testfile.txt", ios::binary);
    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo.\n";
        return 1;
    }

    unordered_map<string, int> frequencias;
    string texto((istreambuf_iterator<char>(arquivo)),
                 istreambuf_iterator<char>());

    // Computando frequências de caracteres
    for (size_t i = 0; i < texto.size();)
    {
        unsigned char c = texto[i];
        size_t char_len;

        if ((c & 0x80) == 0)
            char_len = 1;
        else if ((c & 0xE0) == 0xC0)
            char_len = 2;
        else if ((c & 0xF0) == 0xE0)
            char_len = 3;
        else if ((c & 0xF8) == 0xF0)
            char_len = 4;
        else
        {
            cerr << "Caractere inválido!\n";
            break;
        }

        string caractere = texto.substr(i, char_len);
        ++frequencias[caractere];
        i += char_len;
    }

    cout << "Caracteres e suas frequências: \n";
    for (const auto &[caractere, freq] : frequencias)
    {
        cout << "'" << caractere << "': " << freq << "\n";
    }

    // Montando árvore de códigos
    priority_queue<no *, vector<no *>, comp> pq;
    for (const auto &[caractere, freq] : frequencias)
    {
        pq.push(new no(caractere, freq));
    }

    while (pq.size() >= 2)
    {

        no *esq = pq.top();
        pq.pop();
        no *dir = pq.top();
        pq.pop();

        no *novo = new no("", esq->frequencia + dir->frequencia);
        novo->esq = esq;
        novo->dir = dir;
        pq.push(novo);
    }

    // Vinculando caracteres a seus códigos
    no *raiz = pq.top();
    unordered_map<string, vector<bool>> mapaCodigos;
    vector<bool> codigo(0);
    treeTraversal(raiz, mapaCodigos, codigo);

    cout << "\nCodigos:\n";
    for (const auto &[caractere, codigo] : mapaCodigos)
    {
        cout << "'" << caractere << "': ";
        for (const auto &c : codigo)
        {
            cout << c;
        }
        cout << endl;
    }

    return 0;
}
