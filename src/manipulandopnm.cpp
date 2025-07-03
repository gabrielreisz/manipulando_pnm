// INF110 - Trabalho pratico 2
//
// programa para ler, modificar e gravar uma imagem no formato PNM
//
// Autores: Andre Gustavo dos Santos           (criado em 16/06/14)
//          Andre Gustavo dos Santos           (modificado em 22/05/18)
//          Andre Gustavo dos Santos           (modificado em 13/09/21)
//          Andre Gustavo dos Santos           (modificado em 15/07/24)

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cmath>
#include <cstdlib>

const int MAXALTURA = 1000; // tamanho maximo aceito (pode ser alterado)
const int MAXLARGURA = 1000;

using namespace std;

// Variáveis globais
unsigned char imagem_grayscale[MAXALTURA][MAXLARGURA]; // Para P1 e P2
unsigned char imagem_color[MAXALTURA][MAXLARGURA][3];  // Para P3
int largura, altura;                                   // Dimensoes da imagem
char tipo_imagem[4];                                   // Tipo da imagem (P1, P2 ou P3)

/*
garante que o valor do pixel esteja entre 0-255
*/
unsigned char corrige_erros(int valor)
{
    if (valor < 0)
    {
        return 0;
    }
    if (valor > 255)
    {
        return 255;
    }
    return (unsigned char)valor;
}

// função que lê a imagem do arquivo
// retorna true se a leitura foi bem-sucedida, false caso contrário
bool lerImagem(const string &nomeArquivo)
{
    ifstream arqentrada;
    char comentario[200], c;
    int valor_pixel; // Usado para ler int e depois converter

    arqentrada.open(nomeArquivo.c_str(), ios::in);
    if (!arqentrada)
    {
        cout << "Nao consegui abrir o arquivo " << nomeArquivo << endl;
        return false;
    }

    arqentrada >> tipo_imagem; // Le o tipo de arquivo
    arqentrada.get();          // Le e descarta o \n do final da 1a. linha

    if (strcmp(tipo_imagem, "P2") == 0)
    {
        cout << "Imagem em tons de cinza (P2)\n";
    }
    else if (strcmp(tipo_imagem, "P3") == 0)
    {
        cout << "Imagem colorida (P3)\n";
    }
    else if (strcmp(tipo_imagem, "P1") == 0)
    {
        cout << "Imagem preto e branco (P1)\n";
    }
    else
    {
        cout << "Desculpe, nao trabalho com esse tipo de imagem: " << tipo_imagem << endl;
        arqentrada.close();
        return false;
    }

    while ((c = arqentrada.get()) == '#')    // Enquanto for comentario
        arqentrada.getline(comentario, 200); // Le e descarta a linha "inteira"

    arqentrada.putback(c); // Devolve o caractere lido para a entrada

    arqentrada >> largura >> altura; // Le as dimensoes da imagem
    cout << "Tamanho: " << largura << " x " << altura << endl;
    if (largura > MAXLARGURA || altura > MAXALTURA)
    {
        cout << "Desculpe, ainda nao trabalho com imagens com mais de " << MAXLARGURA << "x" << MAXALTURA << ").\n";
        arqentrada.close();
        return false;
    }

    // apenas P2 e P3 tem o valor maximo na linha seguinte
    if (strcmp(tipo_imagem, "P1") != 0)
    {
        arqentrada >> valor_pixel; // valor maximo do pixel (assumimos 255)
    }

    // Leitura dos pixels
    if (strcmp(tipo_imagem, "P2") == 0 || strcmp(tipo_imagem, "P1") == 0)
    {
        for (int i = 0; i < altura; i++)
        {
            for (int j = 0; j < largura; j++)
            {
                arqentrada >> valor_pixel;
                // Para P1, 1 vira 255 (branco), 0 vira 0 (preto)
                if (strcmp(tipo_imagem, "P1") == 0 && valor_pixel == 1)
                {
                    imagem_grayscale[i][j] = 255;
                }
                else
                {
                    imagem_grayscale[i][j] = (unsigned char)valor_pixel;
                }
            }
        }
    }
    else
    { // P3
        for (int i = 0; i < altura; i++)
        {
            for (int j = 0; j < largura; j++)
            {
                arqentrada >> valor_pixel;
                imagem_color[i][j][0] = (unsigned char)valor_pixel; // R
                arqentrada >> valor_pixel;
                imagem_color[i][j][1] = (unsigned char)valor_pixel; // G
                arqentrada >> valor_pixel;
                imagem_color[i][j][2] = (unsigned char)valor_pixel; // B
            }
        }
    }

    arqentrada.close();
    return true;
}

