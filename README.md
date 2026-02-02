# Remote Start – Hyundai HB20 (2016)

## 🔥 Real project | Embedded System | Automotive

This project is a fully functional **remote engine start system** for a Hyundai HB20 (2016),
built without commercial modules and using the **original car remote key**.

It has been **tested in real vehicle**, in daily use, with full safety logic and watchdog protection.

---

## 🚗 What this system does

- Start the engine remotely using the original key
- Keeps original car behavior (BCM not affected)
- Neutral and safety checks
- Controls ignition, starter motor and clutch relay
- Blocks engine start if vehicle is in gear
- Automatic recovery using hardware Watchdog
- Configurable buzzer feedback
- Failsafe logic with time windows
- Power optimized (runs at low clock)

---

## 🧠 Technical features

- Written in **pure C/C++ for AVR**
- Watchdog enabled by **fuse**
- Brown-out detection (BOD)
- Non-blocking timing with `millis()`
- Event driven logic
- EEPROM persistence
- Signal edge detection
- Hardware protection logic

---

## 🧩 Hardware used

- ATmega microcontroller  
- Original Hyundai remote key signals  
- Relays for: ignition, starter, clutch  
- Sensors: neutral, contact, lock/unlock, turn signal  
- Buzzer output  
- Watchdog enabled by fuse  

---

## 📅 Project history

**Author:** Antonio Valmir da Silva Moraes  
**Started:** 18/12/2025  
**Status:** Finalized and in real use  

This project was built to solve real problems and evolved through
bench tests and real vehicle integration.

---

## ⚠️ Important

This system interacts with automotive electronics.
Use only for study or with full safety validation. 
----------------------------------------------------------
Em portugues:

# Partida Remota – Hyundai HB20 (2016)

## 🔥 Projeto real | Sistema Embarcado | Automotivo

Este projeto é um sistema de **partida remota do motor** para um Hyundai HB20 (2016),
desenvolvido sem módulos comerciais prontos e utilizando o **controle original do veículo**.

Foi **testado em veículo real**, em uso diário, com lógica completa de segurança
e proteção por Watchdog.

---

## 🚗 O que este sistema faz

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

## 🧠 Recursos técnicos

- Código em **C/C++ puro para AVR**
- Watchdog ativado por **fuse**
- Brown-Out Detection (BOD)
- Temporização sem `delay()` usando `millis()`
- Lógica orientada a eventos
- Persistência em EEPROM
- Detecção de bordas de sinal
- Proteções de hardware e software

---

## 🧩 Hardware utilizado

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

## ⚠️ Aviso importante

Este sistema interage com a eletrônica automotiva.
Utilize apenas para estudo ou com validação completa de segurança.

