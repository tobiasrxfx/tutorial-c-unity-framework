# Testes Unitários em C com Unity Framework (Ambiente Docker)

Este projeto demonstra a implementação de testes unitários em **código C** utilizando o framework **Unity**. O foco é a validação de lógica para software embarcado (setor automotivo) utilizando uma abordagem **Host-Based (SIL - Software-In-the-Loop)**.

---

## 1. O Framework Unity

O Unity é um framework de testes unitários projetado especificamente para a linguagem C. Ele é amplamente utilizado em sistemas embarcados por ser leve e compatível com diversos compiladores (GCC, Clang, IAR, etc.).

A filosofia do Unity é baseada em três princípios:
1.  **Simplicidade:** Todo o framework reside em apenas três arquivos de código.
2.  **Portabilidade:** Não depende de bibliotecas complexas do sistema operacional.
3.  **Expressividade:** Possui asserções específicas para tipos de dados comuns em embarcados (inteiros de tamanho fixo, hexadecimais, bits, arrays).

Mais informações sobre o framework em: https://www.throwtheswitch.org/unity.

---

## 2. Instalação

Diferente de frameworks para outras linguagens, o Unity não requer instalação via gerenciador de pacotes. A "instalação" consiste em incluir os arquivos fonte no seu projeto.

### Passo a Passo

1.  Baixe o código fonte oficial:
    ```bash
    git clone https://github.com/ThrowTheSwitch/Unity.git
    ```
2.  Localize a pasta `src` dentro do repositório clonado. Ela contém três arquivos essenciais:
    * `unity.c`
    * `unity.h`
    * `unity_internals.h`

3.  Copie estes três arquivos para uma pasta dedicada no seu projeto (ex: uma pasta chamada `unity/`). A partir deste momento, basta incluir os cabeçalhos no seu código para utilizar o framework.

---

## 3. Funcionalidades e Configurações

Para utilizar o Unity, basta incluir o cabeçalho `#include "unity.h"` no seu arquivo de teste. Abaixo estão as principais funcionalidades.

### Asserções Comuns
O Unity valida os testes através de macros de asserção. Se a condição falhar, o teste é interrompido e o erro é reportado.

* `TEST_ASSERT_EQUAL_INT(a, b)`: Verifica se dois inteiros são iguais.
* `TEST_ASSERT_FLOAT_WITHIN(delta, esperado, atual)`: Verifica se um float está próximo do valor esperado.
* `TEST_ASSERT_TRUE(condicao)`: Verifica se uma condição booleana é verdadeira.
* `TEST_ASSERT_GREATER_THAN(limiar, valor)`: Verifica se o valor é maior que o limiar.

### Estrutura do Teste e Ciclo de Vida
Para o funcionamento correto, o arquivo de teste deve implementar funções de preparação e limpeza, além de estruturar a função `main` com as macros de controle do framework.

1.  **`setUp(void)`**: Executado automaticamente *antes* de cada teste (ideal para inicializar variáveis).
2.  **`tearDown(void)`**: Executado automaticamente *depois* de cada teste (ideal para limpeza de memória).
3.  **`UNITY_BEGIN()`**: Inicializa o ambiente de testes e zera os contadores de sucesso/falha. Deve ser a primeira chamada na `main`.
4.  **`RUN_TEST(func)`**: Executa uma função de teste específica, garantindo que `setUp` e `tearDown` sejam chamados ao redor dela.
5.  **`UNITY_END()`**: Finaliza a execução, imprime o relatório sumarizado no terminal e retorna o código de status (0 se todos passaram).

#### Exemplo da Função `main`
A função principal deve seguir rigorosamente esta estrutura para garantir a geração correta do relatório e integração com o Makefile:

```c
int main(void) {
    UNITY_BEGIN(); // Inicialização

    // Lista de testes a serem executados
    RUN_TEST(test_Battery_ConvertVoltage_Should_CalculateCorrectly);
    RUN_TEST(test_Battery_CheckHealth_Should_ReturnCritical_When_UnderThreshold);

    return UNITY_END(); // Finalização e retorno do status para o SO
}
```

### Flags de Configuração
O comportamento do Unity pode ser alterado via definições de pré-processador (`#define`) ou flags de compilação `-D`.

* **`UNITY_OUTPUT_COLOR`**: Habilita saída colorida no terminal (Verde para sucesso, Vermelho para falha).
* **`UNITY_INT_WIDTH`**: Define o tamanho do inteiro padrão (16, 32 ou 64 bits), caso o compilador não detecte automaticamente.

---

## 4. Estudo de Caso: Sensor de Bateria

Neste tutorial, é simulado um módulo de leitura de bateria automotiva.

### DUT 
Arquivo: `src/sensor_battery.c`
O código converte um valor bruto de ADC (Analog-to-Digital Converter) para voltagem e determina o estado de saúde da bateria ("Crítico", "Baixo" ou "OK").

### Teste
Arquivo: `test/test_sensor_battery.c`
Os testes validam a lógica de negócio isolada do hardware.

Exemplo de validação de voltagem com tolerância:
```c
void test_Battery_ConvertVoltage_MidScale(void) {
    // ADC 2048 em um sistema de 3.3V deve resultar em ~1.65V
    float resultado = Battery_ConvertVoltage(2048);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.65f, resultado);
}
```

## 5. Ambiente de Execução (Docker e Make)

Para garantir que os testes rodem de forma idêntica na máquina de qualquer desenvolvedor ou servidor de CI, foram utilizados Docker e Makefile. Os arquivos correspondentes estão no diretório deste projeto. 

### Makefile
Automatiza o processo de compilação.
* Compila o código fonte (`src`), o framework (`unity`) e o teste (`test`) em um único binário.
* Aplica a flag `-D UNITY_OUTPUT_COLOR` para facilitar a leitura.
* Garante os includes corretos (`-I`).

### Docker
Isola as ferramentas de compilação. Neste caso, foi utilizado uma imagem baseada em **Alpine Linux**, extremamente leve, contendo apenas o essencial (`gcc` e `make`).

### Como rodar os testes

**1. Construir a Imagem:**
Este passo prepara o ambiente com todas as dependências.
```bash
docker build -t unity-test-env .
```
**2. Executar os Testes:** Este comando sobe um container temporário, compila o código, executa os testes e exibe o relatório no terminal.
```bash
docker run --rm unity-test-env
```

### Relatório de Saída
Ao final da execução, o Unity apresentará um sumário:

```text
test/test_sensor_battery.c:20:test_Battery_ConvertVoltage...:PASS
test/test_sensor_battery.c:28:test_Battery_CheckHealth...:PASS

-----------------------
2 Tests 0 Failures 0 Ignored
OK
```

## 6. Conclusão

A utilização do framework Unity permite validar a lógica de software embarcado de forma eficiente, sem a dependência constante do hardware físico. Esta abordagem, conhecida como Host-Based Testing, acelera o ciclo de desenvolvimento e facilita a detecção precoce de falhas.

Ao combinar o Unity com ferramentas de automação e isolamento como Make e Docker, cria-se um ambiente de testes robusto, portátil e imune a diferenças de configuração entre máquinas. Essa estrutura serve como base sólida para práticas profissionais de qualidade de software, permitindo integração fácil com sistemas de Integração Contínua (CI).
