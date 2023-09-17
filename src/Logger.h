#ifndef LOGGER_H
#define LOGGER_H
#include <common.h>

class Logger
{
    static std::shared_ptr<Logger> log;
public:
    Logger() = default;
    // отпущена имплементация с удаленными конструкторами, операторами присваивания
    static Logger* getInstance() {
        if (log == nullptr) {
            log = std::make_shared<Logger>();
        }
        return log.get();
    }

    void endl() {
        std::cout << std::endl;
    }

    template<typename ...Args>
    inline Logger& out(const char* fmt, Args ...args) {
        logger(fmt, args...);
        return *this;
    }

    template<typename ...Args>
    inline Logger& wrn(const char* fmt, Args ...args) {
        logger(fmt, args...);
        return *this;
    }

    template<typename ...Args>
    inline Logger& err(const char* fmt, Args ...args) {
        logger(fmt, args...);
        return *this;
    }

    void where(const char* filename = __builtin_FILE(), const char* fnc = __builtin_FUNCTION(), int line = __builtin_LINE()) {
        std::cout<<"=========== TRACE =============\n";
        std::cout<<"FILE: "<<filename<<"\n";
        std::cout<<"FUNCTION: "<<fnc<<"\n";
        std::cout<<"LINE: "<<line<<"\n";
        std::cout<<"===============================\n";
    }
private:
    //=> C++ STYLE PRINTF :)
    void output(const char* fmt) {
        std::cout << fmt << std::endl;
    }
    /* parsed arguments */
    template<typename T, typename ...Args>
    void output(const char* fmt, T t, Args ...args) {
        for (; *fmt != '\0'; fmt++) {
            if (*fmt == '%') {
                std::cout << t;
                return output(fmt+1,args...);
            }
            std::cout << (char)(*fmt);
        }
    }
    template<typename ...Args>
    void logger(const char* fmt, Args ...args) {
        if (fmt == nullptr) {
            return;
        }
        output(fmt, args...);
        std::cout << std::endl;
    }
};


#endif // LOGGER_H