// função para gravar a imagem
bool gravarImagem(const string &nomeArquivo)
{
    ofstream arqsaida;

    arqsaida.open(nomeArquivo.c_str(), ios::out);
    if (!arqsaida)
    {
        cout << "Nao consegui criar o arquivo " << nomeArquivo << endl;
        return false;
    }

    arqsaida << tipo_imagem << endl;       // tipo
    arqsaida << "# TP2-INF110, por GCR\n"; // comentario
    arqsaida << largura << " " << altura;  // dimensoes

    if (strcmp(tipo_imagem, "P1") != 0)
    {
        arqsaida << " " << 255 << endl; // maior valor
    }
    else
    {
        arqsaida << endl;
    }

    if (strcmp(tipo_imagem, "P2") == 0)
    {
        for (int i = 0; i < altura; i++)
        {
            for (int j = 0; j < largura; j++)
            {
                arqsaida << (int)imagem_grayscale[i][j] << endl; // pixels
            }
        }
    }
    else if (strcmp(tipo_imagem, "P1") == 0)
    {
        for (int i = 0; i < altura; i++)
        {
            for (int j = 0; j < largura; j++)
            {
                // Para P1, 255 vira 1, 0 vira 0 (ou qualquer valor não-zero vira 1)
                if (imagem_grayscale[i][j] > 127)
                { // Considera branco se > 127 (meio-termo)
                    arqsaida << 1 << endl;
                }
                else
                {
                    arqsaida << 0 << endl;
                }
            }
        }
    }
    else
    { // P3
        for (int i = 0; i < altura; i++)
        {
            for (int j = 0; j < largura; j++)
            {
                arqsaida << (int)imagem_color[i][j][0] << " ";
                arqsaida << (int)imagem_color[i][j][1] << " ";
                arqsaida << (int)imagem_color[i][j][2] << endl;
            }
        }
    }

    arqsaida.close();
    return true;
}

// 2. Clarear/Escurecer
void clarearEscurecer(int fator, bool clarear)
{
    cout << (clarear ? "Clareando" : "Escurecendo") << " imagem com fator " << fator << endl;
    if (strcmp(tipo_imagem, "P2") == 0 || strcmp(tipo_imagem, "P1") == 0)
    {
        for (int i = 0; i < altura; i++)
        {
            for (int j = 0; j < largura; j++)
            {
                // Obtém o valor atual do pixel
                int val = (int)imagem_grayscale[i][j];
                if (clarear)
                {
                    val += fator; // Clareia o pixel
                }
                else
                {
                    val -= fator; // Escurece o pixel
                }
                // Garante que o valor permaneça entre 0-255 e atualiza o pixel
                imagem_grayscale[i][j] = corrige_erros(val);
            }
        }
    }
    else
    { // P3
        for (int i = 0; i < altura; i++)
        {
            for (int j = 0; j < largura; j++)
            {
                // obtem o RGB, separa cada parte do RGB
                int r = (int)imagem_color[i][j][0];
                int g = (int)imagem_color[i][j][1];
                int b = (int)imagem_color[i][j][2];

                if (clarear)
                {
                    r += fator; // Clareia o RGB
                    g += fator;
                    b += fator;
                }
                else
                {
                    r -= fator; // Escurece o RGB
                    g -= fator;
                    b -= fator;
                }
                // garante que ta entre 0-255
                imagem_color[i][j][0] = corrige_erros(r);
                imagem_color[i][j][1] = corrige_erros(g);
                imagem_color[i][j][2] = corrige_erros(b);
            }
        }
    }
    cout << "Operacao concluida.\n";
}

// 3. Negativo
void negativo()
{
    cout << "Aplicando filtro Negativo...\n";
    if (strcmp(tipo_imagem, "P2") == 0 || strcmp(tipo_imagem, "P1") == 0)
    {
        for (int i = 0; i < altura; i++)
        {
            for (int j = 0; j < largura; j++)
            {
                imagem_grayscale[i][j] = 255 - imagem_grayscale[i][j]; // Inverte o valor
            }
        }
    }
    else
    { // P3
        for (int i = 0; i < altura; i++)
        {
            for (int j = 0; j < largura; j++)
            {
                imagem_color[i][j][0] = 255 - imagem_color[i][j][0];
                imagem_color[i][j][1] = 255 - imagem_color[i][j][1];
                imagem_color[i][j][2] = 255 - imagem_color[i][j][2];
            }
        }
    }
    cout << "Operacao concluida.\n";
}

