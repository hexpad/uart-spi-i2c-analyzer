#include <iostream>
#include <vector>
#include <string>

class IProtocolParser
{
public:
    virtual ~IProtocolParser() = default;
    virtual void parseBits(const std::vector<int>& bits) = 0;
    virtual void printMessage() const = 0;
};

// UART Parser

class UartParser : public IProtocolParser
{
private:
    std::string decodedMessage;

public:
    void parseBits(const std::vector<int>& bits) override
    {
        decodedMessage = "";

        if(bits.size() % 10 != 0)
            return;

        for(int i = 0; i < bits.size(); i += 10)
        {
            if(bits[i] != 0 || bits[i + 9] != 1)
                continue;

            char character = 0;

            for(int bitPos = 0; bitPos < 8; bitPos++)
            {
                if(bits[i + 1 + bitPos] == 1)
                {
                    character |= (1 << bitPos);
                }
            }

            decodedMessage += character;
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
    std::string decodedMessage;

public:
    void parseBits(const std::vector<int>& stream) override
    {
        decodedMessage = "";
        char character = 0;
        int bitCounter = 0;

        for(int i = 0; i < stream.size(); i += 2)
        {
            int clockPin = stream[i];
            int dataPin = stream[i + 1];

            if(clockPin == 1)
            {
                if(dataPin == 1)
                {
                    character |= (1 << (7 - bitCounter));
                }
                bitCounter++;

                if(bitCounter == 8)
                {
                    decodedMessage += character;
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
    std::string decodedMessage;

public:
    void parseBits(const std::vector<int>& bits) override
    {
        decodedMessage = "";

        if(bits.size() < 16)
            return;

        int incomingAdress = 0;
        for(int i = 0; i < 7; i++)
        {
            if(bits[i] == 1)
            {
                incomingAdress |= (1 << (6 - i));
            }
        }

        if(incomingAdress != targetAdress)
        {
            std::cout << "[I2C Error] Adress mismatch! Target : 0x" << std::hex << targetAdress << std::endl;
            return;
        }

        for (int i = 8; i + 8 <= bits.size(); i += 8)
        {
            char character = 0;
            for (size_t b = 0; b < 8; b++)
            {
                if (bits[i + b] == 1)
                {
                    character |= (1 << (7 - b));
                }
            }
            decodedMessage += character;
        }
    }

    void printMessage() const override
    {
        std::cout << "[I2C Analyzer] Decoded (Addr 0x" << std::hex << targetAdress << "): " << decodedMessage << std::endl;
    }
};


int main()
{
    std::vector<int> uartSignal
    {
        0, 1,1,1,0,1,0,1,0, 1,
        0, 1,0,0,0,0,1,1,0, 1,
        0, 1,1,0,1,0,1,1,0, 1,
        0, 1,0,1,0,0,1,1,0, 1
    };

    std::vector<int> spiSignal
    {
        1,0, 1,1, 1,0, 1,1, 1,0, 1,1, 1,0, 1,1,

        1,0, 1,1, 1,1, 1,1, 1,0, 1,0, 1,0, 1,0
    };

    std::vector<int> i2cSignal
    {
        1,0,1,0,0,0,0,  0,
        0,1,0,0,1,1,1,0,
        0,1,1,0,0,1,0,1,
        0,1,1,0,1,1,1,1
    };

    std::unique_ptr<IProtocolParser> analyzer = nullptr;

    analyzer = std::make_unique<UartParser>();
    analyzer -> parseBits(uartSignal);
    analyzer -> printMessage();

    analyzer = std::make_unique<SpiParser>();
    analyzer->parseBits(spiSignal);
    analyzer->printMessage();

    analyzer = std::make_unique<I2cParser>();
    analyzer->parseBits(i2cSignal);
    analyzer->printMessage();

    return 0;
}
