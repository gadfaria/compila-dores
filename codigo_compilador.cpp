#include <boost/regex.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include <sstream>

using namespace std;
using namespace boost;

struct triple
{
    string first, second;
    int third;
    triple() {}
    triple(string a, string b, int c)
    {
        first = a;
        second = b;
        third = c;
    }
};

vector<pair<string, regex>> expressoes;
map<int, map<string, int>> Automato;
bool vterminal[1000];
map<string, bool> variaveis[1000];
int tokenAtual;
vector<triple> SequenciaTokens;
int termina[] = {6, 8, 19, 48, 49};

int line;

string intToString(int Number)
{
    string Result;
    ostringstream convert;
    convert << Number;
    return convert.str();
}

void resetContTokens()
{
    tokenAtual = 0;
}

bool nextLineToken(triple &aux)
{
    if (tokenAtual == SequenciaTokens.size())
        return false;
    aux = SequenciaTokens[tokenAtual++];
    return true;
}

void carregaExpressoes()
{
    expressoes.push_back(make_pair("INICIO_PROGRAMA", regex("(programa)(.*)")));

    expressoes.push_back(make_pair("T_INTEIRO", regex("(inteiro)(.*)")));
    expressoes.push_back(make_pair("T_CARACTER", regex("(caracter)(.*)")));
    expressoes.push_back(make_pair("T_REAL", regex("(real)(.*)")));

    expressoes.push_back(make_pair("INICIO", regex("(inicio)(.*)")));
    expressoes.push_back(make_pair("FIM", regex("(fim)(.*)")));
    expressoes.push_back(make_pair("COMECA", regex("(comeca)(.*)")));
    expressoes.push_back(make_pair("TERMINA", regex("(termina)(.*)")));

    expressoes.push_back(make_pair("ABRE_PARENTESES", regex("(\\()(.*)")));
    expressoes.push_back(make_pair("FECHA_PARENTESES", regex("(\\))(.*)")));

    expressoes.push_back(make_pair("ENTRADA", regex("(leia)(.*)")));
    expressoes.push_back(make_pair("SAIDA", regex("(escreva)(.*)")));

    expressoes.push_back(make_pair("PARA", regex("(para)(.*)")));
    expressoes.push_back(make_pair("ENQUANTO", regex("(enquanto)(.*)")));
    expressoes.push_back(make_pair("SENAO", regex("(senao)(.*)")));
    expressoes.push_back(make_pair("SE", regex("(se)(.*)")));

    expressoes.push_back(make_pair("RELACIONAL", regex("(!=)(.*)")));
    expressoes.push_back(make_pair("RELACIONAL", regex("(==)(.*)")));
    expressoes.push_back(make_pair("RELACIONAL", regex("(>=)(.*)")));
    expressoes.push_back(make_pair("RELACIONAL", regex("(<=)(.*)")));
    expressoes.push_back(make_pair("RELACIONAL", regex("(>)(.*)")));
    expressoes.push_back(make_pair("RELACIONAL", regex("(<)(.*)")));

    expressoes.push_back(make_pair("ATRIBUICAO", regex("(=)(.*)")));

    expressoes.push_back(make_pair("LOGICO", regex("(e)(.*)")));
    expressoes.push_back(make_pair("LOGICO", regex("(ou)(.*)")));
    expressoes.push_back(make_pair("LOGICO", regex("(!)(.*)")));

    expressoes.push_back(make_pair("ARITMETICO", regex("(\\+)(.*)")));
    expressoes.push_back(make_pair("ARITMETICO", regex("(\\-)(.*)")));
    expressoes.push_back(make_pair("ARITMETICO", regex("(\\*)(.*)")));
    expressoes.push_back(make_pair("ARITMETICO", regex("(\\/)(.*)")));
    expressoes.push_back(make_pair("ARITMETICO", regex("(mod)(.*)")));

    expressoes.push_back(make_pair("VIRGULA", regex("(\\,)(.*)")));
    expressoes.push_back(make_pair("PONTO_VIRGULA", regex("(\\;)(.*)")));

    expressoes.push_back(make_pair("INTEIRO", regex("(((\\d)+(\\.)(\\d)+)|((\\d)+))(.*)")));
    expressoes.push_back(make_pair("REAL", regex("(((\\d)+(\\.)(\\d)+)|((\\d)+))(.*)")));
    expressoes.push_back(make_pair("CARACTER", regex("('(.*)')(.*)")));
    expressoes.push_back(make_pair("VAR", regex("((([A-Z]+[0-9]*)|([a-z]+[0-9]*))+)(.*)")));
    expressoes.push_back(make_pair("CADEIA", regex("(\\\"([A-Z]|[a-z]|[0-9]|\\s|\\+|\\-|\\*|\\=|\\&|\\$|\\\\%|\\.|\\,|\\\\)*\\\")(.*)")));
}

