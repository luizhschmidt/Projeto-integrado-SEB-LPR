/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  * opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "st7735\st7735.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BCima HAL_GPIO_ReadPin(BNorte_GPIO_Port, BNorte_Pin)
#define BBaixo HAL_GPIO_ReadPin(BSul_GPIO_Port, BSul_Pin)
#define BEsquerda HAL_GPIO_ReadPin(BOeste_GPIO_Port, BOeste_Pin)
#define BDireita HAL_GPIO_ReadPin(BLeste_GPIO_Port, BLeste_Pin)
#define L1 HAL_GPIO_TogglePin(L1_GPIO_Port, L1_Pin); HAL_Delay(200)
#define L2 HAL_GPIO_TogglePin(L2_GPIO_Port, L2_Pin); HAL_Delay(200)
#define L3 HAL_GPIO_TogglePin(L3_GPIO_Port, L3_Pin); HAL_Delay(200)
#define L4 HAL_GPIO_TogglePin(L4_GPIO_Port, L4_Pin); HAL_Delay(200)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char senha_inteira[3];
int matricula_do_aluno = 0;
int campo[100];
int tentativas = 0;
int bloqueado = 0;
int limAlunos = 0;
int etapa = 0;
int senha[3];
int entrada[3] = {0, 0, 0};
int presentes = 0;
int presentes2 = 0;
int foraSala = 0;
int totalEntradas = 0;
int totalSaidas = 0;
uint32_t tempoInicio = 0;
uint32_t tempoFim = 0;
uint32_t duracaoTotalM = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void ConfigurarLimiteAlunos(void);
void GerarExibirSenha(void);
void ProcessarEntradaSenha(void);
void ValidarAcesso(void);
void ExecutarControleAula(void);
void EncerrarAula(void);
void DesenharBarraTopo(int atual, int maximo);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  ST7735_Init();
  char buffer[50];

  //GPIO_PinState BCima = BCimaa;
  //GPIO_PinState BBaixo = BBaixoo;
  //GPIO_PinState BEsquerda = BEsquerdaa;
  //GPIO_PinState BDireita = BDireita;


  // 1. TELA DE ESPERA INICIAL (O único lugar que espera botão)
  ST7735_FillScreen(0xFFFF); // Fundo Branco
  ST7735_WriteString(5, 33, "PRESSIONE O BOTAO", Font_7x10, 0x0000, 0xFFFF);
  ST7735_WriteString(5, 45, "DE CIMA PARA INICIAR", Font_7x10, 0x0000, 0xFFFF);

  while(BCima == 1); // Aqui ele trava até você apertar. Depois daqui, tudo é automático.
  tempoInicio = HAL_GetTick(); // <--- Adicione isso aqui
  srand(HAL_GetTick());
  L1; L2; L3; L4;

  // 2. TELA "GERANDO SENHA" (3 segundos automáticos)
  ST7735_FillScreen(0xFFFF);
  ST7735_WriteString(5, 15, "Gerando senha.  ", Font_7x10, 0x0000, 0xFFFF);
  HAL_Delay(500);
  ST7735_WriteString(5, 15, "Gerando senha.. ", Font_7x10, 0x0000, 0xFFFF);
  HAL_Delay(500);
  ST7735_WriteString(5, 15, "Gerando senha...", Font_7x10, 0x0000, 0xFFFF);
  HAL_Delay(500);
  GerarExibirSenha();
  L1; L2; L3; L4;
  ST7735_FillScreen(0xFFFF);
  ST7735_WriteString(5, 27, "VEJA A SENHA NO PC ", Font_7x10, 0x0000, 0xFFFF);
  ST7735_WriteString(5, 40, "   E PRESSIONE F8", Font_7x10, 0x0000, 0xFFFF);

  ST7735_FillScreen(0xFFFF);
  ST7735_WriteString(5, 10, "CONTROLES SENHA:", Font_7x10, 0x0000, 0xFFFF);
  ST7735_WriteString(5, 30, "Esq: +1 | Dir: -1", Font_7x10, 0x0000, 0xFFFF);
  ST7735_WriteString(5, 45, "Baixo: Reset", Font_7x10, 0x0000, 0xFFFF);
  ST7735_WriteString(5, 60, "Cima: Confirmar", Font_7x10, 0x0000, 0xFFFF);
  HAL_Delay(3500); // Tempo para ler
  ST7735_FillScreen(0xFFFF); // Apaga

  //sprintf(buffer, "SENHA: %d%d%d", senha[0], senha[1], senha[2]);
    //ST7735_WriteString(10, 50, buffer, Font_11x18, 0x07FF, 0x0000);
    //HAL_Delay(4000);

  //ST7735_FillScreen(0x0000);
  //HAL_Delay(5000); // Fica parado aqui por 3s

  //ST7735_FillScreen(0x0000); pppppp

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
    {
      switch (etapa)
      {
        case 0:
        case 1:
        case 2:
          ProcessarEntradaSenha();
          break;
        case 3:
          ValidarAcesso();
          break;
        case 4:
        ConfigurarLimiteAlunos();
          etapa = 5;
          break;
        case 5:
          ExecutarControleAula();
          break;
        case 6:
          EncerrarAula();
          break;
      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ST7735_DC_Pin|ST7735_RES_Pin|L1_Pin|L2_Pin
                          |L3_Pin|L4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : ST7735_CS_Pin */
  GPIO_InitStruct.Pin = ST7735_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ST7735_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ST7735_DC_Pin ST7735_RES_Pin L1_Pin L2_Pin
                           L3_Pin L4_Pin */
  GPIO_InitStruct.Pin = ST7735_DC_Pin|ST7735_RES_Pin|L1_Pin|L2_Pin
                          |L3_Pin|L4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : BOeste_Pin BSul_Pin BLeste_Pin BNorte_Pin */
  GPIO_InitStruct.Pin = BOeste_Pin|BSul_Pin|BLeste_Pin|BNorte_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/* USER CODE BEGIN 4 */
int __io_putchar(int ch) {
    // Transmite 1 caractere por vez via UART
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

int __io_getchar(void) {
    uint8_t ch = 0;
    // Recebe 1 caractere por vez do teclado do PC
    HAL_UART_Receive(&huart2, &ch, 1, HAL_MAX_DELAY);
    return ch;
}
/* USER CODE BEGIN 4 */

void GerarExibirSenha(void) {
  char buffer[20];
  for(int i = 0; i < 3; i++) senha[i] = rand() % 10;

  ST7735_FillScreen(0xFFFF);
  //sprintf(buffer, "SENHA: %d%d%d", senha[0], senha[1], senha[2]);
  //ST7735_WriteString(10, 50, buffer, Font_11x18, 0x07FF, 0x0000);
  //HAL_Delay(4000);

  sprintf((char*)senha_inteira, "%d%d%d", senha[0], senha[1], senha[2]);

  ST7735_FillScreen(0xFFFF);
}

void ProcessarEntradaSenha(void) {
  char buffer[20];
  if (BEsquerda == 0) { entrada[etapa]++; if (entrada[etapa] > 9) entrada[etapa] = 0; HAL_Delay(200); }
  if (BDireita == 0)  { entrada[etapa]--; if (entrada[etapa] < 0) entrada[etapa] = 9; HAL_Delay(200); }

  sprintf(buffer, "%d Digito:", etapa + 1);
        ST7735_WriteString(14, 20, buffer, Font_11x18, 0x0000, 0xFFFF);
        sprintf(buffer, "%d", entrada[etapa]);
        ST7735_WriteString(58, 55, buffer, Font_11x18, 0x0000, 0xFFFF);

  if (BCima == 0) {
    etapa++;
    ST7735_FillScreen(0xFFFF);
    HAL_Delay(300);
  }
}

void ValidarAcesso(void) {
  if (entrada[0] == senha[0] && entrada[1] == senha[1] && entrada[2] == senha[2]) {
    ST7735_FillScreen(0xFFFF);
    ST7735_WriteString(5, 50, "ACESSO OK", Font_11x18, 0x0000, 0xFFFF);
    HAL_Delay(2000);
    ST7735_FillScreen(0xFFFF);
    etapa = 4;
  } else {
    tentativas++;
    ST7735_FillScreen(0xFFFF);
    if (tentativas >= 3) {
      ST7735_WriteString(5, 40, "BLOQUEADO", Font_11x18, 0x0000, 0xFFFF);
      while(1); // Trava o sistema por segurança
    } else {
      ST7735_WriteString(5, 40, "ERRO SENHA", Font_7x10, 0x0000, 0xFFFF);
      HAL_Delay(1500);
      etapa = 0; // Reinicia tentativa
      entrada[0] = entrada[1] = entrada[2] = 0;
      ST7735_FillScreen(0xFFFF);
    }
  }
}

void ConfigurarLimiteAlunos(void) {
    bool sis = true;
    char buffer_local[20];

    ST7735_FillScreen(0xFFFF);
        ST7735_WriteString(5, 5, "MENU CONFIG:", Font_7x10, 0x0000, 0xFFFF);
        ST7735_WriteString(5, 25, "Esq: +1 | Dir: +5", Font_7x10, 0x0000, 0xFFFF);
        ST7735_WriteString(5, 40, "Cima: Salvar", Font_7x10, 0x0000, 0xFFFF);
        ST7735_WriteString(5, 55, "Baixo: Reseta", Font_7x10, 0x0000, 0xFFFF);
        HAL_Delay(3000);
        ST7735_FillScreen(0xFFFF);

    ST7735_FillScreen(0xFFFF); // Limpa para iniciar a config
    ST7735_WriteString(5, 10, "LIMITE ALUNOS:", Font_7x10, 0x0000, 0xFFFF);

    while (sis) {
        // 1. Ler os botões diretamente pelos defines
        if (BEsquerda == 0) { limAlunos++; HAL_Delay(200); }
        if (BDireita == 0)  { limAlunos += 5; HAL_Delay(200); }
        if (BBaixo == 0)    { limAlunos = 0; HAL_Delay(200); }

        // 2. Atualizar o display (Amarelo)
        sprintf(buffer_local, "Total: %d  ", limAlunos); // Espaços extras limpam o rastro do número anterior
        ST7735_WriteString(20, 50, buffer_local, Font_11x18, 0x0000, 0xFFFF);

        // 3. Confirmar e Sair
        if (BCima == 0 && limAlunos > 0) {
            sis = false;
            ST7735_FillScreen(0xFFFF);
            ST7735_WriteString(10, 50, "CONFIGURADO!", Font_11x18, 0x0000, 0xFFFF);
            HAL_Delay(1500);
            ST7735_FillScreen(0xFFFF);
        }
    }
}

void ExecutarControleAula(void) {
  char buffer[30];

      DesenharBarraTopo(presentes2, limAlunos);

        sprintf(buffer, "Pres: %d/%d ", presentes2, limAlunos);
           ST7735_WriteString(5, 22, buffer, Font_7x10, 0x0000, 0xFFFF);
           sprintf(buffer, "Fora: %d/3 ", foraSala);
           ST7735_WriteString(5, 35, buffer, Font_7x10, (foraSala >= 3 ? 0xF800 : 0x0000), 0xFFFF);

           // Comandos curtos
           ST7735_WriteString(5, 48, "D: Voltar | E: Saida", Font_7x10, 0x0000, 0xFFFF);
           ST7735_WriteString(5, 61, "C: Entrar | C+B: Fim", Font_7x10, 0x0000, 0xFFFF);



  // ENTRADA (CIMA)
           if (BCima == 0 && BBaixo == 1) {
               if (presentes < limAlunos) {
               ST7735_FillScreen(0xFFFF);
                   ST7735_WriteString(5, 40, "MATRICULA NO PC...    ", Font_7x10, 0x0000, 0xFFFF);
                   HAL_Delay(500);

                   //digitar matricula

                   campo[presentes] = matricula_do_aluno;
                   ST7735_FillScreen(0xFFFF);
                   sprintf(buffer, "%do ALUNO", presentes + 1);
                   ST7735_WriteString(10, 40, buffer, Font_11x18, 0x0000, 0xFFFF);
                   sprintf(buffer, "Mat: %d", campo[presentes]);
                   ST7735_WriteString(10, 65, buffer, Font_7x10, 0x0000, 0xFFFF);
                   presentes++;
                   presentes2++;
                   totalEntradas++;
               } else {
               ST7735_FillScreen(0xFFFF);
               ST7735_WriteString(10, 50, "SALA CHEIA!", Font_11x18, 0x0000, 0xFFFF);
               }
               HAL_Delay(2000);
               ST7735_FillScreen(0xFFFF);
               matricula_do_aluno = 0;

           }

           // SAÍDA (ESQUERDA)
           if (BEsquerda == 0) {
               if (foraSala < 3 && (presentes - foraSala) > 0) {
                   foraSala++;
                   totalSaidas++;
                   presentes2--;
                   ST7735_FillScreen(0xFFFF);
                 ST7735_WriteString(10, 50, "SAIDA OK!", Font_11x18, 0x0000, 0xFFFF);
               }
               else {
               ST7735_FillScreen(0xFFFF);
               ST7735_WriteString(5, 40, "NAO EH POSSIVEL SAIR!", Font_7x10, 0x0000, 0xFFFF);
              L1; L2; L3; L4;
              L1; L2; L3; L4;
               }
               HAL_Delay(1000);
               ST7735_FillScreen(0xFFFF);
           }

           // RETORNO (DIREITA)
           if (BDireita == 0) {
               if (foraSala > 0) {
               foraSala--;
               presentes2++;
               ST7735_FillScreen(0xFFFF);
               ST7735_WriteString(10, 50, "RETORNO OK!", Font_11x18, 0x0000, 0xFFFF);
               }
               else {
               ST7735_FillScreen(0xFFFF);
               ST7735_WriteString(10, 50, "NAO HA NINGUEM PRA ENTRAR!", Font_7x10, 0x0000, 0xFFFF);
               L1; L2; L3; L4;
               L1; L2; L3; L4;
               }
               ST7735_FillScreen(0xFFFF);
               HAL_Delay(1000);

           }

           // FINALIZAR (CIMA + BAIXO)
           if (BCima == 0 && BBaixo == 0) {
           tempoFim = HAL_GetTick();
           duracaoTotalM = (tempoFim - tempoInicio) / 60000;
               etapa = 6;
               ST7735_FillScreen(0xFFFF);
           }


}

void EncerrarAula(void) {
  char buffer[30];
  ST7735_WriteString(5, 10, "FIM DA AULA", Font_7x10, 0x0000, 0xFFFF);
  sprintf(buffer, "Entradas: %d", totalEntradas);
  ST7735_WriteString(5, 28, buffer, Font_7x10, 0x0000, 0xFFFF);
  sprintf(buffer, "Saidas: %d", totalSaidas);
  ST7735_WriteString(5, 42, buffer, Font_7x10, 0x0000, 0xFFFF);
  sprintf(buffer, "Tempo da aula: %lu min", duracaoTotalM);
  ST7735_WriteString(5, 60, buffer, Font_7x10, 0x0000, 0xFFFF);



  L1; L2; L3; L4;
  L1; L2; L3; L4;
  L1; L2; L3; L4;
  L1; L2; L3; L4;
  L1; L2; L3; L4;
  L1; L2; L3; L4;
  L1; L2; L3; L4;
  L1; L2; L3; L4;
  L1; L2; L3; L4;
  L1; L2; L3; L4;
  while(1);
}
void DesenharBarraTopo(int atual, int maximo) {
uint16_t x = 5, y = 5, larguraTotal = 118, altura = 12;

   if (maximo <= 0) maximo = 1;
   if (atual > maximo) atual = maximo;

   uint16_t areaInterna = larguraTotal - 2;
   uint16_t larguraPreenchida = (atual * areaInterna) / maximo;
   uint16_t larguraVazia = areaInterna - larguraPreenchida;

   uint16_t cor = (atual >= maximo) ? 0xF800 : 0x07E0;

   ST7735_FillRectangle(x, y, larguraTotal, 1, 0x0000);
   ST7735_FillRectangle(x, y + altura - 1, larguraTotal, 1, 0x0000);
   ST7735_FillRectangle(x, y, 1, altura, 0x0000);
   ST7735_FillRectangle(x + larguraTotal - 1, y, 1, altura, 0x0000);

   if (larguraPreenchida > 0)
       ST7735_FillRectangle(x + 1, y + 1, larguraPreenchida, altura - 2, cor);

   if (larguraVazia > 0)
       ST7735_FillRectangle(x + 1 + larguraPreenchida, y + 1, larguraVazia, altura - 2, 0xFFFF);
}
/* USER CODE END 4 */
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  * where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