// 4. Espelhar
void espelhar()
{
    cout << "Espelhando imagem...\n";
    if (strcmp(tipo_imagem, "P2") == 0 || strcmp(tipo_imagem, "P1") == 0)
    {
        for (int i = 0; i < altura; i++)
        {
            for (int j = 0; j < largura / 2; j++)
            { // repete ate metade da largura
                unsigned char temp = imagem_grayscale[i][j];
                imagem_grayscale[i][j] = imagem_grayscale[i][largura - 1 - j];
                imagem_grayscale[i][largura - 1 - j] = temp;
            }
        }
    }
    else
    { // P3
        for (int i = 0; i < altura; i++)
        {
            for (int j = 0; j < largura / 2; j++)
            {
                // troca o rgb separado
                unsigned char temp_r = imagem_color[i][j][0];
                unsigned char temp_g = imagem_color[i][j][1];
                unsigned char temp_b = imagem_color[i][j][2];

                imagem_color[i][j][0] = imagem_color[i][largura - 1 - j][0];
                imagem_color[i][j][1] = imagem_color[i][largura - 1 - j][1];
                imagem_color[i][j][2] = imagem_color[i][largura - 1 - j][2];

                imagem_color[i][largura - 1 - j][0] = temp_r;
                imagem_color[i][largura - 1 - j][1] = temp_g;
                imagem_color[i][largura - 1 - j][2] = temp_b;
            }
        }
    }
    cout << "Operacao concluida.\n";
}

// 6. Tons de Cinza (Converte P3 ou P1 para P2)
void converterParaCinza()
{
    if (strcmp(tipo_imagem, "P2") == 0)
    {
        cout << "A imagem ja esta em tons de cinza.\n";
        return;
    }
    if (strcmp(tipo_imagem, "P1") == 0)
    {
        cout << "A imagem ja esta em preto e branco (considerado como tons de cinza para operacoes).\n";
        // ja está no formato grayscale, apenas muda o tipo se quiser tratar P1 como P2
        strcpy(tipo_imagem, "P2");
        cout << "Tipo de imagem atualizado para P2.\n";
        return;
    }

    cout << "Convertendo imagem colorida para tons de cinza...\n";
    for (int i = 0; i < altura; i++)
    {
        for (int j = 0; j < largura; j++)
        {
            // formula simples de media para grayscale
            int gray_val = (imagem_color[i][j][0] + imagem_color[i][j][1] + imagem_color[i][j][2]) / 3;
            imagem_grayscale[i][j] = corrige_erros(gray_val);
        }
    }
    strcpy(tipo_imagem, "P2"); // atualiza o tipo da imagem para P2
    cout << "Conversao concluida.\n";
}

