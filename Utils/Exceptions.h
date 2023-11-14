#define CHECK(condition) \
    if (!(condition)) { \
        throw std::runtime_error(message); \
    }

#define ENSURE(condition) \
    if (!(condition)) { \
        __debugbreak(); \
    }
