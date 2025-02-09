# Projeto RP2040 - Controle de LEDs e Display SSD1306

## Descrição

Este projeto utiliza a placa RP2040 (BitDogLab) para controlar LEDs RGB e um display OLED SSD1306 via comunicação I2C. O sistema também interage com botões físicos e recebe comandos via UART para exibição de números e caracteres no display.

## Funcionalidades

- Controle de LEDs RGB (Verde e Azul)
- Exibição de mensagens no display SSD1306
- Controle de LEDs via botões
- Recebimento de comandos via UART (USB)
- Exibição de números e caracteres recebidos
- Matriz de LEDs WS2812 controlada via PIO
- Comando para desligar todos os LEDs

## Hardware Utilizado

- **Placa**: Raspberry Pi Pico W (RP2040)
- **Display**: SSD1306 (I2C)
- **LEDs**: RGB (GPIOs 11 e 12) e matriz WS2812
- **Botões**: GPIOs 5 e 6
- **Comunicação**: UART e I2C

## Conexões

| Componente    | Pino RP2040 |
| ------------- | ----------- |
| SSD1306 SDA   | GPIO 14     |
| SSD1306 SCL   | GPIO 15     |
| LED Verde     | GPIO 11     |
| LED Azul      | GPIO 12     |
| Botão A       | GPIO 5      |
| Botão B       | GPIO 6      |
| Matriz WS2812 | GPIO 7      |

## Dependências

- **Pico SDK**
- **Biblioteca SSD1306** (incluída no diretório `inc/`)
- **PIO para controle dos LEDs WS2812**

## Como Usar

1. **Compilar e carregar o código** na placa RP2040.
2. **Interação com os botões**:
   - Botão A: Liga/Desliga LED Verde
   - Botão B: Liga/Desliga LED Azul
3. **Comandos via UART**:
   - Digitar um número (0-9) exibe o número no OLED
   - Digitar uma letra exibe a letra no OLED
   - Comando `OFF`: Desliga todos os LEDs

## Estrutura do Código

- `main()`: Inicializa o hardware e entra no loop principal.
- `gpio_callback()`: Manipula os botões para controle dos LEDs.
- `exibir_numero()`: Exibe um número no display OLED e na matriz de LEDs.
- `desligar_leds()`: Desliga todos os LEDs e limpa o display.
- `exibir_letra_oled()`: Exibe uma letra recebida no display OLED.

## Autor

Joseph Borges Morais.

Desenvolvido para fins de aprendizado e experimentação com RP2040, I2C, UART e LEDs WS2812.

## Demonstração

Assista ao vídeo do projeto no YouTube: [Clique aqui](https://youtu.be/Npuo75Xt8LA)




