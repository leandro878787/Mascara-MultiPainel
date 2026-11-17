# Máscara MultiPainel

Plugin de efeito **FFGL para Resolume Arena 7.22**, desenvolvido em C++ para Windows 10/11 64-bit.

## Objetivo

Transformar uma imagem ou vídeo em vários painéis visuais independentes, separados por espaços de fundo. O efeito terá dois modos principais:

- **Contínuo:** distribui a imagem inteira ao longo do conjunto de painéis, como uma única composição panorâmica.
- **Duplicado:** repete a imagem completa em cada painel.

## Controles planejados

- Quantidade de painéis
- Largura e altura dos painéis
- Espaçamento entre painéis
- Formato: retângulo, arco e círculo
- Arredondamento dos cantos
- Zoom e posição horizontal/vertical
- Suavização das bordas
- Cor do fundo

## Ambiente-alvo

- Resolume Arena 7.22
- Windows 10/11 64-bit
- Plugin FFGL (biblioteca DLL)
- SDK oficial: https://github.com/Resolume/ffgl

## Estado do projeto

**Fase 1 — especificação e estrutura inicial.** A implementação do efeito e a compilação da DLL ainda serão feitas e testadas. Não há binário pronto neste momento.

## Desenvolvimento

O projeto será preparado para compilar no Windows por meio do SDK oficial do Resolume e de um workflow do GitHub Actions. O workflow de compilação será habilitado depois que o projeto FFGL estiver integrado à estrutura real do SDK.

Consulte docs/ESPECIFICACAO.md para os requisitos funcionais.
