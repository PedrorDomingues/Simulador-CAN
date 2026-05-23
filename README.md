# Guia Completo: Simulação de Barramento CAN com Docker e SocketCAN

Este repositório contém a infraestrutura e os códigos-fonte para a simulação de uma rede CAN (Controller Area Network) trocando mensagens entre múltiplas ECUs virtuais utilizando `vcan` no Linux e containers Docker.

---

## 1. Preparação do Sistema (Host)

Para rodar a simulação, é necessário instalar as dependências e criar o barramento virtual no kernel do Linux. Execute os comandos abaixo no seu terminal:

```bash
# 1. Instala o compilador, utilitários CAN e o Docker
sudo apt install build-essential can-utils docker.io docker-compose-v2 -y

# 2. Carrega o módulo de CAN virtual no kernel
sudo modprobe vcan

# 3. Cria a interface de rede virtual chamada vcan0
sudo ip link add dev vcan0 type vcan

# 4. Ativa a interface (liga o barramento)
sudo ip link set up vcan0

# 5. Confirma se a interface foi ativada corretamente
ip link show vcan0
```
## 2. Estrutura do Projeto

📂 raiz-do-projeto/
├── 📄 compose.yml           # Arquivo de orquestração do Docker
├── 📂 ecu_motor/            # Pasta da 1ª ECU
│   ├── 📄 Dockerfile        # Instruções de compilação da imagem
│   └── 📄 main.c            # Código-fonte em C
└── 📂 ecu_painel/           # Pasta da 2ª ECU (e assim por diante)
    ├── 📄 Dockerfile
    └── 📄 main.c

## 3. Monitoramente do Barramento CAN

Para visualizar as mensagens trafegando na rede em tempo real abra um terminal dedicado e execute o comando abaixo antes de iniciar as ECUs:
```bash
candump vcan0
```
## 4. Rodando os Containers

Com o barramento criado e o candump escutando, abra um novo terminal na raiz do projeto (onde está o arquivo compose.yml) e suba a simulação:

```bash
sudo docker compose up --build
```