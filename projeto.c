// Declaração das bibliotecas utilizadas
#include <stdio.h> 
#include "pico/stdlib.h" 
#include "hardware/pio.h"
#include "ws2818b.pio.h"   // Utilizada para configuração da matriz de LEDs
#include "hardware/timer.h"

// Definição dos pinos dos componentes
#define BOTAO_A 5      // Pino do botão A
#define BOTAO_B 6      // Pino do botão B
#define azul 11        // Pino do LED azul
#define verde 12       // Pino do LED verde
#define vermelho 13    // Pino do LED vermelho

int valor_atual = 0; // Variável que armazena o número exibido na matriz de LEDs.
bool led_vermelho = false; // Variável de controle para o estado do LED vermelho.
static volatile uint32_t last_time = 0; // Variável para armazenar o tempo do último evento de interrupção.

/* Função para indexar as posições dos LEDs da matriz */
int getIndex(int x, int y) {
    if (x % 2 == 0) {
        return 24 - (x * 5 + y); // Mapeamento para linhas pares
    } else {
        return 24 - (x * 5 + (4 - y)); // Mapeamento para linhas ímpares
    }
}

// Inicializar os pinos do LED RGB
void iniciar_rgb() {
    // Inicializa os pinos do LED RGB
    gpio_init(vermelho);
    gpio_init(verde);
    gpio_init(azul);
    // Configura os pinos como saída
    gpio_set_dir(vermelho, GPIO_OUT);
    gpio_set_dir(verde, GPIO_OUT);
    gpio_set_dir(azul, GPIO_OUT);
}

// Função para acionar o LED vermelho e configurar o tempo ligado
void controlar_led(bool r, int tempo) {
    iniciar_rgb(); // Chamando a função iniciar_RGB
    gpio_put(vermelho, r);
    sleep_ms(tempo);
}

// CONFIGURANDO A MATRIZ DE LEDS com a biblioteca ws2818b
// Definição da estrutura de cor para cada LED da matriz
struct pixel_t {
    uint8_t R, G, B; // Componentes de cor (Red, Green, Blue)
};
typedef struct pixel_t npLED_t;
npLED_t leds[25]; // Array para armazenar o estado de cada LED da matriz

// PIO e state machine para controle dos LEDs 
PIO np_pio; // Instância do PIO
uint sm;    // State machine

// Função para atualizar os LEDs da matriz
void atualizar_matriz() {
    for (uint i = 0; i < 25; i++) {
        pio_sm_put_blocking(np_pio, sm, leds[i].R); // Envia o valor de R para o LED
        pio_sm_put_blocking(np_pio, sm, leds[i].G); // Envia o valor de G para o LED
        pio_sm_put_blocking(np_pio, sm, leds[i].B); // Envia o valor de B para o LED
    }
}

// Função de inicializar a matriz de LEDs com a ws2818b
void iniciar_matriz(uint pino) {
    uint offset = pio_add_program(pio0, &ws2818b_program); // Adiciona o programa PIO
    np_pio = pio0; // Define o PIO a ser utilizado
    sm = pio_claim_unused_sm(np_pio, true); // Configura a state machine
    ws2818b_program_init(np_pio, sm, offset, pino, 800000.f); // Inicializa a matriz de LEDs

    // Inicializa todos os LEDs como desligados
    for (uint i = 0; i < 25; i++) {
        leds[i].R = leds[i].G = leds[i].B = 0;
    }
    atualizar_matriz(); // Atualiza a matriz
}

// Função para configurar a cor de um LED específico
void cor(const uint indice, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[indice].R = r; // Define o valor de R
    leds[indice].G = g; // Define o valor de G
    leds[indice].B = b; // Define o valor de B
}

// Funções para desenhar os números na matriz de LEDs
void numero0() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}}
    };
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    atualizar_matriz();
}

void numero1() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}}
    };

    // Exibir a primeira matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    atualizar_matriz();
}

void numero2() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}}
    };

    // Exibir a primeira matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    atualizar_matriz();
}

void numero3() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0  }},
        {{0, 0, 0}, {0, 0, 0},     {0, 0,     0}, {0, 0, 255}, {0, 0,   0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0,   0}},
        {{0, 0, 0}, {0, 0, 0},     {0, 0,     0}, {0, 0, 255}, {0, 0,   0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0,   0}}
    };

    // Exibir a primeira matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    atualizar_matriz();
}

void numero4() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}}
    };

    // Exibir a primeira matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    atualizar_matriz();
}

void numero5() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}}
    
    };

    // Exibir a matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    atualizar_matriz();
}

void numero6() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}}
    };

    // Exibir a matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    atualizar_matriz();
}

void numero7() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}}
    };

    // Exibir a matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    atualizar_matriz();
}

void numero8() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}}
    };

    // Exibir a matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    atualizar_matriz();
}

void numero9() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}}

    };
}
// Switch para atualizar o número exibido na matriz
void atualizar_valor() {
    switch (valor_atual) {
        case 0: numero0(); break;
        case 1: numero1(); break;
        case 2: numero2(); break;
        case 3: numero3(); break;
        case 4: numero4(); break;
        case 5: numero5(); break;
        case 6: numero6(); break;
        case 7: numero7(); break;
        case 8: numero8(); break;
        case 9: numero9(); break;
    }
}

// Função para inicializar os pinos dos botões como entrada IN
void iniciar_botoes() {
    gpio_init(BOTAO_A); // Inicializa o pino do botão A
    gpio_init(BOTAO_B); // Inicializa o pino do botão B
    gpio_set_dir(BOTAO_A, GPIO_IN); // Configura o pino como entrada
    gpio_set_dir(BOTAO_B, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(BOTAO_A); // Habilita o resistor de pull-up
    gpio_pull_up(BOTAO_B); // Habilita o resistor de pull-up
}

// Função de tratamento de interrupção para os botões
void gpio_irq_handler(uint gpio, uint32_t events) {
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    // Verifica se passou tempo suficiente desde o último evento (debouncing)
    if (current_time - last_time > 250000) { // 250 ms de debouncing
        last_time = current_time; // Atualiza o tempo do último evento
        if (gpio == BOTAO_A) {
            valor_atual = (valor_atual + 1) % 10; // Incrementa o valor
            printf("bta %u\n", last_time); 
        } else if (gpio == BOTAO_B) {
            valor_atual = (valor_atual - 1 + 10) % 10; // Decrementa o valor
            printf("btb %u\n", last_time);
        }
        atualizar_valor(); // Atualiza o número exibido na matriz
    }
}

// Função principal
int main() {
    stdio_init_all(); // Inicializa a comunicação serial
    iniciar_matriz(7); // Inicializa a matriz de LEDs no pino 7
    iniciar_botoes(); // Inicializa os botões
    iniciar_rgb(); // Inicializa o LED RGB
    printf("0\n");
    numero0(); // Define o estado inicial da matriz de LEDs como o número 0

    // Habilita as interrupções para os botões
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (1) {
        controlar_led(1, 100); // O LED vermelho fica 100 ms ligado
        controlar_led(0, 100); // Todos os LEDs ficam 100 ms desligados
    }
    return 0; 
}
