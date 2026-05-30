#include <iostream>
#include <cstddef>

constexpr size_t MAX_MSG_LEN = 32;

class IProtocolParser
{
public:
    virtual ~IProtocolParser() = default;
    virtual void parseBits(const int* bits, size_t size) = 0;
    virtual void printMessage() const = 0;
};

// UART Parser

class UartParser : public IProtocolParser
{
private:
    char decodedMessage[MAX_MSG_LEN];

public:
    void parseBits(const int* bits, size_t size) override
    {
    
        decodedMessage[0] = '\0';
        size_t msgIdx = 0;

        if (size % 10 != 0 || bits == nullptr)
            return;

        for (size_t i = 0; i < size; i += 10)
        {
            if (bits[i] != 0 || bits[i + 9] != 1)
                continue;

            char character = 0;
            for (int bitPos = 0; bitPos < 8; bitPos++)
            {
                if (bits[i + 1 + bitPos] == 1)
                {
                    character |= (1 << bitPos);
                }
            }

            if (msgIdx < MAX_MSG_LEN - 1)
            {
                decodedMessage[msgIdx++] = character;
                decodedMessage[msgIdx] = '\0';
            }
        }
    }

    void printMessage() const override
    {
        std::cout << "[UART Analyzer] : " << decodedMessage << std::endl;
    }
};

// SPI Parser

class SpiParser : public IProtocolParser
{
private:
    char decodedMessage[MAX_MSG_LEN];

public:
    void parseBits(const int* stream, size_t size) override
    {
        decodedMessage[0] = '\0';
        size_t msgIdx = 0;

        if (stream == nullptr) return;

        char character = 0;
        int bitCounter = 0;

        for (size_t i = 0; i < size; i += 2)
        {
            int clockPin = stream[i];
            int dataPin = stream[i + 1];

            if (clockPin == 1)
            {
                if (dataPin == 1)
                {
                    character |= (1 << (7 - bitCounter));
                }
                bitCounter++;

                if (bitCounter == 8)
                {
                    if (msgIdx < MAX_MSG_LEN - 1)
                    {
                        decodedMessage[msgIdx++] = character;
                        decodedMessage[msgIdx] = '\0';
                    }
                    character = 0;
                    bitCounter = 0;
                }
            }
        }
    }

    void printMessage() const override
    {
        std::cout << "[SPI Analyzer] : " << decodedMessage << std::endl;
    }
};

// I2C Parser

class I2cParser : public IProtocolParser
{
private:
    int targetAdress = 0x50;
    char decodedMessage[MAX_MSG_LEN];

public:
    void parseBits(const int* bits, size_t size) override
    {
        decodedMessage[0] = '\0';
        size_t msgIdx = 0;

        if (bits == nullptr || size < 16)
            return;

        int incomingAdress = 0;
        for (int i = 0; i < 7; i++)
        {
            if (bits[i] == 1)
            {
                incomingAdress |= (1 << (6 - i));
            }
        }

        if (incomingAdress != targetAdress)
        {
            std::cout << "[I2C Error] Adress mismatch! Target : 0x" << std::hex << targetAdress << std::endl;
            return;
        }

        for (size_t i = 8; i + 8 <= size; i += 8)
        {
            char character = 0;
            for (size_t b = 0; b < 8; b++)
            {
                if (bits[i + b] == 1)
                {
                    character |= (1 << (7 - b));
                }
            }

            if (msgIdx < MAX_MSG_LEN - 1)
            {
                decodedMessage[msgIdx++] = character;
                decodedMessage[msgIdx] = '\0';
            }
        }
    }

    void printMessage() const override
    {
        std::cout << "[I2C Analyzer] Decoded (Addr 0x" << std::hex << targetAdress << "): " << decodedMessage << std::endl;
    }
};

int main()
{
    const int uartSignal[] = {
        0, 1,1,1,0,1,0,1,0, 1,
        0, 1,0,0,0,0,1,1,0, 1,
        0, 1,1,0,1,0,1,1,0, 1,
        0, 1,0,1,0,0,1,1,0, 1
    };
    constexpr size_t uartSize = sizeof(uartSignal) / sizeof(uartSignal[0]);

    const int spiSignal[] = {
        1,0, 1,1, 1,0, 1,1, 1,0, 1,1, 1,0, 1,1,
        1,0, 1,1, 1,1, 1,1, 1,0, 1,0, 1,0, 1,0
    };
    constexpr size_t spiSize = sizeof(spiSignal) / sizeof(spiSignal[0]);

    const int i2cSignal[] = {
        1,0,1,0,0,0,0,  0,
        0,1,0,0,1,1,1,0,
        0,1,1,0,0,1,0,1,
        0,1,1,0,1,1,1,1
    };
    constexpr size_t i2cSize = sizeof(i2cSignal) / sizeof(i2cSignal[0]);

    UartParser uartParser;
    SpiParser spiParser;
    I2cParser i2cParser;

    IProtocolParser* analyzer = nullptr;

    analyzer = &uartParser;
    analyzer->parseBits(uartSignal, uartSize);
    analyzer->printMessage();

    analyzer = &spiParser;
    analyzer->parseBits(spiSignal, spiSize);
    analyzer->printMessage();

    analyzer = &i2cParser;
    analyzer->parseBits(i2cSignal, i2cSize);
    analyzer->printMessage();

    return 0;
}
