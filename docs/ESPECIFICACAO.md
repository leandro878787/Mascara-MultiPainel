# Especificação funcional — Máscara MultiPainel

## 1. Finalidade

Criar um efeito FFGL que receba a textura de entrada do Resolume e apresente seu conteúdo através de uma sequência configurável de painéis, deixando visível o fundo entre eles.

## 2. Modos de mapeamento

### Contínuo
A coordenada horizontal de cada painel corresponde à sua posição no conjunto completo. A imagem de entrada é amostrada como uma única superfície panorâmica, sem reiniciar o enquadramento em cada painel.

### Duplicado
Cada painel usa seu próprio enquadramento normalizado, exibindo a imagem inteira novamente.

## 3. Geometrias

- Retângulo: painel retangular.
- Arco: painel com topo curvo.
- Círculo: máscara circular/elíptica.

As geometrias devem ser calculadas em coordenadas normalizadas e respeitar os limites do painel.

## 4. Parâmetros planejados

| Parâmetro | Descrição |
|---|---|
| Modo | Contínuo ou Duplicado |
| Quantidade | Número de painéis |
| Largura | Largura relativa de cada painel |
| Altura | Altura relativa de cada painel |
| Espaçamento | Distância entre painéis |
| Formato | Retângulo, Arco ou Círculo |
| Arredondamento | Raio dos cantos |
| Zoom | Ampliação do conteúdo |
| Posição X/Y | Deslocamento do conteúdo |
| Suavização | Feather das bordas da máscara |
| Fundo | Cor exibida fora das máscaras |

## 5. Critérios de aceitação

1. O plugin deve ser reconhecido como efeito FFGL pelo Resolume Arena 7.22.
2. Deve processar a textura de entrada sem alterar a fonte original.
3. O modo Contínuo deve manter a continuidade visual entre os painéis.
4. O modo Duplicado deve reiniciar o enquadramento em cada painel.
5. Os espaços externos às máscaras devem exibir a cor de fundo escolhida.
6. Os parâmetros devem ser ajustáveis pela interface do Resolume.
7. A DLL final deve ser compilada e testada em Windows 64-bit antes de ser considerada pronta.

## 6. Compatibilidade e SDK

Usar o SDK mantido pela Resolume: https://github.com/Resolume/ffgl

A branch principal do SDK documenta suporte a plugins para Resolume 7.3.1 ou superior, portanto é adequada como base para o Arena 7.22. A integração concreta deve seguir os projetos e exemplos de efeito existentes no SDK.
