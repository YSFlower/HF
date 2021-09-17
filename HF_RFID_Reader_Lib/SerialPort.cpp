#include "SerialPort.h"

#include <stdio.h>
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions          */
#include <errno.h>   /* ERROR Number Definitions           */
#include <string.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <fcntl.h>


SerialPort::SerialPort()
{

}

ReturnErrorCode SerialPort::SerialOpen(const char* usbPath, int& fd)
{
    if ((fd = open(usbPath, O_RDWR | O_NDELAY | O_NOCTTY)) < 0)
    {
        return ReturnErrorCode::OPEN_ERROR;
    }
    return ReturnErrorCode::ERROR_CODE_SUCCESS;
}

ReturnErrorCode SerialPort::SerialConfig(const int& fd, const uint8_t nSpeed, const int nBits, const char nEvent, const int nStop)
{
    struct termios old_ios;
    if (tcgetattr(fd, &old_ios) != 0)
        return ReturnErrorCode::GET_TERMINAL_PARAMETER_ERROR;

    struct termios new_ios;
    memset(&new_ios, 0, sizeof(struct termios));
    if (fcntl(fd, F_SETFL, 0) < 0)
        return ReturnErrorCode::CONTROL_FILE_ERROR;

    switch (nSpeed)
    {
//    case 1200:
//        cfsetispeed(&new_ios , B1200);
//        cfsetospeed(&new_ios , B1200);
//        break;
//    case 1800:
//        cfsetispeed(&new_ios , B1800);
//        cfsetospeed(&new_ios , B1800);
//        break;
//    case 2400:
//        cfsetispeed(&new_ios , B2400);
//        cfsetospeed(&new_ios , B2400);
//        break;
//    case 4800:
//        cfsetispeed(&new_ios , B4800);
//        cfsetospeed(&new_ios , B4800);
//        break;
//    case 9600:
//        cfsetispeed(&new_ios , B9600);
//        cfsetospeed(&new_ios , B9600);
//        break;
    case 0x00:
        cfsetispeed(&new_ios , B19200);
        cfsetospeed(&new_ios , B19200);
        break;
    case 0x01:
        cfsetispeed(&new_ios , B38400);
        cfsetospeed(&new_ios , B38400);
        break;
    case 0x02:
        cfsetispeed(&new_ios , B57600);
        cfsetospeed(&new_ios , B57600);
        break;
    case 0x03:
        cfsetispeed(&new_ios , B115200);
        cfsetospeed(&new_ios , B115200);
        break;
//    case 230400:
//        cfsetispeed(&new_ios , B230400);
//        cfsetospeed(&new_ios , B230400);
//        break;
//    case 460800:
//        cfsetispeed(&new_ios , B460800);
//        cfsetospeed(&new_ios , B460800);
//        break;
    default:
        return ReturnErrorCode::CHOOSE_BAUD_RATE_ERROR;
    }

    /* Clear data width mask bit */
    new_ios.c_cflag &= ~CSIZE;
    switch (nBits)
    {
    case 5:
        new_ios.c_cflag |=CS5 ;
        break ;
    case 6:
        new_ios.c_cflag |=CS6 ;
        break ;
    case 7:
        new_ios.c_cflag |=CS7 ;
        break ;
    case 8:
        new_ios.c_cflag |=CS8 ;
        break ;
    default:
        return ReturnErrorCode::CHOOSE_MASK_BIT_ERROR;
    }

    switch (nEvent)
    {
    case 'o':
    case 'O':
        new_ios.c_cflag |= PARENB ;
        new_ios.c_cflag |= PARODD ;
        new_ios.c_iflag |= (INPCK | ISTRIP);
        break ;
    case 'e':
    case 'E':
        new_ios.c_iflag |= (INPCK | ISTRIP);
        new_ios.c_cflag |= PARENB ;
        new_ios.c_cflag &= ~PARODD ;
        break ;
    case 'n':
    case 'N':
        new_ios.c_cflag &= ~PARENB ;
        new_ios.c_iflag &= ~INPCK  ;
        break ;
    default:
        return ReturnErrorCode::CHOOSE_PARITY_ERROR;
    }

    switch (nStop)
    {
    case 1:
        new_ios.c_cflag &= ~CSTOPB;
        break;
    case 2:
        new_ios.c_cflag |= CSTOPB;
        break;
    default:
        return ReturnErrorCode::CHOOSE_STOP_BIT_ERROR;
    }

    /* Enable the receiver and set local mode */
    new_ios.c_cflag |= CLOCAL | CREAD;

    /* No hardware control */
    new_ios.c_cflag &= ~CRTSCTS;

    /* No software control */
    new_ios.c_iflag &= ~(IXON | IXOFF | IXANY);

    /* raw model */
    new_ios.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG | IEXTEN);

    /* no output process */
    new_ios.c_oflag  &= ~OPOST;

    new_ios.c_iflag &= ~(INLCR | IGNCR | ICRNL);
    new_ios.c_iflag &= ~(BRKINT | INPCK | ISTRIP | IXON | ICRNL);
    new_ios.c_oflag &= ~(ONLCR | OCRNL);

    /* min character */
    new_ios.c_cc[VMIN] = 0;

    /* time to wait for data */
    new_ios.c_cc[VTIME] = 0;    // chechunli change

    if (SerialFlush(fd) != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return ReturnErrorCode::FLUSH_ERROR;

    if (tcsetattr(fd,TCSANOW,&new_ios) != 0)
    {
        /* restore the old configuration */
        if (tcsetattr(fd,TCSANOW,&old_ios) != 0)
            return ReturnErrorCode::SET_TERMINAL_PARAMETER_ERROR;
    }
    return ReturnErrorCode::ERROR_CODE_SUCCESS;
}

