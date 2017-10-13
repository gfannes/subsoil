#include "gubg/svg/Turtle.hpp"
#include <ostream>

namespace subsoil { namespace toolbox { 

    struct Params
    {
        double width = 0;

        void stream(std::ostream &os) const
        {
            os << C(width) << std::endl;
        }
    };
    inline std::ostream &operator<<(std::ostream &os, const Params &params)
    {
        params.stream(os);
        return os;
    }

    class Toolbox: Params
    {
    public:

        template <typename Ftor>
        Toolbox(Ftor ftor) { ftor(static_cast<Params&>(*this)); }

        void stream(std::ostream &os) const
        {
            os << static_cast<const Params&>(*this);
        }

    private:
    };
    inline std::ostream &operator<<(std::ostream &os, const Toolbox &tb)
    {
        tb.stream(os);
        return os;
    }

} }
