# Tratamento e Manipulação de Imagens PNM em C++

Este projeto é um programa de console desenvolvido em C++ para a disciplina de **INF 110 - Programação 1** da **Universidade Federal de Viçosa (UFV)**. O software permite ler, aplicar diversos filtros e transformações, e gravar imagens no formato PNM (tipos P1, P2 e P3).

## 🌟 Funcionalidades Implementadas

O programa oferece um menu interativo com uma variedade de operações de manipulação de imagem:

* **Leitura e Gravação**: Suporte para carregar e salvar imagens nos formatos PNM P1 (preto e branco), P2 (tons de cinza) e P3 (colorido).
* **Clarear e Escurecer**: Ajusta o brilho da imagem adicionando ou subtraindo um fator de cada pixel.
* **Negativo**: Inverte as cores da imagem (255 - valor original), transformando pixels claros em escuros e vice-versa.
* **Espelhamento**: Inverte a imagem no eixo horizontal.
* **Conversão para Tons de Cinza**: Converte uma imagem colorida (P3) para o formato P2, utilizando a média dos valores dos canais RGB.
* **Filtro de Prewitt**: Aplica um filtro para detecção de bordas utilizando uma magnitude simplificada dos operadores de Prewitt ($|Gx| + |Gy|$).
* **Filtro de Contorno Invertido**: Um filtro customizado que destaca as bordas, transformando áreas claras em preto, áreas escuras em branco, e preenchendo o restante com um tom de cinza médio.
* **Filtros de Cor**:
    * **Cores Quentes**: Aplica um filtro que realça os tons de vermelho na imagem.
    * **Cores Frias**: Aplica um filtro que realça os tons de azul na imagem.
* **Filtros Temáticos (Bandeira do Brasil)**:
    * **Esquema de Cores**: Aplica as cores da bandeira do Brasil em faixas horizontais que se misturam suavemente com a imagem original.
    * **Formas Geométricas**: Sobrepõe as formas da bandeira (retângulo, losango e círculo) sobre a imagem, mesclando as cores com o conteúdo original.

## 🔧 Como Compilar e Executar

Para compilar e executar o projeto, você precisará de um compilador C++, como o g++.

1.  **Clone o repositório:**
    ```bash
    git clone [https://github.com/gabrielreisz/manipulando_pnm.git](https://github.com/gabrielreisz/manipulando_pnm.git)
    cd manipulando_pnm
    ```

2.  **Compile o código-fonte:**
    O arquivo principal `tp2_gabriel.cpp` está no diretório `src/`. Use o seguinte comando para compilar:
    ```bash
    g++ src/tp2_gabriel.cpp -o tp2_gabriel -Wall
    ```

3.  **Execute o programa:**
    ```bash
    ./tp2_gabriel
    ```

4.  **Siga as instruções do menu:**
    * Primeiro, carregue uma imagem com a **opção 1**.
    * Aplique um ou mais filtros (opções 3 a 13).
    * Salve a imagem modificada com a **opção 2**.
    * Use a **opção 0** para sair.

## 🖼️ Exemplos de Filtros Aplicados

A seguir, alguns exemplos dos filtros disponíveis, aplicados a uma imagem de referência.

| Original                                    | Negativo                                         |
| :------------------------------------------: | :----------------------------------------------: |
| ![Imagem Original](https://github.com/gabrielreisz/manipulando_pnm/blob/45dae25d78dbffc75ab69c4fa396253d89fde8c0/examples/original.png) | ![Imagem com Filtro Negativo](https://github.com/gabrielreisz/manipulando_pnm/blob/45dae25d78dbffc75ab69c4fa396253d89fde8c0/examples/negativo.png) |
| **Filtro de Prewitt** | **Filtro Bandeira do Brasil (Formas)** |
| ![Imagem com Filtro de Prewitt](https://github.com/gabrielreisz/manipulando_pnm/blob/45dae25d78dbffc75ab69c4fa396253d89fde8c0/examples/prewitt.png) | ![Imagem com Filtro da Bandeira](https://github.com/gabrielreisz/manipulando_pnm/blob/45dae25d78dbffc75ab69c4fa396253d89fde8c0/examples/bandeira.png) |



---
*Trabalho desenvolvido por Gabriel Costa Reis em 6 de junho de 2025.*
