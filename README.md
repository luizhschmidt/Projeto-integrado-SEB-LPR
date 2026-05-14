Projeto-integrado-SEB-LPR
Sistema embarcado desenvolvido para controle de presença em sala de aula, com autenticação por senha, configuração de limite de alunos e gerenciamento de entradas 
e saídas em tempo real, tudo controlado por 4 botões físicos e exibido em um display.

Parte 1 — Autenticação por Senha
O professor deve pressionar o botão de cima para dar início à aula. Após isso, o sistema gera uma senha aleatória e a exibe via debuG para queo professor possa
visualizá-la.
Antes de digitar a senha, aparece uma tela com as instruções de cada botão nessa etapa:

Botão:
Cima- Confirma o dígito e avança para o próximo 
Esquerda- Aumenta o dígito em +1
Direita- Diminui o dígito em -1 

O professor digita a senha dígito por dígito conforme exibida no terminal. O sistema permite até 3 tentativas. Caso erre nas 3, o sistema é bloqueado imediatamente 
e não pode ser usado sem um reset físico. Acertando, o sistema avança para a próxima etapa.

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