// Filtro de Prewitt
void filtroPrewitt()
{
    cout << "Aplicando filtro de Prewitt simplificado (|Gx| + |Gy|)...\n";

    // serve para identificar as bordas
    const int Gx[3][3] = {{1, 0, -1}, {1, 0, -1}, {1, 0, -1}};
    const int Gy[3][3] = {{1, 1, 1}, {0, 0, 0}, {-1, -1, -1}};

    // cria copias da imagem original para nao alterar os valores originais durante o codico
    unsigned char copia_grayscale[MAXALTURA][MAXLARGURA];
    unsigned char copia_color[MAXALTURA][MAXLARGURA][3];

    // copia a imagem
    if (strcmp(tipo_imagem, "P2") == 0 || strcmp(tipo_imagem, "P1") == 0)
    {
        for (int i = 0; i < altura; i++)
        {
            for (int j = 0; j < largura; j++)
            {
                copia_grayscale[i][j] = imagem_grayscale[i][j];
            }
        }
    }
    else
    { // P3
        for (int i = 0; i < altura; i++)
        {
            for (int j = 0; j < largura; j++)
            {
                copia_color[i][j][0] = imagem_color[i][j][0];
                copia_color[i][j][1] = imagem_color[i][j][1];
                copia_color[i][j][2] = imagem_color[i][j][2];
            }
        }
    }

    // aplicando o filtro e ignora as bordas
    /*
     usando o gx e gy identifica as bordas e com isso da pra ver a diferenca de intensidade
     caso os valores proximos tem valores com variacao grande, detecta uma borda
    */
    for (int i = 1; i < altura - 1; i++)
    {
        for (int j = 1; j < largura - 1; j++)
        {
            if (strcmp(tipo_imagem, "P2") == 0 || strcmp(tipo_imagem, "P1") == 0)
            {
                int gx = 0, gy = 0;

                for (int ki = -1; ki <= 1; ki++)
                {
                    for (int kj = -1; kj <= 1; kj++)
                    {
                        int valor = copia_grayscale[i + ki][j + kj];
                        gx += Gx[ki + 1][kj + 1] * valor;
                        gy += Gy[ki + 1][kj + 1] * valor;
                    }
                }
                // troquei o calculo que foi sugerido pelo valor absoluto pq ao dar uma pesquisada pareceu ser mais facil usando valor absoluto
                int magnitude = abs(gx) + abs(gy);

                imagem_grayscale[i][j] = corrige_erros(magnitude);
            }
            else
            {
                int gx_r = 0, gy_r = 0;
                int gx_g = 0, gy_g = 0;
                int gx_b = 0, gy_b = 0;

                for (int ki = -1; ki <= 1; ki++)
                {
                    for (int kj = -1; kj <= 1; kj++)
                    {
                        gx_r += Gx[ki + 1][kj + 1] * copia_color[i + ki][j + kj][0];
                        gy_r += Gy[ki + 1][kj + 1] * copia_color[i + ki][j + kj][0];
                        gx_g += Gx[ki + 1][kj + 1] * copia_color[i + ki][j + kj][1];
                        gy_g += Gy[ki + 1][kj + 1] * copia_color[i + ki][j + kj][1];
                        gx_b += Gx[ki + 1][kj + 1] * copia_color[i + ki][j + kj][2];
                        gy_b += Gy[ki + 1][kj + 1] * copia_color[i + ki][j + kj][2];
                    }
                }
                imagem_color[i][j][0] = corrige_erros(abs(gx_r) + abs(gy_r));
                imagem_color[i][j][1] = corrige_erros(abs(gx_g) + abs(gy_g));
                imagem_color[i][j][2] = corrige_erros(abs(gx_b) + abs(gy_b));
            }
        }
    }
    cout << "Filtro de Prewitt simplificado aplicado.\n";
}
// filtro que tentei fazer o contorno e em escala de cinza puxado pra branco as areas fora do cotorno
void filtroContornoInvertido()
{
    cout << "Aplicando filtro de contorno invertido (claros em preto, escuros em branco)...\n";

    unsigned char copia_grayscale[MAXALTURA][MAXLARGURA];
    unsigned char copia_color[MAXALTURA][MAXLARGURA][3];

    // copia a imagem
    if (strcmp(tipo_imagem, "P2") == 0 || strcmp(tipo_imagem, "P1") == 0)
    {
        for (int i = 0; i < altura; ++i)
        {
            for (int j = 0; j < largura; ++j)
            {
                copia_grayscale[i][j] = imagem_grayscale[i][j];
            }
        }
    }
    else
    {
        for (int i = 0; i < altura; ++i)
        {
            for (int j = 0; j < largura; ++j)
            {
                copia_color[i][j][0] = imagem_color[i][j][0];
                copia_color[i][j][1] = imagem_color[i][j][1];
                copia_color[i][j][2] = imagem_color[i][j][2];
            }
        }
    }

    // analisando cada pixel e ignorando as bordas
    for (int i = 1; i < altura - 1; ++i)
    {
        for (int j = 1; j < largura - 1; ++j)
        {
            if (strcmp(tipo_imagem, "P2") == 0 || strcmp(tipo_imagem, "P1") == 0)
            {
                // analisa a diferença entre os valores proximos
                int diff_x = abs(copia_grayscale[i][j + 1] - copia_grayscale[i][j - 1]);
                int diff_y = abs(copia_grayscale[i + 1][j] - copia_grayscale[i - 1][j]);
                int edge_strength = (diff_x + diff_y) / 2; // ve a intensidade
                // aqui inverte preto fica claro e claro fica preto
                if (edge_strength > 40)
                {
                    if (copia_grayscale[i][j] > 127)
                    {
                        imagem_grayscale[i][j] = 0;
                    }
                    else
                    {
                        imagem_grayscale[i][j] = 255;
                    }
                }
                else
                {
                    imagem_grayscale[i][j] = 127;
                }
            }
            // aqui pra rgb, pega o pixel atual e verifica os valores em cima,baixo, direita e esquerda, para calcular a media
            else
            {
                int avg_current = (copia_color[i][j][0] + copia_color[i][j][1] + copia_color[i][j][2]) / 3;
                int avg_right = (copia_color[i][j + 1][0] + copia_color[i][j + 1][1] + copia_color[i][j + 1][2]) / 3;
                int avg_left = (copia_color[i][j - 1][0] + copia_color[i][j - 1][1] + copia_color[i][j - 1][2]) / 3;
                int avg_down = (copia_color[i + 1][j][0] + copia_color[i + 1][j][1] + copia_color[i + 1][j][2]) / 3;
                int avg_up = (copia_color[i - 1][j][0] + copia_color[i - 1][j][1] + copia_color[i - 1][j][2]) / 3;
                // calculando a força do contorno
                int diff_x = abs(avg_right - avg_left);
                int diff_y = abs(avg_down - avg_up);
                int edge_strength = (diff_x + diff_y) / 2;

                // invertendo cores
                if (edge_strength > 40)
                {
                    if (avg_current > 127)
                    {
                        imagem_color[i][j][0] = 0;
                        imagem_color[i][j][1] = 0;
                        imagem_color[i][j][2] = 0;
                    }
                    else
                    {
                        imagem_color[i][j][0] = 255;
                        imagem_color[i][j][1] = 255;
                        imagem_color[i][j][2] = 255;
                    }
                }
                else
                {

                    imagem_color[i][j][0] = 127;
                    imagem_color[i][j][1] = 127;
                    imagem_color[i][j][2] = 127;
                }
            }
        }
    }
    cout << "Operacao concluida.\n";
}