ReturnErrorCode SerialPort::SerialClose(int& fd)
{
    if (fd != -1)
    {
        if (close(fd) != 0)
            return ReturnErrorCode::CLOSE_ERROR;
    }
    return ReturnErrorCode::ERROR_CODE_SUCCESS;
}

ReturnErrorCode SerialPort::SerialWrite(const int& fd, unsigned char* writeBuffer, size_t writeLength, int& actuallyWriteLength)
{
    actuallyWriteLength = 0;
    int ret = write(fd, writeBuffer, writeLength);
    if (ret < 0)
        return ReturnErrorCode::WRITE_ERROR;

    actuallyWriteLength = ret;
    return ReturnErrorCode::ERROR_CODE_SUCCESS;
}

ReturnErrorCode SerialPort::SerialRead(const int& fd, unsigned char* readBuffer, int readLength, int& actuallyReadLength)
{
    actuallyReadLength = 0;
    int len = 0;
    fd_set fs_read;
    //int timeouts = 50;        // timeouts must bigger than 180ms around. for the reason security module receiving longer than 150ms
    struct timeval t;

    t.tv_sec = 0;
    t.tv_usec = 80000;

    //int data_len = datalen;
    int remain = readLength;
    int pos = 0;

    //PRINTF("request num = %d\n", data_len);
    while (remain > 0)
    {
        FD_ZERO(&fs_read);
        FD_SET(fd, &fs_read);

        //PRINTF("remain = %d\n", remain);
        int m = select(fd + 1, &fs_read, NULL, NULL, &t);
        if (m == 0)			// timeout
        {
            actuallyReadLength += readLength - remain;
            return ReturnErrorCode::ERROR_CODE_SUCCESS;
        }
        else if ((m < 0) && (errno == EINTR))
        {
            continue;
        }
        else if (m < 0)
        {
            return ReturnErrorCode::FILE_DESCRIPTOR_ERROR;
        }

        if (FD_ISSET(fd, &fs_read))
        {
            len = read(fd, readBuffer + pos, remain);
            if (len < 0)
            {
                if (errno == EINTR)
                    len = 0;
                else
                    return ReturnErrorCode::READ_ERROR;
            }
            else if (len == 0)
                return ReturnErrorCode::READ_ERROR;
            remain -= len;
            pos += len;
        }
    }
    actuallyReadLength += readLength - remain;
    return ReturnErrorCode::ERROR_CODE_SUCCESS;
}

ReturnErrorCode SerialPort::SerialFlush(const int& fd)
{
    if (tcflush(fd, TCIOFLUSH) != 0)
        return ReturnErrorCode::FLUSH_ERROR;
    return ReturnErrorCode::ERROR_CODE_SUCCESS;
}
