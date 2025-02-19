# Joystick-e-ADC
Atividade 1 - Aula Síncrona 10/02 - EmbarcaTech

Descrição do Projeto:

O joystick fornecerá valores analógicos correspondentes aos eixos X e Y, que serão utilizados para:
Controlar a intensidade luminosa dos LEDs RGB, onde:

• O LED Azul terá seu brilho ajustado conforme o valor do eixo Y. Quando o joystick estiver solto
(posição central - valor 2048), o LED permanecerá apagado. À medida que o joystick for movido para
cima (valores menores) ou para baixo (valores maiores), o LED aumentará seu brilho gradualmente,
atingindo a intensidade máxima nos extremos (0 e 4095).

• O LED Vermelho seguirá o mesmo princípio, mas de acordo com o eixo X. Quando o joystick estiver
solto (posição central - valor 2048), o LED estará apagado. Movendo o joystick para a esquerda
(valores menores) ou para a direita (valores maiores), o LED aumentará de brilho, sendo mais intenso
nos extremos (0 e 4095).

• Os LEDs serão controlados via PWM para permitir variação suave da intensidade luminosa.

Vídeo de demonstrasção: https://drive.google.com/file/d/1xJFD6Mk500R3l32O3Bxv-HDMGsLPK4H0/view?usp=drivesdk

Adicionalmente, o botão do joystick terá as seguintes funcionalidades:
• Alternar o estado do LED Verde a cada acionamento.

Finalmente, o botão A terá a seguinte funcionalidade:
• Ativar ou desativar os LED PWM a cada acionamento.

