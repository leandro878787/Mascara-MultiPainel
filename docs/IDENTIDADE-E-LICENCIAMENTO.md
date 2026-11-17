# Identidade e licenciamento — Máscara MultiPainel

## Identidade do produto

- Nome exibido no Resolume: **Máscara MultiPainel**
- Assinatura do desenvolvedor: **by @leandrone_x**
- Instagram: **@leandrone_x**
- Descrição curta: Efeito FFGL para dividir a imagem em painéis e máscaras, com modos contínuo e duplicado.

O nome principal permanece curto para facilitar a localização na lista de efeitos. A assinatura deve aparecer nos metadados de apresentação, no README, na página do produto e na interface de licença.

## Política comercial definida

- Uma licença ativa por computador.
- Ativação e validação pela internet.
- O cliente usa uma chave de licença fornecida após a compra.
- O cliente pode solicitar a transferência da licença para outro computador. A transferência desativa a ativação anterior antes de liberar a nova.
- Não vincular a licença ao número de série ou à conta do Resolume. A licença é do plugin e do computador autorizado.

## Componentes da solução

### 1. Plugin FFGL (DLL)

Responsabilidades:
- Exibir o estado da licença: Ativado, Verificando, Sem conexão ou Licença inválida.
- Enviar a chave e um identificador derivado do computador ao serviço de ativação.
- Validar localmente a assinatura digital do comprovante recebido.
- Não armazenar senhas, chaves privadas do servidor ou credenciais administrativas.

O identificador do computador deve ser um hash derivado de características técnicas permitidas, sem transmitir ou exibir números de série brutos. Mudanças normais de hardware precisam ter tratamento de recuperação para evitar bloqueios indevidos.

### 2. Serviço online de licenças

Responsabilidades:
- Registrar chaves de licença e seu estado.
- Ativar uma chave em apenas um computador por vez.
- Revalidar ativações e permitir desativação/transferência.
- Registrar eventos essenciais para suporte e auditoria, evitando coletar dados desnecessários.

Endpoints conceituais:
- POST /v1/activate — ativa a licença para o computador.
- POST /v1/validate — verifica se a ativação continua válida.
- POST /v1/deactivate — solicita a liberação do computador.
- POST /v1/transfer — fluxo administrativo de transferência, com confirmação do titular.

Esses caminhos são uma proposta de API; ainda não existe servidor implementado.

### 3. Assinatura digital

O servidor gera um comprovante de licença assinado digitalmente. A DLL contém somente a chave pública necessária para verificar a assinatura.

O comprovante deve conter, no mínimo:
- Identificador do produto;
- Identificador da licença;
- Hash do computador autorizado;
- Estado da licença;
- Data de emissão e, se aplicável, expiração.

A chave privada permanece exclusivamente no servidor, protegida e fora do repositório GitHub. Não usar uma chave secreta compartilhada embutida na DLL como mecanismo principal de segurança.

## Fluxo de ativação

1. O cliente instala a DLL no computador em que usa o Resolume.
2. Abre o painel de licença do Máscara MultiPainel.
3. Informa a chave de licença.
4. O plugin envia a solicitação ao serviço online.
5. O servidor confere se a chave existe, está válida e ainda não está ativada em outro computador.
6. Se estiver tudo certo, o servidor registra a ativação e devolve um comprovante assinado.
7. A DLL verifica a assinatura e libera o efeito.

## Disponibilidade durante apresentações

A ativação inicial exige internet. Depois disso, o plugin pode guardar um comprovante assinado e válido por um período curto de tolerância, para evitar que uma queda temporária de conexão interrompa uma apresentação. A política exata desse período será definida antes do lançamento; a validação online continua sendo necessária para ativar e renovar a licença.

## Etapas de implementação

1. Finalizar e validar o efeito FFGL no Resolume Arena 7.22.
2. Implementar a tela/estado de licença no plugin.
3. Criar o serviço de licenças e seu armazenamento.
4. Implementar assinatura digital e validação na DLL.
5. Testar ativação, tentativa de segundo computador, transferência, reinstalação e indisponibilidade temporária da internet.
6. Publicar a versão comercial e instruções de suporte.

## Estado atual

A assinatura **@leandrone_x** foi adicionada ao texto About dos metadados do plugin no código-fonte. O sistema de licenças descrito aqui é uma especificação de projeto, não uma funcionalidade já implementada. A DLL ainda precisa compilar e ser testada no Resolume Arena 7.22.
