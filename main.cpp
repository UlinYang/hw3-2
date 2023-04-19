#include "mbed.h"

// main() runs in its own thread in the OS
//I2C m_i2c(D14, D15);
I2CSlave i2c_slave(D14, D15);
I2C i2c(A4, A5);
Thread thread_master;
Thread thread_slave;
#define SLAVE_ADDR 0xA0

void slave()
{
  int mode;
  int value;
  char msg[2];
  char buf[2];
  i2c_slave.address(0xA0);
  while (1) {
    int receive_code = i2c_slave.receive();
    switch (receive_code) {
    case I2CSlave::ReadAddressed:
      if (mode==1) { // mode!=0
        value = value + 1;
        msg[0] = mode;
        msg[1] = value;
        printf("Slave Write: mode=%d, value=%d\n", mode, value);
        i2c_slave.write(msg, 2);
      } else if (mode==2) {
        value = value + 2;
        msg[0] = mode;
        msg[1] = value;
        printf("Slave Write: mode=%d, value=%d\n", mode, value);
        i2c_slave.write(msg, 2);
      } else {
        printf("tt\n");
        msg[0] = mode;
        msg[1] = value;
        printf("Slave Write: mode=%d, value=%d\n", mode, value);
        i2c_slave.write(msg, 2);
      }
      break;
    case I2CSlave::WriteGeneral:
      i2c_slave.read(buf, 2);
      mode = buf[0];
      value = buf[1];
      printf("Slave Read General: mode=%d, value=%d\n", mode, value);
      break;
    case I2CSlave::WriteAddressed:
      i2c_slave.read(buf, 2);
      mode = buf[0];
      value = buf[1];
      printf("Slave Read Addressed: mode=%d, value=%d\n", mode, value);
      break;
    }
    
  }
  
}
void i2C_master(){
    printf("test1\n");
    char m_addr = 0x40;
    char cmd[2];
    char res[2];
    cmd[0] = 1;
    cmd[1] = 8;
    printf("write to slave\n");
    //i2c.write(SLAVE_ADDR, cmd, 2);
    ThisThread::sleep_for(1s);
    i2c.write(SLAVE_ADDR, cmd, 2);
    ThisThread::sleep_for(1s);
    i2c.read(SLAVE_ADDR, res, 2);
    ThisThread::sleep_for(1s);
    printf("read from slave\n");
    printf("master read result: mode=%d, value=%d\n", res[0],res[1]);

    printf("test2\n");
    cmd[0] = 2;
    cmd[1] = 30;
    printf("write to slave\n");
    //i2c.write(SLAVE_ADDR, cmd, 2);
    ThisThread::sleep_for(1s);
    i2c.write(SLAVE_ADDR, cmd, 2);
    ThisThread::sleep_for(1s);
    i2c.read(SLAVE_ADDR, res, 2);
    ThisThread::sleep_for(1s);
    printf("read from slave\n");
    printf("master read result: mode=%d, value=%d\n", res[0],res[1]);

}
int main() {
  thread_master.start(i2C_master);
  thread_slave.start(slave);
  
}