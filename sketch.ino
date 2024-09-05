#include <WiFi.h>
#include <PubSubClient.h>

// Configurações - variáveis editáveis
const char* default_SSID = "FIAP-IBM"; // Nome da rede Wi-Fi
const char* default_PASSWORD = "Challenge@24!"; // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "20.206.249.58"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/lampMindSync/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/lampMindSync/attrs"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_2 = "/TEF/lampMindSync/attrs/l"; // Tópico MQTT de envio de informações para Broker
const char* default_ID_MQTT = "fiware_MindSync"; // ID MQTT

const char* topicPrefix = "lampMindSync";

// Pinos
const int ldrPin = 34;      // Pino do LDR (ADC1_CH6 no ESP32)
const int ledGreen = 26;   // Pino do LED verde
const int ledYellow = 27;  // Pino do LED amarelo
const int ledRed = 18;     // Pino do LED vermelho

// Limites de luminosidade
const int luminosidadeOK = 60;        // Limite inferior para luminosidade OK
const int luminosidadeAlerta = 40;    // Limite inferior para luminosidade em alerta

// Variáveis para configurações editáveis
char* SSID = const_cast<char*>(default_SSID);
char* PASSWORD = const_cast<char*>(default_PASSWORD);
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE);
char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1);
char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2);
char* ID_MQTT = const_cast<char*>(default_ID_MQTT);

WiFiClient espClient;// Cliente Wi-Fi
PubSubClient MQTT(espClient);// Cliente MQTT
char EstadoSaida = '0';// Estado do LED: '0' para desligado, '1' para ligado

// Função para inicializar a comunicação serial
void initSerial() {
    Serial.begin(115200);
}

// Função para conectar-se à rede Wi-Fi
void initWiFi() {
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconectWiFi();
}

// Função para configurar a conexão MQTT
void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT); // Define o servidor MQTT e a porta
    MQTT.setCallback(mqtt_callback); // Define a função de callback para mensagens recebidas
}

// Função de configuração inicial: inicialização dos pins e do ldr
void setup() {
    pinMode(ldrPin, INPUT);
    pinMode(ledGreen, OUTPUT);
    pinMode(ledYellow, OUTPUT);
    pinMode(ledRed, OUTPUT);
    initSerial();
    initWiFi();
    initMQTT();
    delay(5000);
    MQTT.publish(TOPICO_PUBLISH_1, "s|on"); / Publica o estado inicial do LED como 'on'
}

// Função de loop principal
void loop() {
    VerificaConexoesWiFIEMQTT(); // Verifica e reconecta Wi-Fi e MQTT se necessário
    EnviaEstadoOutputMQTT();
    if (EstadoSaida == '1') {
        handleLuminosity();
    } else {
        apagaLeds();
    }
    MQTT.loop();
}

// Função para reconectar-se à rede Wi-Fi
void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED)
        return;
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}

// Função de callback para mensagens MQTT recebidas
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (int i = 0; i < length; i++) {
        char c = (char)payload[i];
        msg += c;
    }
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);

    // Forma o padrão de tópico para comparação
    String onTopic = String(topicPrefix) + "@on|";
    String offTopic = String(topicPrefix) + "@off|";

    // Compara com o tópico recebido
    if (msg.equals(onTopic)) {
        EstadoSaida = '1';
    }

    if (msg.equals(offTopic)) {
        EstadoSaida = '0';
    }
}

void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected())
        reconnectMQTT();
    reconectWiFi();
}

// Função para enviar o estado atual do LED ao broker MQTT
void EnviaEstadoOutputMQTT() {
    if (EstadoSaida == '1') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|on");
        Serial.println("- Led Ligado");
    } else if (EstadoSaida == '0') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|off");
        Serial.println("- Led Desligado");
    }
    Serial.println("- Estado do LED onboard enviado ao broker!");
    delay(1000);
}

// Função para reconectar ao broker MQTT
void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE);
        } else {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000);
        }
    }
}

// Função para lidar com a leitura e processamento da luminosidade
// Controla os LEDs com base na luminosidade
void handleLuminosity() {
    int sensorValue = analogRead(ldrPin);
    int luminosidade = map(sensorValue, 0, 4095, 0, 100); // Mapeia o valor para uma escala de 0 a 100

    Serial.print("Luminosidade: ");
    Serial.println(luminosidade);

    if (luminosidade >= 11) {
        // Ambiente OK
        apagaLeds();
        digitalWrite(ledGreen, HIGH);
    } else if (luminosidade < 11 && luminosidade >= 5) {
        // Nível de alerta
        apagaLeds();
        digitalWrite(ledYellow, HIGH);
    } else {
        // Problema
        apagaLeds();
        digitalWrite(ledRed, HIGH);
    }

    String mensagem = String(luminosidade);
    Serial.print("Valor da luminosidade: ");
    Serial.println(mensagem.c_str());
    MQTT.publish(TOPICO_PUBLISH_2, mensagem.c_str());
}

// Função para apagar todos os LEDs
void apagaLeds() {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
}
