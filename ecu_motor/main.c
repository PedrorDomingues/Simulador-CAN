#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/time.h>

int main() {
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame tx_frame, rx_frame;

    setbuf(stdout, NULL); //Para que o print funcione corretamente nos containers

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    strcpy(ifr.ifr_name, "vcan0");
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    struct can_filter rfilter[1];  // Filtro para a ECU não ler todas a mensagens, apenas aquelas que lhe interessam
    rfilter[0].can_id = 0x200; // ID da mensagem que interessa para a ECU
    rfilter[0].can_mask = CAN_SFF_MASK;
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);


    memset(&tx_frame, 0, sizeof(struct can_frame));


    tx_frame.can_id = 0x100; //ID da mensagem
    tx_frame.can_dlc = 2; // Quantidade de bytes da mensagem, valor maximo é 8
    tx_frame.data[0] = 0x2A; // Dado do primeiro byte
    tx_frame.data[1] = 0x3B; // Dado do segundo byte

    printf("ECU Motor [TX: 0x100 | RX: 0x200]\n");

    while (1) {
        write(s, &tx_frame, sizeof(struct can_frame));
        printf("[Motor] Enviei RPM (0x100)\n");

        int nbytes = read(s, &rx_frame, sizeof(struct can_frame));
        if (nbytes > 0) {
            printf("  -> [Motor] Recebi do Painel: %02X %02X\n", rx_frame.data[0], rx_frame.data[1]);
        }
        

        sleep(1);
    }

    close(s);
    return 0;
}
