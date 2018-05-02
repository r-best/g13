#include <unistd.h>

namespace G13{

const size_t LOGITECH_VENDOR_ID = 0x046d;
const size_t G13_PRODUCT_ID = 0xc21c;

class G13_Device{
    public:
        G13_Device();
        ~G13_Device();
};
}