// filtro Cor Quente
void filtroCorQuente()
{
    cout << "Aplicando filtro de cor quente...\n";
    if (strcmp(tipo_imagem, "P2") == 0 || strcmp(tipo_imagem, "P1") == 0)
    {
        cout << "Este filtro so pode ser aplicado a imagens coloridas (P3).\n";
        return;
    }

    for (int i = 0; i < altura; i++)
    {
        for (int j = 0; j < largura; j++)
        {
            // Aumenta o componente vermelho e diminui ligeiramente o azul e verde
            int r = (int)imagem_color[i][j][0] + 50;
            int g = (int)imagem_color[i][j][1] - 20;
            int b = (int)imagem_color[i][j][2] - 30;

            imagem_color[i][j][0] = corrige_erros(r);
            imagem_color[i][j][1] = corrige_erros(g);
            imagem_color[i][j][2] = corrige_erros(b);
        }
    }
    cout << "Operacao concluida.\n";
}

// Novo filtro: Cor Fria
void filtroCorFria()
{
    cout << "Aplicando filtro de cor fria...\n";
    if (strcmp(tipo_imagem, "P2") == 0 || strcmp(tipo_imagem, "P1") == 0)
    {
        cout << "Este filtro so pode ser aplicado a imagens coloridas (P3).\n";
        return;
    }
    // vendo todos os pixels
    for (int i = 0; i < altura; i++)
    {
        for (int j = 0; j < largura; j++)
        {
            /*
            Aqui pra criar o efeito quente, precisamos aumentar o vermelho, diminuir o verde e o azul

            */
            int r = (int)imagem_color[i][j][0] - 30;
            int g = (int)imagem_color[i][j][1] - 20;
            int b = (int)imagem_color[i][j][2] + 50;

            imagem_color[i][j][0] = corrige_erros(r);
            imagem_color[i][j][1] = corrige_erros(g);
            imagem_color[i][j][2] = corrige_erros(b);
        }
    }
    cout << "Operacao concluida.\n";
}

