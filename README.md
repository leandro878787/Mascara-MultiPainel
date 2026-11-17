# Máscara MultiPainel

Plugin de efeito FFGL para Resolume Arena 7.22 (Windows 10/11, 64-bit).

## Estado do projeto

Primeiro protótipo técnico em desenvolvimento. A implementação inicial prepara dois modos de distribuição da imagem e dois formatos de painel. Ainda precisa ser compilada e testada no Arena antes de ser considerada funcional.

## Recursos do protótipo

- **Contínuo:** a imagem é distribuída como um panorama ao longo dos painéis.
- **Duplicado:** a imagem inteira é repetida em cada painel.
- **Formato retangular ou circular/elíptico.**
- Ajuste de quantidade de painéis e espaço entre eles.

## Compilar

O workflow do GitHub Actions em Actions > Compilar DLL FFGL baixa o SDK oficial da Resolume, compila para Windows x64 e publica a DLL como artefato do workflow.

O SDK oficial usado como base é https://github.com/Resolume/ffgl.

## Instalação após uma compilação bem-sucedida

Copie MascaraMultiPainel.dll para:

%USERPROFILE%\Documents\Resolume\Extra Effects

Depois reinicie o Resolume Arena. A DLL só deve ser considerada pronta após confirmar que aparece e funciona no Arena.

## Próximas etapas

1. Validar a compilação no GitHub Actions.
2. Testar a DLL no Resolume Arena 7.22.
3. Melhorar os controles (nomes, faixas e opções visuais).
4. Adicionar arco/topo curvo, cor de fundo, zoom, posição e feather.
