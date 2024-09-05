# Projeto de Monitoramento de Luminosidade com MQTT e ESP32

Este projeto utiliza um ESP32 para monitorar a luminosidade ambiente e controlar LEDs com base nos níveis de luminosidade. O ESP32 se conecta a uma rede Wi-Fi e publica dados em um broker MQTT, além de receber comandos via MQTT para controlar o estado do LED.

## Funcionalidades

- **Conexão Wi-Fi:** O ESP32 se conecta a uma rede Wi-Fi configurada.
- **Conexão MQTT:** O ESP32 se conecta a um broker MQTT para enviar e receber mensagens.
- **Monitoramento de Luminosidade:** O valor da luminosidade é lido de um LDR (resistor dependente de luz) e os LEDs são acionados com base nos níveis de luminosidade.
- **Controle de LEDs:** LEDs verde, amarelo e vermelho indicam diferentes níveis de luminosidade:
  - Verde: Ambiente com luminosidade OK
  - Amarelo: Nível de alerta
  - Vermelho: Problema de luminosidade

## Componentes

- **ESP32:** Placa de microcontrolador com Wi-Fi e Bluetooth.
- **LDR (Resistor Dependente de Luz):** Sensor para medir a luminosidade.
- **LEDs (Verde, Amarelo, Vermelho):** LEDs para indicar o estado da luminosidade.
- **Resistor:** Para limitar a corrente no LDR e LEDs.

## Configuração

### Configurações de Rede e MQTT

1. **SSID e Senha do Wi-Fi:**
   Atualize `default_SSID` e `default_PASSWORD` com o nome e senha da sua rede Wi-Fi.

2. **Broker MQTT:**
   Atualize `default_BROKER_MQTT` com o IP do seu broker MQTT e `default_BROKER_PORT` com a porta do broker.

3. **Tópicos MQTT:**
   - `default_TOPICO_SUBSCRIBE`: Tópico para receber comandos.
   - `default_TOPICO_PUBLISH_1`: Tópico para publicar o estado do LED.
   - `default_TOPICO_PUBLISH_2`: Tópico para publicar o valor da luminosidade.

4. **ID MQTT:**
   Atualize `default_ID_MQTT` com o ID desejado para a conexão MQTT.

### Conexão e Instalação

1. Conecte o ESP32 ao computador via USB.
2. Abra o código no [Arduino IDE](https://www.arduino.cc/en/software).
3. Selecione a placa ESP32 apropriada e a porta correta nas configurações do Arduino IDE.
4. Instale as bibliotecas necessárias: `WiFi` e `PubSubClient`.
5. Carregue o código para o ESP32.

## Uso

1. Após o ESP32 reiniciar, ele tentará se conectar à rede Wi-Fi configurada.
2. Se a conexão com o Wi-Fi for bem-sucedida, o ESP32 se conectará ao broker MQTT.
3. O ESP32 começará a ler o valor da luminosidade do LDR e acenderá os LEDs com base nos níveis de luminosidade:
   - Verde para luminosidade OK
   - Amarelo para nível de alerta
   - Vermelho para problemas de luminosidade
4. O estado atual do LED será publicado no tópico `TOPICO_PUBLISH_1` e o valor da luminosidade será publicado no tópico `TOPICO_PUBLISH_2`.
5. Comandos para ligar ou desligar o LED podem ser enviados para o tópico `TOPICO_SUBSCRIBE`.

## Funções

- **`initSerial()`**: Inicializa a comunicação serial.
- **`initWiFi()`**: Conecta à rede Wi-Fi.
- **`initMQTT()`**: Configura a conexão MQTT.
- **`setup()`**: Configuração inicial do hardware e conexão.
- **`loop()`**: Função principal que executa repetidamente para verificar conexões e processar dados.
- **`reconectWiFi()`**: Reconnecta à rede Wi-Fi.
- **`mqtt_callback()`**: Processa mensagens recebidas via MQTT.
- **`VerificaConexoesWiFIEMQTT()`**: Verifica e reconecta Wi-Fi e MQTT se necessário.
- **`EnviaEstadoOutputMQTT()`**: Envia o estado do LED ao broker MQTT.
- **`reconnectMQTT()`**: Reconecta ao broker MQTT.
- **`handleLuminosity()`**: Lê e processa o valor da luminosidade, acendendo os LEDs apropriados e publicando o valor.
- **`apagaLeds()`**: Desliga todos os LEDs.

## Integrantes

Fernando Carlos Colque Huaranca - rm558095

Heloísa Fleury Jardim - rm556378

Juan Fuentes Rufino - rm557673

Julia Carolina Ferreira Silva - rm558896

Pedro Batista - rm558137