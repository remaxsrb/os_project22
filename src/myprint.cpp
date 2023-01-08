//
// Created by remax on 1/8/23.
//
#include "../h/myprint.hpp"

void myKernelPrintString(char const *string)
{
    uint64 sstatus = Riscv::r_sstatus();
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    while (*string != '\0')
    {
        putc(*string);
        string++;
    }

    Riscv::ms_sstatus(sstatus & Riscv::SSTATUS_SIE ? Riscv::SSTATUS_SIE : 0);
}

void myUserPrintString(char const *string) {
    while (*string != '\0')
    {
        putc(*string);
        string++;
    }

}

void myKernelPrintInteger(uint64 integer)
{
    uint64 sstatus = Riscv::r_sstatus();
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);

    static char digits[] = "0123456789";
    char buffer[16];
    int i, negative;
    uint x;

    negative = 0;
    if (integer < 0)
    {
        negative = 1;
        x = -integer;
    } else
    {
        x = integer;
    }

    i = 0;
    do
    {
        buffer[i++] = digits[x % 10];
    } while ((x /= 10) != 0);
    if (negative)
        buffer[i++] = '-';

    while (--i >= 0) { putc(buffer[i]); }

    Riscv::ms_sstatus(sstatus & Riscv::SSTATUS_SIE ? Riscv::SSTATUS_SIE : 0);
}

void myUserPrintInteger(uint64 integer)
{
    static char digits[] = "0123456789";
    char buffer[16];
    int i, negative;
    uint x;

    negative = 0;
    if (integer < 0)
    {
        negative = 1;
        x = -integer;
    } else
    {
        x = integer;
    }

    i = 0;
    do
    {
        buffer[i++] = digits[x % 10];
    } while ((x /= 10) != 0);
    if (negative)
        buffer[i++] = '-';

    while (--i >= 0) { putc(buffer[i]); }
}

