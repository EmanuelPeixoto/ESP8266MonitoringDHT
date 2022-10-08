#include<ESP8266WiFi.h> //Biblioteca para funcionamento do WiFi do ESP
#include<ESP8266WebServer.h> //Biblioteca para o ESP funcionar como servidor
#include <DHT.h> //Biblioteca para funcionamento do sensor de temperatura e umidade DHT11

const char* ssid = "XXXXX";  // Rede WiFi
const char* password = "XXXXX";  //Senha da Rede WiFi

ESP8266WebServer server(80); //server na porta 80

#define DHTPIN 4 //Pino digital D2 (GPIO4) conectado ao DHT11
#define DHTTYPE DHT11 //Tipo do sensor DHT11
#define LED 2 // LED imbutido

DHT dht(DHTPIN, DHTTYPE); //Inicializando o objeto dht do tipo DHT passando como parâmetro o pino (DHTPIN) e o tipo do sensor (DHTTYPE)

float temperatura; //variável para armazenar a temperatura
float umidade; //Variável para armazenar a umidade

void setup() {
  Serial.begin(115200); //Inicializa a comunicação serial
  pinMode(LED, OUTPUT); // define o LED como output
  delay(100); // ?Intervalo para aguardar a estabilização do sistema
  dht.begin(); //Inicializa o sensor DHT11

  Serial.println("Conectando a Rede: "); //Imprime na serial a mensagem
  Serial.println(ssid); //Imprime na serial o nome da Rede Wi-Fi

  WiFi.begin(ssid, password); //Inicialização da comunicação Wi-Fi

  //Verificação da conexão
  while (WiFi.status() != WL_CONNECTED) { //Enquanto estiver aguardando status da conexão
    delay(1000);
    Serial.print("."); //Imprime pontos
    digitalWrite(LED, 0);
  }
  Serial.println("");
  Serial.println("WiFi Conectado");
  digitalWrite(LED, 1);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); //Função para exibir o IP da ESP

  server.on("/", handle_OnConnect); //Servidor recebe uma solicitação HTTP - chama a função handle_OnConnect
  server.onNotFound(handle_NotFound); //Servidor recebe uma solicitação HTTP não especificada - chama a função handle_NotFound

  server.begin(); //Inicializa o servidor
  Serial.println("Servidor HTTP inicializado");
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){digitalWrite(LED, 0);}
  else{digitalWrite(LED, 1);}
  server.handleClient(); //Chama o método handleClient()
}

void handle_OnConnect() {
  temperatura = dht.readTemperature();  //Realiza a leitura da temperatura
  umidade = dht.readHumidity(); //Realiza a leitura da umidade
  Serial.print("Temperatura: ");
  Serial.print(temperatura); //Imprime no monitor serial o valor da temperatura lida
  Serial.println("ºC");
  Serial.print("Umidade: ");
  Serial.print(umidade); //Imprime no monitor serial o valor da umidade lida
  Serial.println("%");
  server.send(200, "text/html", EnvioHTML(temperatura, umidade)); //Envia as informações usando o código 200, especifica o conteúdo como "text/html" e chama a função EnvioHTML

}

void handle_NotFound() { //Função para lidar com o erro 404
  server.send(404, "text/plain", "Não encontrado"); //Envia o código 404, especifica o conteúdo como "text/pain" e envia a mensagem "Não encontrado"

}

String EnvioHTML(float Temperaturastat, float Umidadestat) { //Exibindo a página da web em HTML
  String ptr = "<!DOCTYPE HTML>\n<HTML>\n"; //Indica o envio do código HTML
  ptr += "<HEAD>\n<META charset=\"UTF-8\" name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"; //Torna a página da Web responsiva em qualquer navegador Web
  ptr += "<META http-equiv='refresh' content='2'>\n";//Atualizar browser a cada 2 segundos
  ptr += "<TITLE>Monitor de Temperatura e Umidade</TITLE>\n"; //Define o título da página

  //Configurações de fonte do título e do corpo do texto da página web
  ptr += "<STYLE>\nhtml {display: block; margin: 0px auto; text-align: center;color: #000000;}\n";
  ptr += "body{margin-top: 50px;}\n";
  ptr += "h1 {margin: 50px auto 30px; margin-bottom: 75px;}\n";
  ptr += "p {font-size: 25px;color: #000000; margin-bottom: 10px;}\n";
  ptr += ".footer {position: fixed; left: 0; bottom: 0; width: 100%;}\n";
  ptr += "</STYLE>\n";
  ptr += "</HEAD>\n";
  ptr += "<BODY>\n";
  ptr += "<DIV id=\"webpage\">\n";
  ptr += "<H1>Monitor de Temperatura e Umidade</H1>\n";

  //Exibe as informações de temperatura, umidade e créditos na página web
  ptr += "<P><B>Temperatura: </B>";
  ptr += (int)Temperaturastat;
  ptr += "ºC</P>";
  ptr += "<P><B>Umidade: </B>";
  ptr += (int)Umidadestat;
  ptr += "%</P>";

  ptr += "</DIV>\n";
  ptr += "</BODY>\n";
  ptr += "<FOOTER class=\"footer\">\n";
  ptr += "<FONT size=\"5px\">Feito por Emanuel Peixoto</FONT>\n";
  ptr += "</FOOTER>\n";
  ptr += "</HTML>";
  return ptr;

}
