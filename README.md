
# Partida Remota – Hyundai HB20 (2016)

##  Projeto real | Sistema Embarcado | Automotivo

Este projeto é um sistema de **partida remota do motor** para um Hyundai HB20 (2016),
desenvolvido sem módulos comerciais prontos e utilizando o **controle original do veículo**.

Foi **testado em veículo real**, em uso diário, com lógica completa de segurança
e proteção por Watchdog.

---

##  O que este sistema faz

- Dá partida no motor remotamente usando o controle original
- Mantém o comportamento original do carro (não interfere no BCM)
- Verificação de neutro e condições de segurança
- Controle de ignição, motor de arranque e relé da embreagem
- Bloqueia a partida se o veículo estiver engatado
- Recuperação automática por Watchdog
- Feedback sonoro configurável
- Lógica à prova de falhas com janelas de tempo
- Baixo consumo (clock reduzido)

---

##  Recursos técnicos

- Código em **C/C++ puro para AVR**
- Watchdog ativado por **fuse**
- Brown-Out Detection (BOD)
- Temporização sem `delay()` usando `millis()`
- Lógica orientada a eventos
- Persistência em EEPROM
- Detecção de bordas de sinal
- Proteções de hardware e software

---

##  Hardware utilizado

- Microcontrolador ATmega  
- Sinais do controle original Hyundai  
- Relés para: ignição, arranque e embreagem  
- Sensores: neutro, contato, trava/destrava, seta  
- Saída para buzzer  
- Watchdog ativado por fuse  

---

## 📅 Histórico do projeto

**Autor:** Antonio Valmir da Silva Moraes  
**Início:** 18/12/2025  
**Status:** Finalizado e em uso real  

Este projeto foi criado para resolver um problema real e evoluiu por meio de
testes em bancada e no veículo.

---

## Vídeos de Demonstração

- Sistema em funcionamento real: **(https://youtube.com/shorts/JTFrbbys-P8)**
                                 **(https://youtube.com/shorts/TTD2mf0zWlY)**
                                 **(https://youtube.com/shorts/fy3Lmz_-D58)**
---

## ⚠️ Aviso importante

Este sistema interage com a eletrônica automotiva.
Utilize apenas para estudo ou com validação completa de segurança.

