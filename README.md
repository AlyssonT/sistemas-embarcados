# Instalação ESP8266 (Windows)

## Instalação:
* Copiar o link:  ```https://arduino.esp8266.com/stable/package_esp8266com_index.json``` e colar em File->Preferences->Additional Boards...

    ![image](https://user-images.githubusercontent.com/73255963/193599159-22d385dd-ee4f-464b-a903-d25f81974bdb.png)

    * Se um erro "Invalid sketchbook..." aparecer, basta trocar o pasta do Sketchbook para uma de sua preferência por meio do botão **Browse**.

* Vá na aba de ```Tools```, depois ```Boards``` e selecione a opção ```Boards Manager```.

    ![image](https://user-images.githubusercontent.com/73255963/193599617-4482a30d-e235-4591-8026-9e54bb982fba.png)

* Procure por ``` ES8266 ``` e instale o pacote mais recente.

* Novamente no menu ```Tools```, na aba ```Port```, selecione a porta em que seu dispositivo está conectado.

* Caso a porta não esteja listada, procure instalar os drivers da conexão USB de seu dispositivo, nesse dispositivo ele usa um chip de conexão ```CH340```.

* Após a instalação desse driver, reinicie a IDE e a porta que seu dispositivo está conectado deve aparecer na aba ```Port:```.

    ![image](https://user-images.githubusercontent.com/73255963/193601206-c2884629-e548-4bdd-97a6-339dafd331c8.png)


* Instalação concluída!

<hr>

## Testando a placa.
* Exemplo de código: **Blink** . Ele pode ser obtido em: File->Examples->ESP8266->Blink

* Altere o parâmetro do método ```Serial.begin()``` para ```9600``` (baud rate da conexão serial).

* Tente compilar o código.

    * Caso surja algum erro relacionado ao Python serial, instale a biblioteca ```pyserial``` e compile novamente. Ela poder ser instalada utilizando pip:

    ```pip install pyserial``` (No linux) ou 
    ```py -m pip install pyserial``` (No windows)
    
* Agora é só subir o código para a placa.
* Se tudo ocorrer bem, seu dispositivo deve acender o led por 1s a cada 2s.


<hr>

## Se conectando ao Wi-Fi.

Exemplo simples para se conectar a sua primeira rede Wi-Fi.

* Abra o exemplo de código **SimpleTime**. Ele pode ser obtido em: File->Examples->ESP32->Time->SimpleTime;
![image](https://user-images.githubusercontent.com/73255963/193603296-a3d053c0-2a73-4ea1-98df-0335edd493f5.png)


* Assim como no exemplo passado, altere o parâmetro do método ```Serial.begin()``` para ```9600```;

* Altere os valores definidos para ```STASSID``` para o nome da rede em que deseja conectar e ```STAPSK``` para a senha da rede.

![image](https://user-images.githubusercontent.com/73255963/193604245-e3b1e756-3bb9-45b8-a94a-7d25cc2269d8.png)

* Nesse exemplo a placa irá se conectar em um servidor que responde a requisição com uma "frase do dia", e irá imprimí-la no serial.

![image](https://user-images.githubusercontent.com/73255963/193605690-05aa49f2-fc34-4f53-92f9-5a3447c95c18.png)

 