// filtro com as cores da bandeira do Brasil
void filtroCoresdoBrasil()
{
    cout << "Aplicando filtro Bandeira do Brasil (esquema de cores)...\n";

    if (strcmp(tipo_imagem, "P2") == 0 || strcmp(tipo_imagem, "P1") == 0)
    {
        cout << "Funciona melhor em imagens coloridas.\n";
        // sai se for preto e branco
        return;
    }

    /*
    queria fazer algo tipo a bandeira do brasil, mas tava dando muito trabalho, entao resolvi fazer somente as cores do brasil
    dividido em 3 grandes retangulos.
    */
    int faixa_verde_fim = altura / 3;
    int faixa_amarela_fim = (altura * 2) / 3;

    for (int i = 0; i < altura; i++)
    {
        for (int j = 0; j < largura; j++)
        { // guardando as cores originais do pixel
            int r = (int)imagem_color[i][j][0];
            int g = (int)imagem_color[i][j][1];
            int b = (int)imagem_color[i][j][2];
            // proximo if,else if e else  aplica o verde, amarelo e azul
            if (i < faixa_verde_fim)
            {

                r = corrige_erros(r * 0.7 + 0 * 0.3);
                g = corrige_erros(g * 0.7 + 158 * 0.3);
                b = corrige_erros(b * 0.7 + 96 * 0.3);
            }
            else if (i < faixa_amarela_fim)
            {

                r = corrige_erros(r * 0.7 + 255 * 0.3);
                g = corrige_erros(g * 0.7 + 223 * 0.3);
                b = corrige_erros(b * 0.7 + 0 * 0.3);
            }
            else
            {

                r = corrige_erros(r * 0.7 + 0 * 0.3);
                g = corrige_erros(g * 0.7 + 39 * 0.3);
                b = corrige_erros(b * 0.7 + 118 * 0.3);
            }
            imagem_color[i][j][0] = r;
            imagem_color[i][j][1] = g;
            imagem_color[i][j][2] = b;
        }
    }
    cout << "Operacao concluida. Cores da bandeira do Brasil aplicadas em faixas suaves.\n";
}
// tentando fazer um filtro que mostra como se fosse as formas geometricas da bandeira do brasil, em cima da imagem

void filtroBandeiraBrasil()
{
    cout << "Aplicando filtro Bandeira do Brasil (formas simplificadas sobrepostas)...\n";
    // so funciona em p3
    if (strcmp(tipo_imagem, "P2") == 0 || strcmp(tipo_imagem, "P1") == 0)
    {
        cout << "Este filtro funciona melhor com imagens coloridas (P3). Converta para P3 ou use uma imagem P3.\n";
        return;
    }

    double fator_mistura = 0.3;

    // calculando as dimensoes para as figuras
    int min_dim = min(largura, altura);
    if (largura < altura)
        min_dim = largura;
    else
        min_dim = altura;

    // definindo a area do losangulo
    //  70% da altura e da largura
    int losango_x_inicio = largura * 0.15;
    int losango_x_fim = largura * 0.85;
    int losango_y_inicio = altura * 0.15;
    int losango_y_fim = altura * 0.85;

    // calculo do meio da imagem para criar o circulo
    int centro_x = largura / 2;
    int centro_y = altura / 2;

    int raio_circulo = min_dim * 0.20;
    int raio_circulo_quadrado = raio_circulo * raio_circulo;

    for (int i = 0; i < altura; i++)
    {
        for (int j = 0; j < largura; j++)
        {

            int r_orig = (int)imagem_color[i][j][0];
            int g_orig = (int)imagem_color[i][j][1];
            int b_orig = (int)imagem_color[i][j][2];

            // círculo azul central
            // se os valores for menor que o raio do circulo, entao ta dentro da area, e deve ser pintado
            int dist_x_sq = (j - centro_x) * (j - centro_x);
            int dist_y_sq = (i - centro_y) * (i - centro_y);

            if (dist_x_sq + dist_y_sq < raio_circulo_quadrado)
            {

                imagem_color[i][j][0] = corrige_erros(r_orig * (1 - fator_mistura) + 0 * fator_mistura);
                imagem_color[i][j][1] = corrige_erros(g_orig * (1 - fator_mistura) + 39 * fator_mistura);
                imagem_color[i][j][2] = corrige_erros(b_orig * (1 - fator_mistura) + 118 * fator_mistura);
            }
            // verificando se o pixel ta dentro da area do losangulo
            else if (j >= losango_x_inicio && j <= losango_x_fim &&
                     i >= losango_y_inicio && i <= losango_y_fim)
            {

                imagem_color[i][j][0] = corrige_erros(r_orig * (1 - fator_mistura) + 255 * fator_mistura);
                imagem_color[i][j][1] = corrige_erros(g_orig * (1 - fator_mistura) + 223 * fator_mistura);
                imagem_color[i][j][2] = corrige_erros(b_orig * (1 - fator_mistura) + 0 * fator_mistura);
            }
            // coloca verde na area restante
            else
            {

                imagem_color[i][j][0] = corrige_erros(r_orig * (1 - fator_mistura) + 0 * fator_mistura);
                imagem_color[i][j][1] = corrige_erros(g_orig * (1 - fator_mistura) + 158 * fator_mistura);
                imagem_color[i][j][2] = corrige_erros(b_orig * (1 - fator_mistura) + 96 * fator_mistura);
            }
        }
    }
    cout << "Operacao concluida. Bandeira do Brasil sobreposta.\n";
}