void carregaAutomato()
{
    memset(vterminal, 0, sizeof vterminal);

    int tam = sizeof(termina) / sizeof(termina[0]);

    for (int i = 0; i < tam; i++)
        vterminal[termina[i]] = true;

    char str[1000];
    FILE *arq = fopen("automato.txt", "r");
    while (fgets(str, 1000, arq) != NULL)
    {
        if (strlen(str) < 4)
            continue;
        string n[3];
        int aux = 0;
        for (int i = 0; i < strlen(str); i++)
        {
            if (str[i] == ' ')
            {
                aux++;
                continue;
            }
            n[aux].push_back(str[i]);
        }
        int v = atoi(n[0].c_str()), u = atoi(n[2].c_str());
        Automato[v][n[1]] = u;
    }
    fclose(arq);
}

bool analiseLexica(string str)
{
    bool flag = true;
    if (str == "")
        return true;
    if (str[str.size() - 1] == '\n')
        str = str.substr(0, str.size() - 1);
    if (str == "")
        return true;

    while (str != "")
    {
        while (str != "" && (str[0] == ' ' || str[0] == '\t'))
            str = str.substr(1, str.size() - 1);
        if (str == "")
            break;

        flag = false;
        for (int i = 0; i < expressoes.size(); i++)
        {
            smatch m;
            if (regex_match(str, m, expressoes[i].second))
            {
                SequenciaTokens.push_back(triple(expressoes[i].first, m[1], line));
                str = m[m.size() - 1];
                flag = true;
                break;
            }
        }
        if (!flag)
            break;
    }
    return flag;
}

vector<string> AnaliseSintatica()
{
    int qtdMain = 0;
    int P = 0, B = 0;
    vector<string> err;
    int atual = 1;
    triple aresta;
    bool vemvariavel = false;

    while (nextLineToken(aresta))
    {
        // cout << Automato[atual].count(aresta.first) << " - " << aresta.first << endl;
        if (Automato[atual].count(aresta.first) <= 0)
        {
            err.push_back("'" + aresta.second + "'" + " erro - linha" + intToString(aresta.third) + "\n");
            vemvariavel = false;
            continue;
        }

        atual = Automato[atual][aresta.first];

        if (aresta.first == "INICIO")
            B++;

        if (aresta.first == "FIM")
        {
            variaveis[B].clear();
            B--;
        }

        if (aresta.first == "COMECA")
            B++;
        if (aresta.first == "TERMINA")
            B--;

        if (aresta.first == "ABRE_PARENTESES")
            P++;
        if (aresta.first == "FECHA_PARENTESES")
            P--;

        if (B < 0)
        {
            err.push_back("'termina/fim' não esperado - linha " + intToString(aresta.third) + "\n");
            atual = 1;
            vemvariavel = false;
            continue;
        }

        if (P < 0)
        {
            err.push_back("')' não esperado - linha " + intToString(aresta.third) + "\n");
            atual = 1;
            vemvariavel = false;
            continue;
        }

        if (aresta.second == "programa")
            qtdMain++;

        if (aresta.first == "T_INTEIRO" || aresta.first == "T_REAL" || aresta.first == "T_CARACTER" || aresta.first == "VIRGULA")
        {
            vemvariavel = true;
        }

        if (aresta.first == "VAR")
        {
            if (vemvariavel)
            {
                bool ok = false;
                for (int i = 0; i <= B && !ok; i++)
                    ok = variaveis[i][aresta.second];
                if (ok)
                {
                    err.push_back("'" + aresta.second + "'" + " variavel já foi declarada - linha " + intToString(aresta.third) + "\n");
                    vemvariavel = false;
                }
                vemvariavel = false;
                variaveis[B][aresta.second] = true;
            }
            else
            {

                bool ok = false;
                for (int i = 0; i <= B && !ok; i++)
                    ok = variaveis[i][aresta.second];

                if (!ok)
                {
                    err.push_back("Variavel '" + aresta.second + "' não foi declarada - linha " + intToString(aresta.third) + "\n");
                    vemvariavel = false;
                }
            }
        }

        if (vterminal[atual])
        {
            if (P != 0)
                err.push_back("Quantidade de parenteses está errado - linha " + intToString(aresta.third) + "\n");
            atual = 1;
            P = 0;
        }
    }

    if (qtdMain == 0)
        err.push_back("Main não encontrada\n");
    else if (qtdMain > 1)
        err.push_back("O código tem mais de uma main\n");

    if (P || B)
    {
        err.push_back("Erro\n");
    }

    return err;
}

