#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "Atividade.pio.h"
#include <string.h>

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO 0x3C

// Definição dos pinos dos LEDs RGB
#define LED_G 11
#define LED_B 12

#define WIDTH 128  // Largura do display
#define HEIGHT 64

// Definição dos botões
#define BUTTON_A 5
#define BUTTON_B 6

#define OUT_PIN 7
#define NUM_PIXELS 25

// Variáveis para armazenar o estado dos LEDs
bool estado_led_verde = false;
bool estado_led_azul = false;

// Inicializa a estrutura do display
ssd1306_t ssd;
PIO pio = pio0;
uint sm;

// Mapas de números 0-9 para matriz 5x5
const double numeros[10][25] = {
    {1,1,1,1,1, 1,0,0,0,1, 1,0,0,0,1, 1,0,0,0,1, 1,1,1,1,1}, // 0
    {0,0,1,0,0, 0,0,1,1,0, 1,0,1,0,0, 0,0,1,0,0, 1,1,1,1,1}, // 1
    {1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,1, 1,1,1,1,1}, // 2
    {1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,1}, // 3
    {1,0,0,0,1, 1,0,0,0,1, 1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,1}, // 4
    {1,1,1,1,1, 0,0,0,0,1, 1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,1}, // 5
    {1,1,1,1,1, 0,0,0,0,1, 1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,1}, // 6
    {1,1,1,1,1, 0,0,0,0,1, 0,0,0,1,0, 0,0,1,0,0, 0,1,0,0,0}, // 7
    {1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,1}, // 8
    {1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,1}  // 9
};

void desenho_pio(const double *desenho) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        uint32_t cor = desenho[24 - i] ? 0x00FF0000 : 0x00000000;
        pio_sm_put_blocking(pio, sm, cor);
    }
}

// Função para atualizar os LEDs
void atualizar_leds() {
    gpio_put(LED_G, estado_led_verde);
    gpio_put(LED_B, estado_led_azul);
}

void exibir_estado() {
    char buffer[50]; // Declara um buffer de tamanho suficiente para armazenar as strings

    printf("LED Verde: %s | LED Azul: %s\n", 
           estado_led_verde ? "Ligado" : "Desligado",
           estado_led_azul ? "Ligado" : "Desligado");

    ssd1306_fill(&ssd, false); // Limpa o display

    if (estado_led_verde) {
        snprintf(buffer, sizeof(buffer), "LED Verde");
        ssd1306_draw_string(&ssd, buffer, 10, 14);
        snprintf(buffer, sizeof(buffer), "Ligado");
        ssd1306_draw_string(&ssd, buffer, 10, 24);
    } else {
        snprintf(buffer, sizeof(buffer), "LED Verde");
        ssd1306_draw_string(&ssd, buffer, 10, 14);
        snprintf(buffer, sizeof(buffer), "Desligado");
        ssd1306_draw_string(&ssd, buffer, 10, 24);
    }

    if (estado_led_azul) {
        snprintf(buffer, sizeof(buffer), "LED Azul");
        ssd1306_draw_string(&ssd, buffer, 10, 44);
        snprintf(buffer, sizeof(buffer), "Ligado");
        ssd1306_draw_string(&ssd, buffer, 10, 54);
    } else {
        snprintf(buffer, sizeof(buffer), "LED Azul");
        ssd1306_draw_string(&ssd, buffer, 10, 44);
        snprintf(buffer, sizeof(buffer), "Desligado");
        ssd1306_draw_string(&ssd, buffer, 10, 54);
    }


    ssd1306_send_data(&ssd); // Atualiza o display
}


// Manipulador da interrupção dos botões
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A) {
        estado_led_verde = !estado_led_verde;
        estado_led_azul = false; // Garante que apenas um LED fique ativo
    } else if (gpio == BUTTON_B) {
        estado_led_azul = !estado_led_azul;
        estado_led_verde = false; // Garante que apenas um LED fique ativo
    }
    atualizar_leds();
    exibir_estado();
}

