#ifndef HEADER_laurot_Id_hpp_ALREADY_INCLUDED
#define HEADER_laurot_Id_hpp_ALREADY_INCLUDED

namespace laurot { 

    namespace id { 
        enum Type
        {
            Question   = 0,
            Answer     = 1,
            Understood = 2,

            Up         = 3,
            Down       = 4,

            Pulse      = 5,
            SetState   = 6,
            HasState   = 7,

            Id         = 8,
            To         = 9,
            When       = 10,
            Duration   = 11,
            State      = 12,
        };
    } 

} 

#endif
