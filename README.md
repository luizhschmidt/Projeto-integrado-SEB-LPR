Projeto-integrado-SEB-LPR

Sistema embarcado desenvolvido em C para a plataforma STM32, com display ST7735, comunicação UART e controle por 4 botões físicos. Gerencia autenticação do professor, registro de alunos e encerramento de aula com relatório final.

Parte 1 — Autenticação por Senha
O professor deve pressionar o botão de cima para dar início à aula. Após isso, o sistema gera uma senha aleatória e a exibe via debug para que o professor possa visualizá-la. Antes de digitar a senha, aparece uma tela com as instruções de cada botão nessa etapa:

Botão:
Cima- Confirma o dígito e avança para o próximo 
Esquerda- Aumenta o dígito em +1
Direita- Diminui o dígito em -1 

O professor digita a senha dígito por dígito conforme exibida no terminal. O sistema permite até 3 tentativas. Caso erre nas 3, o sistema é bloqueado imediatamente e não pode ser usado sem um reset físico. Acertando, o sistema avança para a próxima etapa.

Parte 2 — Configuração do Limite de Alunos
Após digitar a senha corretamente, o professor configura o **número máximo de alunos** que poderão entrar na aula. Antes dessa tela, aparece uma tela com as instruções
de cada botão:

Botão:
Cima- Confirma o valor configurado 
Baixo- Reseta o valor para 0
Esquerda- Aumenta +1 aluno
Direita- Aumenta +5 alunos 

É impossível confirmar com o limite em 0. O sistema não permite iniciar uma aula sem nenhum aluno configurado.

Parte 3 — Controle da Aula
Nessa etapa é possível visualizar em tempo real quantos alunos estão presentes (ex: 50/70) e quantos saíram da sala (ex: 2/3). Os comandos disponíveis também aparecem
Na tela.

Entrada de aluno — Botão Cima
Ao pressionar o botão de cima, o sistema registra a entrada de um aluno. Uma tela passageira exibe qual número de aluno foi registrado (1º aluno, 2º aluno...) e o 
professor digita a matrícula via debug para que ela seja armazenada junto ao registro. Se a sala já estiver cheia, o sistema exibe SALA CHEIA e não permite o registro.

Saída de aluno — Botão Esquerda
Ao pressionar o botão da esquerda, o sistema registra que um aluno saiu da sala temporariamente. O contador de presentes cai (ex: de 5/5 para 4/5) e o de alunos fora 
sobe (ex: de 0/3 para 1/3).
Caso já haja 3 alunos fora da sala, o sistema não permite a saída de mais nenhum, exibindo uma mensagem de aviso.

Retorno de aluno — Botão Direita
Ao pressionar o botão da direita, o sistema registra o retorno de um aluno para a sala. O contador de fora diminui e o de presentes volta ao valor anterior. Caso não
haja ninguém fora, o sistema exibe uma mensagem informando que não há retornos pendentes.

Encerrar aula — Botão Cima + Botão Baixo (simultaneamente)
Pressionar os dois botões ao mesmo tempo encerra a aula e leva para a tela de relatório final.

Parte 4 — Encerramento da Aula
Ao encerrar, o display exibe um resumo com:

- Total de entradas registradas
- Total de saídas registradas
- Tempo total de duração da aula em minutos

Após isso, os LEDs piscam como sinalização de encerramento e o sistema trava, impedindo qualquer uso acidental após o fim da aula.


ESTRUTURA DO CÓDIGO:

main() — Inicialização, tela de espera e loop principal (switch/case por etapa)

GerarExibirSenha() — Gera senha aleatória com rand()

ProcessarEntradaSenha() — Leitura dos botões para entrada dos 3 dígitos da senha

ValidarAcesso() — Compara entrada com senha gerada; controla tentativas e bloqueio

ConfigurarLimiteAlunos() — Loop de configuração do número máximo de alunos

ExecutarControleAula() — Lógica principal de entradas, saídas e retornos

EncerrarAula()  — Exibe relatório, pisca LEDs e trava o sistema

DesenharBarraTopo() — Renderiza barra de progresso visual no topo do display



Decisões Tomadas e Arquitetura de Software:

Máquina de Estados (switch/case): O loop principal gerencia o fluxo por variáveis de controle (etapa). Isso força o sistema a seguir a ordem exata do projeto (Senha -> Limite -> Controle -> Relatório), impedindo o avanço sem validação.

Uso de Macros (#define): Substituição de comandos extensos da HAL (como HAL_GPIO_ReadPin) por termos curtos (BCima, BEsquerda, L1). Isso evitou poluição visual e facilitou a escrita da lógica de botões e LEDs.

Variáveis e Buffers Locais: A criação de arrays de texto (buffers) para escrita no display foi feita estritamente dentro de cada função. Isso economizou a memória RAM do microcontrolador e evitou o uso desnecessário de variáveis globais.

Controle Total por Botões Físicos: Toda a interação do sistema (navegação de menus, incremento de valores, registro de alunos e confirmações) foi concentrada nos 4 botões direcionais mapeados via GPIO, tornando o projeto um sistema embarcado autônomo e independente de periféricos externos de comunicação.

Cálculo Proporcional da Barra: A função DesenharBarraTopo usa uma regra de três simples para preencher a barra de progresso baseada no limite máximo de alunos. Há uma trava para evitar divisão por zero e uma lógica para mudar a cor de verde para vermelho quando a capacidade é atingida.

Travamento por Segurança (while(1)): No bloqueio por erro de senha e na tela final de relatório, o código entra em loop infinito estático. Isso impede que o sistema continue recebendo comandos ou sofra alterações acidentais após o fim da aula ou violação de acesso.



Link do video:

https://youtu.be/utVKLg_h0vM