// Função para exibir número no display
void exibir_numero(char numero) {
    ssd1306_fill(&ssd, false); // Limpa o display
    char str[2] = {numero, '\0'}; // Converte para string
    ssd1306_draw_string(&ssd, str, 50, 25); // Exibe o número na tela
    ssd1306_send_data(&ssd); // Atualiza o display
    desenho_pio(numeros[numero - '0']);
}

void exibir_letra_oled(ssd1306_t *ssd, char letra) {
    char texto[2] = {letra, '\0'}; // Criamos uma string com a letra
    ssd1306_fill(ssd, 0); // Limpa a tela
    ssd1306_draw_string(ssd, texto, 64, 32); // Exibe a letra
    ssd1306_send_data(ssd); // Atualiza o display
}

void apagar_letra_oled(ssd1306_t *ssd, char letra) {
   
    ssd1306_fill(ssd, 0); // Limpa a tela
    
    ssd1306_send_data(ssd); // Atualiza o display
}



void desligar_leds(PIO pio, uint sm) {
    // Apaga todos os LEDs da matriz 5x5
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, 0x00000000);
    }

    // Desliga os LEDs das GPIOs 12 e 13
    gpio_put(LED_G, false);
    gpio_put(LED_B, false);
    
    // Atualiza os estados das variáveis
    estado_led_verde = false;
    estado_led_azul = false;

    // Exibe "Leds Off" na tela OLED
    ssd1306_fill(&ssd, false); // Limpa o display

    // **Solução alternativa: Desenhar caractere por caractere**
    const char *msg = "Leds Desligados";
    int x = 10; // Posição inicial

    for (int i = 0; msg[i] != '\0'; i++) {
        ssd1306_draw_char(&ssd, msg[i], x, 25); // Desenha caractere individualmente
        x += 8; // Avança para a próxima posição (ajuste conforme necessário)
    }

    ssd1306_send_data(&ssd); // Atualiza o display
}

int main() {
    stdio_init_all();

    // Inicialização dos LEDs
    gpio_init(LED_G);
    gpio_init(LED_B);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(LED_B, GPIO_OUT);
    atualizar_leds();

    // Inicialização dos botões
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Configuração da interrupção para os botões
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Inicialização do I2C e do display SSD1306
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    ssd1306_init(&ssd, 128, 64, false, ENDERECO, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    // Inicialização da PIO para matriz de LEDs WS2812
    uint offset = pio_add_program(pio, &Atividade_program);
    sm = pio_claim_unused_sm(pio, true);
    Atividade_program_init(pio, sm, offset, OUT_PIN);

    char buffer[4] = {0}; // Buffer para armazenar entrada serial

    // Loop principal
    while (true) {
        if (stdio_usb_connected()) {
            int c = getchar_timeout_us(100000); // Aguarda entrada serial por 100ms
            
            if (c >= '0' && c <= '9') {
                printf("Número recebido: %c\n", c);
                exibir_numero((char)c);
            } 
            else if (c == '\n' || c == '\r') {
                // Se Enter for pressionado, limpa o display OLED
                printf("Enter pressionado, limpando display...\n");
                ssd1306_fill(&ssd, false);
                ssd1306_send_data(&ssd);
            }
            else if (c != PICO_ERROR_TIMEOUT) {
                // Adiciona o caractere ao buffer para detectar "OFF"
                buffer[0] = buffer[1];
                buffer[1] = buffer[2];
                buffer[2] = (char)c;
                buffer[3] = '\0'; // Garante que seja uma string válida
                
                // Se a entrada for "OFF", desliga todos os LEDs
                if (strcmp(buffer, "OFF") == 0) {
                    printf("Comando recebido: OFF\n");
                    desligar_leds(pio, sm);
                } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) { 
                    // Se for uma letra, exibe no OLED
                    exibir_letra_oled(&ssd, (char)c);
                } else if(strcmp(buffer, "Desligar") == 0) {
                    printf("Comando recebido: Desligar\n");
                    void apagar_letra_oled(ssd1306_t *ssd, char letra);
                }
            }
        }
        sleep_ms(1000);
    }
}