void traducao(string x)
{
    FILE *arq = fopen("traducao.txt", "r");
    map<string, string> trad;
    char str[1000];

    while (fgets(str, 1000, arq) != NULL)
    {
        string aux;
        string aux2;
        bool flag = false;
        for (int i = 0; i < strlen(str); i++)
        {
            if (str[i] == ' ' || str[i] == '\n')
            {
                flag = true;
                continue;
            }
            if (!flag)
                aux.push_back(str[i]);
            else
                aux2.push_back(str[i]);
        }

        trad[aux] = aux2;
    }

    fclose(arq);
    resetContTokens();

    arq = fopen(x.c_str(), "w");

    bool in = false, out = false;

    triple tk;
    fputs("#include<iostream>\n#include<cstdio>\nusing namespace std;\n\n", arq);

    while (nextLineToken(tk))
    {
        if (trad.count(tk.second) > 0)
        {
            if (tk.second == "programa")
            {
                fputs(string("int ").c_str(), arq);
            }
            if (tk.second == "escreva" && out == false)
            {
                out = true;
                fputs(string("cout").c_str(), arq);
            }
            if (tk.second == "leia" && in == false)
            {
                in = true;
                fputs(string("cin").c_str(), arq);
            }
            fputs((trad[tk.second] + " ").c_str(), arq);
        }
        else
        {
            if (tk.first == "T_INTEIRO" || tk.first == "T_REAL" || tk.first == "T_CARACTER")
                continue;
            fputs((tk.second + " ").c_str(), arq);
        }
        if (tk.second == "comeca" || tk.second == ";" || tk.second == "termina" || tk.second == "inicio" || tk.second == "fim")
            fputs(string("\n").c_str(), arq);

        if (tk.second == ";")
        {
            in = false;
            out = false;
        }
    }
    fclose(arq);
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        cout << "####ERRO####" << endl;
        cout << "Apenas um parametro é necessário" << endl;
        return -1;
    }
    int len = strlen(argv[1]);

    if (len < 4 || tolower(argv[1][len - 1]) != 'd' || tolower(argv[1][len - 2]) != 'c' || argv[1][len - 3] != '.')
    {
        cout << "####ERRO####" << endl;
        cout << "Extensão do arquivo precisa ser .cd" << endl;
        return -1;
    }

    FILE *arq;
    arq = fopen(argv[1], "r");
    if (arq == NULL)
    {
        cout << "####ERRO####" << endl;
        cout << "Arquivo não existe" << endl;
        return -1;
    }

    carregaExpressoes();

    char str[1000];
    line = 1;
    bool error = false;
    while (fgets(str, 1000, arq) != NULL)
    {
        if (!analiseLexica(string(str)))
        {
            error = true;
            cout << "Erro na linha " << line << endl;
        }
        line++;
    }
    fclose(arq);

    if (error)
        return -1;

    arq = fopen((string(argv[1], argv[1] + len - 2) + "o").c_str(), "w");
    for (int i = 0; i < SequenciaTokens.size(); i++)
    {
        fputs((SequenciaTokens[i].first + " " + SequenciaTokens[i].second + "\n").c_str(), arq);
    }
    fclose(arq);

    carregaAutomato();
    resetContTokens();
    vector<string> Erros = AnaliseSintatica();

    if (Erros.size() != 0)
    {
        for (int i = 0; i < Erros.size(); i++)
        {
            cout << Erros[i];
        }
        return -1;
    }

    string nomeCode = string(argv[1], argv[1] + len - 2) + "cpp";
    traducao(nomeCode);
    int pos = nomeCode.find(".");
    system(string("g++ " + nomeCode + " -o " + nomeCode.substr(0, pos)).c_str());

    return 0;
}