int main()
{
    string nome_arquivo_entrada, nome_arquivo_saida;
    int escolha;
    bool imagem_carregada = false;
    // as variáveis nome_arquivo_entrada e nome_arquivo_saida são usadas para armazenar os nomes dos arquivos
    //  ler imagem virou uma função separada, então não precisamos mais dessas variáveis aqui

    do
    {
        cout << "\n--- Menu de Manipulacao de Imagens PNM ---\n";
        cout << "1. Ler imagem (P1/P2/P3)\n";
        cout << "2. Salvar imagem\n";
        cout << "3. Clarear imagem\n";
        cout << "4. Escurecer imagem\n";
        cout << "5. Gerar negativo\n";
        cout << "6. Espelhar imagem verticalmente\n";
        cout << "7. Converter para tons de cinza (P2)\n";
        cout << "8. Aplicar filtro de Prewitt (Contornos)\n";
        cout << "9. Aplicar filtro de contorno invertido (claros em preto, escuros em branco)\n";
        cout << "10. Aplicar filtro de cores quentes\n";
        cout << "11. Aplicar filtro de cores frias\n";
        cout << "12. Aplicar filtro cores da Bandeira do Brasil (cores)\n";
        cout << "13. Aplicar filtro da Bandeira do Brasil \n";
        cout << "0. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> escolha;

        if (escolha != 1 && escolha != 0 && !imagem_carregada)
        {
            cout << "Por favor, primeiro carregue uma imagem (Opcao 1).\n";
            continue;
        }

        switch (escolha)
        {
        case 1:
        {
            cout << "Digite o nome do arquivo de entrada (ex: imagem.pnm): ";
            cin >> nome_arquivo_entrada;
            imagem_carregada = lerImagem(nome_arquivo_entrada);
            break;
        }
        case 2:
        {
            if (imagem_carregada)
            {
                cout << "Digite o nome do arquivo de saida (ex: novaimagem.pnm): ";
                cin >> nome_arquivo_saida;
                if (gravarImagem(nome_arquivo_saida))
                {
                    cout << "Imagem salva com sucesso como " << nome_arquivo_saida << endl;
                }
                else
                {
                    cout << "Erro ao salvar a imagem.\n";
                }
            }
            break;
        }
        case 3:
        {
            int fator;
            cout << "Digite o fator de clareamento (1-255): ";
            cin >> fator;
            clarearEscurecer(fator, true);
            break;
        }
        case 4:
        {
            int fator;
            cout << "Digite o fator de escurecimento (1-255): ";
            cin >> fator;
            clarearEscurecer(fator, false);
            break;
        }
        case 5:
        {
            negativo();
            break;
        }
        case 6:
        {
            espelhar();
            break;
        }
        case 7:
        {
            converterParaCinza();
            break;
        }
        case 8:
        {
            filtroPrewitt();
            break;
        }
        case 9:
        {
            filtroContornoInvertido();
            break;
        }
        case 10:
        {
            filtroCorQuente();
            break;
        }
        case 11:
        {
            filtroCorFria();
            break;
        }
        case 12:
        {
            filtroCoresdoBrasil();
            break;
        }
        case 13:
        {
            filtroBandeiraBrasil();
            break;
        }
        case 0:
        {
            cout << "Saindo do programa. Ate mais!\n";
            break;
        }
        default:
        {
            cout << "Opcao invalida. Tente novamente.\n";
            break;
        }
        }
    } while (escolha != 0);

    return 0;
}